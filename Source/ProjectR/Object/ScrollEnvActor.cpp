// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ScrollEnvActor.h"
#include "UtilFunctionIntegrated.h"
#include "Runtime/Engine/Classes/Matinee/MatineeActor.h"

#include "CustomActors/BaseSequenceActor.h"


// Sets default values
AScrollEnvActor::AScrollEnvActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScrollEnvActor::BeginPlay()
{
	Super::BeginPlay();

	PrepareScroll();
}

// Called every frame
void AScrollEnvActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bActive)
		ScrollTick(DeltaTime);

	if (UGamePlayBluePrintFunction::GetElevator() == true)
	{
		UGamePlayBluePrintFunction::SetElevator(false, UGamePlayBluePrintFunction::GetDuration());
		BlendToMatineeCamera(true);
		GoToNextFloor(UGamePlayBluePrintFunction::GetDuration());
	}
	if (UGamePlayBluePrintFunction::GetDuration() == 0.0f)
	{
		GoToNextFloor(0.0f);
	}
}

void AScrollEnvActor::PrepareScroll()
{
	if( IsValid( FirstGroup ) )
		OriginFirstPos = FirstGroup->GetComponentLocation();
	if( IsValid( SecondGroup ) )
		OriginSecondPos = SecondGroup->GetComponentLocation();

	ScrollDistance = OriginFirstPos - OriginSecondPos;
}

void AScrollEnvActor::StartScroll(float InDuration)
{
	bActive = true;
	Duration = InDuration;
	ElapsedScrollTime = 0.0f;
}

void AScrollEnvActor::ScrollTick(float DeltaSeconds)
{
	ElapsedScrollTime += DeltaSeconds;
	if (ElapsedScrollTime < Duration)
	{
		float ScrollRatio = ElapsedScrollTime / Duration;
		FVector ScrollOffset = ScrollDistance * ScrollRatio;
		if (IsValid(FirstGroup))
			FirstGroup->SetRelativeLocation(OriginFirstPos + ScrollOffset);
		if (IsValid(SecondGroup))
			SecondGroup->SetRelativeLocation(OriginSecondPos + ScrollOffset);
	}
	else
	{
		/*
		if (IsValid(FirstGroup))
			FirstGroup->SetRelativeLocation(OriginFirstPos + ScrollDistance);
		if (IsValid(SecondGroup))
			SecondGroup->SetRelativeLocation(OriginSecondPos + ScrollDistance);
			*/
		bActive = false;
		SwapScrollComponent();
	}
}

void AScrollEnvActor::SwapScrollComponent()
{
	if (IsValid(FirstGroup) && IsValid(SecondGroup))
	{
		Swap(FirstGroup, SecondGroup);
		FirstGroup->SetRelativeLocation(OriginFirstPos);
		SecondGroup->SetRelativeLocation(OriginSecondPos);
	}
}

void AScrollEnvActor::GoToNextFloor( float InDuration )
{
	StartScroll( InDuration );
	//PlayMatinee();
	PlaySequence();
}

void AScrollEnvActor::PlayMatinee()
{	
	URGameInstance* gameInstance = RGAMEINSTANCE(this);
	if (gameInstance == nullptr)
		return;

	AMatineeActor* MatineeActor = UUtilFunctionLibrary::GetMatineeActor(gameInstance->GetWorld(), MatineeActorName);
	if (MatineeActor)
	{
		MatineeActor->SetPosition(0.0f);
		MatineeActor->Play();
	}
}

void AScrollEnvActor::PlaySequence()
{
	URGameInstance* gameInstance = RGAMEINSTANCE(this);
	if (gameInstance == nullptr)
		return;

	if (IsValid(gameInstance->SequenceManager))
	{
		FName sequenceKey = TEXT("Elevator_Sequence");
		ABaseSequenceActor* SequenceActor = gameInstance->SequenceManager->FindSequence(sequenceKey);
		if (IsValid(SequenceActor))
		{
			SequenceActor->OnSequenceEndEvent.RemoveDynamic(this, &AScrollEnvActor::OnSequenceEnd);
			SequenceActor->OnSequenceEndEvent.AddDynamic(this, &AScrollEnvActor::OnSequenceEnd);
			SequenceActor->Play();
		}
		else
		{
			SequenceActor = gameInstance->SequenceManager->SpawnSequence(sequenceKey);
			if (IsValid(SequenceActor))
			{
				SequenceActor->OnSequenceEndEvent.RemoveDynamic(this, &AScrollEnvActor::OnSequenceEnd);
				SequenceActor->OnSequenceEndEvent.AddDynamic(this, &AScrollEnvActor::OnSequenceEnd);
			}
		}
	}	
}

void AScrollEnvActor::OnSequenceEnd(FName name)
{
	BlendToMatineeCamera(false);
}

void AScrollEnvActor::BlendToMatineeCamera(bool InBlendToMatinee)
{
	URGameInstance* gameInstance = RGAMEINSTANCE(this);
	if (gameInstance == nullptr)
		return;

	if (InBlendToMatinee)
	{
		ACameraActor* MatineeInCameraActor = UUtilFunctionLibrary::GetMatineeCameraActor(gameInstance->GetWorld(), MatineeCameraName);
		APlayerController* Controller = UGameplayStatics::GetPlayerController(gameInstance->GetWorld(), 0);
		if (IsValid(Controller))
		{
			Controller->SetViewTargetWithBlend(MatineeInCameraActor, 0.5f);
		}
	}
	else
	{
		APlayerController* Controller = UGameplayStatics::GetPlayerController(gameInstance->GetWorld(), 0);
		if (IsValid(Controller))
		{
			Controller->SetViewTargetWithBlend(gameInstance->CameraActor, 0.5f);
		}
	}
}