// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UtilFunctionIntegrated.h"
#include "RPropertyPanel.h"


void URPropertyPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void URPropertyPanel::MakeChildMap()
{
	TArray< UWidget* > ChildrenWidget;
	this->WidgetTree->GetAllWidgets(ChildrenWidget);

	for (auto ChildWidget : ChildrenWidget)
	{
		UTextBlock* ChildText = Cast< UTextBlock >(ChildWidget);
		if (IsValid(ChildText))
		{
			FString WidgetName = ChildText->GetName();
			//property detail(+total Items)
			if (WidgetName.RemoveFromEnd("_Detail"))
				ChildDetailTextMap.Emplace(WidgetName, ChildText);
			//percent
			else if (WidgetName.RemoveFromEnd("_Percent"))
				ChildPercentTextMap.Emplace(WidgetName, ChildText);
			else if (WidgetName.RemoveFromEnd("_LeftBracket"))
				ChildLeftBracketTextMap.Emplace(WidgetName, ChildText);
			else if (WidgetName.RemoveFromEnd("_RightBracket"))
				ChildRightBracketTextMap.Emplace(WidgetName, ChildText);
			else if (WidgetName.RemoveFromEnd("_Operator"))
				ChildOperatorTextMap.Emplace(WidgetName, ChildText);
			else if (WidgetName.RemoveFromEnd("_Unit"))
				ChildUnitTextMap.Emplace(WidgetName, ChildText);
			else
				ChildTextMap.Emplace(WidgetName, ChildText);
		}
	}
}
/*

void URPropertyPanel::InvalidateData(UCharacterBattleInfo* InCharacterBattleInfo)
{
	

	if (ChildTextMap.Num() == 0 )
		MakeChildMap();

	for (TFieldIterator<UProperty> PropIt(CharacterBattleInfo->CharacterProperty.StaticStruct()); PropIt; ++PropIt)
	{
		UProperty* Property		= *PropIt;
		FString PropertyName	= Property->GetNameCPP();  
		FString PropertyStr		= "";
		if (ChildTextMap.Find(PropertyName))
		{
			//Final
			//PropertyStr = UDescriptionFunctionLibrary::GetHeroPropertyString(PropertyName, InCharacterBattleInfo->FinalCharacterProperty);
			PropertyStr = UDescriptionFunctionLibrary::GetHeroPropertyString(PropertyName, InCharacterBattleInfo);
			ChildTextMap[PropertyName]->SetText(FText::FromString(PropertyStr));
				
			//Total Item		
			if (ChildDetailTextMap.Find(PropertyName))
			{
				//PropertyStr = UDescriptionFunctionLibrary::GetHeroPropertyString(PropertyName, InCharacterBattleInfo->TotalItemCharacterProperty);
				PropertyStr = UDescriptionFunctionLibrary::GetItemPropertyString(PropertyName, InCharacterBattleInfo);
				if (IsStringZero(PropertyStr))
				{
					ChildDetailTextMap[PropertyName]->SetVisibility(ESlateVisibility::Collapsed);
					SetVisibilityPropertyText(PropertyName, false);
				}
				else
				{
					ChildDetailTextMap[PropertyName]->SetVisibility(ESlateVisibility::Visible);
					ChildDetailTextMap[PropertyName]->SetText(FText::FromString(PropertyStr));
					SetVisibilityPropertyText(PropertyName, true);
				}
			}

			//Property Value to Percent
			if (ChildPercentTextMap.Find(PropertyName))
			{
				PropertyStr = UDescriptionFunctionLibrary::GetHeroPropertyPercentString(PropertyName, InCharacterBattleInfo);
				ChildPercentTextMap[PropertyName]->SetText(FText::FromString(PropertyStr));
			}
		}
	}
}
*/
bool URPropertyPanel::IsStringZero(FString& InPropertyStr)
{
	if (InPropertyStr.Equals("0") || InPropertyStr.Equals("0.0"))
	{
		return true;
	}

	return false;
}

void URPropertyPanel::SetVisibilityPropertyText(FString& InPropertyName, bool IsVisible)
{
	auto VisibleValue = ESlateVisibility::Visible;

	if (IsVisible == true)
	{
		 VisibleValue= ESlateVisibility::Visible;
	}
	else
	{
		VisibleValue = ESlateVisibility::Collapsed;
	}

	if (ChildLeftBracketTextMap.Find(InPropertyName))
	{
		ChildLeftBracketTextMap[InPropertyName]->SetVisibility(VisibleValue);
	}

	if (ChildOperatorTextMap.Find(InPropertyName))
	{
		ChildOperatorTextMap[InPropertyName]->SetVisibility(VisibleValue);
	}

	if (ChildRightBracketTextMap.Find(InPropertyName))
	{
		ChildRightBracketTextMap[InPropertyName]->SetVisibility(VisibleValue);
	}

	if (ChildUnitTextMap.Find(InPropertyName))
	{
		ChildUnitTextMap[InPropertyName]->SetVisibility(VisibleValue);
	}
}