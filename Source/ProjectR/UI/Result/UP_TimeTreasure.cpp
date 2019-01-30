// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_TimeTreasure.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "Network/PacketFunctionLibrary.h"

#include "GameInfo/UserInfo.h"

#include "UI/RWidgetManager.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/Common/UC_Item_Icons.h"



void UUP_TimeTreasure::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Close)
	{
		Button_Close->OnClicked.Clear();
		Button_Close->OnClicked.AddDynamic(this, &UUP_TimeTreasure::OnClick_ButtonClose);
	}

	if (Button_OK)
	{
		Button_OK->OnClicked.Clear();
		Button_OK->OnClicked.AddDynamic(this, &UUP_TimeTreasure::OnClick_ButtonOK);
	}

	if (Button_Preview)
	{
		Button_Preview->OnClicked.Clear();
		Button_Preview->OnClicked.AddDynamic(this, &UUP_TimeTreasure::OnClick_ButtonPreview);
	}

}

void UUP_TimeTreasure::OnClick_ButtonClose()
{
	URGameInstance* RGameInst = RGAMEINSTANCE(this);

	URWidgetManager* WidgetManager = RGameInst->RWidgetManager;
	if (WidgetManager)
	{
		UTopBar* TopBar = WidgetManager->GetUserWidgetT<UTopBar>(EWidgetBluePrintEnum::WBP_TopBar);
		if (TopBar)
			TopBar->PrevButton();
	}

}

void UUP_TimeTreasure::OnClick_ButtonOK()
{
	UPacketFunctionLibrary::INVENTORY_TIMETREASURE_GET_RQ();
}

void UUP_TimeTreasure::OnClick_ButtonPreview()
{
	URGameInstance* RGameInst = RGAMEINSTANCE(this);
	UTableManager* TableManager = UTableManager::GetInstancePtr();

	FAVATAR Avatar = RGameInst->UserInfo->GetAvatarData();

	int32 iElevatedFloor = 1;// Avatar.elevatedFloor;
	TArray<FTimeTreasureTable*> TTArray = FTimeTreasureTable::LookUp(iElevatedFloor);

	
	URWidgetManager* WidgetManager = RGameInst->RWidgetManager;
	if (WidgetManager)
	{
		UUP_RewardPopup* RewardPopup = UUIFunctionLibrary::ShowRewardPopup();

		for (size_t i = 0; i < TTArray.Num(); ++i)
		{
			FWalletTableInfo* WalletInfo = TableManager->GetWalletRow(TTArray[i]->RewardImageIndex);
			if (!WalletInfo)
				continue;

			FName IconKey = WalletInfo->ItemIconKey;
			FString BottomStr = TEXT("MAX ") + FString::FromInt(TTArray[i]->RewardAmount_Max);

			FString TimeText = WalletInfo->RefillPeriod >= 3600 ? TEXT("H") : TEXT("M");
			FString TopStr = TEXT("+") + FString::FromInt(WalletInfo->RefillAmount * TTArray[i]->RewardAmount_Rate) + TEXT(" / ") + TimeText;

			RewardPopup->ItemIconList[i]->Init_TimeTreasureReward(WalletInfo->ItemIconKey, FText::FromString(TopStr), FText::FromString(BottomStr));
			RewardPopup->ItemPanelList[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			FText txt = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, WalletInfo->CommonStringKey);
			RewardPopup->ItemTextBlockList[i]->SetText(txt);
		}

		
		
	}
}
