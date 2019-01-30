// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UCW_SkillNotice.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"


#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"


#include "GlobalIntegrated.h"

#include "Table/SkillResourceTableInfo.h"
#include "UtilFunctionIntegrated.h"

void UUCW_SkillNotice::InitSkillData()
{
	SkillIDs.Empty();
	auto OwnerCharacter_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());

	if (IsValid(OwnerCharacter_BattleEntityComponent))
	{
		auto OwnerBattleInfo = OwnerCharacter_BattleEntityComponent->GetBattleInfo();
		if (OwnerCharacter_BattleEntityComponent == nullptr)
			return;
		
		if (OwnerBattleInfo->ActiveSkill01.IsNone() == false)
		{		
			SkillIDs.Add(OwnerBattleInfo->ActiveSkill01);
		}

		if (OwnerBattleInfo->ActiveSkill02.IsNone() == false)
		{
			SkillIDs.Add(OwnerBattleInfo->ActiveSkill02);
		}

		if (OwnerBattleInfo->ActiveSkill03.IsNone() == false)
		{
			SkillIDs.Add(OwnerBattleInfo->ActiveSkill03);
		}

		if (OwnerBattleInfo->PassiveSkill01.IsNone() == false)
		{
			SkillIDs.Add(OwnerBattleInfo->PassiveSkill01);
		}

		if (OwnerBattleInfo->PassiveSkill02.IsNone() == false)
		{
			SkillIDs.Add(OwnerBattleInfo->PassiveSkill02);
		}
		
	}

	auto RGameInstance = RGAMEINSTANCE(this);
	ELanguageType CurLanguageType = ELanguageType::VE_MAX;
	
	{
		CurLanguageType = RGameInstance->OptionManager->GetLanguageType();
	}


	SkillTexts.Empty();
	for (auto& Element : SkillIDs)
	{
		auto OwnerCharacter_BaseProperty = Cast<IEntityBaseProperty>(OwnerCharacter.Get());
		if (OwnerCharacter_BaseProperty != nullptr)
		{
			FSkillTableInfo_V2* SkillInfo = URenewal_BattleFunctionLibrary::GetSkillTableInfo_V2(OwnerCharacter_BaseProperty->GetCharacterCategory(), Element);
			if (SkillInfo == nullptr)
				continue;

			FText outText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillInfo->SkillName );
			SkillTexts.Add(outText);			
		}
	}
	
}

int32 UUCW_SkillNotice::GetSkillTextIndex(FName InSkillID)
{
	int32 Index = 0;
	
	for (auto& Element : SkillIDs)
	{
		//UE_LOG(LogBattle, Log, TEXT("Noti ViewSkillIdIn - %s"), *(InSkillID.ToString()));
		//UE_LOG(LogBattle, Log, TEXT("Noti ViewSkillIdOut - %s"), *(Element.ToString()));
		if (Element.Compare(InSkillID) == 0)
		{
			if (SkillTexts.IsValidIndex(Index))
			{
				break;
			}
		}
		++Index;
	}
	if (SkillIDs.Num() == Index)
	{
		//error
		Index = INVALID_INDEXNUM;
	}
	
	return Index;
}