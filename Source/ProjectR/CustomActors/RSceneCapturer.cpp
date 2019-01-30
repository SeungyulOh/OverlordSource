// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RSceneCapturer.h"
#include "CustomActors/RTargettingIndicator.h"


// Sets default values
ARSceneCapturer::ARSceneCapturer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
	SceneCaptureComponent->bCaptureEveryFrame = false;
}

// Called when the game starts or when spawned
void ARSceneCapturer::BeginPlay()
{
	Super::BeginPlay();

	if (SceneCaptureComponent)
	{
		SceneCaptureComponent->bCaptureEveryFrame = true;
		SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

		SceneCaptureComponent->CaptureSceneDeferred();

		FVector2D ViewportSize;
		GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

		RenderTarget = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(GetWorld(), UCanvasRenderTarget2D::StaticClass(), ViewportSize.X, ViewportSize.Y);
		if (RenderTarget)
			SceneCaptureComponent->TextureTarget = RenderTarget;
	}	
}

// Called every frame
void ARSceneCapturer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bActiveCapture)
		return;

	AlignWithCamPosition();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARTargettingIndicator::StaticClass(), OutActors);
	SceneCaptureComponent->ClearShowOnlyComponents(nullptr);
	for (size_t i = 0; i < OutActors.Num(); ++i)
	{
		SceneCaptureComponent->ShowOnlyActorComponents(OutActors[i]);
	}
}

void ARSceneCapturer::SetFOV(float FOV)
{
	if (SceneCaptureComponent)
	{
		SceneCaptureComponent->FOVAngle = FOV;
	}
}

void ARSceneCapturer::AlignWithCamPosition()
{
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (PCM)
	{
		SetActorLocationAndRotation(PCM->GetCameraLocation(), PCM->GetCameraRotation().Quaternion());
	}
}

