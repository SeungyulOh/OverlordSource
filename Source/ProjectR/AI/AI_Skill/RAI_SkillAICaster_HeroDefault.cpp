// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAI_SkillAICaster_HeroDefault.h"
#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Character/Components/AnimStateComponent.h"
#include "GlobalIntegrated.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"


URAI_SkillAICaster_HeroDefault::URAI_SkillAICaster_HeroDefault()
{
	
}

void URAI_SkillAICaster_HeroDefault::Init(AActor* InOwnerActor, TArray< FName >& InSkillList, float InCheckInterval)
{
	Super::Init(InOwnerActor, InSkillList, InCheckInterval);
}

bool URAI_SkillAICaster_HeroDefault::CheckReadyToCastSkill(float DeltaTime)
{	
	// Skip.. if owner is selected character
// 	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
// 	AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
// 	if (IsValid(SelectedCharacter))
// 	{
// 		bool IsSelectedCharacter = (OwnerActor.Get() == SelectedCharacter);
// 		if (IsSelectedCharacter)
// 			return false;
// 	}


	bool CheckSkillCastingState = UpdateElapsedTime(DeltaTime);

	if (CheckSkillCastingState)
	{
		// IsInAttacking..
		UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
		if (AnimStateComponent)
		{
			if (AnimStateComponent->GetCurrentAnimStateType() != EAnimStateEnum::VE_NormalAttack)
				return false;
		}

		USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (!IsValid(SkillComp))
			return false;
		FName CurrentCheckSkill = CheckSkillList[CurrentCheckIndex]->SkillID;
		UISkill_V2* SkillInst = OwnerSkillEntityComponent->GetSkill_V2(CurrentCheckSkill);
		if (SkillInst)
		{
			float CoolTime = SkillInst->GetCurrentCoolTime();
			if (CoolTime <= 0)
			{
				SkillComp->CastInputSkill(CurrentCheckSkill, OwnerActor.Get(), nullptr);
				IncreaseCheckIndex();

#if WITH_EDITOR
				if (GEngine)
				{
					if (RGAMEINSTANCE(this)->GameEnvironmentVar.EnableSkillLog)
					{
						FString logMsg = FString::Printf(TEXT("[ %s ] AI Casting - %s"),
							*OwnerActor->GetName(),
							*CurrentCheckSkill.ToString() );
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, logMsg);
					}
				}
#endif

				return true;
			}else if (CoolTime > this->CheckInterval)
			{
				// pass long time cool skill..
				++CurrentCheckIndex;
				if (CurrentCheckIndex >= CheckSkillList.Num())
					CurrentCheckIndex = 0;		

#if WITH_EDITOR
				if (GEngine)
				{
					
					if (RGAMEINSTANCE(this)->GameEnvironmentVar.EnableSkillLog)
					{
						FString logMsg = FString::Printf(TEXT("[ %s ] Pass long time cool - %s"),
							*OwnerActor->GetName(),
							*CurrentCheckSkill.ToString());
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, logMsg);
					}					
				}
#endif
			}			
		}
	}

	return false;
}

FSkillTableInfo_V2* URAI_SkillAICaster_HeroDefault::GetSkillTableInfo( FName InSkillID )
{
	FSkillTableInfo_V2* SkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(InSkillID);
	return SkillTableInfo;
}
