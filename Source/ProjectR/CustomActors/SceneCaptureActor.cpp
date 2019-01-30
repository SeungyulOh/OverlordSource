// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SceneCaptureActor.h"

// Called when the game starts or when spawned
void ASceneCaptureActor::BeginPlay()
{
	Super::BeginPlay();

}


void ASceneCaptureActor::SetArmDistance(float InDistance)
{
	USpringArmComponent* ArmComponent = Cast<USpringArmComponent>( GetComponentByClass(USpringArmComponent::StaticClass()) );
	if (ArmComponent)
		ArmComponent->TargetArmLength = InDistance;
}
