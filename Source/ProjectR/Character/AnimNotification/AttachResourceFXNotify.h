// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttachResourceFXNotify.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API UAttachResourceFXNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAttachResourceFXNotify();
	TWeakObjectPtr<UParticleSystemComponent> Effect;
	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	// End UAnimNotify interface

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachResourceFXNotify)
	FName				FXKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachResourceFXNotify)
	FVector				SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachResourceFXNotify)
	FRotator			SpawnRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttachResourceFXNotify)
	FName				SocketName;
};
