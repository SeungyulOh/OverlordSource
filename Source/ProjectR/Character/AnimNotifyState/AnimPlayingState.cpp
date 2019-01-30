// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimPlayingState.h"
#include "Character/BattleBaseAnimInstance.h"

void UAnimPlayingState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimPlayingState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (!IsValid(BattleBaseAnimInstance))
	{
		return;
	}
}

void UAnimPlayingState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimPlayingState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimPlayingState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (!IsValid(BattleBaseAnimInstance))
	{
		return;
	}
}