// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "EntityRenderComponent.h"
#include "GlobalIntegrated.h"
#include "CustomStructures/GameEnvironmentVar.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "Character/BattleBaseCharacter.h"
#include "Character/Components/CustomChangeMaterialComponent.h"
#include "Character/Components/EffectComponent.h"
//#include "Character/Components/DamageFloater/SlateDamageFloaterComponent.h"
#include "Character/Components/DamageFloater/SlateDamageFloaterComponentV2.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/SkillCastingEffectComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/BossComponents/GigantComponent.h"
#include "Character/Components/BossComponents/AkhanComponent.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Network/RTSManager.h"

// Sets default values for OwnerCharacter.Get() component's properties
UEntityRenderComponent::UEntityRenderComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set OwnerCharacter.Get() component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;
	CustomMaterialComponent = ObjectInitializer.CreateDefaultSubobject<UCustomChangeMaterialComponent>(this, FName(TEXT("CustomChangeMaterialComponent")));	
}


// Called when the game starts
void UEntityRenderComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();


	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (EntityBaseProperty)
	{
		bool IsUICharacter = (EntityBaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_UICharacter);
		if (IsUICharacter)
			return;
	}

	CustomMaterialComponent = NewObject< UCustomChangeMaterialComponent >(this);
 	if (IsValid(CustomMaterialComponent))
 	{
 		CustomMaterialComponent->OnComponentCreated();
 		CustomMaterialComponent->RegisterComponent();
 	}
	
	TArray< UActorComponent* > HeadUpPoses = OwnerActor->GetComponentsByTag(UArrowComponent::StaticClass(), FName(TEXT("HeadUp")));
	if (HeadUpPoses.Num() != 0)
		HeadUpWidget = Cast< UArrowComponent >(HeadUpPoses[0]);
}

void UEntityRenderComponent::UninitializeComponent()
{
	if (IsValid(SightRangeEffect))
	{
		SightRangeEffect->DestroyComponent();
		SightRangeEffect = nullptr;
	}
	if (SubDecoMesh.IsValid())
	{
		SubDecoMesh->DestroyComponent();
		SubDecoMesh = nullptr;
	}

	if (SelectionIndicatorMeshComponent)
	{
		SelectionIndicatorMeshComponent->UnregisterComponent();
		SelectionIndicatorMeshComponent = nullptr;
	}

	if (SynergyMeshComponent)
	{
		SynergyMeshComponent->UnregisterComponent();
		SynergyMeshComponent = nullptr;
	}

	if (TargetingMeshComponent)
	{
		TargetingMeshComponent->UnregisterComponent();
		TargetingMeshComponent = nullptr;
	}

	if (CustomMaterialComponent)
	{
		CustomMaterialComponent->UnregisterComponent();
		CustomMaterialComponent = nullptr;
	}

	for (auto& Element : SkillAreaArray)
	{
		Element->UnregisterComponent();
	}
	SkillAreaArray.Empty();

	if (DecoMeshComponent)
	{
		//subdecomesh equal???
		//DecoMeshComponent->UnregisterComponent();
		DecoMeshComponent = nullptr;
	}

	if (IsValid(SkillCastingEffectComponent))
	{
		SkillCastingEffectComponent->UnregisterComponent();
		SkillCastingEffectComponent = nullptr;
	}

	if (DebugWidgetComponent)
	{
		DebugWidgetComponent->UnregisterComponent();
		DebugWidgetComponent = nullptr;
	}

	AttachEffectList.Empty();
	AttachEffectMap.Empty();
	DieDestroyEffectList.Empty();

	Super::UninitializeComponent();
}

// Called every frame
void UEntityRenderComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* thisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, thisTickFunction);
}

void UEntityRenderComponent::BeginPlay()
{
	Super::BeginPlay();

	// Attach Spawn Effect
	if (UCharacterHelpFunctionLibrary::IsPossibleToSpawn(OwnerActor.Get()) == false)
	{
		UParticleSystem* SpawnEffectClass = nullptr;
		IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());

		if(EntityBaseProperty)
		{
			ECharacterCategoryTypeEnum CharacterCategoryType = EntityBaseProperty->GetCharacterCategory();

			if (ECharacterCategoryTypeEnum::VE_Hero == CharacterCategoryType)
			{
				ABattleBaseCharacter* BattleCharacter = Cast<ABattleBaseCharacter>(OwnerActor.Get());
				if(BattleCharacter->bSpawnedFromCrewChange)
					SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("Crew_Spawn_Effect"));
				else
					SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("HeroSpawnEffect"));


			}
			else if(ECharacterCategoryTypeEnum::VE_Npc == CharacterCategoryType ||
				ECharacterCategoryTypeEnum::VE_BattleObject == CharacterCategoryType)
			{
				SpawnEffectClass = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("NPCSpawnEffect"));
			}
		}

		if (SpawnEffectClass)
		{
			UParticleSystemComponent* SpawnEffect = UUtilFunctionLibrary::SpawnEmitterAttached(SpawnEffectClass, OwnerActor.Get(), FName("Socket_Root"));
			static float StandardRadius = 96.0f;

			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			if (IsValid(SpawnEffect) && IsValid(BattleEntityComponent))
			{
				UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
				if (BattleInfo)
				{
					float UnitRadius = BattleInfo->CharacterProperty.UnitRadius;
					float EffectScale = UnitRadius / StandardRadius;

					SpawnEffect->SetRelativeScale3D(FVector(EffectScale, EffectScale, EffectScale));
				}
			}
		}
	}
}

void UEntityRenderComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UEntityRenderComponent::RefreshComponent()
{
	if (IsValid(HpBarWidgetComponent) == false)
		return;
		
	HpBarWidgetComponent->RefreshComponent();	
}

void UEntityRenderComponent::SetActiveWidgetComp(bool InVisible)
{
	if (IsValid(HpBarWidgetComponent) == false)
		return;

		HpBarWidgetComponent->SetActiveWidgetDecorator(InVisible);	
}

void UEntityRenderComponent::HpBarWidgetDestroy()
{
	if (HpBarWidgetComponent == nullptr)
		return;

	HpBarWidgetComponent->DestroyComponent();
	HpBarWidgetComponent = nullptr;
}

void UEntityRenderComponent::SetDamageFloaterVisible(bool InVisible)
{
	if (IsValid(SlateDamageFloaterComponent))
	{
		SlateDamageFloaterComponent->SetDamageFloaterVisible(InVisible);
	}
}

void UEntityRenderComponent::AttachHPComponent(ECharacterCategoryTypeEnum CharacterCategory, EBattleObjectTypeEnum BattleObjectType)
{
	if (!OwnerActor.IsValid())
		return;

	bool IsBossActor = URenewal_BattleFunctionLibrary::IsBossActor(OwnerActor.Get());
	if (IsBossActor)
		return;

	FTransform HeadUpTransform = GetHeadUpTransform();
	switch (CharacterCategory)
	{
	case ECharacterCategoryTypeEnum::VE_Minion:
	case ECharacterCategoryTypeEnum::VE_Npc:
	case ECharacterCategoryTypeEnum::VE_Hero:
	case ECharacterCategoryTypeEnum::VE_BattleObject:
	{
		HpBarWidgetComponent = UUtilFunctionLibrary::AttachCharacterWidget(OwnerActor.Get(), FName("WBP_CW_HpBar"), HeadUpTransform, CharacterCategory);
	}
		break;
	case ECharacterCategoryTypeEnum::VE_Object:
	{
		switch (BattleObjectType)
		{
		case EBattleObjectTypeEnum::VE_FloorType:
		case EBattleObjectTypeEnum::VE_TrapType:
			break;
		default:
			HpBarWidgetComponent = UUtilFunctionLibrary::AttachCharacterWidget(OwnerActor.Get(), FName("WBP_CW_HpBar_Small"), HeadUpTransform, CharacterCategory);
			break;
		}
	}break;
	}
}

void UEntityRenderComponent::AttachDebugWidgetComponent(bool bForce)
{
	if (!OwnerActor.IsValid())
		return;
	if (IsValid(DebugWidgetComponent))
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	if(bForce || RGameInstance->GameEnvironmentVar.ShowUpHeadDebugingPanel)
	{
		FTransform HeadUpTransform = GetHeadUpTransform();
		DebugWidgetComponent = UUtilFunctionLibrary::AttachCharacterWidget(OwnerActor.Get(), FName("WBP_Dubugging_Panel"), HeadUpTransform, ECharacterCategoryTypeEnum::VE_None);
	}
}

void UEntityRenderComponent::AttachRenderComponents()
{
	if (!OwnerActor.IsValid())
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (nullptr == EntityBaseProperty)
		return;
	
	float ZPos_SkillArea = RGameInstance->GameEnvironmentVar.ZPos_SkillArea;
	float ZPos_SkillPickArea = RGameInstance->GameEnvironmentVar.ZPos_SkillPickArea;
	float ZPos_Targeting = RGameInstance->GameEnvironmentVar.ZPos_Targeting;
	float ZPos_Select = RGameInstance->GameEnvironmentVar.ZPos_Select;
	

	bool IsControlCharacter = (EntityBaseProperty->GetAccountCharacter() && (ECharacterCategoryTypeEnum::VE_Hero == EntityBaseProperty->GetCharacterCategory()));
	if (IsControlCharacter)
	{
		SelectionIndicatorMeshComponent = NewObject< USelectionIndicatorMeshComponent >(OwnerActor.Get());
		UUtilFunctionLibrary::SettingStaticMeshComponent(OwnerActor.Get(), SelectionIndicatorMeshComponent, 
			STATICM_SELECTION, ZPos_Select);

		SelectionIndicatorMeshComponent->SetOwner(OwnerActor.Get());
		SelectionIndicatorMeshComponent->SetScaleFactor(RGameInstance->GameEnvironmentVar.Character_Selection_Decal_ScaleFactor);
		SelectionIndicatorMeshComponent->bAbsoluteRotation = 0;
	}

	//synergy
	{
		SynergyMeshComponent = NewObject< USelectionIndicatorMeshComponent >(OwnerActor.Get());
		UUtilFunctionLibrary::SettingStaticMeshComponent(OwnerActor.Get(), SynergyMeshComponent,
			STATICM_SYNERGY, 1);

		SynergyMeshComponent->SetOwner(OwnerActor.Get());
		SynergyMeshComponent->SetScaleFactor(RGameInstance->GameEnvironmentVar.Character_SelectionGlow_Decal_ScaleFactor);
	}

	bool IsBossActor = URenewal_BattleFunctionLibrary::IsBossActor(OwnerActor.Get());
	if (false == IsBossActor)
	{
		bool bForceCreate = URTSManager::GetInstancePtr()->IsConnected();
		bool IsEnemy = URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerActor.Get());
		if (bForceCreate || IsEnemy)
		{
			TargetingMeshComponent = NewObject< USelectionIndicatorMeshComponent >(OwnerActor.Get());
			UUtilFunctionLibrary::SettingStaticMeshComponent(OwnerActor.Get(), TargetingMeshComponent,
				STATICM_TARGETING, ZPos_Targeting);

			TargetingMeshComponent->SetOwner(OwnerActor.Get());
			TargetingMeshComponent->SetScaleFactor(RGameInstance->GameEnvironmentVar.Character_EnemyGlow_Decal_ScaleFactor);
			TargetingMeshComponent->ComponentTags.Emplace(TAG_TARGETINGMESH);
		}
	}

	if (!IsControlCharacter)
		return;

	SkillAreaArray.Reserve(2);
	//SkillArea
	USkillAreaIndicatorMeshComponent* SkillAreaIndicatorMeshComponent = NewObject< USkillAreaIndicatorMeshComponent >(OwnerActor.Get());
	if (IsValid(SkillAreaIndicatorMeshComponent))
	{
		UUtilFunctionLibrary::SettingStaticMeshComponent(OwnerActor.Get(), SkillAreaIndicatorMeshComponent,
			STATICM_CIRCLENEW, ZPos_SkillArea);

		SkillAreaIndicatorMeshComponent->SetOwner(OwnerActor.Get());
		SkillAreaIndicatorMeshComponent->SetSkillAreaMeshType(ESkillAreaMeshTypeEnum::VE_SkillArea);
		SkillAreaArray.Emplace(SkillAreaIndicatorMeshComponent);
	}

	//SkillPickArea
	USkillAreaIndicatorMeshComponent* SkillPickAreaMeshComponent = NewObject< USkillAreaIndicatorMeshComponent >(OwnerActor.Get());
	if (IsValid(SkillPickAreaMeshComponent))
	{
		SkillPickAreaMeshComponent->OnComponentCreated();
		SkillPickAreaMeshComponent->RegisterComponent();

		ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
		if (OwnerCharacter)
		{
			FVector Offset(0, 0, -(OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) + ZPos_SkillPickArea);
			SkillPickAreaMeshComponent->SetRelativeLocation(Offset);
		}
		UUtilFunctionLibrary::OptimizeStaticMeshComponent(SkillPickAreaMeshComponent);

		SkillPickAreaMeshComponent->bAbsoluteRotation = 0;

		SkillPickAreaMeshComponent->SetOwner(OwnerActor.Get());
		SkillPickAreaMeshComponent->SetSkillAreaMeshType(ESkillAreaMeshTypeEnum::VE_SkillPickArea);
		SkillPickAreaMeshComponent->SetVisibility(false);
		SkillAreaArray.Emplace(SkillPickAreaMeshComponent);
	}
}


void UEntityRenderComponent::AttachRenderComponentsAfterAttachSkill()
{
	//SkillArea
	//SkillPickArea
}


void UEntityRenderComponent::SetVisible(bool bVisible)
{
	OwnerActor->SetActorHiddenInGame(!bVisible);
	OwnerActor->SetActorEnableCollision(bVisible);
	OwnerActor->SetActorTickEnabled(bVisible);

	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		AWeaponBase* RWeapon = BattleEntityComponent->GetWeaponActor(true);
		if ( IsValid( RWeapon) )
		{
			RWeapon->SetActorHiddenInGame(!bVisible);
#ifndef RULE_TESTDELETE
			RWeapon->SetActorEnableCollision(bVisible);
			RWeapon->SetActorTickEnabled(bVisible);
#endif
		}

		AWeaponBase* LWeapon = BattleEntityComponent->GetWeaponActor(false);
		if (IsValid( LWeapon ))
		{
			LWeapon->SetActorHiddenInGame(!bVisible);
#ifndef RULE_TESTDELETE
			LWeapon->SetActorEnableCollision(bVisible);
			LWeapon->SetActorTickEnabled(bVisible);
#endif
		}
	}
}

void UEntityRenderComponent::DeadClear()
{
	if (SelectionIndicatorMeshComponent)
	{
		SelectionIndicatorMeshComponent->SetVisibility(false);
		SelectionIndicatorMeshComponent->UnregisterComponent();
		SelectionIndicatorMeshComponent = nullptr;
	}

	if (SynergyMeshComponent)
	{
		SynergyMeshComponent->SetVisibility(false);
		SynergyMeshComponent->UnregisterComponent();
		SynergyMeshComponent = nullptr;
	}

	if (TargetingMeshComponent)
	{
		TargetingMeshComponent->SetVisibility(false);
		TargetingMeshComponent->UnregisterComponent();
		TargetingMeshComponent = nullptr;
	}
	if (IsValid(SkillCastingEffectComponent))
	{
		SkillCastingEffectComponent->EndSkillCasting();
		SkillCastingEffectComponent->UnregisterComponent();
		SkillCastingEffectComponent = nullptr;
	}

	if (HpBarWidgetComponent)
	{
		HpBarWidgetComponent->SetActiveWidgetDecorator(false);
		HpBarWidgetComponent->UnregisterComponent();
		HpBarWidgetComponent = nullptr;
	}

	RemoveDieDestroyEffect();

	ClearDecoParticles();
	ClearAllEffectForced();

	for (auto& Element : SkillAreaArray)
	{
		Element->ClearAreaSkillTarget();
	}
}

/*
Effect
*/
UParticleSystemComponent* UEntityRenderComponent::AttachEffect(FName id, UParticleSystem* EffectTemplate, FName AttachPointName )
{
	if (id.IsValid() && !id.IsNone() && IsValid(EffectTemplate))
	{
		if (!AttachEffectMap.Contains(id))
		{
			UParticleSystemComponent* comp = UUtilFunctionLibrary::SpawnEmitterAttached(EffectTemplate, OwnerActor.Get(), AttachPointName);
			if (IsValid(comp))
			{
				AttachEffectList.Emplace(comp);
				AttachEffectMap.Emplace(id, comp);
				return comp;
			}
		}
	}

	return nullptr;
}

void UEntityRenderComponent::RemoveEffect(FName id)
{
	if (id.IsValid() && !id.IsNone())
	{
		if (AttachEffectMap.Contains(id))
		{
			TWeakObjectPtr<UParticleSystemComponent> comp = AttachEffectMap[id];
			if (comp.IsValid())
			{
				comp->DestroyComponent();
				
			}
			
			AttachEffectList.Remove(comp);
			AttachEffectMap.Remove(id);
		}
	}
}

bool UEntityRenderComponent::IsAttachEffect(FName id)
{
	if (id.IsValid() && !id.IsNone())
	{
		if (AttachEffectMap.Contains(id))
		{
			TWeakObjectPtr<UParticleSystemComponent> com = AttachEffectMap[id];
			if (com.IsValid())
			{
				return true;
			}
			else
			{
				RemoveEffect(id);
				return false;
			}
			
		}
	}

	return false;
}

UParticleSystemComponent* UEntityRenderComponent::AttachDieDestroyEffect(UParticleSystem* EffectTemplate, FName AttachPointName, FVector InLocation, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy)
{
	if (IsValid(EffectTemplate))
	{
		UParticleSystemComponent* comp = UUtilFunctionLibrary::SpawnEmitterAttached(EffectTemplate, OwnerActor.Get(), AttachPointName, InLocation, Rotation, LocationType, bAutoDestroy);
		if (IsValid(comp))
		{
			DieDestroyEffectList.Emplace(comp);
			return comp;
		}
	}

	return nullptr;
}

void UEntityRenderComponent::RemoveDieDestroyEffect()
{
	for (TWeakObjectPtr<UParticleSystemComponent>& comp : DieDestroyEffectList)
	{
		if (comp.IsValid())
		{
			comp->DestroyComponent();
		}
	}

	DieDestroyEffectList.Empty();
}

void UEntityRenderComponent::SetDecoEffect(UParticleSystem* TargetEffect)
{
	if (IsValid(TargetEffect))
	{
		UParticleSystemComponent* DecoEffect = URenewal_BattleFunctionLibrary::ParametersUsingParticle(TargetEffect, OwnerActor.Get() );
		if( IsValid( DecoEffect ) )
			DecoParticles.Emplace(DecoEffect);
	}
}

void UEntityRenderComponent::SetDecoEffectSocket(UParticleSystem* TargetEffect, FName SocketName)
{
	if (IsValid(TargetEffect))
	{
		UParticleSystemComponent* DecoEffect = UUtilFunctionLibrary::SpawnEmitterAttached(TargetEffect, OwnerActor.Get(), SocketName);
		if( IsValid(DecoEffect ))
			DecoParticles.Emplace(DecoEffect);
	}
}

void UEntityRenderComponent::SetDecoBodyParts(USkeletalMeshComponent* TargeMesh)
{	
	if (IsValid(TargeMesh))
	{
		USkeletalMeshComponent* MeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(OwnerActor.Get());
		if (MeshComp)
		{
			SubDecoMesh = TargeMesh;
			SubDecoMesh->SetMasterPoseComponent(MeshComp);
			SubDecoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			int32 SubMeshOriginalMaterialNum = SubDecoMesh->GetMaterials().Num();
			SubMeshOriginalMaterialList.Reset(SubMeshOriginalMaterialNum);

			TArray<UMaterialInterface*> Materials = SubDecoMesh->GetMaterials();
			for (int32 i = 0; i < SubMeshOriginalMaterialNum; ++i)
			{
				UMaterialInstanceDynamic* dynamicInstance = SubDecoMesh->CreateDynamicMaterialInstance(i, Materials[i]);
				if (IsValid(dynamicInstance))
				{
					SubMeshOriginalMaterialList.Emplace(dynamicInstance);
				}
			}
		}		
	}
}

void UEntityRenderComponent::SetPartsView()
{
	if (DecoMeshComponent)
	{
		DecoMeshComponent->DestroyComponent();
		DecoMeshComponent = nullptr;
	}
	PrepareDecoParts(true);
}

void UEntityRenderComponent::PrepareDecoParts(bool InViewer /*= false*/)
{
	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (EntityBaseProperty)
	{
		FString HeroUD = EntityBaseProperty->GetHeroUD();
		auto RGameInstance = RGAMEINSTANCE(this);
		if (IsValid(RGameInstance->RInventory))
		{
			FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(EntityBaseProperty->GetCharacterCategory(), EntityBaseProperty->GetCharacterID());
			if (CharacterInfo)
			{
				PrepareDecoParts_Mesh(CharacterInfo->DecoMesh);
				PrepareDecoParts_Particle(CharacterInfo->DecoParticle, CharacterInfo->DecoParticleSocket);
			}
		}
	}	
}

void UEntityRenderComponent::PrepareDecoParts_Mesh(TAssetPtr< USkeletalMesh > DecoMesh)
{
	if (DecoMesh.IsNull())
		return;
	
	if (DecoMeshComponent)
	{
		return;
	}

	DecoMeshComponent = NewObject< USkeletalMeshComponent >(OwnerActor.Get());
	if (IsValid(DecoMeshComponent))
	{
		auto DecoSkeletalMesh = UUtilFunctionLibrary::GetSkeletalMeshFromAssetID(DecoMesh);
		DecoMeshComponent->SetSkeletalMesh(DecoSkeletalMesh);
		DecoMeshComponent->OnComponentCreated();
		DecoMeshComponent->RegisterComponent();
		USkeletalMeshComponent* MeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(OwnerActor.Get());
		DecoMeshComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform);

		DecoMeshComponent->SetVisibility(true);

		SetDecoBodyParts(DecoMeshComponent);
	}
}

void UEntityRenderComponent::PrepareDecoParts_Particle(TAssetPtr< UParticleSystem > InDecoParticle, FName InDecoParticleSocket)
{
	if (InDecoParticle.IsNull() == false)
	{
		UParticleSystem* DecoParticle = UUtilFunctionLibrary::GetParticleSystemFromAssetID(InDecoParticle);
		SetDecoEffectSocket(DecoParticle, InDecoParticleSocket);
	}
}

void UEntityRenderComponent::ClearDecoParticles()
{
	if (DecoParticles.Num() != 0)
	{
		for (auto& Element : DecoParticles)
		{
			if (IsValid(Element))
			{
				Element->Deactivate();				
				Element = nullptr;
			}
		}
		DecoParticles.Empty();
	}
}



void UEntityRenderComponent::OutLine(bool bShow)
{
	if (OwnerActor.IsValid())
	{
		ACharacter* CharacterActor = Cast< ACharacter >(OwnerActor.Get());
		if (CharacterActor)
		{
			USkeletalMeshComponent* SkelMesh = CharacterActor->GetMesh();
			if (SkelMesh)
			{				
				SkelMesh->SetRenderCustomDepth(bShow);
			}
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			if (BattleEntityComponent)
			{
				AWeaponBase* WeaponRight = BattleEntityComponent->GetWeaponActor(true);
				if (IsValid(WeaponRight))
				{
					WeaponRight->WeaponMeshRef->SetRenderCustomDepth(bShow);
				}
				AWeaponBase* WeaponLeft = BattleEntityComponent->GetWeaponActor(false);
				if (IsValid(WeaponLeft))
				{
					WeaponLeft->WeaponMeshRef->SetRenderCustomDepth(bShow);
				}
			}
			if (DecoMeshComponent)
			{
				DecoMeshComponent->SetRenderCustomDepth(bShow);
			}
		}
	}
}

void UEntityRenderComponent::CharacterChangeOpacity(float Opacity)
{
	if (IsValid(CustomMaterialComponent))
	{
		CustomMaterialComponent->ChangeOpacity(Opacity);
	}
}

void UEntityRenderComponent::SetMaterialColor(FColor InColor)
{
	for (auto OriginalMaterial : OriginalMaterialList)
	{
		if (IsValid(OriginalMaterial))
		{
			OriginalMaterial->SetVectorParameterValue(STATUS_COLOR_PARAM, InColor);
		}
	}
}

void UEntityRenderComponent::StartTrail(bool bRight)
{
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		AWeaponBase* Weapon = BattleEntityComponent->GetWeaponActor(bRight);
		{
			if ( IsValid( Weapon ))
			{
				Weapon->StartTrail();
			}
		}
	}
}

void UEntityRenderComponent::EndTrail(bool bRight)
{
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		AWeaponBase* Weapon = BattleEntityComponent->GetWeaponActor(bRight);
		{
			if (IsValid(Weapon))
			{
				Weapon->EndTrail();
			}
		}
	}
}


void UEntityRenderComponent::PrepareMeshMaterial()
{
	USkeletalMeshComponent* MeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(OwnerActor.Get());
	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (IsValid(MeshComp) && EntityBaseProperty != nullptr)
	{
		FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(EntityBaseProperty->GetCharacterCategory(), EntityBaseProperty->GetCharacterID());
		if (CharacterInfo)
		{
			PrepareMeshMaterial_Material(CharacterInfo->MeshMaterial, CharacterInfo->MeshMaterial2);

  			int32 MaterialSlotNum = MeshComp->GetNumMaterials();
			
  			for (int32 j = 0; j < MaterialSlotNum; ++j)
  			{
				UMaterialInstance* MaterialInstance = nullptr;
				if (j == 0 && !CharacterInfo->Material0.IsNull()) {
					MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(CharacterInfo->Material0);	
				}
				else if (j == 1 && !CharacterInfo->Material1.IsNull()) {
					MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(CharacterInfo->Material1);
				}
				else if (j == 2 && !CharacterInfo->Material2.IsNull()) {
					MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(CharacterInfo->Material2);
				}
				else if (j == 3 && !CharacterInfo->Material3.IsNull()) {
					MaterialInstance = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(CharacterInfo->Material3);
				}
				if (MaterialInstance != nullptr) {
					UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(MaterialInstance, MeshComp);
					MeshComp->SetMaterial(j, MID);
				}
  			}
		}

		int32 NumMaterials = MeshComp->GetMaterials().Num();
		OriginalMaterialList.Reset(NumMaterials);
		for (int32 i = 0; i < NumMaterials; ++i)
		{
			UMaterialInstanceDynamic* dynamicInstance = MeshComp->CreateDynamicMaterialInstance(i, MeshComp->GetMaterials()[i]);
			if (IsValid(dynamicInstance))
			{
				OriginalMaterialList.Emplace(dynamicInstance);
			}
		}
	}
}

void UEntityRenderComponent::PrepareMeshMaterial_Material(TAssetPtr< UMaterialInstance > Material1, TAssetPtr< UMaterialInstance > Materia2)
{
	if (Material1.IsNull() == false)
	{
		USkeletalMeshComponent* MeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(OwnerActor.Get());
		if (MeshComp)
		{
			auto MaterialInstance1 = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(Material1);
			MeshComp->SetMaterial(0, MaterialInstance1);
			if (Materia2.IsNull() == false)
			{
				auto MaterialInstance2 = UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(Materia2);
				MeshComp->SetMaterial(1, MaterialInstance2);
			}
		}
	}
}

void UEntityRenderComponent::ShowDeadDisapearEvent()
{
	if (UUtilFunctionLibrary::IsCustomGame(OwnerActor->GetWorld()))
		return;

	UEffectComponent* EffectComponent = UCharacterHelpFunctionLibrary::GetEffectComponent(OwnerActor.Get());
	if (IsValid(EffectComponent))
	{
		float DeathEffectDuration = DEADEFFECT_TIME;
		DeathEffectDuration = RGAMEINSTANCE(this)->GameEnvironmentVar.DeathEffectDuration;
		EffectComponent->DisappearEffectStart(DeathEffectDuration);
	}
}


FTransform UEntityRenderComponent::GetHeadUpTransform()
{
	FTransform HeadUpTransform;
	if (HeadUpWidget)
	{
		HeadUpTransform = HeadUpWidget->GetRelativeTransform();		
	}
	else
	{
		ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
		if (OwnerCharacter && OwnerCharacter->GetCapsuleComponent())
		{
			HeadUpTransform.AddToTranslation(FVector(0, 0, OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 50));
		}
	}
	//del
	TArray<UArrowComponent*>	comps;
	OwnerActor->GetComponents<UArrowComponent>(comps);
	for (UArrowComponent* comp : comps)
	{
		if (IsValid(comp))
		{
			if (comp->GetFName() == TEXT("UIArrow"))
			{
				HeadUpTransform = comp->GetRelativeTransform();
				break;
			}
		}
	}

	return HeadUpTransform;
}

void UEntityRenderComponent::ClearAllEffectForced()
{
	for (auto& elem : AttachEffectMap)
	{
		TWeakObjectPtr<UParticleSystemComponent> comp = (elem.Value);
		if (comp.IsValid())
		{
			comp->ActivateSystem(false);
			comp->DeactivateSystem();
			comp->KillParticlesForced();
			
			comp->DestroyComponent();
		}
	}
	AttachEffectMap.Empty();
}

void UEntityRenderComponent::ShowSkillCastingDecal(ESearchShapeTypeEnum ShapeType, TArray<float>& ShapeScope, int32 CastingRange, float CastingTime, FVector& CastingLocation)
{
	if (OwnerActor.IsValid() == false)
		return;

	if (ESearchShapeTypeEnum::VE_Circle != ShapeType &&
		ESearchShapeTypeEnum::VE_Sector != ShapeType &&
		ESearchShapeTypeEnum::VE_Box != ShapeType)
		return;

	auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (nullptr == OwnerActor_BaseProperty)
		return;

	bool IsHero = false;
	ESkillDecalColorType skilldecalcolortype = ESkillDecalColorType::VE_Npc_Enemy;
	
	if (!(URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerActor.Get())))
		skilldecalcolortype = ESkillDecalColorType::VE_Npc_Friend;

	bool bAirState = false;
	switch (OwnerActor_BaseProperty->GetCharacterCategory())
	{
		case ECharacterCategoryTypeEnum::VE_Npc:
		case ECharacterCategoryTypeEnum::VE_BattleObject:
		{
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
			if (IsValid(BattleEntityComponent))
			{
				UBossBaseComponent* BossComp = BattleEntityComponent->GetBossComponent();
				if (IsValid(BossComp))
				{
					bAirState = (BattleEntityComponent->GetSituationState() == ECharacterSituationState::VE_Air);

					if (Cast<UAkhanComponent>(BossComp))
					{
						skilldecalcolortype = ESkillDecalColorType::VE_Npc_Akhan;
					}
					else if (Cast<UGigantComponent>(BossComp))
					{
						skilldecalcolortype = ESkillDecalColorType::VE_Npc_Gigant;
					}
				}
			}
		}
		break;
		case ECharacterCategoryTypeEnum::VE_Hero:
		{
			IsHero = true;
			if (OwnerActor_BaseProperty->GetAccountCharacter())
				skilldecalcolortype = ESkillDecalColorType::VE_HeroOwner;
			else if (URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerActor.Get()))
				skilldecalcolortype = ESkillDecalColorType::VE_Npc_Enemy;
			else
				skilldecalcolortype = ESkillDecalColorType::VE_Npc_Friend;

		}
		break;
		default:
			return;
		break;
	}
	
	if (!IsValid(SkillCastingEffectComponent))
	{
		SkillCastingEffectComponent = NewObject< USkillCastingEffectComponent >(OwnerActor.Get());
		SkillCastingEffectComponent->OnComponentCreated();
		SkillCastingEffectComponent->RegisterComponent();
		SkillCastingEffectComponent->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		SkillCastingEffectComponent->SetAbsolute(false, false, true);
		SkillCastingEffectComponent->SetCanEverAffectNavigation(false);
		SkillCastingEffectComponent->SetMobility(EComponentMobility::Movable);
		SkillCastingEffectComponent->SetSkillDecalColorType(skilldecalcolortype);
		SkillCastingEffectComponent->SetIsHero(IsHero);

		// SetCapsuleHeightOffset
		ACharacter* OwnerCharacter = Cast< ACharacter >(OwnerActor.Get());
		if (IsValid(OwnerCharacter))
		{
			float ZPos_SkillArea = RGAMEINSTANCE(this)->GameEnvironmentVar.ZPos_SkillArea;
			float DefaultHalfHeight = OwnerCharacter->GetDefaultHalfHeight();
			float OffsetZ = ZPos_SkillArea - DefaultHalfHeight;

			SkillCastingEffectComponent->SetCapsuleHeightOffset(FVector(0, 0, OffsetZ));
		}
	}

	if (IsValid(SkillCastingEffectComponent))
	{
		float SkillApplyRangeX = 0.0f;
		float SkillApplyRangeY = 0.0f;
		float SectorAngle = 0.0f;

		if (ESearchShapeTypeEnum::VE_Circle == ShapeType)
		{
			SkillApplyRangeX = ShapeScope[0]; // Radius
			SkillApplyRangeY = ShapeScope[0]; // Radius
		}
		else if (ESearchShapeTypeEnum::VE_Sector == ShapeType)
		{
			SkillApplyRangeX = ShapeScope[0]; // Radius
			SkillApplyRangeY = ShapeScope[0]; // Radius
			SectorAngle = ShapeScope[1];	  // Angle
		}
		else if (ESearchShapeTypeEnum::VE_Box == ShapeType)
		{
			SkillApplyRangeX = CastingRange;
			SkillApplyRangeY = ShapeScope[1]; // Width
		}
		
		SkillCastingEffectComponent->StartSkillCasting(CastingTime, ShapeType, SectorAngle, SkillApplyRangeX, SkillApplyRangeY, CastingLocation);
	}
}