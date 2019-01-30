// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimSpawnEventState.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/BattleBaseAnimInstance.h"

void UAnimSpawnEventState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimSpawnEventState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

}

void UAnimSpawnEventState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimSpawnEventState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimSpawnEventState End"));
#endif
	
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;

	APawn* PawnOwner = AnimInstance->TryGetPawnOwner();
	if (IsValid(PawnOwner))
	{
		IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(PawnOwner);
		if (BaseProperty)
		{
			if (false == BaseProperty->IsReady())
			{
				BaseProperty->SetReady(true);
			}
		}
	}

	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->EndSpawnAni();
	}

}