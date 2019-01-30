// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_Item_Sell.h"
#include "UI/ItemManagement/UP_GenericSplash.h"
#include "UI/ItemManagement/UC_RewardsSplash.h"
#include "Network/PacketFunctionLibrary.h"
#include "UIFunctionLibrary.h"

void UUC_Popup_Item_Sell::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Cancel)
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_Item_Sell::OnButtonCancelClicked);
	if (Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_Item_Sell::OnButtonCancelClicked);
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUC_Popup_Item_Sell::OnButtonOKClicked);
}

void UUC_Popup_Item_Sell::NativeDestruct()
{
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_OK))
		Button_OK->OnClicked.RemoveAll(this);
	if (IsValid(Button_Close))
		Button_Close->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_Popup_Item_Sell::SetData(TArray<FString> selectedItemList, TArray<FName> selectedWalletKeyList, TArray<int32> selectedWalletAmountList)
{
	SelectedItemUDs.Reset();
	SelectedItemUDs = selectedItemList;
	SelectedWalletKeys = selectedWalletKeyList;
	SelectedWalletAmounts = selectedWalletAmountList;
}

void UUC_Popup_Item_Sell::OnButtonCancelClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Popup_Item_Sell::OnButtonOKClicked()
{
	// Do the batch sell
	check(SelectedItemUDs.Num() + SelectedWalletKeys.Num() != 0);

	RGAMEINSTANCE(this)->HttpClient->OnInventorySell.AddUObject(this, &UUC_Popup_Item_Sell::OnInventorySellRp);

	// adapt to TArray<FString>
	TArray<FString> WalletIdList;
	for (FName WalletKey : SelectedWalletKeys)
	{
		// TODO: dubious. server expects case-sensitive keys. but FName are case-insensitive. Thank you Epic.
		WalletIdList.Emplace(WalletKey.ToString());
	}

	UPacketFunctionLibrary::INVENTORY_ITEM_GRIND_RQ(SelectedItemUDs);
	//UPacketFunctionLibrary::INVENTORY_SELL_RQ(SelectedItemUDs, WalletIdList, SelectedWalletAmounts);
}

void UUC_Popup_Item_Sell::OnInventorySellRp(bool bSuccess, TArray<FREWARD> Rewards)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventorySell.RemoveAll(this);

	if (bSuccess)
	{
		// Show rewards popup and exit
		// TODO: fix use of legacy UP_RewardPopup
		UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_RewardsSplash, UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_SellResult_Title"))));
		check(Splash);
		UUC_RewardsSplash* RewardsSplash = Cast<UUC_RewardsSplash>(Splash->GetContentWidget());
		check(RewardsSplash);

		RewardsSplash->RefreshRewards(Rewards, EItemInventoryRewardOrigin::Sell);

		OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
	}
	else
	{
		// TODO
		ensure(false);
	}
}
