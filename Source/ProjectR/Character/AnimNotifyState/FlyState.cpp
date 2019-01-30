// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "FlyState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "UtilFunctionIntegrated.h"

void UFlyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	UE_LOG(LogBattle, Log, TEXT("Enter FlyAnimNotifyState Begin"));
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (!IsValid(MeshComp))
	{
		return;
	}

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (!IsValid(BattleBaseAnimInstance))
	{
		return;
	}

	APawn* PawnOwner = BattleBaseAnimInstance->TryGetPawnOwner();

	if (!IsValid(PawnOwner))
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(PawnOwner);
	if (IsValid(MovementComponent) == false)
		return;

	OriginalCharacterMovementName = MovementComponent->GetMovementName();
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UFlyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UFlyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	UE_LOG(LogBattle, Log, TEXT("Enter FlyAnimNotifyState End"));
	Super::NotifyEnd(MeshComp, Animation);

	if (!IsValid(MeshComp))
	{
		return;
	}

	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (!IsValid(BattleBaseAnimInstance))
	{
		return;
	}

	APawn* PawnOwner = BattleBaseAnimInstance->TryGetPawnOwner();

	if (!IsValid(PawnOwner))
	{
		return;
	}
	
	UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(PawnOwner);
	if (IsValid(MovementComponent) == false)
		return;

	if (OriginalCharacterMovementName.Equals(TEXT("Swimming"), ESearchCase::CaseSensitive))
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Swimming);
	}
	else if (OriginalCharacterMovementName.Equals(TEXT("Walking"), ESearchCase::CaseSensitive))
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else if (OriginalCharacterMovementName.Equals(TEXT("Falling"), ESearchCase::CaseSensitive))
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	else if (OriginalCharacterMovementName.Equals(TEXT("Flying"), ESearchCase::CaseSensitive))
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	}
	else if (OriginalCharacterMovementName.Equals(TEXT("Custom"), ESearchCase::CaseSensitive))
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Custom);
	}
}