// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_ItemSorting.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "GlobalConstants.h"
#include "Gameinfo/RInventory.h"
#include "UtilFunctionLibrary.h"


void UUC_Popup_ItemSorting::NativeConstruct()
{
	if (Button_AttackPower->SortingBtn)
	{
		Button_AttackPower->SortingBtn->OnClicked.AddDynamic(this, &UUC_Popup_ItemSorting::OnClick_Button_AttackPower);
		Button_AttackPower->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Filter_Power"))));
	}
	if (Button_GettingOrder->SortingBtn)
	{
		Button_GettingOrder->SortingBtn->OnClicked.AddDynamic(this, &UUC_Popup_ItemSorting::OnClick_Button_GettingOrder);
		Button_GettingOrder->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Filter_Get"))));
	}
	if (Button_Grade->SortingBtn)
	{
		Button_Grade->SortingBtn->OnClicked.AddDynamic(this, &UUC_Popup_ItemSorting::OnClick_Button_Grade);
		Button_Grade->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Filter_Grade"))));
	}
}

void UUC_Popup_ItemSorting::NativeDestruct()
{
	if (Button_AttackPower)
		Button_AttackPower->SortingBtn->OnClicked.RemoveAll(this);
	if (Button_GettingOrder)
		Button_GettingOrder->SortingBtn->OnClicked.RemoveAll(this);
	if (Button_Grade)
		Button_Grade->SortingBtn->OnClicked.RemoveAll(this);
}

void UUC_Popup_ItemSorting::SetTypeNDescending(EFilterSortTypeEnum currentType, bool isDescending)
{
	Reset();
	switch (currentType)
	{
	case EFilterSortTypeEnum::VE_AttackPower:
		if(!isDescending)
			Button_AttackPower->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_AttackPower->CheckStateSwitcher->SetActiveWidgetIndex(0);
		break;
	case EFilterSortTypeEnum::VE_SortingOrder:
		if (!isDescending)
			Button_GettingOrder->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_GettingOrder->CheckStateSwitcher->SetActiveWidgetIndex(0);
		break;
	case EFilterSortTypeEnum::VE_Grade:
		if (!isDescending)
			Button_Grade->SortTypeSwitcher->SetActiveWidgetIndex(1);
		Button_Grade->CheckStateSwitcher->SetActiveWidgetIndex(0);
		break;
	default:
		break;
	}
}

void UUC_Popup_ItemSorting::Reset()
{
	Button_Grade->CheckStateSwitcher->SetActiveWidgetIndex(1);
	Button_Grade->SortTypeSwitcher->SetActiveWidgetIndex(0);
	Button_AttackPower->CheckStateSwitcher->SetActiveWidgetIndex(1);
	Button_AttackPower->SortTypeSwitcher->SetActiveWidgetIndex(0);
	Button_GettingOrder->CheckStateSwitcher->SetActiveWidgetIndex(1);
	Button_GettingOrder->SortTypeSwitcher->SetActiveWidgetIndex(0);
}

void UUC_Popup_ItemSorting::OnClick_Button_Grade()
{
	RGAMEINSTANCE(this)->RInventory->OnClickInvenSortingButton.Broadcast((int32)EFilterSortTypeEnum::VE_Grade);
}

void UUC_Popup_ItemSorting::OnClick_Button_AttackPower()
{
	RGAMEINSTANCE(this)->RInventory->OnClickInvenSortingButton.Broadcast((int32)EFilterSortTypeEnum::VE_AttackPower);
}

void UUC_Popup_ItemSorting::OnClick_Button_GettingOrder()
{
	RGAMEINSTANCE(this)->RInventory->OnClickInvenSortingButton.Broadcast((int32)EFilterSortTypeEnum::VE_SortingOrder);
}
