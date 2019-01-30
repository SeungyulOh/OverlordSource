// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "StartLTrail.h"
#include "Character/BattleBaseAnimInstance.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"


void UStartLTrail::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	//UE_LOG(LogBattle, Log, TEXT("Enter StartLTrail Received_Notify "));

	if (IsValid(BattleBaseAnimInstance))
	{
		//UE_LOG(LogBattle, Log, TEXT("Success Cast"));
		APawn* PawnOnwer = AnimInstance->TryGetPawnOwner();

		if (IsValid(PawnOnwer))
		{
			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(PawnOnwer);
			if (IsValid(EntityRenderComponent))
			{
				EntityRenderComponent->StartTrail(false);
			}
		}
	}

}