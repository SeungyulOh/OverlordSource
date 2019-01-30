// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Global/RGameInstance.h"
#include "GameInfo/RInventory.h"

#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"

#include "UI/Common/HeroScrollItemData.h"
#include "UI/Common/HeroBaseScrollItem.h"

#include "UI/RWidgetManager.h"

#include "Network/PacketFunctionLibrary.h"

#include "UC_HeroInfo_Strong_Material.h"
#include "Global/TableManager.h"

void UUC_HeroInfo_Strong_Material::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_HeroInfo_Strong_Material::NativeDestruct()
{
	if (IsValid(Button_Enchant))
		Button_Enchant->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_Enchant);
	if (IsValid(Button_AutoSelect))
		Button_AutoSelect->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_AutoSelect);
	if (IsValid(Button_Sort_Descending))
		Button_Sort_Descending->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_SortBtn);
	if (IsValid(Button_Filter_Show))
		Button_Filter_Show->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_FilterBtn);

	UC_SortingFilter->OnSelectedHeroLightFilter.RemoveDynamic(this, &UUC_HeroInfo_Strong_Material::OnReceive_LightFilter_Result);
	UC_CheckFilter->OnSelectedHeroDetailFilter.RemoveDynamic(this, &UUC_HeroInfo_Strong_Material::OnReceive_DetailFilter_Result);
	Super::NativeDestruct();
}

void UUC_HeroInfo_Strong_Material::Init()
{
	if (IsValid(UC_HeroScrollView))
	{
		UC_HeroScrollView->Init();
		UC_HeroScrollView->MultiSelect = true;
		//UC_HeroScrollView->SetLimitSelectCount(5);
	}
	if (IsValid(Text_ActiveCurrency))
		Text_ActiveCurrency->SetText(FText::AsNumber(0));

	if(IsValid(Button_Enchant))
		Button_Enchant->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_Enchant);
	if (IsValid(Button_AutoSelect))
		Button_AutoSelect->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_AutoSelect);
	if (IsValid(Button_Sort_Descending))
		Button_Sort_Descending->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_SortBtn);
	if (IsValid(Button_Sort_Ascending))
		Button_Sort_Ascending->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_SortBtn);
	if (IsValid(Button_Filter_Show))
		Button_Filter_Show->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong_Material::OnClick_FilterBtn);
	if (IsValid(UC_SortingFilter))
		UC_SortingFilter->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(UC_CheckFilter))
		UC_CheckFilter->SetVisibility(ESlateVisibility::Collapsed);

	UC_SortingFilter->OnSelectedHeroLightFilter.AddDynamic(this, &UUC_HeroInfo_Strong_Material::OnReceive_LightFilter_Result);
	UC_CheckFilter->OnSelectedHeroDetailFilter.AddDynamic(this, &UUC_HeroInfo_Strong_Material::OnReceive_DetailFilter_Result);
}

void UUC_HeroInfo_Strong_Material::SetHero(FString InHeroUD)
{
	m_HeroUD = InHeroUD;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_HeroMaterial_Popup[RInventory Is Empty]"));
	
	m_FilteringList.Empty();
	RGameInstance->RInventory->GetHeroUDs(m_FilteringList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_InventoryTypeAll);
	RGameInstance->RInventory->GetHeroUDs(m_FilteringList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_TypeSpirit);

	TArray<FString> TempList;
	for (FString& ud : m_FilteringList)
	{
		FHERO* HeroInvenData = RGameInstance->RInventory-> GetHeroWithUD(ud);
		auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
		if (HeroTableInfo->Grade != 6)
			TempList.Emplace(ud);
	}
	m_FilteringList.Empty();
	m_FilteringList = TempList;
	RefreshScollData(m_FilteringList);
}

void UUC_HeroInfo_Strong_Material::SetCost(int32 InCost)
{
	m_Cost = InCost;

	if(IsValid(Text_ActiveCurrency))
		Text_ActiveCurrency->SetText(FText::AsNumber(m_Cost));
}

void UUC_HeroInfo_Strong_Material::OnClick_SortBtn()
{
	if (IsValid(UC_SortingFilter))
	{
		if (UC_SortingFilter->GetVisibility() == ESlateVisibility::Collapsed)
		{
			if (UC_CheckFilter->GetVisibility() == ESlateVisibility::Visible)
				UC_CheckFilter->SetVisibility(ESlateVisibility::Collapsed);

			UC_SortingFilter->SetVisibility(ESlateVisibility::Visible);
			UC_SortingFilter->SetWidgetSwitcher(bIsDescending);
			Switcher_Sort->SetActiveWidgetIndex(1);
		}
		else
		{
			UC_SortingFilter->SetVisibility(ESlateVisibility::Collapsed);
			Switcher_Sort->SetActiveWidgetIndex(0);
		}
	}
}

void UUC_HeroInfo_Strong_Material::OnClick_FilterBtn()
{
	if (IsValid(UC_CheckFilter))
	{
		if (UC_CheckFilter->GetVisibility() == ESlateVisibility::Collapsed)
		{
			if (UC_SortingFilter->GetVisibility() == ESlateVisibility::Visible)
			{
				UC_SortingFilter->SetVisibility(ESlateVisibility::Collapsed);
				Switcher_Sort->SetActiveWidgetIndex(0);
			}

			UC_CheckFilter->SetVisibility(ESlateVisibility::Visible);
			UC_CheckFilter->SetSoulFilterMenu();
			UC_CheckFilter->CheckBox_Grade_Six->SetIsEnabled(false);
		}
		else
			UC_CheckFilter->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_HeroInfo_Strong_Material::OnClick_Enchant()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_HeroMaterial_Popup[RInventory Is Empty]"));

	FHERO*  m_Hero = RGameInstance->RInventory->GetHeroWithUD(m_HeroUD);

	int32 EnchantCount = 0;
	for (int32 Num : m_Hero->enchantStat)
		EnchantCount += Num;

	if (m_Hero->enchantPoint+EnchantCount >= 30)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroEnchant_SplashPopUp_Meassage01"))));
		return;
	}

	TArray<FString>	SelectSpiritUDList;

	SelectSpiritUDList.Empty();

	//if (UC_HeroScrollView)
	//	UC_HeroScrollView->GetSelectedHeroUDs(SelectSpiritUDList);

	if (SelectSpiritUDList.GetAllocatedSize() <= 0)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroEnchant_SplashPopUp_Meassage04"))));
		return;
	}
	if (m_Cost > RGameInstance->RInventory->GetGold())
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("USK_up_ItemManage_tip07"))));
		return;
	}

	UPacketFunctionLibrary::INVENTORY_HERO_ENCHANT_CONSUME_RQ(m_HeroUD, SelectSpiritUDList);
}

void UUC_HeroInfo_Strong_Material::OnClick_AutoSelect()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroMaterial_Popup[RGameInstance Is Empty]"));

	SetHero(m_HeroUD);

	m_FilteringList.Empty();
	RGameInstance->RInventory->GetSpiritUDs(m_FilteringList, 5, true);

	/*if (UC_HeroScrollView)
		UC_HeroScrollView->AlreadyCheckSelectedHero(m_FilteringList);*/
}

void UUC_HeroInfo_Strong_Material::OnReceive_LightFilter_Result(int32 eClassTypeEnum)
{
	bIsDescending = !bIsDescending;
	RGAMEINSTANCE(this)->RInventory->HeroChangeFilter(m_FilteringList, (EHeroListFilterEnum)eClassTypeEnum, EHeroInventoryType::VE_TypeSpirit, bIsDescending);
	RefreshScollData(m_FilteringList);
	UC_SortingFilter->SetVisibility(ESlateVisibility::Collapsed);
	Switcher_Sort->SetActiveWidgetIndex(0);
}

void UUC_HeroInfo_Strong_Material::OnReceive_DetailFilter_Result(TArray<FString> HeroList)
{
	RGAMEINSTANCE(this)->RInventory->GetHeroUDs(HeroList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_TypeSpirit);
	RefreshScollData(HeroList);
}

void UUC_HeroInfo_Strong_Material::RefreshScollData(TArray<FString> list)
{
	m_FilteringList = list;
	/*if (IsValid(UC_HeroScrollView))
	{
		UC_HeroScrollView->Init_NoClear(30);
		UC_HeroScrollView->ClearScrollItemData();
		UC_HeroScrollView->HasLimitSelect = true;
		UC_HeroScrollView->LimitSelectCount = 5;
		for (FString heroUD : m_FilteringList)
		{
			UHeroScrollItemData* HeroInvenScrollItemData = Cast<UHeroScrollItemData>(UC_HeroScrollView->AddScrollItemData(UHeroScrollItemData::StaticClass()));
			UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroInvenScrollItemData->HeroIconInfo, heroUD, EHeroOwnershipType::VE_Me);
		}
		UC_HeroScrollView->InvalidateData();
	}*/
}
