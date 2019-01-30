// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UtilFunctionIntegrated.h"

#include "Global/TableManager.h"

#include "Table/CurrencyPropertyTableInfo.h"
#include "Table/ItemTableInfo.h"
#include "Table/ShopProductTableInfo.h"

#include "UC_Splash_FloorShop_Item.h"


void UUC_Splash_FloorShop_Item::NativeConstruct()
{	
	Super::NativeConstruct();
}


void UUC_Splash_FloorShop_Item::NativeDestruct()
{
	Super::NativeDestruct();	
}

void UUC_Splash_FloorShop_Item::SetItem(FREWARD& InReward)
{
	FITEM_ICON_INFO RewardItemIconInfo;
	UUIFunctionLibrary::GetRewardIconByRewardType(InReward, RewardItemIconInfo);

	if (IsValid(Item_Icon))
		Item_Icon->SetItemIconStructure(RewardItemIconInfo);

	switch (InReward.type)
	{
		case EREWARD_TYPE::ITEM:
		{
			FItemTableInfo*		ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InReward.rewardId);
			checkf(ItemTableInfo != nullptr, TEXT("[%s]ItemTableInfo is Empty"), __FUNCTION__);
			if (IsValid(Text_ItemName))
				Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName));
			break;
		}
		case EREWARD_TYPE::WALLET:
		{
			FWalletTableInfo* WalletTableInfo = UTableManager::GetInstancePtr()->GetWalletRow(InReward.rewardId);
			checkf(WalletTableInfo != nullptr, TEXT("[%s]WalletTableInfo is Empty"), __FUNCTION__);
			if (IsValid(Text_ItemName))
				Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, WalletTableInfo->CommonStringKey));
			break;
		}
	}

}

FReply UUC_Splash_FloorShop_Item::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	return FReply::Unhandled();
}

