// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ScreenCaptureActor.h"

template<typename T>
T* GetComponent(AActor* actor)
{
	if (IsValid(actor) == false) return nullptr;
	return Cast<T>(actor->GetComponentByClass(T::StaticClass()));
}

// Sets default values
AScreenCaptureActor::AScreenCaptureActor() : Super()
{
	ScreenCaptureTexture = nullptr;
}

// Called when the game starts or when spawned
void AScreenCaptureActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameViewport()->ExecCommandDelegate.AddUniqueDynamic(this, &AScreenCaptureActor::ProcessConsoleCommand);
	UGameViewportClient::OnScreenToTextureCaptured().AddUObject(this, &AScreenCaptureActor::OnScreenCaptured);
	//GetWorld()->GetGameViewport()->ScreenToTextureCapturedDelegate.AddUniqueDynamic(this, &AScreenCaptureActor::OnScreenCaptured);
	UMeshComponent * component = GetComponent<UMeshComponent>(this);
	if (component != nullptr)
	{
		UMaterialInterface * material = component->GetMaterial(0);
		if (material != nullptr)
		{
			dynamicmaterial = UMaterialInstanceDynamic::Create(material, component);
			component->SetMaterial(0, dynamicmaterial);
		}
	}
}

void AScreenCaptureActor::ProcessConsoleCommand(FString Command)
{
	const TCHAR* cmd = const_cast<TCHAR*>(*Command);
	if (FParse::Command(&cmd, TEXT("SCREENCAPTURE")))
	{
		RequestScreenCapture();
	}
}

void AScreenCaptureActor::RequestScreenCapture()
{
	GetWorld()->GetGameViewport()->RequestScreenToTexture();
}

void AScreenCaptureActor::OnScreenCaptured(int32 xres, int32 yres, UTexture2D* screencapturetexture)
{
	if (dynamicmaterial != nullptr)
	{
		dynamicmaterial->SetTextureParameterValue(FName("ScreenTexture"), screencapturetexture);
	}
}

