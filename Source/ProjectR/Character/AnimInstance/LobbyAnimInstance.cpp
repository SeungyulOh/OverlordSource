// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "LobbyAnimInstance.h"

#include "GlobalIntegrated.h"


#include "Character/AnimInstance/AnimInstanceDefs.h"
#include "UtilFunctionIntegrated.h"

ULobbyAnimInstance::ULobbyAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ULobbyAnimInstance::PostLoad()
{
	Super::PostLoad();
}

void ULobbyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PlayAnimMontageLobbySection(ANIM_MONTAGE_LOBBY_IDLE);
}

void ULobbyAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);
}

void ULobbyAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

bool ULobbyAnimInstance::PlayAnimMontageLobbySection(const FName& SectionName, bool ForcePlay)
{
	if (SectionName.IsNone())
		return false;

	UAnimMontage* AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageLobby);
	if (false == IsValid(AnimMontage))
		return false;
	if (false == AnimMontage->IsValidSectionName(SectionName))
		return false;
	
	if (false == ForcePlay)
	{
		if (Montage_IsPlaying(AnimMontage))
		{
			FName CurrentSection = Montage_GetCurrentSection(AnimMontage);
			if (CurrentSection.IsEqual(SectionName))
				return false;
		}
	}
	
	Montage_Play(AnimMontage, 1.0f);
	Montage_JumpToSection(SectionName, AnimMontage);
	/*
	if (false == NextSectionName.IsNone())
	{
		Montage_SetNextSection(SectionName, NextSectionName, AnimMontage);
	}
	*/
	return true;
}

bool ULobbyAnimInstance::PlayAnimMontageEventSection(const FName& SectionName, bool ForcePlay)
{
	if (SectionName.IsNone())
		return false;

	UAnimMontage* AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageEvent);
	if (false == IsValid(AnimMontage))
		return false;

	if (false == ForcePlay)
	{
		if (Montage_IsPlaying(AnimMontage))
		{
			FName CurrentSection = Montage_GetCurrentSection(AnimMontage);
			if (CurrentSection.IsEqual(SectionName))
				return false;
		}
	}

	Montage_Play(AnimMontage, 1.0f);
	Montage_JumpToSection(SectionName, AnimMontage);

	return true;
}

bool ULobbyAnimInstance::PlayAnimMontageHeroShopSection(const FName& SectionName, bool ForcePlay /*= false*/)
{
	if (SectionName.IsNone())
		return false;

	UAnimMontage* AnimMontage = UUtilFunctionLibrary::GetAnimMontageFromAssetID(AnimMontageHeroShop);
	if (false == IsValid(AnimMontage))
		return false;
	if (false == AnimMontage->IsValidSectionName(SectionName))
		return false;

	if (false == ForcePlay)
	{
		if (Montage_IsPlaying(AnimMontage))
		{
			FName CurrentSection = Montage_GetCurrentSection(AnimMontage);
			if (CurrentSection.IsEqual(SectionName))
				return false;
		}
	}

	Montage_Play(AnimMontage, 1.0f);
	Montage_JumpToSection(SectionName, AnimMontage);
	return true;
}
