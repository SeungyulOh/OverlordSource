// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AttackAnimState.h"
#include "GlobalIntegrated.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/WaypointComponent.h"
#include "Character/BaseAIController.h"

#include "RSkillSet_New.h"
#include "UI/UI_Battle/RAttackIcon.h"


#include "UtilFunctionIntegrated.h"
#include "Character/Components/AnimStateComponent.h"
#include "LeaderControlHelper.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "ContentGameMode/Controllers/ArenaPlayerController.h"
#include "ContentGameMode/Controllers/MirrorPlayerController.h"

UAttackAnimState::UAttackAnimState()
{
	AnimStateType = EAnimStateEnum::VE_NormalAttack;
}

void UAttackAnimState::Initialize()
{
	Super::Initialize();

	RemainAttackTime = 0.0f;
	RemainAttackHitTime = 0.0f;

	if (OwnerActor.IsValid())
	{
		auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (OwnerActor_BaseProperty)
		{
			CharacterCategoryType = OwnerActor_BaseProperty->GetCharacterCategory();

			FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(OwnerActor_BaseProperty->GetCharacterCategory(), OwnerActor_BaseProperty->GetCharacterID());
			if (CharacterInfo)
			{
				MeleeAttack = CharacterInfo->BulletID.IsNone();

				NormalAttackTime = CharacterInfo->NormalAttackTime;
				NormalAttackHitTime = CharacterInfo->NormalAttackHitTime;
				NormalAttackDelayTime = CharacterInfo->NormalAttackDelayTime;
				NormalAttackAnimCount = CharacterInfo->NormalAttackAnimCount;
			}
		}
		bRealTimeMode = URealTimeModManager::IsRealTimeGameMode();
	}
}

void UAttackAnimState::Uninitialize()
{
	Super::Uninitialize();
}

void UAttackAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);

	if (bRealTimeMode)
	{
		URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerActor->GetComponentByClass(URemoteMoveComponent::StaticClass()));
		if (RemoteMoveComponent)
			RemoteMoveComponent->bMoveActive = false;
	}

	NextNormalAttack = false;
	CurrentAttackAnim = 0;

	RemainAttackTime = 0.0f;
	RemainAttackHitTime = 0.0f;

	StartNormalAttack();

}

void UAttackAnimState::Leave()
{
	Super::Leave();

	if (bRealTimeMode)
	{
		URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerActor->GetComponentByClass(URemoteMoveComponent::StaticClass()));
		if (RemoteMoveComponent)
			RemoteMoveComponent->bMoveActive = true;
	}

	AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));
	if(StagePC)
		StagePC->GetLeaderControlHelper()->ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK, false);

	AttackQueue.Empty();

	

	RemainAttackTime = 0.0f;
	RemainAttackHitTime = 0.0f;
}

bool UAttackAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (0.0f < RemainAttackTime)
	{
		RemainAttackTime -= DeltaTime;
		if (0.0f >= RemainAttackTime)
		{
			RemainAttackTime = 0.0f;
			EndNormalAttack();
		}
	}
	if (0.0f < RemainAttackHitTime)
	{
		RemainAttackHitTime -= DeltaTime;
		if (0.0f >= RemainAttackHitTime)
		{
			RemainAttackHitTime = 0.0f;
			HitNormalAttack();
		}
	}

	if (ReservedStopAttack && IsCancelAnim())
	{
		auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
		if (IsValid(OwnerActor_AnimStateComponent))
		{
			OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopAttack);
		}
		ReservedStopAttack = false;
	}

	return true;
}

void UAttackAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	NetSyncAttackAnim = false;
	int32 param_value = 0;
	varParam.GetValue(param_value);
	if (NET_SYNC_ATTACK == param_value)
	{
		NetSyncAttackAnim = true;
	}	
}

bool UAttackAnimState::IsValidState()
{
	return true;
}

bool UAttackAnimState::IsCancelAnim()
{
	if (OwnerActor.IsValid())
	{
		bool bAbleToCancel = (0.0f == RemainAttackHitTime);

		if(bAbleToCancel)
			return bAbleToCancel;
		else
			return bAbleToCancel;
	}

	return false;
}

EAnimStateEnum UAttackAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	if (false == OnlyChecked)
	{
		NextDoCommand = Command;
	}

	EAnimStateEnum ReturnAimState = EAnimStateEnum::VE_NormalAttack;

	switch (Command)
	{
	case ECharacterCommandEnum::VE_Die:
	{
		ReturnAimState = EAnimStateEnum::VE_Die;
	}break;
	case ECharacterCommandEnum::VE_Abnormal:
	{
		ReturnAimState = EAnimStateEnum::VE_Abnormal;
	}break;
	case ECharacterCommandEnum::VE_ActionSkill:
	case ECharacterCommandEnum::VE_CastingSkill:
	{
		ReturnAimState = EAnimStateEnum::VE_SkillAttack;
	}break;
	case ECharacterCommandEnum::VE_StartAttack:
	{
		if (!bRealTimeMode)
			break;

		IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (!BaseProperty)
			break;

		if(UUtilFunctionLibrary::IsMyPlayCharacter(OwnerActor.Get()) || BaseProperty->GetPartyID() == UObjectFunctionLibrary::GetMyPartyID())
		{
			NextNormalAttackAnim();
			StartNormalAttack();
		}
		else
		{
			if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Npc &&
				BaseProperty->GetPartyID() != UObjectFunctionLibrary::GetMyPartyID() &&
				BaseProperty->GetGroupKey() != UObjectFunctionLibrary::GetMyTeamKey())
			{
				NextNormalAttackAnim();
				StartNormalAttack();
			}
		}
	}break;
	case ECharacterCommandEnum::VE_StopAttack:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_Idle;
		}
		else
		{
			ReservedStopAttack = true;
		}
	}break;
	case ECharacterCommandEnum::VE_StartMove:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_Run;
		}
	}break;
	case ECharacterCommandEnum::VE_StopMove:
	{
		ReturnAimState = EAnimStateEnum::VE_Idle;
	}break;
	default:
		break;
	}

	return ReturnAimState;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// RenewalNormalAttack

void UAttackAnimState::StartNormalAttack()
{
	if (OwnerActor.IsValid())
	{
		float AnimSpeed = EntityAIComponent->GetModifiedAnimSpeed();
		RemainAttackTime = NormalAttackTime / AnimSpeed;
		RemainAttackHitTime = NormalAttackHitTime / AnimSpeed;


		UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
		if (IsValid(BattleBaseAnimInstance))
		{
			BattleBaseAnimInstance->SetNormalAttackAnim(CurrentAttackAnim);
		}

	}
}

void UAttackAnimState::EndNormalAttack()
{
	if (OwnerActor.IsValid())
	{
		//if user_inputmode is manual , Set Blackboard data to stop AI
		AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));
		if (StagePC)
		{
			if (StagePC->GetLeaderCharacter() == OwnerActor.Get() && StagePC->AIControlMode_UserInput == EAIControlModeEnum::VE_ManualMode)
			{
				if (StagePC->GetLeaderControlHelper()->ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK))
				{
					StagePC->GetLeaderControlHelper()->ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK, false);
					if (URealTimeModManager::IsRealTimeGameMode())
					{
						if (BattleEntityComponent->bShouldNextAttack)
						{
							BattleEntityComponent->bShouldNextAttack = false;
							NextNormalAttackAnim();
							StartNormalAttack();
						}
						else
						{
							UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
							if (IsValid(BattleBaseAnimInstance))
								BattleBaseAnimInstance->PauseAnimMontageNormalAttack();
						}
						
						/*FString str = TEXT("PauseAnim : ") + FString::FromInt(CurrentAttackAnim);
						GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, str);*/
						return;
					}
				}
				else
				{
					StagePC->GetLeaderControlHelper()->ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STOPAI_FROMATTACK, true);
					if (URealTimeModManager::IsRealTimeGameMode())
					{
						BattleEntityComponent->EndAttack();
						return;
					}
				}
			}
			else
			{
				if (URealTimeModManager::IsRealTimeGameMode())
				{
					int32 SlotIdx = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetSelectedCharacterSlotIndex(OwnerActor.Get());
					if (SlotIdx != ROLE_LEADER_INDEX)
					{
						if (BattleEntityComponent->bShouldNextAttack)
						{
							BattleEntityComponent->bShouldNextAttack = false;
							NextNormalAttackAnim();
							StartNormalAttack();
						}
						else
						{
							UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
							if (IsValid(BattleBaseAnimInstance))
								BattleBaseAnimInstance->PauseAnimMontageNormalAttack();
						}
					}

					return;
				}
				
			}
		}
		
		NextNormalAttack = false;
		bool bNoControlMode = false;
		ABasePlayerController* pBasePC = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
		if (IsValid(StagePC))
		{
			if (StagePC->IsSelectedCharacter(OwnerActor.Get()))
			{
				if (StagePC->GetLeaderControlHelper()->CheckInputCommandOfNormalAttack())
					NextNormalAttack = true;
			}
			else
				NextNormalAttack = true;
		}
		else
		{
			AArenaPlayerController* ArenaPC = Cast<AArenaPlayerController>(pBasePC);
			if (IsValid(ArenaPC))
				bNoControlMode = true;
			AMirrorPlayerController* MirrorDungeonPC = Cast<AMirrorPlayerController>(pBasePC);
			if (IsValid(MirrorDungeonPC))
				bNoControlMode = true;
		}

		if (NextNormalAttack || bNoControlMode)
		{
			NextNormalAttackAnim();
			StartNormalAttack();
		}
		else
		{
			BattleEntityComponent->EndAttack();
		}
	}
}

void UAttackAnimState::HitNormalAttack()
{
	if (OwnerActor.IsValid())
	{
		if (MeleeAttack)
		{
			BattleEntityComponent->HitTarget();
		}
		else
		{
			BattleEntityComponent->ShotProjectile();
		}
	}

	if (!URealTimeModManager::IsRealTimeGameMode())
		return;


	URSkillSet_New* Skillset = UUIFunctionLibrary::GetSkillSet(RGAMEINSTANCE(GEngine)->GetWorld());
	if (!Skillset)
		return;

	bool bAttackIconState = Skillset->Variables.AttackIcon->AttackButtonActive;
	if (bAttackIconState)
	{
		AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));
		if (!StagePC)
			return;

		ULeaderControlHelper* LeaderHelper = StagePC->GetLeaderControlHelper();
		if (!LeaderHelper)
			return;

		if (!AttackQueue.IsEmpty())
			return;

		if (!LeaderHelper->ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK))
		{
			LeaderHelper->ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK, true);
		}

		UBattleEntityComponent* Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (Owner_BattleEntityComponent)
		{
			int32 Animidx = GetCurrentAttackAnimIdx();
			Owner_BattleEntityComponent->SendRTS_AttackNow(true, Animidx);
		}
	}
	else
	{
		int32 SlotIdx = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetSelectedCharacterSlotIndex(OwnerActor.Get());
		if (SlotIdx != ROLE_LEADER_INDEX)
		{
			AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));
			if (!StagePC)
				return;

			ULeaderControlHelper* LeaderHelper = StagePC->GetLeaderControlHelper();
			if (!LeaderHelper)
				return;

			if (!AttackQueue.IsEmpty())
				return;

			UBattleEntityComponent* Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			if (Owner_BattleEntityComponent)
			{
				int32 Animidx = GetCurrentAttackAnimIdx();
				Owner_BattleEntityComponent->SendRTS_AttackNow(true, Animidx);
			}
		}
	}
}

void UAttackAnimState::NextNormalAttackAnim()
{
	++CurrentAttackAnim;

	if (NormalAttackAnimCount <= CurrentAttackAnim)
		CurrentAttackAnim = 0;
}