// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "DieAnimState.h"

#include "UtilFunctionLibrary.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "DirectionTrackManager.h"

UDieAnimState::UDieAnimState()
{
	AnimStateType = EAnimStateEnum::VE_Die;
}

void UDieAnimState::Initialize()
{
	Super::Initialize();
}

void UDieAnimState::Enter(ECharacterCommandEnum Command, FVariantData varParam)
{
	Super::Enter(Command, varParam);
}

void UDieAnimState::Leave()
{
	Super::Leave();
	
}

bool UDieAnimState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	return true;
}

void UDieAnimState::SetAnimParam(ECharacterCommandEnum Command, FVariantData& varParam)
{
	Super::SetAnimParam(Command, varParam);
}

bool UDieAnimState::IsValidState()
{
	return true;
}

bool UDieAnimState::IsCancelAnim()
{
	return false;
}

EAnimStateEnum UDieAnimState::DoCommand(ECharacterCommandEnum Command, bool OnlyChecked)
{
	return EAnimStateEnum::VE_Die;
}