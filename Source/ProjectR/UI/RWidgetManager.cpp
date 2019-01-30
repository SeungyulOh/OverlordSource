// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RWidgetManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "Table/WidgetBluePrintTableInfo.h"
#include "UI/RBaseWidget.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UI/Common/UP_CommonPopupSplash.h"
#include "UI/Common/UP_RewardPopupSplash.h"
#include "UI/Common/UP_Block.h"
#include "UI/Common/UP_Popup_Avatar.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/RealTime/UP_Matching_Waiting.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/UP_Popup_Splash_AttackPower.h"
#include "UI/Common/Popup/UP_Popup_Splash_Treasure.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/Guild/UC_GuildModifyPopup.h"
#include "UI/Guild/UC_Guild_LevelUp.h"
#include "UI/Guild/UC_Guild_Grade.h"
#include "UI/Campaign/UP_BattleRepeatStatePopup.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"
#include "UI/DeckManagement/UC_Popup_HeroConfig.h"
#include "UI/ItemManagement/UC_RewardAfterGrindScrollView.h"
#include "UP_Popup_Reward_AfterGrind.h"
#include "UI/ItemManagement/UP_Bag.h"
#include "UI/Component/HorizontalBox_RadioGroup.h"
#include "UI/ItemManagement/UC_GenericPopup.h"
#include "UI/ItemManagement/UC_GenericPopupContent.h"
#include "UI/ItemManagement/UP_GenericSplash.h"
#include "UI_Lobby/UP_Lobby_Outdoor.h"
#include "UI/Result/UP_Result_ForRTS.h"
#include "GameState/TowerLobbyState.h"

const EUIPageEnum ROOTPAGE = EUIPageEnum::UIPage_LobbyIndoor;

//////////////////////////////////////////////////////////////////////////
// Widget Manager
//////////////////////////////////////////////////////////////////////////
void URWidgetManager::Initialize(void)
{
	EWidgetBluePrintEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
	RemoveAllUserWidget();
	
}

FString URWidgetManager::GetWBPEnumAsString(EWidgetBluePrintEnum eWBP)
{
	if (EWidgetBluePrintEnumPtr)
	{
		return EWidgetBluePrintEnumPtr->GetNameStringByIndex((int32)eWBP);
	}

	return "";
}

FName URWidgetManager::GetWBPEnumAsName(EWidgetBluePrintEnum eWBP)
{
	return FName(*GetWBPEnumAsString(eWBP));
}

UUserWidget* URWidgetManager::CreateUserWidget(EWidgetBluePrintEnum eWBP)
{
#if LOG_TIME
 	double StartTime = FPlatformTime::Seconds();
#endif

	if (RGAMEINSTANCE(this) == nullptr)
		return nullptr;
	
	UUserWidget* userWidget = nullptr;

	FString StringWBP = GetWBPEnumAsString(eWBP);
	if (StringWBP.IsEmpty())
		return nullptr;
	FWidgetBluePrintTableInfo* Result = UTableManager::GetInstancePtr()->GetWidgetBluePrintRow(FName(*StringWBP));
	if (Result && Result->GetWidgetClass())
	{	
		//UE_LOG(LogUI , Log, TEXT("%s"), *Result->GetWidgetClass()->GetName());
		userWidget = CreateWidget<UUserWidget>(RGAMEINSTANCE(this), Result->GetWidgetClass());
		if (IsValid(userWidget))
		{
			UserWidgetMap.Emplace(eWBP, userWidget);
		}
	}

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
	int32 SceneIndex = (int32)(eWBP);
	UE_LOG(LogClass, Log, TEXT("%f ____createwidget(%s)"), PrintTime, *(EnumPtr->GetNameStringByIndex(SceneIndex)));
#endif

	return userWidget;
}

UUserWidget* URWidgetManager::GetUserWidget(EWidgetBluePrintEnum eWBP)
{
	if (UserWidgetMap.Contains(eWBP))
		return *(UserWidgetMap.Find(eWBP));

	return nullptr;
}


void URWidgetManager::ShowUserWidget(EWidgetBluePrintEnum eWBP, bool bForceOrder, int32 order)
{
	UUserWidget* userWidget = GetUserWidget(eWBP);
	if (IsValid(userWidget))
	{
		if (!userWidget->IsInViewport())
		{
			if (bForceOrder)
			{
				AddToViewport(eWBP, userWidget, order);
			}
			else
			{
				++ViewPortOrder;
				AddToViewport(eWBP, userWidget, ViewPortOrder);
			}
		}
	}
}

void URWidgetManager::HideUserWidget(EWidgetBluePrintEnum eWBP)
{
	UUserWidget* userWidget = GetUserWidget(eWBP);
	if (IsValid(userWidget))
	{
		if (userWidget->IsInViewport())
		{
			UserWidgetMap.Remove(eWBP);
			userWidget->RemoveFromViewport();
			userWidget = nullptr;
		}
	}
}



bool URWidgetManager::IsShowUserWidget(EWidgetBluePrintEnum eWBP)
{
	UUserWidget* userWidget = GetUserWidget(eWBP);
	if (IsValid(userWidget))
	{
		return userWidget->IsInViewport();
	}
	return false;
}



void URWidgetManager::RemoveAllUserWidget()
{
	
	for (auto Itor = UserWidgetMap.CreateIterator(); Itor; ++Itor)
	{
		UUserWidget* userWidget = (*Itor).Value;
		if (IsValid(userWidget))
		{
			if (userWidget->IsInViewport())
			{
				userWidget->RemoveFromViewport();
				userWidget = nullptr;
			}
		}
	}
	UserWidgetMap.Reset();
	UIPageStack.Empty();
}

void URWidgetManager::SetUIVisibility(bool bVisible)
{
	for (auto& Element : UserWidgetMap)
	{
		if (Element.Value->IsValidLowLevel())
			Element.Value->SetVisibility(bVisible == true ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void URWidgetManager::ShowUserWidgetAutoCreate(EWidgetBluePrintEnum eWBP, bool bForceOrder, int32 order)
{
	UUserWidget* userWidget = GetUserWidget(eWBP);
	
	if (userWidget == nullptr)
	{
		userWidget = CreateUserWidget(eWBP);
	}

	ShowUserWidget(eWBP , bForceOrder , order);
}

UUserWidget* URWidgetManager::GetUserWidgetAutoCreate(EWidgetBluePrintEnum eWBP)
{
	UUserWidget* userWidget = GetUserWidget(eWBP);
	if (userWidget == nullptr)
	{
		userWidget = CreateUserWidget(eWBP);
	}

	return userWidget;
}

void URWidgetManager::AddToViewport(EWidgetBluePrintEnum InWidgetEnum, UUserWidget* InWidget, int32 ZOrder)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	bool bAddViewPort = true;
	if (InWidget->IsInViewport())
		bAddViewPort = false;

	if (IsValid(InWidget))
	{
		if (bAddViewPort)
		{
			InWidget->AddToViewport(ZOrder);
			InWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

/*
Popups
*/

void URWidgetManager::ChangeInputModeUIOnly(bool bRes)
{
	bInputModeUI = bRes;

	if (RGAMEINSTANCE(this) != nullptr)
	{
		APlayerController* PlayerController = UUtilFunctionLibrary::GetPlayerController(RGAMEINSTANCE(this)->GetWorld());
		if (IsValid(PlayerController))
		{
			if (bRes)
			{
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController);
			}
			else
			{
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController);
			}
		}
	}
}

EUIPageEnum URWidgetManager::GetCurrentUIPage()
{
	int32 Maxidx = UIPageStack.Num() - 1;
	if (UIPageStack.IsValidIndex(Maxidx))
		return UIPageStack[Maxidx];

	return EUIPageEnum::UIPage_None;
}

EUIPageEnum URWidgetManager::GetPrevUIPage()
{
	int32 Maxidx = UIPageStack.Num() - 1;
	if (UIPageStack.IsValidIndex(Maxidx - 1))
		return UIPageStack[Maxidx - 1];

	return EUIPageEnum::UIPage_None;
}



void URWidgetManager::ChangeUIPage(EUIPageEnum Targetpage)
{
	/*
		UIPageStack -> we can get currentUIpage and PrevUIPage and history.
		This is only meaningful in TowerLobbyState for now.

		RootWidget should be UP_Lobby_Indoor.
	*/

	EUIPageEnum CurrentPage = GetCurrentUIPage();
	EUIPageEnum PrevPage = GetPrevUIPage();
	if (Targetpage == CurrentPage)
		return;

	/*
		Element should be added whenever UUtilFunctionLibrary::GetLocalizedString function is called.
		Empty this maps when changing another UIPage.
	*/
	LocalizingKeyMaps.Empty();

	/*
	this stack should be empty only if trying to change UI into RootPage which is currently UP_Lobby_Indoor
	*/
	if (Targetpage == ROOTPAGE)
		UIPageStack.Empty();
	else if (Targetpage == PrevPage)
		UIPageStack.Pop();

	UIPageStack.AddUnique(Targetpage);

	/*Only RemoveFromViewport for every widgets in UserWidgetmap*/
	/*We dont want to deallocate the pointer of them*/
	/*We Want to reuse them*/
	//Comments that do not match the code
	for (auto Itor = UserWidgetMap.CreateIterator(); Itor; ++Itor)
	{
		UUserWidget* userWidget = (*Itor).Value;
		if (IsValid(userWidget))
		{
			if (userWidget->IsInViewport())
			{
				userWidget->RemoveFromViewport();
				userWidget = nullptr;
			}
		}
	}
	UserWidgetMap.Reset();

	/*ChangeUIPage means that we need to do initial settings*/
	ViewPortOrder = 0;

#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif

#if ENABLE_FORCE_GC
	/*Start GC*/
	GEngine->ForceGarbageCollection(true);
	/*GC End*/
#endif

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FTimerDelegate DelayedCall;
	DelayedCall.BindLambda([RGameInstance, Targetpage]
	{
		RGameInstance->RWidgetManager->ShowUIPage(Targetpage);
	});

	RGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(DelayedCall);

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUIPageEnum"), true);
	int32 SceneIndex = (int32)(Targetpage);
	UE_LOG(LogGamePlay, Log, TEXT("%f ____uipage(%s)"), PrintTime, *(EnumPtr->GetNameStringByIndex(SceneIndex)));
#endif
}






void URWidgetManager::AttachUIPage(EUIPageEnum page)
{
	ShowUIPage(page);
}

void URWidgetManager::DetachUIPage(EUIPageEnum page)
{
	HideUIPage(page);
}

void URWidgetManager::ShowUIPage(EUIPageEnum page)
{
	//QUICK_SCOPE_CYCLE_COUNTER(URWidgetManager__ShowUIPage);
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	GameInstance->LoadingSceneManager->HideLoadingScene();

	switch (page)
	{
	case EUIPageEnum::UIPage_Campaign:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Campaign);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_CampaignResult:
	{
		//ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_CampaignReward);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ResultPVE);
	}break;
	case EUIPageEnum::UIPage_EpisodeDetail:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_EpisodeDetail);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_WeekDungeon:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_WeekDungeon);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_WeekStageSelect:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_WeekStageSelect);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_HeroRune_EpisodeDetail:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_HeroRune_EpisodeDetail);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_HeroRune_SelectHero:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_HeroRune_SelectHero);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_Login:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LoginMain);
	}break;
	case EUIPageEnum::UIPage_LobbyOutdoor:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LobbyOutdoor);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_LobbyOutdoorPortraitMode:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LobbyOutdoorPortraitMode);
	}break;
	case EUIPageEnum::UIPage_LobbyIndoor:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LobbyIndoor);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);		
	}break;
	case EUIPageEnum::UIPage_ArenaLobby:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ArenaLobby);
	 	ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_ArenaDeckSetting:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GenericScrollview, true, HEROSCROLLVIEW_ZORDER);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ArenaDeckSetting);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_HeroManagement_Renewal: 
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GenericScrollview, true, HEROSCROLLVIEW_ZORDER);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal);
	}break;
	case EUIPageEnum::UIPage_HeroGrind:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_HeroGrind);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_RuneInfo:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RuneInfo);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_RuneMix:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RuneMix);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_HeroReinforce:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_HeroReinforce , true , HEROSCROLLVIEW_ZORDER - 1 );
	}
	break;
	case EUIPageEnum::UIPage_ItemManufacture:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ItemManufacture);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_DeckSetting:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GenericScrollview, true, HEROSCROLLVIEW_ZORDER);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_DeckManager);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
// 	case EUIPageEnum::UIPage_DeckSetting_OneControl:
// 	{
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_DeckManager_OneControl, false);
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
// 	}
// 	break;
// 	case EUIPageEnum::UIPage_DeckSetting_ThreeControl:
// 	{
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_DeckManager_ThreeControl, false);
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
// 	}
// 	break;
// 	case EUIPageEnum::UIPage_DeckSetting_Raid_OneControl:
// 	{	
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_DeckManager_Raid_OneControl, false);
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
// 	}
// 	break;
// 	case EUIPageEnum::UIPage_DeckSetting_Raid_ThreeControl:
// 	{
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_DeckManager_Raid_ThreeControl, false);
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
// 	}
// 	break;
	case EUIPageEnum::UIPage_Setting:
	{	
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Setting , true , HEROSCROLLVIEW_ZORDER + 1);
	}
	break;
// 	case EUIPageEnum::UIPage_InGameHUD_CampaignNew:
// 	{
// 		ChangeInputModeUIOnly(false);
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_InGameHUD_CampaignNew);
// 	}
//	break;
	case EUIPageEnum::UIPage_InGameHUD_One:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Campaign_One);
	}
	break;
	case EUIPageEnum::UIPage_RaidHUD_Three:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Raid_Three);
	}
	break;
	case EUIPageEnum::UIPage_RaidHUD_One:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Raid_One);
	}
	break;
	case EUIPageEnum::UIPage_InGameHUD_Three:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Campaign_Three);
	}
	break;
	case EUIPageEnum::UIPage_InGameHUD_Arena:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_InGameHUD_Arena);
	}
	break;
	case EUIPageEnum::UIPage_InGameHUD_Mirror:
	{
		ChangeInputModeUIOnly(false);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_InGameHUD_Mirror);
	}
	break;
	case EUIPageEnum::UIPage_ArenaResult:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ArenaResult);
		//ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ResultPVP);
	}break;
	case EUIPageEnum::UIPage_CaptureHero:
	{		
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_CaptureHero);
	}break;
	case EUIPageEnum::UIPage_MailBox:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_MailBox);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;	
	case EUIPageEnum::UIPage_ColosseumLobby:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ColosseumLobby);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_ColosseumSetting:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GenericScrollview , true , HEROSCROLLVIEW_ZORDER);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ColosseumSetting);
	}
	break;
	case EUIPageEnum::UIPage_ColosseumResult:
	{
		//ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ColosseumResult);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ResultPVP);
	}break;
	case EUIPageEnum::UIPage_CrusaderLobby:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_CrusaderLobby);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_CrusaderResult:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ResultPVE);
	}break;
	case EUIPageEnum::UIPage_Raid_List:
	{		
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Raid_List);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_Raid_Select:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Raid_Select);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_Daily_Quest:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Daily_Quest);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_StageInfo:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_StageInfo);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_Shop:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Shop);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_ShopHero:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ShopHero);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_TimeTreasure:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TimeTreasure);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_SweepResult:
	{	
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_SweepResult);

	}break;
	case EUIPageEnum::UIPage_BattleFieldLobby:
	{		
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_BattleFieldLobby);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_BattleFieldResult:
	{	
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_BattleFieldResult);
	}break;
	case EUIPageEnum::UIPage_BattleFieldHeroList:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_BattleFieldHeroList);
	}break;
	case EUIPageEnum::UIPage_ContentsUnlockPopup:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_UnlockPopup);
	}break;
	case EUIPageEnum::UIPage_ContentsLockNotify:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LockNotify);
	}break;		
	case EUIPageEnum::UIPage_GuildLobby:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GuildLobby);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_GuildRaid:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GuildRaid);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_GuildDonation:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GuildDonation);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_GuildVisit:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GuildVisit);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_EnchantResult:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Enchant_Result);		
	}break;
	case EUIPageEnum::UIPage_ResultReward:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ResultReward);
	}break;
// 	case EUIPageEnum::UIPage_QuestGuide:
// 	{
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Quest, true);
// 		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
// 	}break;
	case EUIPageEnum::UIPage_BattleGroundLobby:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_BattleGroundLobby);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_BattleGroundHeroSelect:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_BattleGroundHeroSelect);
	}break;
	case EUIPageEnum::UIPage_BattleGroundGuide:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_BattleGroundGuide);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_RealTimeResult:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RealTimeResult);
	}break;
	case EUIPageEnum::UIPage_RealTimeGroup:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RealTimeGroup);
	}break;
	case EUIPageEnum::UIPage_Avatar:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Avatar);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_HeroInventory:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_HeroInventory);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_LobbyOutside:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LobbyOutdoor);
	}break;
	case EUIPageEnum::UIPage_Bag:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Bag);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_ItemEnchant:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Page_ItemEnchant, true, HEROSCROLLVIEW_ZORDER);
	}break;
	case EUIPageEnum::UIPage_ItemLevelUp:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Page_ItemLevelUp, true, HEROSCROLLVIEW_ZORDER);
	}break;
	case EUIPageEnum::UIPage_ItemUpgrade:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Page_ItemUpgrade , true ,HEROSCROLLVIEW_ZORDER);
	}break;
	case EUIPageEnum::UIPage_ItemOptionReset:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Page_ItemOptionReset);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_ItemInfo:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ItemInfo , true, HEROSCROLLVIEW_ZORDER);
	}break;
	case EUIPageEnum::UIPage_RuneCombine:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Page_RuneCombine);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_RuneManagement:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Page_RuneManagement);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_MirrorDungeon:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_MirrorDungeon);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_MirrorDeckSetting:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GenericScrollview, true, HEROSCROLLVIEW_ZORDER);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_MirrorDeckSetting);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_MirrorResult:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_MirrorResult);
		//ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_FloorShop:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_FloorShop);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_RaidWorldBoss:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RaildWorldBoss);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_TowerBossLobby:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TowerBossLobby);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_RaidLobby:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RaidLobby);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_MissionSystem:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_MissionSystem);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_PartyRaidUI:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GenericScrollview, true, HEROSCROLLVIEW_ZORDER);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_PartyRaidUI);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_HeroRatingUp:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_HeroRatingUp , true , HEROSCROLLVIEW_ZORDER - 1);
		//ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_SkillManagement:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_SkillManagement);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_ResultPageForRTS:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Colosseum_Result);
		break;
	}
	case EUIPageEnum::UIPage_ResultRaid:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Raid_Result);
		break;
	}
	case EUIPageEnum::UIPage_GrindPopup				:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_GrindPopup);
		break;
	}
	case EUIPageEnum::UIPage_LevelUpCommonPopup		:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LevelUpCommonPopup);
		break;
	}
	case EUIPageEnum::UIPage_LevelUpCommonSplash	:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_LevelUpCommonSplash);
		break;
	}
	case EUIPageEnum::UIPage_QuestCompletePopup:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_QuestCompletePopup);
		break;
	}
	case EUIPageEnum::UIPage_QuestPassingAlertPopup:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_QuestPassingAlertPopup);
		break;
	}
	case EUIPageEnum::UIPage_DeckTest:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_DeckTest);
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_SpiritDetail:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_SpiritDetail , true , HEROSCROLLVIEW_ZORDER + 1);
	}break;
	case EUIPageEnum::UIPage_MaterialDetail:
	{
		ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_WalletItemInfo, true, HEROSCROLLVIEW_ZORDER + 1);
	}break;
	default:
	{
		FText enumString = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EUIPageEnum"), (int32)page);
		UE_LOG(LogUI, Warning, TEXT("Invalid EUIPageEnum (%s)"), *enumString.ToString());
	}
	break;

	}

	//temp in here
	if (IsValid(GameInstance->ForceTutorialManager))
		GameInstance->ForceTutorialManager->CheckTutoProgress(page);
}

void URWidgetManager::HideUIPage(EUIPageEnum page)
{
	switch (page)
	{
	case EUIPageEnum::UIPage_Campaign:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_CampaignList);
	}break;
	case EUIPageEnum::UIPage_EpisodeDetail:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_EpisodeDetail);
	}break;
	case EUIPageEnum::UIPage_WeekDungeon:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_WeekDungeon);
	}break;
	case EUIPageEnum::UIPage_HeroRune_EpisodeDetail:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_HeroRune_EpisodeDetail);
	}break;
	case EUIPageEnum::UIPage_HeroRune_SelectHero:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_HeroRune_SelectHero);
	}break;

	case EUIPageEnum::UIPage_CampaignResult:
	{
		//HideUserWidget(EWidgetBluePrintEnum::WBP_CampaignReward);
		HideUserWidget(EWidgetBluePrintEnum::WBP_ResultPVE);
	}break;
	case EUIPageEnum::UIPage_Login:
	{	
		HideUserWidget(EWidgetBluePrintEnum::WBP_LoginMain);
	}break;
	case EUIPageEnum::UIPage_LobbyOutdoor:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_LobbyOutdoor);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_LobbyOutdoorPortraitMode:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_LobbyOutdoorPortraitMode);
	}break;
	case EUIPageEnum::UIPage_LobbyIndoor:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_LobbyIndoor);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_ArenaLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_ArenaLobby);
	}break;
	case EUIPageEnum::UIPage_HeroManagement_Renewal:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Heromanagement_Renewal);
	}break;
	case EUIPageEnum::UIPage_HeroGrind:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_HeroGrind);
	}break;
	case EUIPageEnum::UIPage_RuneInfo:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_RuneInfo);
	}break;
	case EUIPageEnum::UIPage_RuneMix:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_RuneMix);
	}break;
	case EUIPageEnum::UIPage_DeckSetting:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_DeckManager);
	}break;
// 	case EUIPageEnum::UIPage_DeckSetting_OneControl:
// 	{
// 		HideUserWidget(EWidgetBluePrintEnum::WBP_DeckManager_OneControl);
// 	}break;
// 	case EUIPageEnum::UIPage_DeckSetting_ThreeControl:
// 	{
// 		HideUserWidget(EWidgetBluePrintEnum::WBP_DeckManager_ThreeControl);
// 	}break;
// 	case EUIPageEnum::UIPage_DeckSetting_Raid_OneControl:
// 	{
// 		HideUserWidget(EWidgetBluePrintEnum::WBP_DeckManager_Raid_OneControl);
// 	}
// 	break;
// 	case EUIPageEnum::UIPage_DeckSetting_Raid_ThreeControl:
// 	{
// 		HideUserWidget(EWidgetBluePrintEnum::WBP_DeckManager_Raid_ThreeControl);
// 	}
	case EUIPageEnum::UIPage_Setting:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Setting);
	}break;
	case EUIPageEnum::UIPage_ArenaResult:
	{
		//HideUserWidget(EWidgetBluePrintEnum::WBP_ArenaResult);
		HideUserWidget(EWidgetBluePrintEnum::WBP_ResultPVP);
	}break;
	case EUIPageEnum::UIPage_CaptureHero:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_CaptureHero);
	}break;
	case EUIPageEnum::UIPage_MailBox:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_MailBox);
	}break;	
	case EUIPageEnum::UIPage_ColosseumLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_ColosseumLobby);
	}break;
	case EUIPageEnum::UIPage_ColosseumSetting:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_ColosseumSetting);
	}
	break;
	case EUIPageEnum::UIPage_ColosseumResult:
	{
		//HideUserWidget(EWidgetBluePrintEnum::WBP_ColosseumResult);
		HideUserWidget(EWidgetBluePrintEnum::WBP_ResultPVP);
	}break;
	case EUIPageEnum::UIPage_CrusaderLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_CrusaderLobby);
	}break;
	case EUIPageEnum::UIPage_Raid_List:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Raid_List);
	}break;
	case EUIPageEnum::UIPage_Raid_Select:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Raid_Select);
	}break;
	case EUIPageEnum::UIPage_Daily_Quest:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Daily_Quest);
	}break;
	case EUIPageEnum::UIPage_StageInfo:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_StageInfo);
	}break;
	case EUIPageEnum::UIPage_Shop:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Shop);
	}break;
	case EUIPageEnum::UIPage_ShopHero:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_ShopHero);
	}break;	
	case EUIPageEnum::UIPage_SweepResult:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_SweepResult);
	}break;
	case EUIPageEnum::UIPage_BattleFieldLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_BattleFieldLobby);
	}break;
	case EUIPageEnum::UIPage_BattleFieldResult:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_BattleFieldResult);
	}break;
	case EUIPageEnum::UIPage_BattleFieldHeroList:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_BattleFieldHeroList);
	}break;
	case EUIPageEnum::UIPage_ContentsUnlockPopup:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_UnlockPopup);
	}break;
	case EUIPageEnum::UIPage_ContentsLockNotify:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_LockNotify);
	}break;	
	case EUIPageEnum::UIPage_GuildLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_GuildLobby);
	}break;
	case EUIPageEnum::UIPage_GuildRaid:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_GuildRaid);
	}break;
	case EUIPageEnum::UIPage_GuildDonation:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_GuildDonation);
	}break;
	case EUIPageEnum::UIPage_GuildVisit:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_GuildVisit);
	}break;
	case EUIPageEnum::UIPage_EnchantResult:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Enchant_Result);
	}break;
	case EUIPageEnum::UIPage_ResultReward:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_ResultReward);
	}break;
// 	case EUIPageEnum::UIPage_QuestGuide:
// 	{
// 		HideUserWidget(EWidgetBluePrintEnum::WBP_Quest);
// 	}break;
	case EUIPageEnum::UIPage_BattleGroundLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_BattleGroundLobby);
	}break;
	case EUIPageEnum::UIPage_BattleGroundHeroSelect:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_BattleGroundHeroSelect);
	}break;
	case EUIPageEnum::UIPage_RealTimeResult:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_RealTimeResult);
	}break;
	case EUIPageEnum::UIPage_RealTimeGroup:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_RealTimeGroup);
	}break;
	case EUIPageEnum::UIPage_Avatar:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Avatar);
	}break;
	case EUIPageEnum::UIPage_Bag:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Bag);
	}break;
	case EUIPageEnum::UIPage_ItemEnchant:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemEnchant);
	}break;
	case EUIPageEnum::UIPage_ItemLevelUp:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemLevelUp);
	}break;
	case EUIPageEnum::UIPage_ItemUpgrade:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemUpgrade);
	}break;
	case EUIPageEnum::UIPage_ItemOptionReset:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemOptionReset);
	}break;
	case EUIPageEnum::UIPage_RuneCombine:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Page_RuneCombine);
	}break;
	case EUIPageEnum::UIPage_RuneManagement:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Page_RuneManagement);
	}break;
	case EUIPageEnum::UIPage_MirrorDungeon:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_MirrorDungeon);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_MirrorDeckSetting:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_MirrorDeckSetting);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_ArenaDeckSetting:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_ArenaDeckSetting);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}break;
	case EUIPageEnum::UIPage_MirrorResult:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_MirrorResult);
	}
	break;
	case EUIPageEnum::UIPage_FloorShop:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_FloorShop);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_RaidWorldBoss:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_RaildWorldBoss);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_TowerBossLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_TowerBossLobby);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_RaidLobby:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_RaidLobby);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_MissionSystem:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_MissionSystem);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);

	}
	break;
	case EUIPageEnum::UIPage_PartyRaidUI:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_PartyRaidUI);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_HeroRatingUp:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_HeroRatingUp);
		//HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	case EUIPageEnum::UIPage_ItemInfo:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_ItemInfo);
		//HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break;
	case EUIPageEnum::UIPage_HeroReinforce:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_HeroReinforce);
	}
	break;
	case EUIPageEnum::UIPage_SkillManagement:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_SkillManagement);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	}
	break; 
	case EUIPageEnum::UIPage_InGameHUD_One:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Campaign_One);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
		break;
	}
	case EUIPageEnum::UIPage_InGameHUD_Three:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Campaign_Three);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
		break;
	}
	case EUIPageEnum::UIPage_RaidHUD_Three:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Raid_Three);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
		break;
	}
	case EUIPageEnum::UIPage_RaidHUD_One:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Raid_One);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
		break;
	}
	case EUIPageEnum::UIPage_InGameHUD_Arena:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_InGameHUD_Arena);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
		break;
	}
	case EUIPageEnum::UIPage_InGameHUD_Mirror:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_InGameHUD_Mirror);
		HideUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
		break;
	}
	case EUIPageEnum::UIPage_ResultPageForRTS:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Colosseum_Result);
		break;
	}
	case EUIPageEnum::UIPage_ResultRaid:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Raid_Result);
		break;
	}
	case EUIPageEnum::UIPage_GrindPopup:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_GrindPopup);
		break;
	}
	case EUIPageEnum::UIPage_LevelUpCommonPopup:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_LevelUpCommonPopup);
		break;
	}
	case EUIPageEnum::UIPage_LevelUpCommonSplash:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_LevelUpCommonSplash);
		break;
	}	
	case EUIPageEnum::UIPage_QuestCompletePopup:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_QuestCompletePopup);
		break;
	}
	case EUIPageEnum::UIPage_QuestPassingAlertPopup:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_QuestPassingAlertPopup);
		break;
	}
	case EUIPageEnum::UIPage_DeckTest:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_DeckTest);
	}break;
	case EUIPageEnum::UIPage_SpiritDetail:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_SpiritDetail);
	}break;
	case EUIPageEnum::UIPage_MaterialDetail:
	{
		HideUserWidget(EWidgetBluePrintEnum::WBP_Popup_WalletItemInfo);
	}break;

	default:
	{
		FText enumString = UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EUIPageEnum"), (int32)page);
		UE_LOG(LogUI, Warning, TEXT("Invalid EUIPageEnum (%s)"), *enumString.ToString());
	}
	break;
	}

}

void URWidgetManager::RefreshLocalizingText(UUserWidget* InUswerWidget)
{
	auto VisiblueUI = InUswerWidget;

	if (IsValid(VisiblueUI))
	{
		VisiblueUI->SynchronizeProperties();
		TArray<UWidget*> Widgets;
		VisiblueUI->WidgetTree->GetAllWidgets(Widgets);

		for (auto& WidgetElem : Widgets)
		{
			UUserWidget* userWidget = Cast<UUserWidget>(WidgetElem);
			if (IsValid(userWidget))
			{
				RefreshLocalizingText(userWidget);
			}
			 
			ULocalizingTextBlock* LocalizingTextBlock = Cast<ULocalizingTextBlock>(WidgetElem);
			if (IsValid(LocalizingTextBlock))
			{
				LocalizingTextBlock->ApplyLocalizedText();
			}
		}
	}
}

void URWidgetManager::RefreshVisibleUI()
{
	for (auto& Element : UserWidgetMap)
	{
		if (Element.Value->IsValidLowLevel())
		{
			URBaseWidget* VisibleBaseWidget = Cast<URBaseWidget>(Element.Value);
			if (IsValid(VisibleBaseWidget))
			{
				VisibleBaseWidget->InvalidateData();
			}
			RefreshLocalizingText(Element.Value);
		}
	}
}

void URWidgetManager::RefreshLocalizingTextForVisibleUI()
{
	for (auto& Element : UserWidgetMap)
	{
		if (Element.Value->IsValidLowLevel())
		{
			RefreshLocalizingText(Element.Value);
		}
	}
}


void URWidgetManager::AddLocalizingKey(FName key, EStringTableType type)
{
	EStringTableType* Foundtype = LocalizingKeyMaps.Find(key);
	if (!Foundtype)
	{
		LocalizingKeyMaps.Emplace(key, type);
	}
}

bool URWidgetManager::GetLocalizingKeyAndType(FText Intext, FName& key, EStringTableType& type)
{
	if (LocalizingKeyMaps.Num() == 0) //editor error
		return false;

	for (auto i : LocalizingKeyMaps)
	{
		if (Intext.ToString() == UUtilFunctionLibrary::GetLocalizedString(i.Value, i.Key).ToString())
		{
			key = i.Key;
			type = i.Value;

			return true;
		}
	}
	return false;
}

void URWidgetManager::AddUIEvent(EUIEventEnum InEventType)
{
	FUIEventInfo eventInfo;
	eventInfo.EventType = InEventType;

	UIEventQueue.Emplace(eventInfo);
}

void URWidgetManager::AddUIEventInt(EUIEventEnum InEventType, int32 InValue)
{
	FUIEventInfo eventInfo;
	eventInfo.EventType = InEventType;
	eventInfo.IValue = InValue;

	UIEventQueue.Emplace(eventInfo);
}

void URWidgetManager::AddUIEventString(EUIEventEnum InEventType, FString InValue, int32 InIValue /*= 1*/)
{
	FUIEventInfo eventInfo;
	eventInfo.EventType = InEventType;
	eventInfo.StringValue = InValue;
	eventInfo.IValue = InIValue;

	UIEventQueue.Emplace(eventInfo);
}

void URWidgetManager::AddUIEventName(EUIEventEnum InEventType, FName InValue)
{
	FUIEventInfo eventInfo;
	eventInfo.EventType = InEventType;
	eventInfo.NameValue = InValue;

	UIEventQueue.Emplace(eventInfo);
}

void URWidgetManager::RemoveUIEvent(EUIEventEnum InEventType)
{
	int32 EventIndex = 0;
	bool bExe = false;
	for (auto& Element : UIEventQueue)
	{
		if (Element.EventType == InEventType)
		{
			bExe = true;
			break;
		}

		++EventIndex;
	}

	if (bExe)
	{
		UIEventQueue.RemoveAt(EventIndex);
	}
}

void URWidgetManager::RemoveAllUIEvent()
{
	UIEventQueue.Empty();
}

bool URWidgetManager::IsInUIEvent(EUIEventEnum InEventType)
{
	bool bResult = false;
	for (auto& Element : UIEventQueue)
	{
		if (Element.EventType == InEventType)
		{
			bResult = true;
			break;
		}
	}
	return bResult;
}

FUIEventInfo* URWidgetManager::GetUIEventInfo(EUIEventEnum InEventType)
{
	for (FUIEventInfo& Element : UIEventQueue)
	{
		if (Element.EventType == InEventType)
		{
			return &Element;
		}
	}
	return nullptr;
}

void URWidgetManager::ExceptionRTSError()
{
	UUP_CommonPopup* popup = GetUserWidgetT<UUP_CommonPopup>(EWidgetBluePrintEnum::WBP_CommonPopup);
	if (popup)
	{
		popup->OnClickOK.AddDynamic(this, &URWidgetManager::RTSError);
	}
}

void URWidgetManager::RTSError()
{
	auto RGameInstance = RGAMEINSTANCE(this);

	EGamePlayModeTypeEnum Current = RGameInstance->CurrentGameMode;
	EUIMenuIDEnum Target;
	switch (Current)
	{
		case EGamePlayModeTypeEnum::VE_SingleRaid:	Target = EUIMenuIDEnum::VE_SingleRaid;	break;
		case EGamePlayModeTypeEnum::VE_MultiRaid:	Target = EUIMenuIDEnum::VE_MultiRaid;	break;
		default:
		{
			Target = EUIMenuIDEnum::VE_Colosseum;
		}
		break;
	}

	AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Colosseum);
	RGameInstance->EventManager->ClearEvent();
	UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
	if (TowerState)
		TowerState->SetIndoorStart(true);

	RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
}
