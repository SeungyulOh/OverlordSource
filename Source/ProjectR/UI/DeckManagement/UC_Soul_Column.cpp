// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Soul_Column.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/DeckManagement/UC_SkillForDeck.h"
#include "UtilFunctionIntegrated.h"

#include "GameInfo/RInventory.h"
#include "Global/TableManager.h"

void UUC_Soul_Column::NativeConstruct()
{
	Super::NativeConstruct();

	SetEmpty();
}

void UUC_Soul_Column::SetEmpty()
{
	InitHeroPortrait();
	InitSkillIcon();
	InitText();
}

void UUC_Soul_Column::SetSoulInfo(FString InHeroUD)
{
	if (InHeroUD.IsEmpty())
	{
		SetEmpty();
		return;
	}
	//SoulIcon
	if(IsValid(SoulIcon))
	{
// 		FHERO_ICON_INFO iconinfo;
// 		UUIFunctionLibrary::GetHeroIconInfoWithUD(iconinfo, InHeroUD, EHeroOwnershipType::VE_Me);
		SoulIcon->SetHeroIconInfoWithUD(InHeroUD, EHeroOwnershipType::VE_Me);
	}

	//SoulSkill
	if (IsValid(SoulSkill))
	{
		SoulSkill->SetSkillIcon(InHeroUD, true);
	}

	//SoulDescription
	if (IsValid(SoulDescription))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);

		FHERO* pHero = RGameInstance->RInventory->GetHeroWithUD(InHeroUD);
		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(pHero->heroId);
		FSkillTableInfo_V2* SpiritSkill = UTableManager::GetInstancePtr()->GetSkillRow_V2(CharacterTableInfo->SpiritPassive);
		
		checkf(SpiritSkill != nullptr, TEXT("UUC_Soul_Column::SetSoulInfo[ SpiritSkill is Empty]"));

		if (!SpiritSkill->Summary.IsNone())
		{
			SoulDescription->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->Summary));
// 			SkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkill->Description1);
// 			Desc.Emplace(SkillDesc);
		}
		
	}
}

void UUC_Soul_Column::InitHeroPortrait()
{
	SoulIcon->SetDummyHero(true);
}

void UUC_Soul_Column::InitSkillIcon()
{
	SoulSkill->InitSkillIcon();
}

void UUC_Soul_Column::InitText()
{
	SoulDescription->SetText(FText::FromString(TEXT("")));
}
