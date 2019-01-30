// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BattleBaseAnimInstance.h"
#include "Character/AnimInstance/AnimInstanceDefs.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/WaypointComponent.h"
#include "Character/SharedBattle/BattleInfoBlackboard.h"
#include "GameState/Stage/Spawner/LevelActor.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimMontage.h"
#include "ReferenceSkeleton.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Modes/StageGameMode.h"
#include "ContentGameMode/Modes/TestGameMode.h"


#include "Controllers/DPadController.h"
#include "UtilFunctionIntegrated.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Animation/AnimNode_AssetPlayerBase.h"
#include "GlobalIntegrated.h"
#include "Animation/AnimInstanceProxy.h"
#include "DirectionTrackManager.h"



UBattleBaseAnimInstance::UBattleBaseAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	
	
	
	
}

void UBattleBaseAnimInstance::PostLoad()
{
	Super::PostLoad();

////#if !UE_BUILD_SHIPPING
//	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
//	if ( IsValid(RGameInstance->ResourceCacheManager))
//	{
//		if (RGameInstance->ResourceCacheManager->IsEnabledCache())
//			CacheAnimMontageFile();
//	}
////#endif
}

void UBattleBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RootMotionMode = ERootMotionMode::RootMotionFromEverything;

	AddRotatorValue = FRotator::ZeroRotator;

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	if (IsValid(BasePlayerController))
	{
		if(BasePlayerController->GetIsWangRealMode())
		{
			WangRealMode();
		}
	}

	SetBasicIdleAnimSequence();
}

void UBattleBaseAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);
}

void UBattleBaseAnimInstance::NativeUninitializeAnimation()
{
	OwnerCharacterInfo = nullptr;
	Super::NativeUninitializeAnimation();
}

AActor* UBattleBaseAnimInstance::GetOwnerCharacter()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn))
	{
		CheckOwnerCharacterInfo(OwnerPawn);
		
		return OwnerPawn;
	}

	return nullptr;
}

void UBattleBaseAnimInstance::CheckOwnerCharacterInfo(AActor* InOwnerActor)
{
	if (OwnerCharacterInfo != nullptr)
		return;
	if (false == IsValid(InOwnerActor))
		return;
	IEntityBaseProperty* OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(InOwnerActor);
	if (OwnerActor_BaseProperty == nullptr)
		return;

	OwnerCharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(OwnerActor_BaseProperty->GetCharacterCategory(), OwnerActor_BaseProperty->GetCharacterID());
}


UAnimMontage* UBattleBaseAnimInstance::GetAnimMontageFromAnimMontageType(EAnimMontageType AnimMontageType)
{
	UAnimMontage* AnimMontage = nullptr;

	switch (AnimMontageType)
	{
	case EAnimMontageType::VE_Idle:
		AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageIdle);
		break;
	case EAnimMontageType::VE_Run:
		AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageRun);
		break;
	case EAnimMontageType::VE_Attack:
		AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageAttack);
		break;
	case EAnimMontageType::VE_Skill:
		AnimMontage = GetSkillAnimMontage();
		break;
	case EAnimMontageType::VE_Abnormal:
		AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageAbnormal);
		break;
	case EAnimMontageType::VE_Event:
		AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageEvent);
		break;
	case EAnimMontageType::VE_DamageResult:
		AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageDamageResult);
		break;
	}

	return AnimMontage;
}

UAnimMontage* UBattleBaseAnimInstance::GetSkillAnimMontage()
{
	UAnimMontage* AnimMontage = nullptr;

	if (AnimMontageSkillMap.Contains(SkillAniType))
	{
		AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageSkillMap[SkillAniType]);
	}

	return AnimMontage;
}

void UBattleBaseAnimInstance::CacheAnimMontageFile()
{
	UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageIdle);
	UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageRun);
	UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageAttack);
	GetSkillAnimMontage();
	UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageAbnormal);
	UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageEvent);
	UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageDamageResult);	
}

//////////////////////////////////////////////////////////////////////////

bool UBattleBaseAnimInstance::SetAnimState(EAnimStateEnum InAnimState)
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return false;

	if (AnimState == InAnimState)
		return false;

	PrevAnimState = AnimState;
	AnimState = InAnimState;

	FText PrevAnimStateName = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EAnimStateEnum"), (int32)PrevAnimState);
	FText AnimStateName = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EAnimStateEnum"), (int32)AnimState);
	//UE_LOG(LogBattle, Log, TEXT("LOG_ANIMSTATE > %s >> UBattleBaseAnimInstance::SetAnimState >>> %s -> %s"), *(OwnerCharacter->GetName()), *(PrevAnimStateName.ToString()), *(AnimStateName.ToString()));

	//////////////////////////////////////////////////////////////////////////
	// AnimMontage
	if (AnimState == EAnimStateEnum::VE_Idle)
	{
		PlayAnimMontageIdle();
	}
	else if (AnimState == EAnimStateEnum::VE_Run)
	{
		PlayAnimMontageRun();
	}
	else if (AnimState == EAnimStateEnum::VE_Die || AnimState == EAnimStateEnum::VE_Spawn)
	{
		PlayAnimMontageEvent();
	}

	return true;
}

void UBattleBaseAnimInstance::SetNormalAttackType(ENormalAttackTypeEnum InNormalAttackType)
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter))
		//UE_LOG(LogBattle, Log, TEXT("LOG_ANIMSTATE > %s >> UBattleBaseAnimInstance::SetNormalAttackType - ENormalAttackTypeEnum %d"), *(OwnerCharacter->GetName()), (int32)InNormalAttackType);

	RenewalNormalAttack = false;
	NormalAttackType = InNormalAttackType;

	PlayAnimMontageNormalAttack();
}

void UBattleBaseAnimInstance::SetNormalAttackAnim(int32 InNormalAttackAnim)
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter))
		//UE_LOG(LogBattle, Log, TEXT("LOG_ANIMSTATE > %s >> UBattleBaseAnimInstance::SetNormalAttackAnim - ENormalAttackTypeEnum %d"), *(OwnerCharacter->GetName()), InNormalAttackAnim);

	RenewalNormalAttack = true;

	switch (InNormalAttackAnim)
	{
	case 0:	NormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack01;	break;
	case 1:	NormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack02;	break;
	case 2:	NormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack03;	break;
	case 3:	NormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack04;	break;
	case 4:	NormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack05;	break;
	default:NormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack01;	break;
	}
	PlayAnimMontageNormalAttack();
}

void UBattleBaseAnimInstance::SetSkillCasting(bool InSkillCasting)
{
	IsCasting = InSkillCasting;
	if (InSkillCasting)
	{
		bSkillActing = false;
		PlayAnimMontageSkillAttack(ESkillAnimStateType::VE_SkillCasting, SkillAniType);
	}
}

void UBattleBaseAnimInstance::SetSkillActing(bool InSkillActing)
{
	bSkillActing = InSkillActing;
	if (bSkillActing)
	{
		IsCasting = false;
		PlayAnimMontageSkillAttack(ESkillAnimStateType::VE_SkillAction, SkillAniType);
	}
}

void UBattleBaseAnimInstance::SetSkillActingEnd(ESkillAniTypeEnum InSkillAniType)
{
	IsCasting = false;
	bSkillActing = false;
	PlayAnimMontageSkillAttack(ESkillAnimStateType::VE_SkillActionEnd, InSkillAniType);
}

void UBattleBaseAnimInstance::SetAbnormalType(EAbonormalStateFlagEnum InAbonormalStateFlag)
{
	EAbonormalStateFlagEnum OldAbonormalStateFlag = AbonormalStateFlag;
	AbonormalStateFlag = InAbonormalStateFlag;

	AActor* actor = GetOwnerCharacter();
	if (IsValid(actor))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
		RGameInstance->EventManager->OnBuffEnter.Broadcast(GetOwnerCharacter(), (int32)AbonormalStateFlag);
	}

	PlayAnimMontageAbnormal(AbonormalStateFlag, OldAbonormalStateFlag);
}

//////////////////////////////////////////////////////////////////////////


void UBattleBaseAnimInstance::SetFlyState(bool InState)
{
	bool bChangeBasicIdleAnim = (FlyState != InState);
	
	FlyState = InState;

	if (bChangeBasicIdleAnim)
	{
		SetBasicIdleAnimSequence();
		if (AnimState == EAnimStateEnum::VE_Idle)
		{
			PlayAnimMontageIdle();
		}
	}
}

void UBattleBaseAnimInstance::SetAnimPlayRate(float InAnimPlayRate)
{
	if (IsLimitPlayRate && InAnimPlayRate < 0.25f)
	{
		InAnimPlayRate = 0.25f;
	}

	AnimPlayRate = InAnimPlayRate;
}

void UBattleBaseAnimInstance::SetAttackAnimPlayRate(float InAnimPlayRate)
{
	if (IsLimitPlayRate && InAnimPlayRate < 0.25f)
	{
		InAnimPlayRate = 0.25f;
	}
	
	if (AttackAnimPlayRate != InAnimPlayRate)
	{
		AttackAnimPlayRate = InAnimPlayRate;	
	}
}

void UBattleBaseAnimInstance::SetMoveAnimPlayRate(float InAnimPlayRate)
{
	if (IsLimitPlayRate && InAnimPlayRate < 0.25f)
	{
		InAnimPlayRate = 0.25f;
	}

	if (MoveAnimPlayRate != InAnimPlayRate)
	{
		MoveAnimPlayRate = InAnimPlayRate;

		if (AnimState == EAnimStateEnum::VE_Run)
		{
			PlayAnimMontageRun();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void UBattleBaseAnimInstance::SetStateMachineAnimSequence(UAnimMontage* AnimMontage, FName InSlotName, FName InSectionName, bool InLoop)
{
	if (false == IsValid(AnimMontage))
		return;

	if (StateMachineAnimSectionName.IsEqual(InSectionName))
		return;

	int32 SectionIndex = AnimMontage->GetSectionIndex(InSectionName);
	const FAnimTrack* BasicAnimTrack = AnimMontage->GetAnimationData(InSlotName);
	if (BasicAnimTrack)
	{
		if (BasicAnimTrack->AnimSegments.IsValidIndex(SectionIndex))
		{
			StateMachineAnimSequence = BasicAnimTrack->AnimSegments[SectionIndex].AnimReference;
			StateMachineAnimSectionName = InSectionName;
			StateMachineAnimLoop = InLoop;
			AActor* OwnerCharacter = GetOwnerCharacter();
			if (IsValid(OwnerCharacter))
			{
				//UE_LOG(LogBattle, Log, TEXT("LOG_ANIMSTATE > %s >> UBattleBaseAnimInstance::SetStateMachineAnimSequence >>> %s %s"), *(OwnerCharacter->GetName()), *(StateMachineAnimSequence->GetName()), *(StateMachineAnimSectionName.ToString()));
			}
		}
	}
}

void UBattleBaseAnimInstance::SetBasicIdleAnimSequence()
{
	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Idle);
	if (IsValid(AnimMontage))
	{
		FString SectionString = ANIM_MONTAGE_BASIC_IDLE;
		AddStringFromMontageState(SectionString);
		SetStateMachineAnimSequence(AnimMontage, MONTAGE_SLOT_BASIC, FName(*SectionString), true);
	}
}

void UBattleBaseAnimInstance::SetDieAnimSequence(UAnimMontage* AnimMontage, FName InSlotName, FName InSectionName)
{
	if (false == IsValid(AnimMontage))
		return;

	int32 SectionIndex = AnimMontage->GetSectionIndex(InSectionName);
	const FAnimTrack* BasicAnimTrack = AnimMontage->GetAnimationData(InSlotName);
	if (BasicAnimTrack)
	{
		if (BasicAnimTrack->AnimSegments.IsValidIndex(SectionIndex))
		{
			DieAnimSequence = BasicAnimTrack->AnimSegments[SectionIndex].AnimReference;
			
			if (DieAnimSequence)
			{
				for (size_t i = 0; i < DieAnimSequence->Notifies.Num(); ++i)
				{
					if (DieAnimSequence->Notifies[i].NotifyName == TEXT("EndDeadAni"))
					{
						if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_Stage_BossDead ||
							UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_Stage_FailEvent)
						{
							UStageDirectionState* SDS = Cast<UStageDirectionState>(UUtilFunctionLibrary::GetDirectionTrackManager()->GetDirectionState(EDirectionStateTypeEnum::VE_StageDirection));
							if(SDS)
								SDS->BossDeadHelper.Duration = DieAnimSequence->Notifies[i].GetTime(EAnimLinkMethod::Absolute);
						}
						else if(UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_RTS_CS_PLAYLOSE)
						{
							URTSDirectionState* RDS = Cast<URTSDirectionState>(UUtilFunctionLibrary::GetDirectionTrackManager()->GetDirectionState(EDirectionStateTypeEnum::VE_RTSDirection));
							if (RDS)
							{
								RDS->ColosseumEndHelper.Duration = DieAnimSequence->Notifies[i].GetTime(EAnimLinkMethod::Absolute);
							}
						}
						
					}
				}
			}

			StateMachineAnimSequence = nullptr;
			StateMachineAnimSectionName = NAME_None;
			StateMachineAnimLoop = false;
			OwnerCharacterInfo = nullptr;

			AActor* OwnerCharacter = GetOwnerCharacter();
			if (IsValid(OwnerCharacter))
			{
				//UE_LOG(LogBattle, Log, TEXT("LOG_ANIMSTATE > %s >> UBattleBaseAnimInstance::SetDieAnimSequence >>> %s %s"), *(OwnerCharacter->GetName()), *(DieAnimSequence->GetName()), *(InSectionName.ToString()));
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// AnimMontage

void UBattleBaseAnimInstance::PlayAnimMontageIdle()
{
	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Idle);
	if (IsValid(AnimMontage))
	{
		if (PrevAnimState == EAnimStateEnum::VE_SkillAttack)
		{
			if (!PlayActionMontageName.IsNone())
			{
				if (PlayActionMontageName.ToString().Contains(ANIM_MONTAGE_SKILL_ACTION_END))
					return;
			}
		}

		FString SectionString = ANIM_MONTAGE_BASIC_IDLE;
		AddStringFromMontageState(SectionString);
		PlayAnimMontageToSectionString(AnimMontage, SectionString, MoveAnimPlayRate);
	}
}

void UBattleBaseAnimInstance::PlayAnimMontageRun()
{
	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Run);
	if (IsValid(AnimMontage))
	{
		FString SectionString = ANIM_MONTAGE_BASIC_RUN;
		AddStringFromMontageState(SectionString);
		PlayAnimMontageToSectionString(AnimMontage, SectionString, MoveAnimPlayRate);
	}
}

void UBattleBaseAnimInstance::PlayAnimMontageNormalAttack()
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return;

	if (AnimState != EAnimStateEnum::VE_NormalAttack)
		return;

	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Attack);
	if (false == IsValid(AnimMontage))
		return;


	FString AddNumString;
	switch (NormalAttackType)
	{
	case ENormalAttackTypeEnum::VE_NormalAttack01:	AddNumString = "01";	break;
	case ENormalAttackTypeEnum::VE_NormalAttack02:	AddNumString = "02";	break;
	case ENormalAttackTypeEnum::VE_NormalAttack03:	AddNumString = "03";	break;
	case ENormalAttackTypeEnum::VE_NormalAttack04:	AddNumString = "04";	break;
	case ENormalAttackTypeEnum::VE_NormalAttack05:	AddNumString = "05";	break;
	}
	if (AddNumString.IsEmpty())
		return;
	
	FString SectionString = ANIM_MONTAGE_ATTACK + AddNumString;
	AddStringFromMontageState(SectionString);


	//UE_LOG(LogBattle, Log, TEXT("LOG_ANIMSTATE > %s >> UBattleBaseAnimInstance::PlayAnimMontageNormalAttack >>> %s %s"), *(OwnerCharacter->GetName()), *(AnimMontage->GetName()), *SectionString);

	PlayAnimMontageToSectionString(AnimMontage, SectionString, AttackAnimPlayRate);

	
	if (OwnerCharacterInfo->IsHero == false)
		return;

	UAnimMontage* NormalAttackAnim = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Attack);
	if (NormalAttackAnim)
	{
		TArray<FCompositeSection> SectionArray = NormalAttackAnim->CompositeSections;
		for (size_t i = 0; i < SectionArray.Num(); ++i)
		{
			FName CurrentName = SectionArray[i].SectionName;
			FName NextName = SectionArray[i].NextSectionName;
			FString NextStr = TEXT("");
			if (i == SectionArray.Num() - 1)
				NextStr = TEXT("Attack01");
			else if (NextName == TEXT("Idle") || NextName.IsNone())
				NextStr = TEXT("Attack0") + FString::FromInt(i + 2);
			else
				continue;
		
			Montage_SetNextSection(CurrentName, FName(*NextStr), NormalAttackAnim);
		}
	}

}

void UBattleBaseAnimInstance::PlayAnimMontageSkillAttack(ESkillAnimStateType InSkillAnimState, ESkillAniTypeEnum InSkillAniType)
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return;

	if (AnimState != EAnimStateEnum::VE_SkillAttack)
		return;
	if (SkillAniType != InSkillAniType)
		return;
	if (SkillAniType == ESkillAniTypeEnum::VE_None)
		return;


	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Skill);
	if (false == IsValid(AnimMontage))
		return;

	FString SectionString;
	switch (InSkillAnimState)
	{
	case ESkillAnimStateType::VE_SkillCasting:		SectionString = ANIM_MONTAGE_SKILL_CASTING;		break;
	case ESkillAnimStateType::VE_SkillAction:		SectionString = ANIM_MONTAGE_SKILL_ACTION;		break;
	case ESkillAnimStateType::VE_SkillActionEnd:	SectionString = ANIM_MONTAGE_SKILL_ACTION_END;	break;
	}

	if (ESkillAnimStateType::VE_SkillActionEnd == InSkillAnimState)
	{
		FName SectionName = FName(*SectionString);
		if (AnimMontage->IsValidSectionName(SectionName))
		{
			PlayAnimMontageToSectionName(AnimMontage, SectionName, AnimPlayRate);
		}
		else
		{
			if (Montage_IsPlaying(AnimMontage))
			{
				SectionString = ANIM_MONTAGE_SKILL_ACTION;
				SectionName = FName(*SectionString);
				StopAnimMontageToSectionName(AnimMontage, SectionName);
			}
		}
	}
	else
	{
		PlayAnimMontageToSectionString(AnimMontage, SectionString, AnimPlayRate);
	}
}

void UBattleBaseAnimInstance::PlayAnimMontageAbnormal(EAbonormalStateFlagEnum NewFlag, EAbonormalStateFlagEnum OldFlag)
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return;

	if (AnimState != EAnimStateEnum::VE_Abnormal)
		return;

	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Abnormal);
	if (false == IsValid(AnimMontage))
		return;

	bool NewBuffPlay = true;
	FName SectionName;
	switch (NewFlag)
	{
	case EAbonormalStateFlagEnum::BM_Stun:		SectionName = ANIM_MONTAGE_ABNORMAL_STUN;		break;
	case EAbonormalStateFlagEnum::BM_Down:		SectionName = ANIM_MONTAGE_ABNORMAL_DOWNSTART;	break;
	case EAbonormalStateFlagEnum::BM_Push:		SectionName = ANIM_MONTAGE_ABNORMAL_PUSH;	break;
	case EAbonormalStateFlagEnum::BM_KnockBack: SectionName = ANIM_MONTAGE_ABNORMAL_KNOCKBACK; break;
	case EAbonormalStateFlagEnum::BM_Airborne:  SectionName = ANIM_MONTAGE_ABNORMAL_AIRSTART; break;

	case EAbonormalStateFlagEnum::BM_None:
	{
		switch (OldFlag)
		{
		case EAbonormalStateFlagEnum::BM_Stun:	SectionName = ANIM_MONTAGE_ABNORMAL_STUN;		NewBuffPlay = false;	break;
		}
	}
	break;
	case EAbonormalStateFlagEnum::BM_Fear:
		return;
	}
	
	if (SectionName.IsNone())
		return;


	if (NewBuffPlay)
	{
		PlayAnimMontageToSectionName(AnimMontage, SectionName, AnimPlayRate);
	}
	else
	{
		if (Montage_IsPlaying(AnimMontage))
		{
			StopAnimMontageToSectionName(AnimMontage, SectionName);
		}
	}
}

void UBattleBaseAnimInstance::PlayAnimMontageFromDamageResultType(EBattleDamageResultEnum DamageResultType)
{
	if (AnimState != EAnimStateEnum::VE_Abnormal && AnimState != EAnimStateEnum::VE_Run && AnimState != EAnimStateEnum::VE_Idle)
		return;

	if (AnimState == EAnimStateEnum::VE_Abnormal)
	{
		//if (EBattleDamageResultEnum::VE_Hit != DamageResultType && EBattleDamageResultEnum::VE_Critical != DamageResultType)
		//{
			return;
		//}
	}

	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return;

	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_DamageResult);
	if (false == IsValid(AnimMontage))
		return;

	if (Montage_IsPlaying(AnimMontage))
		return;

	FString SectionString;
	switch (DamageResultType)
	{
	case EBattleDamageResultEnum::VE_Hit:
	case EBattleDamageResultEnum::VE_Critical:
//	case EBattleDamageResultEnum::VE_MagicCritical:
	{
		SectionString = ANIM_MONTAGE_DAMAGERESULT_HIT;
	}break;
	case EBattleDamageResultEnum::VE_Dodge:
	{
		SectionString = ANIM_MONTAGE_DAMAGERESULT_DODGE;
	}break;
// 	case EBattleDamageResultEnum::VE_Parry:
// 	{
// 		SectionString = ANIM_MONTAGE_DAMAGERESULT_PARRY;
// 	}break;
	}
	if (SectionString.IsEmpty())
		return;

	AddStringFromMontageState(SectionString);
	PlayAnimMontageToSectionString(AnimMontage, SectionString, AnimPlayRate, false);
}

void UBattleBaseAnimInstance::PlayAnimMontageEvent()
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return;

	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Event);
	if (false == IsValid(AnimMontage))
		return;

	switch (AnimState)
	{
		case EAnimStateEnum::VE_Die:
		{
			StopAllMontages(0.0f);

			FName AnimSlotName = MONTAGE_SLOT_EVENT;
			FText AnimSectionText = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EAnimMontageEventDie"), (int32)OwnerCharacterInfo->DeadType);

			FName AnimSectionName = FName(*AnimSectionText.ToString());

			FString		DefaultAnim = TEXT("Die01");

			if (AnimMontage->IsValidSectionName(AnimSectionName) == false)
				AnimSectionName = FName(*(DefaultAnim));

			if (OwnerCharacterInfo)
			{
				if (/*OwnerCharacterInfo->DeadType && */IsForceDie2)
				{
					auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter);
					if (OwnerActor_BattleEntityComponent)
					{
						auto BattleInfoBlackboard = OwnerActor_BattleEntityComponent->GetBattleInfoBlackboard();
						if (BattleInfoBlackboard)
						{
							UCharacterHelpFunctionLibrary::LaunchCharacter(OwnerCharacter, BattleInfoBlackboard->LastAttackCharacter.Get(), true, false);
							//AnimSectionName = ANIM_MONTAGE_EVENT_DIE_02;
						}
					}
				}
			}

			SetDieAnimSequence(AnimMontage, AnimSlotName, AnimSectionName);

		}break;
		case EAnimStateEnum::VE_Spawn:
		{
			FString AddNumString;
			switch (OwnerCharacterInfo->SpawnType)
			{
			case ESpawnTypeEnum::VE_Spawn01:	AddNumString = "01";	break;
			case ESpawnTypeEnum::VE_Spawn02:	AddNumString = "02";	break;
			case ESpawnTypeEnum::VE_Spawn03:	AddNumString = "03";	break;
			case ESpawnTypeEnum::VE_Spawn04:	AddNumString = "04";	break;
			}
			if (false == AddNumString.IsEmpty())
			{
				FString SectionString = ANIM_MONTAGE_EVENT_SPAWN + AddNumString;
				PlayAnimMontageToSectionString(AnimMontage, SectionString, AnimPlayRate);
			}
		}break;
	}
}


void UBattleBaseAnimInstance::PauseAnimMontageNormalAttack()
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return;

	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Attack);
	if (false == IsValid(AnimMontage))
		return;

	Montage_Pause(AnimMontage);
}

bool UBattleBaseAnimInstance::PlayAnimMontageEventSection(const FName& SectionName, FName NextSectionName)
{
	if (SectionName.IsNone())
		return false;

	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return false;

	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Event);
	if (false == IsValid(AnimMontage))
		return false;

	if (Montage_IsPlaying(AnimMontage))
	{
		FName CurrentSection = Montage_GetCurrentSection(AnimMontage);
		if (CurrentSection.IsEqual(SectionName))
			return false;
	}

	bool PlayResult = PlayAnimMontageToSectionName(AnimMontage, SectionName, AnimPlayRate);
	if (PlayResult)
	{
		if (false == NextSectionName.IsNone())
		{
			Montage_SetNextSection(SectionName, NextSectionName, AnimMontage);
		}
	}

	return PlayResult;
}

bool UBattleBaseAnimInstance::IsPlayingAnimMontageEventSection(const FName& SectionName)
{
	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Event);
	if (false == IsValid(AnimMontage))
		return false;

	return IsPlayingAnimMontageToSectionName(AnimMontage, SectionName);
}


//////////////////////////////////////////////////////////////////////////

bool UBattleBaseAnimInstance::IsPlayingSkillActingAnimMontage()
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	if (IsValid(OwnerCharacter) == false)
		return false;

	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Skill);
	if (IsValid(AnimMontage))
	{
		if (Montage_IsPlaying(AnimMontage))
		{
			if (bSkillActing)
			{
				return true;
			}
		}
	}

	return false;
}

void UBattleBaseAnimInstance::AddStringFromMontageState(FString& InString)
{
	if (FlyState)
	{
		InString += ANIM_MONTAGE_STATE_FLY;
	}
}

bool UBattleBaseAnimInstance::PlayAnimMontageToSectionString(UAnimMontage* AnimMontage, const FString& SectionString, float& PlayRate, bool ActionMontage)
{
	if (false == IsValid(AnimMontage))
		return false;
	if (SectionString.IsEmpty())
		return false;

	FName SectionName = FName(*SectionString);
	return PlayAnimMontageToSectionName(AnimMontage, SectionName, PlayRate, ActionMontage);
}

bool UBattleBaseAnimInstance::PlayAnimMontageToSectionName(UAnimMontage* AnimMontage, const FName& SectionName, float& PlayRate, bool ActionMontage)
{
	if (false == IsValid(AnimMontage))
		return false;
	if (SectionName.IsNone())
		return false;

	if (false == AnimMontage->IsValidSectionName(SectionName))
	{
		UE_LOG(LogBattle, Log, TEXT("NOT FOUND SECTION NAME >>> %s %s"), *(AnimMontage->GetName()), *(SectionName.ToString()));
		return false;
	}

	if (ActionMontage)
	{
		PlayActionMontageName = SectionName;
	}

	if (Montage_IsActive(AnimMontage))
	{
		Montage_SetPlayRate(AnimMontage, PlayRate);

		if (false == Montage_IsPlaying(AnimMontage))
		{
			Montage_Resume(AnimMontage);
		}
	}
	else
	{
		FAlphaBlend AlphaBlend;
		AlphaBlend.SetBlendTime(0.2f);
		AlphaBlend.SetBlendOption(EAlphaBlendOption::Linear);
		AnimMontage->BlendIn = AlphaBlend;
		AnimMontage->BlendOut = AlphaBlend;
		
		float AnimMontageSecond = Montage_Play(AnimMontage, PlayRate);

	}

	if (IsLimitPlayRate)
	{
		Montage_JumpToSection(SectionName, AnimMontage);
	}

	AActor* OwnerCharacter = GetOwnerCharacter();
	//if (IsValid(OwnerCharacter))
		//UE_LOG(LogBattle, Log, TEXT("LOG_ANIMSTATE > %s >> UBattleBaseAnimInstance::PlayAnimMontageToSectionName >>> %s %s"), *(OwnerCharacter->GetName()), *(AnimMontage->GetName()), *(SectionName.ToString()));
	return true;
}

bool UBattleBaseAnimInstance::StopAnimMontageToSectionName(UAnimMontage* AnimMontage, const FName& SectionName)
{
	if (IsPlayingAnimMontageToSectionName(AnimMontage, SectionName))
	{
		Montage_Stop(0.0f, AnimMontage);
		return true;
	}

	return false;
}

bool UBattleBaseAnimInstance::IsPlayingAnimMontageToSectionName(UAnimMontage* AnimMontage, const FName& SectionName)
{
	if (false == IsValid(AnimMontage))
		return false;
	if (SectionName.IsNone())
		return false;

	if (false == Montage_IsPlaying(AnimMontage))
		return false;
	if (false == AnimMontage->IsValidSectionName(SectionName))
		return false;

	FName CurrentSection = Montage_GetCurrentSection(AnimMontage);
	if (false == CurrentSection.IsEqual(SectionName))
		return false;

	return true;
}

float UBattleBaseAnimInstance::GetAnimMontageSectionLength(EAnimMontageType AnimMontageType, const FName& SectionName)
{
	UAnimMontage* AnimMontage = GetAnimMontageFromAnimMontageType(AnimMontageType);

	return GetAnimMontageSectionLength(AnimMontage, SectionName);
}

float UBattleBaseAnimInstance::GetAnimMontageSectionLength(UAnimMontage* AnimMontage, const FName& SectionName)
{
	float SectionLength = 0.f;

	if (IsValid(AnimMontage) && !SectionName.IsNone())
	{
		int32 SectionIndex = AnimMontage->GetSectionIndex(SectionName);
		if (INDEX_NONE < SectionIndex)
		{
			SectionLength = AnimMontage->GetSectionLength(SectionIndex);
		}
	}
	
	return SectionLength;
}

//////////////////////////////////////////////////////////////////////////
// AnimNotify

void UBattleBaseAnimInstance::BeginAttackState(ENormalAttackTypeEnum CheckAttackType)
{
	if (CheckAttackType == NormalAttackType ||
		CheckAttackType == ENormalAttackTypeEnum::VE_NormalAttackNone)
	{
		OnAttackStateDelegate.Broadcast(CheckAttackType, true);
	}
}

void UBattleBaseAnimInstance::EndAttackState(ENormalAttackTypeEnum CheckAttackType)
{
	if (CheckAttackType == NormalAttackType ||
		CheckAttackType == ENormalAttackTypeEnum::VE_NormalAttackNone)
	{
		OnAttackStateDelegate.Broadcast(CheckAttackType, false);
	}

}

void UBattleBaseAnimInstance::BeginUnconditionalExecuteAttackState(ENormalAttackTypeEnum CheckAttackType)
{
	if (CheckAttackType == NormalAttackType || CheckAttackType == ENormalAttackTypeEnum::VE_NormalAttackNone)
	{
		OnnUnconditionalExecuteAttackStateDelegate.Broadcast(CheckAttackType, true);
	}
}

void UBattleBaseAnimInstance::EndUnconditionalExecuteAttackState(ENormalAttackTypeEnum CheckAttackType)
{
	if (CheckAttackType == NormalAttackType ||
		CheckAttackType == ENormalAttackTypeEnum::VE_NormalAttackNone)
	{
		OnnUnconditionalExecuteAttackStateDelegate.Broadcast(CheckAttackType, false);
	}
}

void UBattleBaseAnimInstance::BeginSkillAniState()
{
	
	AActor* Owner = GetOwnerCharacter();

	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Owner);
	if (SkillComponent)
	{
		UISkill_V2* Skill = SkillComponent->GetCurrentSkill_V2();
		if (!Skill)
			return;

		ESkillCategoryTypeEnum CurrentSkillCategory = SkillComponent->GetCurrentSkill_V2()->GetSkillCategoryType();
		if (CurrentSkillCategory == ESkillCategoryTypeEnum::VE_ActiveSkill03)
		{
			/*Bryan added Temp*/
			ATestGameMode* TestGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(Owner->GetWorld()));
			if (!TestGameMode->IsValidLowLevel())
			{
				AStageGameMode* StageGameMode = Cast<AStageGameMode>(UGameplayStatics::GetGameMode(Owner->GetWorld()));
				if (!StageGameMode)
					return;
			}


			URGameInstance* RGameInst = RGAMEINSTANCE(this);
			if (!RGameInst)
				return;

			if (!RGameInst->OptionManager->IsUltimateMatineeView())
				return;

			if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_Stage_BossDead)
				return;

			IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Owner);
			if (BaseProperty != nullptr)
			{
				if (BaseProperty->GetPartyID() != PartyID_PC_MY)
					return;
			}

			FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(BaseProperty->GetCharacterCategory(), BaseProperty->GetCharacterID());
			FString SequenceName = CharacterInfo->DisplayName.ToString() + "_Ultimate_Sequence";
			RGameInst->SequenceManager->PlaySequenceAtTargetActorLocation(FName(*SequenceName), ESequenceActorType::VE_UltimateSkill, Owner);

			UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CombineEffect);

			ALevelSequenceActor* SeqActor = RGameInst->SequenceManager->GetCurrentSequenceActor();
			if (IsValid(SeqActor))
			{
				SeqActor->CustomTimeDilation = Owner->CustomTimeDilation;
				SeqActor->PlaybackSettings.PlayRate = Owner->CustomTimeDilation;
			}
		}
	}

	

	
	
}

void UBattleBaseAnimInstance::EndSkillAniState()
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter);
	if (OwnerActor_SkillEntityComponent)
	{
		OwnerActor_SkillEntityComponent->EndSkillAction_V2();
	}

	OnEndSkillAniDelegate.Broadcast();
}

void UBattleBaseAnimInstance::BeginSkillApply()
{
	AActor* BattleBaseCharacter = GetOwnerCharacter();
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BattleBaseCharacter);
	if (OwnerActor_SkillEntityComponent)
		OwnerActor_SkillEntityComponent->StartSkillApply();
}

void UBattleBaseAnimInstance::EndSkillApply()
{
	AActor* BattleBaseCharacter = GetOwnerCharacter();
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BattleBaseCharacter);
	if (OwnerActor_SkillEntityComponent)
		OwnerActor_SkillEntityComponent->EndSkillApply();
}

void UBattleBaseAnimInstance::EnterAbnormalStateAni()
{

}
void UBattleBaseAnimInstance::EndAbnormalStateAni()
{

}

void UBattleBaseAnimInstance::HitTarget()
{
	if (RenewalNormalAttack)
		return;

	AActor* OwnerCharacter = GetOwnerCharacter();
	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter);
	if (OwnerActor_BattleEntityComponent)
	{
		OwnerActor_BattleEntityComponent->HitTarget();
	}
}

void UBattleBaseAnimInstance::HitTargetDummy()
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter);
	if (OwnerActor_BattleEntityComponent)
	{
		if (OwnerActor_BattleEntityComponent->CurrentAttackTarget.IsValid())
		{
			AActor* TargetActor = OwnerActor_BattleEntityComponent->CurrentAttackTarget.Get();
			OwnerActor_BattleEntityComponent->SpawnHitEffect(EBattleDamageResultEnum::VE_Hit, TargetActor, false);
		}
	}
}

void UBattleBaseAnimInstance::ShotProjectile(EBulltSocketType ShootPoint)
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter);
	if (OwnerActor_BattleEntityComponent)
	{
		OwnerActor_BattleEntityComponent->SetShootSocketPoint(ShootPoint);
		if (false == RenewalNormalAttack)
		{
			OwnerActor_BattleEntityComponent->ShotProjectile();
		}
	}
}

void UBattleBaseAnimInstance::SkillCastingNotify()
{
	AActor* BattleBaseCharacter = GetOwnerCharacter();
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BattleBaseCharacter);
	if (OwnerActor_SkillEntityComponent)
	{
		OwnerActor_SkillEntityComponent->CastingActionSkill_V2();
	}

}

void UBattleBaseAnimInstance::SkillFireNotify()
{
	AActor* BattleBaseCharacter = GetOwnerCharacter();
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BattleBaseCharacter);
	if (OwnerActor_SkillEntityComponent)
	{
		OwnerActor_SkillEntityComponent->FireCurrentSkill_V2();
	}
}

void UBattleBaseAnimInstance::SkillDummyNotify()
{
	AActor* BattleBaseCharacter = GetOwnerCharacter();
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(BattleBaseCharacter);
	if (OwnerActor_SkillEntityComponent)
	{
		OwnerActor_SkillEntityComponent->DummyCurrentSkill_V2();
	}
}

void UBattleBaseAnimInstance::EndSpawnAni()
{
	AActor* OwnerCharacter = GetOwnerCharacter();
	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerCharacter);
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopMove);
	}
}

void UBattleBaseAnimInstance::EndDeadAni()
{
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	AActor* BattleBaseCharacter = GetOwnerCharacter();
	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(BattleBaseCharacter);
	if (OwnerActor_EntityRenderComponent)
	{
		OwnerActor_EntityRenderComponent->ShowDeadDisapearEvent();
	}

	ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(BattleBaseCharacter->GetWorld());
	if (!BasePC)
		return;
	

	

	
}

//////////////////////////////////////////////////////////////////////////

void UBattleBaseAnimInstance::WangRealMode()
{
	float BoneScale = 5.0f;
	FName BoneName = FName("Bip001-Head");
	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	if (IsValid(BasePlayerController))
	{
		BoneName = BasePlayerController->RealMode_BoneName;
		BoneScale = BasePlayerController->RealModeBoneScale;
	}

	auto skelcom = this->GetSkelMeshComponent();
	auto Skelmesh = this->GetSkelMeshComponent()->SkeletalMesh;
	auto CheckCurrentSkeleton = this->GetSkelMeshComponent()->SkeletalMesh->Skeleton;//->>CurrentSkeleton;
	if (CheckCurrentSkeleton)
	{
		const FReferenceSkeleton& RefSkeleton = CheckCurrentSkeleton->GetReferenceSkeleton();
		int32 BoneIndex = RefSkeleton.FindBoneIndex(BoneName);
		if (RefSkeleton.IsValidIndex(BoneIndex))
		{
			const TArray<FTransform>& RefBonePose = RefSkeleton.GetRefBonePose();
			FTransform BoneTransForm = RefBonePose[BoneIndex];
			BoneTransForm.SetScale3D(FVector(BoneScale, BoneScale, BoneScale));
			FAnimInstanceProxy& Proxy = GetProxyOnAnyThread<FAnimInstanceProxy>();
			
			//update
			Skelmesh->RefSkeleton.SetFinalRefPoseTransform(BoneIndex, BoneTransForm);
			
			Skelmesh->Skeleton->UpdateReferencePoseFromMesh(Skelmesh);
			Proxy.GetSkelMeshComponent()->SkeletalMesh->RefSkeleton.SetFinalRefPoseTransform(BoneIndex, BoneTransForm);
			Proxy.GetSkelMeshComponent()->BoneSpaceTransforms[BoneIndex].SetScale3D(BoneTransForm.GetScale3D());

			skelcom->BoneSpaceTransforms[BoneIndex].SetScale3D(BoneTransForm.GetScale3D()); //by danzang
			ParallelCanEvaluate(Skelmesh);			
		}
	}
}