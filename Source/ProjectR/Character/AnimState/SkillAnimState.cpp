// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"
#include "SkillAnimState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Global/RealTimeModManager.h"


USkillAnimState::USkillAnimState()
{
	AnimStateType = EAnimStateEnum::VE_SkillAttack;
}

void USkillAnimState::Initialize()
{
	Super::Initialize();
}

void USkillAnimState::Uninitialize()
{
	if (OwnerActor.IsValid())
	{
		OwnerActor->GetWorldTimerManager().ClearTimer(SkillDelayHandler);
	}

	Super::Uninitialize();
}

void USkillAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);

	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		if (false == BattleBaseAnimInstance->OnEndSkillAniDelegate.IsAlreadyBound(this, &USkillAnimState::EndSkillAni))
		{
			BattleBaseAnimInstance->OnEndSkillAniDelegate.AddDynamic(this, &USkillAnimState::EndSkillAni);
		}
	}

	if (URealTimeModManager::IsRealTimeGameMode())
	{
		URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerActor->GetComponentByClass(URemoteMoveComponent::StaticClass()));
		if (RemoteMoveComponent)
			RemoteMoveComponent->bMoveActive = false;
	}
}

void USkillAnimState::Leave()
{
	Super::Leave();

	SetSkillAniType(ESkillAniTypeEnum::VE_None);
	SetSkillCasting(false);

	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		if (true == BattleBaseAnimInstance->OnEndSkillAniDelegate.IsAlreadyBound(this, &USkillAnimState::EndSkillAni))
		{
			BattleBaseAnimInstance->OnEndSkillAniDelegate.RemoveDynamic(this, &USkillAnimState::EndSkillAni);
		}
	}

	if (URealTimeModManager::IsRealTimeGameMode())
	{
		URemoteMoveComponent* RemoteMoveComponent = Cast<URemoteMoveComponent>(OwnerActor->GetComponentByClass(URemoteMoveComponent::StaticClass()));
		if (RemoteMoveComponent)
			RemoteMoveComponent->bMoveActive = true;
	}
}

bool USkillAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	return true;
}

void USkillAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	//Super::SetAnimParam(Command, varParam);

	if (Command == ECharacterCommandEnum::VE_ActionSkill)
	{
		if (varParam.GetType() == EOnlineKeyValuePairDataType::Int32)
		{
			int32 intParam;
			varParam.GetValue(intParam);
			SetSkillAniType((ESkillAniTypeEnum)intParam);
			SetSkillActing(true);
			SetSkillCasting(false);
			//UE_LOG(LogBattle, Log, TEXT(">>>>>>  USkillAnimState::SetAnimParam VE_ActionSkill - %s ESkillAniTypeEnum %d"), *(OwnerActor->GetName()), intParam);
		}
	}
	if (Command == ECharacterCommandEnum::VE_ActionSkillEnd)
	{
		if (varParam.GetType() == EOnlineKeyValuePairDataType::Int32)
		{
			int32 intParam;
			varParam.GetValue(intParam);
			SetSkillActingEnd((ESkillAniTypeEnum)intParam);
			//FString	stringAnimType = UUtilFunctionLibrary::GetEnumToString(TEXT("ESkillAniTypeEnum"), intParam);
			//UE_LOG(LogBattle, Log, TEXT(">>>>>>  USkillAnimState::SetAnimParam VE_ActionSkillEnd - %s ESkillAniTypeEnum %s"), *(OwnerActor->GetName()), *stringAnimType);
		}
	}
	else if (Command == ECharacterCommandEnum::VE_CastingSkill)
	{
		if (varParam.GetType() == EOnlineKeyValuePairDataType::Int32)
		{
			int32 intParam;
			varParam.GetValue(intParam);
			SetSkillAniType((ESkillAniTypeEnum)intParam);
			SetSkillActing(false);
			SetSkillCasting(true);

			FString	stringAnimType = UUtilFunctionLibrary::GetEnumToString(TEXT("ESkillAniTypeEnum"), intParam);
			//UE_LOG(LogBattle, Log, TEXT(">>>>>>  USkillAnimState::SetAnimParam VE_CastingSkill - %s ESkillAniTypeEnum %s"), *(OwnerActor->GetName()), *stringAnimType);
		}
	}
	
}

bool USkillAnimState::IsValidState()
{
	//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::IsValidState - %s bInSkillDelay %d"), *(OwnerActor->GetName()), bInSkillDelay);

	SkillAniType = ESkillAniTypeEnum::VE_None;

	if (false == bInSkillDelay)
	{
		if (SkillEntityComponent.IsValid())
		{
			SkillAniType = SkillEntityComponent->GetSkillAniType();
			FString	stringAnimType = UUtilFunctionLibrary::GetEnumToString(TEXT("ESkillAniTypeEnum"), (int32)SkillAniType);
			//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::IsValidState - %s SkillAniType %s"), *(OwnerActor->GetName()), *stringAnimType);
		}

		if (SkillAniType != ESkillAniTypeEnum::VE_None)
		{
			//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::IsValidState - %s return true"), *(OwnerActor->GetName()));
			return true;
		}
	}
	//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::IsValidState - %s return false"), *(OwnerActor->GetName()));
	return false;
}

bool USkillAnimState::IsCancelAnim()
{
	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		if(BattleBaseAnimInstance->IsPlayingSkillActingAnimMontage())
		{
			return false;
		}
	}

	return true;
}

EAnimStateEnum USkillAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	EAnimStateEnum ReturnAimState = EAnimStateEnum::VE_SkillAttack;

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
	case ECharacterCommandEnum::VE_StopSkill:
	{
		if (IsCancelAnim())
		{
			ReturnAimState = EAnimStateEnum::VE_Idle;
		}
	}break;
	case ECharacterCommandEnum::VE_StartAttack:
	{
		//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::DoCommand - %s ECharacterCommandEnum::VE_StartAttack"), *(OwnerActor->GetName()));
		if (false == IsValidState())
		{
			//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::DoCommand - %s ECharacterCommandEnum::VE_StartAttack -> EAnimStateEnum::VE_NormalAttack"), *(OwnerActor->GetName()));
			ReturnAimState = EAnimStateEnum::VE_NormalAttack;
		}
	}break;
	case ECharacterCommandEnum::VE_StartMove:
	{
		if (!IsCancelAnim())
			break;

		if (false == IsValidState())
		{
			ReturnAimState = EAnimStateEnum::VE_Run;
		}
		else if (SkillEntityComponent.IsValid())
		{
			if (SkillEntityComponent->IsCurrentSkillChanneling())
			{
				ReturnAimState = EAnimStateEnum::VE_Run;
			}
		}
	}break;
	default:
		break;
	}

	return ReturnAimState;
}

//////////////////////////////////////////////////////////////////////////

void USkillAnimState::SetSkillAniType(ESkillAniTypeEnum	InSkillAniType)
{
	SkillAniType = InSkillAniType;

	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetSkillAniType(SkillAniType);
	}
}

void USkillAnimState::SetSkillCasting(bool InSkillCasting)
{
	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetSkillCasting(InSkillCasting);
	}
}

void USkillAnimState::SetSkillActing(bool InSkillAction)
{
	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetSkillActing(InSkillAction);
	}
}

void USkillAnimState::SetSkillActingEnd(ESkillAniTypeEnum	InSkillAniType)
{
	if (SkillAniType != InSkillAniType)
		return;

	UBattleBaseAnimInstance* BattleBaseAnimInstance = GetBaseAnimInstance();
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->SetSkillActingEnd(InSkillAniType);
	}
}

void USkillAnimState::EndSkillAni()
{
	bInSkillDelay = false;

	if (BattleEntityComponent.IsValid())
	{
		int32 SkillIndex = (int32)SkillAniType - 1;
		if (DelayTimeSkill.IsValidIndex(SkillIndex))
		{
			if (DelayTimeSkill[SkillIndex] > 0.0f)
			{
				//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::EndSkillAni - %s bInSkillDelay TRUE"), *(OwnerActor->GetName()));
				bInSkillDelay = true;
				float FinalDelayTime = DelayTimeSkill[SkillIndex] / BattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_AttackSpeed);
				OwnerActor->GetWorldTimerManager().SetTimer(SkillDelayHandler, this, &USkillAnimState::EndSkillDelay, FinalDelayTime);
			}
		}
	}

	SkillAniType = ESkillAniTypeEnum::VE_None;
}

void USkillAnimState::EndSkillDelay()
{
	if (OwnerActor.IsValid())
		//UE_LOG(LogBattle, Log, TEXT("USkillAnimState::EndSkillDelay - %s bInSkillDelay FALSE"), *(OwnerActor->GetName()));
	bInSkillDelay = false;
}