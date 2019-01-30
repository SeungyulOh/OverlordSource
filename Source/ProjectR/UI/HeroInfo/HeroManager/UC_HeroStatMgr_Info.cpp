// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroStatMgr_Info.h"

void UUC_HeroStatMgr_Info::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_HeroStatMgr_Info::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HeroStatMgr_Info::Init()
{
	FindChildWidgetsWithCertainName<UUC_HeroInfoStat>(this, HeroStat, TEXT("HeroStatInfo_"));
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_HeroStatMgr_Info::OnClick_Close);
		Button_Close->OnClicked.AddDynamic(this, &UUC_HeroStatMgr_Info::OnClick_Close);
	}
		
}

void UUC_HeroStatMgr_Info::Release()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_HeroStatMgr_Info::SetStatInfo(UCharacterBattleInfo* InUIBattleInfo)
{
	for (UUC_HeroInfoStat* Stat : HeroStat)
		Stat->Refresh(InUIBattleInfo);
}

void UUC_HeroStatMgr_Info::OnClick_Close()
{
	Release();
}

