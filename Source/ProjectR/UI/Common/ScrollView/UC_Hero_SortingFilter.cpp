// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Hero_SortingFilter.h"

#include "Utils/UtilFunctionLibrary.h"
#include "UI/HeroManagement/UC_HeroFilterButton.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"

#include "GameInfo/RInventory.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "UP_GenericScrollview.h"

#include "Utils/GamePlayBluePrintFunction.h"
#include "Utils/UIFunctionLibrary.h"
#include "Utils/Client_BattleFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"


void UUC_Hero_SortingFilter::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_AttackPower->SortingBtn)
	{
		Button_AttackPower->SortingBtn->OnClicked.Clear();
		Button_AttackPower->SortingBtn->OnClicked.AddDynamic(this, &UUC_Hero_SortingFilter::OnClick_AttackPower);
		Button_AttackPower->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_Power"))));
		Button_AttackPower->CheckStateSwitcher->SetActiveWidgetIndex(1);
	}

	if (Button_Level->SortingBtn)
	{
		Button_Level->SortingBtn->OnClicked.Clear();
		Button_Level->SortingBtn->OnClicked.AddDynamic(this, &UUC_Hero_SortingFilter::OnClick_Level);
		Button_Level->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_Level"))));
		Button_Level->CheckStateSwitcher->SetActiveWidgetIndex(1);
	}
	if (Button_Grade->SortingBtn)
	{
		Button_Grade->SortingBtn->OnClicked.Clear();
		Button_Grade->SortingBtn->OnClicked.AddDynamic(this, &UUC_Hero_SortingFilter::OnClick_Grade);
		Button_Grade->Text_Type->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("HeroUI_ListMethod_Grade"))));
		Button_Grade->CheckStateSwitcher->SetActiveWidgetIndex(1);
	}


	
	
}

void UUC_Hero_SortingFilter::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Hero_SortingFilter::OnClick_AttackPower()
{
	int32 CurrentIdx = Button_AttackPower->SortTypeSwitcher->GetActiveWidgetIndex();
	bDescending = CurrentIdx == 0 ? true :false;
	CurrentType = EHeroListFilterEnum::VE_AttackPower;

	Filtering();

	Button_AttackPower->SortTypeSwitcher->SetActiveWidgetIndex((int32)bDescending);
	Render_Enable(Button_AttackPower);
}

void UUC_Hero_SortingFilter::OnClick_Level()
{
	int32 CurrentIdx = Button_Level->SortTypeSwitcher->GetActiveWidgetIndex();
	bDescending = CurrentIdx == 0 ? true : false;
	CurrentType = EHeroListFilterEnum::VE_Level;

	Filtering();

	Button_Level->SortTypeSwitcher->SetActiveWidgetIndex((int32)bDescending);
	Render_Enable(Button_Level);
}

void UUC_Hero_SortingFilter::OnClick_Grade()
{
	int32 CurrentIdx = Button_Grade->SortTypeSwitcher->GetActiveWidgetIndex();
	bDescending = CurrentIdx == 0 ? true : false;
	CurrentType = EHeroListFilterEnum::VE_Grade;
	
	Filtering();

	Button_Grade->SortTypeSwitcher->SetActiveWidgetIndex((int32)bDescending);
	Render_Enable(Button_Grade);
}


void UUC_Hero_SortingFilter::Render_Enable(UUC_HeroFilterButton* Exception)
{
	Exception->CheckStateSwitcher->SetActiveWidgetIndex(0);
	if (Button_AttackPower != Exception)
		Button_AttackPower->CheckStateSwitcher->SetActiveWidgetIndex(1);
	if (Button_Level != Exception)
		Button_Level->CheckStateSwitcher->SetActiveWidgetIndex(1);
	if (Button_Grade != Exception)
		Button_Grade->CheckStateSwitcher->SetActiveWidgetIndex(1);
}

void UUC_Hero_SortingFilter::Filtering()
{
	TArray<FString> OutUDs;
	FilterHelper.Filtering(OutUDs, CurrentType, bDescending);

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
		GenericScrollview->Recompose_Itemdatalist(OutUDs);
}

void FFilterHelper::Filtering(TArray<FString>& OutHeroUDs, EHeroListFilterEnum type, bool bDescending)
{
	URInventory* Inventory = RGAMEINSTANCE(GEngine)->RInventory;
	
	TArray<FString> HeroUDs;
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		for (size_t i = 0; i < GenericScrollview->Variables.ScrollView->ItemDataList.Num(); ++i)
		{
			UUC_GenericScrollItemData* itemdata = Cast<UUC_GenericScrollItemData>(GenericScrollview->Variables.ScrollView->ItemDataList[i]);
			if (itemdata)
			{
				HeroUDs.Emplace(itemdata->ItemUD);
			}
		}
	}

	switch (type)
	{
		case EHeroListFilterEnum::VE_AttackPower:
		{
			Algo::Sort(HeroUDs, [Inventory, bDescending](FString Source , FString Dest) {
				FHERO* SourceData = Inventory->GetHeroWithUD(Source);
				FHERO* DestData = Inventory->GetHeroWithUD(Dest);
				int32 Sourcepower = UClient_BattleFunctionLibrary::GetHeroCombatStat(SourceData->heroUD);
				int32 Destpower = UClient_BattleFunctionLibrary::GetHeroCombatStat(DestData->heroUD);
					
				if (bDescending)
					return Sourcepower >= Destpower;
				else
					return Sourcepower <= Destpower;
			});
			break;
		}

		case EHeroListFilterEnum::VE_Level:
		{
			Algo::Sort(HeroUDs, [Inventory, bDescending](FString Source, FString Dest) {
				FHERO* SourceData = Inventory->GetHeroWithUD(Source);
				FHERO* DestData = Inventory->GetHeroWithUD(Dest);

				if (bDescending)
					return SourceData->level >= DestData->level;
				else
					return SourceData->level <= DestData->level;
			});
			break;
		}

		case EHeroListFilterEnum::VE_Grade:
		{
			Algo::Sort(HeroUDs, [Inventory, bDescending](FString Source, FString Dest) {
				FHERO* SourceData = Inventory->GetHeroWithUD(Source);
				FHERO* DestData = Inventory->GetHeroWithUD(Dest);

				FCharacterTableInfo* SourceInfo = UTableManager::GetInstancePtr()->GetCharacterRow(SourceData->heroId);
				FCharacterTableInfo* DestInfo = UTableManager::GetInstancePtr()->GetCharacterRow(DestData->heroId);

				if (bDescending)
				{
					if (SourceInfo->Grade == DestInfo->Grade)
						return SourceData->level >= DestData->level;
					else
						return SourceInfo->Grade >= DestInfo->Grade;
				}
				else
				{
					if (SourceInfo->Grade == DestInfo->Grade)
						return SourceData->level <= DestData->level;
					else
						return SourceInfo->Grade <= DestInfo->Grade;
				}
			});
			break;
		}
	}

	OutHeroUDs = HeroUDs;
}

void FFilterHelper::MultiFiltering(TArray<FString>& OutHeroUDs, EHeroInventoryType InvenType , TArray<CheckBoxOrder> Orders)
{
	URInventory* Inventory = RGAMEINSTANCE(GEngine)->RInventory;
	TArray<FString> InitialHeroUDs = OutHeroUDs;
	if (!Orders.Num())
	{
		OutHeroUDs = InitialHeroUDs;
		return;
	}

	/*1. Class Type Check*/

	auto ClassFilter = [InitialHeroUDs , Inventory](TArray<FString>& OutArray , EClassTypeEnum classtype) {
		for (size_t i = 0; i < InitialHeroUDs.Num(); ++i)
		{
			FString heroUD = InitialHeroUDs[i];
			FHERO* heroinfo = Inventory->GetHeroWithUD(heroUD);
			FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(heroinfo->heroId);
			if (tableinfo)
			{
				if (tableinfo->Class == classtype)
				{
					OutArray.AddUnique(heroUD);
				}
			}
		}
	};

	TArray<FString> ClassTypeResult;
	if (Orders.Contains(CheckBoxOrder::Tanker))
		ClassFilter(ClassTypeResult, EClassTypeEnum::VE_Tanker);
	if (Orders.Contains(CheckBoxOrder::MeleeDealer))
		ClassFilter(ClassTypeResult, EClassTypeEnum::VE_MeleeDealer);
	if (Orders.Contains(CheckBoxOrder::RangeDealer))
		ClassFilter(ClassTypeResult, EClassTypeEnum::VE_RangeDealer);
	if (Orders.Contains(CheckBoxOrder::MagicDealer))
		ClassFilter(ClassTypeResult, EClassTypeEnum::VE_MagicDealer);
	if (Orders.Contains(CheckBoxOrder::Supporter))
		ClassFilter(ClassTypeResult, EClassTypeEnum::VE_Supporter);
	/*1. Class Type Check End*/
		
	/*2. Grade Check*/
	TArray<int32> GradeArray;
	if (Orders.Contains(CheckBoxOrder::GradeOne))
		GradeArray.Emplace(1);
	if (Orders.Contains(CheckBoxOrder::GradeTwo))
		GradeArray.Emplace(2);
	if (Orders.Contains(CheckBoxOrder::GradeThree))
		GradeArray.Emplace(3);
	if (Orders.Contains(CheckBoxOrder::GradeFour))
		GradeArray.Emplace(4);
	if (Orders.Contains(CheckBoxOrder::GradeFive))
		GradeArray.Emplace(5);
	if (Orders.Contains(CheckBoxOrder::GradeSix))
		GradeArray.Emplace(6);
	if (Orders.Contains(CheckBoxOrder::GradeSeven))
		GradeArray.Emplace(7);
	if (Orders.Contains(CheckBoxOrder::GradeEight))
		GradeArray.Emplace(8);
	if (Orders.Contains(CheckBoxOrder::GradeNine))
		GradeArray.Emplace(9);
	if (Orders.Contains(CheckBoxOrder::GradeTen))
		GradeArray.Emplace(10);

	if (GradeArray.Num())
	{
		if (!ClassTypeResult.Num())
			ClassTypeResult = InitialHeroUDs;

		for (size_t j = 0; j < ClassTypeResult.Num();)
		{
			FHERO* hero = Inventory->GetHeroWithUD(ClassTypeResult[j]);
			if (hero)
			{
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (!GradeArray.Contains(tableinfo->Grade))
				{
					ClassTypeResult.RemoveAt(j);
					continue;
				}
			}
			++j;
		}
	}
	/*2. Grade Check End*/

	/*3. Continent Check*/
	TArray<EContinentTypeEnum> ContinentArray;
	if (Orders.Contains(CheckBoxOrder::Lux))
		ContinentArray.Emplace(EContinentTypeEnum::Lux);
	if (Orders.Contains(CheckBoxOrder::Solona))
		ContinentArray.Emplace(EContinentTypeEnum::Solona);
	if (Orders.Contains(CheckBoxOrder::Luna))
		ContinentArray.Emplace(EContinentTypeEnum::Luna);
	if (Orders.Contains(CheckBoxOrder::Nox))
		ContinentArray.Emplace(EContinentTypeEnum::Nox);

	if (ContinentArray.Num())
	{
		if (!ClassTypeResult.Num())
			ClassTypeResult = InitialHeroUDs;

		for (size_t j = 0; j < ClassTypeResult.Num();)
		{
			FHERO* hero = Inventory->GetHeroWithUD(ClassTypeResult[j]);
			if (hero)
			{
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (tableinfo && !ContinentArray.Contains(tableinfo->Continent))
				{
					ClassTypeResult.RemoveAt(j);
					continue;
				}
			}
			++j;
		}
	}


	/*3. Continent Check End*/

	OutHeroUDs = ClassTypeResult;
	
}

void UUC_Hero_MultiFilter::NativeConstruct()
{
	Super::NativeConstruct();

	for (size_t i = 0; i < CheckboxArray.Num(); ++i)
	{
		if (CheckboxArray.IsValidIndex(i))
			CheckboxArray[i]->OnCheckStateChanged.AddDynamic(this, &UUC_Hero_MultiFilter::MultiFiltering);
	}

	if (IsValid(ResetButton))
	{
		ResetButton->OnClicked.AddDynamic(this, &UUC_Hero_MultiFilter::OnClicked_Button_Reset);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.AddDynamic(this, &UUC_Hero_MultiFilter::OnClicked_Button_Close);
	}

	

}

void UUC_Hero_MultiFilter::NativeDestruct()
{
	if(IsValid(ResetButton))
		ResetButton->OnClicked.RemoveDynamic(this, &UUC_Hero_MultiFilter::OnClicked_Button_Reset);
	if(IsValid(CloseButton))
		CloseButton->OnClicked.RemoveDynamic(this, &UUC_Hero_MultiFilter::OnClicked_Button_Close);

	for (size_t i = 0; i < CheckboxArray.Num(); ++i)
	{
		if (CheckboxArray.IsValidIndex(i))
			CheckboxArray[i]->OnCheckStateChanged.RemoveDynamic(this, &UUC_Hero_MultiFilter::MultiFiltering);
	}

	Super::NativeDestruct();
}

void UUC_Hero_MultiFilter::OnClicked_Button_Reset()
{
	for (size_t i = 0; i < CheckboxArray.Num(); ++i)
	{
		if (CheckboxArray.IsValidIndex(i) && CheckboxArray[i]->GetIsEnabled())
			CheckboxArray[i]->SetIsChecked(false);
	}
	MultiFiltering(false);
}

void UUC_Hero_MultiFilter::OnClicked_Button_Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Hero_MultiFilter::MultiFiltering(bool isChecked)
{
	
	TArray<CheckBoxOrder> Orders;
	for (size_t i = 0; i < CheckboxArray.Num(); ++i)
	{
		if (CheckboxArray[i]->IsChecked() && CheckboxArray[i]->GetIsEnabled())
			Orders.Emplace((CheckBoxOrder)i);
	}

	
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		TArray<FString> OutHeroUDs = GenericScrollview->Variables.CachedItemUDList;
		FilterHelper.MultiFiltering(OutHeroUDs, inven_Type, Orders);
		GenericScrollview->Recompose_Itemdatalist(OutHeroUDs);
		GenericScrollview->Variables.UC_Sorting->Filtering();
	}
	
}

void UUC_Hero_MultiFilter::Render(EScrollviewType eType)
{
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnClicked_Button_Reset();
	
	switch (eType)
	{
	case EScrollviewType::VE_HERO_PROTECTED:
	case EScrollviewType::VE_HERO_NORMAL:
	{
		ClassBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		StarBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		AreaBox->SetVisibility(ESlateVisibility::Collapsed);
	}
		break;

	case EScrollviewType::VE_SPIRIT:
	{
		ClassBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		StarBox->SetVisibility(ESlateVisibility::Collapsed);
		AreaBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
		break;

	default:
		break;
	}
}
