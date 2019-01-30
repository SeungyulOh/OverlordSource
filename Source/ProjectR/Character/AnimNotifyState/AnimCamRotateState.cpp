// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimCamRotateState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Etc/CamRotatePivot.h"


void UAnimCamRotateState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimCamRotateState Begin"));
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			CurCamera = RGAMEINSTANCE(this)->CameraActor;

			if (IsValid(CurCamera))
			{
				InitCameraDistance = CurCamera->Camera_Distance;
				InitYawDegree = CurCamera->GetActorRotation().Yaw;

				LastYawDegree = InitYawDegree * -1.0f;
				DiffYawDegree = (180.0f - FMath::Abs<float>(InitYawDegree)) * 2.0f;
				StartPos = CurCamera->GetActorLocation();

				TArray<AActor* > arraylist;
				UGameplayStatics::GetAllActorsOfClass(Owner->GetWorld(), ACamRotatePivot::StaticClass(), arraylist);
				for (AActor* actor : arraylist)
				{
					ACamRotatePivot* pivot = Cast<ACamRotatePivot>(actor);
					if (IsValid(pivot))
					{
						CamPivot = pivot;
						if (IsValid(CamPivot->LeftComponent) && IsValid(CamPivot->RightComponent))
						{
							FVector diff = (CamPivot->RightComponent->GetComponentLocation() - CamPivot->LeftComponent->GetComponentLocation()).GetSafeNormal2D();

							float fDegree = UUtilFunctionLibrary::GetBetweenDegree(CurCamera->GetActorForwardVector().GetSafeNormal2D(), diff);
							if (fDegree > 90.0f || fDegree < -90.0f)
							{
								BlendPos = FVector(CamPivot->RightComponent->GetComponentLocation().X, CamPivot->RightComponent->GetComponentLocation().Y, CurCamera->GetActorLocation().Z);
								EndPos = FVector(CamPivot->LeftComponent->GetComponentLocation().X, CamPivot->LeftComponent->GetComponentLocation().Y, CurCamera->GetActorLocation().Z);
								DiffYawDegree *= -1.0f;
							}
							else
							{
								BlendPos = FVector(CamPivot->LeftComponent->GetComponentLocation().X, CamPivot->LeftComponent->GetComponentLocation().Y, CurCamera->GetActorLocation().Z);
								EndPos = FVector(CamPivot->RightComponent->GetComponentLocation().X, CamPivot->RightComponent->GetComponentLocation().Y, CurCamera->GetActorLocation().Z);
							}

							CenterPos = FVector(CamPivot->GetActorLocation().X, CamPivot->GetActorLocation().Y, CurCamera->GetActorLocation().Z) + CamPivot->GetActorForwardVector() * Radius;

							FVector StartDir = (BlendPos - CenterPos).GetSafeNormal2D();
							FVector EndDIr = (EndPos - CenterPos).GetSafeNormal2D();
							RotateYawDegree = UUtilFunctionLibrary::GetBetweenDegree(StartDir, EndDIr);
							Length = (BlendPos - CenterPos).Size2D();
						}
						break;
					}
				}
			}
		}
	}

	eCurState = eSTATE::eBlendState;
	TotalTime = TotalDuration;
	CurTime = 0.0f;
}

void UAnimCamRotateState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (IsValid(CurCamera) && IsValid(CamPivot))
	{
		switch (eCurState)
		{
		case UAnimCamRotateState::eBlendState:
			{
				CurTime += FrameDeltaTime;
				if (CurTime >= CamBlendTime)
				{
					CurTime = CamBlendTime;
					eCurState = eRotateState;
				}
				
				FVector curPos = StartPos + (BlendPos - StartPos) * CurTime / CamBlendTime;
				switch (BlendRotateType)
				{
				case EBlendRotateType::VE_BlendRotateLinear:
					curPos = FMath::Lerp<FVector, float>(StartPos, StartPos + (BlendPos - StartPos), CurTime / CamBlendTime);
					break;
				case EBlendRotateType::VE_BlendRotateEaseIn:
					curPos = FMath::InterpEaseIn<FVector>(StartPos, StartPos + (BlendPos - StartPos), CurTime / CamBlendTime, BlendEaseValue);
					break;
				case EBlendRotateType::VE_BlendRotateEaseOut:
					curPos = FMath::InterpEaseOut<FVector>(StartPos, StartPos + (BlendPos - StartPos), CurTime / CamBlendTime, BlendEaseValue);
					break;
				case EBlendRotateType::VE_BlendRotateEaseInOut:
					curPos = FMath::InterpEaseInOut<FVector>(StartPos, StartPos + (BlendPos - StartPos), CurTime / CamBlendTime, BlendEaseValue);
					break;
				}
				
				CurCamera->SetActorLocation(curPos);

				if (eCurState == eRotateState)
				{
					CurTime = 0.0f;
				}
			}
			break;
		case UAnimCamRotateState::eRotateState:
			{
				CurTime += FrameDeltaTime;
				if (CurTime >= (TotalTime - CamBlendTime))
				{
					CurTime = TotalTime - CamBlendTime;
					eCurState = eNoState;
				}

				FVector startDir = (BlendPos - CenterPos).GetSafeNormal2D();
				FRotator rot = FRotator::ZeroRotator;
				rot.Yaw = RotateYawDegree * CurTime / (TotalTime - CamBlendTime);

				switch (RotateType)
				{
				case ERotateType::VE_RotateLinear:
					rot.Yaw = FMath::Lerp<float, float>(0.0f, RotateYawDegree, CurTime / (TotalTime - CamBlendTime));
					break;
				case ERotateType::VE_RotateEaseIn:
					rot.Yaw = FMath::InterpEaseIn<float>(0.0f, RotateYawDegree, CurTime / (TotalTime - CamBlendTime), EaseValue);
					break;
				case ERotateType::VE_RotateEaseOut:
					rot.Yaw = FMath::InterpEaseOut<float>(0.0f, RotateYawDegree, CurTime / (TotalTime - CamBlendTime), EaseValue);
					break;
				case ERotateType::VE_RotateEaseInOut:
					rot.Yaw = FMath::InterpEaseInOut<float>(0.0f, RotateYawDegree, CurTime / (TotalTime - CamBlendTime), EaseValue);
					break;
				}

				startDir = rot.RotateVector(startDir);

				FVector curPos = CenterPos + startDir * Length;
				CurCamera->SetActorLocation(curPos);

				float CurDegree = InitYawDegree + DiffYawDegree * CurTime / (TotalTime - CamBlendTime);

				switch (RotateType)
				{
				case ERotateType::VE_RotateLinear:
					CurDegree = FMath::Lerp<float, float>(InitYawDegree, InitYawDegree + DiffYawDegree, CurTime / (TotalTime - CamBlendTime));
					break;
				case ERotateType::VE_RotateEaseIn:
					CurDegree = FMath::InterpEaseIn<float>(InitYawDegree, InitYawDegree + DiffYawDegree, CurTime / (TotalTime - CamBlendTime), EaseValue);
					break;
				case ERotateType::VE_RotateEaseOut:
					CurDegree = FMath::InterpEaseOut<float>(InitYawDegree, InitYawDegree + DiffYawDegree, CurTime / (TotalTime - CamBlendTime), EaseValue);
					break;
				case ERotateType::VE_RotateEaseInOut:
					CurDegree = FMath::InterpEaseInOut<float>(InitYawDegree, InitYawDegree + DiffYawDegree, CurTime / (TotalTime - CamBlendTime), EaseValue);
					break;
				}

				FRotator rotator = CurCamera->GetActorRotation();
				rotator.Yaw = CurDegree;
				CurCamera->SetActorRotation(rotator.Quaternion());
			}
			break;
		}
	}
}

void UAnimCamRotateState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimCamRotateState End"));
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
	}

	if (IsValid(CurCamera) && IsValid(CamPivot))
	{
		CurCamera->SetActorLocation(EndPos);
		FRotator rotator = CurCamera->GetActorRotation();
		rotator.Yaw = LastYawDegree;
		CurCamera->SetActorRotation(rotator.Quaternion());
	}
}

