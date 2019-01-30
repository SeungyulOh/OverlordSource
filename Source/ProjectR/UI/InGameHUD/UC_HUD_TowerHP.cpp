// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_TowerHP.h"
#include "Global/RGameInstance.h"


#include "UtilFunctionIntegrated.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "SharedConstants/GlobalConstants.h"

#include "Global/TableManager.h"
#include "Global/EventManager.h"

#define DANGEROUS_HP_LIMIT 20


bool UUC_HUD_TowerHP::bFirst = true;
void UUC_HUD_TowerHP::NativeConstruct()
{
	Super::NativeConstruct();

	NormalColor = FLinearColor(0.1684f, 1.f, 0.f, 1.f);
	DangerousColor = FLinearColor(0.965f, 0.f, 0.224068f, 1.f);

	auto RGameInstance = RGAMEINSTANCE(this);
	if (!IsValid(RGameInstance->EventManager))
		return;

	RGameInstance->EventManager->OnSpawnActorFromCharacterSpawner.RemoveDynamic(this, &UUC_HUD_TowerHP::CheckProtectObjectSpawn);
	RGameInstance->EventManager->OnSpawnActorFromCharacterSpawner.AddDynamic(this, &UUC_HUD_TowerHP::CheckProtectObjectSpawn);


	HpBarDamageAni = GetAni("Indicator_Danger");
}

void UUC_HUD_TowerHP::NativeDestruct()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnSpawnActorFromCharacterSpawner.RemoveDynamic(this, &UUC_HUD_TowerHP::CheckProtectObjectSpawn);
	}

	if (IsValid(BattleEntityComp))
	{
		BattleEntityComp->OnReceiveDamageDelegate.RemoveDynamic(this, &UUC_HUD_TowerHP::OnDamageNotifyFromBattleCompnonentNotifyDamageDelegate);
	}

	Super::NativeDestruct();
}

void UUC_HUD_TowerHP::ChangeMode(EPhaseClearEnumType InType)
{
	PhaseType = InType;
	SetVisibility(ESlateVisibility::Visible);

	if (InType == EPhaseClearEnumType::VE_Protect)
	{
		if (ClearConditionWidgetSwitcher)
			ClearConditionWidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else if (InType == EPhaseClearEnumType::VE_Resisting)
	{
		if (ClearConditionWidgetSwitcher)
			ClearConditionWidgetSwitcher->SetActiveWidgetIndex(0);
	}
	else if (InType == EPhaseClearEnumType::VE_ResistingEnable)
	{
		if (ClearConditionWidgetSwitcher)
			ClearConditionWidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UUC_HUD_TowerHP::CheckProtectObjectSpawn(AActor* SpawnActor)
{
	if (IsValid(SpawnActor) == false)
		return;

	if (bFirst == false)
	{
		bFirst = true;
		return;
	}
	
	if (ProtectActor.IsValid())
		return;

 	SetUI_ProtectTower(SpawnActor);
	bFirst = false;

	return;
}

void UUC_HUD_TowerHP::SetUI_ProtectTower(AActor* SpawnActor)
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
	{
		ALevelActor* lvActor = BaseStageInfo->GetLevelActor();
		if (lvActor)
		{
			ALevelActorPhase*	Phase = lvActor->GetLevelActorPhase();
			if (Phase && Phase->ClearCondition == EPhaseClearEnumType::VE_Resisting)
				return;
		}
	}

	float CurrentHPPercent = 0;


	ProtectActor = SpawnActor;
	BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnActor);

	if (IsValid(BattleEntityComp))
	{
		BattleEntityComp->OnReceiveDamageDelegate.RemoveDynamic(this, &UUC_HUD_TowerHP::OnDamageNotifyFromBattleCompnonentNotifyDamageDelegate);
		BattleEntityComp->OnReceiveDamageDelegate.AddDynamic(this, &UUC_HUD_TowerHP::OnDamageNotifyFromBattleCompnonentNotifyDamageDelegate);

		CurrentHPPercent = BattleEntityComp->GetCurrentHPPercent();
	}

	IEntityBaseProperty* Owner_BaseProperty = Cast<IEntityBaseProperty>(ProtectActor.Get());
	if (Owner_BaseProperty)
	{
		if (Image_Target)
		{
			FName CharacterID = Owner_BaseProperty->GetCharacterID();
			auto Category = Owner_BaseProperty->GetCharacterCategory();

			if (Category != ECharacterCategoryTypeEnum::VE_Npc && Category != ECharacterCategoryTypeEnum::VE_BattleObject)
				SetVisibility(ESlateVisibility::Collapsed);
			if (Owner_BaseProperty->GetPartyID() != PartyID_NPC_HELP)
				SetVisibility(ESlateVisibility::Collapsed);

			FName PortraitTextureKey;
			switch (Category)
			{
				case ECharacterCategoryTypeEnum::VE_Hero:
				{
					auto CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CharacterID);
					if (CharacterTableInfo)
					{
						PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
					}
				}
				break;
				case ECharacterCategoryTypeEnum::VE_Npc:
				case ECharacterCategoryTypeEnum::VE_BattleObject:
				{
					auto CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(CharacterID);
					if (CharacterTableInfo)
					{
						PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
					}
				}
				break;
				case ECharacterCategoryTypeEnum::VE_Object:
				{
					auto CharacterTableInfo = UTableManager::GetInstancePtr()->GetBattleObjectRow(CharacterID);
					if (CharacterTableInfo)
					{
						PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
					}
				}
				break;
			}
			
			if (PortraitTextureKey.IsNone() == false)
			{
				UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(PortraitTextureKey), Image_Target);
				Image_Target->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}

	if (ProgressBar_HP)
	{
		ProgressBar_HP->SetFillColorAndOpacity(NormalColor);
		ProgressBar_HP->SetPercent(CurrentHPPercent);
	}

	if (Text_HP)
	{
		Text_HP->SetColorAndOpacity(NormalColor);
		int32 CurrentHPPercent_100Rate = CurrentHPPercent * 100;
		//FString a = FString::FormatAsNumber(CurrentHPPercent_100Rate) + "%";
		Text_HP->SetText(FText::AsNumber(CurrentHPPercent_100Rate));
	}

	if (Unit_Percent)
	{
		Unit_Percent->SetColorAndOpacity(NormalColor);
	}

	//if (Attacked == nullptr)
	//	return;

// 	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUC_HUD_TowerHP::ResetPhaseType()
{
	SetVisibility(ESlateVisibility::Collapsed);
	PhaseType = EPhaseClearEnumType::VE_MAX;
}

void UUC_HUD_TowerHP::OnDamageNotifyFromBattleCompnonentNotifyDamageDelegate( AActor* OwnerActor, float FinalDamage, EBattleDamageResultEnum InDamageResultType, AActor* FromActor)
{
	if (IsValid(FromActor) == false)
		return;

	if (IsValid(OwnerActor) == false)
		return;

	if (OwnerActor != ProtectActor.Get())
		return;

	if (ProgressBar_HP == nullptr)
		return;

	if (Text_HP == nullptr)
		return;

	if (Unit_Percent == nullptr)
		return;

	if (IsValid(BattleEntityComp) == false)
		return;

	float CurrentHPPercent = BattleEntityComp->GetCurrentHPPercent();
	ProgressBar_HP->SetPercent(CurrentHPPercent);

	int32 CurrentHPPercent_100Rate = CurrentHPPercent * 100;
	Text_HP->SetText(FText::AsNumber(CurrentHPPercent_100Rate));

	if (CurrentHPPercent_100Rate <= DANGEROUS_HP_LIMIT)
	{
		if (IsDangerous == false)
		{
			ProgressBar_HP->SetFillColorAndOpacity(DangerousColor);

			Text_HP->SetColorAndOpacity(DangerousColor);
			Unit_Percent->SetColorAndOpacity(DangerousColor);

			IsDangerous = true;
		}
	}
	else
	{
		if (IsDangerous)
		{
			ProgressBar_HP->SetFillColorAndOpacity(NormalColor);

			Text_HP->SetColorAndOpacity(NormalColor);
			Unit_Percent->SetColorAndOpacity(NormalColor);
			IsDangerous = false;
		}
	}

	if (Attacked)
	{
		Attacked->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (HpBarDamageAni)
	{
		PlayAnimation(HpBarDamageAni);
	}
	//PlayHitAnimation();
}