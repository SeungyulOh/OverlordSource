// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LobbyCharacter.h"
#include "SharedConstants/GlobalConstants.h"

ALobbyCharacter::ALobbyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyCharacter::Destroyed()
{
	Super::Destroyed();
}

void ALobbyCharacter::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
}