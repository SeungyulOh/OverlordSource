// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "BattleBaseObject.h"

#include "GlobalIntegrated.h"


#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/LateUpdateComponent.h"

#include "BattleObjectAnimInstance.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/ObjectBattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/ObjectSkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/ObjectEntityAIComponent.h"



#include "UtilFunctionIntegrated.h"


ABattleBaseObject::ABattleBaseObject(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	BattleBaseObjectState(STATE::SEARCH_STATE)
{
	//UE_LOG(LogBattle, Log, TEXT("ABattleBaseObject::ABattleBaseObject"));
	
	StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName(TEXT("StaticMesh")));
	StaticMeshComponent->SetupAttachment(RootComponent);

	UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(this);
	if(IsValid(MovementComp))
		MovementComp->SetActive(false);
}

void ABattleBaseObject::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogBattle, Log, TEXT("ABattleBaseObject::UserConstructionScript"));

	SetCharacterCategory( ECharacterCategoryTypeEnum::VE_Object );
	Super::OnConstruction(Transform);
}

void ABattleBaseObject::BeginPlay()
{
	ACharacter::BeginPlay();	
	
	UE_LOG(LogBattle, Log, TEXT("ABattleBaseObject::BeginPlay %d %d"), GetWorld()->AreActorsInitialized(), GetWorld()->ContainsActor(this));

	USkeletalMeshComponent* MeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
	if (IsValid(MeshComp) && IsValid(MeshComp->AnimClass))
	{
		UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(this);
		if (IsValid(MovementComp))
		{
			//MovementComp->DisableMovement();
			MovementComp->bUpdateOnlyIfRendered = false;
			MovementComp->SetActive(false);
		}
	}

	if (EntityBasePropertyInfo.bTestInit)
	{
		InitHeroAfterCreationExe();

		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
		if (pGameMode)
		{
			pGameMode->ObjectHelper.AddObject(this);
		}

	}
}

void ABattleBaseObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEventManager* EventManager = RGAMEINSTANCE(this)->EventManager;
 	checkf(EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);

	if (true == EventManager->OnDeadActor.IsAlreadyBound(this, &ABattleBaseObject::OnDeadActor))
	{
		EventManager->OnDeadActor.RemoveDynamic(this, &ABattleBaseObject::OnDeadActor);
	}


	Super::EndPlay(EndPlayReason);
}

void ABattleBaseObject::InitHeroAfterCreationExe()
{
	UE_LOG(LogBattle, Log, TEXT("ABattleBaseObject::InitHeroAfterCreationExe"));

	// Add ULateUpdateComponent
	LateUpdateComponent = NewObject< ULateUpdateComponent >(this);
	LateUpdateComponent->OnComponentCreated();
	LateUpdateComponent->RegisterComponent();

	BattleEntityComponent = NewObject< UObjectBattleEntityComponent >(this);
	BattleEntityComponent->OnComponentCreated();
	BattleEntityComponent->RegisterComponent();

	SkillEntityComponent = NewObject< UObjectSkillEntityComponent >(this);
	SkillEntityComponent->OnComponentCreated();
	SkillEntityComponent->RegisterComponent();

	EntityAIComponent = NewObject< UObjectEntityAIComponent >(this);
	EntityAIComponent->OnComponentCreated();
	EntityAIComponent->RegisterComponent();

	EntityRenderComponent = NewObject< UEntityRenderComponent >(this);
	EntityRenderComponent->OnComponentCreated();
	EntityRenderComponent->RegisterComponent();

	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(this);
	if (IsValid(BattleComp))
	{
		BattleComp->CreateBattleInfo();

		UCharacterBattleInfo* BattleInfo = BattleComp->GetBattleInfo();
		if (BattleInfo == nullptr)
			return;

		URenewal_BattleFunctionLibrary::SetBattleObjectInfoFromTable(BattleInfo, BattleObjectInfo, GetCharacterID(), GetLevel());
	}

	InitHeroBase();

	UEntityRenderComponent* RenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(this);
	if (IsValid(RenderComp))
	{
		RenderComp->PrepareMeshMaterial();
		RenderComp->PrepareDecoParts();
	}

	if (IsValid(BattleComp))
	{
		BattleComp->CreateBattleInfoBlackboard();
		BattleComp->CreateBattleOperator();
	}

	if (IsValid(BattleComp) && IsValid(BattleComp->GetBattleInfo()))
	{
		BattleComp->SetBoundingRadius(BattleComp->GetBattleInfo()->CharacterProperty.UnitRadius);
	}
			

	BaseObjectInit();

	UCharacterMovementComponent* MovementComp = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(this);
	if (IsValid(MovementComp))
	{
		MovementComp->MaxWalkSpeed = 0.0f;
		MovementComp->SetMovementMode(MOVE_None);
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
		switch (RGameInstance->CurState)
		{
		case EGameStateEnum::VE_Play:
		{
			switch (BattleObjectType)
			{
			case EBattleObjectTypeEnum::VE_CannonType:
				GetWorldTimerManager().SetTimer(AttackTickHandler, this, &ABattleBaseObject::OnAttackTick, BattleObjectInfo.FireDelay, true, 1.0f);
				break;
			case EBattleObjectTypeEnum::VE_TotemTickType:
				GetWorldTimerManager().SetTimer(AttackTickHandler, this, &ABattleBaseObject::OnTotemTick, BattleObjectInfo.FireDelay, true, 1.0f);
				break;
			}
		}
		break;
		}

		if (false == RGameInstance->EventManager->OnDeadActor.IsAlreadyBound(this, &ABattleBaseObject::OnDeadActor))
		{
			RGameInstance->EventManager->OnDeadActor.AddDynamic(this, &ABattleBaseObject::OnDeadActor);
		}
//	}
	
	OnInitHeroAfterCreation();
	
	if (EntityAIComponent)
		EntityAIComponent->ForceSearchTarget();

}

void ABattleBaseObject::EffectInit()
{
	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(this);

	if (IsValid(StaticMeshComponent))
	{
		if (!BattleObjectInfo.Effect_1.ToString().IsEmpty())
		{
			switch (BattleObjectType)
			{
			case EBattleObjectTypeEnum::VE_CannonType:
			case EBattleObjectTypeEnum::VE_TotemTickType:
			case EBattleObjectTypeEnum::VE_TotemType:
			{
				UEntityRenderComponent* RenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(this);
				if (IsValid(RenderComp))
				{
					if (IsValid(BattleComp))
					{
						UCharacterBattleInfo* BattleInfo = BattleComp->GetBattleInfo();
						if (BattleInfo)
						{
							float AttaceRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
							
							UParticleSystem* Particle = UUtilFunctionLibrary::GetParticleSystemFromAssetID(BattleObjectInfo.Effect_1);
							if (IsValid(Particle))
							{
								UParticleSystemComponent* ParticleSystem = RenderComp->AttachDieDestroyEffect(Particle, SOCKET_ROOT, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);
								if (IsValid(ParticleSystem))
								{
									ParticleSystem->SetVectorParameter(TEXT("Decal_Size"), FVector(AttaceRange, AttaceRange, AttaceRange));
								}
							}
						}
					}
				}
			}
			break;
			default:
			{
				UParticleSystemComponent* ParticleSystem = UUtilFunctionLibrary::SpawnEmitterAttached(UUtilFunctionLibrary::GetParticleSystemFromAssetID(BattleObjectInfo.Effect_1), this);
				if (IsValid(ParticleSystem))
				{

				}
			}
			break;
			}
		}

		if (BattleObjectInfo.Effect_2.ToString().IsEmpty())
		{
			UParticleSystemComponent* ParticleSystem = UUtilFunctionLibrary::SpawnEmitterAttached(UUtilFunctionLibrary::GetParticleSystemFromAssetID(BattleObjectInfo.Effect_2), this);
			if (IsValid(ParticleSystem))
			{
			}
		}
	}
}

void ABattleBaseObject::BaseObjectInit()
{
	UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(this);
	if (IsValid(AIComp))
	{
		AIComp->ForceSearchTarget();
	}

	EffectInit();

	IsFindTarget = false;

	switch (BattleObjectType)
	{
	case EBattleObjectTypeEnum::VE_FloorType:
		break;
	case EBattleObjectTypeEnum::VE_TotemType:
	case EBattleObjectTypeEnum::VE_TotemTickType:
	case EBattleObjectTypeEnum::VE_CannonType:
	case EBattleObjectTypeEnum::VE_GimmickType:
	{
		IsFindTarget = true;

		GetWorldTimerManager().SetTimer(FindTargetTickHandler, this, &ABattleBaseObject::OnFindTargetTick, 0.3f, true, 0.3f);


		USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(this);
	}
	break;
	case EBattleObjectTypeEnum::VE_TrapType:
		break;
	}
}

void ABattleBaseObject::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	if (IsReady())
	{
		if (IsValid(BattleEntityComponent))
		{
			AActor* CurrentAttackTarget = BattleEntityComponent->GetCurrentAttackTarget();
			if (IsValid(CurrentAttackTarget) == false)
			{
				if (IsValid(EntityAIComponent))
					EntityAIComponent->ForceSearchTarget();
			}
		}
	}
}

void ABattleBaseObject::OnFindTargetTick()
{
	if (!IsReady())
		return;

	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(this);
	if (/*IsValid(objectMgr) &&*/ IsValid(BattleComp))
	{
		UCharacterBattleInfo* BattleInfo = BattleComp->GetBattleInfo();
		if (IsValid(BattleInfo))
		{
			float fRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
			TArray< TWeakObjectPtr<AActor> > outArray;
			AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
			if (pGameMode)
			{
				pGameMode->ObjectHelper.FindTarget(outArray, this, GetActorLocation(), EFriendshipTypeEnum::VE_None, ESearchShapeTypeEnum::VE_Circle, FVector(fRange, fRange, fRange), 0.0f);
			}

			TArray< TWeakObjectPtr<AActor> > curArray;
			for ( auto other : outArray)
			{
				if (other.IsValid() && other.Get() != this)
				{
					UBattleEntityComponent* otherBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(other.Get());
					if (IsValid(otherBattleEntityComponent))
					{
						if (!otherBattleEntityComponent->IsDead() && !otherBattleEntityComponent->IsSearchSkip())
						{
							if (OverlapList.Contains(other))
							{
								curArray.Emplace(other);
							}
							else
							{
								switch (BattleObjectType)
								{
								case EBattleObjectTypeEnum::VE_GimmickType:
								{
									if (BattleComp->IsEnemy(other.Get()))
									{
										BattleComp->SetCurrentAttackTarget(other.Get());
									}
								}
								break;
								case EBattleObjectTypeEnum::VE_TotemType:
								{
									BattleComp->SetCurrentAttackTarget(other.Get());
								}
								break;
								}

								curArray.Emplace(other);
							}
						}
					}
				}
			}

			TArray< TWeakObjectPtr<AActor> > detchList;
			for ( auto temp : OverlapList)
			{
				if (!curArray.Contains(temp))
				{
					detchList.Emplace(temp);
				}
			}

			OverlapList = curArray;

			for ( auto detch : detchList)
			{
				if ( detch.IsValid() )
				{
					UBattleEntityComponent* detchBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(detch.Get());
					if (IsValid(detchBattleEntityComponent))
					{
						switch (BattleObjectType)
						{
						case EBattleObjectTypeEnum::VE_TotemType:
						{
							if (!detchBattleEntityComponent->IsDead())
							{
								;
							}
						}
						break;
						}
					}
				}
			}
		}
	}
}

void ABattleBaseObject::OnAttackTick()
{
	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(this);
	if (!IsValid(BattleComp))
		return;

	AActor* CurrentAttackTarget = BattleComp->GetCurrentAttackTarget();
	if (IsValid(CurrentAttackTarget) == false)
	{
		return;
	}

	UBattleEntityComponent* CurrentAttackTarget_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CurrentAttackTarget);
	if (IsValid(CurrentAttackTarget_BattleEntityComponent) == false)
	{
		return;
	}

	if ( CurrentAttackTarget_BattleEntityComponent->IsDead() )
		BattleComp->SetCurrentAttackTarget(nullptr);
	
	if (BattleComp->IsInSituationState(ECharacterSituationState::VE_Ground))
	{
		switch (CurrentAttackTarget_BattleEntityComponent->GetSituationState())
		{
		case ECharacterSituationState::VE_ProgressAir:
		case ECharacterSituationState::VE_Air:
			if (BattleComp->GetBattleInfo()->GetCharacterProperty<EAttackTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackType) == EAttackTypeEnum::VE_Melee)
			{
				BattleComp->SetCurrentAttackTarget(nullptr);
			}
		break;
		case ECharacterSituationState::VE_ProgressAirHigh:
		case ECharacterSituationState::VE_AirHigh:
		{
			BattleComp->SetCurrentAttackTarget(nullptr);
		}
		break;	
		}
	}


	UCharacterBattleInfo* BattleInfo = BattleComp->GetBattleInfo();
	float AttackRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
	if (IsValid(BattleComp->GetCurrentAttackTarget() ))
	{
		float Dist = FVector::Dist(BattleComp->GetCurrentAttackTarget()->GetActorLocation(), this->GetActorLocation());
		if (Dist >= AttackRange )
			BattleComp->SetCurrentAttackTarget(nullptr);
	}

	if (!IsValid(BattleComp->GetCurrentAttackTarget() ))
	{
		BattleComp->SetCurrentAttackTarget( URenewal_BattleFunctionLibrary::FindEnemy(this, this->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, AttackRange, EEnemySearchTargetTypeEnum::VE_Close) );
	}

	if (IsValid(BattleComp->GetCurrentAttackTarget() ))
	{		
		UE_LOG(LogBattle, Log, TEXT("On Attack Tick"));
	}
}

void ABattleBaseObject::OnTotemTick()
{

}

void ABattleBaseObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (!IsReady())
		return;

	if (IsValid(OtherActor) && IsValid(OtherComp))
	{
		UShapeComponent* comp = Cast<UShapeComponent>(OtherComp);
		if (!IsValid(comp))
			return;

		UE_LOG(LogBattle, Log, TEXT("OnBeginOverlap %s"), *OtherActor->GetName());
		UBattleEntityComponent* baseCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OtherActor);
		if (!IsValid(baseCharacter_BattleEntityComponent))
			return;

		UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(this);
		if (!IsValid(BattleComp))
			return;

		if (!baseCharacter_BattleEntityComponent->IsDead() && !baseCharacter_BattleEntityComponent->IsSearchSkip())
		{
			if (!OverlapMap.Contains(OtherActor))
			{
				if (BattleComp->IsEnemy(OtherActor))
				{
					switch (BattleObjectType)
					{
					case EBattleObjectTypeEnum::VE_GimmickType:
					{
						BattleComp->SetCurrentAttackTarget(OtherActor);						
					}
					break;
					}
				}

				switch (BattleObjectType)
				{
				case EBattleObjectTypeEnum::VE_TotemType:
				{
					BattleComp->SetCurrentAttackTarget(OtherActor);
				}
				break;
				}

				OverlapMap.Emplace(OtherActor, OtherActor);
			}
		}
	}	
}

void ABattleBaseObject::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{		
		UBattleEntityComponent* baseCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OtherActor);
		if (!IsValid(baseCharacter_BattleEntityComponent))
			return;

		switch (BattleObjectType)
		{
		case EBattleObjectTypeEnum::VE_TotemType:
		{
			if (!baseCharacter_BattleEntityComponent->IsDead())
			{
				;
			}
		}
		break;
		}
		if (OverlapMap.Contains(OtherActor))
		{
			OverlapMap.Remove(OtherActor);
		}

		UE_LOG(LogBattle, Log, TEXT("OnEndOverlap %s"), *OtherActor->GetName());
	}
}

void ABattleBaseObject::SkillFire()
{
	OnApply();
}

void ABattleBaseObject::Apply()
{
	switch (BattleObjectType)
	{
	case EBattleObjectTypeEnum::VE_TrapType:
		{
			OnApply();
		}
		break;
	case EBattleObjectTypeEnum::VE_CannonType:
		{
			bool bAniType = false;
			USkeletalMeshComponent* SkeletalComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
			if (IsValid(SkeletalComp))
			{
				UBattleObjectAnimInstance* animInstance = Cast<UBattleObjectAnimInstance>(SkeletalComp->AnimScriptInstance);
				if (IsValid(animInstance))
				{
					animInstance->IsFireAni = true;
					bAniType = true;
				}
			}

			if (!bAniType)
			{
				SkillFire();
			}
		}
		break;
	default:
		break;
	}
}

void ABattleBaseObject::EndSkillActionExe()
{
	IsSkillUseing = false;
}

void ABattleBaseObject::AttachUI()
{
	Super::AttachUI();

	if (EntityRenderComponent)
	{
		EntityRenderComponent->AttachHPComponent(GetCharacterCategory(), BattleObjectType);
	}
}

UStaticMeshComponent* ABattleBaseObject::GetStaticMesh()
{
	return StaticMeshComponent;
}

UISkill_V2* ABattleBaseObject::GetActiveSkill()
{
	USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(this);
	if (!IsValid(SkillComp))
		return nullptr;

	SkillComp->GetCurrentSkill_V2();

	return nullptr;
}

void ABattleBaseObject::OnReset_Implementation()
{

}

void ABattleBaseObject::OnPlay_Implementation(AActor* Target)
{

}

void ABattleBaseObject::OnApply_Implementation()
{

}

void ABattleBaseObject::GetSimpleCollisionCylinder(float& CollisionRadius, float& CollisionHalfHeight) const
{
	UCapsuleComponent* CapsuleComp = UCharacterHelpFunctionLibrary::GetCapsuleComponent((AActor*)this);
	if(IsValid(CapsuleComp))
		CapsuleComp->GetScaledCapsuleSize(CollisionRadius, CollisionHalfHeight);
}

USceneComponent* ABattleBaseObject::GetSocketSceneComponent(FName socketName)
{
	USkeletalMeshComponent* SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
	if (IsValid(SkeletalMeshComp))
	{
		UBattleObjectAnimInstance* animInstance = Cast<UBattleObjectAnimInstance>(SkeletalMeshComp->AnimScriptInstance);
		if (IsValid(animInstance))
		{
			return SkeletalMeshComp;
		}
	}

	FVector rtVector = FVector::ZeroVector;
	if (IsValid(HeadMeshComponent))
	{
		rtVector = HeadMeshComponent->GetSocketLocation(socketName);
	}

	if (rtVector.IsNearlyZero())
	{
		if (IsValid(BodyMeshComponent))
		{
			rtVector = BodyMeshComponent->GetSocketLocation(socketName);

			if (!rtVector.IsNearlyZero())
				return BodyMeshComponent;
		}
	}
	else
		return HeadMeshComponent;

	return StaticMeshComponent;
}

FVector ABattleBaseObject::GetSocketLocation(FName socketName)
{
	USkeletalMeshComponent* SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
	if (IsValid(SkeletalMeshComp))
	{
		UBattleObjectAnimInstance* animInstance = Cast<UBattleObjectAnimInstance>(SkeletalMeshComp->AnimScriptInstance);
		if (IsValid(animInstance))
		{
			return SkeletalMeshComp->GetSocketLocation(socketName);
		}
	}

	FVector rtVector = FVector::ZeroVector;
	if (IsValid(HeadMeshComponent))
	{
		rtVector = HeadMeshComponent->GetSocketLocation(socketName);
	}

	if (rtVector.IsNearlyZero())
	{
		if (IsValid(BodyMeshComponent))
		{
			rtVector = BodyMeshComponent->GetSocketLocation(socketName);

			if (!rtVector.IsNearlyZero())
				return rtVector;
		}
	}
	else
		return rtVector;	

	return StaticMeshComponent->GetSocketLocation(socketName);
}

void ABattleBaseObject::SetRotateComponent(USceneComponent* component, FVector addPos)
{
	if (IsValid(component))
	{		
		float fDegree = UUtilFunctionLibrary::GetBetweenDegree(component->GetForwardVector(), GetBattleObjectForward());
		FRotator rotator = component->K2_GetComponentRotation();
		rotator.Yaw += fDegree;
		FVector curPos = GetNavAgentLocation();
		component->SetWorldLocationAndRotation(curPos + addPos, rotator.Quaternion());
	}
}

void ABattleBaseObject::SetFollowComponent(AActor* Target, USceneComponent* component, FVector addPos)
{
	if (IsValid(Target) && IsValid(component))
	{
		auto Character_Target = Cast<ACharacter>(Target);
		if (IsValid(Character_Target))
		{
			FVector followPos = Character_Target->GetNavAgentLocation() + addPos;
			component->SetWorldLocation(followPos);
		}
	}
}

FVector ABattleBaseObject::GetBattleObjectForward()
{
	FRotator rt = FRotator::ZeroRotator;

	if (IsValid(BodyMeshComponent))
	{
		rt.Yaw = 90.0f;
		return rt.RotateVector(BodyMeshComponent->GetForwardVector().GetSafeNormal2D());
	}

	USkeletalMeshComponent* SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
	if (IsValid(SkeletalMeshComp))
	{
		UBattleObjectAnimInstance* animInstance = Cast<UBattleObjectAnimInstance>(SkeletalMeshComp->AnimScriptInstance);
		if (IsValid(animInstance))
		{
			rt.Yaw = GetActorRotation().Yaw;
			return rt.RotateVector(animInstance->AddRotatorValue.RotateVector(FVector::ForwardVector));
		}
	}

	return GetActorForwardVector();
}

FVector ABattleBaseObject::GetBattleObjectRight()
{
	FRotator rt = FRotator::ZeroRotator;

	if (IsValid(BodyMeshComponent))
	{
		rt.Yaw = 90.0f;
		return rt.RotateVector(BodyMeshComponent->GetRightVector().GetSafeNormal2D());
	}

	USkeletalMeshComponent* SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
	if (IsValid(SkeletalMeshComp))
	{
		UBattleObjectAnimInstance* animInstance = Cast<UBattleObjectAnimInstance>(SkeletalMeshComp->AnimScriptInstance);
		if (IsValid(animInstance))
		{
			rt.Yaw = GetActorRotation().Yaw;
			return rt.RotateVector(animInstance->AddRotatorValue.RotateVector(FVector::RightVector));
		}
	}

	return GetActorRightVector();
}

FVector ABattleBaseObject::GetBattleObjectUp()
{
	FRotator rt = FRotator::ZeroRotator;

	if (IsValid(BodyMeshComponent))
	{		
		rt.Yaw = 90.0f;
		return rt.RotateVector(BodyMeshComponent->GetUpVector().GetSafeNormal2D());
	}

	USkeletalMeshComponent* SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
	if (IsValid(SkeletalMeshComp))
	{
		UBattleObjectAnimInstance* animInstance = Cast<UBattleObjectAnimInstance>(SkeletalMeshComp->AnimScriptInstance);
		if (IsValid(animInstance))
		{
			rt.Yaw = GetActorRotation().Yaw;
			return rt.RotateVector(animInstance->AddRotatorValue.RotateVector(FVector::UpVector));
		}
	}

	return GetActorUpVector();
}

FRotator ABattleBaseObject::GetBattleObjectRotator()
{
	if (IsValid(BodyMeshComponent))
	{
		FRotator rt = FRotator::ZeroRotator;
		rt.Yaw = 90;
		return BodyMeshComponent->K2_GetComponentRotation() + rt;
	}

	USkeletalMeshComponent* SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(this);
	if (IsValid(SkeletalMeshComp))
	{
		UBattleObjectAnimInstance* animInstance = Cast<UBattleObjectAnimInstance>(SkeletalMeshComp->AnimScriptInstance);
		if (IsValid(animInstance))
		{
			return animInstance->AddRotatorValue + GetActorRotation();
		}
	}

	return GetActorRotation();
}

void ABattleBaseObject::OnDeadActor(AActor* DeadActor, AActor* FromActor)
{
	if (false == IsValid(DeadActor))
		return;
	if (this != DeadActor)
		return;

	BP_OnDeadActor();
}