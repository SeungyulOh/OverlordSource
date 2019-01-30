// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_HeroBreakUp_Splash.h"
#define REWARDMAXCNT 3

void UUC_Popup_HeroBreakUp_Splash::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_Popup_HeroBreakUp_Splash::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Popup_HeroBreakUp_Splash::Init()
{
	FindChildWidgetsWithCertainName<URenewal_BaseHeroIcon>(this, Icon, TEXT("Icon_"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text, TEXT("TextName_"));
}

void UUC_Popup_HeroBreakUp_Splash::SetReward(const TArray<FREWARD>& Inrewards)
{
	for (int32 i = 0; i < REWARDMAXCNT; i++)
	{

	}
}

