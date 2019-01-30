// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroItemInfo.h"
#include "UI/Common/UC_ItemIcon.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/ItemManagement/Item/HeroItemOptions_ScrollItemData.h"
#include "UI/Control/RScrollView.h"
#include "UI/RWidgetManager.h"
#include "UC_GenericPopup.h"
//#include "UC_PopupContent_ItemGrind.h"
#include "UC_ItemEnchant.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"
#include "UC_GenericPopup.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UtilFunctionIntegrated.h"
#include "UP_GenericScrollview.h"
#include "UP_ItemUpgrade.h"

void UUC_ItemInfo_ItemStat::Refresh(EItemStatOption ItemStatOption, int32 Value, bool IsAdditionalStat)
{
	// name
	Text_Name->LocalizingKey = UUIFunctionLibrary::ItemStatOptionToStringKey(ItemStatOption);
	Text_Name->ApplyLocalizedText();

	// value
	if (!IsAdditionalStat)
	{
		Text_Value->SetText(UUtilFunctionLibrary::ItemBasicOptionStatValueToText(ItemStatOption, Value));
	}
	else
	{
		Text_Value->SetText(UUtilFunctionLibrary::ItemAdditionalOptionStatValueToText(ItemStatOption, Value));
	}
}

void UUC_HeroItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ParentPopup))
	{
		//ParentPopup->Text_PopupTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Info_000000")));
		ParentPopup->Button_Close->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonCloseClicked);
		ParentPopup->Button_Close->OnClicked.AddDynamic(this, &UUC_HeroItemInfo::OnButtonCloseClicked);
	}
	Button_Close->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonCloseClicked);
	Button_Close->OnClicked.AddDynamic(this, &UUC_HeroItemInfo::OnButtonCloseClicked);
	Button_Equip->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonEquipClicked);
	Button_Equip->OnClicked.AddDynamic(this, &UUC_HeroItemInfo::OnButtonEquipClicked);
	Button_Unequip->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonUnequipClicked);
	Button_Unequip->OnClicked.AddDynamic(this, &UUC_HeroItemInfo::OnButtonUnequipClicked);
	Button_Lock->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonLockClicked);
	Button_Lock->OnClicked.AddDynamic(this, &UUC_HeroItemInfo::OnButtonLockClicked);
	Button_Unlock->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonUnlockClicked);
	Button_Unlock->OnClicked.AddDynamic(this, &UUC_HeroItemInfo::OnButtonUnlockClicked);
	Button_Exit->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonExitClicked);
	Button_Exit->OnClicked.AddDynamic(this, &UUC_HeroItemInfo::OnButtonExitClicked);
	Color_TowerOptionLabelTextWhenActive = Text_TowerOptionLabel->ColorAndOpacity;
	Color_TowerOptionDescriptionTextWhenActive = Text_TowerOptionDescription->ColorAndOpacity;
}

void UUC_HeroItemInfo::NativeDestruct()
{
	if (IsValid(Button_Lock))
		Button_Lock->OnClicked.RemoveAll(this);
	if (IsValid(Button_Unlock))
		Button_Unlock->OnClicked.RemoveAll(this);

	if (IsValid(Button_Unequip))
		Button_Unequip->OnClicked.RemoveAll(this);
	if (IsValid(Button_Equip))
		Button_Equip->OnClicked.RemoveDynamic(this, &UUC_HeroItemInfo::OnButtonEquipClicked);

	if (IsValid(Button_Close))
		Button_Close->OnClicked.RemoveAll(this);
	
	if (IsValid(ParentPopup))
	{
		if (IsValid(ParentPopup->Button_Close))
			ParentPopup->Button_Close->OnClicked.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UUC_HeroItemInfo::InvalidateData()
{
	Super::InvalidateData();

	if (ItemUD.IsEmpty())
		return;

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (ItemInfo == nullptr) {
		UE_LOG(LogUI, Log, TEXT("RInventory->GetItemDataWithUD returned ItemData with unknown idtemId"));
		return;
	}

	if (IsValid(Text_Level))
	{
		FString str = FString::FromInt(ItemData->level) + TEXT(" / ") + FString::FromInt(ItemInfo->MaxLevel);
		Text_Level->SetText(FText::FromString(str));
	}

	if (IsValid(Text_Enchant))
	{
		FString str = TEXT("+ ") + FString::FromInt(ItemData->enchant);
		Text_Enchant->SetText(FText::FromString(str));
	}

	// (design)
	
	bool IsUpgradeEnabled = (!ItemInfo->UpgradeIndex.IsNone() && ItemInfo->UpgradeIndex.IsValid());
	bool isEnchantEnabled = (ItemInfo->isEnchantable);
	bool isLevelUpEnabled = (ItemInfo->LevelUpPossible);

	SetButtonUpgradeIsEnabled(IsUpgradeEnabled);
	SetButtonEnchantIsEnabled(isEnchantEnabled);
	SetButtonLevelUpIsEnabled(isLevelUpEnabled);

	// Refresh icon - UC_HeroItemOptions_ScrollItem
	ItemIcon->RefreshEquipmentIcon(ItemInfo, ItemData);

	if (IsValid(EquipHeroIcon))
	{
		if(ItemData->equipedHeroUD.IsEmpty())
			EquipHeroIcon->SetVisibility(ESlateVisibility::Collapsed);
		else
			EquipHeroIcon->SetVisibility(ESlateVisibility::Visible);
		EquipHeroIcon->SetHeroIconInfoWithUD(ItemData->equipedHeroUD, EHeroOwnershipType::VE_Me);
	}

	
	FText ItemNameText;
	ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(EStringTableType::VE_Item, ItemInfo->ItemName, ItemNameText);
	Text_ItemName->SetText(ItemNameText);

	// Equip Type
	if(IsValid(Text_EquipType))
		Text_EquipType->SetText(UDescriptionFunctionLibrary::GetItemEquipPositionToText(ItemInfo->EquipPosition));
	//UUIFunctionLibrary::SetImageBrush(UResourceFunctionLibrary::GetHeroEquipTypeSprite(ItemInfo->EquipType), Image_EquipType);
	
	// Locked
	if (Switcher_Lock)
	{
		Switcher_Lock->SetActiveWidgetIndex(ItemData->locked ? 1 : 0);
	}
	// Item Power
	//int32 ItemPowerValue = RGAMEINSTANCE(this)->RInventory->GetItemValue(ItemData->itemId, ItemData->enchant, ItemData->level);
	if(IsValid(Text_PowerValue))
	{
		int32 CombatPower = FItemTableInfo::GetCombatPower(ItemData);
		Text_PowerValue->SetText(FText::AsNumber(CombatPower));
	}
	// Refresh Basic Options

	TArray<int32> BasicOptions;
	ItemInfo->GetBasicOptions(BasicOptions);

	FItemLevelUpTableInfo::ApplyItemLevel(ItemInfo, ItemData, BasicOptions);
	FItemEnchantTableInfo::ApplyEnchant(ItemInfo, ItemData, BasicOptions);

	InvalidateBasicStats(BasicOptions);
	//InvalidateSubstats(BasicOptions);

	// Refresh Additional Options (random options)

	InvalidateAdditionalOptions(ItemInfo, ItemData);

	// Refresh Special options
	// switch on grade
	Switcher_SpecialOptions->SetVisibility(ESlateVisibility::Collapsed);

	/*if (ItemInfo->ItemGrade == EItemGradeEnum::Tower)
	{

		Switcher_SpecialOptions->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Switcher_SpecialOptions->SetActiveWidgetIndex(1);

		if (ItemInfo->TowerOption.IsNone() == false)
		{
			FTowerOptionInfo* TowerOptionInfo = UTableManager::GetInstancePtr()->GetTowerOptionTableRow(ItemInfo->TowerOption);
// 			if (ensure(!TowerOptionInfo))
// 				return;
			if(TowerOptionInfo)
			{
				// Text_TowerOptionGuide
				FText TowerOptionGuideTemplate = TowerOptionGuideText.GetText();
				FFormatNamedArguments Args;
				Args.Add(TEXT("floor"), TowerOptionInfo->ActivateFloor);
				FText GuideText = FText::Format(TowerOptionGuideTemplate, Args);
				Text_TowerOptionGuide->SetText(GuideText);

				//Text_TowerOptionDescription->SetText();
				FText DescriptionText = FText();
				FText localText = FText();
				FTowerSkillTable* pTowerSkillInfo = nullptr;
				if (TowerOptionInfo->TowerSkillIndex01 != NAME_None)
				{
					pTowerSkillInfo = UTableManager::GetInstancePtr()->GetTowerSkillRow(TowerOptionInfo->TowerSkillIndex01);
					if (pTowerSkillInfo)
					{
						localText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, pTowerSkillInfo->Description);
						if (!localText.IsEmpty())
						{
							DescriptionText = FText::Format(FText::FromString(TEXT("{0}{1}")), DescriptionText, localText);
						}
					}
				}
				if (TowerOptionInfo->TowerSkillIndex02 != NAME_None)
				{
					pTowerSkillInfo = UTableManager::GetInstancePtr()->GetTowerSkillRow(TowerOptionInfo->TowerSkillIndex02);
					if (pTowerSkillInfo)
					{
						localText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, pTowerSkillInfo->Description);
						if (!localText.IsEmpty())
						{
							DescriptionText = FText::Format(FText::FromString(TEXT("\n{0}{1}")), DescriptionText, localText);
						}
					}
				}
				if (TowerOptionInfo->TowerSkillIndex03 != NAME_None)
				{
					pTowerSkillInfo = UTableManager::GetInstancePtr()->GetTowerSkillRow(TowerOptionInfo->TowerSkillIndex03);
					if (pTowerSkillInfo)
					{
						localText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, pTowerSkillInfo->Description);
						if (!localText.IsEmpty())
						{
							DescriptionText = FText::Format(FText::FromString(TEXT("\n{0}{1}")), DescriptionText, localText);
						}
					}
				}
				if (TowerOptionInfo->TowerSkillIndex04 != NAME_None)
				{
					pTowerSkillInfo = UTableManager::GetInstancePtr()->GetTowerSkillRow(TowerOptionInfo->TowerSkillIndex04);
					if (pTowerSkillInfo)
					{
						localText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, pTowerSkillInfo->Description);
						if (!localText.IsEmpty())
						{
							DescriptionText = FText::Format(FText::FromString(TEXT("\n{0}{1}")), DescriptionText, localText);
						}
					}
				}
				Text_TowerOptionDescription->SetText(DescriptionText);

				// TODO: NOT CERTAIN whether activatedFloor means "PC가 도달한 최고 층"
				//int32 FloorToCompare = RGAMEINSTANCE(this)->CampaignManager->TowerStatus.activatedFloor;
				//bool Activated = FloorToCompare >= TowerOptionInfo->ActivateFloor;

				//Text_TowerOptionLabel->SetColorAndOpacity(Activated ? Color_TowerOptionLabelTextWhenActive : Color_TowerOptionLabelTextWhenInactive);
				//Text_TowerOptionGuide->SetColorAndOpacity(Activated ? Color_TowerOptionDescriptionTextWhenActive : Color_TowerOptionDescriptionTextWhenInactive);
				//Text_TowerOptionDescription->SetColorAndOpacity(Activated ? Color_TowerOptionDescriptionTextWhenActive : Color_TowerOptionDescriptionTextWhenInactive);
			}
		}
		else
		{
			// dev contingency
			if (Text_TowerOptionLabel)
			{
				Text_TowerOptionLabel->SetColorAndOpacity(Color_TowerOptionLabelTextWhenInactive);
			}
			if (Text_TowerOptionGuide)
			{
				Text_TowerOptionGuide->SetColorAndOpacity(Color_TowerOptionDescriptionTextWhenInactive);
			}
			if (Text_TowerOptionDescription)
			{
				Text_TowerOptionDescription->SetColorAndOpacity(Color_TowerOptionDescriptionTextWhenInactive);
			}
		}
	}
	else if (ItemInfo->ItemGrade == EItemGradeEnum::Unique)
	{
		Switcher_SpecialOptions->SetVisibility(ESlateVisibility::Hidden);
		//Switcher_SpecialOptions->SetActiveWidgetIndex(0);
		//
		//// Rune Options
		//InvalidateRuneOptions(ItemData->runes, ItemInfo);

		//// Rune Word

		////FRuneWordTable* RuneWord = UTableManager::GetInstancePtr()->GetRuneWordFind(ItemData->runes);

		//FName RuneWordRowName = FRuneWordTable::CheckForRuneWord(ItemData->runes, FRuneTable::EquipPositionToNumSocketsByTable(ItemInfo)//FRuneTable::EquipPositionToNumSockets(ItemInfo->EquipPosition)//);

		//if (RuneWordRowName.IsNone())
		//{
		//	Panel_RuneWord->SetVisibility(ESlateVisibility::Collapsed);
		//}
		//else
		//{
		//	FRuneWordTable* RuneWordInfo = UTableManager::GetInstancePtr()->GetRuneWordRow(RuneWordRowName);
		//	check(RuneWordInfo != nullptr);

		//	FRuneWordSkillTable* RuneWordSkillInfo = UTableManager::GetInstancePtr()->GetRuneWordSkillRow(RuneWordInfo->RunewordSkillID);
		//	check(RuneWordSkillInfo != nullptr);

		//	Panel_RuneWord->SetVisibility(ESlateVisibility::Visible);

		//	Text_RuneWordName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, RuneWordInfo->Runeword_Name));

		//	FText RuneWordSkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, RuneWordSkillInfo->Summary);
		//	Text_RuneWordDescription->SetText(RuneWordSkillDesc);
		//}
	}
	else
	{
		Switcher_SpecialOptions->SetVisibility(ESlateVisibility::Collapsed);
	}
	*/

	// (bug list)
	//SetButtonItemResetIsEnabled(!ItemData->locked && ItemData->equipedHeroUD.IsEmpty());
	
	//// Refresh SubBtns panel

	//// Refresh bottom panel buttons
	//if (ItemData->equipedHeroUD.IsEmpty())
	if (!bLeft)
	{
		Button_Equip->SetVisibility(ESlateVisibility::Visible);
		Button_Unequip->SetVisibility(ESlateVisibility::Collapsed);
		Button_Equip->SetIsEnabled(UUtilFunctionLibrary::CanHeroEquipItem(CurrentHeroUD, ItemUD));
	}
	else
	{
		bool bisBagMode = FMath::IsWithin<EScrollviewType>(UUIFunctionLibrary::GetGenericScrollview()->Variables.State, EScrollviewType::VE_BAG_WEAPON, EScrollviewType::VE_BAG_MISC);
		if (bisBagMode)
		{
			Button_Equip->SetVisibility(ESlateVisibility::Collapsed);
			Button_Unequip->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (!ItemData->equipedHeroUD.IsEmpty())
		{
			Button_Equip->SetVisibility(ESlateVisibility::Collapsed);
			Button_Unequip->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UUC_HeroItemInfo::OnButtonCloseClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

//void UUC_HeroItemInfo::OnButtonGrindClicked()
//{
//	// Open BP UC_Popup_ItemGrind
//	auto Popup_ItemGrind = Cast<UUC_GenericPopup>(RGAMEINSTANCE(this)->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemGrind));
//	RGAMEINSTANCE(this)->RWidgetManager->AddToViewport(EWidgetBluePrintEnum::WBP_Popup_ItemGrind, Popup_ItemGrind, GACHAPOPUP_ZORDER);
//
//	auto PopupContent_ItemGrind = Cast<UUC_PopupContent_ItemGrind>(Popup_ItemGrind->GetContentWidget());
//
//	PopupContent_ItemGrind->SetItemUD(ItemUD);
//	PopupContent_ItemGrind->OnItemGrindComplete.AddUObject(this, &UUC_HeroItemInfo::OnItemInventoryUpdate);
//
//	Popup_ItemGrind->InvalidateData();
//}

void UUC_HeroItemInfo::OnButtonEnchantClicked()
{
	// Open BP UP_Page_ItemEnchant
	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (ItemData == nullptr)
		return;

	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	if (ItemData->enchant >= ItemInfo->MaxEnchantLevel)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_ItemEnchant_Impossible_SplashPopUp"));
		return;
	}

	if (ItemData->enchantLocked)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_ItemEnchant_Impossible_SplashPopUp"));
		return;
	}

	// pass ItemUD as UIEvent thing
	UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemEnchant);

	UUC_ItemEnchant* Enchantpage = Cast<UUC_ItemEnchant>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemEnchant));
	if (Enchantpage)
	{
		Enchantpage->SetItemUD(ItemUD);
	}
	
}

void UUC_HeroItemInfo::OnButtonUpgradeClicked()
{
	UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
	UUIFunctionLibrary::GetGenericScrollview()->Variables.ScrollView->ClearAllSelected();
	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemUpgrade);

	UUP_ItemUpgrade* itemupgrade = Cast<UUP_ItemUpgrade>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Page_ItemUpgrade));
	if (itemupgrade)
	{
		itemupgrade->SetItemUD(ItemUD);
	}
}


void UUC_HeroItemInfo::OnButtonLevelUpClicked()
{
	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (ItemData == nullptr)
		return;
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (ItemInfo == nullptr) 
		return;

	if (ItemData->level >= ItemInfo->MaxLevel)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_ItemLevleUp_Impossible_SplashPopUp"));
		return;
	}

	RGAMEINSTANCE(this)->RWidgetManager->RemoveUIEvent(EUIEventEnum::VE_ItemInventoryPage);
	RGAMEINSTANCE(this)->RWidgetManager->AddUIEventString(EUIEventEnum::VE_ItemInventoryPage, ItemUD);

	UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
	RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ItemLevelUp);
}

void UUC_HeroItemInfo::InvalidateBasicStats(const TArray<int32>& BasicOptions)
{
	for (UUC_ItemInfo_ItemStat* ItemStat : BasicStats)
	{
		ItemStat->SetVisibility(ESlateVisibility::Hidden);
	}

	int32 WidgetIndex = 0;
	for (int32 i = 0; i <= (int32)EItemStatOption::MagicResistance; i++)
	{
		if (BasicOptions[i] == 0)
			continue; // No option

		if (!ensure(WidgetIndex < BasicStats.Num()))
			break;
		
		BasicStats[WidgetIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BasicStats[WidgetIndex]->Refresh((EItemStatOption)i, BasicOptions[i], false);

		++WidgetIndex;
	}
}

void UUC_HeroItemInfo::InvalidateSubstats(const TArray<int32>& BasicOptions)
{
	for (UUC_ItemInfo_ItemStat* ItemStat : Substats)
	{
		ItemStat->SetVisibility(ESlateVisibility::Hidden);
	}


	bool bSubstatsExist = false;
	int32 WidgetIndex = 0;
	for (int32 i = (int32)EItemStatOption::AttackRange; i < (int32)EItemStatOption::Max; i++)
	{
		if (BasicOptions[i] == 0)
			continue; // No option

		if (!ensure(WidgetIndex <= Substats.Num()))
			break;

		Substats[WidgetIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Substats[WidgetIndex]->Refresh((EItemStatOption)i, BasicOptions[i], false);

		++WidgetIndex;
		bSubstatsExist = true;
	}


	if (!bSubstatsExist)
		Panel_AdditionalOptions->SetVisibility(ESlateVisibility::Hidden);
	else
		Panel_AdditionalOptions->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUC_HeroItemInfo::InvalidateAdditionalOptions(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	bool IsEmpty = true;

	for (int32 i = 0; i < Substats.Num(); i++)
	{
		EItemStatOption ItemStatOption = EItemStatOption::PhysicalAttackPower;
		int32 Value = 0;

		if (ItemInfo->GetOneAdditionalOption(ItemData, i, ItemStatOption, Value))
		{
			IsEmpty = false;
			Substats[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Substats[i]->Refresh(ItemStatOption, Value, true);
		}
		else
		{
			Substats[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	Panel_AdditionalOptions->SetVisibility(IsEmpty ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UUC_HeroItemInfo::InvalidateRuneOptions(const TArray<FName>& Runes, const FItemTableInfo* ItemInfo)
{
	if (!ensure(RuneStats.Num() >= FRuneTable::MaxNumSockets))
		return;

	if (!ItemInfo)
		return;

	EItemEquipPositionEnum EquipPosition = ItemInfo->EquipPosition;
	int32 NumRuneSockets = FRuneTable::EquipPositionToNumSocketsByTable(ItemInfo);//FRuneTable::EquipPositionToNumSockets(EquipPosition);

	for (int32 i = 0; i < FRuneTable::MaxNumSockets; i++) // WidgetIndex
	{
		if (i >= NumRuneSockets)
		{
			RuneStats[i]->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}
		else
		{
			RuneStats[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (!Runes.IsValidIndex(i) || Runes[i].IsNone())
		{
			RuneStats[i]->RefreshEmpty();
			continue;
		}

		int32 idx = RGAMEINSTANCE(this)->RInventory->RuneWallet.Find(Runes[i]);
		if (!ensure(idx != INDEX_NONE))
			return;
		FRuneTable* RuneTable = UTableManager::GetInstancePtr()->GetRuneOptionRowInt(idx + 1);
		if (!ensure(RuneTable != nullptr))
			return;

		EItemStatOption ItemStat = RuneTable->GetAdditionalOptionStat(EquipPosition);
		int32 StatValue = RuneTable->GetAdditionalOptionValue(EquipPosition);

		RuneStats[i]->RefreshSocketed(RuneTable->RuneMountingImage, ItemStat, StatValue);
	}
}

FName UUC_HeroItemInfo::ItemStatOptionEnumToHeroPropertyUIStringKey(EItemStatOption ItemStatOption)
{
	switch (ItemStatOption)
	{
	case EItemStatOption::PhysicalAttackPower:
		return FName(TEXT("USK_uc_heroProperty_Phyattack"));
		break;
	case EItemStatOption::MagicAttackPower:
		return FName(TEXT("USK_uc_heroProperty_Magicattack"));
		break;
	case EItemStatOption::Armor:
		return FName(TEXT("USK_uc_heroProperty_PhyDefence"));
		break;
	case EItemStatOption::MagicResistance:
		return FName(TEXT("USK_uc_heroProperty_MagicDefence"));
		break;
	case EItemStatOption::MaxHP:
		return FName(TEXT("USK_uc_heroProperty_Health"));
		break;
	case EItemStatOption::AttackRange:
		return FName(TEXT("USK_uc_heroProperty_AttackRange"));
		break;
	case EItemStatOption::MoveSpeed:
		return FName(TEXT("USK_uc_heroProperty_MoveSpeed"));
		break;
	case EItemStatOption::AttackSpeed:
		return FName(TEXT("USK_uc_heroProperty_AttackSpeed"));
		break;
	case EItemStatOption::DodgeValue:
		return FName(TEXT("USK_uc_heroProperty_Dodge"));
		break;
	case EItemStatOption::CriticalValue:
		return FName(TEXT("USK_uc_heroProperty_CriticalRate"));
		break;
	case EItemStatOption::CriticalDamageRate_Scale:
		return FName(TEXT("USK_uc_heroProperty_CriticalDamage"));
		break;
	case EItemStatOption::PierceValue:
		return FName(TEXT("USK_uc_heroProperty_Pierce"));
		break;
	case EItemStatOption::AccuracyValue:
		return FName(TEXT("USK_uc_heroProperty_Hit"));
		break;
	default:
		return NAME_None; // fix
	}
}

void UUC_HeroItemInfo::OnButtonEquipClicked()
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryHeroEquip.AddUObject(this, &UUC_HeroItemInfo::OnInventoryHeroEquipRp);

	UPacketFunctionLibrary::INVENTORY_HERO_EQUIP_RQ(CurrentHeroUD, ItemUD);
}

void UUC_HeroItemInfo::OnButtonUnequipClicked()
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryHeroUnequip.AddUObject(this, &UUC_HeroItemInfo::OnInventoryHeroUnequipRp);

	UPacketFunctionLibrary::INVENTORY_HERO_UNEQUIP_RQ(CurrentHeroUD, ItemUD);
}

void UUC_HeroItemInfo::OnButtonLockClicked()
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryUpdate.AddUObject(this, &UUC_HeroItemInfo::OnInventoryItemHeroLock);

	UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ({ ItemUD }, {}, {}, {});
}

void UUC_HeroItemInfo::OnButtonUnlockClicked()
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryUpdate.AddUObject(this, &UUC_HeroItemInfo::OnInventoryItemHeroLock);

	UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ({}, { ItemUD }, {}, {});
}

void UUC_HeroItemInfo::OnButtonRuneSetting()
{
	RGAMEINSTANCE(this)->RWidgetManager->RemoveUIEvent(EUIEventEnum::VE_ItemInventoryPage);
	RGAMEINSTANCE(this)->RWidgetManager->AddUIEventString(EUIEventEnum::VE_ItemInventoryPage, ItemUD);

	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_RuneManagement);
}


void UUC_HeroItemInfo::OnButtonOptionResetClicked()
{
	RGAMEINSTANCE(this)->RWidgetManager->RemoveUIEvent(EUIEventEnum::VE_ItemInventoryPage);
	RGAMEINSTANCE(this)->RWidgetManager->AddUIEventString(EUIEventEnum::VE_ItemInventoryPage, ItemUD);

	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ItemOptionReset);
}

void UUC_HeroItemInfo::OnButtonExitClicked()
{
	ItemUD.Empty();
	
	SetVisibility(ESlateVisibility::Collapsed);

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview && bLeft)
	{
		switch (GenericScrollview->Variables.State)
		{
		case EScrollviewType::VE_ITEM_WEAPON:
		case EScrollviewType::VE_ITEM_ARMOR_TOP:
		case EScrollviewType::VE_ITEM_ARMOR_BOT:
		case EScrollviewType::VE_ITEM_RING:
		case EScrollviewType::VE_ITEM_AMULET:
			GenericScrollview->Appear(false);
			break;

		default:
			break;
		}
	}
}

void UUC_HeroItemInfo::OnInventoryHeroEquipRp(bool bSuccess)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryHeroEquip.RemoveAll(this);

	if (bSuccess)
	{
		OnItemInventoryUpdatedDelegate.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
	}
	else
	{
		// TODO. Maybe I should close
	}
}

void UUC_HeroItemInfo::OnInventoryHeroUnequipRp(bool bSuccess)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryHeroUnequip.RemoveAll(this);

	if (bSuccess)
	{
		OnItemInventoryUpdatedDelegate.ExecuteIfBound(EItemInventoryUpdate::Updated, true);
	}
	else
	{
		// TODO
	}
}

void UUC_HeroItemInfo::OnInventoryItemHeroLock(bool bSuccess)
{
	//UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UIpage_HeroInventory_LockandReleaseResult"))), FText());
	// what do about failure?

	InvalidateData();

	// ?
	//OnItemInventoryUpdatedDelegate.ExecuteIfBound(EItemInventoryUpdate::OneItemUpdated, false);
}

// temp temp temp
FText UUC_HeroItemInfo::EquipTypeToString(EItemEquipTypeEnum EquipType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemEquipTypeEnum"), true);
	if (!EnumPtr)
		return FText::GetEmpty();

	auto index = EnumPtr->GetIndexByValue((uint8)EquipType);
	return EnumPtr->GetDisplayNameTextByIndex(index);
}

void UUC_ItemInfo_RuneOption::RefreshEmpty()
{
	Switcher_SocketState->SetActiveWidgetIndex(1);

	Image_RuneIcon->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_ItemInfo_RuneOption::RefreshSocketed(FName ImageKey, EItemStatOption ItemStatOption, int32 Value)
{
	Switcher_SocketState->SetActiveWidgetIndex(0);

	Image_RuneIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

	UTexture2D* IconImage = UUtilFunctionLibrary::GetTexture2DFromTable(ImageKey);
	Image_RuneIcon->SetBrushFromTexture(IconImage);

	// name
	Text_Name->LocalizingKey = UUIFunctionLibrary::ItemStatOptionToStringKey(ItemStatOption);
	Text_Name->ApplyLocalizedText();

	// value
	Text_Value->SetText(UUtilFunctionLibrary::ItemAdditionalOptionStatValueToText(ItemStatOption, Value));
}

void UUC_ItemInfo_Button::NativePreConstruct()
{
	
	// labels
	if (ButtonLabel.UIStringKey.IsNone())
	{
		if(Text_LabelEnabled)
			Text_LabelEnabled->SetText(ButtonLabel.FallbackText);
		if(Text_LabelDisabled)
			Text_LabelDisabled->SetText(ButtonLabel.FallbackText);
	}
	else
	{
		if (Text_LabelEnabled && Text_LabelDisabled)
		{
			Text_LabelEnabled->LocalizingKey = ButtonLabel.UIStringKey;
			Text_LabelEnabled->ApplyLocalizedText();
			Text_LabelDisabled->LocalizingKey = ButtonLabel.UIStringKey;
			Text_LabelDisabled->ApplyLocalizedText();
		}
	}

}

void UUC_ItemInfo_Button::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Enabled->OnClicked.AddDynamic(this, &UUC_ItemInfo_Button::OnButtonEnabledClicked);
}

void UUC_ItemInfo_Button::NativeDestruct()
{
	if (IsValid(Button_Enabled))
		Button_Enabled->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_ItemInfo_Button::SetDisabled(bool bDisabled)
{
	Switcher_Enabled->SetActiveWidgetIndex(bDisabled ? 1 : 0);
}

void UUC_ItemInfo_Button::OnButtonEnabledClicked()
{
	OnClicked.Broadcast();
}
