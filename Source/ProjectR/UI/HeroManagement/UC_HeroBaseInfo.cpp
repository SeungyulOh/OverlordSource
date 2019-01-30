// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroBaseInfo.h"

#include "SharedConstants/GlobalConstants.h"

#include "GlobalIntegrated.h"



#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "Table/LevelGrowth.h"
#include "Table/CharacterTableInfo.h"

#include "Network/PacketDataEnum.h"
#include "Network/PacketFunctionLibrary.h"

#include "UI/RWidgetManager.h"
#include "UI/HeroManagement/UC_HeroDetailStat.h"
#include "UI/HeroManagement/UC_Hero_Skill_Icon.h"
#include "UC_HeroInfoStat.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/HorizontalBox_RadioGroup.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "UI/ItemManagement/UP_Bag.h"
#include "UI/ItemManagement/UC_HeroItemInfo.h"
#include "UI/ItemManagement/UC_GenericPopup.h"
#include "UI/Common/UC_ItemIcon.h"

#include "UI/Component/CanvasPanel_RadioGroup.h"


void UUC_HeroBaseInfo::NativeConstruct()
{
#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif

	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);
	FindChildWidgetsWithCertainName<UImage>(this, HeroStarArray, TEXT("Hero_Star_"));
//	FindChildWidgetsWithCertainName<UImage>(this, HeroStarArray, TEXT("Star_"));
	FindChildWidgetsWithCertainName<UUC_Hero_Skill_Icon>(this, HeroSkillIcons, TEXT("UC_Hero_SKill_"));
	Button_FilterOn->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_HeroBaseInfo_FilterOn);
	Button_FilterOn->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_HeroBaseInfo_FilterOn);

// 	if (IsValid(HeroLevelUpPopup))
// 		HeroLevelUpPopup->SetVisibility(ESlateVisibility::Collapsed);
// 	if (IsValid(HeroUpgradePopup))
// 		HeroUpgradePopup->SetVisibility(ESlateVisibility::Collapsed);
// 	if (IsValid(HeroEnchantPopup))
// 		HeroEnchantPopup->SetVisibility(ESlateVisibility::Collapsed);
// 	if (IsValid(Button_LevelUp))
	{
		Button_LevelUp->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_LevelUp);
		Button_LevelUp->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_LevelUp);
	}
	if (IsValid(Button_DetailStat_2))
	{
		Button_DetailStat_2->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_OpenHeroDetailStat);
		Button_DetailStat_2->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_OpenHeroDetailStat);
	}
	if (IsValid(Button_Upgrade))
	{
		Button_Upgrade->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_GradeUp);
		Button_Upgrade->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_GradeUp);
	}
	if (IsValid(Button_Enchant))
	{
		Button_Enchant->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_Enchant);
		Button_Enchant->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_Enchant);
	}
	if (IsValid(Button_HeroShop))
	{
		Button_HeroShop->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_GoHeroShop);
		Button_HeroShop->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_GoHeroShop);
	}
	if (IsValid(Button_FilterOn))
	{
		Button_FilterOn->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_HeroBaseInfo_FilterOn);
		Button_FilterOn->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_HeroBaseInfo_FilterOn);
	}
	if (IsValid(Button_Skills))
	{
		Button_Skills->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_HeroBaseInfo_Skills);
		Button_Skills->OnClicked.AddDynamic(this, &UUC_HeroBaseInfo::OnClick_HeroBaseInfo_Skills);
	}

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;

	UE_LOG(LogGamePlay, Log, TEXT("%f ____NativeConstruct(%s)"),
		PrintTime, *(this->GetName()));
#endif
}

void UUC_HeroBaseInfo::NativeDestruct()
{
	if (IsValid(Button_FilterOn))
		Button_FilterOn->OnClicked.RemoveDynamic(this, &UUC_HeroBaseInfo::OnClick_HeroBaseInfo_FilterOn);
	ClearData();
	Super::NativeDestruct();
}

void UUC_HeroBaseInfo::InvalidateData()
{
	Super::InvalidateData();

	if (Switcher_ItemInventory->GetActiveWidgetIndex() == 0)
	{
		InvalidateItemInventoryScrollView();
	}
}

void UUC_HeroBaseInfo::InvalidateByHeroUD(const FString& InHeroUD)
{
	CurrentHeroUD = InHeroUD;
	if (CurrentHeroUD.IsEmpty())
		return;
	RGameInstance = RGAMEINSTANCE(this);
// 	if (UICharacterBattleInfo)
// 		UICharacterBattleInfo = nullptr;
	auto UICharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(CurrentHeroUD);
	auto HERO = RGameInstance->RInventory->GetHeroWithUD(CurrentHeroUD);
	if (HERO != nullptr)
	{
		UICharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, HERO->heroId);
	}
	if (RadioGroup_Tabs)
		RadioGroup_Tabs->SetActiveRadioIndex(0, true);
	Refresh();
}

void UUC_HeroBaseInfo::UpdateHeroInfo(void)
{
	RGameInstance				=	RGAMEINSTANCE(this);
// 	auto UICharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(CurrentHeroUD);
// 	if (UICharacterBattleInfo)
// 		UICharacterBattleInfo = nullptr;
	auto UICharacterBattleInfo		=	URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(CurrentHeroUD);
	auto HERO					=	RGameInstance->RInventory->GetHeroWithUD(CurrentHeroUD);
	if (HERO != nullptr)
		UICharacterTableInfo	=	URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, HERO->heroId);
	Refresh();
}

void UUC_HeroBaseInfo::OnClick_OpenHeroDetailStat()
{
// 	UUP_Mediator_HeroManagement* HeroManagement = Cast<UUP_Mediator_HeroManagement>(ParentWidget);
// 	if (IsValid(HeroManagement))
// 		HeroManagement->OnClick_OpenHeroDetailStat();
}

void UUC_HeroBaseInfo::OnClick_GoHeroShop()
{
	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_ShopHero);
}

void UUC_HeroBaseInfo::OnClick_LevelUp()
{
// 	if (IsValid(HeroLevelUpPopup))
// 	{
// 		HeroLevelUpPopup->SetVisibility(ESlateVisibility::Visible);
// 		HeroLevelUpPopup->SetupHeroUD(CurrentHeroUD);
// 		HeroLevelUpPopup->RefreshLevelUpUI();
// 	}
}

void UUC_HeroBaseInfo::OnClick_GradeUp()
{
// 	if (IsValid(HeroUpgradePopup))
// 	{
// 		HeroUpgradePopup->SetVisibility(ESlateVisibility::Visible);
// 		HeroUpgradePopup->SetupHeroUD(CurrentHeroUD);
// 	}
}

void UUC_HeroBaseInfo::OnClick_Enchant()
{
// 	if (IsValid(HeroEnchantPopup))
// 	{
// 		HeroEnchantPopup->SetVisibility(ESlateVisibility::Visible);
// 		HeroEnchantPopup->Init(CurrentHeroUD);
// 	}
}

void UUC_HeroBaseInfo::OnClick_HeroBaseInfo_FilterOn()
{
	FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(CurrentHeroUD);
	if (outHero == nullptr)
		return;
	if (outHero->locked)
	{
		UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ({}, {}, {}, { CurrentHeroUD });
	}
	else
	{
		UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ({}, {}, { CurrentHeroUD }, {});
	}
}

void UUC_HeroBaseInfo::OnClick_HeroBaseInfo_Skills()
{
	RGameInstance->EventManager->OnClick_SkillManagement.Broadcast();
}

void UUC_HeroBaseInfo::OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom)
{
	Super::OnItemInventoryUpdated(Result, bWidgetFinished, UpdatedScrollItem, WidgetReturnedFrom);

	OnItemInventoryUpdatedDelegate.Broadcast();
}

void UUC_HeroBaseInfo::OnTabCheckStateChanged(bool bIsChecked, int32 Index)
{
	// override to handle hero info tab

	if (bIsChecked)
	{
		if (Index == 0)
		{
			// Show Hero Info
			Switcher_ItemInventory->SetActiveWidgetIndex(1);
			Refresh();
		}
		else
		{
			Switcher_ItemInventory->SetActiveWidgetIndex(0);
			Super::OnTabCheckStateChanged(bIsChecked, Index);
		}
	}
}

void UUC_HeroBaseInfo::OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	// request UP_Mediator_HeroManagement to display right-side ItemInfo
	auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
	check(ScrollData != nullptr);

	OnEquipmentItemSelected.ExecuteIfBound(ScrollData->ItemUD);

	// Show UC_Popup_ItemInfo
	// TODO: (design) not a popup but a panel
	//UUC_GenericPopup* Popup = RGAMEINSTANCE(this)->RWidgetManager->ShowGenericPopupContent(EWidgetBluePrintEnum::WBP_Popup_ItemInfo);
	//auto PopupContent = Cast<UUC_HeroItemInfo>(Popup->GetContentWidget());

	//PopupContent->ItemUD = ScrollData->ItemUD;
	//PopupContent->IsInHeroManager = true;
	//PopupContent->CurrentHeroUD = CurrentHeroUD;

	//PopupContent->OnItemInventoryUpdatedDelegate.BindUObject(this, &UUC_HeroBaseInfo::OnItemInventoryUpdated, (URScrollItem*)SelectedItem, EWidgetBluePrintEnum::WBP_Popup_ItemInfo);
	//
	//FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ScrollData->ItemUD);
	//check(ItemData != nullptr);
	//PopupContent->EquippedByCurrentHero = !ItemData->equipedHeroUD.IsEmpty() && ItemData->equipedHeroUD == CurrentHeroUD;

	//Popup->InvalidateData();
}

EItemInventoryEnum UUC_HeroBaseInfo::TabIndexToInventoryEnum(int32 Index)
{
	EItemInventoryEnum ItemInventoryEnum = EItemInventoryEnum::Weapons;
	switch (Index)
	{
	case 1:
		ItemInventoryEnum = EItemInventoryEnum::Weapons;
		break;
	case 2:
		ItemInventoryEnum = EItemInventoryEnum::Defense;
		break;
	case 3:
		ItemInventoryEnum = EItemInventoryEnum::Accessories;
		break;
	case 4:
		ItemInventoryEnum = EItemInventoryEnum::Runes;
		break;
	case 5:
		UE_LOG(LogUI, Log, TEXT("UUC_HeroBaseInfo::TabIndexToInventoryEnum: Not sure what Apparels is. Sorry."));
		break;
	default:
		UE_LOG(LogUI, Log, TEXT("UUC_HeroBaseInfo::TabIndexToInventoryEnum: Unexpected index"));
		break;
	}
	return ItemInventoryEnum;
}

bool UUC_HeroBaseInfo::IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	// (design)
	// * do not show items unequippable by the current hero
	// * do not show items equipped by the current hero

	if (ItemData->equipedHeroUD == CurrentHeroUD)
		return false;

	if (!ensure(UICharacterTableInfo != nullptr))
		return true;

	if (!FItemTableInfo::CanEquip(UICharacterTableInfo->Class, ItemInfo->EquipType))
		return false;

	return true;
}

void UUC_HeroBaseInfo::Refresh()
{
	if (UICharacterTableInfo == nullptr)
		return;
	Refresh_HeroName();
	Refresh_CombatStat();
	Refresh_HeroLevel();	
	//Refresh_HeroExp();
	//Refresh_HeroExpSlide();
	//Refresh_HeroExpProgressBar();
	Refresh_HeroClassTypeImage();
	Refresh_HeroWeaponTypeImage();
	Refresh_HeroWeaponType();
	Refresh_HeroRuneCount();
	Refresh_HeroClassType();
	Refresh_HeroDetail();
	Refresh_HeroSkill();
}

void UUC_HeroBaseInfo::Refresh_HeroName()
{
	if (UICharacterTableInfo == nullptr)
		return;

	if (Text_HeroName)
	{
		auto HeroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, UICharacterTableInfo->DisplayName);
		Text_HeroName->SetText(HeroName);
	}

	if (Text_HeroNickName)
	{
		auto HeroNickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, UICharacterTableInfo->Nick);
		Text_HeroNickName->SetText(HeroNickName);
	}

	if (Text_HeroSummary1)
	{
		auto Summary = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, UICharacterTableInfo->Summary);
		Text_HeroSummary1->SetText(Summary);
	}

	if (Text_Desc1)
	{
		auto Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, UICharacterTableInfo->Desc);
		Text_Desc1->SetText(Desc);
	}
}

void UUC_HeroBaseInfo::Refresh_CombatStat()
{
	if (IsValid(ULocalizingTextCombatStat))
	{
		auto HeroCombatStat = UClient_BattleFunctionLibrary::GetHeroCombatStat(CurrentHeroUD);
		ULocalizingTextCombatStat->SetText(FText::AsNumber(HeroCombatStat));
		return;
	}
}

void UUC_HeroBaseInfo::Refresh_HeroLevel()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	if (RGAMEINSTANCE(this)->RInventory == nullptr)
		return;

	FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(CurrentHeroUD);
	checkf(outHero != nullptr, TEXT("UUC_HeroBaseInfo outHero is Empty"));
	FCharacterTableInfo*	CharacterTemp	=	UTableManager::GetInstancePtr()->GetCharacterRow(outHero->heroId);
	checkf(CharacterTemp != nullptr, TEXT("UUC_HeroBaseInfo CharacterTemp is Empty"));
	auto UICharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(CurrentHeroUD);
	if (IsValid(Text_HeroLevel))
		if (IsValid(UICharacterBattleInfo))
			Text_HeroLevel->SetText(FText::AsNumber(UICharacterBattleInfo->GetLevel()));

	if (IsValid(Text_HeroMaxLevel))
			Text_HeroMaxLevel->SetText(FText::AsNumber(CharacterTemp->MaxLevel));

	FCharacterCostGrowth* NeedMaterial	=	 UTableManager::GetInstancePtr()->GetCharacterCostGrowthRow(FName(*FString::FromInt(UICharacterBattleInfo->GetLevel())));
	
	if (UICharacterBattleInfo->GetLevel() < CharacterTemp->MaxLevel && NeedMaterial != nullptr)
	{
		if (Text_NeedHeroRune1)
		{
			Text_NeedHeroRune1->SetVisibility(ESlateVisibility::Visible);
			Text_NeedHeroRune1->SetText(FText::AsNumber(NeedMaterial->Cost_HeroRune));
		}
		if (Text_NeedGold1)
		{
			Text_NeedGold1->SetVisibility(ESlateVisibility::Visible);
			Text_NeedGold1->SetText(FText::AsNumber(NeedMaterial->Cost_Gold));
		}
	}
	else
	{
		if (Text_NeedHeroRune1)
			Text_NeedHeroRune1->SetVisibility(ESlateVisibility::Collapsed);
		if (Text_NeedGold1)
			Text_NeedGold1->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (WidgetSwitcher_Growth)
	{
		if (UICharacterBattleInfo->GetLevel() >= CharacterTemp->MaxLevel)
			WidgetSwitcher_Growth->SetActiveWidgetIndex(1);
		else
			WidgetSwitcher_Growth->SetActiveWidgetIndex(0);
	}

	if (IsValid(Button_LevelUp))
	{
		//NeedMaterial != nullptr && NeedMaterial->Cost_Gold <= RGameInstance->RInventory->GetGold() && NeedMaterial->Cost_HeroRune <= RGameInstance->RInventory->GetHeroRune() && 
// 		if (UICharacterBattleInfo->GetLevel() < CharacterTemp->MaxLevel)
// 		{
// 			Button_LevelUp->SetIsEnabled(true);
// 			if (IsValid(HeroLevelUpPopup) && HeroLevelUpPopup->GetVisibility() == ESlateVisibility::Visible) {
// 				HeroLevelUpPopup->SetVisibility(ESlateVisibility::Visible);
// 				HeroLevelUpPopup->SetupHeroUD(CurrentHeroUD);
// 			}
// 		}
// 		else
// 		{
// 			Button_LevelUp->SetIsEnabled(false);
// 			if (IsValid(HeroLevelUpPopup) && HeroLevelUpPopup->GetVisibility() == ESlateVisibility::Visible) {
// 				HeroLevelUpPopup->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 		}
			
	}

	if (IsValid(Button_Upgrade))
	{
// 		if (UICharacterBattleInfo->GetLevel() == CharacterTemp->MaxLevel && UICharacterBattleInfo->GetLevel()  != DEF_MAX_HERO_LEVEL)
// 		{
// 			Button_Upgrade->SetIsEnabled(true);
// 			if (IsValid(HeroUpgradePopup) && HeroUpgradePopup->GetVisibility() == ESlateVisibility::Visible) {
// 				HeroUpgradePopup->SetVisibility(ESlateVisibility::Visible);
// 				HeroUpgradePopup->SetupHeroUD(CurrentHeroUD);
// 			}
// 
// 			if (IsValid(HeroEnchantPopup) && HeroEnchantPopup->GetVisibility() == ESlateVisibility::Visible) {
// 				HeroEnchantPopup->SetVisibility(ESlateVisibility::Visible);
// 			}
// 		}
// 		else
// 		{
// 			Button_Upgrade->SetIsEnabled(false);
// 			if (IsValid(HeroUpgradePopup) && HeroUpgradePopup->GetVisibility() == ESlateVisibility::Visible) {
// 				HeroUpgradePopup->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 			if (IsValid(HeroEnchantPopup) && HeroEnchantPopup->GetVisibility() == ESlateVisibility::Visible) {
// 				HeroEnchantPopup->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 		}
			
	}
	int32 grade = CharacterTemp->Grade;
	for (UImage* ImgStar : HeroStarArray)
		ImgStar->SetVisibility(ESlateVisibility::Collapsed);

	for (int32 i = 0; i < grade; i++)
		if (HeroStarArray.IsValidIndex(i))
			HeroStarArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (grade <= 5)
		WidgetSwitcher_Star->SetActiveWidgetIndex(0);
	else if (grade < 10)
		WidgetSwitcher_Star->SetActiveWidgetIndex(1);
	else
		WidgetSwitcher_Star->SetActiveWidgetIndex(2);

	if (outHero->locked)
		Switcher_FilterOn->SetActiveWidgetIndex(1);
	else
		Switcher_FilterOn->SetActiveWidgetIndex(0);
}

void UUC_HeroBaseInfo::Refresh_HeroClassTypeImage()
{
	if (Image_HeroJob == nullptr)
		return;
	if (UICharacterTableInfo == nullptr)
		return;

	auto HeroClassType = UICharacterTableInfo->Class;

	auto HeroClassSprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(HeroClassType);
	UUIFunctionLibrary::SetImageBrush(HeroClassSprite, Image_HeroJob);

	if (Text_HeroJob)
	{
		Text_HeroJob->SetText(UResourceFunctionLibrary::GetHeroClassTypeName(HeroClassType));
	}
}

void UUC_HeroBaseInfo::Refresh_HeroWeaponTypeImage()
{
	if (Image_WeaponType == nullptr)
		return;
	if (UICharacterTableInfo == nullptr)
		return;

	auto EquipType		= UICharacterTableInfo->EquipType;
	auto EquipTypeRes	= UResourceFunctionLibrary::GetHeroEquipTypeSprite(EquipType);

	UUIFunctionLibrary::SetImageBrush(EquipTypeRes, Image_WeaponType);

}

//void UUC_HeroBaseInfo::Refresh_HeroExpProgressBar()
//{
//	if (Progressbar_Exp == nullptr)
//	{
//		return;
//	}
//
//	auto CurrentHeroLevel = UICharacterBattleInfo->GetLevel();
//	if (MAX_HERO_LEVEL <= CurrentHeroLevel)
//	{
//		Progressbar_Exp->SetPercent(1.0f);
//
//		return;
//	}
//
//// 	if (UICharacterNextLevelGrowthTableInfo == nullptr)
//// 	{
//// 		return;
//// 	}
//// 
//// 	auto Exp = static_cast<float>(UICharacterBattleInfo->EXP) / static_cast<float>(UICharacterNextLevelGrowthTableInfo->HeroMaxExp);
//// 
//// 	Progressbar_Exp->SetPercent(Exp);
//}

void UUC_HeroBaseInfo::Refresh_HeroWeaponType()
{
	if (UICharacterTableInfo == nullptr)
		return;

	if (Text_WeaponType)
	{
		Text_WeaponType->SetText(UResourceFunctionLibrary::GetHeroEquipTypeName(UICharacterTableInfo->EquipType));
	}

}

void UUC_HeroBaseInfo::Refresh_HeroRuneCount()
{

}

void UUC_HeroBaseInfo::Refresh_HeroClassType()
{
	if (UICharacterTableInfo == nullptr)
		return;
	if (IsValid(Text_EquipArmorType))
	{
		Text_EquipArmorType->SetText(GetClassTypeText(UICharacterTableInfo->EquipArmorType));
	}
}

void UUC_HeroBaseInfo::Refresh_HeroDetail()
{
	SetDetail(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower, InfoStat_PhysicalAttackPower);
	SetDetail(ECharacterPropertyTypeEnum::VE_MagicAttackPower, InfoStat_MagicAttackPower);
	SetDetail(ECharacterPropertyTypeEnum::VE_Armor, InfoStat_Armor);
	SetDetail(ECharacterPropertyTypeEnum::VE_MagicResistance, InfoStat_MagicResistance);
	SetDetail(ECharacterPropertyTypeEnum::VE_MaxHP, InfoStat_HP);
}

void UUC_HeroBaseInfo::Refresh_HeroSkill()
{
	int32 index = 0;
	for(index = 0; index < HeroSkillIcons.Num(); index++)
		HeroSkillIcons[index]->SetVisibility(ESlateVisibility::Collapsed);
	TArray<FHERO_SKILL> Skills;

	for (int32 i = 0; i < 4; i++)
	{
		if (i < 3)
			HeroSkillIcons[i]->SetSkillName(CurrentHeroUD, FName(*(UICharacterTableInfo->DisplayName.ToString() + "Active" + FString::FromInt(i + 1) + "_1")), 1, (ESkillCategoryTypeEnum)i);
		else
			HeroSkillIcons[i]->SetSkillName(CurrentHeroUD, FName(*(UICharacterTableInfo->DisplayName.ToString() + "Passive" + FString::FromInt(i - 2) + "_1")), 1, (ESkillCategoryTypeEnum)i);

		HeroSkillIcons[i]->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (UICharacterTableInfo->ActiveSkill01.IsNone())
		HeroSkillIcons[SKILL_ICON_INDEX_1]->SetDisable(false);
	if (UICharacterTableInfo->ActiveSkill02.IsNone())
		HeroSkillIcons[SKILL_ICON_INDEX_2]->SetDisable(false);
	if (UICharacterTableInfo->ActiveSkill03.IsNone())
		HeroSkillIcons[SKILL_ICON_INDEX_3]->SetDisable(false);
	if (UICharacterTableInfo->PassiveSkill01.IsNone())
		HeroSkillIcons[SKILL_ICON_INDEX_4]->SetDisable(false);
}

FText UUC_HeroBaseInfo::GetClassTypeText(EItemEquipArmorTypeEnum InType)
{
	switch (InType)
	{
	case EItemEquipArmorTypeEnum::VE_Heavy:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Decksetting_Heavey");
	case EItemEquipArmorTypeEnum::VE_Light:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Decksetting_Light");
	case EItemEquipArmorTypeEnum::VE_Leather:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Decksetting_Leather");
	case EItemEquipArmorTypeEnum::VE_Fabric:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Decksetting_Fabric");
	default:
		return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Decksetting_Heavey");
	}
}

bool UUC_HeroBaseInfo::IsShowingItemInventory()
{
	return Switcher_ItemInventory->GetActiveWidgetIndex() == 1;
}

void UUC_HeroBaseInfo::ClearData()
{	
	RGameInstance = nullptr;
	CurrentHeroUD.Empty();	
//	UICharacterBattleInfo = nullptr;
	UICharacterTableInfo = nullptr;
}

void UUC_HeroBaseInfo::SetDetail(ECharacterPropertyTypeEnum InPropertyType, UUC_HeroInfoStat* HeroInfoStat)
{
// 	if (IsValid(UICharacterBattleInfo) == false)
// 		return;
	auto UICharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(CurrentHeroUD);
	HeroInfoStat->Refresh(UICharacterBattleInfo);
}