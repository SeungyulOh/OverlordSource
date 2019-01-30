// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Common/HeroScrollItemData.h"
#include "UI/HeroManagement/UP_HeroInventory.h"

#include "HeroBaseScrollItem.h"

void UHeroBaseScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	//todo del
	if (HeroIcon)
	{
		HeroIcon->SetIsSelectEnable(true);
	}
}

void UHeroBaseScrollItem::InvalidateData()
{
	Super::InvalidateData();
	UHeroInventory_ScrollData* ScrollItemData = Cast<UHeroInventory_ScrollData>(ItemData);
	if (ScrollItemData == nullptr)
		return;

	SetHeroIconInfoWithUD(ScrollItemData->HeroUD);
}

void UHeroBaseScrollItem::SelectIconHero()
{
	if (IsSelectEnable == false)
		return;
	
	if (HeroIcon)
	{
		HeroIcon->SetIsSelectEnable(IsSelectEnable);
		HeroIcon->SelectIconHero();
	}
}

// void UHeroBaseScrollItem::SetHeroIconInfo(const FHERO_ICON_INFO& InHeroIconInfo)
// {
// 	if (HeroIcon)
// 	{
// 		HeroIcon->SetHeroIconInfo(InHeroIconInfo);
// 	}
// }

void UHeroBaseScrollItem::SetHeroIconInfoWithUD(FString InHeroUD, EHeroOwnershipType InOwnerShip /*= EHeroOwnershipType::VE_Me*/)
{
	if (HeroIcon)
	{
		HeroIcon->SetHeroIconInfoWithUD(InHeroUD, InOwnerShip);
	}
}

void UHeroBaseScrollItem::SetHeroIconInfoWithID(FName InHeroID, EHeroOwnershipType InOwnerShip /*= EHeroOwnershipType::VE_Me*/)
{
	if (HeroIcon)
	{
		HeroIcon->SetHeroIconInfoWithID(InHeroID, InOwnerShip);
	}
}

void UHeroBaseScrollItem::SetHeroIconInfoForInven(const FHERO_ICON_INFO& InHeroIconInfo)
{
	if (HeroIcon)
	{
		HeroIcon->SetHeroIconInfoForInven(InHeroIconInfo);
	}
}

void UHeroBaseScrollItem::SetSeletingImage()
{	
	switch (SelectState)
	{
	case ESelectType::VE_None:
		HeroIcon->GetWidgetSelect()->SetVisibility(ESlateVisibility::Hidden);

		break;
	case ESelectType::VE_Selected:
		HeroIcon->GetWidgetSelect()->SetVisibility(ESlateVisibility::Visible);
		HeroIcon->GetWidgetSelect()->SetActiveWidgetIndex((int32)SelectState);
		HeroIcon->Image_Seleting->SetVisibility(ESlateVisibility::Visible);
		HeroIcon->Image_Seleting->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f));

		break;
	case ESelectType::VE_Selecting:
		HeroIcon->GetWidgetSelect()->SetVisibility(ESlateVisibility::Visible);
		HeroIcon->GetWidgetSelect()->SetActiveWidgetIndex((int32)SelectState);
		HeroIcon->Image_Seleting->SetVisibility(ESlateVisibility::Visible);

		break;
	}
	UHeroScrollItemData* HSIData = Cast<UHeroScrollItemData>(ItemData);
	if (HSIData)
	{
		//set select data to itemdata and iconinfo
		switch (SelectState)
		{
		case ESelectType::VE_None:
			HSIData->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
			HeroIcon->GetHeroIconInfoRef()->eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
			break;
		case ESelectType::VE_Selecting:
			HSIData->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Using;
			HeroIcon->GetHeroIconInfoRef()->eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Using;
			break;
		case ESelectType::VE_Selected:
			HSIData->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Selected;
			HeroIcon->GetHeroIconInfoRef()->eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Selected;
			break;
		}
	}
}

void UHeroBaseScrollItem::Selected_OnlyForDeck()
{
	Selected();
}

void UHeroBaseScrollItem::DeselectthisItem()
{
	SelectState = ESelectType::VE_None;
	SetSeletingImage();
// 	IsSelected = true;
// 	Selected();
}

void UHeroBaseScrollItem::SetItemDataSource(int32 InIndex, URScrollItemData* InItem, URScrollView* InScrollView)
{
	ItemData = Cast<UHeroScrollItemData>(InItem);
	UHeroScrollItemData* hero_data = Cast<UHeroScrollItemData>(ItemData);
	if (hero_data == nullptr)
		return;
	
	HeroIcon->SetHeroIconInfoCopy(hero_data->HeroIconInfo);
	FHERO_ICON_INFO* heroIconData = HeroIcon->GetHeroIconInfoRef();
	heroIconData->eIconSelectType = hero_data->HeroIconInfo.eIconSelectType;

	if (hero_data->IsLock)
		HeroIcon->HideLockPanel(false);
}

void UHeroBaseScrollItem::SetSelected(bool bSelected)
{
	UHeroScrollItemData* HSIData = Cast<UHeroScrollItemData>(ItemData);
	if (HSIData)
	{
		if (bSelected)
		{
			HSIData->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Selected;
			HeroIcon->GetHeroIconInfoRef()->eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_Selected;
		}
		else
		{
			HSIData->HeroIconInfo.eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
			HeroIcon->GetHeroIconInfoRef()->eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
		}
	}
	SelectIconHero();
}

void UHeroBaseScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	SetItemDataSource(0, item, ScrollView);
}