#include "ProjectR.h"
#include "RAI_SkillCondition_Base.h"

#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "UtilFunctionIntegrated.h"

URAI_SkillCondition_Base::URAI_SkillCondition_Base()
{
	ClearConditionList();
}

void URAI_SkillCondition_Base::Init(AActor* InOwnerActor, float InCheckInterval)
{
	OwnerActor = InOwnerActor;
	if (InOwnerActor)
	{
		OwnerBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InOwnerActor);
		OwnerSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InOwnerActor);
	}
}

void URAI_SkillCondition_Base::AddSkillConditionID(FName InConditionID)
{
	FSkillConditionTableInfo* SkillConditionInfo = UTableManager::GetInstancePtr()->GetSkillConditionRow(InConditionID);
	if (SkillConditionInfo)
		SkillContionTableInfoList.Emplace(SkillConditionInfo);
}

void URAI_SkillCondition_Base::ClearConditionList()
{
	SkillContionTableInfoList.Empty();
}

AActor* URAI_SkillCondition_Base::GetCheckTargetActor()
{
	return CheckTargetActor.Get();
}

bool URAI_SkillCondition_Base::CheckConditions(EPriorityTypeEnum InAISearchPreferType)
{
	if (!OwnerBattleEntityComponent.IsValid() || !OwnerSkillEntityComponent.IsValid())
		return false;

	for (auto SkillCondifionInfo : SkillContionTableInfoList)
	{
		if ( SkillCondifionInfo != nullptr && CheckCondition( SkillCondifionInfo, InAISearchPreferType))
			return true;
	}

	return false;
}

bool URAI_SkillCondition_Base::CheckCondition(FSkillConditionTableInfo* InSkillConditionInfo, EPriorityTypeEnum InAISearchPreferType )
{	
	/*
	Select check target

	!!!. 
	All check logic depends on target type
	1. If target type is VE_Target, you just check only this target.
	2. If target type is VE_Me, you should search circle range.

	So.. For simpling logic, use different special check functions.
		1. CheckMeTypeCheck
		2. CheckTargetTypeCheck

	*/		
	
	switch (InSkillConditionInfo->ConditionCheckTarget)
	{
		case ESkillTargetTypeEnum::VE_None:
			return false;

		case ESkillTargetTypeEnum::VE_Me:
			return CheckConditonMEType(InSkillConditionInfo);
		case ESkillTargetTypeEnum::VE_Target:
			return CheckConditonTargetType(InSkillConditionInfo, InAISearchPreferType);
	}

	return false;
}

bool URAI_SkillCondition_Base::CheckConditonMEType(FSkillConditionTableInfo* InSkillConditionInfo)
{
	CheckTargetActor = GetAISearchTarget(EPriorityTypeEnum::VE_None);
	AActor* TempCheckTargetActor = OwnerActor.Get();

	TArray< TWeakObjectPtr < AActor > > OutTargets;
	float SearchRange = InSkillConditionInfo->ConditionCheckRange;	
	if (!FMath::IsNearlyZero(SearchRange))
	{
		URenewal_BattleFunctionLibrary::FindTarget(OutTargets, TempCheckTargetActor,
			TempCheckTargetActor->GetActorLocation(), InSkillConditionInfo->ConditionCheckTargetApply,
			ESearchShapeTypeEnum::VE_Circle, FVector(SearchRange, 0.0f, 0.0f));
	}
	else
	{
		OutTargets.Emplace(TempCheckTargetActor);
	}

	int32 SuccessCount = 0;
	for (auto CheckTarget : OutTargets)
	{
		if (CheckTargetState(InSkillConditionInfo, CheckTarget.Get()))
			++SuccessCount;
	}

	if (SuccessCount < InSkillConditionInfo->ConditionCheckApplyValue)
		return false;

	return true;
}

bool URAI_SkillCondition_Base::CheckTargetState(FSkillConditionTableInfo* InSkillConditionInfo, AActor* CheckTarget )
{
	bool bResult = false;

	UBattleEntityComponent* CheckTarget_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CheckTarget);
	USkillEntityComponent* CheckTarget_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(CheckTarget);

	if (CheckTarget_BattleEntityComponent == nullptr || CheckTarget_SkillEntityComponent == nullptr)
		return false;

	/*
	Buff check
	*/	
	if (InSkillConditionInfo->OnAbnormalBuff != EAbnormalApplyTypeEnum::VE_None)
	{				
		if ( CheckTarget_SkillEntityComponent->GetSkillSlotContainer())
		{
			if (InSkillConditionInfo->OnAbnormalBuff == EAbnormalApplyTypeEnum::VE_Buff)
			{
				if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInBuff(EAbnormalApplyTypeEnum::VE_Buff) == false)
					return false;
			}
			else
			{
				if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInBuff(EAbnormalApplyTypeEnum::VE_DeBuff) == false )
					return false;
			}

		}
	}

	if (InSkillConditionInfo->OffAbnormalBuff != EAbnormalApplyTypeEnum::VE_None)
	{		
		if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer())
		{
			if (InSkillConditionInfo->OnAbnormalBuff == EAbnormalApplyTypeEnum::VE_Buff)
			{
				if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInBuff(EAbnormalApplyTypeEnum::VE_Buff) )
					return false;
			}
			else
			{
				if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInBuff(EAbnormalApplyTypeEnum::VE_DeBuff) )
					return false;
			}

		}
	}

	/*
	Group check
	*/
	if (InSkillConditionInfo->OnGroupState != EAbnormalGroupTypeEnum::VE_None)
	{		
		if ( CheckTarget_SkillEntityComponent->GetSkillSlotContainer())
		{
			if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInGroup(InSkillConditionInfo->OnGroupState) == false)
				return false;
		}
	}

	if (InSkillConditionInfo->OffGroupState != EAbnormalGroupTypeEnum::VE_None)
	{		
		if ( CheckTarget_SkillEntityComponent->GetSkillSlotContainer())
		{
			if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInGroup(InSkillConditionInfo->OffGroupState))
				return false;
		}
	}

	/*
	Slot check
	*/
	if (InSkillConditionInfo->OnSlotState != EAbnormalSlotTypeEnum::VE_None)
	{		
		if ( CheckTarget_SkillEntityComponent->GetSkillSlotContainer())
		{
			if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInSlot(InSkillConditionInfo->OnSlotState) == false )
				return false;
		}
	}

	if (InSkillConditionInfo->OffSlotState != EAbnormalSlotTypeEnum::VE_None)
	{		
		if ( CheckTarget_SkillEntityComponent->GetSkillSlotContainer())
		{
			if (CheckTarget_SkillEntityComponent->GetSkillSlotContainer()->IsInSlot(InSkillConditionInfo->OffSlotState) )
				return false;
		}
	}

	// Check remain HP condition
	if (InSkillConditionInfo->RemainHpPercent > 0)
	{
		if (CheckTarget_BattleEntityComponent->GetHP() > (CheckTarget_BattleEntityComponent->GetMaxHP() * ((float)InSkillConditionInfo->RemainHpPercent * 0.01f)))/// 100.0f)))
			return false;
	}		
	
	return true;
}

/*
Just single target check.
*/
bool URAI_SkillCondition_Base::CheckConditonTargetType(FSkillConditionTableInfo* InSkillConditionInfo, EPriorityTypeEnum InAISearchPreferType)
{
	EFriendshipTypeEnum			eConditionCheckTargetApply = InSkillConditionInfo->ConditionCheckTargetApply;
	CheckTargetActor = GetAISearchTarget(InAISearchPreferType, eConditionCheckTargetApply);
	if (!CheckTargetActor.IsValid())
		return false;

	/*
	Validate range.
	*/

	float TargetDistance = FVector::DistSquaredXY(OwnerActor->GetActorLocation(), CheckTargetActor->GetActorLocation());
	switch(eConditionCheckTargetApply)
	{
		case EFriendshipTypeEnum::VE_Friend:
		case EFriendshipTypeEnum::VE_FriendMagic:
		case EFriendshipTypeEnum::VE_FriendPhysical:
		case EFriendshipTypeEnum::VE_MySummon:
		case EFriendshipTypeEnum::VE_SummonFriend:
		case EFriendshipTypeEnum::VE_FriendExceptMe:
		case EFriendshipTypeEnum::VE_FriendLowestHP:
		case EFriendshipTypeEnum::VE_FriendParty:
		{
			float SearchRange = InSkillConditionInfo->ConditionCheckRange;
			if (!FMath::IsNearlyZero(SearchRange))
			{
				if (TargetDistance > FMath::Square(SearchRange))
					return false;
			}
		}
		break;
		//case EFriendshipTypeEnum::VE_Enemy:
		//case EFriendshipTypeEnum::VE_EnemyLowestHP:
		default:
		{
			UBattleEntityComponent* Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CheckTargetActor.Get());
			float TargetBoundingRadius = 0.0f;
			if (Target_BattleEntityComponent)
			{
				TargetBoundingRadius = Target_BattleEntityComponent->GetBoundingRadius();
			}

			if (TargetDistance < FMath::Square(InSkillConditionInfo->ConditionCheckDistanceMin)
				|| TargetDistance > FMath::Square(InSkillConditionInfo->ConditionCheckDistanceMax + TargetBoundingRadius))
				return false;
		}
		break;
	}

	/*
	Check state
	*/
	if (CheckTargetState(InSkillConditionInfo, CheckTargetActor.Get()) == false)
		return false;

	return true;
}

AActor* URAI_SkillCondition_Base::GetAISearchTarget(EPriorityTypeEnum InAISearchPreferType, EFriendshipTypeEnum FriendshipType)
{
	AActor* TargetActor = nullptr;
	if (InAISearchPreferType != EPriorityTypeEnum::VE_None)
	{
		switch(FriendshipType)
		{
			case EFriendshipTypeEnum::VE_Friend:
			case EFriendshipTypeEnum::VE_FriendMagic:
			case EFriendshipTypeEnum::VE_FriendPhysical:
			case EFriendshipTypeEnum::VE_MySummon:
			case EFriendshipTypeEnum::VE_SummonFriend:
			case EFriendshipTypeEnum::VE_FriendExceptMe:
			case EFriendshipTypeEnum::VE_FriendLowestHP:
			case EFriendshipTypeEnum::VE_FriendParty:
			{
				TargetActor = UAIFunctionLibrary::SearchFriend(OwnerActor.Get(), false, InAISearchPreferType);
			}
			break;
			case EFriendshipTypeEnum::VE_Enemy:
			case EFriendshipTypeEnum::VE_EnemyLowestHP:
			{
				TargetActor = UAIFunctionLibrary::SearchEnemy(OwnerActor.Get(), false, InAISearchPreferType);
			}
			break;
			default:
			{
				//UE_LOG(LogBattle, Log, TEXT("URAI_SkillCondition_Base::GetAISearchTarget FriendshipType miss"));
			}
			break;
		}
	}

	if (TargetActor == nullptr )
	{
		TargetActor = OwnerBattleEntityComponent->GetCurrentAttackTarget();
		if (TargetActor == nullptr)
		{
			UEntityAIComponent* OwnerAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
			if (OwnerAIComponent)
			{
				TargetActor = OwnerAIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
				if (OwnerBattleEntityComponent->IsEnemy(TargetActor) == false)
					TargetActor = nullptr;
			}
		}
	}		

	return TargetActor;
}
