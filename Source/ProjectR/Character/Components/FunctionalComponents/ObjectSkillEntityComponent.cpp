// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ObjectSkillEntityComponent.h"
#include "Character/BattleObject/BattleBaseObject.h"



// Sets default values for OwnerCharacter.Get() component's properties
UObjectSkillEntityComponent::UObjectSkillEntityComponent()
{
	
}


// Called when the game starts
void UObjectSkillEntityComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

void UObjectSkillEntityComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

}

// Called every frame
void UObjectSkillEntityComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* thisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, thisTickFunction);
		
	
}

void UObjectSkillEntityComponent::StartSkillApply()
{
	Super::StartSkillApply();

	IsSkillUsing = false;
}

void UObjectSkillEntityComponent::EndSkillAction_V2()
{
	Super::EndSkillAction_V2();

	IsSkillUsing = false;
}