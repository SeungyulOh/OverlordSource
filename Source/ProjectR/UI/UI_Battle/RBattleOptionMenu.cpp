// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RBattleOptionMenu.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"
#include "ContentGameMode/Modes/StageGameMode.h"
#include "ContentGameMode/Modes/ArenaGameMode.h"
#include "ContentGameMode/Modes/MirrorGameMode.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/UserInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/UI_Setting/UP_Setting.h"
#include "UtilFunctionIntegrated.h"
#include "Network/RTSManager.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "GameState/TowerLobbyState.h"

void URBattleOptionMenu::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	Btn_Pause->SetVisibility(ESlateVisibility::Visible);
}

void URBattleOptionMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

bool URBattleOptionMenu::Init()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	BasePlayerController	= UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	PlayStateInfo = RGameInstance->PlayStateInfo;
	if (!IsValid(PlayStateInfo))
	{
		return false;
	}

	if (IsValid(Btn_Pause))
	{
		Btn_Pause->SetVisibility(ESlateVisibility::Visible);

		Btn_Pause->OnClicked.RemoveDynamic(this, &URBattleOptionMenu::OnOptionMenuOpen);
		Btn_Pause->OnClicked.AddDynamic(this, &URBattleOptionMenu::OnOptionMenuOpen);

	}

	if (IsValid(Button_Exit))
	{
		Button_Exit->OnClicked.RemoveDynamic(this, &URBattleOptionMenu::OnClickExit);
		Button_Exit->OnClicked.AddDynamic(this, &URBattleOptionMenu::OnClickExit);

		Button_Exit->SetIsEnabled(true);
	}

	if (IsValid(Button_Setting))
	{
		Button_Setting->OnClicked.RemoveDynamic(this, &URBattleOptionMenu::OptionButton);
		Button_Setting->OnClicked.AddDynamic(this, &URBattleOptionMenu::OptionButton);
	}

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &URBattleOptionMenu::OnOptionMenuClose);
		Button_Close->OnClicked.AddDynamic(this, &URBattleOptionMenu::OnOptionMenuClose);
	}

	if (IsValid(Button_Resume))
	{
		Button_Resume->OnClicked.RemoveDynamic(this, &URBattleOptionMenu::OnOptionMenuClose);
		Button_Resume->OnClicked.AddDynamic(this, &URBattleOptionMenu::OnOptionMenuClose);
	}

	
	return true;
}

void URBattleOptionMenu::ExitButton()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	UUP_CommonPopup* Popup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_ASK_BATTLE_QUIT), ECommonPopupType::VE_OKCANCEL);

	if (IsValid(Popup))
	{
		Popup->OnClickOK.Clear();
		Popup->OnClickOK.AddDynamic(this, &URBattleOptionMenu::ExitButtonOK);
		Popup->OnClickCancel.Clear();
		Popup->OnClickCancel.AddDynamic(this, &URBattleOptionMenu::ExitButtonCancel);
	}

}

void URBattleOptionMenu::ExitButtonOK()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	AProjectRGameMode* RGameMode = UUtilFunctionLibrary::GetGamePlayMode();

	if (Cast<AArenaRGameMode>(RGameMode))
	{
		RGameInstance->EventManager->ClearEvent();
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
 		RGameInstance->ArenaManager->RequestPlayDone();
 		RGameInstance->ArenaManager->RequestStatus();
 		RGameInstance->ArenaManager->RequestRankigList();
	}
	else if(Cast<AMirrorGameMode>(RGameMode))
		RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_MirrorDungeon);
	else if (Cast<ARaidRGameMode>(RGameMode))
	{
		URTSManager::GetInstancePtr()->REQ_BT_EXIT();
		RGameInstance->RealTimeModManager->ClearData();
		RGameInstance->RealTimeModManager->OnExitGame();
	}
	GoContentsLobby();
}

void URBattleOptionMenu::GoContentsLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager) && IsValid(RGameInstance->EventManager))
	{
		RGameInstance->RWidgetManager->UIEventQueue.Reset();
		switch (RGameInstance->CurrentGameMode)
		{
		case EGamePlayModeTypeEnum::VE_SingleRaid:
		{
			if (IsValid(RGameInstance->RaidManager))
			{
				RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_SingleRaid);
			}
		}
		break;
		case EGamePlayModeTypeEnum::VE_Campaign:
		{
			if (IsValid(RGameInstance->CampaignManager))
			{

				RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Campaign);
			}
		}
		break;
		case EGamePlayModeTypeEnum::VE_HeroEpisode:
		{
			if (IsValid(RGameInstance->HeroRuneDungeonManager))
			{
				if (RGameInstance->HeroRuneDungeonManager->IsRetry())
				{
					RGameInstance->HeroRuneDungeonManager->EndRetry();
				}
				RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_HeroRune);
				RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_HeroRuneSelect);
			}
		}
		break;
		case EGamePlayModeTypeEnum::VE_Colosseum:
			RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Colosseum);
		break;

		case EGamePlayModeTypeEnum::VE_Arena:
			RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_Arena);
		break;

		case EGamePlayModeTypeEnum::VE_BattleGround:
			RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_BattleGround);
		break;
		case EGamePlayModeTypeEnum::VE_MirrorDungeon:
			RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_MirrorDungeon);
		break;
		}

		/*Temp Added because of lighting issues*/
		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);

		RGameInstance->EventManager->ClearEvent();
		if(RGameInstance->CurrentGameMode != EGamePlayModeTypeEnum::VE_Arena)
			RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
		UGamePlayBluePrintFunction::SetPause(false);
		ExitButtonCancel();
	}
}

void URBattleOptionMenu::ExitButtonCancel()
{
	
}

void URBattleOptionMenu::OptionButton()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Setting);
	UUIFunctionLibrary::InvalidateJoystick(RGameInstance->GetWorld());
}

void URBattleOptionMenu::PauseButton()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (URealTimeModManager::IsRealTimeGameMode())
	{
		return;
	}
		 
	//bool TogglePause = !(UGameplayStatics::IsGamePaused(BasePlayerController));
	UGamePlayBluePrintFunction::SetPause(true);

	// Jaeyoung
	if (IsValid(RGameInstance->RWidgetManager))
	{
		if (RGameInstance->RWidgetManager->IsShowUserWidget(EWidgetBluePrintEnum::WBP_Chat))
		{
			RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_Chat);
		}
	}


	if (IsValid(Btn_Pause))
	{
		Btn_Pause->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URBattleOptionMenu::ResumeButton()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (URealTimeModManager::IsRealTimeGameMode())
	{
		return;
	}
	UGamePlayBluePrintFunction::SetPause(false);
	if (IsValid(Btn_Pause))
	{
		Btn_Pause->SetVisibility(ESlateVisibility::Visible);
	}
}

void URBattleOptionMenu::IsPrevProcess()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (!IsValid(RGameInstance->RWidgetManager))
		return;
	if (!IsValid(Btn_Pause))
		return;

	if (ESlateVisibility::Collapsed == Btn_Pause->GetVisibility())
	{
		if (RGameInstance->RWidgetManager->IsShowUserWidget(EWidgetBluePrintEnum::WBP_Setting))
		{
			UUserWidget* UserWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Setting);
			if (IsValid(UserWidget))
			{
				UUP_Setting* SettingWidget = Cast<UUP_Setting>(UserWidget);
				if (IsValid(SettingWidget))
				{
					SettingWidget->OnClick_ButtonCancel();
				}
			}
		}
		else
		{
			OnOptionMenuClose();
		}
	}
	else
	{
		OnOptionMenuOpen();
	}
}

void URBattleOptionMenu::OnClickExit()
{
	//UGameplayStatics::PlaySound2D();
	ExitButton();

}

void URBattleOptionMenu::OnOptionMenuOpen()
{
	//UGameplayStatics::PlaySound2D()
	PauseButton();
	PlayAni("Toggle");
	Switcher_IconImage->SetActiveWidgetIndex(1);
}

void URBattleOptionMenu::OnOptionMenuClose()
{
	//UGameplayStatics::PlaySound2D()
	Popup_Option->SetVisibility(ESlateVisibility::Collapsed);
	BackImage->SetVisibility(ESlateVisibility::Collapsed);
	
	ResumeButton();
	PlayAni("Toggle",true);

	Switcher_IconImage->SetActiveWidgetIndex(0);
}
