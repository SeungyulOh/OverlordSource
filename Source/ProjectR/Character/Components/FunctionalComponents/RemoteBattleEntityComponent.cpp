// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "RemoteBattleEntityComponent.h"

#include "GlobalIntegrated.h"

#include "Character/Components/AnimStateComponent.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/RemoteMoveComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Character/SharedBattle/BattleOperator.h"

// Sets default values for OwnerActor.Get() component's properties
URemoteBattleEntityComponent::URemoteBattleEntityComponent()
{
	
}


// Called when the game starts
void URemoteBattleEntityComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

void URemoteBattleEntityComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}

// Called every frame
void URemoteBattleEntityComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* thisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, thisTickFunction);
}

void URemoteBattleEntityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URemoteBattleEntityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

//////////////////////////////////////////////////////////////////////////

void URemoteBattleEntityComponent::Dead( bool InIsDestroy /*= false*/)
{
	if (OwnerActor.IsValid() == false)
		return;
	

	IEntityBaseProperty* prop = Cast<IEntityBaseProperty>(OwnerActor);
	if (prop)
	{
		if (!RGAMEINSTANCE(GEngine)->HttpClient->IsValidKID(prop->GetUserKID()))
		{
			auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
			if (IsValid(OwnerActor_AnimStateComponent))
			{
				OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_Die);
				return;
			}
		}
	}
	

	OnDeadEvent(DeadFromActor.Get());

	UE_LOG(LogBattle, Log, TEXT("Dead Character : %s"), *(OwnerActor->GetName()));

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_Die);
	}

	auto OwnerActor_RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_RenderComponent))
	{
		auto SelectionIndicatorMeshCom = OwnerActor_RenderComponent->GetSelectionIndicatorMeshComponent();
		if (SelectionIndicatorMeshCom)
		{
			SelectionIndicatorMeshCom->SetVisibility(false);
		}

		auto EnemyGlowMeshComponent = OwnerActor_RenderComponent->GetTargetingMeshComponent();
		if (EnemyGlowMeshComponent)
		{
			EnemyGlowMeshComponent->SetVisibility(false);
		}

		OwnerActor_RenderComponent->RemoveDieDestroyEffect();
	}

	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_SkillEntityComponent))
	{
		if (OwnerActor_SkillEntityComponent->GetCurrentSkill_V2() != nullptr)
		{
			OwnerActor_SkillEntityComponent->GetCurrentSkill_V2()->End();
			OwnerActor_SkillEntityComponent->SetCurrentSkill_V2(nullptr);
		}
	}

	CharacterBattleInfo->SetHPZeroAllSummonedMinion();
	if (OwnerActor.IsValid() && SummonerCharacter.IsValid())
	{
		auto SummonerCharacter_BattelEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SummonerCharacter.Get());
		if (IsValid(SummonerCharacter_BattelEntityComponent))
		{
			if (false == SummonerCharacter_BattelEntityComponent->IsDead())
			{
				UCharacterBattleInfo* BattleInfo = SummonerCharacter_BattelEntityComponent->GetBattleInfo();
				BattleInfo->RemoveSummonedMinion(OwnerActor.Get());
				SummonerCharacter = nullptr;
			}
		}
	}

	auto OwnerActor_AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
	if (IsValid(OwnerActor_AIComponent))
		OwnerActor_AIComponent->AIStop();

	DeadClear();
	
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.RemoveObject(OwnerActor.Get(), InIsDestroy);
	}	
}


void URemoteBattleEntityComponent::SetHP(float Value)
{
	// Do Nothing
}

void URemoteBattleEntityComponent::AddHP(float Value, AActor* FromActor, bool IsDisplayValue)
{
	// Do Nothing
}

//////////////////////////////////////////////////////////////////////////

void URemoteBattleEntityComponent::Remote_Dead(AActor* FromActor)
{
	Super::Dead();
}


void URemoteBattleEntityComponent::Remote_SetHP(float Value)
{
	if (OwnerActor.IsValid() == false)
		return;
	if (bRealTimeMode && IsDying())
		return;

	CharacterBattleInfo->SetHP(Value);
}

void URemoteBattleEntityComponent::Remote_AddHP(float Value, bool IsDisplayValue)
{
	if (OwnerActor.IsValid() == false)
		return;
	if (bRealTimeMode && IsDying())
		return;

	CharacterBattleInfo->AddHP(FMath::TruncToInt(Value));
}

void URemoteBattleEntityComponent::RecvRTS_AttackNow(AActor* TargetActor, bool bAttack)
{
	if (OwnerActor.IsValid() == false)
		return;

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerActor.Get());
	if (false == IsValid(OwnerActor_AnimStateComponent))
		return;

	if (bAttack)
	{
		if (IsValid(TargetActor))
		{
			FVector DirectionVector = TargetActor->GetActorLocation() - OwnerActor->GetActorLocation();
			DirectionVector.Z = 0;

			FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator();
			OwnerActor->SetActorRotation(lookatRotation);

			UAIFunctionLibrary::Attack(OwnerActor.Get(), TargetActor);
		}
	}
	else
	{
		UAIFunctionLibrary::StopAttack(OwnerActor.Get());
	}
}

void URemoteBattleEntityComponent::RecvRTS_NormalAttack(AActor* TargetActor, EBattleDamageResultEnum HitDamageResult, int32 HitDamage, uint32 TargetHP,  uint8 sequence)
{
	Super::RecvRTS_NormalAttack(TargetActor, HitDamageResult, HitDamage, TargetHP, sequence);
}

void URemoteBattleEntityComponent::CSOnDamage(AActor* Attacker, FCSNormalAttackSyncData& AttackData)
{
	UE_LOG(LogBattle, Log, TEXT("=====>>>>> URemoteBattleEntityComponent::CSOnDamage() : %f"), FPlatformTime::Seconds());

	if (OwnerActor.IsValid() == false)
		return;

	//BattleOperator->ApplyAdditionalDamage(AttackData.HitDamage, Attacker, AttackData.HitResult);


	EBattleDamageResultEnum DamageFloterResult = AttackData.HitResult;
	OnReceiveDamageEvent(AttackData.HitDamage, DamageFloterResult, Attacker);

	// save blackboard data
	GetBattleInfoBlackboard()->FinalDamage = AttackData.HitDamage;
	GetBattleInfoBlackboard()->LastDamageResultType = AttackData.HitResult;
	GetBattleInfoBlackboard()->LastAttackCharacter = Attacker;


	CharacterBattleInfo->SetHP(AttackData.SyncHP);

	if (0 >= AttackData.SyncHP)
	{
		if (BattleOperator->CheckPassiveDamageOnDead(OwnerActor.Get()) == true)
		{
			CharacterBattleInfo->SetHP(1);
		}
	}

	if (AttackData.HitResult == EBattleDamageResultEnum::VE_Dead)
	{
		DeadFromActor = Attacker;
	}
}

void URemoteBattleEntityComponent::ForceDead(AActor* FromActor)
{
	Remote_SetHP(-1);
	SetDying(true);
	
	if (IsValid(FromActor))
	{
		DeadFromActor = FromActor;
	}
}

