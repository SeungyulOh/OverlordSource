// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"

#include "GameInfo/PlayStateInfo.h"

#include "RSearchTargetIcon.h"

#include "Particles/TypeData/ParticleModuleTypeDataMesh.h"

#include "CUstomActors/RTargettingIndicator.h"

void URSearchTargetIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &URSearchTargetIcon::BtnEventOnClicked);
		Button_Click->OnClicked.AddDynamic(this, &URSearchTargetIcon::BtnEventOnClicked);
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());

}

void URSearchTargetIcon::NativeDestruct()
{
	//Button_Click = nullptr;

	Super::NativeDestruct();
}

void URSearchTargetIcon::BtnEventOnClicked()
{
	if (BasePlayerController)
	{
		//bool bToggle = !(BasePlayerController->GetIsFocusAttack());
		auto Target = ChangeTarget();
		bool bKeepEffect = ((Target != nullptr) ? true : false);
		
		RGAMEINSTANCE(this)->PlayStateInfo->SetbLeaderhasTarget(bKeepEffect);

		AActor* Leader = BasePlayerController->GetSelectedCharacter();
		if (Leader && Target)
		{
			if (TargettingIndicator.IsValid())
			{
				TargettingIndicator->Destroy();
			}


			FTransform transform;
			transform.SetLocation(Leader->GetActorLocation());

			TargettingIndicator = Leader->GetWorld()->SpawnActorDeferred<ARTargettingIndicator>(ARTargettingIndicator::StaticClass(),transform);
			TargettingIndicator->CurrentTarget = Target;
			TargettingIndicator->SourceActor = Leader;
			TargettingIndicator->FinishSpawning(transform);

			RGAMEINSTANCE(GEngine)->GameEnvironmentVar.OutLineElapsedTime = 0.f;
		}
	}
	PlayAni("Click_Pressed", true);
}


void URSearchTargetIcon::SetClickedButton(bool InEnableClick)
{
	IsTouchEnable = InEnableClick;

#ifdef RULE_TWOTOUCHBUTTON
	return;
#endif

	if (InEnableClick)
	{
		Button_Click->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Button_Click->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

AActor* URSearchTargetIcon::ChangeTarget()
{
	TWeakObjectPtr<AActor> LeaderCharacter = BasePlayerController->GetSelectedCharacter();
	if (!LeaderCharacter.IsValid())
		return nullptr;
	
	TargetAlreadySelectedList.Empty();

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
	if (IsValid(BattleEntityComponent) == false)
		return nullptr;

	if (IsValid(BattleEntityComponent->GetBattleInfo()) == false)
		return nullptr;

	float SightRange = URenewal_BattleFunctionLibrary::GetCustomizedSightRange(LeaderCharacter->GetWorld(), BattleEntityComponent->GetBattleInfo(), true);
	EPriorityTypeEnum SearchPreferType = BattleEntityComponent->GetBattleInfo()->CharacterProperty.AIPriorityType;
	float InRangeThreshold = BattleEntityComponent->GetBattleInfo()->CharacterProperty.RangeThreshold;
	FVector OwnerLocation = LeaderCharacter->GetActorLocation();

	TArray< TWeakObjectPtr<AActor> > CheckBattleCharacter;
	URenewal_BattleFunctionLibrary::FindTarget(CheckBattleCharacter, LeaderCharacter.Get(), OwnerLocation, EFriendshipTypeEnum::VE_Enemy, ESearchShapeTypeEnum::VE_Circle, FVector(SightRange, SightRange, SightRange));

	if (CheckBattleCharacter.Num() == 0)
		return nullptr;

	//if (NextTargetHandler.IsValid())
	//{
	//GetWorldTimerManager().ClearTimer(NextTargetHandler);
	//}

	TArray< TWeakObjectPtr<AActor> > SelectableTagetList;
	for (auto BattleChar : CheckBattleCharacter)
	{
		TWeakObjectPtr<AActor> CheckTarget = BattleChar;

		if (CheckTarget.IsValid() && !UAIFunctionLibrary::IsDead(CheckTarget.Get()))
		{
			SelectableTagetList.Emplace(CheckTarget);
		}
	}

	if (SelectableTagetList.Num() == 0)
		return nullptr;


	UEntityAIComponent*  EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(LeaderCharacter.Get());
	if (EntityAIComponent == nullptr)
		return nullptr;


	TWeakObjectPtr<AActor> CurrentTargetPtr = EntityAIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);

	if (CurrentTargetPtr.IsValid())
	{
		if (!TargetAlreadySelectedList.Contains(CurrentTargetPtr))
			TargetAlreadySelectedList.Emplace(CurrentTargetPtr);
	}

	
	TargetAlreadySelectedList.RemoveAll([](const TWeakObjectPtr< AActor > Ptr) {
		return (!Ptr.IsValid() || UAIFunctionLibrary::IsDead(Ptr.Get()));
	});

	// NextTarget Search
	TWeakObjectPtr<AActor> NextTarget;


	float MinDist = 100000.f;
	FVector LeaderPos = LeaderCharacter->GetActorLocation();
	for (TWeakObjectPtr<AActor> Target : SelectableTagetList)
	{
		if (!TargetAlreadySelectedList.Contains(Target))
		{
			FVector Dist = LeaderPos - Target->GetActorLocation();
			if (Dist.Size() < MinDist)
			{
				MinDist = Dist.Size();
				NextTarget = Target;
			}
				
		}
	}

	if (!NextTarget.IsValid())
	{
		NextTarget = SelectableTagetList[0];
		TargetAlreadySelectedList.Empty();

	}

	if (NextTarget.IsValid())
	{
		//EntityAIComponent->SetBlackboardForceTarget(NextTarget.Get());
		EntityAIComponent->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor, NextTarget.Get());
		BattleEntityComponent->SetCurrentAttackTarget(NextTarget.Get());

		bool IsBossActor = URenewal_BattleFunctionLibrary::IsBossActor(NextTarget.Get());
		if (IsBossActor == false)
		{
			UUtilFunctionLibrary::NextTargetEffect(LeaderCharacter.Get(), NextTarget.Get(), 1.0f);
		}


// 		#if WITH_EDITOR
// 				FVector TargetLoc = NextTarget.Get()->GetActorLocation();
// 				FColor color = FColor::Green;
// 			
// 				color = FColor::Red;
// 				
// 				DrawDebugLine(GetWorld(), OwnerLocation, TargetLoc, color, true, 0.5f, SDPG_MAX, 5.f);
// 		#endif
	}

	return NextTarget.Get();
}
