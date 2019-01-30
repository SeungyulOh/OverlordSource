// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TopBar.h"
#include "Public/Components/TextBlock.h"
#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Shop/UP_HeroShop.h"
#include "UI/Common/State/BasePropertyState.h"
#include "UI/Common/State/PropertyState.h"
#include "UI/Common/UC_TopBar_Property.h"
#include "UI/Common/UC_TopBar_Info.h"
#include "UI/HeroManagement/UC_SkillManagement.h"
#include "Table/TopbarTableInfo.h"
#include "NotifyList/NotifyEventList.h"
#include "UI/Shop/UP_Shop.h"
#include "Lobby/TowerLobbyLevelScriptActor.h"
#include "CustomActors/RHeroGacha.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Network/RTSManager.h"
#include "Network/Chat/ChatManager.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "UI/HeroInfo/HeroUpgrade/UP_HeroInfo_Upgrade.h"
#include "UI/Colosseum/UC_Colosseum_Class.h"
#include "GameState/TowerLobbyState.h"

UTopBar::FOnClickPrevButton UTopBar::OnClickPrevButton;
void UTopBar::NativeConstruct()
{ 
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UTopBar::RefreshDataUI);
		RGameInstance->EventManager->OnUpdateUserInfo.AddDynamic(this, &UTopBar::RefreshDataUI);
	}

	Super::NativeConstruct();
	UC_TopBar_Info->NativeConstruct();
	FindChildWidgets<UUC_TopBar_Property>(this, TopBar_Propertys);

	if (IsValid(Button_Search))
	{
		bOpenTopBarInfo = false;
		Button_Search->OnClicked.RemoveDynamic(this, &UTopBar::OnClick_Search);
		Button_Search->OnClicked.AddDynamic(this, &UTopBar::OnClick_Search);
	}

	if (IsValid(Button_Mail))
	{
		Button_Mail->OnClicked.RemoveDynamic(this, &UTopBar::OnClick_Mail);
		Button_Mail->OnClicked.AddDynamic(this, &UTopBar::OnClick_Mail);
	}

	if (IsValid(Button_Alarm))
	{
		Button_Alarm->OnClicked.RemoveDynamic(this, &UTopBar::OnClick_Setting);
		Button_Alarm->OnClicked.AddDynamic(this, &UTopBar::OnClick_Setting);
	}

	if (IsValid(Btn_Top_Prev))
	{
		Btn_Top_Prev->OnClicked.RemoveDynamic(this, &UTopBar::PrevButton);
		Btn_Top_Prev->OnClicked.AddDynamic(this, &UTopBar::PrevButton);
	}
	if (IsValid(Button_Chat))
	{
		Button_Chat->OnClicked.RemoveDynamic(this, &UTopBar::OnClick_Chat);
		Button_Chat->OnClicked.AddDynamic(this, &UTopBar::OnClick_Chat);
	}
	if (IsValid(Button_Avatar))
	{
		Button_Avatar->OnClicked.RemoveDynamic(this, &UTopBar::OnClick_Avatar);
		Button_Avatar->OnClicked.AddDynamic(this, &UTopBar::OnClick_Avatar);
	}

	if (IsValid(Button_OpenTotalMenu))
	{
		bOpenMenuFlag = false;
		Button_OpenTotalMenu->OnClicked.RemoveDynamic(this, &UTopBar::OnClick_OpenTotalMenu);
		Button_OpenTotalMenu->OnClicked.AddDynamic(this, &UTopBar::OnClick_OpenTotalMenu);
	}

	if (IsValid(Button_CloseTotalMenu))
	{
		Button_CloseTotalMenu->OnClicked.RemoveDynamic(this, &UTopBar::OnClick_OpenTotalMenu);
		Button_CloseTotalMenu->OnClicked.AddDynamic(this, &UTopBar::OnClick_OpenTotalMenu);
	}
		
	UC_Colosseum_Tier->SetColosseumClassIndex(RGameInstance->ColosseumManager->ColosseumStatus.gradeId);
	UIPageEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUIPageEnum"), true);

	RefreshDataUI();
	RefreshState();
}

void UTopBar::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance && IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UTopBar::RefreshDataUI);
	}

	Super::NativeDestruct();
}

void UTopBar::PrevButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	/*if Current State is not Lobby State , just change it to LobbyState */
	if (RGameInstance->GetCurrentStateType() != EGameStateEnum::VE_Lobby)
	{
		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
		return;
	}

	/*if you want to do specific tasks, do bind this delegate */
	OnClickPrevButton.ExecuteIfBound();

	EUIPageEnum PrevUIPage = RGameInstance->RWidgetManager->GetPrevUIPage();
	if (PrevUIPage == EUIPageEnum::UIPage_None)
		PrevUIPage = EUIPageEnum::UIPage_LobbyIndoor;

	switch (PrevUIPage)
	{
	case EUIPageEnum::UIPage_LobbyIndoor:
	case EUIPageEnum::UIPage_LobbyOutdoor:
	{
		/*
		Why GoUIPage ?
		these two pages needs to play LevelSequence before rendering UIPages
		*/
		UUIFunctionLibrary::GoUIPage(PrevUIPage);
	}break;

	/*Please don't add any other UI pages Here*/

	default:
		RGameInstance->RWidgetManager->ChangeUIPage(PrevUIPage);
		break;
	}
}

void UTopBar::OnClick_Search()
{
	auto RGameInstance = RGAMEINSTANCE(this);

	
	{
		if (RGameInstance->IsAutoPlayMode())
			return;

	}
	if (!bOpenTopBarInfo)
	{
		UC_TopBar_Info->RefreshProperty();
		UC_TopBar_Info->SetVisibility(ESlateVisibility::Visible);
		WidgetSwitcher_SearchBtn->SetActiveWidgetIndex(1);
	}
	else
	{
		UC_TopBar_Info->SetVisibility(ESlateVisibility::Collapsed);
		WidgetSwitcher_SearchBtn->SetActiveWidgetIndex(0);
	}

	bOpenTopBarInfo = !bOpenTopBarInfo;
}

void UTopBar::OnClick_Mail()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (RGameInstance->IsAutoPlayMode())
			return;

	}
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Mail);
}

void UTopBar::OnClick_Setting()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	if (RGameInstance->IsAutoPlayMode())
		return;
	if ( IsValid(RGameInstance->RWidgetManager))
		RGameInstance->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_Setting);
}

void UTopBar::OnClick_Chat()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		if (RGameInstance->RWidgetManager->IsShowUserWidget(EWidgetBluePrintEnum::WBP_Chat))
		{
			RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Chat);
			FString nick = RGameInstance->ChatManager->m_nick;
			if(!nick.IsEmpty())
				RGameInstance->EventManager->SetChatMinimalize.Broadcast(true);
		}
		else
		{
			RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Chat);
			RGameInstance->EventManager->SetChatMinimalize.Broadcast(false);
		}
	}
}

void UTopBar::OnClick_Avatar()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Colosseum);
}

void UTopBar::OnClick_OpenTotalMenu()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (bOpenMenuFlag)
		WidgetSwitcher_MenuListBtn->SetActiveWidgetIndex(0);
	else
		WidgetSwitcher_MenuListBtn->SetActiveWidgetIndex(1);

	bOpenMenuFlag = !bOpenMenuFlag;
}


void UTopBar::RefreshData()
{
	RefreshPropertyInfo();
	RefreshAvatarInfo();
	UC_TopBar_Info->RefreshProperty();
	/*
	avatar info
	*/

}

void UTopBar::RefreshPropertyInfo()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RWidgetManager))
		return;

	//if (WidgetSiwtcher_Account == nullptr)
	//	return;

	auto CurrentUIPage = RGameInstance->RWidgetManager->GetCurrentUIPage();
	if (CurrentUIPage == EUIPageEnum::UIPage_Shop)
	{
		auto UserWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Shop);
		if (UserWidget)
		{
			UUP_Shop* ShopWidget = Cast<UUP_Shop>(UserWidget);
			RefreshProperty_InShop(ShopWidget->CurrentShopDisplayIndex);
		}
		return;
	}

	if (UIPageEnumPtr == nullptr)
		return;

	auto EnumString = UIPageEnumPtr->GetNameStringByIndex((int32)CurrentUIPage);
	if (EnumString.IsEmpty())
		return;

	FName UIPageStringKey = FName(*EnumString);
	if (UIPageStringKey.IsNone())
		return;

	auto TopbarTableInfo = UTableManager::GetInstancePtr()->GetTopbarRow(UIPageStringKey);
	if (TopbarTableInfo == nullptr)
	{
		EUIPageEnum defaultPage = EUIPageEnum::UIPage_None;

		auto EnumStringDefault = UIPageEnumPtr->GetNameStringByIndex((int32)defaultPage);
		if (EnumStringDefault.IsEmpty())
			return;

		FName UIPageStringKeyDefault = FName(*EnumStringDefault);
		if (UIPageStringKeyDefault.IsNone())
			return;

		auto TopbarTableInfoDefault = UTableManager::GetInstancePtr()->GetTopbarRow(UIPageStringKeyDefault);
		if (TopbarTableInfoDefault)
		{
			SetSetting(TopbarTableInfoDefault);
		}
	}
	else
	{
		SetSetting(TopbarTableInfo);
	}
}

void UTopBar::RefreshProperty()
{
	for (auto& Element : TopBar_Propertys)
	{
		Element->RefreshCurrent();
	}
}

void UTopBar::SetSetting(FTopbarTableInfo* InTableInfo)
{
	if (!InTableInfo)
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RWidgetManager))
		return;

	bool bBottomBar = (InTableInfo->BottomBar != 0);
	if (bBottomBar)
	{
		RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_BottomBar);
	}

	for (auto& TopBar_Property_Elem : TopBar_Propertys)
	{
		TopBar_Property_Elem->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 Index = 0; Index < InTableInfo->PropertyKeys.Num(); Index++)
	{
		TopBar_Propertys[Index]->SetPropertyType(InTableInfo->PropertyKeys[Index]);
		TopBar_Propertys[Index]->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (IsValid(top_bg))
	{
		if (InTableInfo->BackBoardVisible)
		{
			top_bg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			top_bg->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UTopBar::RefreshAvatarInfo()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->UserInfo) || !IsValid(RGameInstance->GuildInfo))
		return;

	//if (WidgetSiwtcher_Account == nullptr)
	//	return;

	if (Text_AccountName)
	{
		auto AvatarName = FText::FromString(RGameInstance->UserInfo->Avatar.nick);
		Text_AccountName->SetText(AvatarName);
	}

	float AvatarEXP = RGameInstance->UserInfo->AvatarExpMax == 0 ? 0.0f : (float)(RGameInstance->UserInfo->Avatar.exp) / (float)(RGameInstance->UserInfo->AvatarExpMax);
	if (Text_AccountEXP)
	{
		Text_AccountEXP->SetText(FText::AsPercent(AvatarEXP));
	}

	if (ProgressBar_AvatarEXP)
	{
		ProgressBar_AvatarEXP->SetVisibility(ESlateVisibility::Visible);
		ProgressBar_AvatarEXP->SetPercent(AvatarEXP);
	}

	if (Text_Guild)
	{
		auto AvatarGuild = FText::FromString(RGameInstance->GuildInfo->MyGuildInfo.name);
		Text_Guild->SetText(AvatarGuild);
	}

	if (Text_AvatarLevel)
	{
		auto AvatarLevel = FText::AsNumber(RGameInstance->UserInfo->Avatar.level);
		Text_AvatarLevel->SetText(AvatarLevel);
	}

// 	if (ProgressBar_AvatarEXP)
// 	{
// 		float AvatarEXP = static_cast<float>(RGameInstance->UserInfo->Avatar.exp) / static_cast<float>(RGameInstance->UserInfo->AvatarExpMax);
// 		ProgressBar_AvatarEXP->SetPercent(AvatarEXP);
// 	}
}

void UTopBar::RefreshDataUI()
{
	RefreshData();

	CheckNotify();
}

void UTopBar::CheckNotify()
{
	CheckNotify_AlarmNotify();
}

void UTopBar::CheckNotify_AlarmNotify()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->NotifyEventList) && IsValid(RGameInstance->NotifyManager))
	{
		if (RGameInstance->NotifyEventList->NotifyList.Num() != 0)
		{
			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Topbar, true);
		}
		else
		{
			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Topbar, false);
		}
	}
}

void UTopBar::RefreshState()
{
	SetStateSwitcher();
}

void UTopBar::SetStateSwitcher()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RWidgetManager))
		return;
	
	if (WidgetSiwtcher_Account == nullptr)
		return;

	auto CurrentUIPage = RGameInstance->RWidgetManager->GetCurrentUIPage();

	if (CurrentUIPage == EUIPageEnum::UIPage_LobbyIndoor || CurrentUIPage == EUIPageEnum::UIPage_LobbyOutdoorPortraitMode)
	{
		WidgetSiwtcher_Account->SetActiveWidgetIndex(0);
	}
	else
	{
		WidgetSiwtcher_Account->SetActiveWidgetIndex(1);
		SetCurrentUIString(CurrentUIPage);
	}
}

void UTopBar::SetCurrentUIString(EUIPageEnum InCurrentUIPage)
{
	if (!IsValid(Text_Content))
		return;

	if (UIPageEnumPtr == nullptr)
		return;

	auto EnumString = UIPageEnumPtr->GetNameStringByIndex((int32)InCurrentUIPage);
	if (EnumString.IsEmpty())
		return;

	FName UIPageStringKey = FName(*EnumString);
	if (UIPageStringKey.IsNone())
		return;

	CurUIPageEnum = InCurrentUIPage;

	auto TopbarTableInfo = UTableManager::GetInstancePtr()->GetTopbarRow(UIPageStringKey);
	FText UIPage_ContentText;
	UIPage_ContentText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UIPageStringKey);
	if (TopbarTableInfo)
	{
		if (!TopbarTableInfo->TextKey.IsNone())
		{
			UIPage_ContentText = UUtilFunctionLibrary::GetLocalizedString(TopbarTableInfo->TextType, TopbarTableInfo->TextKey);
			Text_Content->SetText(UIPage_ContentText);
		}
		else
		{
			Text_Content->SetText(UIPage_ContentText);
		}
	}
	else
	{
		if (InCurrentUIPage == EUIPageEnum::UIPage_HeroRune_EpisodeDetail)
		{
			FFormatOrderedArguments Args;
			auto RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance)
			{
				FText HeroText = RGameInstance->HeroRuneDungeonManager->GetHeroDisplayText();
				Args.Emplace(HeroText);
			}
			UIPage_ContentText = FText::Format(UIPage_ContentText, Args);
		}

		Text_Content->SetText(UIPage_ContentText);
	}
	if (InCurrentUIPage == EUIPageEnum::UIPage_ArenaDeckSetting)
	{
		URGameInstance*	GameInstance = RGAMEINSTANCE(this);
		if(GameInstance->ArenaManager->isDefenceSetting() == true)
			Text_Content->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UIPage_ArenaDefenderSetting")));
		else
			Text_Content->SetText(UIPage_ContentText);
	}
}

void UTopBar::RefreshProperty_InShop(int32 InShopTabIndex)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RWidgetManager))
		return;

	auto CurrentUIPage = RGameInstance->RWidgetManager->GetCurrentUIPage();

	if (UIPageEnumPtr == nullptr)
		return;

	auto EnumString = UIPageEnumPtr->GetNameStringByIndex((int32)CurrentUIPage);
	if (EnumString.IsEmpty())
		return;

	FString tabString = GetShopTabString(InShopTabIndex);
	if (tabString.IsEmpty())
		return;

	FName UIPageStringKey = FName(*(EnumString + "_" + tabString));
	if (UIPageStringKey.IsNone())
		return;

	auto TopbarTableInfo = UTableManager::GetInstancePtr()->GetTopbarRow(UIPageStringKey);
	if (TopbarTableInfo == nullptr)
	{
		EUIPageEnum defaultPage = EUIPageEnum::UIPage_None;

		auto EnumStringDefault = UIPageEnumPtr->GetNameStringByIndex((int32)defaultPage);
		if (EnumStringDefault.IsEmpty())
			return;

		FName UIPageStringKeyDefault = FName(*EnumStringDefault);
		if (UIPageStringKeyDefault.IsNone())
			return;

		auto TopbarTableInfoDefault = UTableManager::GetInstancePtr()->GetTopbarRow(UIPageStringKeyDefault);
		if (TopbarTableInfoDefault)
		{
			SetSetting(TopbarTableInfoDefault);
		}
	}
	else
	{
		SetSetting(TopbarTableInfo);
	}
}

FString UTopBar::GetShopTabString(int32 InShopTabIndex)
{
	switch (InShopTabIndex)
	{
	case 1:		return FString(TEXT("Currency"));
	case 2:		return FString(TEXT("Manufacture"));
	case 3:		return FString(TEXT("Honor"));
	case 4:		return FString(TEXT("Package"));
	case 5:		return FString(TEXT("GEM"));

	default:	return FString();
	}
}




void UTopBar::RefreshCurrentUIString()
{
	SetCurrentUIString(CurUIPageEnum);
}