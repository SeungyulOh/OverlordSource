// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AttachResourceFXNotify.h"
#include "GlobalIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"

#include "UtilFunctionIntegrated.h"

UAttachResourceFXNotify::UAttachResourceFXNotify()
	: Super()
{
	SpawnLocation = FVector::ZeroVector;
	SpawnRotator = FRotator::ZeroRotator;

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAttachResourceFXNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			if (FXKey.IsValid() && !FXKey.IsNone())
			{
				UUtilFunctionLibrary::SpawnFX(Effect,FXKey, Owner, SpawnLocation, SpawnRotator, SocketName);
			}
		}
	}
}


FString UAttachResourceFXNotify::GetNotifyName_Implementation() const
{
	if (FXKey.IsNone())
	{
		return Super::GetNotifyName_Implementation();
	}
	else
	{
		return FXKey.ToString();
	}
}