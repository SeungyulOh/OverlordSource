// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_PopupContent_EquipmentItemFilter.h"

#include "UI/Control/RScrollView.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "Table/ItemTableInfo.h"
#include "UI/Common/ItemInventory_ScrollData.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "Table/CharacterTableInfo.h"
#include "UI/Control/RScrollItem.h"
#include "UC_GenericPopup.h"
#include "UI/RWidgetManager.h"
#include "UI/Component/CheckBox_Radio.h"
#include "Network/PacketDataStructures.h"

void UUC_PopupContent_EquipmentItemFilter::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_PopupContent_EquipmentItemFilter::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_PopupContent_EquipmentItemFilter::InvalidateData()
{
	Super::InvalidateData();
}

void UUC_FilterItem::NativePreConstruct()
{
	Super::NativePreConstruct();

	// set the localized text and default check state

	CheckBox_Filter->SetCheckedState(CheckedByDefault ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	if (OverrideText.IsEmpty())
	{
		Text_Filter->LocalizingKey = LocalizingKey;
		Text_Filter->StringTableType = StringTableType;
		Text_Filter->ApplyLocalizedText();
	}
	else
	{
		Text_Filter->SetText(OverrideText);
	}
}
