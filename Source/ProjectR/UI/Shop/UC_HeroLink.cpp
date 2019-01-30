// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroLink.h"

#include "GlobalIntegrated.h"

#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UI/Common/HeroScrollItemData.h"
#include "Table/CharacterTableInfo.h"


void UUC_HeroLink::NativeConstruct()
{	
	Super::NativeConstruct();

	OnSelectedItem.RemoveDynamic(this, &UUC_HeroLink::OnClickSelected);
	OnSelectedItem.AddDynamic(this, &UUC_HeroLink::OnClickSelected);
}

void UUC_HeroLink::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (false == IsValid(item))
		return;

	Refresh();
}

void UUC_HeroLink::Refresh()
{
	if (false == IsValid(this->ItemData))
		return;

	UHeroScrollItemData* ScrollItemData = Cast<UHeroScrollItemData>(this->ItemData);
	if (false == IsValid(ScrollItemData))
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RInventory) == false)
		return;

// 	FHERO* HeroInfo = RGameInstance->HeroInventory->GetInventoryDataWithUD(ScrollItemData->HeroIconInfo.heroUD);
// 	if (HeroInfo != nullptr)
// 	{
// 		if (IsValid(RuneText))
// 		{
// 			RuneText->SetText(FText::AsNumber(HeroInfo->rune));
// 		}
// 		if (IsValid(Switcher_HeroOwn))
// 		{
// 			Switcher_HeroOwn->SetActiveWidgetIndex(HeroInfo->activated);
// 		}
// 	}

	FCurrencyPropertyTableInfo* CurrencyProperty = nullptr;
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ScrollItemData->HeroIconInfo.heroId);
	if (CharacterTableInfo)
	{
		if (IsValid(JobImage))
		{
			UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(CharacterTableInfo->Class);
			UUIFunctionLibrary::SetImageBrush(Sprite, JobImage);
		}
		if (IsValid(BackImage))
		{
			FString text = TEXT("Campaign_BG_Hero_") + CharacterTableInfo->DisplayName.ToString();
			BackImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(FName(*text)));
		}
		if (IsValid(FrontHeroText))
		{
			FrontHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Nick));
		}
		if (IsValid(BackHeroText))
		{
			BackHeroText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		}
		if (IsValid(Switcher_HeroUnlockType))
		{
			if (EUIMenuIDEnum::VE_DetailHeros == CharacterTableInfo->AcquisitionCondition)
			{
				Switcher_HeroUnlockType->SetActiveWidgetIndex(0);
			}
			else
			{
				Switcher_HeroUnlockType->SetActiveWidgetIndex(1);
			}
			
		}
// 		if (IsValid(CurrencyValue))
// 		{
// 			ESlateVisibility CurrencyVisible = ESlateVisibility::Collapsed;
// 			if (0 < CharacterTableInfo->Currency1Value)
// 			{
// 				CurrencyProperty = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(CharacterTableInfo->Currency1);
// 				if (CurrencyProperty != nullptr)
// 				{
// 					CurrencyValue->SetCurrency(CurrencyProperty->CurrencyType, CharacterTableInfo->Currency1Value);
// 					CurrencyVisible = ESlateVisibility::SelfHitTestInvisible;
// 				}
// 			}
// 			CurrencyValue->SetVisibility(CurrencyVisible);
// 		}
// 		if (IsValid(CurrencyValue2))
// 		{
// 			ESlateVisibility CurrencyVisible = ESlateVisibility::Collapsed;
// 			if (0 < CharacterTableInfo->Currency2Value)
// 			{
// 				CurrencyProperty = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(CharacterTableInfo->Currency2);
// 				if (CurrencyProperty != nullptr)
// 				{
// 					CurrencyValue2->SetCurrency(CurrencyProperty->CurrencyType, CharacterTableInfo->Currency2Value);
// 					CurrencyVisible = ESlateVisibility::SelfHitTestInvisible;
// 				}
// 			}
// 			CurrencyValue2->SetVisibility(CurrencyVisible);
// 		}
	}
}

void UUC_HeroLink::OnClickSelected(URScrollItem* InSelectScrollItem)
{
	/*
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->HeroInventory) == false)
		return;

	if (false == IsValid(this->ItemData))
		return;

	UHeroScrollItemData* ScrollItemData = Cast<UHeroScrollItemData>(this->ItemData);
	if (false == IsValid(ScrollItemData))
		return;

	RGameInstance->HeroInventory->SetHeroUDInHeroManagement(ScrollItemData->HeroIconInfo.heroUD);

	// Link to Hero Manager
	UUIFunctionLibrary::GoContents(GetWorld(), EUIMenuIDEnum::VE_Heros);
	*/
}

