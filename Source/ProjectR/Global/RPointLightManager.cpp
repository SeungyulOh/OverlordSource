// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RPointLightManager.h"


#include "GlobalIntegrated.h"




URPointLightManager::URPointLightManager()
{
	
}

void URPointLightManager::AddPointLight(FVector Location, float Inlifetime, float InAttenuationRadius, FLinearColor InLightColor, float InLightIntensity, bool isHaveTarget)
{
	FTransform transform;
	transform.SetLocation(Location);

	UWorld* world = RGAMEINSTANCE(this)->GetWorld();
	if (!world)
		return;

	ABasePointLight* PointLightDynamic = world->SpawnActor<ABasePointLight>(ABasePointLight::StaticClass(), transform);
	if (PointLightDynamic)
	{

		PointLightDynamic->LifeTime = Inlifetime;
		PointLightDynamic->AttenuationRadius = InAttenuationRadius;
		PointLightDynamic->LightColor = InLightColor;
		PointLightDynamic->LightIntensity = InLightIntensity;

		if (!isHaveTarget)
		{
			PointLightDynamic->PointLight->SetAttenuationRadius(InAttenuationRadius);
			PointLightDynamic->PointLight->SetLightColor(InLightColor);
			PointLightDynamic->PointLight->SetIntensity(0.f);

			PointLightArray.Emplace(PointLightDynamic);
		}
		else
		{

			PointLightDynamic->PointLight->SetAttenuationRadius(InAttenuationRadius);
			PointLightDynamic->PointLight->SetLightColor(InLightColor);
			PointLightDynamic->PointLight->SetIntensity(InLightIntensity);
			PointLightArray.Emplace(PointLightDynamic);

			PointLightDynamic->isFollowingBullet = true;
			if (TargetBulletArray.IsValidIndex(0))
			{
				PointLightDynamic->TargetBullet = (AActor*)(TargetBulletArray[0].Get());
				PointLightDynamic->AttachToActor(PointLightDynamic->TargetBullet.Get(), FAttachmentTransformRules::SnapToTargetIncludingScale);
				TargetBulletArray.RemoveAt(0);
			}
				
		}
		
	}
		
}
