// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "WeaponBase.h"

#include "SharedConstants/GlobalConstants.h"





AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer) : Super( ObjectInitializer )
{
	WeaponMeshRef	= nullptr;
	TrailRef		= nullptr;
	TrailWidth		= 1.0f;
	TrailWidthMode = ETrailWidthModeEnum::VE_Second;
	TrailFirstSocket = FName("WP_trail01");
	TrailSecondSocket = FName("WP_trail02");	
	ShotPointRef = nullptr;

	SetActorTickEnabled(false);
}

void AWeaponBase::StartTrail() 
{
	if (IsValid(TrailRef))
	{
		TrailRef->BeginTrails(TrailFirstSocket, TrailSecondSocket, GetEngineTrailWidthMode( TrailWidthMode ), TrailWidth);
	}
}

void AWeaponBase::EndTrail()
{
	if (IsValid(TrailRef))
	{
		TrailRef->EndTrails();
	}
}

void AWeaponBase::SetCastShadow(bool Active)
{
	UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>( GetComponentByClass(UStaticMeshComponent::StaticClass()) );
	if (StaticMeshComp)
	{
		StaticMeshComp->SetCastShadow(Active);
	}
}

ETrailWidthMode AWeaponBase::GetEngineTrailWidthMode(ETrailWidthModeEnum GameTrailWidthMode)
{
	switch (GameTrailWidthMode)
	{
		case ETrailWidthModeEnum::VE_TCentre:
			return ETrailWidthMode::ETrailWidthMode_FromCentre;
		case ETrailWidthModeEnum::VE_First:
			return ETrailWidthMode::ETrailWidthMode_FromFirst;
		case ETrailWidthModeEnum::VE_Second:
			return ETrailWidthMode::ETrailWidthMode_FromSecond;
	}

	return ETrailWidthMode::ETrailWidthMode_FromCentre;
}

void AWeaponBase::BeginPlay()
{
	if (IsValid(WeaponMeshRef))
	{
		auto Materials = WeaponMeshRef->GetMaterials();
		OriginalMaterialList.Reset(Materials.Num());
		for (int32 i = 0; i < Materials.Num(); ++i)
		{
			UMaterialInstanceDynamic* dynamicInstance = UMaterialInstanceDynamic::Create(Materials[i], this);
			if (IsValid(dynamicInstance))
			{
				OriginalMaterialList.Emplace(dynamicInstance);
			}
		}

		WeaponMeshRef->SetCanEverAffectNavigation(false);
	}
}

void AWeaponBase::Destroyed()
{
	//WeaponMeshRef->K2_DestroyComponent(this);
	/*WeaponMeshRef = nullptr;
	TrailRef = nullptr;
	ShotPointRef = nullptr;
	OriginalMaterialList.Empty();*/
	
	Super::Destroyed();
}

void AWeaponBase::SetVisibility(bool bVisible)
{
	if (WeaponMeshRef)
	{
		WeaponMeshRef->SetVisibility(bVisible);
	}
}