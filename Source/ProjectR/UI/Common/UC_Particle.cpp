// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Particle.h"
#include "GlobalIntegrated.h"




void UUC_Particle::NativeConstruct()
{
	Super::NativeConstruct();

	UClass* CaptureBlueprintActor = RGAMEINSTANCE(this)->CaptureBlueprintActor;
	//UClass* CaptureBlueprintActor = LoadObject<UClass>(this, TEXT("Blueprint'/Game/UMG/Materials/BaseMat/Capture_ParticleActor.Capture_ParticleActor_C'"));
	BP_Capture_ParticleActor =	RGAMEINSTANCE(this)->GetWorld()->SpawnActor<AActor>(CaptureBlueprintActor, Transform_ParticleActor);
	InitialParticle = SelectedParticle;
	SetTemplate_SelectedParticle();
	
	UClass* CaptureBlueprint = RGAMEINSTANCE(this)->CaptureBlueprint;
	//CaptureBlueprint = LoadObject<UClass>(this, TEXT("Blueprint'/Game/UMG/Materials/BaseMat/Capture_Particle.Capture_Particle_C'"));


	BP_Capture_Particle = RGAMEINSTANCE(this)->GetWorld()->SpawnActor<AActor>(CaptureBlueprint, Transform_CaptureParticle);
#if WITH_EDITOR
	if(BP_Capture_Particle!=nullptr)

#endif
	{
		USceneCaptureComponent2D* SceneCaptrueComp = BP_Capture_Particle->FindComponentByClass<USceneCaptureComponent2D>();
		if (SceneCaptrueComp && RenderTarget)
		{
			SceneCaptrueComp->TextureTarget = RenderTarget;

			UMaterial* TargetMaterial = Cast<UMaterial>(Image_Target->Brush.GetResourceObject());
			UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(TargetMaterial, this);
			if (MID)
				MID->SetTextureParameterValue(FName(TEXT("Target")), RenderTarget);

			Image_Target->Brush.SetResourceObject(MID);
		}

		if (bActive_AtSpawned)
			ActivateSelectedParticle();
	}
}

void UUC_Particle::NativeDestruct()
{
	if (BP_Capture_Particle.IsValid())
		BP_Capture_Particle->Destroy();
	if (BP_Capture_ParticleActor.IsValid())
		BP_Capture_ParticleActor->Destroy();

	Super::NativeDestruct();
}

void UUC_Particle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUC_Particle::ActivateSelectedParticle()
{
	USceneCaptureComponent2D* SceneCaptureComp = BP_Capture_Particle->FindComponentByClass<USceneCaptureComponent2D>();
	if (SceneCaptureComp)
	{
		SceneCaptureComp->bCaptureEveryFrame = true;
	}

	UParticleSystemComponent* PSC = BP_Capture_ParticleActor->FindComponentByClass<UParticleSystemComponent>();
	PSC->ActivateSystem();
}

void UUC_Particle::DeactivateSelectedParticle()
{
	UParticleSystemComponent* PSC = BP_Capture_ParticleActor->FindComponentByClass<UParticleSystemComponent>();
	PSC->DeactivateSystem();
}

bool UUC_Particle::isActivated()
{
	UParticleSystemComponent* PSC = BP_Capture_ParticleActor->FindComponentByClass<UParticleSystemComponent>();
	return PSC->IsActive();
}

void UUC_Particle::SwapParticle()
{
	Swap<UParticleSystem*>(SelectedParticle, PendingParticle);
	SetTemplate_SelectedParticle();
}

void UUC_Particle::SetTemplate_SelectedParticle()
{
	if (BP_Capture_ParticleActor.IsValid())
	{
		UParticleSystemComponent* PSC = BP_Capture_ParticleActor.Get()->FindComponentByClass<UParticleSystemComponent>();
		if (PSC && SelectedParticle)
			PSC->SetTemplate(SelectedParticle);
	}
}

void UUC_Particle::Set_InitialState()
{
	if (SelectedParticle != InitialParticle)
		SwapParticle();
}

UParticleSystemComponent* UUC_Particle::GetPSC()
{
	UParticleSystemComponent* PSC = BP_Capture_ParticleActor->FindComponentByClass<UParticleSystemComponent>();
	if (PSC)
		return PSC;

	return nullptr;
}

