// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_SkillAICaster.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Global/RGameUserSetting.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"


URAI_SkillAICaster::URAI_SkillAICaster()
{
	
}

void URAI_SkillAICaster::Init(AActor* InOwnerActor, TArray< FName >& InSkillList, float InCheckInterval)
{
	Clear();

	OwnerActor = InOwnerActor;
	if (InOwnerActor)
	{
		OwnerSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InOwnerActor);
	}	
	CheckInterval = InCheckInterval;


	for (FName& SkillID : InSkillList )
	{
		AddNewSkillCondition(SkillID);
	}
}

void URAI_SkillAICaster::Clear()
{
	OwnerActor = nullptr;
	OwnerSkillEntityComponent = nullptr;	
	CheckInterval = 0.0f;

	CheckSkillList.Empty();
}

bool URAI_SkillAICaster::CheckReadyToCastSkill(float DeltaTime)
{
	return false;
}

void URAI_SkillAICaster::IncreaseCheckIndex()
{
	++CurrentCheckIndex;
	if (CurrentCheckIndex >= CheckSkillList.Num())
		CurrentCheckIndex = 0;

	CheckElapsedTime = 0.0f;
	bActivateTimeInterval = true;
}

bool URAI_SkillAICaster::UpdateElapsedTime(float DeltaTime)
{
	if (!bActivateTimeInterval)
		return true;

	CheckElapsedTime += DeltaTime;
	if (CheckElapsedTime >= CheckInterval)
	{
		CheckElapsedTime = 0.0f;
		bActivateTimeInterval = false;
		return true;
	}


	return false;
}


void URAI_SkillAICaster::AddNewSkillCondition(FName InSkillID)
{
	USkillInfoWithCondition* SkillInfoWithCondition = NewObject< USkillInfoWithCondition >(this);
	SkillInfoWithCondition->SkillID = InSkillID;	

	UISkill_V2* SkillInst = OwnerSkillEntityComponent->GetSkill_V2(InSkillID);
	if (SkillInst == nullptr)
		return;	

	FSkillTableInfo_V2* SkillTableInfo = GetSkillTableInfo(SkillInst->GetSkillTableID());
	if (SkillTableInfo)
	{
		SkillInfoWithCondition->MaxRestTime = SkillTableInfo->AIForceUseOverTime;

		TArray< FName > ValidSkillConditionIDList;
		if (!SkillTableInfo->AICondition1.IsNone())
		{
			ValidSkillConditionIDList.Emplace(SkillTableInfo->AICondition1);
		}

		if (!SkillTableInfo->AICondition2.IsNone())
		{
			ValidSkillConditionIDList.Emplace(SkillTableInfo->AICondition2);
		}

		if (!SkillTableInfo->AICondition3.IsNone())
		{
			ValidSkillConditionIDList.Emplace(SkillTableInfo->AICondition3);
		}

		if (ValidSkillConditionIDList.Num() > 0)
		{
			URAI_SkillCondition_Base* SkillCondition = NewObject< URAI_SkillCondition_Base >(this);
			if (SkillCondition)
			{
				SkillCondition->Init(OwnerActor.Get());
				for (auto SkillConditionID : ValidSkillConditionIDList)
					SkillCondition->AddSkillConditionID(SkillConditionID);

				SkillInfoWithCondition->SkillCondtion = SkillCondition;
				SkillInfoWithCondition->AISearchPreferType = SkillTableInfo->AISearchTargetType;

				CheckSkillList.Emplace( SkillInfoWithCondition );
			}
		}
	}
}

bool URAI_SkillAICaster::CheckReadyToCastSkill_V2(float DeltaTime)
{	
	bool bResult = false;
	if (CanCastSkill())
	{		
		USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (!IsValid(SkillComp))
			return bResult;

		bool bUseMaxElapsedRestTime = false;
		bool bInAttack = false;

		UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
		if (AnimStateComponent)
		{
			bInAttack = ( AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_NormalAttack );			
		}

		

		// Check max rest time		
		// If owner is in attack state, check max rest time.
		bool isUserTrying = false;
		for (USkillInfoWithCondition* SkillCondition : CheckSkillList)
		{
			if (IsValid(SkillCondition) == false)
				continue;

			/*If users are trying to use skill for themselves*/
			AStagePlayerController* StagePC = Cast<AStagePlayerController>(UGameplayStatics::GetPlayerController(RGAMEINSTANCE(GEngine)->GetWorld(), 0));
			if (StagePC)
			{
				ULeaderControlHelper* LeaderHelper = StagePC->GetLeaderControlHelper();
				if (LeaderHelper->LeaderCharacter.Get() == OwnerActor.Get())
				{
// 					if (LeaderHelper->GetLeaderSkillControl()->bSkillTouch && SkillCondition->SkillID == LeaderHelper->GetLeaderSkillControl()->InputSkillName)
// 					{
// 						isUserTrying = true;
// 						continue;
// 					}
						
				}
			}
			
			SkillCondition->RestElapsedTime += DeltaTime;
			if (bInAttack && ( SkillCondition->MaxRestTime > 0 ) )
			{
				if (SkillCondition->RestElapsedTime > SkillCondition->MaxRestTime)
				{
					SkillCondition->RestElapsedTime = 0.0f;
					SkillComp->CastInputSkill(SkillCondition->SkillID, OwnerActor.Get(), nullptr);
					bResult = true;
					bUseMaxElapsedRestTime = true;
				}
			}
		}

		if (isUserTrying)
			return bResult;

		if ( !bUseMaxElapsedRestTime && CheckSkillConditions(DeltaTime))
		{
			SkillComp->CastInputSkill(CurrentReadyToFireSkill, OwnerActor.Get(), CheckTargetActor.Get(), CheckTargetLocation);
			bResult = true;
		}		
	}

	return bResult;
}

bool URAI_SkillAICaster::CanCastSkill()
{
	

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		if (BattleEntityComponent->IsAbleToUseSkill() == false)
			return false;
	}

	auto SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (SkillEntityComponent)
	{
		if (SkillEntityComponent->IsSkillCasting())
			return false;

		if (SkillEntityComponent->IsSkillActing())
			return false;
	}

	return true;
}

bool URAI_SkillAICaster::CheckSkillConditions(float DeltaTime)
{
	for (USkillInfoWithCondition* SkillInfoWithCondition : CheckSkillList)
	{
		UISkill_V2* SkillInst = OwnerSkillEntityComponent->GetSkill_V2(SkillInfoWithCondition->SkillID);
		if (SkillInst)
		{
			float CoolTime = SkillInst->GetCurrentCoolTime();
			if (CoolTime <= 0)
			{
				if (SkillInfoWithCondition->SkillCondtion)
				{
					if (SkillInfoWithCondition->SkillCondtion->CheckConditions(SkillInfoWithCondition->AISearchPreferType))
					{
						CurrentReadyToFireSkill = SkillInfoWithCondition->SkillID;
						CheckTargetActor = SkillInfoWithCondition->SkillCondtion->GetCheckTargetActor();
						//CheckTargetLocation = ((CheckTargetActor.IsValid()) ? CheckTargetActor->GetActorLocation() : FVector::ZeroVector);
						CheckTargetLocation = ((CheckTargetActor.IsValid()) ? UMapFunctionLibrary::GetNavAgentLocation(CheckTargetActor.Get()) : FVector::ZeroVector);
						
						return true;
					}						
				}
			}
		}			
	}

	return false;
}