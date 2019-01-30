// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChangeAniSpeedState.h"

#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Character/BattleBaseAnimInstance.h"

void UChangeAniSpeedState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UChangeAniSpeedState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	TotalTime = TotalDuration;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			auto EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Owner);
			if (IsValid(EntityAIComponent))
			{
				CurState = eChangeState;
				StartSpeed = OriChangeAniSpeed = EntityAIComponent->GetModifiedAnimSpeed();
				EndSpeed = ChangeAniSpeed;
				RestoreTime = TotalTime - ChangeTime;
				CurTime = 0.0f;
			}
		}
	}
}

void UChangeAniSpeedState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			auto EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Owner);
			if (IsValid(EntityAIComponent))
			{

				switch (CurState)
				{
				case eChangeState:
				{
					CurTime += FrameDeltaTime;
					float fRate = CurTime / ChangeTime;
					if (CurTime >= ChangeTime)
					{
						fRate = 1.0f;
						CurState = eWaitingState;
					}

					float curSpeed = 1.0f;
					switch (BlendType)
					{
					case EBlendAniSpeedType::VE_BlendLinear:
						curSpeed = FMath::Lerp<float, float>(StartSpeed, EndSpeed, fRate);
						break;
					case EBlendAniSpeedType::VE_BlendEaseIn:
						curSpeed = FMath::InterpEaseIn<float>(StartSpeed, EndSpeed, fRate, BlendEaseValue);
						break;
					case EBlendAniSpeedType::VE_BlendEaseOut:
						curSpeed = FMath::InterpEaseOut<float>(StartSpeed, EndSpeed, fRate, BlendEaseValue);
						break;
					case EBlendAniSpeedType::VE_BlendEaseInOut:
						curSpeed = FMath::InterpEaseInOut<float>(StartSpeed, EndSpeed, fRate, BlendEaseValue);
						break;
					}

					EntityAIComponent->SetModifiedAnimSpeed(curSpeed);
				}
				break;
				case eWaitingState:
				{
					CurTime += FrameDeltaTime;
					if (CurTime >= RestoreTime)
					{
						RestoreTime = TotalTime - CurTime;
						CurTime = 0.0f;
						CurState = eRestoreState;
					}
				}
				break;
				case eRestoreState:
				{
					CurTime += FrameDeltaTime;
					float fRate = CurTime / RestoreTime;
					if (CurTime >= RestoreTime)
					{
						fRate = 1.0f;
						CurState = eNoState;
					}

					float curSpeed = 1.0f;
					switch (BlendType)
					{
					case EBlendAniSpeedType::VE_BlendLinear:
						curSpeed = FMath::Lerp<float, float>(StartSpeed, EndSpeed, fRate);
						break;
					case EBlendAniSpeedType::VE_BlendEaseIn:
						curSpeed = FMath::InterpEaseIn<float>(StartSpeed, EndSpeed, fRate, BlendEaseValue);
						break;
					case EBlendAniSpeedType::VE_BlendEaseOut:
						curSpeed = FMath::InterpEaseOut<float>(StartSpeed, EndSpeed, fRate, BlendEaseValue);
						break;
					case EBlendAniSpeedType::VE_BlendEaseInOut:
						curSpeed = FMath::InterpEaseInOut<float>(StartSpeed, EndSpeed, fRate, BlendEaseValue);
						break;
					}

					EntityAIComponent->SetModifiedAnimSpeed(curSpeed);
				}
				break;
				}
			}
		}
	}
}

void UChangeAniSpeedState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UChangeAniSpeedState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			auto EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Owner);
			if (IsValid(EntityAIComponent))
			{
				EntityAIComponent->SetModifiedAnimSpeed(OriChangeAniSpeed);
			}
		}
	}
}



