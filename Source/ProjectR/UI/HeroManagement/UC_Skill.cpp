// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Skill.h"

#include "GlobalIntegrated.h"



#include "UI/UI_Icon/SkillIcon.h"
#include "UtilFunctionIntegrated.h"
#include "Table/SkillResourceTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Network/PacketDataStructures.h"

#include "GameInfo/RInventory.h"


void UUC_Skill::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgets<USkillIcon>(this, SkillIcons);
	for (size_t i = 0; i < SkillIcons.Num(); ++i)
	{
		SkillIcons[i]->SetNotifyState(ENotifyStateFlagEnum(int(ENotifyStateFlagEnum::BM_UpgradeSkill1) + i));
	}

	Init();
}

void UUC_Skill::NativeDestruct()
{
	ClearHeroInfo();
	ClearSkillInfoContainers();

	Super::NativeDestruct();
}

/*
========================================================================================================================
Initialize
========================================================================================================================
*/
void UUC_Skill::Init()
{
	RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 	{
// 		return;
// 	}
	
	ClearHeroInfo();
	ClearSkillInfoContainers();
}

void UUC_Skill::ClearHeroInfo()
{
	CurrentHeroUD.Empty();
	CurrentHeroID = "";
}

void UUC_Skill::ClearSkillInfoContainers()
{
	HeroSkillInfos.Empty();
	SkillResourceInfos.Empty();
	SkillTableInfos.Empty();
	
	auto MaxSkillCount = static_cast<int32>(ESkillCategoryTypeEnum::VE_MaxSkillCount);

	HeroSkillInfos.SetNum(MaxSkillCount);
	SkillResourceInfos.SetNum(MaxSkillCount);
	SkillTableInfos.SetNum(MaxSkillCount);
}


/*
========================================================================================================================
Invalidate Data By HeroUD
========================================================================================================================
*/
void UUC_Skill::InvalidateByHeroUD(const FString& InHeroUD)
{
	CurrentHeroUD = InHeroUD;
	
	auto HeroInvenData = RGameInstance->RInventory->GetHeroWithUD(CurrentHeroUD);
	if (HeroInvenData)
	{
		CurrentHeroID = HeroInvenData->heroId;
	}
	else
	{
		CurrentHeroID = "";
	}

	InvalidateSkillInfo();

	Refresh();
}

/*
========================================================================================================================
Invalidate Data By HeroID
========================================================================================================================
*/
void UUC_Skill::InvalidateByHeroID(const FName& InHeroID)
{
	CurrentHeroUD.Empty();
	CurrentHeroID = InHeroID;

	InvalidateSkillInfo();

	Refresh();
}

/*
========================================================================================================================
Invalidate Data SkillData
========================================================================================================================
*/

void UUC_Skill::InvalidateSkillInfo()
{
	if (CurrentHeroUD.IsEmpty() == true)
	{
		InvalidateSkilInfoByHeroID();
	}
	else
	{
		InvalidateSkilInfoByHeroUD();
	}
}

void UUC_Skill::InvalidateSkilInfoByHeroUD()
{
	ClearSkillInfoContainers();

	const auto HeroInvenData = RGameInstance->RInventory->GetHeroWithUD(CurrentHeroUD);
	if (HeroInvenData == nullptr)
	{
		return;
	}

	int32 index = 0;
	for (const auto& skillLevel : HeroInvenData->skills)
	{
		FHERO_SKILL OutHeroSkill;
		UCharacterHelpFunctionLibrary::MakeHeroSkillStructure(HeroInvenData->heroId, skillLevel, index, OutHeroSkill);
		InvalidateSkillInfoContainers(OutHeroSkill);
		++index;
	}
}

void UUC_Skill::InvalidateSkillInfoContainers(const FHERO_SKILL& HeroSkillElem)
{
	InvaldateHeroSkillInfo(HeroSkillElem);
	InvalidateCurrentAndNextSkillInfo(HeroSkillElem);
	InvalidateCurrentSkillResourceInfo(HeroSkillElem);
}

void UUC_Skill::InvaldateHeroSkillInfo(const FHERO_SKILL& HeroSkillElem)
{
	auto		SkillSlotNumber = HeroSkillElem.slotNo;
	if (HeroSkillInfos.IsValidIndex(SkillSlotNumber) == false)
	{
		return;
	}

	HeroSkillInfos[SkillSlotNumber] = HeroSkillElem;
}

void UUC_Skill::InvalidateCurrentAndNextSkillInfo(const FHERO_SKILL& HeroSkillElem)
{
	auto		SkillSlotNumber		= HeroSkillElem.slotNo;
	auto		SkillCategoryType	= static_cast<ESkillCategoryTypeEnum>(SkillSlotNumber);
	const auto	CharacterTableData	= UTableManager::GetInstancePtr()->GetCharacterRow(CurrentHeroID);

	if (SkillTableInfos.IsValidIndex(SkillSlotNumber) == false)
	{
		return;
	}


	//Skill Info Setting
	auto& SkillInfo			= SkillTableInfos[SkillSlotNumber];

	int32 SkillDescLevel = HeroSkillElem.level;
	if (HeroSkillElem.level == 0)
	{
		SkillDescLevel = 1;
	}

	switch (SkillCategoryType)
	{
	case ESkillCategoryTypeEnum::VE_ActiveSkill01:
		if (!CharacterTableData->ActiveSkill01.IsNone())
		{
			URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(SkillInfo, ECharacterCategoryTypeEnum::VE_Hero, CharacterTableData->ActiveSkill01, SkillDescLevel);
		}
		break;

	case ESkillCategoryTypeEnum::VE_ActiveSkill02:
		if (!CharacterTableData->ActiveSkill02.IsNone())
		{
			URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(SkillInfo, ECharacterCategoryTypeEnum::VE_Hero, CharacterTableData->ActiveSkill02, SkillDescLevel);
		}
		break;

	case ESkillCategoryTypeEnum::VE_ActiveSkill03:
		if (!CharacterTableData->ActiveSkill03.IsNone())
		{
			URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(SkillInfo, ECharacterCategoryTypeEnum::VE_Hero, CharacterTableData->ActiveSkill03, SkillDescLevel);
		}
		break;

	case ESkillCategoryTypeEnum::VE_PassiveSkill01:
		if (!CharacterTableData->PassiveSkill01.IsNone())
		{
			URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(SkillInfo, ECharacterCategoryTypeEnum::VE_Hero, CharacterTableData->PassiveSkill01, SkillDescLevel);
		}
		break;

	case ESkillCategoryTypeEnum::VE_PassiveSkill02:
		if (!CharacterTableData->PassiveSkill02.IsNone())
		{
			URenewal_BattleFunctionLibrary::GetSkillTableWorkedInfo(SkillInfo, ECharacterCategoryTypeEnum::VE_Hero, CharacterTableData->PassiveSkill02, SkillDescLevel);
		}
		break;

	default:
		break;
	}
}

void UUC_Skill::InvalidateCurrentSkillResourceInfo(const FHERO_SKILL& HeroSkillElem)
{
	auto		SkillSlotNumber = HeroSkillElem.slotNo;

	//Skill Resource Setting
	if (SkillResourceInfos.IsValidIndex(SkillSlotNumber) == false)
	{
		return;
	}

	auto& SkillInfo		= SkillTableInfos[SkillSlotNumber];
	auto& SkillResource = SkillResourceInfos[SkillSlotNumber];

	if (SkillInfo.SkillTableInfo_V2)
	{
		auto SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo.SkillTableInfo_V2->SkillResourceID);
		if (SkillResourceRow)
			SkillResource = *SkillResourceRow;
	}
}

void UUC_Skill::InvalidateSkilInfoByHeroID()
{

}

/*
========================================================================================================================
Refresh
========================================================================================================================
*/
void UUC_Skill::Refresh()
{
	if ( IsValid(RGameInstance->NotifyManager))
	{
		RGameInstance->NotifyManager->CheckHeroSkillUpgradeEach(CurrentHeroUD);
	}

	int32 SkillIndex = 0;
	for (const auto& SkillIconElem : SkillIcons)
	{
		SkillIconElem->InvalidateData_SkillInfo(CurrentHeroUD, HeroSkillInfos[SkillIndex], SkillResourceInfos[SkillIndex], SkillTableInfos[SkillIndex]);
		++SkillIndex;
	}
}

/*
========================================================================================================================
Getter
========================================================================================================================
*/

bool UUC_Skill::GetSkillResourceInfoBySkillCategory(ESkillCategoryTypeEnum InSkillCategoryType, FSkillResourceTableInfo& InSkillResourceTableInfo)
{
	auto CurrentClickedSkillCategoryIndex = static_cast<int32>(InSkillCategoryType);

	if (SkillResourceInfos.IsValidIndex(CurrentClickedSkillCategoryIndex) == false)
	{
		return false;
	}

	InSkillResourceTableInfo = SkillResourceInfos[CurrentClickedSkillCategoryIndex];

	return true;
}

bool UUC_Skill::GetHeroSkillInfoBySkillCategory(ESkillCategoryTypeEnum InSkillCategoryType, FHERO_SKILL& InHeroSkillInfo)
{
	auto CurrentClickedSkillCategoryIndex = static_cast<int32>(InSkillCategoryType);

	if (HeroSkillInfos.IsValidIndex(CurrentClickedSkillCategoryIndex) == false)
	{
		return false;
	}

	InHeroSkillInfo = HeroSkillInfos[CurrentClickedSkillCategoryIndex];

	return true;
}