// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Item/WeaponBase.h"

#include "CustomChangeMaterialComponent.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "UtilFunctionIntegrated.h"


// Sets default values for this component's properties
UCustomChangeMaterialComponent::UCustomChangeMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	OwnerActor = GetOwner();
	IsActive = false;
	ActiveTime = 0.5f;
	ElapsedActiveTime = 0.0f;
	CurrentCustomMaterial = nullptr;
}


// Called when the game starts
void UCustomChangeMaterialComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();
	IsActive = false;
	ActiveTime = 0.5f;
	ElapsedActiveTime = 0.0f;
	CurrentCustomMaterial = nullptr;	
}

void UCustomChangeMaterialComponent::Active(bool InActive, UMaterialInterface* InCustomMaterial)
{
	//true == true, false == false
	if (IsActive == InActive)
	{
		if (InCustomMaterial == nullptr || CurrentCustomMaterial.Get() == InCustomMaterial)
		{
			return;
		}
		else if (IsActive == true)
		{
			ResetMaterials();
		}
	}
	else
	{
		if (CurrentCustomMaterial.Get() != InCustomMaterial && IsActive)
		{
			ResetMaterials();
		}
	}
	if (InActive)
	{
		SetCustomMaterial(InCustomMaterial);
	}
	else
	{
		ResetMaterials();
	}
	IsActive = InActive;
	CurrentCustomMaterial = InCustomMaterial;
}

void UCustomChangeMaterialComponent::ActivateDeathMaterials()
{
	ResetMaterials();

	{
		auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (OwnerActor_BattleEntityComponent == nullptr)
			return;

		auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
		if (OwnerActor_EntityRenderComponent == nullptr)
			return;

		ACharacter* OwnerCharacter = Cast< ACharacter>(OwnerActor.Get());
		if (OwnerCharacter)
		{
			ApplySkeletalMeshCompDeathMaterials(OwnerCharacter->GetMesh(), OwnerActor_EntityRenderComponent->OriginalMaterialList);
			ApplySkeletalMeshCompDeathMaterials(OwnerActor_EntityRenderComponent->SubDecoMesh.Get(), OwnerActor_EntityRenderComponent->SubMeshOriginalMaterialList);
		}
	}

	IsActive = true;
}

void UCustomChangeMaterialComponent::ChangeOpacity(float Opacity)
{
	if (FMath::IsNearlyEqual(Opacity, 1.0f))
	{
		Active(false, nullptr);
	}
	else
	{
		auto OpacityMaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromTable(PARAMETER_TRANSLUCENT);

		Active(true, OpacityMaterialInstance);
		SetCustomMaterialScalarParameterValue(PARAMETER_OPACITY, Opacity);
	}
}

void UCustomChangeMaterialComponent::SetCustomMaterial(UMaterialInterface* InCustomMaterial)
{
	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (OwnerActor_BattleEntityComponent == nullptr)
		return;
	
	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent == nullptr)
		return;

	ACharacter* OwnerCharacter = Cast< ACharacter>(OwnerActor.Get());
	if ( OwnerCharacter)
	{
		ApplySkeletalMeshCompImp(OwnerCharacter->GetMesh(), OwnerActor_EntityRenderComponent->OriginalMaterialList, InCustomMaterial);
			
		//weapon			
		if (OwnerActor_BattleEntityComponent->RWeapon.IsValid())
		{
			ApplyWeaponImp(OwnerActor_BattleEntityComponent->RWeapon.Get(), InCustomMaterial);
		}
		if (OwnerActor_BattleEntityComponent->LWeapon.IsValid())
		{
			ApplyWeaponImp(OwnerActor_BattleEntityComponent->LWeapon.Get(), InCustomMaterial);
		}
			
		if (OwnerActor_EntityRenderComponent->SubDecoMesh.IsValid())
		{
			ApplySkeletalMeshCompImp(OwnerActor_EntityRenderComponent->SubDecoMesh.Get(), OwnerActor_EntityRenderComponent->SubMeshOriginalMaterialList, InCustomMaterial);
		}
	}
}

void UCustomChangeMaterialComponent::ResetMaterials()
{
	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (OwnerActor_BattleEntityComponent == nullptr)
		return;

	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent == nullptr)
		return;

	ACharacter* OwnerCharacter = Cast< ACharacter>(OwnerActor.Get());
	if (OwnerCharacter)
	{
		int32 MaterialMax = OwnerActor_EntityRenderComponent->OriginalMaterialList.Num();
		if (IsValid(OwnerCharacter->GetMesh()))
		{
			for (int32 Index = 0; Index < MaterialMax; ++Index)
			{
				OwnerCharacter->GetMesh()->SetMaterial(Index, OwnerActor_EntityRenderComponent->OriginalMaterialList[Index]);
			}

			if (OwnerActor_BattleEntityComponent->RWeapon.IsValid() && IsValid(OwnerActor_BattleEntityComponent->RWeapon->WeaponMeshRef))
			{
				int32 WeaponMaterialMax = OwnerActor_BattleEntityComponent->RWeapon->OriginalMaterialList.Num();
				for (int32 Index = 0; Index < WeaponMaterialMax; ++Index)
				{
					OwnerActor_BattleEntityComponent->RWeapon->WeaponMeshRef->SetMaterial(Index, OwnerActor_BattleEntityComponent->RWeapon->OriginalMaterialList[Index]);
				}
			}
			if (OwnerActor_BattleEntityComponent->LWeapon.IsValid() && IsValid(OwnerActor_BattleEntityComponent->LWeapon->WeaponMeshRef))
			{
				int32 WeaponMaterialMax = OwnerActor_BattleEntityComponent->LWeapon->OriginalMaterialList.Num();
				for (int32 Index = 0; Index < WeaponMaterialMax; ++Index)
				{
					OwnerActor_BattleEntityComponent->LWeapon->WeaponMeshRef->SetMaterial(Index, OwnerActor_BattleEntityComponent->LWeapon->OriginalMaterialList[Index]);
				}
			}

			if (OwnerActor_EntityRenderComponent->SubDecoMesh.IsValid())
			{
				int32 SubMaterialMax = OwnerActor_EntityRenderComponent->SubMeshOriginalMaterialList.Num();
				for (int32 Index = 0; Index < SubMaterialMax; ++Index)
				{
					OwnerActor_EntityRenderComponent->SubDecoMesh->SetMaterial(Index, OwnerActor_EntityRenderComponent->SubMeshOriginalMaterialList[Index]);
				}
			}
		}
	}
}

UMaterial* UCustomChangeMaterialComponent::GetDeathMaterial(UMaterial* InMaterial)
{
	FString DeathMaterialPath;
	if (!GetDeathMaterialPath(InMaterial->GetPathName(), DeathMaterialPath))
	{
		return nullptr;
	}

	TSoftObjectPtr<UMaterial> DeathMaterialRef(DeathMaterialPath);
	UMaterial* DeathMaterial = DeathMaterialRef.Get();
	if (!IsValid(DeathMaterial))
	{
		DeathMaterial = DeathMaterialRef.LoadSynchronous();
	}

	return DeathMaterial;
}

bool UCustomChangeMaterialComponent::GetDeathMaterialPath(const FString& InPath, FString& OutPath)
{
	// ?
	int32 Index = InPath.Find(TEXT("/"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	if (Index == INDEX_NONE)
	{
		UE_LOG(LogGamePlay, Log, TEXT("UCustomChangeMaterialComponent::GetDeathMaterialPath: unexpected InPath: %s"), *InPath);
		return false;
	}

	OutPath = InPath.Mid(0, Index);
	OutPath += TEXT("/Death");
	OutPath += InPath.Mid(Index);

	//UE_LOG(LogGamePlay, Log, TEXT("GetDeathMaterialPath: %s => %s"), *InPath, *OutPath);

	return true;
}

//ApplyStaticMeshCompImp
void UCustomChangeMaterialComponent::ApplyWeaponImp(AWeaponBase* Weapon, UMaterialInterface* InCustomMaterial)
{
	if ( Weapon == nullptr || Weapon->WeaponMeshRef == nullptr || InCustomMaterial == nullptr)
		return;

	TArray<UMaterialInstanceDynamic *> InOriginArray = Weapon->OriginalMaterialList;
	int32 MaterialMax = InOriginArray.Num();
	for (int32 Index = 0; Index < MaterialMax; ++Index)
	{
		UMaterialInstanceDynamic* MaterialInstance = Weapon->WeaponMeshRef->CreateDynamicMaterialInstance(Index, InCustomMaterial);

		if (MaterialInstance)
		{
			UTexture* OutValue = nullptr;
			if (Weapon->OriginalMaterialList.IsValidIndex(Index))
			{
				InOriginArray[Index]->GetTextureParameterValue(PARAMETER_DIFFUSE_TEXTURE, OutValue);
				Weapon->WeaponMeshRef->SetMaterial(Index, MaterialInstance);
				MaterialInstance->SetTextureParameterValue(PARAMETER_DIFFUSE_TEXTURE, OutValue);
			}
		}
	}
}

void UCustomChangeMaterialComponent::ApplySkeletalMeshCompImp(USkeletalMeshComponent* InTargetMesh, TArray< UMaterialInstanceDynamic* > InOriginArray, UMaterialInterface* InCustomMaterial)
{
	if (InTargetMesh == nullptr || InCustomMaterial == nullptr)
		return;

	int32 MaterialMax = InOriginArray.Num();
	for (int32 Index = 0; Index < MaterialMax; ++Index)
	{
		UMaterialInstanceDynamic* MaterialInstance = InTargetMesh->CreateDynamicMaterialInstance(Index, InCustomMaterial);

		if (MaterialInstance)
		{
			UTexture* OutValue = nullptr;
			if (InOriginArray.IsValidIndex(Index))
			{
				auto InOriginMat = InOriginArray[Index];
				if ( IsValid( InOriginMat ))
				{
					// DEBUG - determine path for base material
					UMaterial* BaseMaterial = InOriginMat->GetMaterial();
					//UE_LOG(LogGamePlay, Log, TEXT("Material Name: %s, PathName: %s"), *BaseMaterial->GetName(), *BaseMaterial->GetPathName());
					
					InOriginMat->GetTextureParameterValue(PARAMETER_DIFFUSE_TEXTURE, OutValue);
					InTargetMesh->SetMaterial(Index, MaterialInstance);
					MaterialInstance->SetTextureParameterValue(PARAMETER_DIFFUSE_TEXTURE, OutValue);
				}
			}			
		}
	}
}


void UCustomChangeMaterialComponent::ApplySkeletalMeshCompDeathMaterials(USkeletalMeshComponent* InTargetMesh, TArray< UMaterialInstanceDynamic* > InOriginArray)
{
	if (InTargetMesh == nullptr)
		return;

	int32 MaterialMax = InOriginArray.Num();
	for (int32 Index = 0; Index < MaterialMax; ++Index)
	{

		UTexture* OutValue = nullptr;
		if (InOriginArray.IsValidIndex(Index))
		{
			auto InOriginMat = InOriginArray[Index];
			if (IsValid(InOriginMat))
			{
				UMaterial* BaseMaterial = InOriginMat->GetMaterial();
				//UE_LOG(LogGamePlay, Log, TEXT("ApplySkeletalMeshCompDeathMaterials: Material Name: %s, PathName: %s"), *BaseMaterial->GetName(), *BaseMaterial->GetPathName());

				UMaterial* DeathEffectMaterial = GetDeathMaterial(BaseMaterial);
				if (IsValid(DeathEffectMaterial))
				{
					UMaterialInstanceDynamic* MaterialInstance = InTargetMesh->CreateDynamicMaterialInstance(Index, DeathEffectMaterial);

					InOriginMat->GetTextureParameterValue(PARAMETER_DIFFUSE_TEXTURE, OutValue);
					InTargetMesh->SetMaterial(Index, MaterialInstance);
					MaterialInstance->SetTextureParameterValue(PARAMETER_DIFFUSE_TEXTURE, OutValue);
				}
				else
				{
					UE_LOG(LogGamePlay, Log, TEXT("ApplySkeletalMeshCompDeathMaterials: Corresponding death effect material not found for %s"), *BaseMaterial->GetName());
				}
			}
		}
	}
}

void UCustomChangeMaterialComponent::SetCustomMaterialScalarParameterValue(FName ParamName, float Value)
{
	if (OwnerActor.IsValid())
	{
		auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
		if (OwnerActor_EntityRenderComponent == nullptr)
			return;

		// Character
		ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
		if( OwnerCharacter )
			ApplyCustomMaterialScalarParameterValue(OwnerCharacter->GetMesh()->GetMaterials(), ParamName, Value);

		auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (OwnerActor_BattleEntityComponent)
		{
			// RWeapon
			if (OwnerActor_BattleEntityComponent->RWeapon.IsValid())
			{
				AWeaponBase* WeaponPtr = OwnerActor_BattleEntityComponent->RWeapon.Get();
				if (WeaponPtr && WeaponPtr->WeaponMeshRef)
				{
					ApplyCustomMaterialScalarParameterValue(WeaponPtr->WeaponMeshRef->GetMaterials(), ParamName, Value);
				}
			}
			// LWeapon
			if (OwnerActor_BattleEntityComponent->LWeapon.IsValid())
			{
				AWeaponBase* WeaponPtr = OwnerActor_BattleEntityComponent->LWeapon.Get();
				if (WeaponPtr && WeaponPtr->WeaponMeshRef)
				{
					ApplyCustomMaterialScalarParameterValue(WeaponPtr->WeaponMeshRef->GetMaterials(), ParamName, Value);
				}
			}
		}

		// Deco
		if (OwnerActor_EntityRenderComponent->SubDecoMesh.IsValid())
		{
			USkeletalMeshComponent* SubDecoMeshPtr = OwnerActor_EntityRenderComponent->SubDecoMesh.Get();
			if (SubDecoMeshPtr)
			{
				ApplyCustomMaterialScalarParameterValue(SubDecoMeshPtr->GetMaterials(), ParamName, Value);
			}
		}
	}
}

void UCustomChangeMaterialComponent::ApplyCustomMaterialScalarParameterValue(TArray<UMaterialInterface*> MaterialList, FName ParamName, float Value)
{
	int32 MaterialMax = MaterialList.Num();
	for (int32 Index = 0; Index < MaterialMax; ++Index)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast< UMaterialInstanceDynamic >(MaterialList[Index]);
		if (MaterialInstance)
		{
			MaterialInstance->SetScalarParameterValue(ParamName, Value);
		}
	}
}
