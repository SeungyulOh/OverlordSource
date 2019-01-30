// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EndSkillAniButContinueSkill.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "UtilFunctionIntegrated.h"

void UEndSkillAniButContinueSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	//UE_LOG(LogBattle, Log, TEXT("Enter End Skill Ani Received_Notify "));

	if (IsValid(BattleBaseAnimInstance))
	{
		//UE_LOG(LogBattle, Log, TEXT("Success Cast"));
		
		APawn* PawnOwner = AnimInstance->TryGetPawnOwner();

		if (IsValid(PawnOwner))
		{
			USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(PawnOwner);
			UBattleEntityComponent*	BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(PawnOwner);
			if (IsValid(SkillEntityComponent) && IsValid(BattleEntityComponent))
			{
				//This Skill has to be continued with the normal attack
				SkillEntityComponent->SetSkillAniType(ESkillAniTypeEnum::VE_None);
				SkillEntityComponent->SetSkillActing(false);
				
				BattleEntityComponent->EnterAttack();
			}
		}
	}
}