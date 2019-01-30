// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ItemSortFilterPopup.h"

#include "GlobalIntegrated.h"

#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Component/CheckBox_Radio.h"


void UUC_ItemSortFilterPopup::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory))
	{
		SortDescType = RGameInstance->RInventory->GetItemSortDescType();
		SortStandType = RGameInstance->RInventory->GetItemSortStandType();

		if (IsValid(UpRadioGroup))
		{
			UpRadioGroup->SetActiveRadioIndex(SortDescType == true ? 0 : 1);
		}

		if (IsValid(DownRadioGroup))
		{
			DownRadioGroup->SetActiveRadioIndex(SortStandType);
		}
	}

	if (Button_OK)
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnClick_Ok);
		Button_OK->OnClicked.AddDynamic(this, &UUC_ItemSortFilterPopup::OnClick_Ok);
	}

	if (Button_Close)
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnClick_Close);
		Button_Close->OnClicked.AddDynamic(this, &UUC_ItemSortFilterPopup::OnClick_Close);
	}

	if (CheckBox_descending)
	{
		CheckBox_descending->OnCheckStateChanged.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnDownRadioChange);
		CheckBox_descending->OnCheckStateChanged.AddDynamic(this, &UUC_ItemSortFilterPopup::OnDownRadioChange);
		
	}

	if (CheckBox_ascending)
	{
		CheckBox_ascending->OnCheckStateChanged.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnDownRadioChange);
		CheckBox_ascending->OnCheckStateChanged.AddDynamic(this, &UUC_ItemSortFilterPopup::OnDownRadioChange);
	}

	if (CheckBox_Basic)
	{
		CheckBox_Basic->OnCheckStateChanged.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
		CheckBox_Basic->OnCheckStateChanged.AddDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
	}
	if (CheckBox_Grade)
	{
		CheckBox_Grade->OnCheckStateChanged.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
		CheckBox_Grade->OnCheckStateChanged.AddDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
	}
	if (CheckBox_EnchantLevel)
	{
		CheckBox_EnchantLevel->OnCheckStateChanged.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
		CheckBox_EnchantLevel->OnCheckStateChanged.AddDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
	}
	if (CheckBox_Basic)
	{
		CheckBox_ItemType->OnCheckStateChanged.RemoveDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
		CheckBox_ItemType->OnCheckStateChanged.AddDynamic(this, &UUC_ItemSortFilterPopup::OnUpRadioChange);
	}


	
}

void UUC_ItemSortFilterPopup::NativeDestruct()
{	
	Super::NativeDestruct();
}

void UUC_ItemSortFilterPopup::OnDownRadioChange(bool _ischecked)
{
	if(CheckBox_ascending->IsChecked())
		SortDescType = false;
	else if(CheckBox_descending->IsChecked())
		SortDescType = true;
}


void UUC_ItemSortFilterPopup::OnUpRadioChange(bool _ischecked)
{
	if(CheckBox_Basic->GetCheckedState() == ECheckBoxState::Checked)
		SortStandType = (int32)EInventorySortStandardTypeEnum::VE_Base;
	else if (CheckBox_Grade->GetCheckedState() == ECheckBoxState::Checked)
		SortStandType = (int32)EInventorySortStandardTypeEnum::VE_Grade;
	else if (CheckBox_EnchantLevel->GetCheckedState() == ECheckBoxState::Checked)
		SortStandType = (int32)EInventorySortStandardTypeEnum::VE_Enchant;
	else if (CheckBox_ItemType->GetCheckedState() == ECheckBoxState::Checked)
		SortStandType = (int32)EInventorySortStandardTypeEnum::VE_ItemType;
}

void UUC_ItemSortFilterPopup::OnClick_Ok()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager) && IsValid(RGameInstance->RInventory))
	{
		RGameInstance->RInventory->SetItemSortDescType(SortDescType);
		RGameInstance->RInventory->SetItemSortStandType(SortStandType);

//		RGameInstance->EventManager->OnClickOK.Broadcast();

		OnClick_Close();
	}
}

void UUC_ItemSortFilterPopup::OnClick_Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
	
	//UUP_Item_Management* ItemManagement = FindParentRBaseWidget<UUP_Item_Management>(ParentWidget);
	//if (IsValid(ItemManagement))
	//{
	//	ItemManagement->PlayAni(TEXT("Filter"), true);
	//}
}