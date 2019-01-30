// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"

#include "GameInfo/RInventory.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/SharedUtils/Renewal_BattleFunctionLibrary.h"

#include "UC_HeroInfo_Strong_Stat.h"
#include "UI/HeroInfo/HeroStrong/UC_HeroInfo_Strong.h"


void UUC_HeroInfo_Strong_Stat::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(Button_Plus))
	{
		Button_Plus->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Stat::OnClick_Plus);
		Button_Plus->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong_Stat::OnClick_Plus);
	}
	if (IsValid(Button_Minus))
	{
		Button_Minus->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Stat::OnClick_Minus);
		Button_Minus->OnClicked.AddDynamic(this, &UUC_HeroInfo_Strong_Stat::OnClick_Minus);
	}
}

void UUC_HeroInfo_Strong_Stat::NativeDestruct()
{
	if (IsValid(Button_Plus))
		Button_Plus->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Stat::OnClick_Plus);
	if (IsValid(Button_Minus))
		Button_Minus->OnClicked.RemoveDynamic(this, &UUC_HeroInfo_Strong_Stat::OnClick_Minus);
	Super::NativeDestruct();
}

void UUC_HeroInfo_Strong_Stat::InvalidateData()
{
	if (sharedStat->EnableStat() && sharedStat->LessThanMax(ExistStat + NewStat))
	{
		Button_Plus->SetIsEnabled(true);
		Button_Minus->SetIsEnabled(NewStat > 0 ? true : false);
	}
	else
	{
		Button_Plus->SetIsEnabled(false);
		Button_Minus->SetIsEnabled(false);
	}
	if (NewStat > 0)
		Button_Minus->SetIsEnabled(true);
	SetCaption();
	SetEnchantPoint();
	SetCurrentValue();
	SetNextValue();
}

void UUC_HeroInfo_Strong_Stat::InvalidateNewStat()
{
	NewStat = 0;
}

void UUC_HeroInfo_Strong_Stat::SetCaption()
{
	FString TitleString = TEXT("");
	switch (eEnchantStateSlot)
	{
	case 	EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower:
		TitleString = TEXT("USK_uc_heroProperty_Phyattack");
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MagicAttackPower:
		TitleString = TEXT("USK_uc_heroProperty_Magicattack");
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::Armor:
		TitleString = TEXT("USK_uc_heroProperty_Type_2");
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MagicResistance:
		TitleString = TEXT("USK_uc_heroProperty_MagicDefence");
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MaxHP:
		TitleString = TEXT("USK_uc_heroProperty_Type_4");
		break;
	}
	if (IsValid(TextBlock_StatName))
		TextBlock_StatName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(*TitleString)));
}

void UUC_HeroInfo_Strong_Stat::SetEnchantPoint()
{
	if (Text_EnchantPoint)
	{
		Text_EnchantPoint->SetText(FText::FromString(FString::FromInt(ExistStat + NewStat)));
		if (NewStat > 0)
		{
			if(sharedStat->LessThanMax(ExistStat + NewStat))
				Text_EnchantPoint->SetColorAndOpacity(FSlateColor(FLinearColor(0.026f, 0.617f, 1.f)));
			else
				Text_EnchantPoint->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f)));
		}
		else
			Text_EnchantPoint->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f)));
	}
}

void UUC_HeroInfo_Strong_Stat::SetCurrentValue()
{
	if (IsValid(TextBlock_CurrentValule))
	{
		TextBlock_CurrentValule->SetText(FText::AsNumber(iProperty));
	}
}

void UUC_HeroInfo_Strong_Stat::SetNextValue()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("UUC_StatHeroEnchant::SetCurrentValue[ RGameInstance is Empty]"));

	FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(CurrentUD);

	checkf(HeroData != nullptr, TEXT("%s is Hero Empty"), *CurrentUD);

//	UCharacterBattleInfo* BattleCharacterInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(RGameInstance, CurrentUD);

	int32	AttackPower = 0;

	AttackPower = int32(eEnchantStateSlot);

	int CProperty = 0;
	if(HeroData->enchantStat.GetAllocatedSize() > 0)
		CProperty = URenewal_BattleFunctionLibrary::GetHeroEnchantPoint(CurrentClass, eEnchantStateSlot, HeroData->enchantStat[AttackPower]);

	if (NewStat > 0)
	{
		int AProperty = URenewal_BattleFunctionLibrary::GetHeroEnchantPoint(CurrentClass, eEnchantStateSlot, ExistStat);
		int BProperty = URenewal_BattleFunctionLibrary::GetHeroEnchantPoint(CurrentClass, eEnchantStateSlot, ExistStat + NewStat);
		iNextProperty = iProperty + (BProperty - AProperty);
	}
	else
	{
		iNextProperty = iProperty;
	}
	if (IsValid(TextBlock_NextValule))
		TextBlock_NextValule->SetText(FText::AsNumber(iNextProperty));
	if (IsValid(TextBlock_ChangeValue))
	{
		if ((CProperty + iNextProperty - iProperty) > 0)
			TextBlock_ChangeValue->SetText(FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(CProperty + iNextProperty - iProperty)));
		else
			TextBlock_ChangeValue->SetText(FText::FromString(TEXT("")));
	}
}

void UUC_HeroInfo_Strong_Stat::OnClick_Plus()
{
	if (sharedStat->EnableStat() && sharedStat->LessThanMax(ExistStat + NewStat))
	{
		NewStat += sharedStat->Write();
		sharedStat->InvalidateData();
	}
}

void UUC_HeroInfo_Strong_Stat::OnClick_Minus()
{
	sharedStat->Erase();
	if (NewStat > 0)
		NewStat--;
	sharedStat->InvalidateData();
}

void UUC_HeroInfo_Strong_Stat::SetStat(const FString& InCurrentUD, EHERO_ENCHANT_STAT_SLOT slotEnum, HeroStrongSharedStat* InSharedStat)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(InSharedStat != nullptr, TEXT("InSharedState is nullptr"));
	checkf(RGameInstance != nullptr, TEXT("UUC_StatHeroEnchant::SetCurrentValue[ RGameInstance is Empty]"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("UUC_StatHeroEnchant::SetCurrentValue[ RGameInstance is Empty]"));
	CurrentUD = InCurrentUD;
	UCharacterBattleInfo* BattleCharacterInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(CurrentUD);
	if (slotEnum == EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower)
	{
		if (BattleCharacterInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower))
			eEnchantStateSlot = EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower;
		else if (BattleCharacterInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower))
			eEnchantStateSlot = EHERO_ENCHANT_STAT_SLOT::MagicAttackPower;
	}
	else
		eEnchantStateSlot = slotEnum;
	sharedStat = InSharedStat;
	sharedStat->AddWidget(this);
	FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(CurrentUD);

	checkf(HeroData != nullptr, TEXT("%s is Hero Empty"), *CurrentUD);
	if (HeroData->enchantStat.IsValidIndex((int32)eEnchantStateSlot))
		ExistStat = HeroData->enchantStat[(int32)eEnchantStateSlot];
	else
		ExistStat = 0;
	FCharacterTableInfo* CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroData->heroId);
	CurrentClass = CharacterInfo->Class;

	int32 CharacterProperty = 0;
	switch (eEnchantStateSlot)
	{
	case 	EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower:
		CharacterProperty = BattleCharacterInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower);
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MagicAttackPower:
			CharacterProperty = BattleCharacterInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MagicAttackPower);
			break;
	case 	EHERO_ENCHANT_STAT_SLOT::Armor:
		CharacterProperty = BattleCharacterInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_Armor);
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MagicResistance:
		CharacterProperty = BattleCharacterInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicResistance);
		break;
	case 	EHERO_ENCHANT_STAT_SLOT::MaxHP:
		CharacterProperty = BattleCharacterInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
		break;
	}
	BattleCharacterInfo = nullptr;
	iProperty = CharacterProperty;
}

int32 UUC_HeroInfo_Strong_Stat::GetNewStat() const
{
	return NewStat;
}

EHERO_ENCHANT_STAT_SLOT UUC_HeroInfo_Strong_Stat::GetEnchantSlot() const
{
	return eEnchantStateSlot;
}
