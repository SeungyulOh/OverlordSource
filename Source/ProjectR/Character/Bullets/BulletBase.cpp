// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"

#include "UtilFunctionIntegrated.h"
#include "Table/CharacterTableInfo.h"

#include "BulletBase.h"


ABulletBase::ABulletBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	InitMembers();
}

void ABulletBase::Init()
{
	OwnerCharacter = nullptr;
	OwnerSkill_V2 = nullptr;
}

void ABulletBase::InitMembers()
{
	Init();
}

void ABulletBase::SetSkill_V2(UISkill_V2* Skill)
{
	OwnerSkill_V2 = Skill;
}


UISkill_V2* ABulletBase::GetOwnerSkill_V2()
{
	return OwnerSkill_V2;
}

void ABulletBase::SetSelfDestory(float Time)
{
	DestroyTime = Time;
	bSelfDestroy = true;
}

void ABulletBase::UpdateDestroy(float DeltaSecond)
{
	if (bSelfDestroy)
	{
		DestroyTime -= DeltaSecond;
		if (DestroyTime <= 0.0f)
		{
			Destroy();
		}
	}
}

void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletBase::Destroyed()
{
	Super::Destroyed();
}

void ABulletBase::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	UpdateDestroy(DeltaSecond);
}

void ABulletBase::SetForcePause(bool bPause, float time)
{
	if (bPause)
	{
		ForcePauseStart(time);
	}
	else
	{
		ForcePauseEnd();
	}
}

void ABulletBase::ForcePauseStart(float time)
{
	SetPauseSetting(true);

	GetWorldTimerManager().ClearTimer(ForcePauseHandler);
	
	if (time > 0.0f)
	{
		GetWorldTimerManager().SetTimer(ForcePauseHandler, this, &ABulletBase::ForcePauseEnd, time, false);
	}

	if(bSelfDestroy)
	{
		bSelfDestroy = false;
		IsSelfDestroyBullet = true;
	}
}

void ABulletBase::ForcePauseEnd()
{
	SetPauseSetting(false);

	GetWorldTimerManager().ClearTimer(ForcePauseHandler);

	if (IsSelfDestroyBullet)
	{
		bSelfDestroy = true;
	}
}

void ABulletBase::SetPauseSetting(bool bPause)
{
	bool bEnable = !bPause;
	this->SetActorTickEnabled(bEnable);

	TArray<UActorComponent*> components;
	this->GetComponents(components);

	for (UActorComponent* component : components)
	{
		component->PrimaryComponentTick.SetTickFunctionEnable(bEnable);
	}
}
