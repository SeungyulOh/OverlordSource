// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemInventoryPage.h"


#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "Table/ItemTableInfo.h"
#include "UI/Common/ItemInventory_ScrollData.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "Table/CharacterTableInfo.h"
#include "UI/Control/RScrollItem.h"
#include "UC_GenericPopup.h"

#include "UtilFunctionIntegrated.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UC_HeroItemInfo.h"

#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/Control/RScrollView.h"


#include "UI/UI_Common/UP_RewardPopup.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/Component/HorizontalBox_RadioGroup.h"
#include "UI/Component/CheckBox_Radio.h"

void UUP_ItemInventoryPage::NativePreConstruct()
{
	Super::NativePreConstruct();

	// add content widget
	if (IsValid(ContentWidgetClass.Get()) && !IsValid(ContentWidget))
	{
		ContentWidget = CreateWidget<UUserWidget>(GetWorld(), ContentWidgetClass.Get());
		if (!ensure(IsValid(ContentWidget)))
			return;

#if WITH_EDITOR
		ContentWidget->SetDesignerFlags(GetDesignerFlags());
#endif

		// Adding UP; taking up the whole screen
		
		auto RootWidget = Cast<UPanelWidget>(GetRootWidget());
		check(IsValid(RootWidget));
		RootWidget->AddChild(ContentWidget);
		auto ContentWidgetSlot = Cast<UCanvasPanelSlot>(ContentWidget->Slot);
		check(IsValid(ContentWidgetSlot));
		FAnchors Anchors;
		Anchors.Minimum.X = 0.0;
		Anchors.Minimum.Y = 0.0;
		Anchors.Maximum.X = 1.0;
		Anchors.Maximum.Y = 1.0;
		ContentWidgetSlot->SetAnchors(Anchors);

		FMargin Offset;
		ContentWidgetSlot->SetOffsets(Offset);
	}
}

void UUP_ItemInventoryPage::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize PageContent
	if (ensure(IsValid(ContentWidget)))
	{
		auto PageContent = Cast<UUP_ItemInventoryPage_ContentBase>(ContentWidget);
		if (IsValid(PageContent))
		{
			PageContent->ItemInventoryPage = this;

			check(IsValid(RGAMEINSTANCE(this)->RWidgetManager));
			if (ensure(RGAMEINSTANCE(this)->RWidgetManager->UIEventQueue.IsValidIndex(0)))
			{
				// Get the ItemUD
				FUIEventInfo ItemUDInfo = RGAMEINSTANCE(this)->RWidgetManager->UIEventQueue[0];
				switch (ItemUDInfo.EventType)
				{
				case EUIEventEnum::VE_ItemInventoryPage:
					PageContent->SetItemUD(ItemUDInfo.StringValue);
					RGAMEINSTANCE(this)->RWidgetManager->RemoveUIEvent(EUIEventEnum::VE_ItemInventoryPage);
					break;
				case EUIEventEnum::VE_RuneInventoryPage:
					PageContent->SetWalletKey(ItemUDInfo.NameValue);
					RGAMEINSTANCE(this)->RWidgetManager->RemoveUIEvent(EUIEventEnum::VE_RuneInventoryPage);
					break;
				default:
					ensure(false);
					break;
				}

				CurrentTab = PageContent->GetDefaultTab();
			}
		}
	}

	// this is a page so it has to invalidate itself
	InvalidateContentWidget();

}

void UUP_ItemInventoryPage::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_ItemInventoryPage::InvalidateData()
{
	Super::InvalidateData();

	// Not invalidating content widget

	InvalidateItemInventoryScrollView();
}

UUserWidget* UUP_ItemInventoryPage::GetContentWidget()
{
	return ContentWidget;
}

EItemInventoryEnum UUP_ItemInventoryPage::TabIndexToInventoryEnum(int32 Index)
{
	switch (Index)
	{
	case 0:
		return EItemInventoryEnum::Weapons;
	case 1:
		return EItemInventoryEnum::Defense;
	case 2:
		return EItemInventoryEnum::Accessories;
	default:
		check(false);
		break;
	}
	return EItemInventoryEnum::Weapons; // no
}

bool UUP_ItemInventoryPage::IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	auto PageContent = GetPageContent();
	if (!ensure(IsValid(PageContent)))
		return true;

	return PageContent->IsInventoryIncludingEquipmentItem(ItemInfo, ItemData);
}

void UUP_ItemInventoryPage::OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
	check(IsValid(ScrollData));

	auto PageContent = GetPageContent();
	if (!ensure(IsValid(PageContent)))
		return;

	PageContent->OnInventoryEquipmentItemSelected(ScrollData->ItemUD);
}

void UUP_ItemInventoryPage::OnInventoryScrollViewSelectedItem_WalletItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
	check(IsValid(ScrollData));

	auto PageContent = GetPageContent();
	if (!ensure(IsValid(PageContent)))
		return;

	PageContent->OnInventoryWalletItemSelected(ScrollData->WalletKey);
}

void UUP_ItemInventoryPage::UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData)
{
	Super::UpdateScrollItemData_EquipmentItem(ScrollItemData);

	auto PageContent = GetPageContent();
	if (!ensure(IsValid(PageContent)))
		return;

	PageContent->UpdateScrollItemData_EquipmentItem(ScrollItemData);
}

void UUP_ItemInventoryPage::InvalidateContentWidget()
{
	auto PageContent = GetPageContent();
	if (!ensure(IsValid(PageContent)))
		return;

	//PageContent->OnInventoryWalletItemSelected();
	PageContent->InvalidateData();
}

UUP_ItemInventoryPage_ContentBase* UUP_ItemInventoryPage::GetPageContent()
{
	if (IsValid(ContentWidget))
	{
		auto PageContent = Cast<UUP_ItemInventoryPage_ContentBase>(ContentWidget);
		if (IsValid(PageContent))
		{
			return PageContent;
		}
	}

	return nullptr;
}

void UUP_ItemInventoryPage_ContentBase::ClosePage()
{
	check(RGAMEINSTANCE(this)->RWidgetManager);

	auto TopBar = Cast<UTopBar>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar));
	if (!ensure(IsValid(TopBar)))
		return;

	TopBar->PrevButton();
}

void UUP_RuneInventoryPage::NativeConstruct()
{
	Super::NativeConstruct();
	UC_Popup_ItemSorting->Button_AttackPower->SetVisibility(ESlateVisibility::Collapsed);
	UC_Popup_ItemSorting->Button_GettingOrder->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_RuneInventoryPage::NativeDestruct()
{
	Super::NativeDestruct();
}

EItemInventoryEnum UUP_RuneInventoryPage::TabIndexToInventoryEnum(int32 Index)
{
	return EItemInventoryEnum::Runes;
}

void UUP_ItemInventoryPage::UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData)
{
	// override to display ContentPage.WalletKey rune to be displayed as selected
	Super::UpdateScrollItemData_Material(ScrollItemData);

	auto PageContent = GetPageContent();
	if (!ensure(IsValid(PageContent)))
		return;

	PageContent->UpdateScrollItemData_Material(ScrollItemData);
}
