// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ItemLevelUp.h"

#include "Table/ItemTableInfo.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"


#include "GameInfo/RInventory.h"

#include "UC_GenericPopup.h"
#include "UC_HeroItemInfo.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UC_StatChange.h"
#include "UC_EnchantMaterialButton.h"
#include "UP_ItemEnchantResult_NewDesign.h"
#include "UP_GenericSplash.h"
#include "UC_ItemLevelUpResult.h"
#include "UC_ExpBar.h"
#include "UC_ItemEnchant.h"
#include "SharedConstants/WalletConstants.h"


#define LEVELUP_RATE 0.01


void UUP_ItemLevelUp::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(variables.Button_LevelUp))
		variables.Button_LevelUp->OnClicked.AddDynamic(this, &UUP_ItemLevelUp::OnButtonLevelUpClicked);
	if(IsValid(variables.Slider_SelectedAmount))
		variables.Slider_SelectedAmount->OnValueChanged.AddDynamic(this, &UUP_ItemLevelUp::OnSliderValueChanged);
	if(IsValid(variables.Button_Slider_Minus))
		variables.Button_Slider_Minus->OnClicked.AddDynamic(this, &UUP_ItemLevelUp::OnButtonSliderMinusClicked);
	if(IsValid(variables.Button_Slider_Plus))
		variables.Button_Slider_Plus->OnClicked.AddDynamic(this, &UUP_ItemLevelUp::OnButtonSliderPlusClicked);
	if(IsValid(variables.Button_LevelupMax))
		variables.Button_LevelupMax->OnClicked.AddDynamic(this, &UUP_ItemLevelUp::OnButtonMaxClicked);
	if (IsValid(variables.Button_Cancel))
		variables.Button_Cancel->OnClicked.AddDynamic(this, &UUP_ItemLevelUp::OnClick_Button_Close);
	if (IsValid(variables.Button_Exit))
		variables.Button_Exit->OnClicked.AddDynamic(this, &UUP_ItemLevelUp::OnClick_Button_Close);
}

void UUP_ItemLevelUp::NativeDestruct()
{
	if (IsValid(variables.Button_LevelUp))
		variables.Button_LevelUp->OnClicked.RemoveDynamic(this, &UUP_ItemLevelUp::OnButtonLevelUpClicked);
	if (IsValid(variables.Slider_SelectedAmount))
		variables.Slider_SelectedAmount->OnValueChanged.RemoveDynamic(this, &UUP_ItemLevelUp::OnSliderValueChanged);
	if (IsValid(variables.Button_Slider_Minus))
		variables.Button_Slider_Minus->OnClicked.RemoveDynamic(this, &UUP_ItemLevelUp::OnButtonSliderMinusClicked);
	if (IsValid(variables.Button_Slider_Plus))
		variables.Button_Slider_Plus->OnClicked.RemoveDynamic(this, &UUP_ItemLevelUp::OnButtonSliderPlusClicked);
	if (IsValid(variables.Button_LevelupMax))
		variables.Button_LevelupMax->OnClicked.RemoveDynamic(this, &UUP_ItemLevelUp::OnButtonMaxClicked);
	if (IsValid(variables.Button_Cancel))
		variables.Button_Cancel->OnClicked.RemoveDynamic(this, &UUP_ItemLevelUp::OnClick_Button_Close);
	if (IsValid(variables.Button_Exit))
		variables.Button_Exit->OnClicked.RemoveDynamic(this, &UUP_ItemLevelUp::OnClick_Button_Close);

	Super::NativeDestruct();
}

void UUP_ItemLevelUp::InvalidateData()
{
	Super::InvalidateData();

	check(!ItemUD.IsEmpty());

	ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (!ensure(ItemData != nullptr))
		return;
	ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	BaseLevel = ItemData->level;
	LevelAfter = BaseLevel;
	int32 MaxLevel = ItemInfo->MaxLevel;
	BaseLevel >= MaxLevel ? SetLevelupWidgetEnable(false) : SetLevelupWidgetEnable(true);

	if(variables.ItemIcon->EquipmentIcon)
 		variables.ItemIcon->EquipmentIcon->Refresh(ItemInfo, ItemData, false, false);

	FText EquipmentName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName);

	if(variables.Text_ItemName)
		variables.Text_ItemName->SetText(EquipmentName);
// 
// 	if (variables.Text_EquipPosition)
// 		variables.Text_EquipPosition->SetText(UDescriptionFunctionLibrary::GetItemEquipPositionToText(ItemInfo->EquipPosition));

	if (variables.Text_ItemLevel)
		variables.Text_ItemLevel->SetText(FText::FromString(FString::FromInt(BaseLevel) + FString(" / ") + FString::FromInt(MaxLevel)));

	if (variables.Text_ItemEnchant)
		variables.Text_ItemEnchant->SetText(FText::FromString(FString("+ ") + FString::FromInt(ItemData->enchant)));

	if (variables.Text_MaterialName)
	{
		switch (ItemInfo->EquipPosition)
		{
		case EItemEquipPositionEnum::VE_Weapon:
			variables.Text_MaterialName->SetText(TEXT("S_CET00100300"), EStringTableType::VE_Common);
			break;
		case EItemEquipPositionEnum::VE_Armor:
			variables.Text_MaterialName->SetText(TEXT("S_CET00200300"), EStringTableType::VE_Common);
			break;
		case EItemEquipPositionEnum::VE_Pants:
			variables.Text_MaterialName->SetText(TEXT("S_CET00300300"), EStringTableType::VE_Common);
			break;
		case EItemEquipPositionEnum::VE_Necklace:
		case EItemEquipPositionEnum::VE_Ring:
			variables.Text_MaterialName->SetText(TEXT("S_CET00400300"), EStringTableType::VE_Common);
			break;
		default:
			break;
		}
	}

	TArray<int32> CurrentOption;
	GetBasicStats(CurrentOption);
	SetCurrentStats(CurrentOption);

	FItemLevelUpTableInfo* LevelUpInfo = FItemLevelUpTableInfo::LookUp(ItemInfo->EquipType, ItemInfo->ItemGrade, BaseLevel);
	if (!ensure(LevelUpInfo != nullptr))
		return;
	FWalletTableInfo* MaterialInfo = UTableManager::GetInstancePtr()->GetWalletRow(LevelUpInfo->NeedCurrency);
	if (!ensure(MaterialInfo != nullptr))
		return;

	//MaterialCount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(LevelUpInfo->NeedCurrency);
	variables.MaterialIcon->Refresh(MaterialInfo->ItemIconKey, 0, false, 0);


	RefreshUI();
// 	FText MaterialName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, MaterialInfo->CommonStringKey);
// 	Text_MaterialName->SetText(MaterialName);

// 	TArray<int32> ExpsByLevel;
// 	int32 ExpTillMaxLevel = -ItemData->exp;
// 	for (int32 i = ItemData->level; i <= ItemInfo->MaxLevel - 1; i++)
// 	{
// 		FItemLevelUpTableInfo* Info = FItemLevelUpTableInfo::LookUp(ItemInfo->EquipType, ItemInfo->ItemGrade, i);
// 		if (!ensure(Info != nullptr))
// 			return;
// 
// 		ExpsByLevel.Push(Info->NeedExp);
// 
// 		ExpTillMaxLevel += Info->NeedExp;
// 	}
// 
// 	// ExpBar
// 	int32 CurrentExp = ItemData->exp;
// 
// 	ExpBar->ExpsByLevel = ExpsByLevel;
// 	BaseExp = ItemData->exp;

	// Gold
	MaxGold = RGAMEINSTANCE(this)->RInventory->GetCurrency(WALLET_GOLD);
	GoldCostRate = LevelUpInfo->NeedGold;

	// MaxMaterialCount is min(MaterialCount, ExpTillMaxLevel, MaxGold / GoldCostRate)
// 	MaxMaterialCount = ExpTillMaxLevel < MaterialCount ? ExpTillMaxLevel : MaterialCount;
// 	MaxMaterialCount = MaxGold / GoldCostRate < MaxMaterialCount ? MaxGold / GoldCostRate : MaxMaterialCount;

	//CurrentMaterialCount = 0;
	InvalidateOnCurrentMaterialCountChanged();
}

void UUP_ItemLevelUp::RefreshUI()
{
	RefreshLevelUpChangeText();
	TArray<int32> BasicStats;
	GetBasicStats(BasicStats);
	RefreshStatChanges(BasicStats, LevelAfter - ItemData->level);
	RefreshSliderWidget(ItemInfo);
	RefreshLevelupButton();
}

void UUP_ItemLevelUp::SetItemUD(FString InItemUD)
{
	ItemUD = InItemUD;
}

void UUP_ItemLevelUp::SetCurrentStats(TArray<int32> options)
{
	if (variables.BasicStat1)
		variables.BasicStat1->SetVisibility(ESlateVisibility::Collapsed);
	if (variables.BasicStat2)
		variables.BasicStat2->SetVisibility(ESlateVisibility::Collapsed);
	if (variables.Substat1)
		variables.Substat1->SetVisibility(ESlateVisibility::Collapsed);
	if (variables.Substat2)
		variables.Substat2->SetVisibility(ESlateVisibility::Collapsed);

	bool Option1 = false;
	bool Option2 = false;
	bool Option3 = false;
	bool Option4 = false;

	for (int32 i = 0; i < options.Num(); i++)
	{
		if (options[i] > 0)
		{
			if (!Option1) //1¹ø ½ºÅÈ
			{
				variables.BasicStat1->SetVisibility(ESlateVisibility::Visible);
				variables.BasicStat1->Refresh((EItemStatOption)i, options[i], false);
				Option1 = true;
			}
			else if (!Option2) //2¹ø ½ºÅÈ
			{
				variables.BasicStat2->SetVisibility(ESlateVisibility::Visible);
				variables.BasicStat2->Refresh((EItemStatOption)i, options[i], false);
				Option2 = true;
			}
			else if (!Option3) //3¹ø ½ºÅÈ
			{
				variables.Substat1->SetVisibility(ESlateVisibility::Visible);
				variables.Substat1->Refresh((EItemStatOption)i, options[i], false);
				Option3 = true;
			}
			else if (!Option4) //4¹ø ½ºÅÈ
			{
				variables.Substat2->SetVisibility(ESlateVisibility::Visible);
				variables.Substat2->Refresh((EItemStatOption)i, options[i], false);
				Option4 = true;
			}
		}
	}
}

void UUP_ItemLevelUp::GetBasicStats(TArray<int32>& BasicStatArr)
{
	BasicStatArr.Emplace(ItemInfo->MaxHP);
	BasicStatArr.Emplace(ItemInfo->PhysicalAttackPower);
	BasicStatArr.Emplace(ItemInfo->MagicAttackPower);
	BasicStatArr.Emplace(ItemInfo->Armor);
	BasicStatArr.Emplace(ItemInfo->MagicResistance);
}

FName UUP_ItemLevelUp::GetStatName(EBasicStatType type)
{
	switch (type)
	{
	case EBasicStatType::MaxHP:
		return FName(TEXT("S_GOPT04"));
	case EBasicStatType::PhysicalAttackPower:
		return FName(TEXT("S_GOPT14"));
	case EBasicStatType::MagicAttackPower:
		return FName(TEXT("S_GOPT13"));
	case EBasicStatType::Armor:
		return FName(TEXT("S_GOPT07"));
	case EBasicStatType::MagicResistance:
		return FName(TEXT("S_GOPT10"));
	default:
		return FName("");
	}
}

void UUP_ItemLevelUp::RefreshSliderWidget(FItemTableInfo* itemInfo)
{
	FItemLevelUpTableInfo* LevelupInfo = FItemLevelUpTableInfo::LookUp(itemInfo->EquipType, itemInfo->ItemGrade, itemInfo->ItemLevel);
	FWalletTableInfo* MaterialInfo = UTableManager::GetInstancePtr()->GetWalletRow(LevelupInfo->NeedCurrency);
	if (!ensure(MaterialInfo != nullptr))
		return;

	switch (itemInfo->EquipPosition)
	{
	case EItemEquipPositionEnum::VE_Weapon:
		HaveCurrency = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_EXP_WEAPON01);
		break;
	case EItemEquipPositionEnum::VE_Armor:
	case EItemEquipPositionEnum::VE_Pants:
		HaveCurrency = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_EXP_ARMOR01);
		break;
	case EItemEquipPositionEnum::VE_Necklace:
	case EItemEquipPositionEnum::VE_Ring:
		HaveCurrency = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_EXP_ACCESSARY01);
		break;
	default:
		break;
	}

	NeedCurrency = 0;

	for (int32 i = BaseLevel; i < LevelAfter; i++)
	{
		FItemLevelUpTableInfo* levelupInfo = FItemLevelUpTableInfo::LookUp(itemInfo->EquipType, itemInfo->ItemGrade, i);
		NeedCurrency += levelupInfo->NeedExp;
	}

	variables.Text_CurrecyInProgressBar->SetText(FText::FromString(FString::FromInt(NeedCurrency) + FString(" / ") + FString::FromInt(HaveCurrency)));
	variables.MaterialIcon->Refresh(MaterialInfo->ItemIconKey, NeedCurrency, false, 0);
	if (NeedCurrency > HaveCurrency)
		variables.Text_CurrecyInProgressBar->SetColorAndOpacity(FLinearColor(1.f, 0.f , 0.f));
	else
		variables.Text_CurrecyInProgressBar->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));

	float barPer = (float)(LevelAfter - BaseLevel) / (float)(itemInfo->MaxLevel - BaseLevel);
	variables.Slider_SelectedAmount->SetValue(barPer);
	variables.Image_BackgroundBar->SetRenderScale(FVector2D(barPer, 1.f));
}

void UUP_ItemLevelUp::RefreshLevelupButton()
{
	NeedTotalGold = 0;
	int32 HaveGold = 0;

	HaveGold = RGAMEINSTANCE(this)->RInventory->GetGold();

	for (int32 i = BaseLevel; i < LevelAfter; i++)
	{
		FItemLevelUpTableInfo* levelupInfo = FItemLevelUpTableInfo::LookUp(ItemInfo->EquipType, ItemInfo->ItemGrade, i);

		NeedTotalGold += levelupInfo->NeedGold * levelupInfo->NeedExp;
	}
	//NeedTotalGold = NeedGold * NeedCurrency;
	//variables.Button_LevelUp->SetGoldAmount(NeedTotalGold);
	variables.Text_GoldEnabled->SetText(FText::AsNumber(NeedTotalGold));

	if (NeedTotalGold > HaveGold)
		//variables.Button_LevelUp->SetTextRed();
		variables.Text_GoldEnabled->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f));
	else
		variables.Text_GoldEnabled->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));
		//variables.Button_LevelUp->SetTextWhite();
	
	if (BaseLevel == LevelAfter)
		variables.Button_LevelUp->SetIsEnabled(false);
	else
		variables.Button_LevelUp->SetIsEnabled(true);
}

bool UUP_ItemLevelUp::IsInventoryIncludingEquipmentItem(const FItemTableInfo* Info, const FITEM* InItemData)
{
	// (design)
	// * do not show current item
	// * do not show items that can't be levelled up

	if (InItemData->itemUD == ItemUD)
		return false;

	if (!ItemInfo->LevelUpPossible || InItemData->level >= ItemInfo->MaxLevel)
		return false;

	return true;
}

void UUP_ItemLevelUp::OnSliderValueChanged(float Value)
{
	float GapValue = 1.f / (float)(ItemInfo->MaxLevel - BaseLevel);
	int32 SetLevel = Value / GapValue;
	if (SetLevel > 0)
	{
		LevelAfter = SetLevel + BaseLevel;
		RefreshUI();
	}
	else if (SetLevel == 0)
	{
		LevelAfter = BaseLevel;
		RefreshUI();
	}
// 	CurrentMaterialCount = FMath::RoundToInt((float)MaxMaterialCount * Value);
// 
// 	InvalidateOnCurrentMaterialCountChanged();
}

void UUP_ItemLevelUp::InvalidateOnCurrentMaterialCountChanged()
{
	// invalidate Text_MaterialCount, slider, Button_LevelUp.bDisabled, gold amount, ExpBar, stats

// 	Text_MaterialCount->SetText(FText::FromString(FString::Printf(TEXT("%d/ %d"), CurrentMaterialCount, MaterialCount)));
// 
// 	Slider_SelectedAmount->SetValue(MaxMaterialCount == 0 ? 0.0 : (float)CurrentMaterialCount / MaxMaterialCount);
// 
// 	Button_LevelUp->SetDisabled(CurrentMaterialCount == 0);
// 	Button_LevelUp->SetGoldAmount(CurrentMaterialCount * GoldCostRate);
// 
// 	ExpBar->CurrentValue = BaseExp + CurrentMaterialCount;
// 	ExpBar->InvalidateData();
// 
// 	// LevelAfter
// 	int32 Level;
// 	int32 _Residue;
// 	ExpBar->GetLevelAndResidue(Level, _Residue);
// 	int32 LevelAfter = BaseLevel + Level;
// 	Text_ItemLevelAfter->SetText(FText::AsNumber(LevelAfter));

	// stats
	//RefreshStatChanges(LevelAfter);
}

void UUP_ItemLevelUp::RefreshLevelUpChangeText()
{
	if (variables.Text_CurrentLevel)
		variables.Text_CurrentLevel->SetText(FText::AsNumber(BaseLevel));

	if (variables.Text_CurrentLevelWithMax)
		variables.Text_CurrentLevelWithMax->SetText(FText::AsNumber(LevelAfter));

	if (variables.Text_MaxLevel)
		variables.Text_MaxLevel->SetText(FText::AsNumber(ItemInfo->MaxLevel));
}

void UUP_ItemLevelUp::RefreshStatChanges(TArray<int32> OptionArr, int32 LevelGap)
{
	for (int32 i = 0; i < variables.LevelupStatArr.Num(); i++)
		variables.LevelupStatArr[i]->SetVisibility(ESlateVisibility::Collapsed);

	int32 widgetIndex = 0;

	for (int32 i = 0; i < OptionArr.Num(); i++)
	{
		if (OptionArr[i] == 0)
			continue;

		int32 CurrVal = OptionArr[i];
		int32 AddVal = UKismetMathLibrary::Round((float)OptionArr[i] * LEVELUP_RATE * LevelGap);
		int32 NextVal = CurrVal + AddVal;
		FName ValName = GetStatName((EBasicStatType)i);

		variables.LevelupStatArr[widgetIndex]->SetVisibility(ESlateVisibility::Visible);
		variables.LevelupStatArr[widgetIndex]->SetStat(FString::FromInt(CurrVal), FString::FromInt(NextVal), ValName.ToString(), FString::FromInt(AddVal));

		widgetIndex++;
	}

// 	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
// 	if (!ensure(ItemData != nullptr))
// 		return;


// 	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
// 	if (!ensure(ItemInfo != nullptr))
// 		return;

// 	FITEM NextLevelItemData = *ItemData;
// 	NextLevelItemData.level = LevelAfter;
// 
// 	TArray<int32> CurrentBasicOptions;
// 	ItemInfo->GetBasicOptions(CurrentBasicOptions);
// 
// 	TArray<int32> NextBasicOptions = CurrentBasicOptions;
// 
// 	FItemLevelUpTableInfo *ItemLevelUpInfo = FItemLevelUpTableInfo::LookUp(ItemInfo->EquipType, ItemInfo->ItemGrade, ItemData->level);
// 	if (ItemLevelUpInfo == nullptr)
// 		return;
// 
// 	NextBasicOptions[(int32)EItemStatOption::PhysicalAttackPower] += ItemLevelUpInfo->PlusPhysicalAttackPower * LevelAfter;
// 	NextBasicOptions[(int32)EItemStatOption::MagicAttackPower] += ItemLevelUpInfo->PlusMagicAttackPower * LevelAfter;
// 	NextBasicOptions[(int32)EItemStatOption::Armor] += ItemLevelUpInfo->PlusArmor * LevelAfter;
// 	NextBasicOptions[(int32)EItemStatOption::MagicResistance] += ItemLevelUpInfo->PlusMagicResistance * LevelAfter;
// 
// 	int32 StatIndex = 0;
// 	for (int32 i = 0; i < CurrentBasicOptions.Num(); i++)
// 	{
// 		if (CurrentBasicOptions[i] > 0)
// 		{
// 
// 		}
// 	}
// // 
// // 	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemData, CurrentBasicOptions);
// // 	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, &NextLevelItemData, NextBasicOptions);
// // 
// // 	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemData, CurrentBasicOptions);
// // 	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, &NextLevelItemData, NextBasicOptions);
// // 
// // 	TArray<UUC_StatChange*> EmptyArr;
// // 	UUC_ItemEnchant::RefreshStatChanges(BasicStatChanges, EmptyArr, CurrentBasicOptions, NextBasicOptions, true);
}

void UUP_ItemLevelUp::SetLevelupWidgetEnable(bool bEnable)
{
	variables.Slider_SelectedAmount->SetIsEnabled(bEnable);
	variables.Button_Slider_Minus->SetIsEnabled(bEnable);
	variables.Button_Slider_Plus->SetIsEnabled(bEnable);
	variables.Button_LevelupMax->SetIsEnabled(bEnable);
	variables.Button_LevelUp->SetIsEnabled(bEnable);
}

void UUP_ItemLevelUp::OnInventoryEquipmentItemSelected(FString SelectedItemUD)
{
	// swap and invalidate all

// 	ItemUD = SelectedItemUD;
// 	CurrentMaterialCount = 0;
// 
// 	InvalidateData();
// 
// 	check(IsValid(ItemInventoryPage));
// 
// 	ItemInventoryPage->bInventoryUpdated = true;
// 	ItemInventoryPage->InvalidateData();
}

void UUP_ItemLevelUp::OnButtonLevelUpClicked()
{
	check(ItemData != nullptr);

	if (!ensure(ItemInfo != nullptr))
		return;

	if (ItemData->level >= ItemInfo->MaxLevel)
		return;

	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemLevelUp.AddUObject(this, &UUP_ItemLevelUp::OnInventoryItemLevelUpRp, *ItemData);

	int32 HaveGold = 0;
	HaveGold = RGAMEINSTANCE(this)->RInventory->GetGold();

	if (NeedTotalGold > HaveGold)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UI_Currency_Warning_Gold")), FText());
		return;
	}
	
	if (NeedCurrency > HaveCurrency)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UI_ItemLevleUp_Impossible_SplashPopUp03")), FText());
		return;
	}
	
	UPacketFunctionLibrary::INVENTORY_ITEM_LEVELUP_RQ(ItemUD, NeedCurrency);
}

void UUP_ItemLevelUp::OnInventoryItemLevelUpRp(bool bSuccess, bool bIsLucky, FITEM ItemDataBefore)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryItemLevelUp.RemoveAll(this);
	
	if (ensure(bSuccess))
	{
 		const FITEM* ItemDataAfterEnchant = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
 		const FItemTableInfo* pItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemDataAfterEnchant->itemId);

		// Show BP UP_Splash_ItemEnchantResult
		
		auto Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_ItemLevelUpResult);
		check(IsValid(Splash));
		auto SplashContent = Cast<UUC_ItemLevelUpResult>(Splash->GetContentWidget());
		check(IsValid(SplashContent));

		SplashContent->IsLucky = bIsLucky;
		SplashContent->ItemUD = ItemUD;
		SplashContent->ItemDataBefore = ItemDataBefore;

		SplashContent->InvalidateData();

		// (design) do not close page

		if (ItemDataAfterEnchant->level == pItemInfo->MaxLevel)
			RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_ItemLevelUp);
		else
			InvalidateData();
	}
	else
	{
		//ClosePage();
	}
}

void UUP_ItemLevelUp::OnClick_Button_Close()
{
	RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_ItemLevelUp);
}

void UUP_ItemLevelUp::OnButtonSliderMinusClicked()
{
	if (LevelAfter <= BaseLevel)
		return;
	
	LevelAfter -= 1;
	RefreshUI();
}

void UUP_ItemLevelUp::OnButtonSliderPlusClicked()
{
	if (LevelAfter >= ItemInfo->MaxLevel)
		return;

	LevelAfter += 1;
	RefreshUI();
}

void UUP_ItemLevelUp::OnButtonMaxClicked()
{
	LevelAfter = ItemInfo->MaxLevel;

	RefreshUI();
}
