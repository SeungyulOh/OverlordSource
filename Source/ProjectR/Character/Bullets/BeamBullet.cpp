// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BeamBullet.h"

#include "Character/BattleObject/BattleBaseObject.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"



ABeamBullet::ABeamBullet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Scene")));
	EffectComponent = ObjectInitializer.CreateDefaultSubobject< UParticleSystemComponent >(this, TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(RootComponent);
}

void ABeamBullet::InitTarget(AActor* Target)
{
	TargetObject = Target;

	UpdateEffect();
		

	SetSelfDestory(1.0f);
}

void ABeamBullet::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	UpdateEffect();
}

void ABeamBullet::UpdateEffect()
{
	if (!IsValid(TargetObject))
		return;

	if (!IsValid(EffectComponent))
		return;

	ABattleBaseObject* battleObject = Cast<ABattleBaseObject>(TargetObject);
	if (IsValid(battleObject))
	{
		EffectComponent->SetBeamTargetPoint(0, battleObject->GetStaticMesh()->GetSocketLocation(SOCKET_DAMAGE), 0);
	}
	else
	{
		ACharacter* TargetCharacter = Cast< ACharacter >(TargetObject);
		if( TargetCharacter )
			EffectComponent->SetBeamTargetPoint(0, TargetCharacter->GetMesh()->GetSocketLocation(SOCKET_DAMAGE), 0);
	}
}