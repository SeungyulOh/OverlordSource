// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GameFramework/Actor.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "CustomActors/RTargettingIndicator.h"
#include "CustomStructures/HeroSlotInfo.h"

#include "GameInfo/PlayStateInfo.h"
#include "RFocusAttackIcon.h"

void URFocusAttackIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &URFocusAttackIcon::BtnEventOnClicked);
		Button_Click->OnClicked.AddDynamic(this, &URFocusAttackIcon::BtnEventOnClicked);
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());

}

void URFocusAttackIcon::NativeDestruct()
{
	Super::NativeDestruct();
}


void URFocusAttackIcon::BtnEventOnClicked()
{
	if (this->GetVisibility() == ESlateVisibility::Collapsed)
		return;

	if (!BasePlayerController)
		return;

	bool bLeaderhasTarget = FocusAttack();	
	RGAMEINSTANCE(this)->PlayStateInfo->SetbLeaderhasTarget(bLeaderhasTarget);

	PlayAni("Click_Pressed", true);
}


void URFocusAttackIcon::SetClickedButton(bool InEnableClick)
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

bool URFocusAttackIcon::FocusAttack()
{
	bool bResult = false;
	if (RGAMEINSTANCE(this) == nullptr)
		return bResult;
	TWeakObjectPtr<AActor> LeaderChar = BasePlayerController->GetSelectedCharacter();
	if (!LeaderChar.IsValid())
		return bResult;

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderChar.Get());
	if (IsValid(BattleEntityComponent) == false)
		return bResult;

	AActor* CurrentTarget = BattleEntityComponent->GetCurrentAttackTarget();
	if (CurrentTarget == nullptr)
		return bResult;
	

	auto SelectedCharacterArray = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
	for (FHeroSlotInfo& info : SelectedCharacterArray)
	{
		if (IsValid(info.SpawnedTargetActor) == false || info.CharacterCrewState != ECharacterCrewStateEnum::VE_Battle)
			continue;

		if (info.SpawnedTargetActor != LeaderChar)
		{
			UEntityAIComponent*  EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(info.SpawnedTargetActor);
			if (IsValid(EntityAIComponent))
			{
				EntityAIComponent->SetBlackboardForceTarget(CurrentTarget);
			}
		}

		UUtilFunctionLibrary::NextTargetEffect(info.SpawnedTargetActor, CurrentTarget, 1.0f, false);
		bResult = true;

		ARTargettingIndicator** FoundActor = ActivePSCMap.Find(info.SpawnedTargetActor);
		ARTargettingIndicator* TargettingIndicator = nullptr;
		if (FoundActor)
		{
			TargettingIndicator = *FoundActor;
			if (TargettingIndicator && IsValidLowLevel())
				TargettingIndicator->Destroy();
		}
		
		FTransform transform;
		transform.SetLocation(info.SpawnedTargetActor->GetActorLocation());

		TargettingIndicator = info.SpawnedTargetActor->GetWorld()->SpawnActorDeferred<ARTargettingIndicator>(ARTargettingIndicator::StaticClass(), transform);
		TargettingIndicator->CurrentTarget = CurrentTarget;
		TargettingIndicator->SourceActor = info.SpawnedTargetActor;
		TargettingIndicator->FinishSpawning(transform);

		RGAMEINSTANCE(GEngine)->GameEnvironmentVar.OutLineElapsedTime = 0.f;
		ActivePSCMap.Add(info.SpawnedTargetActor, TargettingIndicator);

	}
	return bResult;
}
