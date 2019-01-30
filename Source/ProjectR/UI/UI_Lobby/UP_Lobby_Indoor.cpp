#include "ProjectR.h"
#include "UP_Lobby_Indoor.h"

#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "Lobby/IndoorLobbyStateActor.h"

#include "Global/RGameInstance.h"
#include "Global/EventManager.h"
#include "Global/CampaignManager.h"
#include "Global/TableManager.h"
#include "Global/ContentsUnLockManager.h"
#include "Global/SequenceManager.h"
#include "Global/QuestManager.h"
#include "Global/MissionManager.h"
#include "Global/ColosseumManager.h"

#include "Network/Chat/ChatManager.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/Common/NPCSystemMessage.h"
#include "UI/RWidgetManager.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"



void UUP_Lobby_Indoor::NativeConstruct()
{
	Super::NativeConstruct();
	//temp
	/*UC_Lobby_MenuBasic->GetWidgetFromName(FName(TEXT("ShopHero_Overlay")))->SetVisibility(ESlateVisibility::Visible);
	UC_Lobby_MenuOpen->GetWidgetFromName(FName(TEXT("ShopHero_Overlay")))->SetVisibility(ESlateVisibility::Visible);*/
	OutmenuAnim = this->GetAni("OutMenu");
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (OutmenuAnim)
	{
		OutmenuAnim->OnAnimationFinished.RemoveDynamic(this, &UUP_Lobby_Indoor::SetMinimalizeOverlay);
		OutmenuAnim->OnAnimationFinished.AddDynamic(this, &UUP_Lobby_Indoor::SetMinimalizeOverlay);
	}
	RGameInstance->EventManager->GetNoMissionSignal.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickFloorMissionButton);
	RGameInstance->EventManager->GetNoMissionSignal.AddDynamic(this, &UUP_Lobby_Indoor::OnClickFloorMissionButton);
	RGameInstance->EventManager->SetChatMinimalize.RemoveDynamic(this, &UUP_Lobby_Indoor::ChatMinimalizeOnOff);
	RGameInstance->EventManager->SetChatMinimalize.AddDynamic(this, &UUP_Lobby_Indoor::ChatMinimalizeOnOff);
	RGameInstance->EventManager->UpdateLastMessage.RemoveDynamic(this, &UUP_Lobby_Indoor::UpdateMessage);
	RGameInstance->EventManager->UpdateLastMessage.AddDynamic(this, &UUP_Lobby_Indoor::UpdateMessage);
	FString nick = RGameInstance->ChatManager->m_nick;
	FString message = RGameInstance->ChatManager->m_message;

	if (nick.IsEmpty() && Chat_Minimalize_Overlay)
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	else
		UpdateMessage(nick, message);

	Button_OutTransition->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnButtonOutTransitionClicked);
	Button_OutTransition->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnButtonOutTransitionClicked);
// 	Button_DropDown->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnButtonDropDownClicked);
// 	Button_DropDown->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnButtonDropDownClicked);
// 	Button_Dropup->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnButtonDropUpClicked);
// 	Button_Dropup->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnButtonDropUpClicked);
	SetFloorInfo();

// 	Button_FloorShop->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickFloorShopButton);
// 	Button_FloorShop->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickFloorShopButton);
// 
// 	Button_FloorRaid->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickFloorRaidButton);
// 	Button_FloorRaid->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickFloorRaidButton);
// 
// 	Button_Adventure->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickAdventureButton);
// 	Button_Adventure->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickAdventureButton);

	if (IsValid(GotoMissionContentsButton))
	{
		GotoMissionContentsButton->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickGotoMissionContents);
		GotoMissionContentsButton->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickGotoMissionContents);
	}

	if (Button_MenuOpen->IsValidLowLevel())
	{
		Button_MenuOpen->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickMenuOpen);
		Button_MenuOpen->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickMenuOpen);
	}

	if (Button_MenuClose->IsValidLowLevel())
	{
		Button_MenuClose->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickMenuClose);
		Button_MenuClose->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickMenuClose);
	}

	if (Button_HeroShop->IsValidLowLevel())
	{
		Button_HeroShop->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickHeroShop);
		Button_HeroShop->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickHeroShop);
	}

	if (Button_NPC_Speech->IsValidLowLevel())
	{
		Button_NPC_Speech->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickNPCSpeechBtn);
		Button_NPC_Speech->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickNPCSpeechBtn);
	}

	if (Button_Bag->IsValidLowLevel())
	{
		Button_Bag->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickBag);
		Button_Bag->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickBag);
	}

	if (Button_HeroManagement->IsValidLowLevel())
	{
		Button_HeroManagement->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickHeroManagement);
		Button_HeroManagement->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickHeroManagement);
	}

	if (Button_CSGrade)
	{
		Button_CSGrade->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::OnClickedCSGrade);
		Button_CSGrade->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::OnClickedCSGrade);
	}

	if (UC_Notify)
	{
		int32 SummonTicket = RGameInstance->RInventory->GetCurrencyWithKey(TEXT("SUMMONTICKET02"));
		if(SummonTicket >= 100)
			UC_Notify->SetVisibleByNotify(ENotifyStateFlagEnum::BM_Shop, true);
		else
			UC_Notify->SetVisibleByNotify(ENotifyStateFlagEnum::BM_Shop, false);
	}
	
// 	RGameInstance->AttendanceMnager->ShowAttendanceWidget();

// 	RGameInstance->EventManager->OutMenuAnimPlay.RemoveDynamic(this, &UUP_Lobby_Indoor::OutMenuAnimPlay);
// 	RGameInstance->EventManager->OutMenuAnimPlay.AddDynamic(this, &UUP_Lobby_Indoor::OutMenuAnimPlay);

	//AIndoorLobbyStateActor* indoorActor = RGameInstance->GetIndoorStateActor();
	
	SetHeroText();
	
	SetImportantMissionInfo();

	//SetWelcomeText();

// 	Invite_Popup_Indoor->SetVisibility(ESlateVisibility::Collapsed);

}

void UUP_Lobby_Indoor::NativeDestruct()
{
	if (IsValid(Button_OutTransition))
		Button_OutTransition->OnClicked.RemoveAll(this);
	if (IsValid(Button_DropDown))
		Button_DropDown->OnClicked.RemoveAll(this);
	if (IsValid(Button_Dropup))
		Button_Dropup->OnClicked.RemoveAll(this);
	
// 	RGAMEINSTANCE(this)->EventManager->OutMenuAnimPlay.RemoveAll(this);

	Super::NativeDestruct();
}

void UUP_Lobby_Indoor::SetWelcomeText()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	FText WelcomeText = FText::Format( 
		UUtilFunctionLibrary::GetLocalizedString(
			EStringTableType::VE_SystemMessage, 
			TEXT("NPC_Welcome_Message")),
			FText::FromString(RGameInstance->UserInfo->Avatar.nick));

	NPCMessage->SetNPCMessageText(WelcomeText);
}

void UUP_Lobby_Indoor::OnClickTutorial()
{
	RGAMEINSTANCE(this)->ChangeState(EGameStateEnum::VE_Tutorial);
}

void UUP_Lobby_Indoor::OnClickMenuOpen()
{
	MenuOpenButtonSwitcher->SetActiveWidgetIndex(1);

	OutMenuAnimPlay(false);
}

void UUP_Lobby_Indoor::OnClickMenuClose()
{
	MenuOpenButtonSwitcher->SetActiveWidgetIndex(0);

	OutMenuAnimPlay(true);
}

void UUP_Lobby_Indoor::OnClickNPCSpeechBtn()
{
	if (MenuOpenButtonSwitcher->GetActiveWidgetIndex() == 0)
		OnClickMenuOpen();
	else
		OnClickMenuClose();
}

void UUP_Lobby_Indoor::OnClickedCSGrade()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Colosseum);
}

void UUP_Lobby_Indoor::OnClickHeroShop()
{
	UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_ShopHero);
	URGameInstance* GameInst = RGAMEINSTANCE(this);

	FStreamableDelegate Callback;
	//todo qsmname - check legacy table, todo Sync Click(deferred)
	Callback.BindLambda([GameInst]() {
		GameInst->ResourceCacheManager->OnAsyncLoadedCompleted.ExecuteIfBound();

		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_ShopHero);
	});
	GameInst->ResourceCacheManager->cache_HeroShop(Callback);
	
}

void UUP_Lobby_Indoor::OnClickBag()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Item);
}

void UUP_Lobby_Indoor::OnClickHeroManagement()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_DetailHeros);
}

void UUP_Lobby_Indoor::OnButtonOutTransitionClicked()
{
	RGAMEINSTANCE(this)->SequenceManager->ClearSequence();

	UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_LobbyOutdoor);
}

void UUP_Lobby_Indoor::OnButtonDropDownClicked()
{
	WidgetSwitcher_DropBox->SetActiveWidgetIndex(1);
}

void UUP_Lobby_Indoor::OnButtonDropUpClicked()
{
	WidgetSwitcher_DropBox->SetActiveWidgetIndex(0);
}


// void UUP_Lobby_Indoor::OnClickFloorShopButton()
// {
// 	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_FloorShop);
// }

// void UUP_Lobby_Indoor::OnClickFloorRaidButton()
// {
// 	URGameInstance* GameInstance = RGAMEINSTANCE(this);
// 	if (!GameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_SingleRaid))
// 	{
// #if WITH_EDITOR
// 		if (GameInstance->GameEnvironmentVar.bCheckContentsUnlock)
// #endif // WITH_EDITOR
// 		{
// 			FText TitleText;
// 			FText DescText;
// 
// 			GameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_SingleRaid, TitleText, DescText);
// 			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
// 
// 			return;
// 		}
// 	}
// 
// 	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_TowerBoss);
// }

// void UUP_Lobby_Indoor::OnClickAdventureButton()
// {
// 	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Campaign);
// }

void UUP_Lobby_Indoor::SetFloorInfo()
{
	auto ColosseumManager = RGAMEINSTANCE(this)->ColosseumManager;

	FColosseumGrade* ColosseumGrade = UTableManager::GetInstancePtr()->GetColosseumGrade(
		FName(*(FString::FromInt(ColosseumManager->ColosseumStatus.gradeId))));
	if (ColosseumGrade == nullptr)
		return;

	int32 Leaguetier = (ColosseumManager->ColosseumStatus.gradeId - 1) / 4;

	FText grade = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI,  
		FName(*(TEXT("UI_Colosseum_Tier") + FString::FromInt(Leaguetier + 1))));

	if (CurrentCSGrade)
		CurrentCSGrade->SetText(grade);
	if (ActivateFloor_Close_TB)
		ActivateFloor_Close_TB->SetText(ColosseumGrade->Grade);
// 	FAVATAR Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();
// 	FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;

	/*ActivateFloor_Close_TB->SetText(FText::AsNumber(TowerStatus.activatedFloor));
	ActivateFloor_Open_TB->SetText(FText::AsNumber(TowerStatus.activatedFloor));
	ElevatedFloor_Open_TB->SetText(FText::AsNumber(Avatar.elevatedFloor));*/
	//CurrentFloor_Open_TB->SetText(FText::AsNumber(TowerStatus.currentFloor));
}


void UUP_Lobby_Indoor::OutMenuAnimPlay(bool reverse)
{
	m_reverse = reverse;
	if(!m_reverse && Chat_Minimalize_Overlay)
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	PlayAni("OutMenu", m_reverse);
}

void UUP_Lobby_Indoor::SetMinimalizeOverlay()
{
	if (m_reverse)
	{
		if(!RGAMEINSTANCE(this)->ChatManager->m_message.IsEmpty() && Chat_Minimalize_Overlay)
			Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUP_Lobby_Indoor::ChatMinimalizeOnOff(bool isOn)
{
	if (!Chat_Minimalize_Overlay)
		return;
	if (isOn)
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Visible);
	else
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_Lobby_Indoor::UpdateMessage(FString nick, FString message)
{
// 	if(Chat_Minimalize_Overlay)
// 	{
// 		if (Chat_Minimalize_Overlay->GetVisibility() == ESlateVisibility::Collapsed)
// 			Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Visible);
// 	}
// 
// 	if(Text_NickName)
// 		Text_NickName->SetText(FText::FromString(nick));
// 
// 	if(Text_Message)
// 		Text_Message->SetText(FText::FromString(message));
}

void UUP_Lobby_Indoor::SetHeroText()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
	AIndoorLobbyStateActor* indoorActor = nullptr;
	if (TowerLobbyGameMode)
		indoorActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));

	if (IsValid(indoorActor) && IsValid(Image_Job) && IsValid(indoorActor->heroSprite))
	{
		UUIFunctionLibrary::SetImageBrush(indoorActor->heroSprite, Image_Job);
		Text_HeroName->SetText(indoorActor->heroName);

		SetLobbyHeroGrade(indoorActor->heroGrade);
// 		Text_HeroNickName->SetText(indoorActor->heroNickName);
	}
}

void UUP_Lobby_Indoor::SetLobbyHeroGrade(int32 InGrade)
{
	if (IsValid(HeroStar))
	{
		HeroStar->SetStar(InGrade);
	}
}

void UUP_Lobby_Indoor::SetImportantMissionInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FFloorQuestTableInfo* pTableData = UTableManager::GetInstancePtr()->GetFloorQuestRow(FName(*FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId).ToString()));
	if (pTableData)
	{
		ImportantMissionOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
		ImportantMissionTargetText->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pTableData->TargetStringKey));
		ImportantCurrentText->SetText(FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.progress));
		ImportantMaxText->SetText(FText::AsNumber(pTableData->ConditionGoal));
	}
	else
	{
		ImportantMissionOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_Lobby_Indoor::OnClickGotoMissionContents()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nCurrentQuest = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
	int32 nbaseIndex = 0;
	int32 nFloor = 0;

	for (; nFloor < RGameInstance->QuestManager->ImportantQuestList.Num(); ++nFloor)
	{
		nbaseIndex += RGameInstance->QuestManager->ImportantQuestList[nFloor].Num();
		if (nbaseIndex >= nCurrentQuest)
			break;
	}
	++nFloor; //magic

	if(RGameInstance->ContentsUnLockManager->IsUnlockFloor(nFloor))
	{
		UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Campaign);
		RGameInstance->QuestManager->UseNavigation();
	}
	else
	{
		FText title;
		FText desc;

		RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, title, desc, nFloor);
		UUIFunctionLibrary::ShowCommonPopupSplash(title, desc);
	}
}

void UUP_Lobby_Indoor::OnClickFloorMissionButton()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	FFLOOR_MISSION_STATUS currentStatus = GameInstance->MissionManager->GetMissionStatus();
	if (currentStatus.floor == 0)
	{
		UUP_CommonPopup* popup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Error_Message_000023")), ECommonPopupType::VE_OKCANCEL);
		popup->Button_OK->OnClicked.RemoveDynamic(this, &UUP_Lobby_Indoor::GoCampaign);
		popup->Button_OK->OnClicked.AddDynamic(this, &UUP_Lobby_Indoor::GoCampaign);
	}
	else
	{
		GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_MissionSystem);
	}
}

void UUP_Lobby_Indoor::GoCampaign()
{
	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_Campaign);
}
