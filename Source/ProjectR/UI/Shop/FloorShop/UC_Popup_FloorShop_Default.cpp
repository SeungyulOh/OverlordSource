// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GameInfo/RInventory.h"

#include "GlobalIntegrated.h"
#include "Global/ShopManager.h"



#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"

#include "UC_Popup_FloorShop_Default.h"
#include "UI/UI_Common/UP_CommonPopup.h"


void UUC_Popup_FloorShop_Default::NativeConstruct()
{	
	Super::NativeConstruct();
	Init();
}


void UUC_Popup_FloorShop_Default::NativeDestruct()
{
	Super::NativeDestruct();	
}

void UUC_Popup_FloorShop_Default::Init()
{
	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonClickClose);
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonClickClose);
	}
	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonClickClose);
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonClickClose);
	}
	if (IsValid(Button_Ok))
	{
		Button_Ok->OnClicked.RemoveDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonClickOk);
		Button_Ok->OnClicked.AddDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonClickOk);
	}
}

void UUC_Popup_FloorShop_Default::FloorActivePopup()
{
	int32	PopupStatus = int32(EPopupStatus::FloorActive);
	if(IsValid(WidgetSwitcher_State))
		WidgetSwitcher_State->SetActiveWidgetIndex(PopupStatus);
	if(IsValid(Text_ActiveCurrency))
		Text_ActiveCurrency->SetText(FText::AsNumber(1));
}

void UUC_Popup_FloorShop_Default::FreeRefreshPopup()
{
	int32	PopupStatus = int32(EPopupStatus::FreeRefresh);
	if (IsValid(WidgetSwitcher_State))
		WidgetSwitcher_State->SetActiveWidgetIndex(PopupStatus);
}

void UUC_Popup_FloorShop_Default::PayRefreshPopup()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	UShopManager*		ShopManager = RGameInstance->ShopManager;
	int32	PopupStatus = int32(EPopupStatus::PayRefresh);
	if (IsValid(WidgetSwitcher_State))
		WidgetSwitcher_State->SetActiveWidgetIndex(PopupStatus);
// 	if (IsValid(Text_RefreshCurrency))
// 	{
// 		int32 nNeedGemCount = ShopManager->CurFloorStoreStatus.resetPrice;
// 		Text_RefreshCurrency->SetText(FText::AsNumber(nNeedGemCount));
// 	}
}

void UUC_Popup_FloorShop_Default::OnButtonClickOk()
{
	URGameInstance*		RGameInstance	=	RGAMEINSTANCE(this);
	URInventory*		RInventory		=	RGameInstance->RInventory;
	UShopManager*		ShopManager		=	RGameInstance->ShopManager;
	URWidgetManager*	RWidgetManager	=	RGameInstance->RWidgetManager;

	checkf(RInventory != nullptr, TEXT("UUC_Popup_FloorShop_Default::OnButtonClickOk[RInventory is Empty]"));
	checkf(ShopManager != nullptr, TEXT("UUC_Popup_FloorShop_Default::OnButtonClickOk[ShopManager is Empty]"));
	checkf(RWidgetManager != nullptr, TEXT("UUC_Popup_FloorShop_Default::OnButtonClickOk[RWidgetManager is Empty]"));

	EPopupStatus			PopupStatus		=	EPopupStatus(WidgetSwitcher_State->GetActiveWidgetIndex());
	uint32					CurFloor		=	ShopManager->GetCurFloor();
// 	ESHOP_FLOORSTORE_TYPE	CurStore		=	ShopManager->GetCurStore();
	
	CurFloor = FMath::Max(uint32(1), CurFloor);
	CurFloor = FMath::Min(uint32(24), CurFloor);

	switch (PopupStatus)
	{
		case EPopupStatus::FloorActive:
		{
			if (RInventory->GetFloorStoreTicket() > 0)
			{
				ShopManager->RequestFloorStoreStatus(CurFloor, true);
				this->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				UUP_CommonPopup* Popup = UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), 
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName("Confirm_Message_000056")), ECommonPopupType::VE_OK);

				Popup->OnClickOK.RemoveDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonTicketFail);
				Popup->OnClickOK.AddDynamic(this, &UUC_Popup_FloorShop_Default::OnButtonTicketFail);
			}

			break;
		}
		case EPopupStatus::FreeRefresh:
		{
// 			if (CurFloor < 5 && CurStore != ESHOP_FLOORSTORE_TYPE::COMMON)
// 				break;
// 
// 			ShopManager->RequestFloorStoreReset(CurFloor, uint32(CurStore), false);
// 			this->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
		case EPopupStatus::PayRefresh:
		{
// 			if (CurFloor < 5 && CurStore != ESHOP_FLOORSTORE_TYPE::COMMON)
// 				break;
// 
// 			int32 Gem = RInventory->GetGem();
// 			int32 nNeedGemCount = ShopManager->CurFloorStoreStatus.resetPrice;
// 
// 			if (Gem < nNeedGemCount)
// 			{
// 				UUP_CommonPopupSplash* CommonPopup = UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_OptionValueShortage")));
// 				break;
// 			}
// 
// 
// // 			ShopManager->RequestFloorStoreReset(CurFloor, uint32(CurStore), true);
// 			this->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
	}
}

void UUC_Popup_FloorShop_Default::OnButtonClickClose()
{
	EPopupStatus			PopupStatus = EPopupStatus(WidgetSwitcher_State->GetActiveWidgetIndex());

	this->SetVisibility(ESlateVisibility::Collapsed);

	switch (PopupStatus)
	{
		case EPopupStatus::FloorActive:
		{
			OnButtonTicketFail();
			break;
		}
		case EPopupStatus::FreeRefresh:
		{
			break;
		}
		case EPopupStatus::PayRefresh:
		{
			break;
		}
	}
}

void UUC_Popup_FloorShop_Default::OnButtonTicketFail()
{
	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	URWidgetManager*	RWidgetManager = RGameInstance->RWidgetManager;

	checkf(RWidgetManager != nullptr, TEXT("UUC_Popup_FloorShop_Default::OnButtonClickClose[RWidgetManager is Empty]"));

	RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_LobbyIndoor);
}

