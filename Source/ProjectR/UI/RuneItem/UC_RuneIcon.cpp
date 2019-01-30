// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_RuneIcon.h"

#include "UtilFunctionIntegrated.h"

void UUC_RuneIcon::NativeConstruct()
{
	Super::NativeConstruct();
}
void UUC_RuneIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_RuneIcon::SetRune(int32 InRuneId, int32 InRuneCount)
{
	RuneId = InRuneId;
	if (IsValid(Image_Rune) == false)
		return;

	if (RuneId)
		RuneTalbe = UTableManager::GetInstancePtr()->GetRuneOptionRowInt(RuneId);
	
	if (RuneTalbe)
	{
		UObject* object =  UUIFunctionLibrary::GetIconImageFromTable(RuneTalbe->RuneInventoryImage);
		UUIFunctionLibrary::SetImageBrush(object, Image_Rune);
	}
	if (TextBlock_Count)
		TextBlock_Count->SetText(FText::AsNumber(InRuneCount));
}

void UUC_RuneIcon::SelectIconRune()
{
	if (bIsSelectEnable == false)
		return;

	SelectHeroIconBySwitcher();
}

void UUC_RuneIcon::SelectHeroIconBySwitcher()
{
	if (WidgetSwitcher_Select == nullptr)
		return;

	if (bIsSelected == true)
	{
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Visible);
		WidgetSwitcher_Select->SetActiveWidgetIndex(0);
	}
	else
	{
		WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Hidden);
	}
}