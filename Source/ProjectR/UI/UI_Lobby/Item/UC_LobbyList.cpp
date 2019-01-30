// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
//#include "UI/UIDefs.h"
#include "UC_LobbyList.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"

#include "UI/Component/LocalizingTextBlock.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/UC_Notify.h"
#include "Network/PacketFunctionLibrary.h"

#define BG_LOBBY_COLOSSEUM			FName("BG_Lobby_Colosseum")
#define BG_LOBBY_ARENA				FName("BG_Lobby_Arena")
#define BG_LOBBY_BATTLEGROUND		FName("BG_Lobby_BattleGround")
#define BG_LOBBY_ULTIMATE			FName("BG_Lobby_Openround")
#define BG_LOBBY_RANK				FName("BG_Lobby_Ranking")
#define BG_LOBBY_SCENARIO			FName("BG_Lobby_Scenario")
//#define BG_LOBBY_EPISODE			FName("BG_Lobby_Openround")
#define BG_LOBBY_RAID				FName("BG_Lobby_Raid")
#define BG_LOBBY_CRUSADER			FName("BG_Lobby_Crusader")
#define BG_LOBBY_DAILY				FName("BG_Lobby_Daily")
#define BG_LOBBY_HERO_EPISODE		FName("BG_Lobby_HeroEpisode")

#define TITLE_LOBBY_COLOSSEUM		FName("UI_lobby_title_Colosseum")
#define TITLE_LOBBY_ARENA			FName("UI_lobby_title_Arena")
#define TITLE_LOBBY_BATTLEGROUND	FName("UIPage_BattleZone")
#define TITLE_LOBBY_ULTIMATE		FName("UIPage_KingBattle")
#define TITLE_LOBBY_RANK			FName("UIPage_RankingBattle")
#define TITLE_LOBBY_SCENARIO		FName("UI_lobby_title_Scenario")
#define TITLE_LOBBY_EPISODE			FName("UIPage_HeroRune_Result")
#define TITLE_LOBBY_RAID			FName("UIPage_Raid_List")
#define TITLE_LOBBY_CRUSADER		FName("UI_lobby_title_Crusader")
#define TITLE_LOBBY_DAILY			FName("UI_lobby_title_Eventcampaign")

#define DESC_LOBBY_COLOSSEUM		FName("UIPage_Colosseum_EnterRule")
#define DESC_LOBBY_ARENA			FName("UIPage_Arena_EnterRule")
#define DESC_LOBBY_BATTLEGROUND		FName("UIPage_BattleZone_EnterRule")
#define DESC_LOBBY_ULTIMATE			FName("UIPage_KingBattle_EnterRule")
#define DESC_LOBBY_RANK				FName("UIPage_RankingBattle_EnterRule")
#define DESC_LOBBY_SCENARIO			FName("UI_Scenario_Desc")
#define DESC_LOBBY_EPISODE			FName("UIPage_HeroRune_Desc")
#define DESC_LOBBY_RAID				FName("UIPage_Raid_Desc")
#define DESC_LOBBY_CRUSADER			FName("UI_Crusader_Desc")
#define DESC_LOBBY_DAILY			FName("UIPage_DailyDungeon_EnterRule")




void UUC_LobbyList::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUC_LobbyList::OnClickButton);
		Button_Click->OnClicked.AddDynamic(this, &UUC_LobbyList::OnClickButton);
	}

	RGameInstance = RGAMEINSTANCE(this);
}


void UUC_LobbyList::NativeDestruct()
{
	if (RGameInstance)
	{
		RGameInstance->GetTimerManager().ClearTimer(Handler_RestTime);
	}

	Super::NativeDestruct();
}


void UUC_LobbyList::SetState(EUIMenuIDEnum InUIMenuID)
{
	if (UIMenuID == InUIMenuID)
		return;
	
	UIMenuID = InUIMenuID;
	
	FName TextureKey;
	FName TitleKey;
	FName DescKey;
	FFormatOrderedArguments Args;
	ENotifyStateFlagEnum Notify;

	switch (UIMenuID)
	{
		case EUIMenuIDEnum::VE_Colosseum:
		{
			TextureKey = BG_LOBBY_COLOSSEUM;
			ContentsUnlockKey = EOVERLORD::COLOSSEUM;
			TitleKey = TITLE_LOBBY_COLOSSEUM;
			DescKey = DESC_LOBBY_COLOSSEUM;
			Notify = ENotifyStateFlagEnum::BM_Colosseum;
		}
		break;
		case EUIMenuIDEnum::VE_Arena:
		{
			TextureKey = BG_LOBBY_ARENA;
			ContentsUnlockKey = EOVERLORD::ARENA;
			TitleKey = TITLE_LOBBY_ARENA;
			DescKey = DESC_LOBBY_ARENA;
			Notify = ENotifyStateFlagEnum::BM_Arena;
		}
		break;
		case EUIMenuIDEnum::VE_BattleGround:
		{
			TextureKey = BG_LOBBY_BATTLEGROUND;
			ContentsUnlockKey = EOVERLORD::BATTLEGROUND_20;
			TitleKey = TITLE_LOBBY_BATTLEGROUND;
			DescKey = DESC_LOBBY_BATTLEGROUND;
			Notify = ENotifyStateFlagEnum::BM_BattleGround;
		}
		break;
		case EUIMenuIDEnum::VE_UltimateBattle:
		{
			TextureKey = BG_LOBBY_ULTIMATE;
			ContentsUnlockKey = EOVERLORD::TUTORIAL;
			TitleKey = TITLE_LOBBY_ULTIMATE;
			DescKey = DESC_LOBBY_ULTIMATE;
			Notify = ENotifyStateFlagEnum::BM_None;
		}
		break;
		case EUIMenuIDEnum::VE_RankBattle:
		{
			TextureKey = BG_LOBBY_RANK;
			ContentsUnlockKey = EOVERLORD::TUTORIAL;
			TitleKey = TITLE_LOBBY_RANK;
			DescKey = DESC_LOBBY_RANK;
			Notify = ENotifyStateFlagEnum::BM_None;
		}
		break;
		case EUIMenuIDEnum::VE_Campaign:
		{
			TextureKey = BG_LOBBY_SCENARIO;
			ContentsUnlockKey = EOVERLORD::CAMPAIGN;
			TitleKey = TITLE_LOBBY_SCENARIO;
			DescKey = DESC_LOBBY_SCENARIO;
			Notify = ENotifyStateFlagEnum::BM_Campaign;
		}
		break;
		case EUIMenuIDEnum::VE_HeroRuneSelect:
		{
			TextureKey = BG_LOBBY_HERO_EPISODE;//RGameInstance->HeroRuneDungeonManager->GetHeroTextureKey();
			ContentsUnlockKey = EOVERLORD::HERO_EPISODE;
			TitleKey = TITLE_LOBBY_EPISODE;
			DescKey = DESC_LOBBY_EPISODE;
			Notify = ENotifyStateFlagEnum::BM_None;

// 			FText HeroText = RGameInstance->HeroRuneDungeonManager->GetHeroDisplayText();
// 			Args.Emplace(HeroText);
		}
		break;
		case EUIMenuIDEnum::VE_Raid_List:
		{
			TextureKey = BG_LOBBY_RAID;
			ContentsUnlockKey = EOVERLORD::RAID;
			TitleKey = TITLE_LOBBY_RAID;
			DescKey = DESC_LOBBY_RAID;
			Notify = ENotifyStateFlagEnum::BM_Raid;
		}
		break;
		case EUIMenuIDEnum::VE_Crusader:
		{
			TextureKey = BG_LOBBY_CRUSADER;
			ContentsUnlockKey = EOVERLORD::CRUSADER;
			TitleKey = TITLE_LOBBY_CRUSADER;
			DescKey = DESC_LOBBY_CRUSADER;
			Notify = ENotifyStateFlagEnum::BM_None;
		}
		break;
		case EUIMenuIDEnum::VE_WeekDungeon:
		{
			TextureKey = BG_LOBBY_DAILY;
			ContentsUnlockKey = EOVERLORD::CHALLENGE;
			TitleKey = TITLE_LOBBY_DAILY;
			DescKey = DESC_LOBBY_DAILY;
			Notify = ENotifyStateFlagEnum::BM_WeekDungeon;
		}
		break;
		case EUIMenuIDEnum::VE_None:
		default:
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
			ContentsUnlockKey = EOVERLORD::TUTORIAL;
			Notify = ENotifyStateFlagEnum::BM_None;
		}
		break;
	}
	
	//if (TextureKey.IsNone())
	//	return;


	//checkcontents
	if (Image_Icon)
	{
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(TextureKey);
		UUIFunctionLibrary::SetImageBrush(Image, Image_Icon);
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
// 	if (Overlay_Time)
// 	{
// 		if (UIMenuID == EUIMenuIDEnum::VE_HeroRuneSelect)
// 		{
// 			if (Handler_RestTime.IsValid())
// 			{
// 				RGameInstance->GetTimerManager().ClearTimer(Handler_RestTime);
// 			}
// 			RGameInstance->GetTimerManager().SetTimer(Handler_RestTime, this, &UUC_LobbyList::OnCheckRestTime, 1.0f, true);
// 			OnCheckRestTime();
// 			Overlay_Time->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 		else
// 		{
// 			if (Handler_RestTime.IsValid())
// 			{
// 				RGameInstance->GetTimerManager().ClearTimer(Handler_RestTime);
// 			}
// 			Overlay_Time->SetVisibility(ESlateVisibility::Collapsed);
// 		}
// 	}

	if (LText_Title)
	{
		if (Args.Num() > 0)
		{
			LText_Title->LocalizingKey = "";

			FText TitleText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TitleKey);
			TitleText = FText::Format(TitleText, Args);

			LText_Title->SetText(TitleText);
		}
		else
		{
			LText_Title->LocalizingKey = TitleKey;
			LText_Title->ApplyLocalizedText();
		}
	}

	if (LText_Desc)
	{
		LText_Desc->LocalizingKey = DescKey;
		LText_Desc->ApplyLocalizedText();
	}

	if (UC_Notify)
	{
		UC_Notify->SetNotifyState(Notify);
		UC_Notify->CheckNotify();
	}

	bIsLockContents = CheckContentsUnLock();

	if (bIsLockContents)
	{
		PlayAni("CLOSE");
		if (Text_UnLockLevel)
		{
			int32 Index = (int32)ContentsUnlockKey;
			FName IndexName = FName(*FString::FromInt(Index));
			FContentsUnlockTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetContentsUnlockRow(IndexName);
			if (TableInfo)
			{
				int32 level = TableInfo->Level;
				if (level == 0)
				{
					level = 99;
				}
				Text_UnLockLevel->SetText(FText::AsNumber(level));
			}
		}
	}
	else
	{
		PlayAni("OPEN");
	}
}

bool UUC_LobbyList::CheckContentsUnLock()
{
	if (ContentsUnlockKey == EOVERLORD::TUTORIAL)
		return true;

	if (RGameInstance && IsValid(RGameInstance->ContentsUnLockManager))
	{
// 		if (!RGameInstance->ContentsUnLockManager->IsContentsUnlock(ContentsUnlockKey))
// 		{
// 			return true;
// 		}
	}
	return false;
}

void UUC_LobbyList::OnClickButton()
{
	//if (bIsLockContents)
	//	return;

	if (UIMenuID == EUIMenuIDEnum::VE_Max
		|| UIMenuID == EUIMenuIDEnum::VE_None)
		return;

	//temp
	if (UIMenuID == EUIMenuIDEnum::VE_UltimateBattle
		|| UIMenuID == EUIMenuIDEnum::VE_RankBattle)
	{
		RGameInstance->ContentsUnLockManager->ShowLockNotificationPopup(EOVERLORD::TUTORIAL);
		return;
	}

// 	if (!RGameInstance->ContentsUnLockManager->IsContentsUnlock(ContentsUnlockKey))
// 	{
// 		RGameInstance->ContentsUnLockManager->ShowLockNotificationPopup(ContentsUnlockKey);
// 		return;
// 	}

	UUIFunctionLibrary::GoContents(UIMenuID, 0);
}