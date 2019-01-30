// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CustomAmbientComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "GlobalIntegrated.h"

// Sets default values for this component's properties
UCustomAmbientComponent::UCustomAmbientComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCustomAmbientComponent::InitializeComponent()
{
	Super::InitializeComponent();

	 OwnerActor = GetOwner();		
}


// Called every frame
void UCustomAmbientComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
#ifndef RULE_TIMERACTOR
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
#endif

	if (IsActive)
	{
		ElapsedActiveTime += DeltaTime;
		if (ElapsedActiveTime >= ActiveTime)
		{
			IsActive = false;

			FColor BaseColor = GetModifiedColor(0.0f);
			if (UnderTargetEffect)
			{
				EnterTargetEffect();
			}
			else
			{
				if (IsForceActive == false)
				{
					SetCustomAmbient(GetModifiedColor(0.0f));
				}
				else
				{
					ActiveTime = ForceActiveTime;
					ElapsedActiveTime = 0.0f;
					LightColor = ForceLightColor;
					SetCustomAmbient(GetModifiedColor(ForceStartIntensity));
				}
			}
		}		
	}
}

void UCustomAmbientComponent::Active(float activeTime, float StartIntensity, FColor CustomLightColor, bool bIsForceColor/* = false*/)
{

	IsActive = true;
	ActiveTime = activeTime;
	ElapsedActiveTime = 0.0f;
	LightColor = CustomLightColor;	
	SetCustomAmbient(GetModifiedColor(StartIntensity));
	if (bIsForceColor)
	{
		IsForceActive = bIsForceColor;
		ForceLightColor = CustomLightColor;
		ForceActiveTime = activeTime;
		ForceStartIntensity = StartIntensity;
	}
}

void UCustomAmbientComponent::SetCustomAmbient(FColor InColor)
{
	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent == nullptr)
		return;

	if ( OwnerActor.IsValid() )
	{
		for (auto OriginalMaterial : OwnerActor_EntityRenderComponent->OriginalMaterialList)
		{
			if ( IsValid( OriginalMaterial ) )
			{
				OriginalMaterial->SetVectorParameterValue(FName(TEXT("CustomLightColor")), InColor);
			}
			else
			{
				UE_LOG(LogCriticalErrors, Log, TEXT("UCustomAmbientComponent Material NULL - %s"), *(OwnerActor->GetName()));
			}
		}
	}
}

FColor UCustomAmbientComponent::GetModifiedColor(float InIntensity)
{
	return FColor(LightColor.R* InIntensity, LightColor.G* InIntensity, LightColor.B* InIntensity);	
}

void UCustomAmbientComponent::EnterTargetEffect()
{
	UnderTargetEffect = true;
	LightColor = RGAMEINSTANCE(this)->GameEnvironmentVar.TargetEffectLightColor;
	SetCustomAmbient(GetModifiedColor(RGAMEINSTANCE(this)->GameEnvironmentVar.TargetEffectIntensity));
}

void UCustomAmbientComponent::ExitTargetEffect()
{
	UnderTargetEffect = false;
	SetCustomAmbient(GetModifiedColor(0.0f));
}
