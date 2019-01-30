// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_Dice.h"

#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/RWidgetManager.h"
#include "Network/RTSManager.h"

#define MAX_DiceCount		3

void UUC_HUD_Dice::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_DiceNumber, TEXT("DiceNum_"));
	FindChildWidgetsWithCertainName<UImage>(this, Image_DiceNumber, TEXT("DiceImg_"));

	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR)) return;

	/*for (int32 n = 0; n < MAX_DiceCount; ++n)
	{
		FlipBookActiveTime.Emplace(0.0f);
	}*/
	FlipBookActiveTime.SetNum(MAX_DiceCount);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
// 		RGameInstance->EventManager->OnModRoomGuildRaidDiceReady.RemoveDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDiceReady);
// 		RGameInstance->EventManager->OnModRoomGuildRaidDiceReady.AddDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDiceReady);
// 		RGameInstance->EventManager->OnModRoomGuildRaidDice.RemoveDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDice);
// 		RGameInstance->EventManager->OnModRoomGuildRaidDice.AddDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDice);
// 		RGameInstance->EventManager->OnModRoomGuildRaidDicePaidReward.RemoveDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDicePaidReward);
// 		RGameInstance->EventManager->OnModRoomGuildRaidDicePaidReward.AddDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDicePaidReward);
		RGameInstance->TimerManager->ClearTimer(DiceBidding_TimeHandler);
	}
}

void UUC_HUD_Dice::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
//		RGameInstance->EventManager->OnModRoomGuildRaidDiceReady.RemoveDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDiceReady);
//		RGameInstance->EventManager->OnModRoomGuildRaidDice.RemoveDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDice);
//		RGameInstance->EventManager->OnModRoomGuildRaidDicePaidReward.RemoveDynamic(this, &UUC_HUD_Dice::OnModRoomGuildRaidDicePaidReward);
		RGameInstance->TimerManager->ClearTimer(DiceBidding_TimeHandler);
	}

	Super::NativeDestruct();
}

void UUC_HUD_Dice::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DiceActive)
	{
		UpdateFlipBook(InDeltaTime);
	}

	if (DiceBiddingReady)
	{
		if (DiceBidding_TimeHandler.IsValid())
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			
			{
				float RemainTime = RGameInstance->GetTimerManager().GetTimerRemaining(DiceBidding_TimeHandler);
				if (IsValid(DiceEndTime))
				{
					DiceEndTime->SetText(FText::AsNumber(FMath::CeilToInt(RemainTime)));
				}
			}
		}
	}
}

void UUC_HUD_Dice::OnModRoomGuildRaidDiceReady(int32 InIndex)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	ReadyToRewardUI = false;
	DiceBiddingReady = true;
	KillBossRewardIndex = InIndex;

	float DiceDelay = 0.1f;
// 	auto RealTimeModDSN = RGameInstance->RealTimeModManager->GetRealTimeModDSN();
// 	FName ModeID = FName(*(RealTimeModDSN.modId));
// 	auto TableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(ModeID);
// 	if (TableInfo)
// 	{
// 		DiceDelay = TableInfo->DiceDelay;
// 	}
// 
// 	if (IsValid(UC_Item_Icons))
// 	{
// 		TMap<int32, FREALTIME_REWARD>& RewardMap = RGameInstance->RealTimeModManager->KillBossRewardItem;
// 		if (RewardMap.Contains(KillBossRewardIndex))
// 		{
// 			FREALTIME_REWARD RealTimeReward = RewardMap[KillBossRewardIndex];
// 			for (FREWARD& RewardData : RealTimeReward.rewards)
// 			{
// 				FITEM_ICON_INFO RewardItemIconInfo;
// 				UUIFunctionLibrary::GetRewardIconByRewardType(RewardData, RewardItemIconInfo);
// 				UC_Item_Icons->SetItemIconStructure(RewardItemIconInfo);
// 
// 				if (IsValid(TextItemName))
// 				{
// 					TextItemName->SetText(UUIFunctionLibrary::GetRewardText(RewardData));
// 				}
// 			}
// 		}
// 	}

	if (IsValid(PanelDiceEndTime))
	{
		PanelDiceEndTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsValid(DiceEndTime))
	{
		DiceEndTime->SetText(FText::AsNumber((int32)DiceDelay));
	}

	if (IsValid(Switcher_Binding))
	{
		Switcher_Binding->SetActiveWidgetIndex(0);
	}

	if (IsValid(Button_Giveup))
	{
		Button_Giveup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	for (auto DiceImage : Image_DiceNumber)
	{
		DiceImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	RGameInstance->TimerManager->ClearTimer(DiceBidding_TimeHandler);
	RGameInstance->TimerManager->SetTimer(DiceBidding_TimeHandler, this, &UUC_HUD_Dice::EndDiceTimer, DiceDelay, false);
}

void UUC_HUD_Dice::EndDiceTimer()
{
	if (!DiceBiddingReady)
		return;

	OnClick_Bidding(true);
}

void UUC_HUD_Dice::OnClick_Bidding(bool Participate)
{
	if (!DiceBiddingReady)
		return;

	DiceBiddingReady = false;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (IsValid(PanelDiceEndTime))
	{
		PanelDiceEndTime->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(Switcher_Binding))
	{
		Switcher_Binding->SetActiveWidgetIndex(1);
	}

	if (IsValid(Button_Giveup))
	{
		Button_Giveup->SetVisibility(ESlateVisibility::Collapsed);
	}

	RGameInstance->TimerManager->ClearTimer(DiceBidding_TimeHandler);
	
	//TODO: must delete
// #if WITH_PROUD_NET_BINDING
// 	UProudNetManager::GetInstancePtr()->m_ModRoomProxy.Req_GuildRaid_Dice(Proud::HostID::HostID_Server, Proud::RmiContext::ReliableSend, KillBossRewardIndex, Participate);
// #endif
//
// 	if (Participate == false)
// 	{
// 		RGameInstance->EventManager->OnModRoomGuildRaidDiceEnd.Broadcast();
// 	}
}

void UUC_HUD_Dice::OnModRoomGuildRaidDice(int32 DiceScore)
{
	TArray<int32> NumPlace;
	int32 HundredPlace = DiceScore / 100;
	NumPlace.Add(HundredPlace);
	int32 TensPlace = (DiceScore % 100) / 10;
	NumPlace.Add(TensPlace);
	int32 OnePlace = DiceScore % 10;
	NumPlace.Add(OnePlace);

	int32 CheckIndex = 0;
	for (auto DiceText : Text_DiceNumber)
	{
		if (IsValid(DiceText))
		{
			int32 DiceNum = 0;
			if (NumPlace.IsValidIndex(CheckIndex))
			{
				DiceNum = NumPlace[CheckIndex];
			}
			DiceText->SetText(FText::AsNumber(DiceNum));
		}
		++CheckIndex;
	}
	
	DiceActive = true;
	DiceActiveTime = 0.0f;
	for (auto& ActiveTime : FlipBookActiveTime)
	{
		ActiveTime = 0.0f;
	}

	UpdateFlipBook(0.0f);
}

void UUC_HUD_Dice::UpdateFlipBook(float InDeltaTime)
{
	if (!DiceActive)
		return;

	DiceActiveTime += InDeltaTime;

	bool FlipBookActive = false;

	for (int n = 0; n < MAX_DiceCount; ++n)
	{
		if (!FlipBookExecuteTime.IsValidIndex(n) || !FlipBookActiveTime.IsValidIndex(n) || !Image_DiceNumber.IsValidIndex(n))
			continue;

		if (FlipBookTime < FlipBookActiveTime[n])
			continue;

		float ScalarParam = 0.0f;
		if (0.0f < DiceActiveTime)
		{
			if (FlipBookExecuteTime[n] <= DiceActiveTime)
			{
				FlipBookActiveTime[n] += InDeltaTime;
				ScalarParam = FlipBookActiveTime[n] / FlipBookTime;
			}
		}
		
		auto DiceImage = Image_DiceNumber[n];
		if (IsValid(DiceImage))
		{
			auto DynamicMaterial = DiceImage->GetDynamicMaterial();
			if (IsValid(DynamicMaterial))
			{
				//UE_LOG(LogNetwork, Log, TEXT("Dice Number : %d ScalarParam %f"), n, ScalarParam);

				if (1.0f < ScalarParam)
				{
					ScalarParam = 1.0f;
					DiceImage->SetVisibility(ESlateVisibility::Collapsed);
				}

				DynamicMaterial->SetScalarParameterValue(ScalarParmName, ScalarParam);
			}
		}

		FlipBookActive = true;
	}

	if (false == FlipBookActive)
	{
		DiceActive = false;

		if (ReadyToRewardUI)
		{
			ShowDiceReward();
		}
	}
}

void UUC_HUD_Dice::OnModRoomGuildRaidDicePaidReward()
{
	ReadyToRewardUI = true;
	
	if (!DiceActive)
	{
		ShowDiceReward();
	}
}

void UUC_HUD_Dice::ShowDiceReward()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UUIFunctionLibrary::ShowRewardPopupSplash(
		ERewardSplashType::VE_GuildRaid,
		RGameInstance->RealTimeModManager->DiceOwnerName,
		RGameInstance->RealTimeModManager->DiceRewardName,
		RGameInstance->RealTimeModManager->DiceRewardData.rewards);

	ReadyToRewardUI = false;
}