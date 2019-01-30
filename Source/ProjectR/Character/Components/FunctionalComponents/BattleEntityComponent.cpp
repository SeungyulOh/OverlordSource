// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/SharedBattle/BattleOperator.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/DamageFloater/SlateDamageFloaterComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/DamageFloater/SlateDamageFloaterComponentV2.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/AnimState/IAnimState.h"
#include "Skill_V2/NetSkillManager.h"
#include "DirectionTrackManager.h"
#include "SkillEntityComponent.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "DamageFloater_PS.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Bullets/NormalBullet.h"
#include "Character/AnimState/AttackAnimState.h"
#include "TutorialLevelScriptActor.h"


#include "Network/RTSManager.h"


#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "LeaderControlHelper.h"
#include "UtilFunctionLibrary.h"

//#define CHECK_KILL_ZONE
#define KILL_ZONE_HEIGHT -3000.0f

#define WITH_DAMAGE_DEBUG 0


// Sets default values for OwnerActor.Get() component's properties
UBattleEntityComponent::UBattleEntityComponent()
{
	// Set OwnerActor.Get() component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
#ifdef CHECK_KILL_ZONE
	PrimaryComponentTick.bCanEverTick = true;
#else
	PrimaryComponentTick.bCanEverTick = false;
#endif

	// ...
	bNeverNeedsRenderUpdate = true;
	bAllowAnyoneToDestroyMe = true;

	ConstructorHelpers::FObjectFinder<UClass> BPFinder(TEXT("Blueprint'/Game/Blueprints/HUD/BP_DamageFloater.BP_DamageFloater_C'"));
	if (BPFinder.Succeeded())
		DamageFloater_BPClass = BPFinder.Object;
}


// Called when the game starts
void UBattleEntityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	OwnerActor = Cast< AActor >( GetOwner() );

	if (OwnerActor.IsValid())
	{
		bRealTimeMode = URealTimeModManager::IsRealTimeGameMode();
		if (bRealTimeMode)
		{
			SetDying(false);
		}

		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if (false == IsValid(RGameInstance))
// 		{
// 			return;
// 		}

		auto OwnerBaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
		if (nullptr == OwnerBaseProperty)
		{
			return;
		}

		bOwnerIsAccountChar = OwnerBaseProperty->GetAccountCharacter();
		if(bOwnerIsAccountChar)
		{
// 			bForceTutorialing = (IsValid(RGameInstance->ForceTutorialManager) && RGameInstance->ForceTutorialManager->IsForceTutorialing());

			bForceTutorialColosseum =
				(bForceTutorialing &&
				(EGamePlayModeTypeEnum::VE_Colosseum == UUtilFunctionLibrary::GetGamePlayModeEnum()));
		}
	}
}

void UBattleEntityComponent::UninitializeComponent()
{
	if (RWeapon.IsValid())
	{
		RWeapon->K2_DestroyActor();
		RWeapon = nullptr;
	}

	if (LWeapon.IsValid())
	{
		LWeapon->K2_DestroyActor();
		LWeapon = nullptr;
	}
	WeaponObjectList.Empty();

	if (ShotPoint && IsCreateShotPoint)
	{
		ShotPoint->UnregisterComponent();
		ShotPoint = nullptr;
	}

	OnReceiveDamageDelegate.Clear();
	OnHitDelegate.Clear();
	OnNotifyDamageDelegate.Clear();
	OnReceiveHealDelegate.Clear();

	Super::UninitializeComponent();
}

// Called every frame
void UBattleEntityComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* thisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, thisTickFunction);
		
#ifdef CHECK_KILL_ZONE
	if (OwnerActor.IsValid())
	{
		if (OwnerActor->GetActorLocation().Z < KILL_ZONE_HEIGHT
			&& IsDead() == false)
		{
			AddHP(-999999, OwnerActor.Get());
			Dead();
		}
	}
#endif
}

void UBattleEntityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBattleEntityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


USkillEntityComponent* UBattleEntityComponent::GetOwnerSkillComponent()
{
	if (OwnerSkillComponent == nullptr)
	{
		IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(OwnerActor.Get());
		if (ComponentGetter)
			OwnerSkillComponent = ComponentGetter->GetSkillEntityComponent();
		else
			OwnerSkillComponent = Cast< USkillEntityComponent >( OwnerActor->GetComponentByClass(USkillEntityComponent::StaticClass()) );
	}

	return OwnerSkillComponent;
}


// Set battleInfo from table	
void	UBattleEntityComponent::SetBattleInfoFromTable()
{
	if (OwnerActor.IsValid())
	{
		IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
		if (EntityBaseProperty)
		{
			URenewal_BattleFunctionLibrary::SetCharacterBattleInfoFromTable(CharacterBattleInfo, EntityBaseProperty->GetCharacterCategory(), EntityBaseProperty->GetCharacterID(), EntityBaseProperty->GetLevel());
			SetAvatarAbilityProperty();
			// call on spawnCharacter
			//EntityBaseProperty->InitStartProperty();
		}		
	}	
}

void UBattleEntityComponent::SetAvatarAbilityProperty()
{
	if (OwnerActor.IsValid())
	{
		TArray<int32> AbilityPoints;
		if (GetAvatarAbilityPoints(AbilityPoints))
		{
			URenewal_BattleFunctionLibrary::SetAvatarAbilityProperty(CharacterBattleInfo, AbilityPoints);
		}
	}
}

bool UBattleEntityComponent::GetAvatarAbilityPoints(TArray<int32>& AbilityPoints)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return false;

	IEntityBaseProperty* BaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if (BaseProperty)
	{
		TArray<int32> AllPoints;
		if (BaseProperty->GetAccountCharacter())
		{
			if (nullptr == RGameInstance->UserInfo)
				return false;

			AllPoints = RGameInstance->UserInfo->Avatar.ability;
		}
		else
		{
			return false;
		}

		if (0 >= AllPoints.Num())
			return false;
		
		URenewal_BattleFunctionLibrary::SetAvatarPointByJobClass(BaseProperty->GetJobClass(), AbilityPoints, AllPoints);
	}
	return true;
}

void UBattleEntityComponent::InitBattleProperty()
{
	if (CharacterBattleInfo)
	{
		CharacterBattleInfo->SetHP(CharacterBattleInfo->GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MaxHP));
	}
}



//-------------------------------------------------------------------------------------------
// Create members
//-------------------------------------------------------------------------------------------

void UBattleEntityComponent::CreateBattleInfo()
{
	if (CharacterBattleInfo == nullptr)
	{
		CharacterBattleInfo = NewObject< UCharacterBattleInfo >();
		CharacterBattleInfo->Init();

		IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
		if (EntityBaseProperty)
		{
			CharacterBattleInfo->SetLevel(EntityBaseProperty->GetLevel());
			CharacterBattleInfo->SetGrade(EntityBaseProperty->GetGrade());
		}		
	}
}


void UBattleEntityComponent::CreateBattleInfoBlackboard()
{
	if (BattleInfoBlackboard == nullptr)
	{
		BattleInfoBlackboard = NewObject< UBattleInfoBlackboard >((UObject*)GetTransientPackage(), UBattleInfoBlackboard::StaticClass());
		BattleInfoBlackboard->SetOwner(OwnerActor.Get());
		BattleInfoBlackboard->CounterInit();
	}
}


void UBattleEntityComponent::CreateBattleOperator()
{
	if (BattleOperator == nullptr)
	{
		BattleOperator = NewObject< UBattleOperator >((UObject*)GetTransientPackage(), UBattleOperator::StaticClass());
		BattleOperator->SetOwner( OwnerActor.Get());
	}
}



//-------------------------------------------------------------------------------------------
// Getter battle objects
//-------------------------------------------------------------------------------------------
UBattleInfoBlackboard*	UBattleEntityComponent::GetBattleInfoBlackboard()
{
	return BattleInfoBlackboard;
}

UBattleOperator* UBattleEntityComponent::GetBattleOperator()
{
	return BattleOperator;
}

UCharacterBattleInfo* UBattleEntityComponent::GetBattleInfo()
{
	return CharacterBattleInfo;
}

void UBattleEntityComponent::ChangeAIPriorityType(EPriorityTypeEnum PriorityType)
{
	if (CharacterBattleInfo)
	{
		CharacterBattleInfo->CharacterProperty.AIPriorityType = PriorityType;
	}
}


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::SetCurrentAttackTarget(AActor* TargetActor)
{
	/*if (TargetActor != nullptr)
	{
		auto OwnerActor_AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		if (IsValid(OwnerActor_AIComponent))
		{
			OwnerActor_AIComponent->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_KeepSafeTarget, TargetActor);
		}
	}*/

	if (CurrentAttackTarget == TargetActor)
		return;

	if (OwnerActor.IsValid())
	{
		bool bPlayTestEditor = false;
		auto RGameInstance = RGAMEINSTANCE(this);
#if WITH_EDITOR
		if (RGameInstance->GetCurrentStateType() == EGameStateEnum::VE_PlayTest)
		{
			bPlayTestEditor = true;
		}
#endif

		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BF) ||
			URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR) ||
			URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG) ||
			URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) ||
			URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR) ||
			URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
			bPlayTestEditor)
		{
			if (UUtilFunctionLibrary::IsLeaderCharacter(OwnerActor.Get()))
			{
				bool bExecuteNotify = true;
				if (CurrentAttackTarget.IsValid() && (TargetActor == nullptr))
				{
					IEntityBaseProperty* CurrentTarget_BaseProperty = Cast< IEntityBaseProperty >(CurrentAttackTarget.Get());
					if (CurrentTarget_BaseProperty)
					{
						if (CurrentTarget_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Npc ||
							CurrentTarget_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_BattleObject)
						{
							bExecuteNotify = false;
						}
					}
				}

				if (bExecuteNotify)
				{
					checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
					RGameInstance->EventManager->OnTargetChange.Broadcast(TargetActor);	
				}
			}
		}
	}

	CurrentAttackTarget = TargetActor;
}

AActor* UBattleEntityComponent::GetCurrentAttackTarget()
{
	if (CurrentAttackTarget.IsValid())
	{
		UBattleEntityComponent* Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CurrentAttackTarget.Get());
		if (IsValid(Target_BattleEntityComponent))
		{
			if (Target_BattleEntityComponent->IsDead())
			{
				SetCurrentAttackTarget(nullptr);
			}
		}
	}
	
	return CurrentAttackTarget.Get();
}

//-------------------------------------------------------------------------------------------
// 1. ::HitTarget				:: Anim notify & check validation <---
// 2. ::HitTargetActor			:: real hit target function 
// 3. ::OnDamageFromCharacter	:: Calculate final damage  
// 4. ::OnDamage				:: Apply additional damage & exceptional process 
// 5. ::OnReceiveDamageEvent	:: Process receive damage event
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::HitTarget(bool bPhysicalAttack, bool bSkillAttack)
{
	if (OwnerActor.IsValid() == false)
		return;
		
	auto BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());

	if ( BaseProperty != nullptr)
	{
		if (BaseProperty->IsReady() == false)
			return;

		if (IsDead())
			return;

		if (bRealTimeMode)
		{
			CSHitTarget();
		}
		else
		{
			if (IsPauseState)
				return;

			if (CurrentAttackTarget.IsValid())
			{
				EBattleDamageResultEnum DamageResult = EBattleDamageResultEnum::VE_None;

				// If target is not in valid attackable state
				if (!(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerActor.Get(), CurrentAttackTarget.Get())))
				{
					SetCurrentAttackTarget(nullptr);
					return;
				}

				BattleInfoBlackboard->SetBlackBoardSkillAttack(bSkillAttack);

				if (false == bSkillAttack)
				{
					bPhysicalAttack = (ECharacterPropertyTypeEnum::VE_PhysicalAttackPower == GetBattleInfo()->CharacterProperty.AttackPropertyType);
				}

				DamageResult = HitTargetActor( CurrentAttackTarget.Get(), bPhysicalAttack, bSkillAttack );
			}
		}
	}
}

//-------------------------------------------------------------------------------------------
// 1. ::HitTarget				:: Anim notify & check validation
// 2. ::HitTargetActor			:: real hit target function <---
// 3. ::OnDamageFromCharacter	:: Calculate final damage  
// 4. ::OnDamage				:: Apply additional damage & exceptional process 
// 5. ::OnReceiveDamageEvent	:: Process receive damage event
//-------------------------------------------------------------------------------------------
EBattleDamageResultEnum	UBattleEntityComponent::HitTargetActor(AActor* TargetActor, bool bPhysicalAttack, bool bSkillAttack)
{
	if (OwnerActor.IsValid() == false)
		return EBattleDamageResultEnum::VE_None;

	auto TargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	auto TargetActor_BaseProperty = Cast<IEntityBaseProperty>(TargetActor);
	auto OwnerActor_SkillEntityComponent = GetOwnerSkillComponent();

	if ( IsValid(TargetActor_BattleEntityComponent) && TargetActor_BaseProperty != nullptr)
	{
		if (IsValid(TargetActor) && TargetActor_BattleEntityComponent->IsDead() == false && TargetActor_BaseProperty->IsReady())
		{
			/*
			Old : Update battle black board..
			*/
			if ( TargetActor_BattleEntityComponent->GetBattleInfoBlackboard() != nullptr )
			{
				TargetActor_BattleEntityComponent->GetBattleInfoBlackboard()->LastAttackType = EAttackTypeEnum::VE_Melee;
				TargetActor_BattleEntityComponent->GetBattleInfoBlackboard()->LastAttackPropertyType = GetBattleInfo()->GetCharacterProperty<ECharacterPropertyTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackPropertyType);
				
			}

			EBattleDamageResultEnum DamageResult = TargetActor_BattleEntityComponent->OnDamageFromCharacter(OwnerActor.Get(), bPhysicalAttack, bSkillAttack);

			OnHitDelegate.Broadcast(OwnerActor.Get(), DamageResult, TargetActor, false);

			return DamageResult;			
		}
	}

	return EBattleDamageResultEnum::VE_None;
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
EBattleDamageResultEnum	UBattleEntityComponent::HitTargetWithBullet(AActor* TargetActor, bool bPhysicalAttack, bool bSkillAttack)
{
	if (bRealTimeMode)
	{
		CSHitTarget(true);
	}
	else
	{
		if (OwnerActor.IsValid() == false)
			return EBattleDamageResultEnum::VE_None;

		auto TargetActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(TargetActor);
		auto TargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
		auto TargetActor_BaseProperty = Cast<IEntityBaseProperty>(TargetActor);

		if (IsValid(TargetActor_BattleEntityComponent) && TargetActor_BaseProperty != nullptr)
		{
			if (IsValid(TargetActor) && TargetActor_BattleEntityComponent->IsDead() == false && TargetActor_BaseProperty->IsReady())
			{
				if (TargetActor_BattleEntityComponent->GetBattleInfoBlackboard() != nullptr)
				{
					TargetActor_BattleEntityComponent->GetBattleInfoBlackboard()->LastAttackType = EAttackTypeEnum::VE_Range;
					TargetActor_BattleEntityComponent->GetBattleInfoBlackboard()->LastAttackPropertyType = GetBattleInfo()->GetCharacterProperty<ECharacterPropertyTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackPropertyType);									
				}


				if (!(URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerActor.Get(), TargetActor)))
					return EBattleDamageResultEnum::VE_None;

				BattleInfoBlackboard->SetBlackBoardSkillAttack(bSkillAttack);
				EBattleDamageResultEnum DamageResult = TargetActor_BattleEntityComponent->OnDamageFromCharacter(OwnerActor.Get(), bPhysicalAttack, bSkillAttack);
				OnHitDelegate.Broadcast(OwnerActor.Get(), DamageResult, TargetActor, true);
				return DamageResult;
			}
		}
	}
	return EBattleDamageResultEnum::VE_None;
}

//-------------------------------------------------------------------------------------------
// On hit, show effect
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::SpawnHitEffect(EBattleDamageResultEnum DamageResult, AActor* TargetActor, bool bBullet)
{
	if (OwnerActor.IsValid() == false)
		return;

	if (IsValid(TargetActor) == false)
		return;
	
	bool bCheckResult = (DamageResult != EBattleDamageResultEnum::VE_Miss && DamageResult != EBattleDamageResultEnum::VE_None && DamageResult != EBattleDamageResultEnum::VE_Dodge);
	if (false == bCheckResult)
		return;

	auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (OwnerActor_BaseProperty == nullptr)
		return;

	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(OwnerActor_BaseProperty->GetCharacterCategory(), OwnerActor_BaseProperty->GetCharacterID());
	if (CharacterInfo == nullptr)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	FVector HitPoint = URenewal_BattleFunctionLibrary::FindHitPoint(OwnerActor.Get(), TargetActor);

	FName SpawnEffectName = CharacterInfo->HitEffect;
	if (OwnerActor_BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero)
	{
		if (DamageResult == EBattleDamageResultEnum::VE_Critical
			/*|| DamageResult == EBattleDamageResultEnum::VE_MagicCritical*/)
		{
			SpawnEffectName = RGameInstance->GameEnvironmentVar.CriticalHitEffect;
		}
	}
	if (SpawnEffectName.IsNone())
	{
		SpawnEffectName = RGameInstance->GameEnvironmentVar.CommonHitEffect;

	}
	UUtilFunctionLibrary::SpawnFX(HitEffect,SpawnEffectName, TargetActor, HitPoint, FRotator::ZeroRotator, TEXT(""));


	/*PointLight Settings*/
	/*AActor* Leader = RGameInstance->PlayStateInfo->GetOwnerCharacter(ROLE_LEADER_INDEX);
	if (IsValid(Leader) && Leader == OwnerActor)
	{
		RGameInstance->PointLightManager->AddPointLight(HitPoint, 0.5f, 500.f, FLinearColor::Red, 50.f);
	}*/
}



//-------------------------------------------------------------------------------------------
// Releated attack
//-------------------------------------------------------------------------------------------
bool UBattleEntityComponent::IsAttacking()
{
	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		return (OwnerActor_AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_NormalAttack) ? true : false;
	}

	return false;
}

void UBattleEntityComponent::EnterAttack()
{
	if (OwnerActor.IsValid() == false)
		return;

// 	if (UCharacterHelpFunctionLibrary::IsRemoteEntity(OwnerActor.Get()))
// 		return;

	/*
	TODO : Should i check skill state by myself?
	*/
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());

	if (IsValid(OwnerActor_SkillEntityComponent))
	{
		if (OwnerActor_SkillEntityComponent->IsSkillCasting())
			return;
		if (OwnerActor_SkillEntityComponent->IsSkillActing())
			return;
	}

	auto CurrentAttackTarget_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CurrentAttackTarget.Get());
	if (false == IsValid(CurrentAttackTarget_BattleEntityComponent))
		return;
	if (CurrentAttackTarget_BattleEntityComponent->IsDead())
		return;

	/*
	TODO : Rotate to target on attack start.
	*/
	bool bRotate = true;
	if (IsValid(BossComponent))
	{
		bRotate = BossComponent->IsMoveEnable();
	}

	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (BaseProperty)
	{
		if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_BattleObject)
			bRotate = false;
	}

	if (bRotate)
	{
		if (!IsDead() && CurrentAttackTarget.IsValid())
		{
			FVector DirectionVector = CurrentAttackTarget->GetActorLocation() - OwnerActor->GetActorLocation();
			DirectionVector.Z = 0;
			FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator();
			OwnerActor->SetActorRotation(lookatRotation);
		}
	}

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartAttack);
	}
}


//-------------------------------------------------------------------------------------------
// End Attack
//-------------------------------------------------------------------------------------------
void	UBattleEntityComponent::EndAttack()
{
	if (OwnerActor.IsValid() == false)
		return;

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StopAttack);
	}
}




//-------------------------------------------------------------------------------------------
// Get current remained hp ratio
//-------------------------------------------------------------------------------------------
float	UBattleEntityComponent::GetCurrentHPPercent()
{
	if (OwnerActor.IsValid() == false)
		return 1.0f;


	auto HP = (float)CharacterBattleInfo->GetHP();
	auto MaxHP = (float)CharacterBattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MaxHP);

	return HP / MaxHP;
}

//-------------------------------------------------------------------------------------------
// Is dead?
//-------------------------------------------------------------------------------------------
bool	UBattleEntityComponent::IsDead()
{
	if (OwnerActor.IsValid() == false)
		return true;

	if (!IsValid(CharacterBattleInfo))
		return true;

	bool result = (CharacterBattleInfo->GetHP() <= 0);
	return result;
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::Dead( bool InIsDestroy /*= false*/)
{
	if (OwnerActor.IsValid() == false)
		return;

	
	// for invoke event, real dead process is done in Dead function.
	OnDeadEvent(DeadFromActor.Get());
	
	ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld());
	if (!BasePC)
		return;

	
	auto BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (BaseProperty)
	{
		if (ECharacterCategoryTypeEnum::VE_NeutralNpc == BaseProperty->GetCharacterCategory())
		{
			auto OwnerActor_RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
			if (IsValid(OwnerActor_RenderComponent))
			{
				OwnerActor_RenderComponent->ShowDeadDisapearEvent();
			}
		}
		else if (OwnerActor.Get() != BasePC->GetLeaderCharacter())
		{
			auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
			if (IsValid(OwnerActor_AnimStateComponent))
			{
				OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_Die);
			}

			
		}

		if (OwnerActor.Get() != BasePC->GetLeaderCharacter())
		{
			auto OwnerActor_AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
			if (IsValid(OwnerActor_AIComponent))
				OwnerActor_AIComponent->AIStop();

			DeadClear();
		}
	}

	if (CharacterBattleInfo)
	{
		CharacterBattleInfo->SetHPZeroAllSummonedMinion();
	}
	if ( OwnerActor.IsValid() && SummonerCharacter.IsValid() )
	{
		auto SummonerCharacter_BattelEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SummonerCharacter.Get());
		if (IsValid(SummonerCharacter_BattelEntityComponent))
		{
			if(false == SummonerCharacter_BattelEntityComponent->IsDead())
			{
				UCharacterBattleInfo* BattleInfo = SummonerCharacter_BattelEntityComponent->GetBattleInfo();
				BattleInfo->RemoveSummonedMinion(OwnerActor.Get());
				SummonerCharacter = nullptr;
			}
		}
	}

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.RemoveObject(OwnerActor.Get(), InIsDestroy);
	}

	UBattleBaseAnimInstance* BattleBaseAnimInstance = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(OwnerActor.Get());
	if (false == IsValid(BattleBaseAnimInstance))
	{
		auto OwnerActor_RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
		if (IsValid(OwnerActor_RenderComponent))
		{
			OwnerActor_RenderComponent->ShowDeadDisapearEvent();
		}
	}
}


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::DeadClear()
{
	if (OwnerActor.IsValid() == false)
		return;

	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_SkillEntityComponent))
		OwnerActor_SkillEntityComponent->ClearSkillEntityComponent();

	ClearAllAbnormalState();

	if (CharacterBattleInfo != nullptr)
		CharacterBattleInfo->RemoveAllSummonedMinion();

	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent)
	{
		OwnerActor_EntityRenderComponent->DeadClear();
	}
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::SetHP(float Value)
{
	if (OwnerActor.IsValid() == false)
		return;
	if (bRealTimeMode && IsDying())
		return;

	CharacterBattleInfo->SetHP(FMath::TruncToInt(Value));

	if (UUtilFunctionLibrary::isTutorialGameMode())
	{
		UUtilFunctionLibrary::GetTutorialLevelScriptActor()->OnHPChanged.Broadcast(OwnerActor.Get(), GetCurrentHPPercent());
	}
}

int32 UBattleEntityComponent::GetHP()
{
	return CharacterBattleInfo->GetHP();
}

int32 UBattleEntityComponent::GetMaxHP()
{
	return CharacterBattleInfo->GetCharacterPropertyMaxValue<int32>(ECharacterPropertyTypeEnum::VE_HP);
}

void UBattleEntityComponent::AddHP(float Value, AActor* FromActor, bool IsDisplayValue)
{
	if (OwnerActor.IsValid() == false)
		return;
	if (bRealTimeMode && IsDying())
		return;

	CharacterBattleInfo->AddHP(FMath::TruncToInt(Value));

	if (UUtilFunctionLibrary::isTutorialGameMode())
	{
		UUtilFunctionLibrary::GetTutorialLevelScriptActor()->OnHPChanged.Broadcast(OwnerActor.Get(), GetCurrentHPPercent());
	}
}

/*
TODO : Move to skillentityComponent
*/
void UBattleEntityComponent::NotifyReceiveHeal(float value, AActor* FromActor)
{
	if (value > 0)
	{
		OnReceiveHealDelegate.Broadcast(value, FromActor);
	}
}

//-------------------------------------------------------------------------------------------
// Is searchSkip?
//-------------------------------------------------------------------------------------------
bool	UBattleEntityComponent::IsSearchSkip()
{
	if (OwnerActor.IsValid() == false)
		return false;

	return false;
}



//-------------------------------------------------------------------------------------------
// Get current remained hp ratio
//-------------------------------------------------------------------------------------------
bool UBattleEntityComponent::IsEnemy(AActor* TargetActor)
{
	return URenewal_BattleFunctionLibrary::IsEnemyActor(OwnerActor.Get(), TargetActor);
}


//-------------------------------------------------------------------------------------------
// Am i enemy character
//-------------------------------------------------------------------------------------------
bool	UBattleEntityComponent::AmIEnemyWithMyPlayer()
{
	if (OwnerActor.IsValid() == false)
		return false;

	return URenewal_BattleFunctionLibrary::IsEnemyAgainstCurrentPlayer(OwnerActor.Get());
}



/*
Related projectile
*/

//-------------------------------------------------------------------------------------------
// shot the projectile
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::ShotProjectile()
{
	if (OwnerActor.IsValid() == false)
		return;

	if ( CurrentAttackTarget.IsValid() == false)
		return;

	UWorld* const World = GetWorld();
	if (World == nullptr)
		return;

	if (IsValid(ShotPoint) == false)
	{
		CreateShotPoint();
	}

	if (CharacterBattleInfo->BulletType == nullptr || false == IsValid(ShotPoint))
	{
		UE_LOG(LogBattle, Log, TEXT("Invalid bullet - %s"), *(OwnerActor->GetName()));
		return;
	}

	/*
	TODO : check bullet type
	*/
	if (false == CharacterBattleInfo->BulletType->IsChildOf(ANormalBullet::StaticClass()))
	{
		UE_LOG(LogCriticalErrors, Log, TEXT("CharacterBattleInfo->BulletType Is Not ANormalBullet Class!!! %s"), CharacterBattleInfo->BulletType);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor.Get();
	SpawnParams.Instigator = OwnerActor->Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.bNoFail = true;

	FVector ShotLocation = GetShotPointLocation();

	// spawn the projectile at the muzzle
	ANormalBullet* const Projectile = World->SpawnActor<ANormalBullet>(CharacterBattleInfo->BulletType, ShotLocation, ShotPoint->GetComponentRotation(), SpawnParams);
	if (Projectile)
	{
		Projectile->InitBullet(OwnerActor.Get(),
			CharacterBattleInfo->CharacterProperty.BulletSpeed,
			ShotLocation, CurrentAttackTarget.Get(), FVector::ZeroVector);
		Projectile->SetNormalAttackBullet_V2();
	}
}


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
FVector UBattleEntityComponent::GetShotPointLocation(FName SocketName)
{
	if (OwnerActor.IsValid() == false)
		return FVector::ZeroVector;

	if (ShotPoint == nullptr)
	{
		CreateShotPoint();
		if (ShotPoint == nullptr)
		{
			UE_LOG(LogBattle, Log, TEXT("Invalid shotpoint - %s"), *GetName());
			return FVector::ZeroVector;
		}
	}

	USkeletalMeshComponent* MeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(OwnerActor.Get());
	if (MeshComp)
	{
		if (SocketName != EXCEPTION_NONE)
		{
			ShotPoint->SetWorldLocation(MeshComp->GetSocketLocation(SocketName));
		}
		else
		{
			switch (CheckShootPoint)
			{
			case EBulltSocketType::VE_ShootR:
			{
				ShotPoint->SetWorldLocation(MeshComp->GetSocketLocation(SOCKET_SHOOTR));
			}
			break;
			case EBulltSocketType::VE_ShootL:
			{
				ShotPoint->SetWorldLocation(MeshComp->GetSocketLocation(SOCKET_SHOOTL));
			}
			break;
			default:
			{
				if (IsValid(ShotPoint))
				{
					ShotPoint->SetWorldLocation(MeshComp->GetSocketLocation(SOCKET_SHOOT));
				}
				else
				{
					return FVector::ZeroVector;
				}
			}
			break;
			}

		}
	}
	return ShotPoint->GetComponentLocation();
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
EBulltSocketType UBattleEntityComponent::GetShotPointType()
{
	if (OwnerActor.IsValid() == false)
		return EBulltSocketType::VE_None;

	return CheckShootPoint;
}


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::SetShootSocketPoint(EBulltSocketType Type)
{
	if (OwnerActor.IsValid() == false)
		return;

	CheckShootPoint = Type;
}

void UBattleEntityComponent::AssignShotPoint(UArrowComponent* InShotPointRef)
{
	ShotPoint = InShotPointRef;
}


void UBattleEntityComponent::CreateShotPoint()//FName InSocketName)
{
	ShotPoint = NewObject< UArrowComponent >(this);
	ShotPoint->OnComponentCreated();
	ShotPoint->RegisterComponent();
	IsCreateShotPoint = true;
	AssignShotPoint(ShotPoint);	
}


//-------------------------------------------------------------------------------------------
// 1. ::HitTarget				:: Anim notify & check validation
// 2. ::HitTargetActor			:: real hit target function 
// 3. ::OnDamageFromCharacter	:: Calculate final damage  <---
// 4. ::OnDamage				:: Apply additional damage & exceptional process 
// 5. ::OnReceiveDamageEvent	:: Process receive damage event
//-------------------------------------------------------------------------------------------
EBattleDamageResultEnum	UBattleEntityComponent::OnDamageFromCharacter(AActor* FromActor, bool bPhysicalAttack, bool bSkillAttack)
{
	if (OwnerActor.IsValid() == false)
		return EBattleDamageResultEnum::VE_None;

	if (IsValid(FromActor))
	{
		/*
		Check target or owner state, than if it is necessary, force to set damage result.
		*/
		EBattleDamageResultEnum DamageResultType = EBattleDamageResultEnum::VE_None;
		int32 FinalDamage = URenewal_BattleFunctionLibrary::GetFinalDamageValue(FromActor, OwnerActor.Get(), DamageResultType, bPhysicalAttack, bSkillAttack);
		
#if WITH_EDITOR && WITH_DAMAGE_DEBUG
		UE_LOG(LogBattle, Log, TEXT("##DamageResult : %s"), *(UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EBattleDamageResultEnum"), DamageResultType ).ToString()) );
#endif

		return OnDamage(FromActor, FinalDamage, DamageResultType, bSkillAttack);
	}

	return EBattleDamageResultEnum::VE_None;
}


//-------------------------------------------------------------------------------------------
// 1. ::HitTarget				:: Anim notify & check validation
// 2. ::HitTargetActor			:: real hit target function 
// 3. ::OnDamageFromCharacter	:: Calculate final damage  
// 4. ::OnDamage				:: Apply additional damage & exceptional process <---
// 5. ::OnReceiveDamageEvent	:: Process receive damage event
//-------------------------------------------------------------------------------------------
EBattleDamageResultEnum	UBattleEntityComponent::OnDamage(AActor* FromActor, float FinalDamage, EBattleDamageResultEnum DamageResultType, bool bSkillAttack)
{
	if (OwnerActor.IsValid() == false)
		return EBattleDamageResultEnum::VE_None;	

	if (IsValid(FromActor) == false)
		return EBattleDamageResultEnum::VE_None;
	auto FromActor_BaseProperty = Cast<IEntityBaseProperty>(FromActor);
	auto FromActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(FromActor);	
	if (FromActor_BaseProperty == nullptr || IsValid(FromActor_BattleEntityComponent) == false )
		return EBattleDamageResultEnum::VE_None;

	if( GetBattleInfoBlackboard() == nullptr || FromActor_BattleEntityComponent->GetBattleInfoBlackboard() == nullptr)
		return EBattleDamageResultEnum::VE_None;

#if WITH_EDITOR && WITH_DAMAGE_DEBUG
	int32  PrevHP = OwnerActor->GetHP();
#endif

	// Apply additional damage from skill
	FinalDamage = BattleOperator->ApplyAdditionalDamage(FinalDamage, FromActor, DamageResultType);
	// check protect or special abnormal state related immune.
	FinalDamage = BattleOperator->CheckProtectState(FinalDamage, FromActor);
	

#if WITH_EDITOR && WITH_DAMAGE_DEBUG
	UE_LOG(LogBattle, Log, TEXT("##Final Damage: %f "), FinalDamage);
	int32  FinalHP = OwnerActor->GetHP();
	UE_LOG(LogBattle, Log, TEXT("##HP : %d -> %d,  Changed HP: %d \n\n\n"), PrevHP, FinalHP, PrevHP - FinalHP);
#endif
	
	// save blackboard data
	GetBattleInfoBlackboard()->FinalDamage = FinalDamage;
	GetBattleInfoBlackboard()->LastDamageResultType = DamageResultType;
	GetBattleInfoBlackboard()->LastAttackCharacter = FromActor;
	
	OnNotifyDamageDelegate.Broadcast(FromActor, OwnerActor.Get(), FinalDamage, bSkillAttack);

	return ApplyFinalDamage(FinalDamage, FromActor, DamageResultType);
}



EBattleDamageResultEnum UBattleEntityComponent::ApplyFinalDamage(float FinalDamage, AActor* FromActor, EBattleDamageResultEnum DamageResultType)
{
	if(IsDead())
		return EBattleDamageResultEnum::VE_Dead;

	if( DamageResultType != EBattleDamageResultEnum::VE_Heal )
		SearchTargetFromHitOnIdle(FromActor);

	OnReceiveDamageEvent(FinalDamage, DamageResultType, FromActor);

	if (bForceTutorialColosseum)
	{
		AdjustForceTutorialColosseumDamage(FinalDamage);
	}

	if (DamageResultType == EBattleDamageResultEnum::VE_Dead)
	{
		DeadFromActor = FromActor;
	}

	int32 PreDamagedHP = GetHP();
	if (FinalDamage < PreDamagedHP)
	{
		AddHP(-FinalDamage, FromActor, true);
	}
	else
	{
		//check HP battle tutorial or force tutorial
		if (RecoverHPForTutorial())
		{
			return DamageResultType;
		}

		auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		OwnerActor_EntityAIComponent->SetBlackboardForceTargetLocation();
		OwnerActor_EntityAIComponent->StopMove();

		/*
		Damage related passive check
		*/
		if (BattleOperator->CheckPassiveDamageOnDead(OwnerActor.Get()) == true)
		{
			SetHP(1);
			return EBattleDamageResultEnum::VE_Dead;
		}
		else
		{
			SetHP(0);
			if (!DeadFromActor.IsValid())
			{
				DeadFromActor = FromActor;
			}

			return EBattleDamageResultEnum::VE_Dead;
		}
	}



	//DeadFromActor = nullptr;

	return DamageResultType;
}

bool UBattleEntityComponent::RecoverHPForTutorial()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
		if ((UUtilFunctionLibrary::IsCustomGame(OwnerActor->GetWorld()) || IsCustomState) ||
			(bForceTutorialing && bOwnerIsAccountChar))
		{
			float hp = CharacterBattleInfo->GetCharacterPropertyMaxValue<float>(ECharacterPropertyTypeEnum::VE_HP);
			SetHP(hp);

			return true;
		}
//	}
	return false;
}

//-------------------------------------------------------------------------------------------
// 1. ::HitTarget				:: Anim notify & check validation
// 2. ::HitTargetActor			:: real hit target function 
// 3. ::OnDamageFromCharacter	:: Calculate final damage  
// 4. ::OnDamage				:: Apply additional damage & exceptional process ffldj 
// 5. ::OnReceiveDamageEvent	:: Process receive damage event		<---
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::OnReceiveDamageEvent(float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor)
{
	AProjectRGameMode* GameMode = UUtilFunctionLibrary::GetGamePlayMode();

	if (GameMode->GameModeState == EGameModeStateEnum::VE_End || GameMode->GameModeState == EGameModeStateEnum::VE_Result)
		return;

	if (UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect() == EDirectionEffectTypeEnum::DE_Stage_BossDead)
		return;

	if (OwnerActor.IsValid() == false)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
	if (!IsValid(BasePlayerController))
	{
		UE_LOG(LogBattle, Log, TEXT("Can't get controller nullptr"));
		return;
	}

	if ( DamageResultType == EBattleDamageResultEnum::VE_Critical || /*DamageResultType == EBattleDamageResultEnum::VE_MagicCritical ||*/
		( DamageResultType == EBattleDamageResultEnum::VE_Hit && RGameInstance->GameEnvironmentVar.AlwaysShakeCameraOnHit ) )
	{			
		if (BasePlayerController->IsSelectedCharacter(OwnerActor.Get()))
		{ 
			BasePlayerController->PlayCameraShake(nullptr, 10.0f);
		}
	}

	
		
	OnReceiveDamageDelegate.Broadcast(OwnerActor.Get(), Damage, DamageResultType, FromActor);

	if (DamageResultType == EBattleDamageResultEnum::VE_Hit ||
		DamageResultType == EBattleDamageResultEnum::VE_Critical ||
// 		DamageResultType == EBattleDamageResultEnum::VE_MagicCritical ||
		DamageResultType == EBattleDamageResultEnum::VE_Dodge //||
//		DamageResultType == EBattleDamageResultEnum::VE_Parry
		)
	{
		auto animInstance = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(OwnerActor.Get());
		if (IsValid(animInstance))
		{
			animInstance->PlayAnimMontageFromDamageResultType(DamageResultType);
		}		
	}

	
	bool bIsBullet = false;
	if (GetBattleInfoBlackboard()->LastAttackType == EAttackTypeEnum::VE_Range )
	{
		bIsBullet = true;
	}

#if LOG_BATTLE
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBattleDamageResultEnum"), true);
	UE_LOG(LogBattle, Warning, TEXT("DamageResult - %s"), *(EnumPtr->GetEnumName((int32)DamageResultType)));
#endif

	auto FromActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(FromActor);
	if (DamageResultType != EBattleDamageResultEnum::VE_Heal && DamageResultType != EBattleDamageResultEnum::VE_NoneHitDamage)
	{
		if (FromActor_BattleEntityComponent)
			FromActor_BattleEntityComponent->SpawnHitEffect(DamageResultType, OwnerActor.Get(), bIsBullet);
	}


	auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (OwnerActor_EntityRenderComponent)
	{
		switch (DamageResultType)
		{
		case EBattleDamageResultEnum::VE_Hit:
		case EBattleDamageResultEnum::VE_Critical:
// 		case EBattleDamageResultEnum::VE_MagicCritical:
		{
			if (URenewal_BattleFunctionLibrary::IsControlCharacter(OwnerActor.Get()))
			{
				if (URenewal_BattleFunctionLibrary::IsEnemyActor(OwnerActor.Get(), FromActor))
				{
					switch (DamageResultType)
					{
					case EBattleDamageResultEnum::VE_Hit:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveHit;
						break;
					case EBattleDamageResultEnum::VE_Critical:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveCritical;
						break;

					case EBattleDamageResultEnum::VE_Miss:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveMiss;
						break;
					case EBattleDamageResultEnum::VE_Immune:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveImmune;
						break;
					case EBattleDamageResultEnum::VE_Counter:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveCounter;
						break;
					case EBattleDamageResultEnum::VE_Dodge:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveDodge;
						break;

					case EBattleDamageResultEnum::VE_Dead:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveDead;
						break;
					case EBattleDamageResultEnum::VE_Pierce:
						DamageResultType = EBattleDamageResultEnum::VE_ReceivePierce;
						break;
					case EBattleDamageResultEnum::VE_ResistantDamage:
						DamageResultType = EBattleDamageResultEnum::VE_ReceiveResistantDamage;
						break;
					}
				}
			}
		}
		break;
		}

		float damageFloaterScale = 1.0f;

		
		if (DamageResultType != EBattleDamageResultEnum::VE_Hit &&
			DamageResultType != EBattleDamageResultEnum::VE_Critical &&
			DamageResultType != EBattleDamageResultEnum::VE_Miss &&
			DamageResultType != EBattleDamageResultEnum::VE_Heal &&
			DamageResultType != EBattleDamageResultEnum::VE_ReceiveHit &&
			DamageResultType != EBattleDamageResultEnum::VE_ReceiveCritical &&
			DamageResultType != EBattleDamageResultEnum::VE_Dodge &&
			DamageResultType != EBattleDamageResultEnum::VE_Dead && 
			DamageResultType != EBattleDamageResultEnum::VE_NoneHitDamage
			)
		{
			UE_LOG(LogClass, Log, TEXT("%i"), (int32)DamageResultType);
			return;
		}
			

		//New DamageFloater
		float HalfHeight = OwnerActor->GetSimpleCollisionHalfHeight();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(OwnerActor.Get()->GetActorLocation() + FVector(0.f, 0.f, HalfHeight));
		//SpawnTransform.SetRotation(CamQuat);
		UWorld* CurrentWorld = OwnerActor.Get()->GetWorld();
		ADamageFloater_PS* DamageFloater = Cast<ADamageFloater_PS>(CurrentWorld->SpawnActorDeferred<ADamageFloater_PS>(DamageFloater_BPClass, SpawnTransform));
		DamageFloater->Initialize(FromActor, OwnerActor.Get(), Damage , DamageResultType);
		DamageFloater->FinishSpawning(SpawnTransform);
		//
	}
}

void UBattleEntityComponent::LateUpdate()
{
	if (IsDying() && IsDeadClearCall)
	{
		IsDeadClearCall = false;
		Dead();
	}
}


void UBattleEntityComponent::ForceDead(AActor* FromActor)
{
	SetHP(-1);
	SetDying(true);

	if (IsValid(FromActor))
	{
		DeadFromActor = FromActor;
	}

	OwnerActor->GetWorldTimerManager().SetTimerForNextTick(this, &UBattleEntityComponent::LateUpdate);
}


void UBattleEntityComponent::OnDeadEvent(AActor* FromActor )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (CommandSlotIndex != INVALID_INDEXNUM)
	{
		if (CommandSlotIndex != ROLE_LEADER_INDEX)
		{
			UDirectionTrackManager* DirectionTrackManager = UUtilFunctionLibrary::GetDirectionTrackManager();

			RGameInstance->PlayStateInfo->SetSpawnState(CommandSlotIndex, EHeroSpawnStateEnum::VE_Dead);
			RGameInstance->PlayStateInfo->GetSelectedCharacterSlot(CommandSlotIndex).DeadLocation = OwnerActor->GetActorLocation();

			bool bForceToCrewDead = true;
			ALevelActor*	LevelActor = BaseStageInfo->GetLevelActor();
			if (LevelActor)
			{
				ALevelActorPhase* PhaseInfo = LevelActor->GetLevelActorPhase();
				if (PhaseInfo)
				{
					if (PhaseInfo->ClearCondition == EPhaseClearEnumType::VE_CharacterAllSurvival)
					{
						bForceToCrewDead = false;
					}
				}
			}
			
			if(bForceToCrewDead)
				RGameInstance->EventManager->OnCrewDead.Broadcast(CommandSlotIndex);
		}
	}
	UGamePlayBluePrintFunction::Die(OwnerActor.Get());
	RGameInstance->EventManager->OnDeadActor.Broadcast(OwnerActor.Get(), FromActor);
	if (IsBoss())
	{
		if (BaseStageInfo && BaseStageInfo->GetBoss().Num() == 0)
		{
			UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_BossDead);

			ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
			if (BasePC)
				BasePC->ActorSettingOnTheCamera(OwnerActor.Get());
		}
	}

	//todo : Change Event
	URenewal_BattleFunctionLibrary::ComputeTargetDecal(OwnerActor.Get(), false);
}


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void UBattleEntityComponent::SearchTargetFromHitOnIdle(AActor* Offender)
{
	if (OwnerActor.IsValid() == false)
		return;

	if (CurrentAttackTarget == nullptr)
	{
		if (URenewal_BattleFunctionLibrary::ExceptionEnemyCondition(OwnerActor.Get(), Offender))
		{
			SetCurrentAttackTarget(Offender);			
		}
	}
}


void UBattleEntityComponent::ValidateFinalCharacterProperty()
{
	CharacterBattleInfo->ValidateFinalCharacterProperty();
}




/*
Character & Abnormal state
*/

bool UBattleEntityComponent::IsInState(ECharacterStateTypeEnum CharacterState)
{
	if (OwnerActor.IsValid() == false)
		return false;

	if (IsDead())
		return false;

	USkillEntityComponent* SkillEntityComponent = GetOwnerSkillComponent();

	switch (CharacterState)
	{
		case ECharacterStateTypeEnum::VE_Idle:
		{
			if (!(OwnerActor->GetVelocity().Size() >= 0.01f) && !(IsAttacking() == true))
			{
				if (SkillEntityComponent)
				{
					if (SkillEntityComponent->GetCurrentSkill_V2() != nullptr)
						return false;
				}
				else
				{
					return true;
				}
			}
		}
		break;
		case ECharacterStateTypeEnum::VE_Moving:
		{
			if (OwnerActor->GetVelocity().Size() >= 0.01f)
				return true;
		}
		break;
		case ECharacterStateTypeEnum::VE_Attacking:
		{
			UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
			if (AnimStateComponent)
			{
				if (AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_NormalAttack)
					return true;
			}			
		}
		break;

		case ECharacterStateTypeEnum::VE_SkillCasting:
		{
			if (SkillEntityComponent)
			{
				if (SkillEntityComponent->GetCurrentSkill_V2() != nullptr && SkillEntityComponent->IsSkillCasting())
				{
					return true;
				}
			}
		}
		break;
		case ECharacterStateTypeEnum::VE_SkillActing:
		{
			if (SkillEntityComponent)
			{
				if (SkillEntityComponent->GetCurrentSkill_V2() != nullptr && SkillEntityComponent->IsSkillActing())
				{
					return true;
				}
			}
		}
		break;
	}

	return false;
}

bool UBattleEntityComponent::SetAbnormalState(EAbonormalStateFlagEnum AbnormalStateFlag, bool bSet)
{
	
	//Abnormal
	bool result = false;
	int32 AbnormalStateValue = static_cast<int32>(AbnormalStateFlag);
	
	if (bSet)
	{
		bool bAlreadySet = false;
		AbnormalStateBit = (AbnormalStateBit | AbnormalStateValue);
		result = !bAlreadySet;
	}
	else
	{
		AbnormalStateBit = (AbnormalStateBit & (~AbnormalStateValue));
		result = true;
	}

	if (result)
	{
		switch (AbnormalStateFlag)
		{
		case EAbonormalStateFlagEnum::BM_Hide:
		{
			auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
			if (OwnerActor_EntityRenderComponent)
			{
				bool HPBarVisible = true;
				if (bSet)
				{
					bool bIsCasterisPlayer = (AmIEnemyWithMyPlayer() == false);
					if (bIsCasterisPlayer)
					{
						OwnerActor_EntityRenderComponent->CharacterChangeOpacity(0.5f);
					}
					else
					{
						OwnerActor_EntityRenderComponent->CharacterChangeOpacity(0.0f);
						HPBarVisible = false;
					}
				}
				else
				{
					OwnerActor_EntityRenderComponent->CharacterChangeOpacity(1.0f);
				}

				OwnerActor_EntityRenderComponent->SetActiveWidgetComp(HPBarVisible);
			}
		}
		break;

		case EAbonormalStateFlagEnum::BM_Silence:
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance)
			{
				RGameInstance->EventManager->OnSynergyEnter.Broadcast(bSet);
			}
		}
		break;
		case EAbonormalStateFlagEnum::BM_Airborne:
		case EAbonormalStateFlagEnum::BM_Stun:
		case EAbonormalStateFlagEnum::BM_Down:		
		case EAbonormalStateFlagEnum::BM_Frozen:
		case EAbonormalStateFlagEnum::BM_Fear:
		case EAbonormalStateFlagEnum::BM_KnockBack:
		case EAbonormalStateFlagEnum::BM_Push:
		case EAbonormalStateFlagEnum::BM_Hold:
		{
			USkillEntityComponent* SkillEntityComponent = GetOwnerSkillComponent();
			if (bSet)
			{
#if LOG_CHARACTER
				UE_LOG(LogBattle, Log, TEXT("AActor::SetAbnormalState - true"));
#endif
				if (SkillEntityComponent)
				{
					SkillEntityComponent->ForceProcessCurrentSkill();
				}

				EndAttack();

				auto OwnerActor_EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
				if (IsValid(OwnerActor_EntityRenderComponent))
				{
					OwnerActor_EntityRenderComponent->EndTrail(true);
					OwnerActor_EntityRenderComponent->EndTrail(false);
				}

				auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
				if (IsValid(OwnerActor_AnimStateComponent))
				{
					FVariantData AbnormalData = FVariantData((int32)AbnormalStateFlag);
					OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_Abnormal, AbnormalData);
				}
			}
			
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance)
			{
				RGameInstance->EventManager->OnSynergyEnter.Broadcast(bSet);
			}
		}
		break;
		default:
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance)
			{
				RGameInstance->EventManager->OnSynergyEnter.Broadcast(bSet);
			}
		}
		break;
		}
	}

	return result;
}





int32 UBattleEntityComponent::AttachAbnormalStateApplier(ESkillApplierTypeEnum InAbnormalApplyType, bool bSet)
{
	int32 AttachCount = 1;
	if (!bSet)
		AttachCount = -1;

	int32 resultCount = 0;
	if (AttachedAbnormalStateApplierCount.Contains(InAbnormalApplyType) == false )
	{
		if (bSet)
		{
			AttachedAbnormalStateApplierCount.Emplace(InAbnormalApplyType, 1);
			resultCount = 1;
		}
	}
	else
	{
		AttachedAbnormalStateApplierCount[InAbnormalApplyType] += AttachCount;
		if (AttachedAbnormalStateApplierCount[InAbnormalApplyType] < 0)
			AttachedAbnormalStateApplierCount[InAbnormalApplyType] = 0;
		resultCount = AttachedAbnormalStateApplierCount[InAbnormalApplyType];
	}

	return resultCount;
}

bool UBattleEntityComponent::IsInAbnormalStateApplier(ESkillApplierTypeEnum InAbnormalApplyType)
{
	if (AttachedAbnormalStateApplierCount.Contains(InAbnormalApplyType) == false)
		return false;

	if (AttachedAbnormalStateApplierCount[InAbnormalApplyType] > 0)
		return true;

	return false;
}

bool UBattleEntityComponent::IsInAbnormalState(EAbonormalStateFlagEnum InAbnormalStateFlag)
{
	int32 CheckFlag = (int32)InAbnormalStateFlag;
	return ( ( AbnormalStateBit & CheckFlag) == CheckFlag );
}

bool UBattleEntityComponent::IsInUnActableState()
{
	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Stun))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Down))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Frozen))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Fear))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Airborne))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_KnockBack))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Push))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Captivate))
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Pull))
		return true;

	return false;
}

bool UBattleEntityComponent::IsInUnMovableState()
{
	if (IsInUnActableState())
		return true;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Hold))
		return true;
	
	return false;
}

bool UBattleEntityComponent::IsAbleToUseSkill()
{
	if (IsInUnActableState())
		return false;

	if (IsInAbnormalState(EAbonormalStateFlagEnum::BM_Silence ))
		return false;
	
	return true;
}



bool UBattleEntityComponent::IsInBlockBuffType( EAbnormalSlotTypeEnum InSlotType )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return false;

	// if buff
	if (RGameInstance->AbnormalStateGroupTable_V2->IsInBuff(InSlotType))
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_abnormal_buff))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_abnormal_buff);
			return true;
		}
	}
	else
	{
		// debuff
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_abnormal_debuff))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_abnormal_debuff);
			return true;
		}
	}

	return false;
}

bool UBattleEntityComponent::IsInBlockGroupType( EAbnormalSlotTypeEnum InSlotType )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return false;
		
	if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_Helplessness, InSlotType))
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_group_helplessness))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_group_helplessness);
			return true;
		}
	}

	if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Bleeding, InSlotType))
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_group_dot_bleeding))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_group_dot_bleeding);
			return true;
		}
	}

	if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Poison, InSlotType))
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_group_dot_poison))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_group_dot_poison);
			return true;
		}
	}

	if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Fire, InSlotType))
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_group_dot_fire))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_group_dot_fire);
			return true;
		}
	}

	return false;
}

bool UBattleEntityComponent::IsInBlockSlotType( EAbnormalSlotTypeEnum InSlotType )
{
	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Stun)
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_stun))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_stun);
			return true;
		}
	}

	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Down)
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_down))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_down);
			return true;
		}
	}

	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Fear )
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_fear))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_fear);
			return true;
		}
	}

	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Ice )
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_ice))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_ice);
			return true;
		}
	}

	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Freezing)
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_freezing))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_freezing);
			return true;
		}
	}

	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Hold)
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_hold))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_hold);
			return true;
		}
	}

	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Silence)
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_silence))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_silence);
			return true;
		}
	}

	if (InSlotType == EAbnormalSlotTypeEnum::VE_Slot_Airborne)
	{
		if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_slot_airborne))
		{
			OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_slot_airborne);
			return true;
		}
	}

	return false;
}


bool UBattleEntityComponent::IsInExceptBlockGroupType(EAbnormalSlotTypeEnum InSlotType)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return false;

	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_group_poison))
	{
		if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Poison, InSlotType) == false )
			return true;
	}

	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_group_fire_ignite))
	{
		if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Fire, InSlotType) == false
			&& RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Ignite, InSlotType) == false)
			return true;
	}

	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_group_bleeding))
	{
		if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_DOT_Bleeding, InSlotType) == false )
			return true;
	}
	
	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_group_mark_weapon))
	{
		if (RGameInstance->AbnormalStateGroupTable_V2->IsInGroup(EAbnormalGroupTypeEnum::VE_Group_Mark_Weapon, InSlotType) == false)
			return true;
	}

	return false;
}

bool UBattleEntityComponent::IsInExceptBlockSlotType(EAbnormalSlotTypeEnum InSlotType)
{
	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_slot_stun))
	{
		if (InSlotType != EAbnormalSlotTypeEnum::VE_Slot_Stun)
			return true;
	}

	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_slot_down))
	{
		if (InSlotType != EAbnormalSlotTypeEnum::VE_Slot_Down)
			return true;
	}

	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_slot_fear))
	{
		if (InSlotType != EAbnormalSlotTypeEnum::VE_Slot_Fear)
			return true;
	}

	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_slot_hold))
	{
		if (InSlotType != EAbnormalSlotTypeEnum::VE_Slot_Hold)
			return true;
	}
	

	if (IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_debuff_except_slot_ice_Freezing))
	{
		if (InSlotType != EAbnormalSlotTypeEnum::VE_Slot_Ice && InSlotType != EAbnormalSlotTypeEnum::VE_Slot_Freezing )
			return true;
	}	
		
	return false;
}



void UBattleEntityComponent::ClearAllAbnormalState()
{
	AbnormalStateBit = 0;
	AttachedAbnormalStateApplierCount.Empty();
}



ECharacterSituationState UBattleEntityComponent::GetSituationState()
{
	return SituationState;
}

void UBattleEntityComponent::SetSituationState(ECharacterSituationState InSituationState)
{
	SituationState = InSituationState;
}

void UBattleEntityComponent::ChangeSituation(ECharacterSituationState ChangeState)
{
	SituationState = ChangeState;

	UCharacterMovementComponent* MovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(OwnerActor.Get());
	auto OwnerActor_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());

	if (MovementComponent && OwnerActor_EntityAIComponent)
	{
		switch (SituationState)
		{
		case ECharacterSituationState::VE_Ground:
		{
			MovementComponent->bConstrainToPlane = 0;
			MovementComponent->SetMovementMode(MOVE_Walking);
			OwnerActor_EntityAIComponent->AIRestart();
		}
		break;
		case ECharacterSituationState::VE_ProgressGround:
		{
			MovementComponent->bConstrainToPlane = 0;
			OwnerActor_EntityAIComponent->AIStop();
		}
		break;
		case ECharacterSituationState::VE_Air:
		{
			MovementComponent->bConstrainToPlane = 1;
			MovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
			MovementComponent->SetMovementMode(MOVE_Flying);
			OwnerActor_EntityAIComponent->AIRestart();
		}
		break;
		case ECharacterSituationState::VE_ProgressAir:
		{
			MovementComponent->bConstrainToPlane = 0;
			MovementComponent->SetMovementMode(MOVE_Flying);
			OwnerActor_EntityAIComponent->AIStop();
		}
		break;
		case ECharacterSituationState::VE_AirHigh:
		{
			MovementComponent->bConstrainToPlane = 1;
			MovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
			OwnerActor_EntityAIComponent->AIStop();
		}
		break;
		case ECharacterSituationState::VE_ProgressAirHigh:
		{
			MovementComponent->bConstrainToPlane = 0;
			MovementComponent->SetMovementMode(MOVE_Flying);
			OwnerActor_EntityAIComponent->AIStop();
		}
		break;
		}
	}
	IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
	if(EntityBaseProperty)
		EntityBaseProperty->OnChangeSituation();
}

bool UBattleEntityComponent::IsInSituationState(ECharacterSituationState CheckState)
{
	if (SituationState == CheckState)
	{
		return true;
	}

	return false;
}


float UBattleEntityComponent::GetMoveAniSpeed()
{	
	return GetBattleInfo()->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MoveSpeed) / GetBattleInfo()->GetCharacterProperty<float>(ECharacterPropertyTypeEnum::VE_MoveSpeed);
}

UBossBaseComponent* UBattleEntityComponent::GetBossComponent()
{
	return BossComponent;
}

void UBattleEntityComponent::SetBossComponent(UBossBaseComponent* InBossComponent)
{
	BossComponent = InBossComponent;
}

int32 UBattleEntityComponent::GetCommandSlotIndex()
{
	return CommandSlotIndex;
}

void UBattleEntityComponent::SetCommandSlotIndex(int32 InCommandSlotIndex)
{
	CommandSlotIndex = InCommandSlotIndex;
}


/*
Weapon
*/
AWeaponBase* UBattleEntityComponent::GetWeaponActor(bool bRight)
{
	if (bRight)
		return RWeapon.Get();
	
	return LWeapon.Get();
}

AWeaponBase* UBattleEntityComponent::AttachWeaponBase(TSubclassOf<AWeaponBase> weaponClass, FName TargetSocket)
{
	if (IsValid(GetWorld()))
	{
		AActor* actor = GetWorld()->SpawnActor(weaponClass);
		if (IsValid(actor))
		{
			AWeaponBase* weaponActor = Cast<AWeaponBase>(actor);
			if (IsValid(weaponActor))
			{
				USkeletalMeshComponent* MeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(OwnerActor.Get());
				if (IsValid(MeshComp))
				{
					FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
					rules.bWeldSimulatedBodies = true;
					weaponActor->AttachToComponent(MeshComp, rules, TargetSocket);
				}

				return weaponActor;
			}
		}
	}

	return nullptr;
}

void UBattleEntityComponent::PrepareWeaponsBase()
{
	DestroyWeaponsBase();

	if (CharacterBattleInfo->RWeapon)
		AttachWeaponInit(CharacterBattleInfo->RWeapon, false);
	if (CharacterBattleInfo->LWeapon)
		AttachWeaponInit(CharacterBattleInfo->LWeapon, true);
}

void UBattleEntityComponent::DestroyWeaponsBase()
{
	if (IsValid(GetWorld()))
	{
		for (AWeaponBase* weapon : WeaponObjectList)
		{
			OwnerActor->GetWorld()->DestroyActor(weapon);
		}

		WeaponObjectList.Reset();
	}

}

FVector UBattleEntityComponent::GetForwardVector()
{
	if (IsValid(BossComponent))
		return BossComponent->GetForwardVector();

	return OwnerActor->GetActorForwardVector();
}

void UBattleEntityComponent::EquipItem(EItemEquipPositionEnum Position, const FString& WeaponItemKey)
{
	if (WeaponItemKey.IsEmpty())
	{
		UnEquipItem(Position);
	}
	else
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if (!IsValid(RGameInstance))
// 			return;

		FITEM* WeaponItemData = RGameInstance->RInventory->GetItemDataWithUD(WeaponItemKey);
		if (WeaponItemData == nullptr)
			return;

		EquipItemFITEM(Position, *(WeaponItemData));
	}
}

void UBattleEntityComponent::EquipItemFITEM(EItemEquipPositionEnum Position, FITEM& Item)
{
	if (Item.itemId == EXCEPTION_NONE)
		return;

	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Item.itemId);
	if (ItemTableInfo == nullptr)
		return;

	UCharacterBattleInfo* CharBattleInfo = GetBattleInfo();
	if (false == IsValid(CharBattleInfo))
		return;

	if (Position != ItemTableInfo->EquipPosition)
		return;


	FHERO						HeroInvenData;
	FWorkedCharacterTableInfo	WorkedCharacterTableInfo;

	RGAMEINSTANCE(this)->RInventory->GetHeroAndTableData(Item.equipedHeroUD, HeroInvenData, WorkedCharacterTableInfo);
	URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(CharBattleInfo->ItemStatsByEquipPosition[(int32)Position], Item);
}

void UBattleEntityComponent::EquipItemFITEMTest(EItemEquipPositionEnum Position, FITEM& Item)
{
	if (Item.itemId == EXCEPTION_NONE)
		return;
	
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Item.itemId);
	if (ItemTableInfo == nullptr)
		return;

	UCharacterBattleInfo* CharBattleInfo = GetBattleInfo();
	if (false == IsValid(CharBattleInfo))
		return;

	if (Position != ItemTableInfo->EquipPosition)
		return;

	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OwnerActor.Get());
	if (!BaseProperty)
		return;

	FWorkedCharacterTableInfo	WorkedCharacterTableInfo;
	URenewal_BattleFunctionLibrary::GetCharacterTableWorkedInfo(WorkedCharacterTableInfo, ECharacterCategoryTypeEnum::VE_Hero, BaseProperty->GetCharacterID(), BaseProperty->GetLevel());
	URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(CharBattleInfo->ItemStatsByEquipPosition[(int32)Position], Item);

}

void UBattleEntityComponent::UnEquipItem(EItemEquipPositionEnum Position)
{
	int32 EquipPos = (int32)Position;
	if (!CharacterBattleInfo->ItemSkillIDList[EquipPos].IsNone())
	{
		auto SkillEntityComponet = GetOwnerSkillComponent();
		if (SkillEntityComponet)
		{
			SkillEntityComponet->RemoveItemSkill(CharacterBattleInfo->ItemSkillIDList[EquipPos]);
			CharacterBattleInfo->ItemSkillIDList[EquipPos] = FName();
		}
	}

	if (Position == EItemEquipPositionEnum::VE_Weapon)
	{
		PrepareWeaponsBase();
	}
}

void UBattleEntityComponent::AttachWeaponInit(UClass* WeaponClassType, bool bLeft )
{
#if LOG_CHARACTER
	UE_LOG(LogBattle, Log, TEXT("UBattleEntityComponent::AttachWeaponInit"));
#endif
	if (IsValid(WeaponClassType))
	{
		FName socketName;
		if (bLeft)
		{
			socketName = TEXT("Socket_LeftWeapon");
		}
		else
		{
			socketName = TEXT("Socket_RightWeapon");
		}

		AWeaponBase* weapon = AttachWeaponBase(WeaponClassType, socketName);
		if (IsValid(weapon))
		{
			IEntityBaseProperty* EntityBaseProperty = Cast< IEntityBaseProperty >(OwnerActor.Get());
			if (EntityBaseProperty)
			{
				FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(EntityBaseProperty->GetCharacterCategory(), EntityBaseProperty->GetCharacterID());
				if (CharacterInfo)
				{
					float CaclWeaponScale = CharacterInfo->ModelScale * CharacterInfo->WeaponScale;
					weapon->SetActorScale3D(FVector(CaclWeaponScale, CaclWeaponScale, CaclWeaponScale));
				}
			}
			

			if (bLeft)
				LWeapon = weapon;
			else
				RWeapon = weapon;

			WeaponObjectList.Emplace(weapon);
			AssignShotPoint(weapon->ShotPointRef);
		}
	}
}

bool UBattleEntityComponent::IsCloseEnoughForAttack(AActor* InTargetActor, float SpecialAttackRange /*= 0.0f*/, float ExtraDist /*= 0.0f*/ )
{
	if (false == IsValid(InTargetActor))
		return false;

	auto InTargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTargetActor);
	if (IsValid(InTargetActor_BattleEntityComponent))
	{
		if (IsValid(InTargetActor) && InTargetActor_BattleEntityComponent->IsDead() == false)
		{
			float AttackRange = GetBattleInfo()->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
			if (0.0f < SpecialAttackRange)
			{
				AttackRange = SpecialAttackRange;
			}

			float CurrentAttackTargetBoundingRadius = InTargetActor_BattleEntityComponent->BoundingRadius;

			float Distance = OwnerActor->GetHorizontalDistanceTo(InTargetActor);
			
			if ((AttackRange + CurrentAttackTargetBoundingRadius + ExtraDist ) >= (Distance)) // add some extra value(10.f) for checking before AI.
			{		
				return true;
			}
		}
	}
	return false;
}

bool UBattleEntityComponent::IsGoalActorCellAdjacent(AActor* InTargetActor)
{
	UGridMapManager* GridMapManager = RGAMEINSTANCE(this)->GridMapManager;

	if (false == IsValid(GridMapManager))
		return false;
	
	FCellIndex GoalActorCellIndex;
	FCellIndex OwnerActorCellIndex;

	GridMapManager->GetCellIndexFromWorldPos(InTargetActor->GetActorLocation(), GoalActorCellIndex);	
	GridMapManager->GetCellIndexFromWorldPos(OwnerActor->GetActorLocation(), OwnerActorCellIndex);

	if (GridMapManager->AreTwoCellsAdjacent(GoalActorCellIndex, OwnerActorCellIndex) == true)
	{
		return true;
	}

	return false;
}

bool UBattleEntityComponent::IsCloseEnoughToActor(AActor* InTargetActor, float ExtraDist )
{
	auto InTargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTargetActor);
	if (IsValid(InTargetActor_BattleEntityComponent))
	{
		if (IsValid(InTargetActor) && InTargetActor_BattleEntityComponent->IsDead() == false)
		{			
			float CurrentTargetBoundingRadius = InTargetActor_BattleEntityComponent->BoundingRadius;

			float Distance = OwnerActor->GetHorizontalDistanceTo(InTargetActor);
			
			if ((BoundingRadius + CurrentTargetBoundingRadius + ExtraDist) >= (Distance)) // add some extra value(10.f) for checking before AI.
			{
				return true;
			}
		}
	}
	return false;
}

bool UBattleEntityComponent::IsTargetInRange(AActor* Target, float Range)
{
	if (false == IsValid(Target))
		return false;

	auto Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
	if (IsValid(Target_BattleEntityComponent))
	{
		if (false == Target_BattleEntityComponent->IsDead())
		{
			float TargetBoundingRadius = Target_BattleEntityComponent->BoundingRadius;
			float Distance = OwnerActor->GetHorizontalDistanceTo(Target);

			if ((TargetBoundingRadius + Range) >= (Distance))
			{
				return true;
			}
		}
	}
	return false;
}

bool UBattleEntityComponent::IsCurrentTargetInNormalAttackRange()
{
	AActor* Target = GetCurrentAttackTarget();
	if (Target)
	{
		return IsCloseEnoughForAttack(Target);
	}
	return false;
}

void UBattleEntityComponent::SetSummonOwner(AActor* InOwner)
{
	SummonerCharacter = InOwner;	
}

AActor* UBattleEntityComponent::GetSummonOwner()
{
	return SummonerCharacter.Get();	
}

bool UBattleEntityComponent::IsBoss()
{
	return bBoss;
}

void UBattleEntityComponent::SetBoss(bool InBoss)
{
	bBoss = InBoss;
}

bool UBattleEntityComponent::IsKeyMonster()
{
	return bKeyMonster;
}

void UBattleEntityComponent::SetKeyMonster(bool InKeyMonster)
{
	bKeyMonster = InKeyMonster;
}

float UBattleEntityComponent::GetBoundingRadius()
{
	return BoundingRadius;
}

void UBattleEntityComponent::SetBoundingRadius(float InBoundingRadius)
{
	BoundingRadius = InBoundingRadius;
}

float UBattleEntityComponent::GetCapsuleComponentHalfHeightValue()
{
	if (OwnerActor.IsValid())
	{
		auto OwnerCharacter = Cast<ACharacter>(OwnerActor.Get());
		if (IsValid(OwnerCharacter))
		{
			auto CapsuleComponent = OwnerCharacter->GetCapsuleComponent();
			if (IsValid(CapsuleComponent))
			{
				return CapsuleComponent->GetUnscaledCapsuleHalfHeight();
			}
		}
	}

	return 0.0f;
}

ECharacterPropertyTypeEnum UBattleEntityComponent::GetAttackPowerType()
{
	UCharacterBattleInfo* battle_info = GetBattleInfo();
	if (battle_info)
	{
		return battle_info->GetCharacterProperty<ECharacterPropertyTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackPropertyType);
	}
	return ECharacterPropertyTypeEnum::VE_None;
}

void UBattleEntityComponent::SendRTS_AttackNow(bool bAttack, int32 AnimSequence)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
//
// 	if (false == RGameInstance->GameEnvironmentVar.ColosseumRTSMode_Attack)
// 		return;

	AActor* Attacker = OwnerActor.Get();
	if (nullptr == Attacker)
		return;

	IEntityBaseProperty* AttackerBaseProp = Cast<IEntityBaseProperty>(Attacker);
	if (nullptr == AttackerBaseProp)
		return;
	
	int32 actorUK = AttackerBaseProp->GetPlayerUK();
	FVector actorLocation = Attacker->GetActorLocation();
	int32 targetUK = 0;

	if (bAttack)
	{
		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		if (EntityAIComponent)
		{
			AActor* Target = EntityAIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
			if (Target)
			{
				/*AActor* Target = GetCurrentAttackTarget();
				if (!IsValid(Target))
					return;*/
				IEntityBaseProperty* TargetBaseProp = Cast<IEntityBaseProperty>(Target);
				if (nullptr == TargetBaseProp)
					return;

				targetUK = TargetBaseProp->GetPlayerUK();
			}
		}
		
	}

#if LOG_NET_RTS
	UE_LOG(LogRTS, Log, TEXT("RTS>> [UBattleEntityComponent] SendRTS_NormalAttack >> actorUK %d targetUK %d bAttack %d"), actorUK, targetUK, bAttack);
#endif
	if (AttackerBaseProp->GetGroupKey() == UObjectFunctionLibrary::GetMyTeamKey())
	{
		UAnimStateComponent* AnimStateComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
		if (AnimStateComp)
		{
			UAttackAnimState* AttackAnimState = Cast<UAttackAnimState>(AnimStateComp->GetAnimState(EAnimStateEnum::VE_NormalAttack));
			if (AttackAnimState)
			{
				if (AttackAnimState->AttackQueue.IsEmpty())
				{
					AttackAnimState->AttackQueue.Enqueue(AnimSequence);

					URTSManager::GetInstancePtr()->REQ_BT_ATTACK_NOW(actorUK, actorLocation, targetUK, bAttack, AnimSequence);
					UE_LOG(LogRTS, Log, TEXT("RTS>> [UBattleEntityComponent] SendRTS_NormalAttack >> actorUK %d targetUK %d bAttack %d"), actorUK, targetUK, bAttack);

 					//FString str = TEXT("Send Packet AttackNow Sequence : ") + FString::FromInt(AnimSequence);
 					//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, str);
				}
			}
		}
	}
		
}

void UBattleEntityComponent::RecvRTS_AttackNow(AActor* TargetActor, bool bAttack)
{
	if (OwnerActor.IsValid() == false)
		return;

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (false == IsValid(OwnerActor_AnimStateComponent))
		return;

	UAttackAnimState* AttackAnimState = Cast<UAttackAnimState>(OwnerActor_AnimStateComponent->GetAnimState(EAnimStateEnum::VE_NormalAttack));
	if (AttackAnimState)
	{
		AttackAnimState->AttackQueue.Empty();
	}

	if (bAttack)
	{
		/*FString str = TEXT("Receive Packet AttackNow Sequence");
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, str);*/

		if (IsValid(TargetActor))
		{
			FVector DirectionVector = TargetActor->GetActorLocation() - OwnerActor->GetActorLocation();
			DirectionVector.Z = 0;

			FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator();
			OwnerActor->SetActorRotation(lookatRotation);

			AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));
			if (StagePC)
			{
				if (StagePC->AIControlMode_UserInput == EAIControlModeEnum::VE_ManualMode)
				{
					if (StagePC->GetLeaderCharacter() == OwnerActor.Get())
					{
						if (StagePC->GetLeaderControlHelper()->ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK))
						{
							//if Receive Packet is arrived earlier than "EndNormalAttack"
							bShouldNextAttack = true;
						}
						else
						{
							//if Receive Packet is arrived after "EndNormalAttack"
							UAIFunctionLibrary::Attack(OwnerActor.Get(), TargetActor);
						}
					}
					else //Crew
					{
						UBattleBaseAnimInstance* battleAnim = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(OwnerActor.Get());
						if (battleAnim)
						{
							
							UAnimMontage* AttackMontage = battleAnim->GetAnimMontageFromAnimMontageType(EAnimMontageType::VE_Attack);
							if (AttackMontage)
							{
								FAnimMontageInstance* MontageInstance = battleAnim->GetActiveInstanceForMontage(AttackMontage);
								if (MontageInstance)
								{
									//if target anim is paused
									if (!MontageInstance->bPlaying)
									{
										//if Receive Packet is arrived after "EndNormalAttack"
										UAIFunctionLibrary::Attack(OwnerActor.Get(), TargetActor);
									}
									else
									{
										//if Receive Packet is arrived earlier than "EndNormalAttack"
										bShouldNextAttack = true;
									}
								}
								else
									UAIFunctionLibrary::Attack(OwnerActor.Get(), TargetActor);
							}
							else
								UAIFunctionLibrary::Attack(OwnerActor.Get(), TargetActor);
						}
					}
						
				}
			}
		}
	}
	else
	{
 		/*FString str = TEXT("Receive Packet AttackNow false!!!");
 		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);*/

		UAIFunctionLibrary::StopAttack(OwnerActor.Get());

		AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));
		if (StagePC)
		{
			if (StagePC->GetLeaderCharacter() == OwnerActor.Get())
				StagePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);
			else
			{
				UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
				if (AIComponent)
					AIComponent->ClearBlackboardValue(ERAIBalckboardValueEnum::VE_CurrentTargetActor);
			}
		}
	}
}

void UBattleEntityComponent::RecvRTS_NormalAttack(AActor* TargetActor, EBattleDamageResultEnum HitDamageResult, int32 HitDamage, uint32 TargetHP, uint8 sequence)
{
	if (!IsValid(TargetActor))
		return;
	if (OwnerActor.IsValid() && TargetActor)
	{
		FVector DirectionVector = TargetActor->GetActorLocation() - OwnerActor->GetActorLocation();
		DirectionVector.Z = 0;
		FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator();
		OwnerActor->SetActorRotation(lookatRotation);
	}
	UBattleEntityComponent* target_battle_comp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (!IsValid(target_battle_comp))
		return;

	if (IsCSNormalAttackReceived)
	{
		CSHitTarget(false);
	}

	CSNormalAttackSyncData.Reset();

	if (target_battle_comp->IsDead())
		return;

	FCSNormalAttackSyncData hit_data;
	hit_data.Clear();

	hit_data.Target = TargetActor;
	hit_data.HitResult = HitDamageResult;;
	hit_data.HitDamage = HitDamage;
	hit_data.SyncHP = TargetHP;

	CSNormalAttackSyncData.Emplace(hit_data);

	//UE_LOG(LogBattle, Log, TEXT("=====>>>>> UBattleEntityComponent::RecvRTS_NormalAttack() : %f"), FPlatformTime::Seconds());

	if (EBattleDamageResultEnum::VE_Dead == HitDamageResult)
	{
		IEntityBaseProperty* TargetActor_BaseProp = Cast<IEntityBaseProperty>(TargetActor);
		if (TargetActor_BaseProp)
		{
			TargetActor_BaseProp->ReSetRealTimeParam();
		}
	}
	Attack_RTS_Sequence = sequence;
	IsCSNormalAttackReceived = true;
	SetCurrentAttackTarget(TargetActor);
}

void UBattleEntityComponent::CSHitTarget(bool HitWithBullet)
{
	if (IsCSNormalAttackReceived && 0 < CSNormalAttackSyncData.Num())
	{
		AActor* Attacker = OwnerActor.Get();

		AActor* TargetActor = nullptr;
		IEntityBaseProperty* TargetBaseProperty = nullptr;
		UBattleEntityComponent* TargetBattleEntityComp = nullptr;
		UBattleInfoBlackboard* TargetBattleBlackboard = nullptr;

		for (auto AttackData : CSNormalAttackSyncData)
		{
			if (AttackData.Target.IsValid())
			{
				TargetActor = AttackData.Target.Get();

				TargetBaseProperty = Cast<IEntityBaseProperty>(TargetActor);
				TargetBattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);

				if (IsValid(TargetBattleEntityComp) && nullptr != TargetBaseProperty)
				{
//					UE_LOG(LogBattle, Log, TEXT("UBattleEntityComponent::CSHitTarget %s MaxHP %d CurHP %d SyncHP %d"), *(TargetActor->GetName()), TargetBattleEntityComp->GetMaxHP(), TargetBattleEntityComp->GetHP(), AttackData.SyncHP);

					if (/*false == TargetBattleEntityComp->IsDead() && */TargetBaseProperty->IsReady())
					{
						TargetBattleBlackboard = TargetBattleEntityComp->GetBattleInfoBlackboard();
						if (TargetBattleBlackboard)
						{
							if (false == HitWithBullet)	TargetBattleBlackboard->LastAttackType = EAttackTypeEnum::VE_Melee;
							else						TargetBattleBlackboard->LastAttackType = EAttackTypeEnum::VE_Range;

							TargetBattleBlackboard->LastAttackPropertyType = GetBattleInfo()->GetCharacterProperty<ECharacterPropertyTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackPropertyType);
						}

						TargetBattleEntityComp->CSOnDamage(Attacker, AttackData);
						TargetBattleEntityComp->OnNotifyDamageDelegate.Broadcast(Attacker, TargetActor, AttackData.HitDamage, false);
					}
				}
				OnHitDelegate.Broadcast(Attacker, AttackData.HitResult, TargetActor, false);
			}
		}

		if (EBattleDamageResultEnum::VE_Heal == CSNormalAttackerChangeHPData.HitResult ||
			EBattleDamageResultEnum::VE_NoneHitDamage == CSNormalAttackerChangeHPData.HitResult)
		{
			CSOnDamage(nullptr, CSNormalAttackerChangeHPData);
		}
		CSNormalAttackerChangeHPData.Clear();
	}

	IsCSNormalAttackReceived = false;
	CSNormalAttackSyncData.Reset();
}

void UBattleEntityComponent::CSOnDamage(AActor* Attacker, FCSNormalAttackSyncData& AttackData)
{
	//UE_LOG(LogBattle, Log, TEXT("=====>>>>> UBattleEntityComponent::CSOnDamage() : %f"), FPlatformTime::Seconds());

	// for DamageFloater (Not to Apply)
	//BattleOperator->ApplyAdditionalDamage(AttackData.HitDamage, Attacker, AttackData.HitResult);

	OnReceiveDamageEvent(AttackData.HitDamage, AttackData.HitResult, Attacker);

	// save blackboard data
	GetBattleInfoBlackboard()->FinalDamage = AttackData.HitDamage;
	GetBattleInfoBlackboard()->LastDamageResultType = AttackData.HitResult;
	GetBattleInfoBlackboard()->LastAttackCharacter = Attacker;

	SetHP(AttackData.SyncHP);

	//if (0 >= AttackData.SyncHP)
	if (AttackData.HitResult == EBattleDamageResultEnum::VE_Dead)
	{
		DeadFromActor = Attacker;
	}
}

void UBattleEntityComponent::StopBattleEntityForced()
{
	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		UIAnimState* CurrAnimState = OwnerActor_AnimStateComponent->GetCurrentAnimState();
		if (CurrAnimState)
		{
			CurrAnimState->Leave();
		}
	}

	auto EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	EntityRenderComponent->CharacterChangeOpacity(1.0f);
	EntityRenderComponent->ClearDecoParticles();
	EntityRenderComponent->ClearAllEffectForced();

	ClearAllAbnormalState();

	if (CharacterBattleInfo)
	{
		CharacterBattleInfo->RemoveAllSummonedMinion();
	}
}

void UBattleEntityComponent::AdjustForceTutorialColosseumDamage(float& FinalDamage)
{
	if (0.0f < FinalDamage)
	{
		if (bOwnerIsAccountChar)
		{
			FinalDamage = FinalDamage * 0.1f;
		}
		else
		{
			FinalDamage = FinalDamage * 10.0f;
		}
	}
}