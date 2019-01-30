
#include "ProjectR.h"
#include "UP_Lobby_Outdoor.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "CustomActors/BaseSequenceActor.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Engine/UserInterfaceSettings.h"
#include "Network/Chat/ChatManager.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"

#include "Lobby/TowerLobbyLevelScriptActor.h"
#include "NoExportTypes.h"
#include "UI/Common/UC_Item_Icons.h"
#include "Network/PacketFunctionLibrary.h"

#define CONTENTS_BATTLEFIELD 0
#define CONTENTS_CAMPAIGN 1
#define CONTENTS_RAID 2
#define CONTENTS_DUNGEON 3
#define CONTENTS_PVP 4

void UUP_Lobby_Outdoor::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	RGameInstance->EventManager->GetNoMissionSignal.AddDynamic(this, &UUP_Lobby_Outdoor::OnClickFloorMissionButton);
	RGameInstance->EventManager->SetChatMinimalize.AddDynamic(this, &UUP_Lobby_Outdoor::ChatMinimalizeOnOff);
	RGameInstance->EventManager->UpdateLastMessage.AddDynamic(this, &UUP_Lobby_Outdoor::UpdateMessage);

	FString nick = RGameInstance->ChatManager->m_nick;
	FString message = RGameInstance->ChatManager->m_message;
	if (nick.IsEmpty())
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	else
		UpdateMessage(nick, message);

	//Btn_PVP->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonPVPClicked);
	Btn_Campaign->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonCampaignClicked);
	Btn_BattleField->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonBattleFieldClicked);
	Btn_Raid->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonPVEClicked);
	Btn_Dungeon->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonDungeonClicked);

	Button_Arena->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonArenaClicked);
	Button_Colosseum->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonColosseumClicked);
	Button_NormalStage->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonNormalStageClicked);
	Button_EliteStage->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtoEliteStageClicked);
	Button_Raid->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonRaidClicked);
	Button_WorldBoss->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonWorldBossClicked);
	Button_MirrorDungeon->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonMirrorDungeonClicked);
	Button_WeekDungeon->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonWeekDungeonClicked);

	variables.Button_GotoDailyMission->OnClicked.Clear();
	variables.Button_GotoDailyMission->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonDailyMissionClicked);

	RGameInstance->QuestManager->OnReceiveQuestInfo.Clear();
	RGameInstance->QuestManager->OnReceiveQuestInfo.AddDynamic(this, &UUP_Lobby_Outdoor::SetDailyQuest);

	UPacketFunctionLibrary::DAILY_QUEST_STATUS_RQ();
	//	엄재영 Test
// 	Button_Test->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonTestClicked);
// 	Button_Test->OnClicked.AddDynamic(this, &UUP_Lobby_Outdoor::OnButtonTestClicked);
	//
	OnButtonBattleFieldClicked();
	Invite_Popup_Outdoor->SetVisibility(ESlateVisibility::Collapsed);
	bGetAllDoorFinished = false;
	SetFloorInfo();


	/*Unlock Check*/
	ContentsLayoutsettings();
	/**/

}

void UUP_Lobby_Outdoor::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	RGameInstance->EventManager->GetNoMissionSignal.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnClickFloorMissionButton);
	RGameInstance->EventManager->SetChatMinimalize.RemoveDynamic(this, &UUP_Lobby_Outdoor::ChatMinimalizeOnOff);
	RGameInstance->EventManager->UpdateLastMessage.RemoveDynamic(this, &UUP_Lobby_Outdoor::UpdateMessage);

	Btn_PVP->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonPVPClicked);
	Btn_Campaign->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonCampaignClicked);
	Btn_BattleField->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonBattleFieldClicked);
	Btn_Raid->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonPVEClicked);
	Btn_Dungeon->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonDungeonClicked);

	Button_Arena->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonArenaClicked);
	Button_Colosseum->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonColosseumClicked);
	Button_NormalStage->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonNormalStageClicked);
	Button_EliteStage->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtoEliteStageClicked);
	Button_Raid->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonRaidClicked);
	Button_WorldBoss->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonWorldBossClicked);
	Button_MirrorDungeon->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonMirrorDungeonClicked);
	Button_WeekDungeon->OnClicked.RemoveDynamic(this, &UUP_Lobby_Outdoor::OnButtonWeekDungeonClicked);

	bGetAllDoorFinished = false;

	Super::NativeDestruct();
}

void UUP_Lobby_Outdoor::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bGetAllDoorFinished)
	{
		DoorObjInit();
	}
	else
	{
		SetButtonLocationWithMapObj();
	}
}

void UUP_Lobby_Outdoor::DoorObjInit()
{
	UWorld* GameWorld = RGAMEINSTANCE(this)->GetWorld();
	PlayerController = UUtilFunctionLibrary::GetLobbyPlayerController(GameWorld);
	AActor* Actor = nullptr;

	if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
	{
		LobbyOutDoorBFObj = Cast<AEmitter>(UUtilFunctionLibrary::FindActor<AActor>(GameWorld, AEmitter::StaticClass(), FName(TEXT("FX_2_Lobby_portal2"))));

		for (TActorIterator<AActor> ActorItr(GameWorld); ActorItr; ++ActorItr)
		{
			if (!GameWorld)
				break;
			Actor = *ActorItr;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_B01_SM2")))
				LobbyOutDoorPVPObj = Actor;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_C01_SM2")))
				LobbyOutDoorDungeonObj = Actor;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_D01_SM2")))
				LobbyOutDoorCampaignObj = Actor;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_A01_SM2")))
				LobbyOutDoorRaidObj = Actor;
		}
	}
	else
	{
		LobbyOutDoorBFObj = Cast<AEmitter>(UUtilFunctionLibrary::FindActor<AEmitter>(GameWorld, AEmitter::StaticClass(), FName(TEXT("FX_2_Lobby_portal2"))));

		for (TActorIterator<AActor> ActorItr(GameWorld); ActorItr; ++ActorItr)
		{
			if (!GameWorld)
				break;
			Actor = *ActorItr;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_B01_SM")))
				LobbyOutDoorPVPObj = Actor;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_C01_SM_24")))
				LobbyOutDoorDungeonObj = Actor;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_D01_SM_18")))
				LobbyOutDoorCampaignObj = Actor;
			if (IsValid(Actor) && Actor->GetFName() == FName(TEXT("Lobby_Out_Side_A01_SM_14")))
				LobbyOutDoorRaidObj = Actor;
		}
	}
	if (LobbyOutDoorBFObj && LobbyOutDoorPVPObj && LobbyOutDoorDungeonObj && LobbyOutDoorCampaignObj && LobbyOutDoorRaidObj)
		bGetAllDoorFinished = true;
}

void UUP_Lobby_Outdoor::OnButtonPVPClicked()
{

}

void UUP_Lobby_Outdoor::OnButtonCampaignClicked()
{
	UPaperSprite* sprite = UUtilFunctionLibrary::GetSpriteFromTable(TEXT("UI_Outdoor_ContentsSymbol_Adventure"));

	UUIFunctionLibrary::SetImageBrush(sprite, Image_ContentsSymbol);
	Text_ContentsTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Outdoor_Title_Adventure"));
	WidgetSwitcher_UI->SetActiveWidgetIndex(CONTENTS_CAMPAIGN);
	
	SetButtonColorRelease(iCurrentPush);
	iCurrentPush = CONTENTS_CAMPAIGN;
	SetButtonColorPush(iCurrentPush);
	SetCampaignProgress();
}

void UUP_Lobby_Outdoor::OnButtonBattleFieldClicked()
{
	UPaperSprite* sprite = UUtilFunctionLibrary::GetSpriteFromTable(TEXT("UI_Outdoor_ContentsSymbol_PVP"));

	UUIFunctionLibrary::SetImageBrush(sprite, Image_ContentsSymbol);
	Text_ContentsTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Outdoor_Title_PVP"));
	WidgetSwitcher_UI->SetActiveWidgetIndex(CONTENTS_BATTLEFIELD);

	SetButtonColorRelease(iCurrentPush);
	iCurrentPush = CONTENTS_BATTLEFIELD;
	SetButtonColorPush(iCurrentPush);
// 	UE_LOG(LogUI, Log, TEXT("In Developing Contents"));
// 	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Content_Lock_Result")), FText());
}

void UUP_Lobby_Outdoor::OnButtonPVEClicked()
{
	UPaperSprite* sprite = UUtilFunctionLibrary::GetSpriteFromTable(TEXT("UI_Outdoor_ContentsSymbol_PVE"));

	UUIFunctionLibrary::SetImageBrush(sprite, Image_ContentsSymbol);
	Text_ContentsTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Outdoor_Title_Raid"));
	WidgetSwitcher_UI->SetActiveWidgetIndex(CONTENTS_RAID);
	
	SetButtonColorRelease(iCurrentPush);
	iCurrentPush = CONTENTS_RAID;
	SetButtonColorPush(iCurrentPush);
}

void UUP_Lobby_Outdoor::OnButtonDungeonClicked()
{
	UPaperSprite* sprite = UUtilFunctionLibrary::GetSpriteFromTable(TEXT("UI_Outdoor_ContentsSymbol_Dungeon"));

	UUIFunctionLibrary::SetImageBrush(sprite, Image_ContentsSymbol);
	Text_ContentsTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Outdoor_Title_Dungeon"));
	WidgetSwitcher_UI->SetActiveWidgetIndex(CONTENTS_DUNGEON);
	
	SetButtonColorRelease(iCurrentPush);
	iCurrentPush = CONTENTS_DUNGEON;
	SetButtonColorPush(iCurrentPush);
}

void UUP_Lobby_Outdoor::OnButtonRaidClicked()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_SingleRaid, 1))
	{
		RGameInstance->RaidManager->TodayType = RGameInstance->GetRaidDayType();
		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_RaidLobby);
	}
	else
	{
		FText TitleText;
		FText DescText;

		RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_SingleRaid, TitleText, DescText, 1);
		UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
	}
}

void UUP_Lobby_Outdoor::OnButtonArenaClicked()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if (!GameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Arena))
	{
#if WITH_EDITOR
		if (GameInstance->GameEnvironmentVar.bCheckContentsUnlock)
#endif // WITH_EDITOR
		{
			FText TitleText;
			FText DescText;

			GameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Arena, TitleText, DescText);
			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
			return;
		}
	}

	if (GameInstance->RInventory->IsEmptyDefenceDeck())
	{
		GameInstance->ArenaManager->SetDefenceSetting(true);
		GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ArenaDeckSetting);
		return;
	}

	GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ArenaLobby);
}

void UUP_Lobby_Outdoor::OnButtonColosseumClicked()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
// 	if (!GameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Colosseum))
// 	{
// #if WITH_EDITOR
// 		if(GameInstance->GameEnvironmentVar.bCheckContentsUnlock)
// #endif // WITH_EDITOR
// 		{
// 			FText TitleText;
// 			FText DescText;
// 
// 			GameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Colosseum, TitleText, DescText);
// 			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
// 
// 			return;
// 		}
// 	}

	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Colosseum);
}

void UUP_Lobby_Outdoor::OnButtonNormalStageClicked()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Campaign, 1))
	{
		RGameInstance->CampaignManager->SetStageDifficulty(EStageDifficultyEnum::Normal);
		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_Campaign);
	}
	else
	{
		FText TitleText;
		FText DescText;

		RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, TitleText, DescText, 1);
		UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
	}
}

void UUP_Lobby_Outdoor::OnButtoEliteStageClicked()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_CampaignHard, 1))
	{
		RGameInstance->CampaignManager->SetStageDifficulty(EStageDifficultyEnum::Hard);
		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_Campaign);
	}
	else
	{
		FText TitleText;
		FText DescText;

		RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_CampaignHard, TitleText, DescText, 1);
		UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
	}
}

void UUP_Lobby_Outdoor::OnButtonTestClicked()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);

	ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentMode)
		CurrentMode->ChangeState(ETowerLobbyStateEnum::Indoor, ETowerLobbySubstateEnum::RaidTowerBoss);

	GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_DeckTest);
}

void UUP_Lobby_Outdoor::OnButtonMirrorDungeonClicked()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if (!GameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_MirrorDungeon))
	{
#if WITH_EDITOR
		if (GameInstance->GameEnvironmentVar.bCheckContentsUnlock)
#endif // WITH_EDITOR
		{
			FText TitleText;
			FText DescText;

			GameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_MirrorDungeon, TitleText, DescText);
			UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);

			return;
		}
	}

	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_MirrorDungeon);
}

void UUP_Lobby_Outdoor::OnButtonWeekDungeonClicked()
{
	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Content_Lock_Result")), FText());
}

void UUP_Lobby_Outdoor::OnButtonWorldBossClicked()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_MultiRaid, 1))
	{
		RGameInstance->RaidManager->TodayType = ERaidDayType::WEEK;
		UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_RaidLobby);
	}
	else
	{
		FText TitleText;
		FText DescText;

		RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_MultiRaid, TitleText, DescText, 1);
		UUIFunctionLibrary::ShowCommonPopupSplash(TitleText, DescText);
	}
}

void UUP_Lobby_Outdoor::OnButtonDailyMissionClicked()
{
	UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_Daily_Quest);
}

void UUP_Lobby_Outdoor::SetFloorInfo()
{
	FAVATAR Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();
	FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;

}

void UUP_Lobby_Outdoor::SetButtonLocationWithMapObj()
{
	SetOffset(LobbyOutDoorPVPObj, Btn_PVP_Overlay);
	SetOffset(LobbyOutDoorBFObj, Btn_BattleField_Overlay);
	SetOffset(LobbyOutDoorDungeonObj, Btn_Dungeon_Overlay);
	SetOffset(LobbyOutDoorCampaignObj, Btn_Campaign_Overlay);
	SetOffset(LobbyOutDoorRaidObj, Btn_Raid_Overlay);
}

void UUP_Lobby_Outdoor::SetOffset(AActor * actor, UOverlay * overlay)
{
	FVector2D TempLoca(0, 0);
	if (PlayerController && actor && actor->IsValidLowLevel())
		PlayerController->ProjectWorldLocationToScreen(actor->GetTargetLocation(), TempLoca);

	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));

	TempLoca /= ViewportScale;

	if (overlay->Slot)
		overlay->SetRenderTranslation(FVector2D(TempLoca.X, TempLoca.Y));
}

void UUP_Lobby_Outdoor::SetMinimalizeOverlay()
{
	if (m_reverse)
	{
		if (!RGAMEINSTANCE(this)->ChatManager->m_message.IsEmpty())
			Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUP_Lobby_Outdoor::SetButtonColorRelease(int32 index)
{
	switch (index)
	{
	case CONTENTS_PVP:
		Text_PVP->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		ForeImage_PVP->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		BackImage_PVP->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		break;
	case CONTENTS_CAMPAIGN:
		Text_Campaign->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		ForeImage_Campaign->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		BackImage_Campaign->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		break;
	case CONTENTS_BATTLEFIELD:
		Text_BattleField->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		ForeImage_BattleField->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		BackImage_BattleField->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		break;
	case CONTENTS_RAID:
		Text_Raid->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		ForeImage_Raid->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		BackImage_Raid->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		break;
	case CONTENTS_DUNGEON:
		Text_Dungeon->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		ForeImage_Dungeon->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		BackImage_Dungeon->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		break;
	default:
		break;
	}
}

void UUP_Lobby_Outdoor::SetDailyQuest()
{
	UQuestManager* QuestManager = RGAMEINSTANCE(this)->QuestManager;
	int32 CanGetNum = 0;

	variables.CurrentScore->SetText(FText::AsNumber(QuestManager->CurrentScore));

	UTableManager* TM = UTableManager::GetInstancePtr();
	// right top side ui
	TArray<FName> RowNames = TM->GetDailyQuestRewardsRowsNames();
	for (int32 i = 0; i < RowNames.Num(); ++i)
	{
		FName& Key = RowNames[i];

		FDailyQuestRewardsTable* pTable = TM->GetDailyQuestRewardsTableRow(Key);
		if (pTable)
		{
			variables.CanGet_Images[i]->SetVisibility(ESlateVisibility::Collapsed);
			variables.AlreadyGet_Overlays[i]->SetVisibility(ESlateVisibility::Collapsed);
			variables.RewardButtons[i]->SetVisibility(ESlateVisibility::Collapsed);

			switch (QuestManager->RewardableList[i])
			{
			case CANTREWARD:
				break;
			case CANREWARD:
				variables.CanGet_Images[i]->SetVisibility(ESlateVisibility::Visible);
				variables.RewardButtons[i]->SetVisibility(ESlateVisibility::Visible);
				break;
			case ALREADYREWARD:
				variables.AlreadyGet_Overlays[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				break;
			}

			FITEM_ICON_INFO iconinfo;
			UUIFunctionLibrary::GetItemIconInfoByWallet(pTable->WalletId, pTable->Amt, iconinfo);
			variables.Item_Icons[i]->SetItemIconStructure(iconinfo, true);
		}
	}
}

void UUP_Lobby_Outdoor::SetCampaignProgress()
{
	URGameInstance* GameIns = RGAMEINSTANCE(this);
	UCampaignManager* CM = GameIns->CampaignManager;

	if (!CM)
		return;

	int32 currNormalFloor = CM->GetCurrentNormalFloor();
	int32 currHardFloor = CM->GetCurrentHardFloor();
	int32 currNormalStage = CM->GetNormalProgressIndex();
	int32 currHardStage = CM->GetHardProgressIndex();

// 	CM->SetNormalProgressIndex(currNormalStage % 6);
// 	CM->SetHardProgressIndex(currHardStage % 6);

	FText NormalStageTxt= FText::Format(FText::FromString("{0}-{1}"), FText::AsNumber(currNormalFloor), FText::AsNumber((currNormalStage % 6) + 1));
	FText HardStageTxt = FText::Format(FText::FromString("{0}-{1}"), FText::AsNumber(currHardFloor), FText::AsNumber((currHardStage % 6) + 1));

	if (currNormalFloor > 25)
	{
		Text_NormalFloor->SetText(FText::AsNumber(25));
		Text_NormalStage->SetText(FText::FromString("25-6"));
	}
	else
	{
		Text_NormalFloor->SetText(FText::AsNumber(currNormalFloor));
		Text_NormalStage->SetText(NormalStageTxt);
	}
	if (currHardFloor > 25)
	{
		Text_EliteFloor->SetText(FText::AsNumber(25));
		Text_EliteStage->SetText(FText::FromString("25-6"));
	}
	else
	{
		Text_EliteFloor->SetText(FText::AsNumber(currHardFloor));
		Text_EliteStage->SetText(HardStageTxt);
	}
}

void UUP_Lobby_Outdoor::ContentsLayoutsettings()
{
	FText Titietxt;
	FText Desctxt;
	auto ContentsUnLockManager = RGAMEINSTANCE(this)->ContentsUnLockManager;
	if (!ContentsUnLockManager)
		return;
	bool bCanPlayArena = ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Arena);
	bool bCanPlayCampaignNormal = ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Campaign , 1);
	bool bCanPlayCampaignElite = ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_CampaignHard, 1);
	bool bCanPlayMirror = ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_MirrorDungeon);
	bool bCanPlaySingleRaid = ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_SingleRaid , 1);
	bool bCanPlayMultiRaid = ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_MultiRaid, 1);

	if (!bCanPlayArena)
	{
		ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Arena, Titietxt, Desctxt);
		variables.Overlay_ArenaLock->SetVisibility(ESlateVisibility::Visible);
		variables.Text_ArenaLock->SetText(Desctxt);
	}
	else
	{
		variables.Overlay_ArenaLock->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!bCanPlayCampaignNormal)
	{
		ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, Titietxt, Desctxt, 1);
		variables.Overlay_ScenarioNormalLock->SetVisibility(ESlateVisibility::Visible);
		variables.Text_ScenarioNormalLock->SetText(Desctxt);
	}
	else
	{
		variables.Overlay_ScenarioNormalLock->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!bCanPlayCampaignElite)
	{
		ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_CampaignHard, Titietxt, Desctxt, 1);
		variables.Overlay_ScenarioEliteLock->SetVisibility(ESlateVisibility::Visible);
		variables.Text_ScenarioEliteLock->SetText(Desctxt);
	}
	else
	{
		variables.Overlay_ScenarioEliteLock->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!bCanPlayMirror)
	{
		ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_MirrorDungeon, Titietxt, Desctxt);
		variables.Overlay_MirrorDungeonLock->SetVisibility(ESlateVisibility::Visible);
		variables.Text_MirrorDungeonLock->SetText(Desctxt);
	}
	else
	{
		variables.Overlay_MirrorDungeonLock->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!bCanPlaySingleRaid)
	{
		ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_SingleRaid, Titietxt, Desctxt, 1);
		variables.Overlay_SingleRaidLock->SetVisibility(ESlateVisibility::Visible);
		variables.Text_SingleRaidLock->SetText(Desctxt);
	}
	else
	{
		variables.Overlay_SingleRaidLock->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!bCanPlayMultiRaid)
	{
		ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_MultiRaid, Titietxt, Desctxt, 1);
		variables.Overlay_WorldBossLock->SetVisibility(ESlateVisibility::Visible);
		variables.Text_WorldBossLock->SetText(Desctxt);
	}
	else
	{
		variables.Overlay_WorldBossLock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_Lobby_Outdoor::ChatMinimalizeOnOff(bool isOn)
{
	if(isOn)
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Visible);
	else
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_Lobby_Outdoor::UpdateMessage(FString nick, FString message)
{
	if (Chat_Minimalize_Overlay->GetVisibility() == ESlateVisibility::Collapsed)
		Chat_Minimalize_Overlay->SetVisibility(ESlateVisibility::Visible);
	Text_NickName->SetText(FText::FromString(nick));
	Text_Message->SetText(FText::FromString(message));
}

void UUP_Lobby_Outdoor::OnClickFloorMissionButton()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	FFLOOR_MISSION_STATUS currentStatus = GameInstance->MissionManager->GetMissionStatus();
	if (currentStatus.floor == 0)
	{
		UUP_CommonPopup* popup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Error_Message_000023")), ECommonPopupType::VE_OKCANCEL);
	}
	else
	{
		GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_MissionSystem);
	}
}

void UUP_Lobby_Outdoor::OnClicked_RewardIcon(int32 index)
{
	UPacketFunctionLibrary::DAILY_QUEST_REWARD_RQ(index);
}

void UUP_Lobby_Outdoor::SetButtonColorPush(int32 index)
{
	switch (index)
	{
	case CONTENTS_PVP:
		Text_PVP->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		ForeImage_PVP->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		BackImage_PVP->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		break;
	case CONTENTS_CAMPAIGN:
		Text_Campaign->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		ForeImage_Campaign->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		BackImage_Campaign->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		break;
	case CONTENTS_BATTLEFIELD:
		Text_BattleField->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		ForeImage_BattleField->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		BackImage_BattleField->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		break;
	case CONTENTS_RAID:
		Text_Raid->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		ForeImage_Raid->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		BackImage_Raid->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		break;
	case CONTENTS_DUNGEON:
		Text_Dungeon->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		ForeImage_Dungeon->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		BackImage_Dungeon->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(63, 215, 255)));
		break;
	default:
		break;
	}
}
