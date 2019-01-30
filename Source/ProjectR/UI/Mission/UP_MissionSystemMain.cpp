

#include "ProjectR.h"
#include "UP_MissionSystemMain.h"
#include "UI/Control/RScrollView.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/UI_Common/UP_RewardPopup.h"

#include "Global/RGameInstance.h"
#include "Global/MissionManager.h"
#include "Global/TableManager.h"
#include "UC_MissionScrollItem.h"
#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketFunctionLibrary.h"
#include "UC_FloorMissionReward_Popup.h"
#include "GameInfo/UserInfo.h"


#define MISSION_COUNT 3

void UUP_MissionSystemMain::NativeConstruct()
{
	Super::NativeConstruct();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	FindChildWidgetsWithCertainName<UUC_MissionScrollItem>(this, ScrollItemList, TEXT("UC_MissionScrollItem_"));

	RGameInstance->MissionManager->OnMissionStatusRp.RemoveDynamic(this, &UUP_MissionSystemMain::OnMissionStatusRp);
	RGameInstance->MissionManager->OnMissionStatusRp.AddDynamic(this, &UUP_MissionSystemMain::OnMissionStatusRp);

	RequestRewardButton->OnClicked.RemoveDynamic(this, &UUP_MissionSystemMain::OnClickDayTotalRewardButton);
	RequestRewardButton->OnClicked.AddDynamic(this, &UUP_MissionSystemMain::OnClickDayTotalRewardButton);

	RGameInstance->MissionManager->OnMissionRewardGetRp.RemoveDynamic(this, &UUP_MissionSystemMain::OnMissionRewardGetRp);
	RGameInstance->MissionManager->OnMissionRewardGetRp.AddDynamic(this, &UUP_MissionSystemMain::OnMissionRewardGetRp);

	SetMissionStateText();
	InvalidateMission();
	SetBotPanel();
}

void UUP_MissionSystemMain::NativeDestruct()
{
// 	RGAMEINSTANCE(this)->MissionManager->OnMissionStatusRp.RemoveDynamic(this, &UUP_MissionSystemMain::OnMissionStatusRp);
// 	RequestRewardButton->OnClicked.RemoveDynamic(this, &UUP_MissionSystemMain::OnClickDayTotalRewardButton);
	Super::NativeDestruct();
}

void UUP_MissionSystemMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ResetTimeTick(InDeltaTime);
}


void UUP_MissionSystemMain::OnMissionStatusRp()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	//SetMissionStateText();
	InvalidateMission();
	SetBotPanel();
}

void UUP_MissionSystemMain::OnMissionRewardGetRp(TArray<FREWARD>& InRewards, TArray<FREWARD>& InExtraRewards)
{
	UUP_RewardPopup* pRewardPopup = UUIFunctionLibrary::ShowCommonRewardPopupByRewards(InRewards);
	if(0 < InExtraRewards.Num())
	{
		ExtraRewards = InExtraRewards;
		if (pRewardPopup)
		{
			pRewardPopup->VerticalBox_QuestGuide->OnClicked.RemoveDynamic(this, &UUP_MissionSystemMain::OnCloseRewardPopup);
			pRewardPopup->VerticalBox_QuestGuide->OnClicked.AddDynamic(this, &UUP_MissionSystemMain::OnCloseRewardPopup);
		}
	}
}

void UUP_MissionSystemMain::OnCloseRewardPopup()
{
	FTimerDelegate ContentsCallback;
	auto RGameInstance = RGAMEINSTANCE(this);
	ContentsCallback.BindLambda([=]
	{
		UUIFunctionLibrary::ShowCommonRewardPopupByRewards(ExtraRewards);
		ExtraRewards.Empty();
	});
	RGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(ContentsCallback);
	
}

void UUP_MissionSystemMain::OnClickDayTotalRewardButton()
{
	FFLOOR_MISSION_STATUS missionStatus = RGAMEINSTANCE(this)->MissionManager->GetMissionStatus();
	if(missionStatus.step != 5)
	{
		UC_Mission_popup->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUP_MissionSystemMain::SetMissionStateText()
{
	UMissionManager* MissionManager = RGAMEINSTANCE(this)->MissionManager;

	if (!MissionManager)
		return;

	FText FloorText = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI,TEXT("UI_DailyMission_Floor")), MissionManager->GetMissionStatus().floor);
	CurrentFloorText->SetText(FloorText);
	//SetTimeText(nRemainTime);
}

void UUP_MissionSystemMain::SetTimeText(int32 InRemainTime)
{
	if (0 < InRemainTime)
	{
		int32 nHour = 0;
		int32 nMin = 0;
		int32 nSec = 0;

		nHour = InRemainTime / 3600;
		nMin = (InRemainTime % 3600) / 60;
		nSec = InRemainTime % 60;

		FText HourText = UUIFunctionLibrary::Get_0n_Number(nHour);
		FText MinText = UUIFunctionLibrary::Get_0n_Number(nMin);
		FText SecText = UUIFunctionLibrary::Get_0n_Number(nSec);

		FText TimeText = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_DailyMission_Timer")), HourText, MinText, SecText);

		RemainTimeText->SetText(FText::Format(FText::FromString(TEXT("{0} {1}")), TimeText, CountText));
		RemainTimeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	}
	else
	{
		RemainTimeText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_MissionSystemMain::InvalidateMission()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UMissionManager* MissionManager = RGameInstance->MissionManager;

	FFLOOR_MISSION_STATUS currentStatus = MissionManager->GetMissionStatus();

	for (int32 i = 0; i < MISSION_COUNT; i++)
	{
		if (i < currentStatus.missions.Num())
		{
			ScrollItemList[i]->missionData = currentStatus.missions[i];
			ScrollItemList[i]->InvalidateData();
			ScrollItemList[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			ScrollItemList[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUP_MissionSystemMain::SetBotPanel()
{
	FFLOOR_MISSION_STATUS MissionStatus = RGAMEINSTANCE(this)->MissionManager->GetMissionStatus();
	CurrentProgressText->SetText(FText::AsNumber(MissionStatus.step));
	MaxProgressText->SetText(FText::AsNumber(5));
	float fPercent = (float)MissionStatus.step / 5.0f;
	MissionProgressBar->SetPercent(fPercent);

	if (MissionStatus.step < 5)
	{
		RequestRewardButton->SetIsEnabled(true);
		RewardButtonText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Button_TimeTreasure_RewardInfo")));
	}
	else
	{
		RequestRewardButton->SetIsEnabled(false);
		RewardButtonText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_DailyMission_Button01")));
	}
}

void UUP_MissionSystemMain::OnReceiveMissionButtonClick()
{
	FText popuptext = FText::Format( FText::FromString(TEXT("{0}\n{1}")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage01")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage02")) );

	UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
																		popuptext, ECommonPopupType::VE_OKCANCEL);

	if (commonpopup)
	{
		commonpopup->OnClickOK.RemoveDynamic(this, &UUP_MissionSystemMain::ReceiveMissionOK);
		commonpopup->OnClickOK.AddDynamic(this, &UUP_MissionSystemMain::ReceiveMissionOK);
	}
}

void UUP_MissionSystemMain::ReceiveMissionOK()
{
// 	UPacketFunctionLibrary::DAILYMISSION_STATUS_RQ(true, nCurrentFloor);
}

void UUP_MissionSystemMain::OnNewMissionButtonClick()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nHaveCount = RGameInstance->RInventory->GetMISSION_ADD_SCROLL();
	if (nHaveCount <= 0)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_PopUpMessage03")), ECommonPopupType::VE_OK);
		return;
	}

// 	if (MissionScrollView->GetScrollItemDataCount() == MAX_MISSION_COUNT)
// 	{
// 		FText text = FText::Format(FText::FromString(TEXT("{0}\n{1}")),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_PopUpMessage03")),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_PopUpMessage04")));
// 
// 		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 			text, ECommonPopupType::VE_OK);
// 
// 		return;
// 	}

	FText popuptext = FText::Format(FText::FromString(TEXT("{0}\n{1}")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage03")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage04")));

	UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		popuptext, ECommonPopupType::VE_OKCANCEL);

	if (commonpopup)
	{
		commonpopup->SetCurrency(WALLET_MISSION_ADD_SCROLL, 1);
		commonpopup->OnClickOK.RemoveDynamic(this, &UUP_MissionSystemMain::NewMissionOK);
		commonpopup->OnClickOK.AddDynamic(this, &UUP_MissionSystemMain::NewMissionOK);
	}

}

void UUP_MissionSystemMain::NewMissionOK()
{
// 	UPacketFunctionLibrary::DAILYMISSION_ADD_RQ();
}

void UUP_MissionSystemMain::OnRefreshSpecialButtonClick()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FString> IDList = RGameInstance->MissionManager->GetSelectedIDList();
	if (IDList.Num() <= 0)
	{
		UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_PopUpMessage02")), ECommonPopupType::VE_OK);
		return;
	}

	auto TableInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("DailyMissionResetSpecialGem")));
	checkf(TableInfo != nullptr, TEXT("[%s]TableInfo is nullptr"), __FUNCTION__);

	int32 nNeedGemCount = 0;
	if (false == TableInfo->GetValue(nNeedGemCount))
		return;

	int32 nHaveGemCount = RGameInstance->RInventory->GetGem();
	if (nHaveGemCount < nNeedGemCount)
	{
		UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Currency_Warning_Gem")), ECommonPopupType::VE_OK);
		return;
	}

	FText popuptext = FText::Format(FText::FromString(TEXT("{0}\n{1}")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage05")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage06")));

	UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		popuptext, ECommonPopupType::VE_OKCANCEL);

	if (commonpopup)
	{
		commonpopup->SetCurrency(WALLET_GEM, nNeedGemCount);
		commonpopup->OnClickOK.RemoveDynamic(this, &UUP_MissionSystemMain::RefreshSpecialOK);
		commonpopup->OnClickOK.AddDynamic(this, &UUP_MissionSystemMain::RefreshSpecialOK);
	}
}

void UUP_MissionSystemMain::RefreshSpecialOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FString> IDList = RGameInstance->MissionManager->GetSelectedIDList();
//  	UPacketFunctionLibrary::DAILYMISSION_RESET_RQ(IDList, true);
}

void UUP_MissionSystemMain::OnRefreshNormalButtonClick()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FString> IDList = RGameInstance->MissionManager->GetSelectedIDList();
	if (IDList.Num() <= 0)
	{
		UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_PopUpMessage02")), ECommonPopupType::VE_OK);
		return;
	}

	auto TableInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("DailyMissionResetNormalGem")));
	checkf(TableInfo != nullptr, TEXT("[%s]TableInfo is nullptr"), __FUNCTION__);

	int32 nNeedGemCount = 0;
	if (false == TableInfo->GetValue(nNeedGemCount))
		return;

	int32 nHaveGemCount = RGameInstance->RInventory->GetGem();
	if (nHaveGemCount < nNeedGemCount)
	{
		UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Currency_Warning_Gem")), ECommonPopupType::VE_OK);
		return;
	}

	FText popuptext = FText::Format(FText::FromString(TEXT("{0}\n{1}")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage07")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("DailyMission_ConfirmMessage08")));

	UUP_CommonPopup* commonpopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		popuptext, ECommonPopupType::VE_OKCANCEL);

	if (commonpopup)
	{
		commonpopup->SetCurrency(WALLET_GEM, nNeedGemCount);
		commonpopup->OnClickOK.RemoveDynamic(this, &UUP_MissionSystemMain::RefreshNormalOK);
		commonpopup->OnClickOK.AddDynamic(this, &UUP_MissionSystemMain::RefreshNormalOK);
	}
}

void UUP_MissionSystemMain::RefreshNormalOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FString> IDList = RGameInstance->MissionManager->GetSelectedIDList();

// 	UPacketFunctionLibrary::DAILYMISSION_RESET_RQ(IDList, false);
}

void UUP_MissionSystemMain::ResetTimeTick(float InDeltaTime)
{
// 	const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
// 	// 		nCurrentSeasonOutTime += InDeltaTime;
// 
// 	if(0 < nRemainTime)
// 	{
// 		if (CurrentTime.GetTicks() - nCurrentCheckTime >= FTimespan::FromSeconds(1.0f).GetTicks())
// 		{
// 			nCurrentCheckTime = CurrentTime.GetTicks();
// 
// 			--nRemainTime;
// 
// 			SetTimeText(nRemainTime);
// 
// 			bool bTimeEnd = nRemainTime <= 0;
// 			if (bTimeEnd)
// 			{
// 				UPacketFunctionLibrary::DAILYMISSION_STATUS_RQ(false, 1);
// 			}
// 
// 		}
// 	}
}
