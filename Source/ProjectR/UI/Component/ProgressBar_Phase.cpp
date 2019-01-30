// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProgressBar_Phase.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"



UProgressBar_Phase::UProgressBar_Phase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UProgressBar_Phase::RemoveFromParent()
{
	Super::RemoveFromParent();
}

void UProgressBar_Phase::SetTargetActor(AActor* InTargetActor, EProgressTypeEnum InProgressType)
{
	if (!IsValid(InTargetActor))
		return;

	TargetActor = InTargetActor;
	ProgressType = InProgressType;

	UBattleEntityComponent* TargetBattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor.Get());
	if (TargetBattleEntityComp == nullptr)
		return;

	MaxHP = TargetBattleEntityComp->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
	MaxPhase = 4;

	//Set Hp Phase Value
	int32 Level = TargetBattleEntityComp->GetBattleInfo()->GetLevel();
	FString TableKey = FString::FromInt(Level);
	FHPProgressInfo* HPProgressInfo = UTableManager::GetInstancePtr()->GetHPProgressInfoTableRow(FName(*TableKey));
	if (HPProgressInfo)
	{
		HPPhaseValue = HPProgressInfo->ReferenceValue;
	}

	InitPhaseList();
}

void UProgressBar_Phase::SetTargetActorList(TArray<AActor*> InTargetActors, EProgressTypeEnum InProgressType)
{
	if (InTargetActors.Num() == 0)
		return;

	TargetActorsList = InTargetActors;
	ProgressType = InProgressType;

	MaxHP = 0;
	MaxPhase = 4;
	int32 Level = 0;
	for (auto target : TargetActorsList)
	{
		UBattleEntityComponent* TargetBattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(target);
		if (TargetBattleEntityComp != nullptr)
		{ 
			MaxHP += TargetBattleEntityComp->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
			Level = TargetBattleEntityComp->GetBattleInfo()->GetLevel();
		}
	}
	FString TableKey = FString::FromInt(Level);
	FHPProgressInfo* HPProgressInfo = UTableManager::GetInstancePtr()->GetHPProgressInfoTableRow(FName(*TableKey));
	if (HPProgressInfo)
	{
		HPPhaseValue = (HPProgressInfo->ReferenceValue /** InTargetActors.Num()*/);
	}

	InitPhaseList();
}

void UProgressBar_Phase::SetNeedle(USlider* InNeedle)
{
	HPNeedle = InNeedle;
}

void UProgressBar_Phase::InitPhaseList()
{
	if (MaxPhase == 0 || MaxHP == 0)
		return;
	
	PhaseList.Reset();
	
	int32 unitHP = (int32)(MaxHP / HPPhaseValue);
	int32 remainHP = MaxHP % HPPhaseValue;

	/*if (unitHP == 0)
	{
		
	}*/
	PhaseList.Reserve(unitHP + 1);
	for (int32 i = 0; i < unitHP; ++i)
	{
		PhaseList.Emplace(HPPhaseValue);
	}
	
	if (remainHP > 0)
	{
		PhaseList.Emplace(remainHP);
	}

	//CurrentPhase = -1;
	CurrentPhase = unitHP;
	CurrentHP = MaxHP;

	CurrentPercent = 1.0f;
	PrevPercent = 1.0f;
	//ChangeProgressBarTint();

	//todo SetColor
	
	switch (ProgressType)
	{
		case EProgressTypeEnum::VE_Layer1:
			ChangeProgressBarTintLayer1();
		break;
		case EProgressTypeEnum::VE_Layer2:
			ChangeProgressBarTintLayer2();
			SynchronizeProperties();
		break;
		default:
			ChangeProgressBarTint();
		break;
	}
}

void UProgressBar_Phase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	//PercentDelegate.Unbind();
	if(PercentDelegate.IsBound() == false)
	{
		PercentDelegate.BindDynamic(this, &UProgressBar_Phase::GetPhasePercent);
	}
}

void UProgressBar_Phase::ReleaseSlateResources(bool bReleaseChildren)
{
	if (PercentDelegate.IsBound())
	{
		PercentDelegate.Unbind();
	}
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UProgressBar_Phase::PostLoad()
{
	Super::PostLoad();
}

int32 UProgressBar_Phase::GetCurrentHP()
{
	return CurrentHP;
}

int32 UProgressBar_Phase::GetSingleTargetCurrentHP()
{
	auto TargetActor_BaseProperty = Cast<IEntityBaseProperty>(TargetActor.Get());
	auto TargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor.Get());
	if (TargetActor_BaseProperty != nullptr && IsValid(TargetActor_BattleEntityComponent))
	{
		if (!TargetActor_BaseProperty->IsReady())
			return CurrentHP;
		if (!IsValid(TargetActor_BattleEntityComponent->GetBattleInfo()))
			return CurrentHP;

		CurrentHP = TargetActor_BattleEntityComponent->GetBattleInfo()->GetHP();
	}
	return CurrentHP;
}

int32 UProgressBar_Phase::GetMultiTargetCurrentHP()
{
	CurrentHP = 0;

	for (AActor* target : TargetActorsList)
	{
		auto TargetActor_BaseProperty = Cast<IEntityBaseProperty>(target);
		auto TargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(target);
		if (TargetActor_BaseProperty != nullptr && IsValid(TargetActor_BattleEntityComponent))
		{
// 			if (!TargetActor_BaseProperty->IsReady())
// 				continue;
			if (!IsValid(TargetActor_BattleEntityComponent->GetBattleInfo()))
				continue;

			CurrentHP += TargetActor_BattleEntityComponent->GetBattleInfo()->GetHP();
		}
	}

	return CurrentHP;
}

float UProgressBar_Phase::GetPhasePercent()
{
	if (!TargetActor.IsValid() && TargetActorsList.Num() == 0)
	{
		TargetActor = nullptr;
		return CurrentPercent;
	}

	int32 checkPhase = 0;
	int32 remainHP = 0; //= CurrentHP;

	if (TargetActor.IsValid())
		remainHP = GetSingleTargetCurrentHP();
	else //if (0 < TargetActorsList.Num())
		remainHP = GetMultiTargetCurrentHP();

	for (int32 phaseHP : PhaseList)
	{
		if (remainHP <= phaseHP)
		{
			if (PrevPercent != CurrentPercent)
			{
				PrevPercent -= HPDecreaseValue;
				if (PrevPercent <= CurrentPercent)
				{
					PrevPercent = CurrentPercent;
					PrevHP = CurrentHP;
				}
			}
			CurrentPercent = (float)remainHP / (float)phaseHP;
			break;
		}

		remainHP -= phaseHP;

		++checkPhase;
	}

	if (CurrentPhase != checkPhase)
	{
		CurrentPhase = checkPhase;
		//SetPercent(CurrentPhase);

		//PercentDelegate.BindDynamic(this, &UProgressBar_Phase::GetPhasePercent);

		switch (ProgressType)
		{
		case EProgressTypeEnum::VE_Layer1:
			ChangeProgressBarTintLayer1();
			break;
		case EProgressTypeEnum::VE_Layer2:
			ChangeProgressBarTintLayer2();
			break;
		default:
			ChangeProgressBarTint();
			break;
		}
	}

	if (IsValid(HPNeedle))
	{
		HPNeedle->SetValue(CurrentPercent);
	}

	if (ProgressType == EProgressTypeEnum::VE_Layer1)
	{
		return PrevPercent;
	}

	return CurrentPercent;
}

void UProgressBar_Phase::ChangeProgressBarTint()
{
	FColor Background = FColor::White;
	FColor FillImage = FColor::White;

	int32 PhaseRate = CurrentPhase % MaxPhase;

	if(PhaseColor.Num() == 0)
	{
		switch (PhaseRate)
		{
		case 0:
			Background = UProgressBar_Phase::Black;
			FillImage = UProgressBar_Phase::Red;
			break;
		case 1:
			Background = UProgressBar_Phase::Red;
			FillImage = UProgressBar_Phase::Orange;
			break;
		case 2:
			Background = UProgressBar_Phase::Orange;
			FillImage = UProgressBar_Phase::Cyan;
			break;
		case 3:
			Background = UProgressBar_Phase::Cyan;
			FillImage = UProgressBar_Phase::Blue;
			break;
		}
	}
	else
	{
		if (PhaseColor.IsValidIndex(PhaseRate) == false)
			return;

		if (CurrentPhase > 0)
		{
			if (PhaseRate == 0)
			{
				Background = PhaseColor[MaxPhase].ToFColor(true);
			}
			else
			{
				Background = PhaseColor[PhaseRate - 1].ToFColor(true);
			}
		}
		else
		{
			Background = UProgressBar_Phase::Black;
		}
		FillImage = PhaseColor[PhaseRate].ToFColor(true);
	}

	if (Background != FColor::White)
	{
		//WidgetStyle.BackgroundImage.Tint_DEPRECATED = FLinearColor(Background);
		//WidgetStyle.BackgroundImage.TintColor = FSlateColor(WidgetStyle.BackgroundImage.Tint_DEPRECATED);
		WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor(Background));
	}
	
	if (FillImage != FColor::White)
	{
		//WidgetStyle.FillImage.Tint_DEPRECATED = FLinearColor(FillImage);
		//WidgetStyle.FillImage.TintColor = FSlateColor(WidgetStyle.FillImage.Tint_DEPRECATED);
		WidgetStyle.FillImage.TintColor = FSlateColor(FLinearColor(FillImage));
	}
}

void UProgressBar_Phase::SetCustomPhaseColor(TArray<FLinearColor> InPhaseColor)
{
	PhaseColor = InPhaseColor;
}

void UProgressBar_Phase::ChangeProgressBarTintLayer1()
{
	FColor Background = FColor::White;
	FColor FillImage = FColor::White;
	int32 PhaseRate = CurrentPhase % MaxPhase - 1;
	if (PhaseRate < 0)
	{
		PhaseRate += MaxPhase - 1;
	}

	if (PhaseColor.IsValidIndex(PhaseRate) == false)
		return;

	if (CurrentPhase > 0)
	{
		Background = PhaseColor[PhaseRate].ToFColor(true);
	}
	else
	{
		Background = UProgressBar_Phase::Black;
	}
	FillImage = PhaseColor[MaxPhase - 1].ToFColor(true);

	if (Background != FColor::White)
	{
		//WidgetStyle.BackgroundImage.Tint_DEPRECATED = FLinearColor(Background);
		if (CurrentPhase > 0)
		{
			//WidgetStyle.BackgroundImage.Tint_DEPRECATED = PhaseColor[PhaseRate];
			WidgetStyle.BackgroundImage.TintColor = FLinearColor(PhaseColor[PhaseRate]);
		}
		else
		{
			//WidgetStyle.BackgroundImage.Tint_DEPRECATED = FLinearColor(UProgressBar_Phase::Black);
			WidgetStyle.BackgroundImage.TintColor = FLinearColor(UProgressBar_Phase::Black);
		}
		//WidgetStyle.BackgroundImage.TintColor = FSlateColor(WidgetStyle.BackgroundImage.Tint_DEPRECATED);
	}
	
	if (FillImage != FColor::White)
	{
		//WidgetStyle.FillImage.Tint_DEPRECATED = FLinearColor(FillImage);
		//WidgetStyle.FillImage.Tint_DEPRECATED = FLinearColor(PhaseColor[MaxPhase]);
		//WidgetStyle.FillImage.TintColor = FSlateColor(WidgetStyle.FillImage.Tint_DEPRECATED);
		WidgetStyle.FillImage.TintColor = FSlateColor(FLinearColor(FillImage));
	}
}

void UProgressBar_Phase::ChangeProgressBarTintLayer2()
{
	//FColor Background = FColor::White;
	FColor FillImage = FColor::White;

	int32 PhaseRate = CurrentPhase % MaxPhase;
	if (PhaseColor.IsValidIndex(PhaseRate) == false)
		return;

	FillImage = PhaseColor[PhaseRate].ToFColor(true);

	
	//WidgetStyle.BackgroundImage.Tint_DEPRECATED = FLinearColor(Background);
	//WidgetStyle.BackgroundImage.TintColor = FSlateColor(WidgetStyle.BackgroundImage.Tint_DEPRECATED);

	if (FillImage != FColor::White)
	{
		//WidgetStyle.FillImage.Tint_DEPRECATED = FLinearColor(FillImage);
		//WidgetStyle.FillImage.Tint_DEPRECATED = FLinearColor(PhaseColor[PhaseRate]);
		//WidgetStyle.FillImage.TintColor = FSlateColor(WidgetStyle.FillImage.);
		WidgetStyle.FillImage.TintColor = FSlateColor(FLinearColor(FillImage));
	}
	else
	{
		WidgetStyle.FillImage.TintColor = WidgetStyle.BackgroundImage.TintColor;
	}
}

int32 UProgressBar_Phase::GetCurrentPhase()
{
	return CurrentPhase;
}