// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroManager_Info.h"

#include "Global/RGameInstance.h"

#include "GameInfo/RInventory.h"


#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Utils/Client_BattleFunctionLibrary.h"
#include "Utils/ResourceFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"

#include "UI/Component/HorizontalBox_RadioGroup.h"

#include "Network/PacketFunctionLibrary.h"
#include "Global/TableManager.h"

#include "UI/RWidgetManager.h"
#include "UI/HeroManagement/UC_HeroInfoStat.h"
#include "UI/HeroManagement/UC_Hero_Skill_Icon.h"
#include "UI/HeroInfo/HeroUpgrade/UC_HeroInfo_Upgrade.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"


#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "Table/CharacterTableInfo.h"
#include "../HeroUpgrade/UP_HeroInfo_Upgrade.h"


void UUC_HeroManager_Info::NativeConstruct()
{
	FindChildWidgetsWithCertainName<UCheckBox_Radio>(this, TabCheckBoxes, TEXT("CheckBox_"));
	FindChildWidgetsWithCertainName<UUC_Bag_Tab>(this, BagTabs, TEXT("TabBag_"));

	Init();

	Super::NativeConstruct();
}

void UUC_HeroManager_Info::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HeroManager_Info::InvalidateData()
{
	Super::InvalidateData();
	if (Switcher_ItemInventory->GetActiveWidgetIndex() != 0)
	{
		FilterVisibility(false);
		InvalidateItemInventoryScrollView();
	}
}

void UUC_HeroManager_Info::Init()
{
	FindChildWidgetsWithCertainName<UUC_HeroInfoStat>(this, HeroStat, TEXT("HeroStat"));
	FindChildWidgetsWithCertainName<UUC_Hero_Skill_Icon>(this, HeroSkillIcons, TEXT("HeroSkill"));

	if (IsValid(Button_Rock))
	{
		Button_Rock->OnClicked.RemoveDynamic(this, &UUC_HeroManager_Info::OnClick_Lock);
		Button_Rock->OnClicked.AddDynamic(this, &UUC_HeroManager_Info::OnClick_Lock);
	}
	if (UC_Popup_ItemSorting)
	{
		UC_Popup_ItemSorting->Button_AttackPower->SetVisibility(ESlateVisibility::Collapsed);
		UC_Popup_ItemSorting->Button_GettingOrder->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void UUC_HeroManager_Info::SetHero(FString InHeroUD)
{
	URGameInstance*		Game	=	RGAMEINSTANCE(this);
	URInventory*		Inven	=	Game->RInventory;

	checkf(Game != nullptr, TEXT("UUC_HeroManager_Info[RGameInstance Is Empty]"));
	checkf(Inven != nullptr, TEXT("UUC_HeroManager_Info[RInventory Is Empty]"));

	if (RadioGroup_Tabs)
		RadioGroup_Tabs->SetActiveRadioIndex(0, true);

	m_CurHeroUD = InHeroUD;

	if (m_CurHeroUD.IsEmpty())
		return;

	m_Hero = Inven->GetHeroWithUD(m_CurHeroUD);

	if (m_Hero == nullptr)
		return;

	m_UIBattleInfo		=	URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(m_CurHeroUD);
	m_UITableInfo		=	URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, m_Hero->heroId);
	m_CostGrowthInfo	=	UTableManager::GetInstancePtr()->GetCharacterCostGrowthRow(FName(*FText::AsNumber(m_Hero->level).ToString()));

	checkf(m_UIBattleInfo != nullptr, TEXT("UUC_HeroManager_Info[m_UIBattleInfo Is Empty]"));
	checkf(m_UITableInfo != nullptr, TEXT("UUC_HeroManager_Info[m_UITableInfo Is Empty]"));
	checkf(m_CostGrowthInfo != nullptr, TEXT("UUC_HeroManager_Info[m_CostGrowthInfo Is Empty]"));

	SetHeroInfo();
	SetHeroSkill();
	SetHeroStat();
}

void UUC_HeroManager_Info::SetHeroInfo()
{
	if (IsValid(Img_Favorite))
	{
		if (m_Hero->locked)
			Img_Favorite->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(255, 255, 255)));
		else
			Img_Favorite->SetColorAndOpacity(FLinearColor::FromSRGBColor(FColor(37, 48, 60)));
	}

	if (IsValid(Text_HeroName))
	{
		FText HeroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_UITableInfo->DisplayName);
		Text_HeroName->SetText(HeroName);
	}

	if (IsValid(Text_HeroNickName))
	{
		FText HeroNickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_UITableInfo->Nick);
		Text_HeroNickName->SetText(HeroNickName);
	}

	if (IsValid(ProgressBar_HeroExp))
	{
		if (0 < m_CostGrowthInfo->Cost_HeroRune)
		{
			float fPercent = (float)m_Hero->exp / (float)m_CostGrowthInfo->Cost_HeroRune;
			ProgressBar_HeroExp->SetPercent(fPercent);
		}
		else
			ProgressBar_HeroExp->SetPercent(1.0f);
	}

	if (IsValid(Text_HeroExp))
	{
		if (0 < m_CostGrowthInfo->Cost_HeroRune)
		{
			float fPercent = (float)m_Hero->exp / (float)m_CostGrowthInfo->Cost_HeroRune;
			Text_HeroExp->SetText(FText::AsPercent(fPercent));
		}
		else
		{
			Text_HeroExp->SetText(FText::FromString(TEXT("MAX")));
		}
	}


	if(IsValid(Text_AttackPower))
	{
		int32 HeroAttackPower = UClient_BattleFunctionLibrary::GetHeroCombatStat(m_CurHeroUD);
		Text_AttackPower->SetText(FText::AsNumber(HeroAttackPower));
	}

	if (IsValid(Text_Level))
		Text_Level->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), m_Hero->level, m_UITableInfo->MaxLevel));


	if (IsValid(Text_ReinForce))
	{
		int32		sum = 0;

		for (int32 p : m_Hero->enchantStat)
			sum += p;
		bool bEnchantMax = (sum == MAX_HERO_ENCHANT_COUNT);


		if (!bEnchantMax)
		{
			Text_ReinForce->SetText(FText::Format(FText::FromString(TEXT(" + {0}")), sum));
		}
		else
		{
			Text_ReinForce->SetText(FText::FromString(TEXT("MAX")));
		}

	}

	if (IsValid(Text_CombatStat))
	{
		int32 nCombatStat = UClient_BattleFunctionLibrary::GetHeroCombatStat(m_Hero->heroUD);
		Text_CombatStat->SetText(FText::AsNumber(nCombatStat));
	}

	if (IsValid(UC_herostar))
		UC_herostar->SetStar(m_CurHeroUD);
	

	if (IsValid(Image_Job))
	{
		UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(m_UITableInfo->Class);
		if (Sprite)
			UUIFunctionLibrary::SetImageBrush(Sprite, Image_Job);
	}
}

void UUC_HeroManager_Info::SetHeroSkill()
{
	int32 index = 0;
	for (index = 0; index < HeroSkillIcons.Num(); index++)
	{
		HeroSkillIcons[index]->SetVisibility(ESlateVisibility::Collapsed);
		HeroSkillIcons[index]->SetDisable(true);
		HeroSkillIcons[index]->PanelUnlock->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (HeroSkillIcons.Num() == 0)
		return;

	if(HeroSkillIcons.IsValidIndex(SKILL_ICON_INDEX_4))
	{
		HeroSkillIcons[SKILL_ICON_INDEX_4]->SetSkillName(m_CurHeroUD, FName(*(m_UITableInfo->DisplayName.ToString() + "Passive1_1")), 1, ESkillCategoryTypeEnum::VE_PassiveSkill01);
		HeroSkillIcons[SKILL_ICON_INDEX_4]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	for (int32 i = 0; i < 3; i++)
	{
		HeroSkillIcons[i]->SetSkillName(m_CurHeroUD, FName(*(m_UITableInfo->DisplayName.ToString() + "Active" + FString::FromInt(i+1) + "_1")), 1, (ESkillCategoryTypeEnum)(i));
		HeroSkillIcons[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (m_UITableInfo->ActiveSkill01.IsNone() && HeroSkillIcons.IsValidIndex(SKILL_ICON_INDEX_1))
	{
		HeroSkillIcons[SKILL_ICON_INDEX_1]->SetDisable(false);
		HeroSkillIcons[SKILL_ICON_INDEX_1]->PanelUnlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HeroSkillIcons[SKILL_ICON_INDEX_1]->LocalizingTextBlock_Skill_Passive->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (m_UITableInfo->ActiveSkill02.IsNone() && HeroSkillIcons.IsValidIndex(SKILL_ICON_INDEX_2))
	{
		HeroSkillIcons[SKILL_ICON_INDEX_2]->SetDisable(false);
		HeroSkillIcons[SKILL_ICON_INDEX_2]->PanelUnlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HeroSkillIcons[SKILL_ICON_INDEX_2]->LocalizingTextBlock_Skill_Passive->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (m_UITableInfo->ActiveSkill03.IsNone() && HeroSkillIcons.IsValidIndex(SKILL_ICON_INDEX_3))
	{
		HeroSkillIcons[SKILL_ICON_INDEX_3]->SetDisable(false);
		HeroSkillIcons[SKILL_ICON_INDEX_3]->PanelUnlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HeroSkillIcons[SKILL_ICON_INDEX_3]->LocalizingTextBlock_Skill_Passive->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (m_UITableInfo->PassiveSkill01.IsNone() && HeroSkillIcons.IsValidIndex(SKILL_ICON_INDEX_4))
	{
		HeroSkillIcons[SKILL_ICON_INDEX_4]->SetDisable(false);
		HeroSkillIcons[SKILL_ICON_INDEX_4]->PanelUnlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HeroSkillIcons[SKILL_ICON_INDEX_4]->LocalizingTextBlock_Skill_Passive->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 패시브02는 구현이 안돼있어서 unlock상태로 보이게 함.
// 	if (m_UITableInfo->PassiveSkill02.IsNone() && HeroSkillIcons.IsValidIndex(SKILL_ICON_INDEX_5))
	if(HeroSkillIcons.IsValidIndex(SKILL_ICON_INDEX_5))
	{
		HeroSkillIcons[SKILL_ICON_INDEX_5]->SetSkillName(m_CurHeroUD, FName(*(m_UITableInfo->DisplayName.ToString() + "Passive1_1")), 1, ESkillCategoryTypeEnum::VE_PassiveSkill01);

		HeroSkillIcons[SKILL_ICON_INDEX_5]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HeroSkillIcons[SKILL_ICON_INDEX_5]->SetDisable(false);
		HeroSkillIcons[SKILL_ICON_INDEX_5]->PanelUnlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HeroSkillIcons[SKILL_ICON_INDEX_5]->LocalizingTextBlock_Skill_Passive->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UUC_HeroManager_Info::SetHeroStat()
{
	for (UUC_HeroInfoStat* Stat : HeroStat)
	{
		if (IsValid(Stat))
			Stat->Refresh(m_UIBattleInfo);
	}

	FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(m_Hero->heroId);
	if (tableinfo)
	{
		FText txt = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, tableinfo->AttributeSkillString);
		if (IsValid(CounterSkillText))
			CounterSkillText->SetText(txt);

		txt = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, tableinfo->AttributeSynergyString);
		if (IsValid(CounterSynergyText))
			CounterSynergyText->SetText(txt);
	}
}

void UUC_HeroManager_Info::SetCurrentTab(EItemInventoryEnum InEnum)
{
	Switcher_ItemInventory->SetActiveWidgetIndex(1);
	CurrentTab = InEnum;
	InvalidateData();
}

void UUC_HeroManager_Info::OnClick_Lock()
{
	if (m_Hero == nullptr)
		return;

	if (m_Hero->locked)
		UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ({}, {}, {}, { m_CurHeroUD });
	else
		UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ({}, {}, { m_CurHeroUD }, {});
}

void UUC_HeroManager_Info::OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom)
{
	Super::OnItemInventoryUpdated(Result, bWidgetFinished, UpdatedScrollItem, WidgetReturnedFrom);

	OnItemInventoryUpdatedDelegate.Broadcast();
}

void UUC_HeroManager_Info::OnTabCheckStateChanged(bool bIsChecked, int32 Index)
{
	if (bIsChecked)
	{
		if (Index == 0)
		{
			Switcher_ItemInventory->SetActiveWidgetIndex(0);
			//Refresh();
		}
		else
		{
			Switcher_ItemInventory->SetActiveWidgetIndex(1);
			Super::OnTabCheckStateChanged(bIsChecked, Index);
		}
	}
}

void UUC_HeroManager_Info::OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem)
{
	auto ScrollData = Cast<UInventoryScrollDataBase>(SelectedItem->ItemData);
	check(ScrollData != nullptr);

	OnEquipmentItemSelected.ExecuteIfBound(ScrollData->ItemUD);
}

EItemInventoryEnum UUC_HeroManager_Info::TabIndexToInventoryEnum(int32 Index)
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
			ItemInventoryEnum = EItemInventoryEnum::Costume;
			break;
		default:
			UE_LOG(LogUI, Log, TEXT("UUC_HeroBaseInfo::TabIndexToInventoryEnum: Unexpected index"));
			break;
	}
	return ItemInventoryEnum;
}

bool UUC_HeroManager_Info::IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	if (ItemData->equipedHeroUD == m_CurHeroUD)
		return false;

	if (!ensure(m_UITableInfo != nullptr))
		return true;

	if (!FItemTableInfo::CanEquip(m_UITableInfo->Class, ItemInfo->EquipType))
		return false;

	return true;
}

void UUC_HeroManager_Info::InvalidateItemInventoryScrollView_Heros()
{
	if (InventoryScrollView && Switcher_ItemInventoryScrollView)
	{
		TArray<FString> TargetHeroUDs;

		UUP_HeroInfo_Upgrade* Upgrade = Cast<UUP_HeroInfo_Upgrade>(RGAMEINSTANCE(GEngine)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_HeroRatingUp));
		if (Upgrade)
			Upgrade->UpgradeHeroInfo->Filtering(TargetHeroUDs);

		if (TargetHeroUDs.Num() == 0)
		{
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(1);
		}
		else
		{
			InventoryScrollView->ClearScrollItemData(ITEMMAX_COUNT);
			Switcher_ItemInventoryScrollView->SetActiveWidgetIndex(0);

			for (size_t i = 0; i < TargetHeroUDs.Num(); ++i)
			{
				if (i >= ITEMMAX_COUNT)
					break;

				UInventoryScrollDataBase* ScrollItemData = nullptr;
				if (CachedItemDataList.IsValidIndex(i))
				{
					ScrollItemData = Cast<UInventoryScrollDataBase>(CachedItemDataList[i]);
					InventoryScrollView->ItemDataList.Emplace(ScrollItemData);
				}
				checkf(ScrollItemData, TEXT("failed to AddScrollItemData"));

				ScrollItemData->ItemUD = TargetHeroUDs[i];
				ScrollItemData->WalletKey = TEXT("");

				UpdateScrollItemData_Hero(ScrollItemData);
			}

			InventoryScrollView->InvalidateData();
		}
	}
}
