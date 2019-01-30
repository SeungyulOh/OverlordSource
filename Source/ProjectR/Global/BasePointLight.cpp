// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BasePointLight.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"




// Sets default values
ABasePointLight::ABasePointLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetCastShadows(false);
	PointLight->IntensityUnits = ELightUnits::Candelas;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> PLCurve(TEXT("CurveFloat'/Game/Blueprints/LightningDirectionalLight/CurvePointLight.CurvePointLight'"));
	if (PLCurve.Object)
	{
		PointLightCurve = PLCurve.Object;
	}

}

// Called when the game starts or when spawned
void ABasePointLight::BeginPlay()
{
	Super::BeginPlay();

	PointLightManager = RGAMEINSTANCE(this)->PointLightManager;


	for (auto& postProcessVolume : GetWorld()->PostProcessVolumes)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			PrevVignetteIntensity = PPVolume->Settings.VignetteIntensity;
			break;
		}
	}	
}

// Called every frame
void ABasePointLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isFollowingBullet == false)
	{
		if (ElapsedTime < LifeTime)
		{
			ElapsedTime += DeltaTime;
			float CurrentIntensity = PointLightCurve->GetFloatValue(ElapsedTime / LifeTime);
			PointLight->SetIntensity(CurrentIntensity * LightIntensity);

			URenderFunctionLibrary::SetVignetteIntensity(PrevVignetteIntensity + CurrentIntensity);
		}
		else
		{
			URenderFunctionLibrary::SetVignetteIntensity(PrevVignetteIntensity);
			int32 iFoundIdx = 0;
			PointLightManager->PointLightArray.Find(this, iFoundIdx);
			if (iFoundIdx != INDEX_NONE)
				PointLightManager->PointLightArray.RemoveAt(iFoundIdx);

			this->Destroy();
		}
	}
	else
	{
		if (!TargetBullet.IsValid())
		{
			int32 iFoundIdx = 0;
			PointLightManager->PointLightArray.Find(this, iFoundIdx);
			if (iFoundIdx != INDEX_NONE)
				PointLightManager->PointLightArray.RemoveAt(iFoundIdx);

			this->Destroy();
		}
	}
	
		

}

