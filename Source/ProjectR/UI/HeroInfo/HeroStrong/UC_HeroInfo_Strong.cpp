// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"

#include "GameInfo/RInventory.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"
#include "../HeroManager/UP_HeroManager.h"
#include "UI/RWidgetManager.h"
#include "Utils/UIFunctionLibrary.h"
#include "UC_HeroInfo_Strong.h"

#include "Network/PacketFunctionLibrary.h"
#include "ResourceFunctionLibrary.h"
#include "Client_BattleFunctionLibrary.h"
#include "Table/CharacterCostEnchantGrade.h"
#include "UP_GenericScrollview.h"

void UUC_HeroInfo_Strong::NativeConstruct()
{
 	Super::NativeConstruct();

	URGameInstance*			RGameInstance = RGAMEINSTANCE(this);
	CurrentUsePoint = 0;

	if (variables.Button_EnchantPointPlus)
	{
		variables.Button_EnchantPointPlus->OnClicked.Clear();
		variables.Button_EnchantPointPlus->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong::OnClick_EnchanPointPlus);
		variables.Button_EnchantPointPlus->OnPressed.Clear();
		variables.Button_EnchantPointPlus->OnPressed.AddDynamic(this, &UUC_HeroInfo_Strong::OnPress_EnchantPointPlus);
		variables.Button_EnchantPointPlus->OnReleased.Clear();
		variables.Button_EnchantPointPlus->OnReleased.AddDynamic(this, &UUC_HeroInfo_Strong::OnRelease_EnchantPointPlus);
	}

	if (IsValid(variables.Button_Apply))
	{
		variables.Button_Apply->OnClicked.Clear();
		variables.Button_Apply->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong::OnClick_Apply);
	}

	if (variables.Button_Close)
	{
		variables.Button_Close->OnClicked.Clear();
		variables.Button_Close->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong::OnClick_Close);
	}

	if (variables.Button_Cancel)
	{
		variables.Button_Cancel->OnClicked.Clear();
		variables.Button_Cancel->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong::OnClick_Close);
	}
}

void UUC_HeroInfo_Strong::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HeroInfo_Strong::SetHero(FString HeroUD)
{
	URGameInstance*			RGameInstance = RGAMEINSTANCE(this);

	m_CurrentHeroUD = HeroUD;
	m_Hero = RGameInstance->RInventory->GetHeroWithUD(m_CurrentHeroUD);
	checkf(m_Hero != nullptr, TEXT("UUC_HeroInfo_Upgrade HeroObject Is Empty"));

	FCharacterTableInfo* Table = UTableManager::GetInstancePtr()->GetCharacterRow(m_Hero->heroId);
	checkf(Table != nullptr, TEXT("UUC_HeroInfo_Upgrade CharacterTable Is Empty"));

	int32 AlreadyUsePoint = 0;

	if (IsValid(variables.HeroIcon))
		variables.HeroIcon->SetHeroIconInfoWithUD(m_Hero->heroUD, EHeroOwnershipType::VE_Me);

	if (IsValid(variables.Text_HeroName))
		variables.Text_HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, Table->DisplayName));

	if (variables.Text_HeroNickName)
		variables.Text_HeroNickName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, Table->Nick));

	if (variables.Image_Class)
		UUIFunctionLibrary::SetImageBrush(UResourceFunctionLibrary::GetHeroClassTypeSprite(Table->Class), variables.Image_Class);

	if (variables.Text_CombatPower)
		variables.Text_CombatPower->SetText(FText::AsNumber(UClient_BattleFunctionLibrary::GetHeroCombatStat(HeroUD)));

	if (variables.HeroStar)
		variables.HeroStar->SetStar(HeroUD);

	if (variables.Text_HeroLevel)
		variables.Text_HeroLevel->SetText(FText::FromString(FString::FromInt(m_Hero->level) + FString(" / ") + FString::FromInt(Table->MaxLevel)));

	for (int32 i = 0; i < m_Hero->enchantStat.Num(); i++)
		AlreadyUsePoint += m_Hero->enchantStat[i];

	FCharacterCostEnchantGrade* EnchantCostInfo = UTableManager::GetInstancePtr()->GetCharacterCostEnchantGradeInfo(FName(*FString::FromInt(Table->Grade)));

	if (variables.Text_EnchantLevel)
		variables.Text_EnchantLevel->SetText(FText::FromString(FString::FromInt(AlreadyUsePoint) + FString(" / ") + EnchantCostInfo->MaxEnchantPoint.ToString()));

	if (variables.Text_EL_Before)
		variables.Text_EL_Before->SetText(FText::FromString(FString::FromInt(AlreadyUsePoint)));

	if (variables.Text_EL_After)
		variables.Text_EL_After->SetText(FText::FromString(FString::FromInt(AlreadyUsePoint + CurrentUsePoint)));

	if (variables.Text_EnchantPoint)
		variables.Text_EnchantPoint->SetText(FText::FromString(FString::FromInt(m_Hero->enchantPoint)));

	if (variables.Text_EP_After)
		variables.Text_EP_After->SetText(FText::FromString(FString::FromInt(CurrentUsePoint)));


	sharedStat.InitStat(m_Hero->enchantPoint, AlreadyUsePoint, FCString::Atoi(*EnchantCostInfo->StatMaxPoint.ToString()), variables.Text_EnchantPoint, variables.Text_EP_After, variables.Text_EL_After);
	InvalidateDataStat();
}

void UUC_HeroInfo_Strong::SetMaterial(TArray<FString>& InHeroUD, TArray<int32>& InPer)
{
// 	for (int32 i = 0; i < WidgetSwitcher_Icon.Num(); i++)
// 	{
// 		if(IsValid(WidgetSwitcher_Icon[i]) && WidgetSwitcher_Icon.IsValidIndex(i))
// 			WidgetSwitcher_Icon[i]->SetActiveWidgetIndex(1);
// 		if (MaterialHeroIcon.IsValidIndex(i) && InHeroUD.IsValidIndex(i))
// 			MaterialHeroIcon[i]->SetHeroIconInfoWithUD(InHeroUD[i], EHeroOwnershipType::VE_Me);
// 		if (IsValid(Text_EnchantPer[i]) && Text_EnchantPer.IsValidIndex(i) && InPer.IsValidIndex(i))
// 			Text_EnchantPer[i]->SetText(FText::Format(FText::FromString(TEXT("{0}.0%")), InPer[i]));
// 
// 		if (InHeroUD.IsValidIndex(i) == false)
// 		{
// 			if (IsValid(WidgetSwitcher_Icon[i]) && WidgetSwitcher_Icon.IsValidIndex(i))
// 				WidgetSwitcher_Icon[i]->SetActiveWidgetIndex(0);
// 			if (IsValid(Text_EnchantPer[i]) && Text_EnchantPer.IsValidIndex(i))
// 				Text_EnchantPer[i]->SetText(FText::Format(FText::FromString(TEXT("{0}.0%")), 0));
// 		}
// 	}
}

void UUC_HeroInfo_Strong::InvalidateDataStat()
{
	variables.HeroStats[0]->SetStat(m_CurrentHeroUD, EHERO_ENCHANT_STAT_SLOT::MaxHP, &sharedStat);
	variables.HeroStats[1]->SetStat(m_CurrentHeroUD, EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower, &sharedStat);
	variables.HeroStats[2]->SetStat(m_CurrentHeroUD, EHERO_ENCHANT_STAT_SLOT::Armor, &sharedStat);
	variables.HeroStats[3]->SetStat(m_CurrentHeroUD, EHERO_ENCHANT_STAT_SLOT::MagicResistance, &sharedStat);
	for (UUC_HeroInfo_Strong_Stat* HeroStat : variables.HeroStats)
	{
		HeroStat->InvalidateNewStat();
		HeroStat->InvalidateData();
	}
		
	if (m_Hero->enchantPoint <= 0)
		variables.Button_Apply->SetIsEnabled(false);
	else
		variables.Button_Apply->SetIsEnabled(true);
	sharedStat.InvalidateData();
}

void UUC_HeroInfo_Strong::OnClick_EnchanPointPlus()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		GenericScrollview->SetState(EScrollviewType::VE_HERO_ENCHANT);
		GenericScrollview->Appear(true);
	}
}

void UUC_HeroInfo_Strong::OnPress_EnchantPointPlus()
{
	variables.Switcher_EnchantPointPlusImage->SetActiveWidgetIndex(1);
}

void UUC_HeroInfo_Strong::OnRelease_EnchantPointPlus()
{
	variables.Switcher_EnchantPointPlusImage->SetActiveWidgetIndex(0);
}

void UUC_HeroInfo_Strong::OnClick_Apply()
{
	URGameInstance*			RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_HeroInfo_Upgrade[RGameInstance Is Empty]"));

	TArray<int32> Stats;
	for (int32 i = 0; i < 5; i++)
		Stats.Emplace(0);

	UCharacterBattleInfo* BattleCharacterInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(m_CurrentHeroUD);

	int32 AttackPower = 0;

	if (BattleCharacterInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower))
		AttackPower = 0;
	else if (BattleCharacterInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower))
		AttackPower = 1;

	for (UUC_HeroInfo_Strong_Stat* HeroStat : variables.HeroStats)
	{
		int32	EnchantSlot = (int32)HeroStat->GetEnchantSlot();
		if (HeroStat->GetEnchantSlot() == EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower)
		{
			EnchantSlot += AttackPower;
		}
		if (HeroStat->GetNewStat() > 0 && Stats.IsValidIndex(EnchantSlot))
			Stats[EnchantSlot] = HeroStat->GetNewStat();
	}

	int32 Sum = 0;

	for (int32 i = 0; i < 5; i++)
	{
		if(Stats.IsValidIndex(i) == false)
			continue;

		Sum += Stats[i];
	}

	if (Sum == 0)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroEnchant_SplashPopUp_Meassage03"))));

		return;
	}


	m_PrevStat = m_Hero->enchantStat;
	BattleCharacterInfo = nullptr;
	UPacketFunctionLibrary::INVENTORY_HERO_ENCHANT_STAT_RQ(m_CurrentHeroUD, Stats);
}

void UUC_HeroInfo_Strong::OnClick_Close()
{
	UUP_GenericScrollview* GenericScrollView = UUIFunctionLibrary::GetGenericScrollview();

	if (GenericScrollView)
	{
		if (GenericScrollView->isInsideViewport())
			GenericScrollView->Appear(false);
	}
	RGAMEINSTANCE(this)->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_HeroReinforce);
}

void HeroStrongSharedStat::InitStat(int32 InStat, int32 enchantLv, int32 maxP, UTextBlock* text_ep, UTextBlock* text_ep_after, UTextBlock* text_el_after)
{
	sharedInt = InStat;
	m_enchantLv = enchantLv;
	sharedIntBackup = InStat;
	WidgetList.Empty();
	Text_EnchantPoint = text_ep;
	Text_EP_After = text_ep_after;
	Text_EL_After = text_el_after;
	statMaxPoint = maxP;
	UsePoint = 0;
}

void HeroStrongSharedStat::AddWidget(URBaseWidget* InWidget)
{
	WidgetList.AddUnique(InWidget);
}

int32 HeroStrongSharedStat::Write()
{
	if (EnableStat())
	{
		sharedInt--;
		++UsePoint;
		if (Text_EP_After)
			Text_EP_After->SetText(FText::AsNumber(UsePoint));
		if (Text_EL_After)
			Text_EL_After->SetText(FText::AsNumber(m_enchantLv + (sharedIntBackup - sharedInt)));

		if (Text_EnchantPoint)
		{
			Text_EnchantPoint->SetText(FText::AsNumber(sharedInt));
		}
		return 1;
	}
 	return 0;
}

void HeroStrongSharedStat::Erase()
{
	if (sharedInt < sharedIntBackup)
		sharedInt++;
	if (sharedInt >= sharedIntBackup)
		sharedInt = sharedIntBackup;

	--UsePoint;
	if (Text_EP_After)
		Text_EP_After->SetText(FText::AsNumber(UsePoint));
	if (Text_EL_After)
		Text_EL_After->SetText(FText::AsNumber(m_enchantLv + (sharedIntBackup - sharedInt)));

	if (Text_EnchantPoint)
	{
		Text_EnchantPoint->SetText(FText::AsNumber(sharedInt));
	}
}

bool HeroStrongSharedStat::EnableStat() const
{
	if (sharedInt <= 0)
		return false;
 	return true;
}

void HeroStrongSharedStat::InvalidateData()
{
	for (URBaseWidget* widget : WidgetList)
		widget->InvalidateData();
}

bool HeroStrongSharedStat::LessThanMax(int32 newStat)
{
	if (newStat < statMaxPoint)
		return true;
	else
		return false;
}
