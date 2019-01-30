// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimTrailState.h"

#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleObjectAnimInstance.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "UtilFunctionIntegrated.h"

void UAnimTrailState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimTrailState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	//UE_LOG(LogBattle, Log, TEXT("Enter EnterSkillAni Received_Notify "));

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	APawn* PawnOnwer = AnimInstance->TryGetPawnOwner();

	if (IsValid(PawnOnwer))
	{
		UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(PawnOnwer);
		if (IsValid(EntityRenderComponent))
		{
			EntityRenderComponent->StartTrail(IsRTrail);
		}
	}
}

void UAnimTrailState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimTrailState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimTrailState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	//UE_LOG(LogBattle, Log, TEXT("Enter End Skill Ani Received_Notify "));

	APawn* PawnOnwer = AnimInstance->TryGetPawnOwner();

	if (IsValid(PawnOnwer))
	{
		UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(PawnOnwer);
		if (IsValid(EntityRenderComponent))
		{
			EntityRenderComponent->EndTrail(IsRTrail);
		}
	}

}