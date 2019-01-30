// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "BattleBaseCharacter.h"


#include "GlobalIntegrated.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "Character/Bullets/BulletBase.h"
#include "Character/Bullets/NormalBullet.h"
//#include "Character/Party/Party.h"

#include "Table/CharacterTableInfo.h"

#include "Components/LateUpdateComponent.h"
#include "Components/SkillAreaIndicatorMeshComponent.h"
#include "Components/CustomAmbientComponent.h"
#include "Components/EffectComponent.h"
#include "Components/MovingStateCheckComponent.h"
#include "Components/CustomChangeMaterialComponent.h"
#include "Components/BossComponents/BossBaseComponent.h"
#include "Components/BossComponents/GigantComponent.h"
#include "Components/SelectionIndicatorMeshComponent.h"
#include "Components/WidgetDecoratorComponent.h"
#include "Components/CustomWidgetComponent.h"
#include "Components/WaypointComponent.h"
#include "Components/RemoteMoveComponent.h"
#include "Components/AnimStateComponent.h"
#include "Components/GridMapInspectorComponent.h"
#include "Components/SpecialMovingComponent.h"
#include "Components/CustomCharacterMovementComponent.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"




#include "UtilFunctionIntegrated.h"

#include "UI/CharacterUI/CharacterUserWidget.h"

#include "CustomStructures/DamageResultRate.h"
#include "CustomStructures/GameEnvironmentVar.h"

#include "SharedBattle/BattleOperator.h"
#include "Item/WeaponBase.h"


#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"

/*Move*/
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WidgetComponent.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "BattleBaseAnimInstance.h"
#include "TutorialLevelScriptActor.h"

#include "Components/FunctionalComponents/BattleEntityComponent.h"
#include "Components/FunctionalComponents/SkillEntityComponent.h"
#include "Components/FunctionalComponents/EntityAIComponent.h"
#include "Components/FunctionalComponents/EntityRenderComponent.h"

#include "Components/FunctionalComponents/RemoteBattleEntityComponent.h"
#include "Components/FunctionalComponents/RemoteSkillEntityComponent.h"
#include "Components/FunctionalComponents/RemoteEntityAIComponent.h"
#include "Components/VibrationComponent.h"

#include "Components/AvoidanceComponent.h"

#include "CustomActors/BaseSequenceActor.h"
#include "UtilFunctionLibrary.h"

//#include "Components/CustomCharacterMovementComponent.h"
//#define USE_CUSTOM_CAPSULE_SIZE

ABattleBaseCharacter::ABattleBaseCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UAvoidanceComponent>(ACharacter::CharacterMovementComponentName))	
{
		
}

void ABattleBaseCharacter::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);
}

void ABattleBaseCharacter::AttachComponents()
{
	UClass* MyClass = ABattleBaseCharacter::StaticClass();
	ABattleBaseCharacter* BattleChar_CDO = MyClass->GetDefaultObject<ABattleBaseCharacter>();

	switch (EntityBasePropertyInfo.EntityType)
	{
		case EEntityTypeEnum::VE_None:
		case EEntityTypeEnum::VE_Normal:
		{
			BattleEntityComponent = NewObject< UBattleEntityComponent >(this);
			BattleEntityComponent->OnComponentCreated();
			BattleEntityComponent->RegisterComponent();

			SkillEntityComponent = NewObject< USkillEntityComponent >(this);
			SkillEntityComponent->OnComponentCreated();
			SkillEntityComponent->RegisterComponent();

			EntityAIComponent = NewObject< UEntityAIComponent >(this);
			EntityAIComponent->OnComponentCreated();
			EntityAIComponent->RegisterComponent();
		}
		break;
		case EEntityTypeEnum::VE_RemoteEntity:
		{
			BattleEntityComponent = NewObject< URemoteBattleEntityComponent >(this);
			BattleEntityComponent->OnComponentCreated();
			BattleEntityComponent->RegisterComponent();

			SkillEntityComponent = NewObject< URemoteSkillEntityComponent >(this);
			SkillEntityComponent->OnComponentCreated();
			SkillEntityComponent->RegisterComponent();

			EntityAIComponent = NewObject< URemoteEntityAIComponent >(this);
			EntityAIComponent->OnComponentCreated();
			EntityAIComponent->RegisterComponent();

			RemoteMoveComponent = NewObject< URemoteMoveComponent >(this);
			RemoteMoveComponent->OnComponentCreated();
			RemoteMoveComponent->RegisterComponent();
		}
		break;
	}

#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif
	EntityRenderComponent = NewObject< UEntityRenderComponent >(this);
	EntityRenderComponent->OnComponentCreated();
	EntityRenderComponent->RegisterComponent();
	
	// Add AnimStateComponent
	AnimStateComponent = NewObject< UAnimStateComponent >(this);
	AnimStateComponent->OnComponentCreated();
	AnimStateComponent->RegisterComponent();

#ifndef RULE_TESTWIDGET
	// Add ULateUpdateComponent
	LateUpdateComponent = NewObject< ULateUpdateComponent >(this);
	LateUpdateComponent->OnComponentCreated();
	LateUpdateComponent->RegisterComponent();
#endif

	// Add Custom Ambient component
#ifndef RULE_TESTWIDGET
	CustomAmbientComponent = NewObject< UCustomAmbientComponent >(this);
	CustomAmbientComponent->OnComponentCreated();
	CustomAmbientComponent->RegisterComponent();
#endif

	// Add Effect component
	EffectComponent = NewObject< UEffectComponent >(this);
	EffectComponent->OnComponentCreated();
	EffectComponent->RegisterComponent();
	EffectComponent->HitHightlightStart();

#ifdef CHECK_NEXT_MOVE_VALIDATION
	// GridMapInspectorComponent
	GridMapInspectorComponent = NewObject< UGridMapInspectorComponent >(this);
	GridMapInspectorComponent->OnComponentCreated();
	GridMapInspectorComponent->RegisterComponent();
#endif

	if (EntityBasePropertyInfo.CharacterCategory != ECharacterCategoryTypeEnum::VE_BattleObject)
	{
		// special moving component (knock back)
		SpecialMovingComponent = NewObject< USpecialMovingComponent >(this);
		SpecialMovingComponent->OnComponentCreated();
		SpecialMovingComponent->RegisterComponent();
	}

#ifndef RULE_TESTWIDGET
	VibrationComponent = NewObject<UVibrationComponent>(this);
	VibrationComponent->OnComponentCreated();
	VibrationComponent->RegisterComponent();
#endif

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;
	//UE_LOG(LogGamePlay, Log, TEXT("%f____AttachComponents(%s) "), PrintTime, *(this->GetName()));
#endif
	

	if (ECharacterCategoryTypeEnum::VE_NeutralNpc == EntityBasePropertyInfo.CharacterCategory)
	{
		UCapsuleComponent* CharacterUCapsuleComp = GetCapsuleComponent();
		if (IsValid(CharacterUCapsuleComp))
		{
			if (false == CharacterUCapsuleComp->OnComponentBeginOverlap.IsAlreadyBound(this, &ABattleBaseCharacter::OnBeginOverlap))
			{
				CharacterUCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABattleBaseCharacter::OnBeginOverlap);
			}
		}
	}
}

void ABattleBaseCharacter::InitMembers()
{
}

void ABattleBaseCharacter::InitHeroAfterCreationExe()
{
	//InitMembers();

	bool IsUICharacter = (GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_UICharacter);
	if (IsUICharacter)
	{
		BattleEntityComponent = NewObject< UBattleEntityComponent >(this);
		BattleEntityComponent->OnComponentCreated();
		BattleEntityComponent->RegisterComponent();

		EntityRenderComponent = NewObject< UEntityRenderComponent >(this);
		EntityRenderComponent->OnComponentCreated();
		EntityRenderComponent->RegisterComponent();

		BattleEntityComponent->CreateBattleInfo();
		BattleEntityComponent->SetBattleInfoFromTable();
		EntityRenderComponent->PrepareMeshMaterial();
		EntityRenderComponent->PrepareDecoParts();
#ifdef RULE_TESTDELETE
		bool bExe = true;
		if (this->GetAccountCharacter())
		{
			auto RGameInstance = RGAMEINSTANCE(this);
			const auto HeroItemData = RGameInstance->RInventory->GetInventoryData(this->GetHeroUD());
			if (HeroItemData != nullptr)
			{
				for (int32 i = 0; i < 4; ++i)
				{
					if (HeroItemData->equipedItemUDs.IsValidIndex(i) == false)
						continue;
					if (HeroItemData->equipedItemUDs[i].IsEmpty())
						continue;

					BattleEntityComponent->EquipItem(EItemEquipPositionEnum(i), HeroItemData->equipedItemUDs[i]);
					if (i == 0)
					{
						if (BattleEntityComponent->WeaponObjectList.Num() > 0)
							bExe = false;
					}
				}
			}
		}

		if (bExe)
#endif //RULE_TESTDELETE
		{
			BattleEntityComponent->PrepareWeaponsBase();
		}
		//SetReady(false);
		GetMesh()->ComponentTags.Emplace(TAG_NOSOUND);

#ifdef RULE_TESTDELETE
		GetCharacterMovement()->DisableMovement();
#endif
		return;
	}
	else
	{
		AttachComponents();
	}
		
	if (BattleEntityComponent)
	{
		BattleEntityComponent->CreateBattleInfo();
		BattleEntityComponent->SetBattleInfoFromTable();

		/*Check if there is eqipped spirit slots*/
		UPlayStateInfo* playstateinfo = RGAMEINSTANCE(this)->PlayStateInfo;
		if (playstateinfo)
		{
			UE_LOG(LogClass, Log, TEXT("1"));
		}
		/**/
	}

	if (IsUICharacter == false)
	{
		InitHeroBase();
	}
		
	if (EntityRenderComponent)
	{
		EntityRenderComponent->PrepareMeshMaterial();
		EntityRenderComponent->PrepareDecoParts();
	}
	
	if (BattleEntityComponent)
	{
#ifdef RULE_TESTDELETE
		bool bExe = true;
		if (this->GetAccountCharacter())
		{
			auto RGameInstance = RGAMEINSTANCE(this);
			const auto HeroItemData = RGameInstance->RInventory->GetInventoryData(this->GetHeroUD());
			if (HeroItemData != nullptr)
			{
				for (int32 i = 0; i < 4; ++i)
				{
					if (HeroItemData->equipedItemUDs.IsValidIndex(i) == false)
						continue;
					if (HeroItemData->equipedItemUDs[i].IsEmpty())
						continue;

					BattleEntityComponent->EquipItem(EItemEquipPositionEnum(i), HeroItemData->equipedItemUDs[i]);
					if (i == 0)
					{
						if (BattleEntityComponent->WeaponObjectList.Num() > 0)
							bExe = false;
					}
				}
			}
		}

		if (bExe)
#endif //RULE_TESTDELETE
		{
			BattleEntityComponent->PrepareWeaponsBase();
		}
		BattleEntityComponent->CreateBattleInfoBlackboard();
		BattleEntityComponent->CreateBattleOperator();
	}

	if (SkillEntityComponent && BattleEntityComponent)
	{
		SkillEntityComponent->SetBattleEntityComponent(BattleEntityComponent);
	}

	OnInitHeroAfterCreation();

	// Apply character property	
	if (!IsValid(GetCharacterMovement()))
		return;

	GetCharacterMovement()->bUpdateOnlyIfRendered = false;	
		
	if (EntityAIComponent)
	{
		EntityAIComponent->ForceSearchTarget();
	}

	UAvoidanceComponent* AvoidanceComponent = UCharacterHelpFunctionLibrary::GetComponent<UAvoidanceComponent>(this);
	if (IsValid(AvoidanceComponent))
	{
		AvoidanceComponent->InitAvoidanceMovement(EntityAIComponent);
	}

	SetReady(false);
}



void ABattleBaseCharacter::SetSpawnEvent()
{	
	//////////////////////////////////////////////////////////////////////////
	// Has ESpawnTypeEnum
	if (UCharacterHelpFunctionLibrary::IsPossibleToSpawn(this))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// General Spawn
	SetReady(true);
}

void ABattleBaseCharacter::SetSpawnEventForArena()
{
	SetReady(true);
}



void ABattleBaseCharacter::InitHeroAfterCreation()
{
	InitHeroAfterCreationExe();
}




void ABattleBaseCharacter::EndSkillActionExe()
{

}



void ABattleBaseCharacter::InitHeroBase()
{
	if( EntityAIComponent )
		EntityAIComponent->SetBlackboardData();
	if (EntityBasePropertyInfo.bCreateUI)
		AttachUI();
	
}



void ABattleBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


/*
Overrided functions
*/
void ABattleBaseCharacter::Tick(float DeltaSecond)
{
	//Super::Tick(DeltaSecond);
	
	if (IsStopTick)
		return;

	if (IsReady() == false)
		return;

	if (ECharacterCategoryTypeEnum::VE_UICharacter == GetCharacterCategory())
		return;

	if (BattleEntityComponent == nullptr)
		return;

	//const bool IsDeSpawnedPlayerCharacter = (CommandSlotIndex != INVALID_INDEXNUM && UUtilFunctionLibrary::GetSpawnState(this, CommandSlotIndex) != EHeroSpawnStateEnum::VE_Spawned);
	bool IsDeadCharacter = BattleEntityComponent->IsDead();
	if (IsDeadCharacter)
	{
		if (GetCharacterMovement())
		{
			IsStopTick = true;
			GetCharacterMovement()->AvoidanceGroup.bGroup0 = 0;
	
#ifdef RULE_TESTWIDGET
			BattleEntityComponent->LateUpdate();
#endif
		}
		return;
	}
		
	if (SkillEntityComponent)
	{
		SkillEntityComponent->UpdateSkillComponent(DeltaSecond);		
	}


	if ( IsValid(BattleEntityComponent->GetBattleInfoBlackboard()))
	{
		//UE_LOG(LogBattle, Log, TEXT("ClearBlackboard : %s"), *(this->GetName()));
		BattleEntityComponent->GetBattleInfoBlackboard()->ClearBlackBoardData();
	}
}

void ABattleBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (EntityBasePropertyInfo.bTestInit)
	{		
		InitHeroAfterCreation();
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
		if (pGameMode)
		{
			pGameMode->ObjectHelper.AddObject(this);
		}

		InitStartProperty();
		SetSpawnEvent();

		if( SkillEntityComponent )
			SkillEntityComponent->TryToAttachSkills_V2();
		
	}
}

void ABattleBaseCharacter::Destroyed()
{
	if (BattleEntityComponent)
	{
		BattleEntityComponent->UnregisterComponent();
		BattleEntityComponent = nullptr;
	}

	if (SkillEntityComponent)
	{
		SkillEntityComponent->UnregisterComponent();
		SkillEntityComponent = nullptr;
	}

	if (EntityAIComponent)
	{
		EntityAIComponent->UnregisterComponent();
		EntityAIComponent = nullptr;
	}

	if (EntityRenderComponent)
	{
		EntityRenderComponent->UnregisterComponent();
		EntityRenderComponent = nullptr;
	}

	if (AnimStateComponent)
	{
		AnimStateComponent->UnregisterComponent();
		AnimStateComponent = nullptr;
	}

	if (LateUpdateComponent)
	{
		LateUpdateComponent->UnregisterComponent();
		LateUpdateComponent = nullptr;
	}

	if (CustomAmbientComponent)
	{
		CustomAmbientComponent->UnregisterComponent();
		CustomAmbientComponent = nullptr;
	}

	if (EffectComponent)
	{
		EffectComponent->UnregisterComponent();
		EffectComponent = nullptr;
	}

	if (GridMapInspectorComponent)
	{
		GridMapInspectorComponent->UnregisterComponent();
		GridMapInspectorComponent = nullptr;
	}

	if (SpecialMovingComponent)
	{
		SpecialMovingComponent->UnregisterComponent();
		SpecialMovingComponent = nullptr;
	}

	if (VibrationComponent)
	{
		VibrationComponent->UnregisterComponent();
		VibrationComponent = nullptr;
	}

	Super::Destroyed();
}

void ABattleBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (BattleEntityComponent && IsValid(BattleEntityComponent->GetBattleInfo()))
		BattleEntityComponent->GetBattleInfo()->RemoveAllSummonedMinion();

	if (ECharacterCategoryTypeEnum::VE_NeutralNpc == EntityBasePropertyInfo.CharacterCategory)
	{
		UCapsuleComponent* CharacterUCapsuleComp = GetCapsuleComponent();
		if (IsValid(CharacterUCapsuleComp))
		{
			if (CharacterUCapsuleComp->OnComponentBeginOverlap.IsAlreadyBound(this, &ABattleBaseCharacter::OnBeginOverlap))
			{
				CharacterUCapsuleComp->OnComponentBeginOverlap.RemoveDynamic(this, &ABattleBaseCharacter::OnBeginOverlap);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}

bool ABattleBaseCharacter::IsReadyForFinishDestroy()
{
	bool Result = Super::IsReadyForFinishDestroy();

	return Result;
}




//----------------------------------------------------------------------------------------
// Battle base characet interface
//----------------------------------------------------------------------------------------
void ABattleBaseCharacter::AttachUI()
{
#ifdef RULE_SUBMARINE
	if (EntityRenderComponent)
	{
		EntityRenderComponent->AttachHPComponent(GetCharacterCategory());
		EntityRenderComponent->AttachRenderComponents();
#if WITH_EDITOR
		EntityRenderComponent->AttachDebugWidgetComponent();
#endif
	}
#endif
}





#define ImplClass ABattleBaseCharacter
#include "Character/Interfaces/EntityBasePropertyImplTemplatecpp.h"
#undef ImplClass



void ABattleBaseCharacter::InitStartProperty()
{
	// Apply character property

	if (BattleEntityComponent)
	{

#if WITH_EDITOR
		BattleEntityComponent->GetBattleInfo()->ValidateFinalCharacterProperty();

		TotalProperty = BattleEntityComponent->GetBattleInfo()->FinalCharacterProperty;
		BaseProperty = BattleEntityComponent->GetBattleInfo()->CharacterProperty;
		ItemProperty = BattleEntityComponent->GetBattleInfo()->TotalItemCharacterProperty;
#endif

		BattleEntityComponent->InitBattleProperty();

		UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
		if (BattleInfo)
		{
			GetCharacterMovement()->MaxWalkSpeed = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);

			//if (GetCapsuleComponent())
			{
				if (BattleEntityComponent)
				{
					float UnitRadius = BattleInfo->CharacterProperty.UnitRadius;
					BattleEntityComponent->SetBoundingRadius(UnitRadius);
					GetCharacterMovement()->AvoidanceConsiderationRadius = UnitRadius * 1.5f;

					auto RGameInstance = RGAMEINSTANCE(this);
					//FGameEnvironmentVar OutGameVars;
					//UUtilFunctionLibrary::GetGameEnvironmentVar(this, OutGameVars);
					if (VibrationComponent && UnitRadius >= RGameInstance->GameEnvironmentVar.ExceptionVibrationRadius)
					{
						VibrationComponent->SetActive(false);
						VibrationComponent->SetComponentTickEnabled(false);
						VibrationComponent->ClearEvent();
						//UCharacterHelpFunctionLibrary::GetCharacterCategory(this);
					}
					else if(VibrationComponent)
					{
						UE_LOG(LogGamePlay, Log, TEXT("Need VibrationComponent???"));
					}
				}

				//if (EntityAIComponent)
				//	EntityAIComponent->InitNaviSphere(GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			}
			
			if (EntityAIComponent)
				EntityAIComponent->SetModifiedAnimSpeed(BattleInfo->CharacterProperty.AttackSpeed);
		}
	}

	if (CustomCharacterMovementComponent)
		CustomCharacterMovementComponent->bEnablePhysicsInteraction = false;
}

void ABattleBaseCharacter::CacheInstance()
{
	auto AnimInstance = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(this);
	if (AnimInstance == nullptr)
		return;

	int32 MaxNum = int32(EAnimMontageType::VE_DamageResult);
	for (int32 i = 0; i <= MaxNum; ++i)
	{
		EAnimMontageType CheckNum = EAnimMontageType(i);
		if (CheckNum == EAnimMontageType::VE_Skill)
			continue;

		AnimInstance->GetAnimMontageFromAnimMontageType(CheckNum);
	}

	AnimInstance->SetSkillAniType(ESkillAniTypeEnum::VE_SkillAni01);
	AnimInstance->GetSkillAnimMontage();
	AnimInstance->SetSkillAniType(ESkillAniTypeEnum::VE_SkillAni02);
	AnimInstance->GetSkillAnimMontage();
	AnimInstance->SetSkillAniType(ESkillAniTypeEnum::VE_SkillAni03);
	AnimInstance->GetSkillAnimMontage();
	AnimInstance->SetSkillAniType(ESkillAniTypeEnum::VE_None);
}





void ABattleBaseCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (ECharacterCategoryTypeEnum::VE_NeutralNpc != EntityBasePropertyInfo.CharacterCategory)
		return;

	if (!IsValid(BattleEntityComponent))
		return;
	if (!IsValid(SkillEntityComponent))
		return;
	if (BattleEntityComponent->IsDead())
		return;

	IEntityBaseProperty* OverlapActorBaseProperty = Cast<IEntityBaseProperty>(OtherActor);
	if (nullptr == OverlapActorBaseProperty)
		return;
	if (!OverlapActorBaseProperty->GetAccountCharacter())
		return;

	auto ActiveSkill = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill01);
	if (!IsValid(ActiveSkill))
		return;

	SkillEntityComponent->CastInputSkill(ActiveSkill->GetSkillID(), this, OtherActor);

	if (UUtilFunctionLibrary::isTutorialGameMode())
	{
		UUtilFunctionLibrary::GetTutorialLevelScriptActor()->OnReceiveHeal.Broadcast();
	}
}
