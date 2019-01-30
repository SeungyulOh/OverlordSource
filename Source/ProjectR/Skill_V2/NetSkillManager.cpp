// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "NetSkillManager.h"

#include "GlobalIntegrated.h"

//#include "Global/BattleFieldManager.h"
#include "Skill_V2/SkillSyncApplyData.h"
#include "Skill_V2/SharedSkill/Skill_Passive_V2.h"

#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/RemoteBattleEntityComponent.h"



#include "Character/Interfaces/EntityBaseProperty.h"

#include "UtilFunctionIntegrated.h"
#include "Table/SkillTableInfo_V2.h"

#include "TimerManager.h"

//#include "Kismet/KismetMathLibrary.h"



void UNetSkillManager::Init(URGameInstance* game_inst)
{
	RGameInstance = game_inst;

	
	{
		InitNetSkillMgr = true;
	}
}

void UNetSkillManager::Update(float delta_time)
{
	if (InitNetSkillMgr)
	{
		CheckReceiveWaitTime(delta_time);
	}
}

bool UNetSkillManager::TrySkillCasting(AActor* Caster, UISkill_V2* CastSkill, AActor* ActorTarget, FVector LocationTarget)
{
	
	if (false == URealTimeModManager::IsRealTimeGameMode())
		return false;

	if (false == IsValid(Caster))
		return false;

	if (false == CheckCasterCondition(Caster))
		return false;

	USkillEntityComponent* Caster_SkillEntityComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (false == IsValid(Caster_SkillEntityComp))
		return false;

	if (false == IsValid(CastSkill))
		return false;

	if (false == CastSkill->RTS_CanCastSkill())
		return false;

	if (false == SendRTS_CastSkill(CastSkill, Caster, ActorTarget, LocationTarget))
	{
		Caster_SkillEntityComp->SetVisibleSkillArea_V2(false);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//
// Receive BattleField (CS Style)
//
//////////////////////////////////////////////////////////////////////////
void UNetSkillManager::RecvRTS_SkillFailed(int32 CasterUK, uint8 errortype)
{
	TWeakObjectPtr<AActor> CastActor = nullptr;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		CastActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(CasterUK);
	}
	if (CastActor.IsValid() == false)
		return;

	FString str = TEXT("SkillFailed Caster : ") + CastActor->GetName();

	switch (errortype) {
	case RTSCE::RTSCE_SKILL_BLOCKED_CAST: // 
		str += TEXT(" SKILL_BLOCKED_CAST");
		break;
	case RTSCE::RTSCE_SKILL_NOT_READY: // 
		str += TEXT(" SKILL_NOT_READY");
		break;
	case RTSCE::RTSCE_SKILL_INVALID_TARGET: // 
		str += TEXT(" SKILL_INVALID_TARGET");
		break;
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, str);

	AActor* Caster = CastActor.Get();
	USkillEntityComponent* CaterSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (nullptr == CaterSkillComp)
		return;

	ClearReceiveWaitInfo(CaterSkillComp);
}

void UNetSkillManager::RecvRTS_CastSkill(OVERLORD::RTS::S2C::ACK_BT_SKILL_CASTEDT* RecvData)
{
// 	if (false == IsValid(RGameInstance))
// 		return;

// 	if (false == IsValid(RGameInstance->GameObjectMgr))
// 		return;

	if (nullptr == RecvData)
		return;

	if (false == RecvData->casting)
		return;

	TWeakObjectPtr<AActor> CastActor = nullptr;//RGameInstance->GameObjectMgr->GetObjectByPlayerUK(RecvData->actorUK);
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (!pGamePlayMode)
// 		return;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;
	
// 	CastActor = pGamePlayMode->GetObjectByPlayerUK(RecvData->actorUK);
	CastActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->actorUK);
	if (CastActor.IsValid() == false)
		return;

	AActor* Caster = CastActor.Get();
	USkillEntityComponent* CaterSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (nullptr == CaterSkillComp)
		return;

	ClearReceiveWaitInfo(CaterSkillComp);

	UISkill_V2* CastingSkill = CaterSkillComp->GetSkillByCategory_V2((ESkillCategoryTypeEnum)(RecvData->slot));
	if (nullptr == CastingSkill)
		return;

	FSkillTableInfo_V2* SkillInfo = CastingSkill->GetSkillInfo();
	if (nullptr == SkillInfo)
		return;

	//UE_LOG(LogBattle, Log, TEXT("=====>>>>> RecvRTS_CastSkill(%s) : %f"), *(SkillInfo->SkillName.ToString()), FPlatformTime::Seconds());

	AActor* AimTarget = nullptr;
	FVector AimLocation = FVector::ZeroVector;
	if (RecvData->pivot)
	{
		if (RecvData->pivot->startPos)
		{
			FVector CasterLoc;
			SetLocation(CasterLoc, RecvData->pivot->startPos.get());
			UMapFunctionLibrary::AdjustActorLocation(Caster, CasterLoc);
			//Caster->SetActorLocation(CasterLoc);
		}
		//FRotator CasterRot;
		//SetRotation(CasterRot, RecvData->pivot->direction.get());
		//Caster->SetActorRotation(CasterRot);


		TWeakObjectPtr<AActor> AimTargetActor = nullptr;// = RGameInstance->GameObjectMgr->GetObjectByPlayerUK(RecvData->pivot->targetUK);
// 		AimTargetActor = pGamePlayMode->GetObjectByPlayerUK(RecvData->pivot->targetUK);
		AimTargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->pivot->targetUK);
		if (AimTargetActor.IsValid())
		{
			AimTarget = AimTargetActor.Get();
		}

		if (RecvData->pivot->location)
		{
			SetLocation(AimLocation, RecvData->pivot->location.get());
		}

		if (AimLocation.IsNearlyZero())
		{
			AimLocation = AimTarget->GetActorLocation();
		}
	}

	CastingSkill->Casting(AimTarget, AimLocation);
}

void UNetSkillManager::RecvRTS_ApplySkill(OVERLORD::RTS::S2C::ACK_BT_SKILL_CASTEDT* RecvData)
{
// 	if (false == IsValid(RGameInstance))
// 		return;

// 	if (false == IsValid(RGameInstance->GameObjectMgr))
// 		return;
	
	TWeakObjectPtr<AActor> CastActor = nullptr;// RGameInstance->GameObjectMgr->GetObjectByPlayerUK(RecvData->actorUK);
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (!pGamePlayMode)
// 		return;

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;
	
// 	CastActor = pGamePlayMode->GetObjectByPlayerUK(RecvData->actorUK);//GetObjectList();
	CastActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->actorUK);
	if (CastActor.IsValid() == false)
		return;

	AActor* Caster = CastActor.Get();

	USkillEntityComponent* CaterSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (nullptr == CaterSkillComp)
		return;

	UISkill_V2* ApplySkill = CaterSkillComp->GetSkillByCategory_V2((ESkillCategoryTypeEnum)(RecvData->slot));
	if (nullptr == ApplySkill)
		return;

	FSkillTableInfo_V2* SkillInfo = ApplySkill->GetSkillInfo();
	if (nullptr == SkillInfo)
		return;

	//UE_LOG(LogBattle, Log, TEXT("=====>>>>> RecvRTS_ApplySkill(%s) : %f"), *(SkillInfo->SkillName.ToString()), FPlatformTime::Seconds());

	ESkillCategoryTypeEnum ApplySkillCategory = ApplySkill->GetSkillCategoryType();
	UISkill_V2* CurrentSkill = nullptr;
	if (ESkillCategoryTypeEnum::VE_PassiveSkill01 != ApplySkillCategory)
	{
		ClearReceiveWaitInfo(CaterSkillComp);

		CurrentSkill = CaterSkillComp->GetCurrentSkill_V2();
		if (CurrentSkill && ApplySkill != CurrentSkill)
		{
#if LOG_NET_REALTIME
			UE_LOG(LogBattle, Log, TEXT("Exist CurrentSkill(%s)"), *(CurrentSkill->GetSkillID().ToString()));
#endif
			ESkillStateEnum skill_state = CurrentSkill->GetSkillState();
			if (ESkillStateEnum::VE_Fire > skill_state)
			{
				CaterSkillComp->FireCurrentSkill_V2();
			}
			CaterSkillComp->EndSkillAction_V2();
			CurrentSkill = nullptr;
		}
	}

	int32 TargetCount = (int32)(RecvData->targets.size());
	TArray<FSkillSyncApplyData> ApplyDataList;
	ApplyDataList.Init(FSkillSyncApplyData(), TargetCount);
	for (int32 n = 0; n < TargetCount; ++n)
	{
		SetSkillApplyData(ApplyDataList[n], RecvData->targets[n].get());
	}

	//if (0 >= ApplyDataList.Num())
	//{
	//	UE_LOG(LogBattle, Log, TEXT("=====>>>>> RecvRTS_ApplySkill(%s) : Num of received TargetData is 0"), *(SkillInfo->SkillName.ToString()));
	//}

	FSkillSyncApplyData CasterApplyData;
	if(RecvData->caster)
	{
		SetSkillApplyData(CasterApplyData, RecvData->caster.get());
	}
	//else
	//{
	//	UE_LOG(LogBattle, Log, TEXT("=====>>>>> RecvRTS_ApplySkill(%s) : Num of received CasterData is 0"), *(SkillInfo->SkillName.ToString()));
	//}

	ApplySkill->SetSkillSyncData(CasterApplyData, ApplyDataList);

	if (ESkillCategoryTypeEnum::VE_PassiveSkill01 != ApplySkillCategory)
	{
		AActor* AimTarget = nullptr;
		FVector AimLocation = FVector::ZeroVector;
		if (RecvData->pivot)
		{
			if (RecvData->pivot->startPos)
			{
				FVector CasterLoc;
				SetLocation(CasterLoc, RecvData->pivot->startPos.get());
				UMapFunctionLibrary::AdjustActorLocation(Caster, CasterLoc);
				//Caster->SetActorLocation(CasterLoc);
			}
			//FRotator CasterRot;
			//SetRotation(CasterRot, RecvData->pivot->direction.get());
			//Caster->SetActorRotation(CasterRot);

			TWeakObjectPtr<AActor> AimTargetActor = nullptr;// RGameInstance->GameObjectMgr->GetObjectByPlayerUK(RecvData->pivot->targetUK);

// 			AimTargetActor = pGamePlayMode->GetObjectByPlayerUK(RecvData->pivot->targetUK);
			AimTargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->pivot->targetUK);
			if (AimTargetActor.IsValid())
			{
				AimTarget = AimTargetActor.Get();
			}

			if (RecvData->pivot->location)
			{
				SetLocation(AimLocation, RecvData->pivot->location.get());
			}

			if (AimLocation.IsNearlyZero() && AimTarget)
			{
				AimLocation = AimTarget->GetActorLocation();
			}

			if (RecvData->pivot->direction && SkillInfo->TargetType == ESkillTargetTypeEnum::VE_Me 
				&& SkillInfo->EffectShape != ESearchShapeTypeEnum::VE_Circle)
			{
				FVector Direction;
				SetLocation(Direction, RecvData->pivot->direction.get());
				AimLocation += Direction;
			}
		}

		if(nullptr == CurrentSkill)
		{
			//UE_LOG(LogBattle, Log, TEXT("=====>>>>> RecvRTS_ApplySkill(%s) Cast ApplySkill because CurrentSkill is Null"), *(SkillInfo->SkillName.ToString()));
			if (false == ApplySkill->Casting(AimTarget, AimLocation))
			{
				// Process OnDamage and Apply Skill
				ApplySkill->ForceApplySkillSyncData();
				ApplySkill->End();
			}
		}
	}
	else
	{
		USkill_Passive_V2* PassiveSkillInstance = Cast<USkill_Passive_V2>(ApplySkill);
		PassiveSkillInstance->FireCoditionCheckSkill();
	}
}

void UNetSkillManager::RecvRTS_IntervalSkill(OVERLORD::RTS::S2C::ACK_BT_SKILL_CASTED_INTERVALT* RecvData)
{
// 	if (false == IsValid(RGameInstance))
// 		return;

// 	if (false == IsValid(RGameInstance->GameObjectMgr))
// 		return;

	TWeakObjectPtr<AActor> CastActor = nullptr;//RGameInstance->GameObjectMgr->GetObjectByPlayerUK(RecvData->actorUK);
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (!pGamePlayMode)
// 		return;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;

// 	CastActor = pGamePlayMode->GetObjectByPlayerUK(RecvData->actorUK);
	CastActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->actorUK);
	if (CastActor.IsValid() == false)
		return;

	AActor* Caster = CastActor.Get();

	USkillEntityComponent* CaterSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (nullptr == CaterSkillComp)
		return;

	ClearReceiveWaitInfo(CaterSkillComp);

	UISkill_V2* ApplySkill = CaterSkillComp->GetSkillByCategory_V2((ESkillCategoryTypeEnum)(RecvData->slot));
	if (nullptr == ApplySkill)
		return;

	FSkillTableInfo_V2* SkillInfo = ApplySkill->GetSkillInfo();
	if (nullptr == SkillInfo)
		return;

	if (RecvData->pivot)
	{
		if (RecvData->pivot->startPos)
		{
			FVector CasterLoc;
			SetLocation(CasterLoc, RecvData->pivot->startPos.get());
			UMapFunctionLibrary::AdjustActorLocation(Caster, CasterLoc);
			//Caster->SetActorLocation(CasterLoc);
		}
		//FRotator CasterRot;
		//SetRotation(CasterRot, RecvData->pivot->direction.get());
		//Caster->SetActorRotation(CasterRot);
	}

	int32 TargetCount = (int32)(RecvData->targets.size());
	TArray<FSkillSyncApplyData> ApplyDataList;
	ApplyDataList.Init(FSkillSyncApplyData(), TargetCount);
	for (int32 n = 0; n < TargetCount; ++n)
	{
		SetSkillApplyData(ApplyDataList[n], RecvData->targets[n].get());
	}

	FSkillSyncApplyData CasterApplyData;
	SetSkillApplyData(CasterApplyData, RecvData->caster.get());

	ApplySkill->SetSkillSyncData(CasterApplyData, ApplyDataList);
	ESkillCategoryTypeEnum ApplySkillCategory = ApplySkill->GetSkillCategoryType();
	if (ESkillCategoryTypeEnum::VE_PassiveSkill01 != ApplySkillCategory)
	{
		if (ESkillCastingTypeEnum::VE_Channeling == SkillInfo->CastingType)
		{
			ApplySkill->Fire();

			if (0 >= RecvData->leftCount)
			{
				ApplySkill->ReserveEnd();
			}
		}
		else
		{
			ApplySkill->GetApplyTarget();
			ApplySkill->Apply();
		}
	}
	else
	{
		USkill_Passive_V2* PassiveSkillInstance = Cast<USkill_Passive_V2>(ApplySkill);
		PassiveSkillInstance->FireCoditionCheckSkill();
	}
}

void UNetSkillManager::RecvRTS_UpdateSkillSlot(OVERLORD::RTS::S2C::ACK_BT_SYNC_SKILL_UPDATET* RecvData)
{
// 	if (false == IsValid(RGameInstance))
// 		return;

// 	if (false == IsValid(RGameInstance->GameObjectMgr))
// 		return;

	AActor* Caster = nullptr;
	TWeakObjectPtr<AActor> CastActor = nullptr;// RGameInstance->GameObjectMgr->GetObjectByPlayerUK(RecvData->actorUK);
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		CastActor = pGamePlayMode->GetObjectByPlayerUK(RecvData->actorUK);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		CastActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->actorUK);
	}
	if (CastActor.IsValid())
	{
		Caster = CastActor.Get();
	}

	AActor* Target = nullptr;
	USkillEntityComponent* TargetSkillComp = nullptr;
	UBattleEntityComponent* TargetBattleComp = nullptr;
	URemoteMoveComponent* TargetMoveComp = nullptr;
	FCSNormalAttackSyncData AttackData;

	int32 TargetCount = (int32)(RecvData->targets.size());
	TArray<FSkillSyncApplyData> ApplyDataList;
	ApplyDataList.Init(FSkillSyncApplyData(), TargetCount);
	for (int32 n = 0; n < TargetCount; ++n)
	{
		SetSkillApplyData(ApplyDataList[n], RecvData->targets[n].get());
	}

	int32 ApplyCount = ApplyDataList.Num();
	for (auto& ApplyData : ApplyDataList)
	{
		if(false == ApplyData.Target.IsValid())
			continue;

		Target = ApplyData.Target.Get();
		if (0 < ApplyData.ApplySlots.Num())
		{
			TargetSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Target);
			if (TargetSkillComp)
			{
				if (0 < ApplyData.ApplySlots.Num())
				{
					TargetSkillComp->UpdateRealTimeSkillSlot(ApplyData);
				}
			}
		}

		if (0 < ApplyData.Appliers.Num())
		{
			TargetBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
			if (TargetBattleComp)
			{
				for (auto& ApplierData : ApplyData.Appliers)
				{
					if (EBattleDamageResultEnum::VE_NoneHitDamage == ApplierData.DamageResult ||
						EBattleDamageResultEnum::VE_Heal == ApplierData.DamageResult)
					{
						AttackData.Clear();
						AttackData.Target = Target;
						AttackData.HitResult = ApplierData.DamageResult;
						AttackData.HitDamage = ApplierData.Damage;
						AttackData.SyncHP = ApplierData.SyncHP;

						TargetBattleComp->CSOnDamage(Caster, AttackData);
					}
				}
			}
		}
	}
}

void UNetSkillManager::RecvRTS_UpdateSkillCool(OVERLORD::RTS::S2C::ACK_BT_SKILL_ACTIVATEDT* RecvData)
{
	TWeakObjectPtr<AActor> CastActor = nullptr;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		CastActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->actorUK);
	}
	if (CastActor.IsValid() == false)
		return;

	AActor* Caster = CastActor.Get();

	USkillEntityComponent* CaterSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (nullptr == CaterSkillComp)
		return;

	UISkill_V2* CastingSkill = CaterSkillComp->GetSkillByCategory_V2((ESkillCategoryTypeEnum)(RecvData->slot));
	if (nullptr == CastingSkill)
		return;

	CastingSkill->SkillSyncData.isCooling = false;
}

void UNetSkillManager::RecvRTS_SkillBlocked(OVERLORD::RTS::S2C::ACK_BT_SKILL_BLOCKEDT* RecvData)
{
	TWeakObjectPtr<AActor> CastActor = nullptr;//RGameInstance->GameObjectMgr->GetObjectByPlayerUK(RecvData->actorUK);
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		CastActor = pGamePlayMode->GetObjectByPlayerUK(RecvData->actorUK);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		CastActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(RecvData->actorUK);
	}
	if (CastActor.IsValid() == false)
		return;

	AActor* Caster = CastActor.Get();

	USkillEntityComponent* CaterSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (nullptr == CaterSkillComp)
		return;

	UISkill_V2* CastingSkill = CaterSkillComp->GetSkillByCategory_V2((ESkillCategoryTypeEnum)(RecvData->slot));
	if (nullptr == CastingSkill)
		return;

	UISkill_V2* CurrentSkill = CaterSkillComp->GetCurrentSkill_V2();
	if (CurrentSkill && CastingSkill == CurrentSkill)
	{
		CaterSkillComp->EndSkillActionForced_V2();
		CastingSkill->DecreaseCoolTime((float)(RecvData->coolbonusMSec * 0.001f));
	}
}


bool UNetSkillManager::SendRTS_CastSkill(UISkill_V2* Skill, AActor* Caster, AActor* ActorTarget, FVector LocationTarget)
{
// 	if (false == IsValid(RGameInstance))
// 		return false;

	if (false == IsValid(Skill) || false == IsValid(Caster))
		return false;

	FSkillTableInfo_V2* SkillInfo = Skill->GetSkillInfo();
	if (nullptr == SkillInfo)
		return false;

	auto Caster_BaseProp = Cast<IEntityBaseProperty>(Caster);
	if (nullptr == Caster_BaseProp)
		return false;

	auto Caster_SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster);
	if (nullptr == Caster_SkillComp)
		return false;

	int32 CasterUK = Caster_BaseProp->GetPlayerUK();
	int32 SkillCategory = (int32)(Skill->GetSkillCategoryType());

	int32 AimTargetUK = 0;
	FVector CasterLocation = Caster->GetActorLocation();
	FVector CastDirection = Caster->GetActorForwardVector();
	switch (SkillInfo->TargetType)
	{
	case ESkillTargetTypeEnum::VE_Me:
		AimTargetUK = CasterUK;
		CastDirection = LocationTarget - CasterLocation;
		LocationTarget = CasterLocation;
		break;

	case ESkillTargetTypeEnum::VE_Target:
		if (IsValid(ActorTarget))
		{
			auto Target_BaseProp = Cast<IEntityBaseProperty>(ActorTarget);
			if (Target_BaseProp)
			{
				AimTargetUK = Target_BaseProp->GetPlayerUK();
			}
			CastDirection = ActorTarget->GetActorLocation() - CasterLocation;
		}
		LocationTarget = FVector::ZeroVector;
		break;

	case ESkillTargetTypeEnum::VE_TargetLocation:
		CastDirection = LocationTarget - CasterLocation;
		break;
	}

	URTSManager::GetInstancePtr()->REQ_BT_SKILL_CAST(CasterUK, CasterLocation, SkillCategory, AimTargetUK, LocationTarget, CastDirection);

	if (ECharacterCategoryTypeEnum::VE_NeutralNpc != Caster_BaseProp->GetCharacterCategory())
	{
		SetReceiveWaitInfo(Caster_SkillComp);
	}

	return true;
}

void UNetSkillManager::SetSkillApplyData(FSkillSyncApplyData& ApplyData, OVERLORD::RTS::STRUCT::ACTOR_SNA_UPDATET* TargetInfo)
{
	ApplyData.Clear();

	if (nullptr == TargetInfo)
		return;

	int32 Count = 0;
	int32 LastIndex = 0;
	TWeakObjectPtr<AActor> TargetActor = nullptr;
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		TargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(TargetInfo->actorUK);
	}
	if (TargetActor.IsValid())
	{
		ApplyData.Target = TargetActor.Get();

		// SkillSlot
		Count = (int32)TargetInfo->slots.size();
		if (0 < Count)
		{
			ApplyData.ApplySlots.Init(EAbnormalSlotTypeEnum::VE_None, Count);
			ApplyData.RemoveSlots.Init(false, Count);
			for (int32 n = 0; n < Count; ++n)
			{
				ApplyData.ApplySlots[n] = (EAbnormalSlotTypeEnum)(TargetInfo->slots[n]->type);
				ApplyData.RemoveSlots[n] = TargetInfo->slots[n]->removed;

				//if (ApplyData.RemoveSlots[n])
				//{
				//	UE_LOG(LogBattle, Log, TEXT("=====>>>>> SetSkillApplyData - Remove Slot(%d)"), TargetInfo->slots[n]->type);
				//}
				//else
				//{
				//	UE_LOG(LogBattle, Log, TEXT("=====>>>>> SetSkillApplyData - Set Slot(%d)"), TargetInfo->slots[n]->type);
				//}
			}
		}

		// SlotApplier
		Count = (int32)TargetInfo->appliers.size();
		if (0 < Count)
		{
			ApplyData.Appliers.Init(FSkillSyncSlotApplierData(), Count);
			for (int32 n = 0; n < Count; ++n)
			{
				ApplyData.Appliers[n].Remove = TargetInfo->appliers[n]->removed;
				
				ApplyData.Appliers[n].SlotApplier.SkillApplierType = (ESkillApplierTypeEnum)(TargetInfo->appliers[n]->EF);
				ApplyData.Appliers[n].SlotApplier.TargetPercent = TargetInfo->appliers[n]->PE;
				ApplyData.Appliers[n].SlotApplier.TargetValue = TargetInfo->appliers[n]->VA;

				if(TargetInfo->appliers[n]->hit)
				{
					ApplyData.Appliers[n].SyncHP = TargetInfo->appliers[n]->hit->curHp;
					ApplyData.Appliers[n].DamageResult = (EBattleDamageResultEnum)(TargetInfo->appliers[n]->hit->type);
					ApplyData.Appliers[n].Damage = TargetInfo->appliers[n]->hit->damage;
				}

				if(TargetInfo->appliers[n]->move)
				{
					SetLocation(ApplyData.Appliers[n].TargetLoc, TargetInfo->appliers[n]->move->location.get());
					SetRotation(ApplyData.Appliers[n].TargetRot, TargetInfo->appliers[n]->move->direction.get());

					//DrawDebugBox(RGAMEINSTANCE(GEngine)->GetWorld(), ApplyData.Appliers[n].TargetLoc, FVector(20.f, 20.f, 20.f), FColor::Red, false, 2.f, 0, 10.f);
				}

				if (EBattleDamageResultEnum::VE_Dead == ApplyData.Appliers[n].DamageResult)
				{
					auto TargetActor_BaseProp = Cast<IEntityBaseProperty>(TargetActor.Get());
					if (TargetActor_BaseProp)
					{
#if LOG_NET_REALTIME
						UE_LOG(LogRTS, Log, TEXT("[UNetSkillManager] Dead >> Entity %d Medals %d ServerPlayerUK %d"), TargetActor_BaseProp->GetPlayerUK(), TargetInfo.EntityStatus.Medals, TargetInfo.PlayerUK);
#endif
						TargetActor_BaseProp->ReSetRealTimeParam();
					}
				}

				//if (ApplyData.RemoveAppliers[n])
				//{
				//	UE_LOG(LogBattle, Log, TEXT("=====>>>>> SetSkillApplyData - Remove Applier(%d)"), TargetInfo->appliers[n]->EF);
				//}
				//else
				//{
				//	UE_LOG(LogBattle, Log, TEXT("=====>>>>> SetSkillApplyData - Set Applier(%d)"), TargetInfo->appliers[n]->EF);
				//}
			}
		}
	}
}

void UNetSkillManager::SetLocation(FVector& Location, OVERLORD::RTS::STRUCT::RTS_VECTORT* RecvLocation)
{
	if (RecvLocation)
	{
		Location = FVector(RecvLocation->X, RecvLocation->Y, RecvLocation->Z);
	}
	else
	{
		Location = FVector::ZeroVector;
	}
}

void UNetSkillManager::SetRotation(FRotator& Rotation, OVERLORD::RTS::STRUCT::RTS_VECTORT* RecvDirection)
{
	if (RecvDirection)
	{
		FVector Dir = FVector(RecvDirection->X, RecvDirection->Y, RecvDirection->Z);
		Rotation = Dir.Rotation();
	}
	else
	{
		Rotation = FRotator::ZeroRotator;
	}
}

bool UNetSkillManager::CheckCasterCondition(AActor* Caster)
{
	if (Caster)
	{
		auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster);
		if (IsValid(BattleEntityComp))
		{
			return BattleEntityComp->IsAbleToUseSkill();
		}
	}
	return false;
}

void UNetSkillManager::CheckReceiveWaitTime(float DeltaTime)
{
	if (0 < RecvWaitCount)
	{
		for (FRecvWaitData& RecvWait : RecvWaitDataList)
		{
			if (RecvWait.SendSkillEntityComp.IsValid())
			{
				if (RecvWait.SendSkillEntityComp->IsSkillActing())
				{
					RecvWait.SendElapsedTime += DeltaTime;
					if (RecvWait.RecvWaitTime <= RecvWait.SendElapsedTime)
					{
						RecvWait.SendSkillEntityComp->SetNetSkillSending(false);
						RecvWait.Clear(RecvWaitCount);
					}
				}
				else
				{
					RecvWait.Clear(RecvWaitCount);
				}
			}
		}
	}
}

void UNetSkillManager::SetReceiveWaitInfo(USkillEntityComponent* CasterSkillEntityComp)
{
	if (IsValid(CasterSkillEntityComp))
	{
		CasterSkillEntityComp->SetNetSkillSending(true);

		bool bSet = false;
		for (FRecvWaitData& RecvWait : RecvWaitDataList)
		{
			if (false == RecvWait.SendSkillEntityComp.IsValid())
			{
				RecvWait.SendSkillEntityComp = CasterSkillEntityComp;
				RecvWait.SendElapsedTime = 0.0f;
				RecvWait.RecvWaitTime = RECEIVE_MAX_WAIT_TIME;

				RecvWaitCount++;
				bSet = true;
				break;
			}
		}

		if (false == bSet)
		{
			FRecvWaitData RecvWait;
			RecvWait.SendSkillEntityComp = CasterSkillEntityComp;
			RecvWait.SendElapsedTime = 0.0f;
			RecvWait.RecvWaitTime = RECEIVE_MAX_WAIT_TIME;

			RecvWaitDataList.Add(RecvWait);
			RecvWaitCount++;
		}
	}
}

void UNetSkillManager::ClearReceiveWaitInfo(USkillEntityComponent* CasterSkillEntityComp)
{
	if (CasterSkillEntityComp && 0 < RecvWaitCount)
	{
		for (FRecvWaitData& RecvWait : RecvWaitDataList)
		{
			if (RecvWait.SendSkillEntityComp.Get() == CasterSkillEntityComp)
			{
				CasterSkillEntityComp->SetNetSkillSending(false);

				RecvWait.Clear(RecvWaitCount);
				return;
			}
		}
	}
}