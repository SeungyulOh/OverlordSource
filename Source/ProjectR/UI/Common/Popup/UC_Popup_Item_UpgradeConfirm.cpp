// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_Item_UpgradeConfirm.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"


void UUC_Popup_Item_UpgradeConfirm::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Cancel)
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_Item_UpgradeConfirm::OnButtonCancelClicked);
	if (Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_Item_UpgradeConfirm::OnButtonCancelClicked);
	if (Button_OK)
		Button_OK->OnClicked.AddDynamic(this, &UUC_Popup_Item_UpgradeConfirm::OnButtonOKClicked);
}

void UUC_Popup_Item_UpgradeConfirm::NativeDestruct()
{
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.RemoveAll(this);
	if (IsValid(Button_OK))
		Button_OK->OnClicked.RemoveAll(this);
	if (IsValid(Button_Close))
		Button_Close->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_Popup_Item_UpgradeConfirm::OnButtonCancelClicked()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Popup_Item_UpgradeConfirm::OnButtonOKClicked()
{
	UPacketFunctionLibrary::INVENTORY_ITEM_UPGRADE_RQ(m_ItemUD, m_MaterialList);
	this->SetVisibility(ESlateVisibility::Collapsed);
}