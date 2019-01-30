// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_Item_Grind.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "UI/ItemManagement/UP_GenericSplash.h"
#include "UI/ItemManagement/UC_RewardsSplash.h"
#include "Global/RGameInstance.h"
#include "Network/HttpRequestClient.h"
#include "UIFunctionLibrary.h"
#include "UtilFunctionLibrary.h"

void UUC_Popup_Item_Grind::NativeConstruct()
{
	Super::NativeConstruct();

	if(Button_Cancel)
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_Item_Grind::OnButtonCancelClicked);
	if (Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_Item_Grind::OnButtonCancelClicked);
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUC_Popup_Item_Grind::OnButtonOKClicked);
}

void UUC_Popup_Item_Grind::NativeDestruct()
{
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_OK))
		Button_OK->OnClicked.RemoveAll(this);
	if (IsValid(Button_Close))
		Button_Close->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_Popup_Item_Grind::SetData(TArray<FString> selectedItemList, TArray<FName> selectedWalletKeyList, TArray<int32> selectedWalletAmountList)
{
	SelectedItemUDs = selectedItemList;
	SelectedWalletKeys = selectedWalletKeyList;
	SelectedWalletAmounts = selectedWalletAmountList;
}

void UUC_Popup_Item_Grind::OnButtonCancelClicked()
{
	OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::NoChange, true);
}

void UUC_Popup_Item_Grind::OnButtonOKClicked()
{
	check(SelectedItemUDs.Num() != 0);

	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemGrind.AddUObject(this, &UUC_Popup_Item_Grind::OnInventoryGrindRp);

	UPacketFunctionLibrary::INVENTORY_ITEM_GRIND_RQ(SelectedItemUDs);
}

void UUC_Popup_Item_Grind::OnInventoryGrindRp(bool bSuccess, TArray<FREWARD> Rewards)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemGrind.RemoveAll(this);

	if (bSuccess)
	{
		UUP_GenericSplash* Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_RewardsSplash, UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_GrindResult_Title"))));
		check(Splash);
		UUC_RewardsSplash* RewardsSplash = Cast<UUC_RewardsSplash>(Splash->GetContentWidget());
		check(RewardsSplash);

		RewardsSplash->RefreshRewards(Rewards, EItemInventoryRewardOrigin::Grind);

		OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		// TODO
		ensure(false);
	}
}
