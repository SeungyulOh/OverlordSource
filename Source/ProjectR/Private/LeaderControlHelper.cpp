// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"

#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RAttackIcon.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RCampaignHUD_RaidThreeControl.h"

#include "UI/UI_Battle/RHeroIcon_ForInGame.h"
#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"

#include "RSkillSet_New.h"


#include "GameInfo/PlayStateInfo.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Table/SkillTableInfo_V2.h"

#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/AvoidanceComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/AnimState/AttackAnimState.h"

#include "Character/BattleBaseCharacter.h"
#include "Character/Components/CustomAmbientComponent.h"

#include "GameState/Stage/BaseLevelScriptActor.h"

#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "ContentGameMode/Modes/StageGameMode.h"

#include "CustomActors/RSceneCapturer.h"

#include "LeaderControlHelper.h"
#include "DirectionTrackManager.h"


ULeaderControlHelper::ULeaderControlHelper(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void ULeaderControlHelper::Tick(float DeltaTime)
{
	if (!LeaderCharacter.IsValid())
	{
		InitDeferred();
	}

	TargetEffectProcess();
}



void ULeaderControlHelper::CallbackInputDPadValue(EInputCommandTypeEnum InputCommand, FVector2D AxisValue)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));

	URSkillSet_New* Skillset = UUIFunctionLibrary::GetSkillSet(GameInstance->GetWorld());

	switch (InputCommand)
	{
	case EInputCommandTypeEnum::VE_LeftAxis:
	{
		if (!Skillset)
			return;

		bool bAttackIconState = Skillset->Variables.AttackIcon->AttackButtonActive;
		if (!bAttackIconState && AxisValue.IsNearlyZero())
			SetDPadStateFlag(EDPadControllerStateFlagEnum::VE_Idle, true);
		else
		{
			if (!AxisValue.IsNearlyZero())
			{
				SetDPadStateFlag(EDPadControllerStateFlagEnum::VE_Move, true);
				SetAIEnableOnce();
			}
		}
	}break;
	}
	ExcuteDPadState();
}

void ULeaderControlHelper::InitDeferred()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	checkf(GameInstance->EventManager != nullptr, TEXT("ULeaderControlHelper[EventManager Is Empty]"));
	checkf(GameInstance->InputManager != nullptr, TEXT("ULeaderControlHelper[InputManager Is Empty]"));
	checkf(BasePC != nullptr, TEXT("BasePC Is Empty]"));
#ifdef WITH_EDITOR
	//test map에서 컨트롤러동작안하는문제 떄문에 임시로 추가
	// 	GameInstance->InitBeforeLaunch();
	// 	GameInstance->InitAfterLaunch();
	GameInstance->InputManager->Initialize(GameInstance);

#endif // WITH_EDITOR	

	LeaderCharacter = GameInstance->PlayStateInfo->GetOwnerCharacter(ROLE_LEADER_INDEX);

	DPadSkillInterface = UUIFunctionLibrary::GetSkillSet(BasePC);
}

void ULeaderControlHelper::BindingDelegates()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	checkf(GameInstance->EventManager != nullptr, TEXT("ULeaderControlHelper[EventManager Is Empty]"));
	checkf(GameInstance->InputManager != nullptr, TEXT("ULeaderControlHelper[InputManager Is Empty]"));
	if (!IsValid(BasePC))
		BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(this)->GetWorld()));

	GameInstance->InputManager->OnInputAxisValueDelegate.Clear();
	GameInstance->InputManager->OnInputAxisValueDelegate.AddDynamic(this, &ULeaderControlHelper::CallbackInputDPadValue);

}

bool ULeaderControlHelper::CheckLeaderCharacterAttackAnim()
{
	checkf(BasePC != nullptr, TEXT("CheckLeaderCharacterAttackAnim[BasePC Is Empty]"));
	if (BasePC->GetLeaderAIModeType() == EAIControlModeEnum::VE_ManualMode)
	{
		if (BasePC->CurInputCommandType == EInputCommandTypeEnum::VE_AttackRelease)
		{
			if (BasePC->InputAttackHold)
			{
				//InputAttackHold = false;
				return true;
			}

			auto SelectCharacter_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(LeaderCharacter.Get());
			if (IsValid(SelectCharacter_AnimStateComponent))
			{
				SelectCharacter_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopAttack);
			}

			return false;
		}
	}

	return true;
}

bool ULeaderControlHelper::CheckInputCommandOfNormalAttack()
{
	checkf(BasePC != nullptr, TEXT("CheckInputCommandOfNormalAttack[BasePC Is Empty]"));
	if (BasePC->GetLeaderAIModeType() == EAIControlModeEnum::VE_ManualMode)
	{
		if (BasePC->CurInputCommandType == EInputCommandTypeEnum::VE_Attack)
		{
			if (BasePC->InputAttackHold)
			{
				return true;
			}
		}
		else if (BasePC->CurInputCommandType == EInputCommandTypeEnum::VE_AttackRelease)
		{
			if (BasePC->InputAttackHold)
			{
				return true;
			}

		}
		else if (BasePC->CurInputCommandType == EInputCommandTypeEnum::VE_None)
		{
			BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);
		}
	}
	else
	{
		return true;
	}
	

	return false;
}

bool ULeaderControlHelper::AttackProcess_OnlyManualMode()
{

	FVector2D DPadValue = RGAMEINSTANCE(GEngine)->InputManager->GetDPadMoveValue();

	UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(LeaderCharacter.Get());
	if (!AIComponent)
		return false;

	//if current phase is to find next waypoint
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo && DPadValue.IsNearlyZero())
	{
		ALevelActor* lvActor = BaseStageInfo->GetLevelActor();
		if (lvActor)
		{
			ALevelActorPhase* PhaseInfo = lvActor->GetLevelActorPhase();
			if (PhaseInfo)
			{
				if (PhaseInfo->ClearCondition == EPhaseClearEnumType::VE_Move)
				{
					BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_SemiAutoMode);
					return false;
				}
			}
		}
	}
	
	AActor* FoundActor = AIComponent->GetNearestSearchTarget();
	if (!FoundActor)
		return false;

	if(DPadValue.IsNearlyZero())
		BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_SemiAutoMode);
	else
	{
		
		auto Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
		if (Owner_BattleEntityComponent)
		{
			bool IsException = !(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(LeaderCharacter.Get(), FoundActor));
			if (IsException == false)
			{
				if (Owner_BattleEntityComponent->IsCloseEnoughForAttack(FoundActor))
				{
					if (Owner_BattleEntityComponent->IsEnemy(FoundActor))
					{
						BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_SemiAutoMode);
						return true;
					}
				}
			}
			else
				BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);
		}
	}

	return false;
	
	
}

void ULeaderControlHelper::ShowAttackRange_OnlyManualMode(bool isOn)
{
	float AttackRange = 0.f;
	UBattleEntityComponent* Leader_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
	if (Leader_BattleEntityComponent)
		AttackRange = Leader_BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);

	TArray<USkillAreaIndicatorMeshComponent*> AreaIndicatorComponents = UCharacterHelpFunctionLibrary::GetSkillAreaArray(LeaderCharacter.Get());
	for (size_t i = 0; i < AreaIndicatorComponents.Num(); ++i)
	{
		if (AreaIndicatorComponents[i]->GetSkillAreaMeshType() == ESkillAreaMeshTypeEnum::VE_SkillArea)
		{
			AreaIndicatorComponents[i]->ShowSkillArea(AttackRange, AttackRange);
			AreaIndicatorComponents[i]->SetVisibility(isOn);
		}
	}
}

void ULeaderControlHelper::TargetEffectProcess()
{
	if (LeaderCharacter.IsValid() == false)
		return;

	UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(LeaderCharacter.Get());
	if (IsValid(AIComponent) == false)
		return;

	AActor* TargetActor = AIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);

	if (!AIComponent->IsSetBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor))
	{
		TargetActor = nullptr;
	}

	if (TargetActor == LeaderCharacter.Get() || TargetActor == nullptr)
		return;

	auto SetInitialState = [](AActor* InTargetActor, bool bOn) {
		if (InTargetActor)
		{
			UEntityRenderComponent* CurrentRenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(InTargetActor);
			if (IsValid(CurrentRenderComp))
			{
				CurrentRenderComp->OutLine(bOn);

				auto TargetingComp = CurrentRenderComp->GetTargetingMeshComponent();
				if (TargetingComp)
					TargetingComp->SetVisibility(bOn);
			}
		}
	};

	
	if (CurrentAttackTarget.IsValid())
	{
		if (CurrentAttackTarget.Get() == TargetActor)
			return;

		if (CurrentAttackTarget.Get() != TargetActor)
		{
			SetInitialState(CurrentAttackTarget.Get(), false);
			CurrentAttackTarget = TargetActor;
			SetInitialState(CurrentAttackTarget.Get(), true);

		}
	}
	else 
	{
		CurrentAttackTarget = TargetActor;
		SetInitialState(CurrentAttackTarget.Get(), true);
	}


}

AActor* ULeaderControlHelper::FindCurrentBattleCrew(int32 InType)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	checkf(GameInstance->PlayStateInfo != nullptr, TEXT("ULeaderControlHelper[PlayStateInfo Is Empty]"));
	TArray<FHeroSlotInfo> HeroSlotArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (size_t i = 0; i < HeroSlotArray.Num(); ++i)
	{
		if (InType == (int32)ESkillOrigin::VE_Crew1)
		{
			if (HeroSlotArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew1 && HeroSlotArray[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				return HeroSlotArray[i].SpawnedTargetActor;
		}

		if (InType == (int32)ESkillOrigin::VE_Crew2)
		{
			if (HeroSlotArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew2 && HeroSlotArray[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				return HeroSlotArray[i].SpawnedTargetActor;
		}
		
	}
	return nullptr;
}





void ULeaderControlHelper::SetAIEnableOnce()
{
	if (bCrewAIEnable)
		return;
	
	bCrewAIEnable = true;
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	checkf(GameInstance != nullptr, TEXT("ULeaderControlHelper[RGameInstance Is Empty]"));
	GameInstance->EventManager->OnLeaderMove.Broadcast();
}




void ULeaderControlHelper::ExcuteDPadState()
{
	UAvoidanceComponent* LeaderMovementComp = Cast<UAvoidanceComponent>(UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(LeaderCharacter.Get()));
	if (!IsValid(LeaderMovementComp))
		return;
	
	URSkillSet_New* skillset = UUIFunctionLibrary::GetSkillSet(RGAMEINSTANCE(GEngine)->GetWorld());
	if (skillset)
	{
		URAttackIcon* AttackIcon = skillset->Variables.AttackIcon;
		if (AttackIcon->AttackButtonActive && BasePC->AIControlMode_UserInput == EAIControlModeEnum::VE_ManualMode)
		{
			bool FoundTarget = AttackProcess_OnlyManualMode();
			if (FoundTarget)
				return;
		}
	}

	if (IsDPadStateFlag(EDPadControllerStateFlagEnum::VE_Move))
	{
		//if leader succeeded in moving with user input, make leader stop AI
		if (LeaderMovementComp->MoveProcess_From_BasePlayerController())
		{
			BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);

			if (BasePC->AIControlMode_UserInput == EAIControlModeEnum::VE_ManualMode)
			{
				UAIFunctionLibrary::CheckNextWayPoint(BasePC->GetSelectedCharacter());
				ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMMOVE, true);
			}
		}
			
	}
	else if (IsDPadStateFlag(EDPadControllerStateFlagEnum::VE_Idle))
	{
		if (BasePC->AIControlMode_UserInput == EAIControlModeEnum::VE_ManualMode)
		{
			UAnimStateComponent* AnimStateComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(LeaderCharacter.Get());
			if (ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMMOVE))
			{
				ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMMOVE, false);
				BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);

				
				if (AnimStateComp)
					AnimStateComp->DoCommand(ECharacterCommandEnum::VE_StopMove);
			}
			if (ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMATTACK))
			{
				ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMATTACK, false);
				BasePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);
			}
				
		}
		else
		{
			auto SpawnedCharacter_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(LeaderCharacter.Get());
			if(SpawnedCharacter_EntityAIComponent)
				SpawnedCharacter_EntityAIComponent->SearchNearAttackTarget(true);
			
			BasePC->SetLeaderCharacterAIAutoMode(BasePC->AIControlMode_UserInput);
		}
	}

	
	DPadStateFlag = 0;
}

void ULeaderControlHelper::SetDPadStateFlag(EDPadControllerStateFlagEnum CheckState, bool bState)
{
	if (bState)
	{
		DPadStateFlag |= (int32)CheckState;
	}
	else
	{
		DPadStateFlag &= (~(int32)CheckState);
	}
}



//void ULeaderSkillControl::Caster_FindTargetedActors(TArray<AActor*>& outActors, FVector2D SourceVector, AActor* Caster)
//{
// 	UBaseStageInfo* StageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
// 	if (!StageInfo)
// 		return;
// 	if (Caster == nullptr)
// 		return;
// 
// 	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
// 	FSkillTableInfo_V2* CurrentSkillTable = nullptr;
// 	if (SkillEntityComponent)
// 	{
// 		UISkill_V2* Skill = SkillEntityComponent->GetSkill_V2(InputSkillName);
// 		if (Skill)
// 			CurrentSkillTable = Skill->GetSkillInfo();
// 	}
// 		
// 	if (!CurrentSkillTable)
// 		return;
// 
// 	if (CurrentSkillTable->TargetApply == EFriendshipTypeEnum::VE_Enemy)
// 	{
// 		for (size_t i = 0; i < StageInfo->ArrMonsterInfo.Num(); ++i)
// 		{
// 			if (StageInfo->ArrMonsterInfo[i] != nullptr)
// 			{
// 				FVector2D MonsterPos = FVector2D(StageInfo->ArrMonsterInfo[i]->GetActorLocation().X, StageInfo->ArrMonsterInfo[i]->GetActorLocation().Y);
// 				MonsterPos -= SourceVector;
// 				if (MonsterPos.Size() < SkillRange)
// 					outActors.Emplace(StageInfo->ArrMonsterInfo[i]);
// 			}
// 		}
// 
// 		for (size_t i = 0; i < StageInfo->ArrBossInfo.Num(); ++i)
// 		{
// 			if (StageInfo->ArrBossInfo[i] != nullptr)
// 			{
// 				FVector2D MonsterPos = FVector2D(StageInfo->ArrBossInfo[i]->GetActorLocation().X, StageInfo->ArrBossInfo[i]->GetActorLocation().Y);
// 				MonsterPos -= SourceVector;
// 				if (MonsterPos.Size() < SkillRange)
// 					outActors.Emplace(StageInfo->ArrBossInfo[i]);
// 			}
// 		}
// 			
// 	}
// 	else if (CurrentSkillTable->TargetApply == EFriendshipTypeEnum::VE_FriendParty)
// 	{
// 		for (size_t i = 0; i < StageInfo->ArrPlayerInfo.Num(); ++i)
// 		{
// 			FVector2D MonsterPos = FVector2D(StageInfo->ArrPlayerInfo[i]->GetActorLocation().X, StageInfo->ArrPlayerInfo[i]->GetActorLocation().Y);
// 			MonsterPos -= SourceVector;
// 			if (MonsterPos.Size() < SkillRange)
// 				outActors.Emplace(StageInfo->ArrPlayerInfo[i]);
// 		}
// 			
// 	}
//}


void FManualAI_BlackBoard::SetBlackBoardData(EAIStopFrom InData, bool InValue)
{
	if (BlackBoardArray.Num() == 0)
		BlackBoardArray.Init(false, (int32)EAIStopFrom::STOPAI_MAX);

	int32 iIdx = (int32)InData;
	if (BlackBoardArray.IsValidIndex(iIdx))
		BlackBoardArray[iIdx] = InValue;
}

bool FManualAI_BlackBoard::GetBlackBoardData(EAIStopFrom InData)
{
	int32 iIdx = (int32)InData;
	if (BlackBoardArray.IsValidIndex(iIdx))
		return BlackBoardArray[iIdx];

	return false;
}
