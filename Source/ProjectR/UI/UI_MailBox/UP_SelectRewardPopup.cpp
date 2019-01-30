// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_SelectRewardPopup.h"
#include "GlobalIntegrated.h"

#include "UI/Common/SelectRewardScrollItemData.h"
#include "UI/Common/UC_Item_Icon_with_Name.h"
#include "UI/Control/RScrollView.h"
#include "UtilFunctionIntegrated.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

void UUP_SelectRewardPopup::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAni("Popup_Appear");

	if (SelectOKButton)
	{
		SelectOKButton->OnClicked.RemoveDynamic(this, &UUP_SelectRewardPopup::OnClickReceive);
		SelectOKButton->OnClicked.AddDynamic(this, &UUP_SelectRewardPopup::OnClickReceive);
	}

	if (Button_Close)
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUP_SelectRewardPopup::OnClickCancel);
		Button_Close->OnClicked.AddDynamic(this, &UUP_SelectRewardPopup::OnClickCancel);
	}
	
	if (ItemScrollView)
	{
		ItemScrollView->Init();
		ItemScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUP_SelectRewardPopup::OnSelectItem);
		ItemScrollView->OnScrollViewSelectedItem.AddDynamic(this, &UUP_SelectRewardPopup::OnSelectItem);
	}
	
}

void UUP_SelectRewardPopup::NativeDestruct()
{
	ItemScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUP_SelectRewardPopup::OnSelectItem);

	Super::NativeDestruct();
}

void UUP_SelectRewardPopup::OnClickReceive()
{
	OnSelectRewardConfirmOK();

	PlayAni("Popup_Appear",true);

	
}

void UUP_SelectRewardPopup::OnSelectRewardConfirmOK()
{
	TArray<FString> uds;
	uds.Emplace(CurrentMailUD);
	UPacketFunctionLibrary::SPECIAL_GIFT_OPEN_RQ(uds, CurrentSelected);
}

void UUP_SelectRewardPopup::OnClickCancel()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->MailManager->CleanReceiveList();//AddReceiveList(this->ItemData);
	}

	PlayAni("Popup_Appear", true);
}

void UUP_SelectRewardPopup::Init(const FString& InMailUD, const TArray<FREWARD>& InRewards)
{
	ItemScrollView->Init();
	ItemScrollView->ClearScrollItemData();
	CurrentMailUD = InMailUD;
	CurrentSelected = -1;
	if (IsValid(SelectOKButton))
		SelectOKButton->SetIsEnabled(false);

	int dataIndex = 0;
	for (FREWARD rewardItem : InRewards)
	{
		USelectRewardScrollItemData* RewardItemData = Cast<USelectRewardScrollItemData>(ItemScrollView->AddScrollItemData(USelectRewardScrollItemData::StaticClass()));
		FITEM_ICON_INFO iconInfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(rewardItem, iconInfo);
		
		if (IsValid(RewardItemData))
		{
			RewardItemData->SetData(iconInfo, dataIndex);
			RewardItemData->SetItemName(UUIFunctionLibrary::GetRewardText(rewardItem));
		}
		++dataIndex;
	}

	ItemScrollView->InvalidateData();
}

void UUP_SelectRewardPopup::OnSelectItem(URScrollItem* selectItem)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	if(IsValid(ItemScrollView))
	{
		int32 Max = ItemScrollView->GetScrollItemAll().Num();
		for (int32 i = 0; i < Max; ++i)
		{
			UUC_Item_Icon_with_Name* pSelectedItem = static_cast<UUC_Item_Icon_with_Name*>(ItemScrollView->GetScrollItem(i));
			if (pSelectedItem)
				pSelectedItem->SetSelectedItem(false);
		}
	}

	if (IsValid(selectItem))
	{
		UUC_Item_Icon_with_Name* pSelectedItem = static_cast<UUC_Item_Icon_with_Name*>(selectItem);
		if(IsValid(pSelectedItem))
		{
			pSelectedItem->SetSelectedItem(true);
			auto SelectedItemData = static_cast<USelectRewardScrollItemData*>(selectItem->ItemData);
			if (SelectedItemData)
			{
				CurrentSelected = SelectedItemData->ItemOrderIndex;
				if (IsValid(SelectOKButton))
					SelectOKButton->SetIsEnabled(true);
			}
		}
	}
}