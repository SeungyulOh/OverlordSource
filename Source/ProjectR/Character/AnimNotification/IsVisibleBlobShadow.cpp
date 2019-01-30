// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "IsVisibleBlobShadow.h"
#include "Character/BattleBaseAnimInstance.h"

void UIsVisibleBlobShadow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	//UE_LOG(LogBattle, Log, TEXT("Enter StartRTrail Received_Notify "));

	if (IsValid(AnimInstance))
	{
		//UE_LOG(LogBattle, Log, TEXT("Success Cast"));
		APawn* PawnOnwer = AnimInstance->TryGetPawnOwner();

		if (IsValid(PawnOnwer))
		{
		}
	}

}
