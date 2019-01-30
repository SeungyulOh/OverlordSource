// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"
#include "Global/ShopManager.h"

#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"

#include "Utils/UtilFunctionLibrary.h"

#include "UP_FloorShop.h"


void UUP_FloorShop::NativeConstruct()
{	
	Super::NativeConstruct();
	Init();
}


void UUP_FloorShop::NativeDestruct()
{
	Super::NativeDestruct();	
}

void UUP_FloorShop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetTime();
}

void UUP_FloorShop::Init()
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UShopManager*		ShopManager		=	GameInstance->ShopManager;
	OwnerWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::Init[ShopManager is Empty]"));

	ShopManager->SetCurFloor(1);
	ShopManager->RequestFloorStoreStatus(ShopManager->GetCurFloor(), false);

	if (TimeHandler_ShopTime.IsValid())
		OwnerWorld->GetTimerManager().ClearTimer(TimeHandler_ShopTime);
}

void UUP_FloorShop::SetUI()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::SetUI[ShopManager is Empty]"));

	FindChildWidgetsWithCertainName<UUC_FloorShop_ScrollItem>(this, ItemInfo, TEXT("ShopItem_0"));
	FindChildWidgetsWithCertainName<UUC_FloorShop_ScrollItem>(this, RulerItemInfo, TEXT("RulerShopItem_0"));
	SetGroupList();
	SetFloorText();
	SetResetText();
	SetButton();
// 	SetItem(ShopManager->GetCurStore());
// 	OwnerWorld->GetTimerManager().SetTimer(TimeHandler_ShopTime, this, &UUP_FloorShop::OnTimeOut, ShopManager->CurFloorStoreStatus.timeLeft, false);
// 	OwnerWorld->GetTimerManager().SetTimer(TimeHandler_ResetTime, this, &UUP_FloorShop::OnItemResetTimeOut, ShopManager->CurFloorStoreStatus.resetTokenTimeLeft, false);
	TimeVisibleState();
}

void UUP_FloorShop::SetGroupList()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = RGameInstance->ShopManager;

	checkf(ShopManager != nullptr, TEXT("UUC_Popup_FloorShop_Default::OnButtonClickOk[ShopManager is Empty]"));

	uint32					CurFloor = ShopManager->GetCurFloor();
// 	ESHOP_FLOORSTORE_TYPE	CurStore = ShopManager->GetCurStore();

	if (IsValid(CheckBox_Normal))
	{
		CheckBox_Normal->OnCheckRadioCheck.RemoveDynamic(this, &UUP_FloorShop::OnNormalCheckBoxClick);
		CheckBox_Normal->OnCheckRadioCheck.AddDynamic(this, &UUP_FloorShop::OnNormalCheckBoxClick);
	}
	if (IsValid(CheckBox_Coinage))
	{
		CheckBox_Coinage->OnCheckRadioCheck.RemoveDynamic(this, &UUP_FloorShop::OnCoinageCheckBoxClick);
		CheckBox_Coinage->OnCheckRadioCheck.AddDynamic(this, &UUP_FloorShop::OnCoinageCheckBoxClick);
	}
	if (IsValid(CheckBox_Ruler))
	{
		CheckBox_Ruler->OnCheckRadioCheck.RemoveDynamic(this, &UUP_FloorShop::OnRulerCheckBoxClick);
		CheckBox_Ruler->OnCheckRadioCheck.AddDynamic(this, &UUP_FloorShop::OnRulerCheckBoxClick);
	}
	if (CurFloor < 5)
	{
		CheckBox_Coinage->SetIsEnabled(false);
		CheckBox_Ruler->SetIsEnabled(false);
	}
	else
	{
		CheckBox_Coinage->SetIsEnabled(true);
		CheckBox_Ruler->SetIsEnabled(true);
	}
}

void UUP_FloorShop::SetButton()
{
	if (IsValid(Button_Reset))
	{
		Button_Reset->OnClicked.RemoveDynamic(this, &UUP_FloorShop::OnButtonClickRest);
		Button_Reset->OnClicked.AddDynamic(this, &UUP_FloorShop::OnButtonClickRest);
	}
}

void UUP_FloorShop::SetFloorText()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::SetFloorText[ShopManager is Empty]"));

	if (IsValid(Text_Floor))
		Text_Floor->SetText(FText::AsNumber(ShopManager->GetCurFloor()));
}

void UUP_FloorShop::SetResetText()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	URInventory*		RInventory = GameInstace->RInventory;

	checkf(RInventory != nullptr, TEXT("UUP_FloorShop::SetResetText[RInventory is Empty]"));

	if (IsValid(Text_Floor))
		Text_ResetCountMin->SetText(FText::AsNumber(RInventory->GetFloorStoreResetToken()));
}

// void UUP_FloorShop::SetItem(ESHOP_FLOORSTORE_TYPE InCurStore)
// {
// 	URGameInstance*		GameInstace		=	RGAMEINSTANCE(this);
// 	UShopManager*		ShopManager		=	GameInstace->ShopManager;
// 	UUserInfo*			UserInfo		=	GameInstace->UserInfo;
// 
// 	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::SetItem[ShopManager is Empty]"));
// 	checkf(UserInfo != nullptr, TEXT("UUP_FloorShop::SetItem[UserInfo is Empty]"));
// 
// 	FAVATAR				Avatar			=	UserInfo->GetAvatarData();
// 
// 	TArray<FSHOP_FLOORSTORE_PRODUCT> ItemList;
// 
// 	switch (InCurStore)
// 	{
// 		case ESHOP_FLOORSTORE_TYPE::COMMON:
// 		{
// 			ItemList = ShopManager->CurFloorStoreStatus.common;
// 			break;
// 		}
// 		case ESHOP_FLOORSTORE_TYPE::COIN:
// 		{
// 			ItemList = ShopManager->CurFloorStoreStatus.coin;
// 			break;
// 		}
// 		case ESHOP_FLOORSTORE_TYPE::LORD:
// 		{
// 			ItemList = ShopManager->CurFloorStoreStatus.lord;
// 			break;
// 		}
// 	}
// 
// 	for (int Index = 0; Index < Normal_Max_Item; Index++)
// 	{
// 		if(ItemInfo.IsValidIndex(Index) == false)
// 			continue;
// 		if (ItemList.IsValidIndex(Index) == false)
// 			continue;
// 
// 		//ItemInfo[Index]->Init(Popup_SellItem);
// 		//ItemInfo[Index]->SetItemInfo(ItemList[Index]);
// 
// 		int32		RequieredLevel = (Index - Sells_PerLevel_StartValue) * 10;
// 
// // 		if (InCurStore == ESHOP_FLOORSTORE_TYPE::COMMON)
// // 		{
// // 			if (Index >= 5 && Avatar.level < RequieredLevel)
// // 				ItemInfo[Index]->Lock(RequieredLevel);
// // 		}
// // 
// // 		if (InCurStore == ESHOP_FLOORSTORE_TYPE::COIN)
// // 		{
// // 			if (Index >= 5 && Avatar.level < RequieredLevel+5)
// // 				ItemInfo[Index]->Lock(RequieredLevel + 5);
// // 		}
// 	}
// }

void UUP_FloorShop::SetTime()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::SetFloorText[ShopManager is Empty]"));

	float FloorTime			=	OwnerWorld->GetTimerManager().GetTimerRemaining(TimeHandler_ShopTime);
	float FloorMinCount		=	FloorTime / 60.0f;
	float FloorHourCount	=	FloorMinCount / 60.0f;
	FloorMinCount -= (int32(FloorHourCount) * 60);

	FText FloorText = FText::FromString(TEXT("{0}{1} {2}{3}"));

	if (IsValid(Text_ShopTime))
		Text_ShopTime->SetText(FText::Format(FloorText, int32(FloorHourCount), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonExit_Hour")),  int32(FloorMinCount), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Min"))));

	float ItemResetTime			=	OwnerWorld->GetTimerManager().GetTimerRemaining(TimeHandler_ResetTime);
	float ItemResetMinCount		=	ItemResetTime / 60.0f;
	float ItemResetHourCount	=	ItemResetMinCount / 60.0f;
	ItemResetMinCount -= (int32(ItemResetHourCount) * 60);

	FText ItemResetText= FText::FromString(TEXT("{0}{1} {2}{3}"));

	if (IsValid(Text_ResetTime))
		Text_ResetTime->SetText(FText::Format(ItemResetText, int32(ItemResetHourCount), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Arenalobby_SeasonExit_Hour")), int32(ItemResetMinCount), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Min"))));
}

void UUP_FloorShop::ShopOpenPopupActive()
{
	Popup_OpenFloorShop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Popup_OpenFloorShop->FloorActivePopup();
}

void UUP_FloorShop::ShopOpenPopupDisable()
{
	Popup_OpenFloorShop->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_FloorShop::SetRewardItem(TArray<FREWARD>& InReward)
{
	if (IsValid(Splash_AddItem))
	{
		Splash_AddItem->SetItem(InReward[0]);
		Splash_AddItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

// void UUP_FloorShop::SetItemList(UWidgetSwitcher* InWidgetSwitch, ESHOP_FLOORSTORE_TYPE InType, bool InButtonEnable, int32 InShopSwitch)
// {
// 	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
// 	UShopManager*		ShopManager = GameInstace->ShopManager;
// 
// 	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::OnRulerCheckBoxClick[ShopManager is Empty]"));
// 
// 	if (IsValid(InWidgetSwitch))
// 	{
// 		ShopManager->SetCurStore(InType);
// 		SetItem(ShopManager->GetCurStore());
// 		InWidgetSwitch->SetActiveWidgetIndex(0);
// 		Button_Reset->SetIsEnabled(InButtonEnable);
// 	}
// 	if (IsValid(WidgetSwitcher_Shop))
// 		WidgetSwitcher_Shop->SetActiveWidgetIndex(InShopSwitch);
// }

void UUP_FloorShop::OnNormalCheckBoxClick(bool IsCheck)
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::OnRulerCheckBoxClick[ShopManager is Empty]"));

// 	if (IsCheck)
// 		SetItemList(WidgetSwitcher_Normal, ESHOP_FLOORSTORE_TYPE::COMMON, true, 0);
}

void UUP_FloorShop::OnCoinageCheckBoxClick(bool IsCheck)
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::OnRulerCheckBoxClick[ShopManager is Empty]"));

// 	if (IsCheck)
// 		SetItemList(WidgetSwitcher_Normal, ESHOP_FLOORSTORE_TYPE::COIN, true, 0);
}

void UUP_FloorShop::OnRulerCheckBoxClick(bool IsCheck)
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::OnRulerCheckBoxClick[ShopManager is Empty]"));

// 	if (IsCheck)
// 		SetItemList(WidgetSwitcher_Normal, ESHOP_FLOORSTORE_TYPE::LORD, false, 1);
}

void UUP_FloorShop::OnButtonClickRest()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	URInventory*		RInventory = GameInstace->RInventory;
	UShopManager*		ShopManager = GameInstace->ShopManager;

// 	if (ShopManager->GetCurStore() == ESHOP_FLOORSTORE_TYPE::LORD)
// 		return;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::OnButtonClickRest[ShopManager is Empty]"));
	checkf(RInventory != nullptr, TEXT("UUP_FloorShop::OnButtonClickRest[RInventory is Empty]"));

	if (RInventory->GetFloorStoreResetToken() > 0)
	{
		Popup_OpenFloorShop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Popup_OpenFloorShop->FreeRefreshPopup();
	}
	else
	{
		Popup_OpenFloorShop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Popup_OpenFloorShop->PayRefreshPopup();
	}
}

void UUP_FloorShop::OnTimeOut()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;
	TimeVisibleState();
	ShopManager->RequestFloorStoreStatus(ShopManager->GetCurFloor(), false);
}

void UUP_FloorShop::OnItemResetTimeOut()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;
	TimeVisibleState();
	ShopManager->RequestFloorStoreStatus(ShopManager->GetCurFloor(), false);
}

void UUP_FloorShop::TimeVisibleState()
{
	URGameInstance*		GameInstace = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = GameInstace->ShopManager;
	URInventory*		RInventory = GameInstace->RInventory;

	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::SetUI[ShopManager is Empty]"));
	checkf(RInventory != nullptr, TEXT("UUP_FloorShop::SetResetText[RInventory is Empty]"));

	if (IsValid(Text_ShopTime))
	{
// 		if (ShopManager->CurFloorStoreStatus.timeLeft <= 0)
// 			Text_ShopTime->SetVisibility(ESlateVisibility::Collapsed);
// 		else
// 			Text_ShopTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (IsValid(Text_ResetTime))
	{
		if (RInventory->GetFloorStoreResetToken() >= 5)
			Text_ResetTime->SetVisibility(ESlateVisibility::Collapsed);
		else
			Text_ResetTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

