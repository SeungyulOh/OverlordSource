// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_DailyContentsList.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"


void UUC_DailyContentsList::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Proceed)
	{
		Button_Proceed->OnClicked.RemoveDynamic(this, &UUC_DailyContentsList::OnClickDirectLink);
		Button_Proceed->OnClicked.AddDynamic(this, &UUC_DailyContentsList::OnClickDirectLink);
	}
}

void UUC_DailyContentsList::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_DailyContentsList::OnClickDirectLink()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32  SubKey = 0;
	bool bCloseContents = false;
	switch (eContentType)
	{
	case EGamePlayModeTypeEnum::VE_Campaign:
		SubKey = 1;
		if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Campaign, SubKey))
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Campaign);
		else 
			bCloseContents = true;
		break;
	case EGamePlayModeTypeEnum::VE_Arena:
		if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Arena, SubKey))
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Arena);
		else
			bCloseContents = true;
		break;
	case EGamePlayModeTypeEnum::VE_Colosseum:
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Colosseum);
		break;
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
		if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_MirrorDungeon, SubKey))
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_MirrorDungeon);
		else
			bCloseContents = true;
		break;
	case EGamePlayModeTypeEnum::VE_SingleRaid:
		SubKey = 1;
		if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_SingleRaid, SubKey))
		{
			RGameInstance->RaidManager->TodayType = RGameInstance->GetRaidDayType();
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_SingleRaid);
		}
		else
			bCloseContents = true;
		break;
	default:
		break;
	}

	if(bCloseContents)
	{
		FText TitleText;
		FText DescText;

		RGameInstance->ContentsUnLockManager->GetUnlockReasonText(eContentType, TitleText, DescText, SubKey);
		UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
	}

}

void UUC_DailyContentsList::SetContents(EGamePlayModeTypeEnum InContentType, int32 InContentsScore)
{
	eContentType = InContentType;

	FText ContentsName = FText();

	switch (eContentType)
	{
	case EGamePlayModeTypeEnum::VE_Campaign:
		ContentsName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_LobbyMenu_Adventure");
		break;
	case EGamePlayModeTypeEnum::VE_Arena:
		ContentsName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_lobby_title_Arena");
		break;
	case EGamePlayModeTypeEnum::VE_Colosseum:
		ContentsName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_lobby_title_Colosseum");
		break;
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
		ContentsName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UIPage_MirrorDungeon");
		break;
	case EGamePlayModeTypeEnum::VE_SingleRaid:
		ContentsName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UIPage_TowerBossLobby");
		break;
	default:
		break;
	}

	if (IsValid(ContentsNameText))
	{
		ContentsNameText->SetText(ContentsName);
	}
	
	if(IsValid(ContentsScore))
	{
		ContentsScore->SetText(FText::AsNumber(InContentsScore));
	}
}
