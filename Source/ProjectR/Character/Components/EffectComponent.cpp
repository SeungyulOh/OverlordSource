// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EffectComponent.h"


#include "UtilFunctionIntegrated.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/CustomChangeMaterialComponent.h"
#include "Character/Components/CustomAmbientComponent.h"
#include "DirectionTrackManager.h"
#include "GlobalIntegrated.h"


// Sets default values for this component's properties
UEffectComponent::UEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEffectComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	OwnerActor = GetOwner();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		HitHightlightDuration = RGameInstance->GameEnvironmentVar.HitHightlightDuration;
	}

#ifndef RULE_TIMERACTOR
	if (!CustomAmbientComponent.IsValid())
	{
		CustomAmbientComponent = Cast< UCustomAmbientComponent >(OwnerActor->GetComponentByClass(UCustomAmbientComponent::StaticClass()));
	}
#endif

	if (!CustomChangeMaterialComponent.IsValid())
	{
		CustomChangeMaterialComponent = UCharacterHelpFunctionLibrary::GetCustomMaterialComponent(OwnerActor.Get());
	}
}

void UEffectComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	if (bHitHight)
	{
		auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (OwnerActor_BattleEntityComponent)
		{
			OwnerActor_BattleEntityComponent->OnReceiveDamageDelegate.RemoveDynamic(this, &UEffectComponent::OnReceiveDamage);
		}
	}
}

// Called every frame
void UEffectComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
#ifndef RULE_TIMERACTOR
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#endif

	switch (CurEffectType)
	{
	case EEffectTypeEnum::VE_AppearEffect:
		AppearEffectTick(DeltaTime);
		break;
	case EEffectTypeEnum::VE_DisappearEffect:
		DisappearEffectTick(DeltaTime);
		break;
	case EEffectTypeEnum::VE_LightFlicker:
		LightFlickerTick(DeltaTime);
		break;
	}
}

void UEffectComponent::ChangeOpacity(float Opacity)
{
	if (CustomChangeMaterialComponent.IsValid())
	{
		if (FMath::IsNearlyEqual(Opacity, 1.0f))
		{
			CustomChangeMaterialComponent->Active(false, nullptr);
		}
		else
		{
			if (OpacityMaterialInstance == nullptr)
			{
				OpacityMaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromTable(PARAMETER_TRANSLUCENT);
			}

			if (IsValid(OpacityMaterialInstance))
			{
				CustomChangeMaterialComponent->Active(true, OpacityMaterialInstance);
				CustomChangeMaterialComponent->SetCustomMaterialScalarParameterValue(PARAMETER_OPACITY, Opacity);
			}
		}
	}
}

void UEffectComponent::OnReceiveDamage(AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor)
{
	if (!OwnerActor.IsValid())
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	if (CustomAmbientComponent.IsValid())
	{
		ABasePlayerController* PlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
		if(PlayerCtrl->IsSelectedCharacter(OwnerActor.Get()))
		{
			switch (DamageResultType)
			{
			case EBattleDamageResultEnum::VE_Critical:
// 			case EBattleDamageResultEnum::VE_MagicCritical:
				CustomAmbientComponent->Active(HitHightlightDuration, 1.0f, RGameInstance->GameEnvironmentVar.HitCriticalHightlightColor);
				break;
			default:
				CustomAmbientComponent->Active(HitHightlightDuration, 1.0f, RGameInstance->GameEnvironmentVar.HitHightlightColor);
				break;
			}
		}
		else
		{
			switch (DamageResultType)
			{
			case EBattleDamageResultEnum::VE_Critical:
// 			case EBattleDamageResultEnum::VE_MagicCritical:
				CustomAmbientComponent->Active(RGameInstance->GameEnvironmentVar.ControlledCharacter_HitHightlightDuration, 1.0f, RGameInstance->GameEnvironmentVar.ControlledCharacter_HitCriticalHightlightColor);
				break;
			default:
				CustomAmbientComponent->Active(RGameInstance->GameEnvironmentVar.ControlledCharacter_HitHightlightDuration, 1.0f, RGameInstance->GameEnvironmentVar.ControlledCharacter_HitHightlightColor);
				break;
			}
		}
	}
}

void UEffectComponent::SetProperty(float InInterval, float InDuration)
{
	FlickingInterval = InInterval;
	FlickingDuration = InDuration;
}

void UEffectComponent::LightFlickerStart() //none
{
	if (!OwnerActor.IsValid())
		return;

	CurEffectType = EEffectTypeEnum::VE_LightFlicker;

	ElapsedTime = 0.0f;
	bActive = true;
}

void UEffectComponent::AppearEffectStart(float InDuration)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
	if (IsValid(OwnerCharacter) == false)
		return;

	CurEffectType = EEffectTypeEnum::VE_AppearEffect;

	Duration = InDuration;
	ElapsedTime = 0.0f;
	bActive = true;

	BaseHeight = OwnerCharacter->GetActorLocation();
	BaseHeight.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 20.0f;
	MaxHeight = OwnerCharacter->GetActorLocation();
	MaxHeight.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	if (CustomChangeMaterialComponent.IsValid())
	{
		UMaterialInstance* DissolveMaterial = UUtilFunctionLibrary::GetMaterialInstanceFromTable(TEXT("SpawnInst"));
		if (DissolveMaterial)
		{
			CustomChangeMaterialComponent->Active(true, DissolveMaterial->GetMaterial());
		}
	}

	bool bEnemy = URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerActor.Get());

#ifdef RULE_TESTDELETE
	//FName TargetName;
	//((bEnemy) ? TargetName = TEXT("MaskingEnemyEffect") : TargetName = TEXT("MaskingEnemyEffect"));
	//UParticleSystem* SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TargetName);
#endif

	UParticleSystem* SpawnEffectClass = nullptr;

	if (false == bEnemy)
	{
		SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("MaskingFriendEffect"));
	}
	else
	{
		SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("MaskingEnemyEffect"));
	}

	if (IsValid(SpawnEffectClass))
	{
		SpawnEffect = UUtilFunctionLibrary::CustomSpawnEmitterAtLocation(OwnerActor->GetWorld(), SpawnEffectClass, MaxHeight);
		static float StandardRadius = 96.0f;

		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (SpawnEffect.IsValid() && IsValid(BattleEntityComponent))
		{
			float UnitRadius = BattleEntityComponent->GetBattleInfo()->CharacterProperty.UnitRadius;
			float EffectScale = UnitRadius / StandardRadius;

			SpawnEffect->SetRelativeScale3D(FVector(EffectScale, EffectScale, EffectScale));
		}
	}
}

void UEffectComponent::DisappearEffectStart(float InDuration)
{
	if (!OwnerActor.IsValid())
		return;
	
	auto BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (nullptr == BaseProperty)
		return;

	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent == nullptr)
		return;

	CurEffectType = EEffectTypeEnum::VE_DisappearEffect;

	Duration = InDuration;
	ElapsedTime = 0.0f;
	bActive = true;

	if (CustomChangeMaterialComponent.IsValid())
	{
		CustomChangeMaterialComponent->ActivateDeathMaterials();		
	}

	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (OwnerActor_BattleEntityComponent)
		OwnerActor_BattleEntityComponent->DestroyWeaponsBase();	
}

void UEffectComponent::HitHightlightStart()
{
	bHitHight = true;

	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (OwnerActor_BattleEntityComponent)
	{
		OwnerActor_BattleEntityComponent->OnReceiveDamageDelegate.AddDynamic(this, &UEffectComponent::OnReceiveDamage);
	}
}

void UEffectComponent::SetAppearEffectMaterialParam(float ProgressRatio)
{
	if (!OwnerActor.IsValid())
		return;

	float ProgressPos = FMath::InterpEaseIn(MaxHeight.Z, BaseHeight.Z, ProgressRatio, 2.0f);

	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
	if (IsValid(OwnerCharacter))
	{
		TArray< UMaterialInterface* >	DynamicMaterials = OwnerCharacter->GetMesh()->GetMaterials();
		ApplyScalarMaterialParam(DynamicMaterials, FName("ClipProgressPos"), ProgressPos);

		auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
		if (OwnerActor_BattleEntityComponent)
		{
			if (OwnerActor_BattleEntityComponent->RWeapon.IsValid() && IsValid(OwnerActor_BattleEntityComponent->RWeapon->WeaponMeshRef))
			{
				DynamicMaterials = OwnerActor_BattleEntityComponent->RWeapon->WeaponMeshRef->GetMaterials();
				ApplyScalarMaterialParam(DynamicMaterials, FName("ClipProgressPos"), ProgressPos);
			}

			if (OwnerActor_BattleEntityComponent->LWeapon.IsValid() && IsValid(OwnerActor_BattleEntityComponent->LWeapon->WeaponMeshRef))
			{
				DynamicMaterials = OwnerActor_BattleEntityComponent->LWeapon->WeaponMeshRef->GetMaterials();
				ApplyScalarMaterialParam(DynamicMaterials, FName("ClipProgressPos"), ProgressPos);
			}
		}
	}

	if (SpawnEffect.IsValid())
	{
		FVector NewEffectPos = BaseHeight;
		NewEffectPos.Z = ProgressPos;

		SpawnEffect->SetWorldLocation(NewEffectPos);
	}
}

void UEffectComponent::SetDisappearEffectMaterialParam(float ProgressRatio)
{
	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent == nullptr)
		return;

	ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
	if (OwnerCharacter)
	{
		TArray< UMaterialInterface* >	DynamicMaterials = OwnerCharacter->GetMesh()->GetMaterials();
		int32 MaterialMax = DynamicMaterials.Num();
		for (int32 Index = 0; Index < MaterialMax; ++Index)
		{
			UMaterialInstanceDynamic* MaterialInstance = Cast< UMaterialInstanceDynamic >(DynamicMaterials[Index]);

			if (MaterialInstance)
			{
				float Amount = FMath::InterpEaseOut(0.0f, 1.0f, ProgressRatio, 4.0f);
				MaterialInstance->SetScalarParameterValue(FName("control"), Amount);
			}
		}

		if (OwnerActor_EntityRenderComponent->SubDecoMesh.IsValid())
		{
			DynamicMaterials = OwnerActor_EntityRenderComponent->SubDecoMesh->GetMaterials();
			MaterialMax = OwnerActor_EntityRenderComponent->SubMeshOriginalMaterialList.Num();
			for (int32 Index = 0; Index < MaterialMax; ++Index)
			{
				UMaterialInstanceDynamic* MaterialInstance = Cast< UMaterialInstanceDynamic >(DynamicMaterials[Index]);

				if (MaterialInstance)
				{
					float Amount = FMath::InterpEaseOut(0.0f, 1.0f, ProgressRatio, 4.0f);
					MaterialInstance->SetScalarParameterValue(FName("control"), Amount);
				}
			}
		}
	}
}

void UEffectComponent::ApplyScalarMaterialParam(TArray< UMaterialInterface* > InMaterialInstances, FName ParamName, float InParam)
{
	int32 MaterialMax = InMaterialInstances.Num();
	for (int32 Index = 0; Index < MaterialMax; ++Index)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast< UMaterialInstanceDynamic >(InMaterialInstances[Index]);

		if (MaterialInstance)
		{
			MaterialInstance->SetScalarParameterValue(ParamName, InParam);
		}
	}
}

void UEffectComponent::AppearEffectTick(float DeltaTime)
{
	if (bActive)
	{
		if (ElapsedTime >= Duration)
		{
			if (SpawnEffect.IsValid())
				SpawnEffect->Deactivate();

			if(CustomChangeMaterialComponent.IsValid())
				CustomChangeMaterialComponent->Active(false, nullptr);

			bool bEnemy = URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerActor.Get());

			UParticleSystem* SpawnEffectClass = nullptr;
			
			if (false == bEnemy)
			{
				SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("MaskingFriendAfterEffect"));
			}
			else
			{
				SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("MaskingEnemyAfterEffect"));
			}
			
			if (SpawnEffectClass)
			{
				SpawnEffect = UUtilFunctionLibrary::SpawnEmitterAttached(SpawnEffectClass, OwnerActor.Get(), FName("Socket_Root"));

				UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
				if (SpawnEffect.IsValid() && IsValid(BattleEntityComponent))
				{
					UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
					if (BattleInfo)
					{
						float UnitRadius = BattleInfo->CharacterProperty.UnitRadius;
						float EffectScale = UnitRadius / RADIUSSIZE_DEFAULT;

						SpawnEffect->SetRelativeScale3D(FVector(EffectScale, EffectScale, EffectScale));
					}
				}
			}

			bActive = false;

			CurEffectType = EEffectTypeEnum::VE_None;
		}
		else
		{
			SetAppearEffectMaterialParam(ElapsedTime / Duration);
			ElapsedTime += DeltaTime;
		}
	}
}

void UEffectComponent::DisappearEffectTick(float DeltaTime)
{
	if (bActive)
	{
		if (ElapsedTime >= Duration)
		{
			bActive = false;

			ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
			AActor* SelectedActor = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetCurrentSelectedCharacterActor();
			if (BasePC)
			{
				IEntityBaseProperty* Prop = Cast<IEntityBaseProperty>(OwnerActor.Get());

				if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
				{
					UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_RTS_CS_PLAYLOSE);
				}
				else if(OwnerActor.Get() == SelectedActor)
					UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_FailEvent);
				else
				{
					if (Prop->GetGroupKey() == GroupKey_SINGLE_ENEMY)
					{
						if (UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get())->IsBoss())
						{
							UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_BossDead);
						}
					}
				}
			}
			

			OwnerActor->K2_DestroyActor();
			OwnerActor = nullptr;

			CurEffectType = EEffectTypeEnum::VE_None;

			
		}
		else
		{
			SetDisappearEffectMaterialParam(ElapsedTime / Duration);
			ElapsedTime += DeltaTime;
		}
	}
}

void UEffectComponent::LightFlickerTick(float DeltaTime)
{
	if (ElapsedTime >= FlickingDuration)
	{
		bActive = false;
	}
	else
	{
		ElapsedTime += DeltaTime;

		float CurIntensity = FMath::Abs((int32)(FMath::Sin(ElapsedTime / FlickingInterval) + 0.5f)) * 2.0f; // over light
		if (CustomAmbientComponent.IsValid())
		{
			CustomAmbientComponent->Active(1.0f, CurIntensity, FColor::Red);
		}
	}
}

void UEffectComponent::SetCustomAmbient(FColor InColor)
{
	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent == nullptr)
		return;

	if (OwnerActor.IsValid())
	{
		for (auto OriginalMaterial : OwnerActor_EntityRenderComponent->OriginalMaterialList)
		{
			if (IsValid(OriginalMaterial))
			{
				OriginalMaterial->SetVectorParameterValue(FName(TEXT("CustomLightColor")), InColor);
			}
			else
			{
				UE_LOG(LogCriticalErrors, Log, TEXT("UEffectComponent Material NULL - %s"), *(OwnerActor->GetName()));
			}
		}
	}
}

FColor UEffectComponent::GetModifiedColor(float InIntensity)
{
	//return FColor(LightColor.R* InIntensity, LightColor.G* InIntensity, LightColor.B* InIntensity);
	return FColor();
}
