// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ScrollItem_UserHero.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "UI/Avatar/Item/ItemData_UserHero.h"

void UUC_ScrollItem_UserHero::NativeConstruct()
{
	Super::NativeConstruct();
	//AVATAR_ABILITY_UP_RQ
}

void UUC_ScrollItem_UserHero::NativeDestruct()
{
	Super::NativeDestruct();
	//GetAvatarAbilityTableRow
}

void UUC_ScrollItem_UserHero::SetUseHeroScrollItemData()
{
	if (IsValid(ItemData) == false)
		return;
	
	auto ScrollData = Cast<UItemData_UserHero>(ItemData);
	if (IsValid(ScrollData) == false)
		return;
	
	if(Image_Hero)
	{
		auto Image = UUIFunctionLibrary::GetIconImageFromTable(ScrollData->Image_Hero);
		UUIFunctionLibrary::SetImageBrush(Image, Image_Hero);
	}
			
	if (Text_UserName)
	{
		FText Text;
		ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(EStringTableType::VE_Hero, ScrollData->Text_UserName, Text);
		
		//auto Text = FText::FromName(ScrollData->Text_UserName);
		Text_UserName->SetText(Text);
	}
	
	if (Text_Use)
	{
		FText UseValue;
		if (ScrollData->UseValue == 0)
		{
			//None Image!
			ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(EStringTableType::VE_UI, "UI_Lobby_Norecord", UseValue);
		}
		else
		{
			UseValue = FText::AsNumber(ScrollData->UseValue);
		}

		//auto UseValue = FText::AsNumber(ScrollData->UseValue);
		Text_Use->SetText(UseValue);
	}

	if (Text_Description)
	{
		Text_Description->SetVisibility(ScrollData->UseValue == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (Text_Colon)
	{
		Text_Colon->SetVisibility(ScrollData->UseValue == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (Bar_Use)
	{
		float UsePoint = 0;
		if (ScrollData->UseValue != 0)
		{
			UsePoint = static_cast<float>(ScrollData->UseValue) / static_cast<float>(ScrollData->UseMax);
		}

		Bar_Use->SetPercent(UsePoint);
	}	
	
	if (ScrollData->UseValue == 0)
	{
		//None Image!
	}

}
