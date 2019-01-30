// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RemoteEntityAIComponent.h"
#include "Character/BaseAIController.h"
#include "UtilFunctionIntegrated.h"


// Sets default values for OwnerCharacter.Get() component's properties
URemoteEntityAIComponent::URemoteEntityAIComponent()
{	
}


// Called when the game starts
void URemoteEntityAIComponent::InitializeComponent()
{
	Super::InitializeComponent();
	auto SpawnedCharacter_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
	if (SpawnedCharacter_EntityAIComponent)
		SpawnedCharacter_EntityAIComponent->SetAIControlMode(EAIControlModeEnum::VE_ManualMode);
}

void URemoteEntityAIComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}

// Called every frame
void URemoteEntityAIComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* thisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, thisTickFunction);
}

void URemoteEntityAIComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URemoteEntityAIComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


// void URemoteEntityAIComponent::AIRestart()
//{
//	// nothing..
//}