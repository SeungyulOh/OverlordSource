// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroLevelUp.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"

#include "GameInfo/RInventory.h"

#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Utils/Client_BattleFunctionLibrary.h"
#include "Utils/ResourceFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "Network/PacketFunctionLibrary.h"


void UUC_HeroLevelUp::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_HeroLevelUp::NativeDestruct()
{
	this->SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeDestruct();
}

void UUC_HeroLevelUp::Init()
{
	FindChildWidgetsWithCertainName<UImage>(this, HeroStar, TEXT("LHeroStar_"));
	FindChildWidgetsWithCertainName<UUC_StatHeroLevelUp>(this, HeroStats, TEXT("LevelUpStat_"));

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_HeroLevelUp::OnClick_Close);
		Button_Close->OnClicked.AddDynamic(this, &UUC_HeroLevelUp::OnClick_Close);
	}
		

	if (IsValid(Button_Plus))
	{
		Button_Plus->OnClicked.RemoveDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderPlus);
		Button_Plus->OnClicked.AddDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderPlus);
	}
		
	if (IsValid(Button_Minus))
	{
		Button_Minus->OnClicked.RemoveDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderMinus);
		Button_Minus->OnClicked.AddDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderMinus);
	}
		
	if (IsValid(Button_Max))
	{
		Button_Max->OnClicked.RemoveDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderMax);
		Button_Max->OnClicked.AddDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderMax);
	}
		
	if (IsValid(Slider_Level))
	{
		Slider_Level->OnValueChanged.RemoveDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderValueChange);
		Slider_Level->OnValueChanged.AddDynamic(this, &UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderValueChange);
	}
		
}

void UUC_HeroLevelUp::SetHero(FString InHeroUD)
{
	URGameInstance*		Game = RGAMEINSTANCE(this);
	URInventory*		Inven = Game->RInventory;

	checkf(Game != nullptr, TEXT("UUC_HeroManager_Info[RGameInstance Is Empty]"));
	checkf(Inven != nullptr, TEXT("UUC_HeroManager_Info[RInventory Is Empty]"));

	for (UUC_StatHeroLevelUp* HeroStat : HeroStats)
		HeroStat->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(Slider_Level))
	{
		Slider_Level->SetValue(0.0f);
		Image_Progress->SetRenderScale(FVector2D(0.f, 1.f));
	}
		

	m_CurHeroUD = InHeroUD;

	if (m_CurHeroUD.IsEmpty())
		return;

	m_Hero = Inven->GetHeroWithUD(m_CurHeroUD);

	if (m_Hero == nullptr)
		return;

	m_UIBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(m_CurHeroUD);
	m_UITableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, m_Hero->heroId);

	m_MinLevel = m_Hero->level;
	m_MaxLevel = m_UITableInfo->MaxLevel;

	checkf(m_UIBattleInfo != nullptr, TEXT("UUC_HeroManager_Info[m_UIBattleInfo Is Empty]"));
	checkf(m_UITableInfo != nullptr, TEXT("UUC_HeroManager_Info[m_UITableInfo Is Empty]"));

	SetHeroInfo();
	InvalidateDataStat(m_MinLevel, m_MinLevel);
	HeroLevelUp_SliderReset();
}

void UUC_HeroLevelUp::SetHeroInfo()
{
	if (IsValid(LHeroName))
	{
		FText HeroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_UITableInfo->DisplayName);
		LHeroName->SetText(HeroName);
	}

	if (IsValid(LHeroNickName))
	{
		FText HeroNickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, m_UITableInfo->Nick);
		LHeroNickName->SetText(HeroNickName);
	}

	int32 HeroAttackPower = UClient_BattleFunctionLibrary::GetHeroCombatStat(m_CurHeroUD);

	if (IsValid(Text_LAttackPower))
		Text_LAttackPower->SetText(FText::AsNumber(HeroAttackPower));

	if (IsValid(Text_LLevel))
		Text_LLevel->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), m_Hero->level, m_UITableInfo->MaxLevel));

	if(IsValid(Text_CurrentLevel))
		Text_CurrentLevel->SetText(FText::AsNumber(m_Hero->level));
	if (IsValid(Text_NextLevel))
		Text_NextLevel->SetText(FText::AsNumber(m_Hero->level));

	if (IsValid(Text_LReinForce))
	{
		int32		sum = 0;

		for (int32 p : m_Hero->enchantStat)
			sum += p;

		if (sum)
			Text_LReinForce->SetVisibility(ESlateVisibility::Visible);
		else
			Text_LReinForce->SetVisibility(ESlateVisibility::Collapsed);

		Text_LReinForce->SetText(FText::Format(FText::FromString(TEXT("Hardening phase+ {0}")), sum));
	}

	for (UImage* ImgStar : HeroStar)
	{
		if (IsValid(ImgStar))
			ImgStar->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 i = 0; i < m_UITableInfo->Grade; i++)
	{
		if (HeroStar.IsValidIndex(i))
			HeroStar[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsValid(WidgetSwitcher_Star))
	{
		if (m_UITableInfo->Grade <= 5)
			WidgetSwitcher_Star->SetActiveWidgetIndex(0);
		else if (m_UITableInfo->Grade < 10)
			WidgetSwitcher_Star->SetActiveWidgetIndex(1);
		else
			WidgetSwitcher_Star->SetActiveWidgetIndex(2);
	}

	if (IsValid(Image_LJob))
	{
		UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(m_UITableInfo->Class);
		if (Sprite)
			UUIFunctionLibrary::SetImageBrush(Sprite, Image_LJob);
	}

	if (IsValid(IconLevelUpHero))
		IconLevelUpHero->SetHeroIconInfoWithUD(m_CurHeroUD, EHeroOwnershipType::VE_Me);
}

void UUC_HeroLevelUp::InvalidateDataStat(int32 currentLevel, int32 nextLevel)
{
	const UDataTable* HeroGrowthTable = UTableManager::GetInstancePtr()->GetHeroGrowthTable(m_UITableInfo->GrowthTableID);
	FGrowthHeroInfo_V2* CurrentHeroGrowthInfo = UTableManager::GetHeroGrowthInfo(HeroGrowthTable, currentLevel);
	FGrowthHeroInfo_V2* NextHeroGrowthInfo = UTableManager::GetHeroGrowthInfo(HeroGrowthTable, nextLevel);
	checkf(HeroGrowthTable != nullptr, TEXT("HeroLevelUp HeroGrowthTable Is Empty"));
	checkf(CurrentHeroGrowthInfo != nullptr, TEXT("HeroLevelUp CurrentHeroGrowthInfo Is Empty"));
	checkf(NextHeroGrowthInfo != nullptr, TEXT("HeroLevelUp NextHeroGrowthInfo Is Empty"));

 	if (HeroStats.IsValidIndex(HERO_STAT_MaxHP) && IsValid(HeroStats[HERO_STAT_MaxHP]))
 	{
 		HeroStats[HERO_STAT_MaxHP]->SetStat(
 			FString::FromInt(m_UITableInfo->MaxHP + CurrentHeroGrowthInfo->MaxHP),
 			FString::FromInt(m_UITableInfo->MaxHP + NextHeroGrowthInfo->MaxHP),
 			TEXT("USK_uc_heroProperty_Type_4"),
 			FString::FromInt(NextHeroGrowthInfo->MaxHP - CurrentHeroGrowthInfo->MaxHP));
 		HeroStats[HERO_STAT_MaxHP]->SetVisibility(ESlateVisibility::Visible);
 	}
 
 	if (HeroStats.IsValidIndex(HERO_STAT_AttackPower) && IsValid(HeroStats[HERO_STAT_AttackPower]))
 	{
		if (m_UITableInfo->AttackType == EAttackTypeEnum::VE_Magic)
		{
			HeroStats[HERO_STAT_AttackPower]->SetStat(
				FString::FromInt(m_UITableInfo->MagicAttackPower + CurrentHeroGrowthInfo->MagicAttackPower),
				FString::FromInt(m_UITableInfo->MagicAttackPower + NextHeroGrowthInfo->MagicAttackPower),
				TEXT("USK_uc_heroProperty_Magicattack"),
				FString::FromInt(NextHeroGrowthInfo->MagicAttackPower - CurrentHeroGrowthInfo->MagicAttackPower));
		}
		else
		{
			HeroStats[HERO_STAT_AttackPower]->SetStat(
				FString::FromInt(m_UITableInfo->PhysicalAttackPower + CurrentHeroGrowthInfo->PhysicalAttackPower),
				FString::FromInt(m_UITableInfo->PhysicalAttackPower + NextHeroGrowthInfo->PhysicalAttackPower),
				TEXT("USK_uc_heroProperty_Phyattack"),
				FString::FromInt(NextHeroGrowthInfo->PhysicalAttackPower - CurrentHeroGrowthInfo->PhysicalAttackPower));
		}

 		
 		HeroStats[HERO_STAT_AttackPower]->SetVisibility(ESlateVisibility::Visible);
 	}
 	if (HeroStats.IsValidIndex(HERO_STAT_Armor) && IsValid(HeroStats[HERO_STAT_Armor]))
 	{
 		HeroStats[HERO_STAT_Armor]->SetStat(
 			FString::FromInt(m_UITableInfo->Armor + CurrentHeroGrowthInfo->Armor),
 			FString::FromInt(m_UITableInfo->Armor + NextHeroGrowthInfo->Armor),
 			TEXT("USK_uc_heroProperty_Type_2"),
 			FString::FromInt(NextHeroGrowthInfo->Armor - CurrentHeroGrowthInfo->Armor));
 		HeroStats[HERO_STAT_Armor]->SetVisibility(ESlateVisibility::Visible);
 	}
 
 	if (HeroStats.IsValidIndex(HERO_STAT_MagicResistance) && IsValid(HeroStats[HERO_STAT_MagicResistance]))
 	{
 		HeroStats[HERO_STAT_MagicResistance]->SetStat(
 			FString::FromInt(m_UITableInfo->MagicResistance + CurrentHeroGrowthInfo->MagicResistance),
 			FString::FromInt(m_UITableInfo->MagicResistance + NextHeroGrowthInfo->MagicResistance),
 			TEXT("USK_uc_heroProperty_MagicDefence"),
 			FString::FromInt(NextHeroGrowthInfo->MagicResistance - CurrentHeroGrowthInfo->MagicResistance));
 		HeroStats[HERO_STAT_MagicResistance]->SetVisibility(ESlateVisibility::Visible);
 	}
}

void UUC_HeroLevelUp::RefreshLevelUpUI()
{
	URGameInstance*		RGameInstance	=	RGAMEINSTANCE(this);
	float				CurMaxLevel		=	0;
	float				EnableLevel		=	m_MaxLevel - m_MinLevel;
	float				CurLevel		=	m_MinLevel + EnableLevel * FMath::Clamp<float>(Slider_Level->GetValue(), 0, 1.0f);

	CurMaxLevel = (float)m_MaxLevel;

	InvalidateDataStat(m_MinLevel, CurLevel);

	if (Text_SliderMin)
		Text_SliderMin->SetText(FText::AsNumber((int32)CurLevel));

	if (Text_SliderMax)
		Text_SliderMax->SetText(FText::AsNumber((int32)CurMaxLevel));

	if(Text_NextLevel)
		Text_NextLevel->SetText(FText::AsNumber((int32)CurLevel));

	const int32 MyGold = RGameInstance->RInventory->GetGold();
	const int32 MyRune = RGameInstance->RInventory->GetHeroRune();

	m_NeedGold		=	0;
	m_NeedHeroRune	=	0;

	FCharacterCostGrowth*	NeedMaterial = nullptr;
	for (int i = m_MinLevel; i < (int)CurLevel; i++)
	{
		NeedMaterial = UTableManager::GetInstancePtr()->GetCharacterCostGrowthRow(FName(*FString::FromInt(i)));
		checkf(NeedMaterial != nullptr, TEXT("FCharacterCostGrowth is Empty"));
		m_NeedGold += NeedMaterial->Cost_Gold;
		m_NeedHeroRune += NeedMaterial->Cost_HeroRune;
	}
	Button_LevelUp->SetIsEnabled(true);
	if (m_NeedGold > MyGold || m_NeedHeroRune > MyRune || (int32)CurLevel <= m_Hero->level)
		Button_LevelUp->SetIsEnabled(false);

	if (IsValid(Text_Gold))
	{
		if (m_NeedGold > MyGold)
			Text_Gold->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f)));
		else
			Text_Gold->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));

		Text_Gold->SetText(FText::AsNumber(m_NeedGold));
	}
	if (IsValid(Text_HeroRune))
	{
		if (m_NeedHeroRune > MyRune)
			Text_HeroRune->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f)));
		else
			Text_HeroRune->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));

		Text_HeroRune->SetText(FText::AsNumber(m_NeedHeroRune));
	}
}

void UUC_HeroLevelUp::HeroLevelUp_SliderReset()
{
	if (IsValid(Slider_Level) == false)
		return;

	int32 EnableLevel = m_MaxLevel - m_MinLevel;
	float fSlideValue = FMath::Clamp<float>(Slider_Level->GetValue(), 0, 1.0f);
	float CurLevel = m_MinLevel + (float)EnableLevel * fSlideValue;
	float SliderPercent2 = 1.0f / (static_cast<float>(EnableLevel));
	float NextLevel = m_MinLevel;
	if (NextLevel > 0 && NextLevel >= m_Hero->level)
	{
		Slider_Level->SetValue((NextLevel - m_MinLevel) * SliderPercent2);
		Image_Progress->SetRenderScale(FVector2D((NextLevel - m_MinLevel) * SliderPercent2, 1.f));
	}
		
	RefreshLevelUpUI();
}

bool UUC_HeroLevelUp::isLackOfMaterial()
{
	URGameInstance*		RGameInstance	=	RGAMEINSTANCE(this);
	const int32			MyGold			=	RGameInstance->RInventory->GetGold();
	const int32			MyRune			=	RGameInstance->RInventory->GetHeroRune();

	if (MyGold < m_NeedGold || MyRune < m_NeedHeroRune)
		return true;

	return false;
}

void UUC_HeroLevelUp::Refresh(FString InHeroUD)
{
	SetHero(InHeroUD);
}

void UUC_HeroLevelUp::OnClick_Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderPlus()
{
	if (IsValid(Slider_Level) == false)
		return;

	int32 EnableLevel		=	m_MaxLevel - m_MinLevel;
	float fSlideValue		=	FMath::Clamp<float>(Slider_Level->GetValue(), 0, 1.0f);
	float CurLevel			=	m_MinLevel + (float)EnableLevel * fSlideValue;
	float SliderPercent2	=	1.0f / (static_cast<float>(EnableLevel));
	float NextLevel			=	CurLevel + 1.0f;

	if (NextLevel <= m_MaxLevel)
	{
		Slider_Level->SetValue((NextLevel - m_MinLevel) * SliderPercent2);
		Image_Progress->SetRenderScale(FVector2D((NextLevel - m_MinLevel) * SliderPercent2, 1.f));
	}
		

	RefreshLevelUpUI();
}

void UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderMinus()
{
	if (IsValid(Slider_Level) == false)
		return;

	int32 EnableLevel		=	m_MaxLevel - m_MinLevel;
	float fSlideValue		=	FMath::Clamp<float>(Slider_Level->GetValue(), 0, 1.0f);
	float CurLevel			=	m_MinLevel + (float)EnableLevel * fSlideValue;
	float SliderPercent2	=	1.0f / (static_cast<float>(EnableLevel));
	float NextLevel			=	CurLevel - 1.0f;

	if (NextLevel > 0 && NextLevel >= m_Hero->level)
	{
		Slider_Level->SetValue((NextLevel - m_MinLevel) * SliderPercent2);
		Image_Progress->SetRenderScale(FVector2D((NextLevel - m_MinLevel) * SliderPercent2, 1.f));
	}
		

	RefreshLevelUpUI();
}

void UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderMax()
{
	if (IsValid(Slider_Level) == false)
		return;

	int32 EnableLevel		=	m_MaxLevel - m_MinLevel;
	float fSlideValue		=	FMath::Clamp<float>(Slider_Level->GetValue(), 0, 1.0f);
	float CurLevel			=	m_MinLevel + (float)EnableLevel * fSlideValue;
	float SliderPercent2	=	1.0f / (static_cast<float>(EnableLevel));
	float NextLevel			=	m_MaxLevel;

	if (NextLevel > 0 && NextLevel >= m_Hero->level)
	{
		Slider_Level->SetValue((NextLevel - m_MinLevel) * SliderPercent2);
		Image_Progress->SetRenderScale(FVector2D((NextLevel - m_MinLevel) * SliderPercent2, 1.f));
	}
		

	RefreshLevelUpUI();
}

void UUC_HeroLevelUp::OnClick_HeroLevelUp_SliderValueChange(float InChangeValue)
{
	if (IsValid(Slider_Level) == false)
		return;

	Slider_Level->SetValue(InChangeValue);

	if (IsValid(Image_Progress))
		Image_Progress->SetRenderScale(FVector2D(InChangeValue, 1.f));

	int32 EnableLevel = m_MaxLevel - m_MinLevel;
	int32 CurLevel = m_MinLevel + EnableLevel * FMath::Clamp<float>(Slider_Level->GetValue(), 0, 1.0f);

	float SliderPercent2 = 1.0f / (static_cast<float>(EnableLevel));
	float NextLevel = CurLevel;

	if (NextLevel > 0 && NextLevel >= m_Hero->level)
	{
		Slider_Level->SetValue((NextLevel - m_MinLevel) * SliderPercent2);
		Image_Progress->SetRenderScale(FVector2D((NextLevel - m_MinLevel) * SliderPercent2, 1.f));
	}
		

	RefreshLevelUpUI();
}