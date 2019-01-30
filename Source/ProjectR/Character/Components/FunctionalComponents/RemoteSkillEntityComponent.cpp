// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RemoteSkillEntityComponent.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"


// Sets default values for OwnerActor.Get() component's properties
URemoteSkillEntityComponent::URemoteSkillEntityComponent()
{
	
}


// Called when the game starts
void URemoteSkillEntityComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void URemoteSkillEntityComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}

// Called every frame
void URemoteSkillEntityComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* thisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, thisTickFunction);
}

void URemoteSkillEntityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URemoteSkillEntityComponent::EndPlay(const EEndPlayReason::Type EEndPlayReason)
{
	Super::EndPlay(EEndPlayReason);
}

void URemoteSkillEntityComponent::UpdateSkillComponent(float DeltaSecond)
{
	if (!bActivated)
		return;

	if ( IsValid( CurrentSkill_V2) )
	{
		CurrentSkill_V2->Update(DeltaSecond);
	}

	UpdateSkillInst(DeltaSecond);
}