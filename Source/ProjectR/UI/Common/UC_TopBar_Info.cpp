// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_TopBar_Info.h"
#include "GlobalIntegrated.h"

#include "UI/RWidgetManager.h"
#include "UI/Common/State/BasePropertyState.h"
#include "UI/Common/State/PropertyState.h"
#include "UI/Common/UC_TopBar_Property.h"
#include "Table/TopbarTableInfo.h"

void UUC_TopBar_Info::NativeConstruct()
{
	FindChildWidgets<UUC_TopBar_Property>(this, TopBar_Info_Propertys);
	SetPropertySet();
	RefreshProperty();
}

void UUC_TopBar_Info::NativeDestruct()
{
}

void UUC_TopBar_Info::RefreshProperty()
{
	for (auto& Element : TopBar_Info_Propertys)
		Element->RefreshCurrent();
}

void UUC_TopBar_Info::SetPropertySet()
{
	auto TopbarTableInfo = UTableManager::GetInstancePtr()->GetTopbarRow("UIPage_TopbarPropertySet");

	if (!TopbarTableInfo)
		return;

	/*auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RWidgetManager))
		return;*/

	for (auto& TopBar_Property_Elem : TopBar_Info_Propertys)
	{
		TopBar_Property_Elem->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 Index = 0; Index < TopbarTableInfo->PropertyKeys.Num(); Index++)
	{
		TopBar_Info_Propertys[Index]->SetPropertyType(TopbarTableInfo->PropertyKeys[Index]);
		TopBar_Info_Propertys[Index]->SetVisibility(ESlateVisibility::Visible);
	}
}
