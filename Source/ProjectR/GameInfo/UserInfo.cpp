// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UserInfo.h"

#include "GlobalIntegrated.h"
#include "Utils/UIFunctionLibrary.h"
#include "UI/Common/UP_Popup_Avatar.h"
//





#include "GameInfo/RInventory.h"
#include "NotifyList/NotifyEventList.h"
#include "UI/RWidgetManager.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"


void UUserInfo::Initialize(/*URGameInstance* InRGameInstance*/)
{
	//Get Data from Server 
	StaminaMax = 0;
	AvatarExpMax = 0;
	StaminaPVPMax = 0;

	Avatar.nick = "";
	Avatar.exp = 0;
}

void UUserInfo::SetAvatarData(FAVATAR& avatar)
{
	bool bIsLevelUp = IsLevelUpAvatar(Avatar.level, avatar.level);
	//Init Userinfo
	if (Avatar.level == 0)
	{
		bIsLevelUp = false;
	}
	
	UCampaignManager* CampaignMgr = RGAMEINSTANCE(this)->CampaignManager;
	checkf(CampaignMgr, TEXT("CampaignMgr null"));

	/*if (avatar.elevatedFloor != 1 && avatar.elevatedFloor == Avatar.elevatedFloor + 1)
		CampaignMgr->isUpdatedFloor = true;*/


	Avatar = avatar;
	FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(Avatar.level)));
	if (tableInfo)
	{
		StaminaMax = tableInfo->StaminaMax;
		AvatarExpMax = tableInfo->AvatarMaxExp;
		StaminaPVPMax = tableInfo->StaminaPVPMax;
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	RGameInstance->EventManager->OnUpdateUserInfo.Broadcast();

	FDateTime::ParseIso8601( *(avatar.now), RGameInstance->ServerDateTime);
		
	RGameInstance->CurrentDateTime = RGameInstance->ServerDateTime.Now();
	RGameInstance->CurrentUTCDateTime = RGameInstance->ServerDateTime.UtcNow();
	RGameInstance->UTC_DiffTimespan = RGameInstance->CurrentDateTime - RGameInstance->CurrentUTCDateTime;
	RGameInstance->RInventory->StartRefillCheck();
	if (bIsLevelUp)
	{
		RGameInstance->CampaignManager->bLevelUp = true;
// 		if (RGameInstance->ContentsUnLockManager)
// 		{
// 			UUIFunctionLibrary::ShowAvatarLevelUpPopup(true);
// 		}
// 		if (IsValid(RGameInstance->NotifyEventList))
// 		{
// 			RGameInstance->NotifyEventList->AddNotifyEvent(ENotifyEventTypeEnum::VE_AvatarLevelUp);
// 		}
	}
	
	RGameInstance->MailManager->UpdateMailNotify();
}

bool UUserInfo::IsLevelUpAvatar(int32 BeforLevel, int32 AfterLevel)
{
	if (BeforLevel + 1 == AfterLevel)
		return true;

	return false;
}


void UUserInfo::SetAvatarProfile(TArray<FACCOUNT_RECORD>& InRecord, TArray<FHERO_USAGE_TAB>& InHeroUsageTab)
{
	AccountRecord = InRecord;
	int32 AllBattleFieldTotalCount = 0;
	int32 AllBattleFieldWinCount = 0;
	for (int32 i = 2; i < 5; ++i)
	{
		if (AccountRecord.IsValidIndex(i))
		{
			AllBattleFieldTotalCount += AccountRecord[i].totalCount;
			AllBattleFieldWinCount += AccountRecord[i].winCount;
		}
	}
	
	if (AccountRecord.IsValidIndex(2))
	{
		AccountRecord[2].totalCount = AllBattleFieldTotalCount;
		AccountRecord[2].winCount = AllBattleFieldWinCount;
	}

	HeroUsageTab = InHeroUsageTab;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	RGameInstance->EventManager->OnUpdateUserRecord.Broadcast();
}

void UUserInfo::SetAvatarIconList(TArray<int32>& InIconList)
{
	AvatarIconList = InIconList;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->EventManager != nullptr, TEXT("%s[ EventManager Is Empty ]"), __FUNCTION__);
	RGameInstance->EventManager->OnUpdateAvatarIcon.Broadcast();
}

void UUserInfo::CleanAvatarAccountInfo()
{
	AccountRecord.Empty();
	HeroUsageTab.Empty();
	AvatarIconList.Empty();
}


// void UUserInfo::UpdateCharacterStone(FName CharacterName, int32 Value)
// {
// 	if (CharacterStoneMap.Contains(CharacterName))
// 	{
// 		CharacterStoneMap[CharacterName] = Value;
// 	}
// }
// 
// int32 UUserInfo::GetCharacterStone(FName CharacterName)
// {
// 	if (CharacterStoneMap.Contains(CharacterName))
// 		return CharacterStoneMap[CharacterName];
// 
// 	return 0;
// }

bool UUserInfo::GetPlayHeroesUDsByDeckUsingType(EDeckUsingTypeEnum TargetType, TArray<FString>& InPlayHeroUDs)
{
// 	switch (TargetType)
// 	{
// 	case EDeckUsingTypeEnum::VE_Arena:
// 		break;
// 	case EDeckUsingTypeEnum::VE_Campaign:
// 		InPlayHeroUDs = Avatar.decks.campaign;
// 		return true;
// 	case EDeckUsingTypeEnum::VE_WeekDungeon:
// 		InPlayHeroUDs = Avatar.decks.challenge;
// 		return true;
// 	case EDeckUsingTypeEnum::VE_Raid:
// 		InPlayHeroUDs = Avatar.decks.raid;
// 		return true;
// 	case EDeckUsingTypeEnum::VE_Crusader:
// 		return true;
// 	default:
// 		return false;
// 		break;
// 	}

	return false;
}

bool UUserInfo::GetCurrentPlayHeroesUDsByDeckUsingType(TArray<FString>& InPlayHeroUDs)
{
	return GetPlayHeroesUDsByDeckUsingType(CurrentDeckUsingType, InPlayHeroUDs);
}

int32 UUserInfo::GetQuestState(int32 sequence)
{
	return -1;
}

bool UUserInfo::IsEnoughPVPStamina(FName TableKey)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FStaminaUseTableInfo* pTableInfo = UTableManager::GetInstancePtr()->GetStaminaUseTableRow(TableKey);
	return (pTableInfo != nullptr) ? (pTableInfo->StaminaPVP <= RGameInstance->RInventory->GetStaminaPVP()) : false;
}

void UUserInfo::UpdateHeroConfig(TArray<FHERO_CONFIG> InHeroConfigList)
{
	HeroCampaignConfigMap.Reset();

	for (auto& config : InHeroConfigList)
	{
		switch (config.contents)
		{
		case ECONTENTS_TYPE::CAMPAIGN:
			HeroCampaignConfigMap.Emplace(config.heroUD, config);
			break;
// 		case ECONTENTS_TYPE::HERO_EPISODE:
// 			HeroHeroEpisodeConfigMap.Emplace(config.heroUD, config);
// 			break;
		case ECONTENTS_TYPE::RAID:
			HeroRaidConfigMap.Emplace(config.heroUD, config);
			break;
		case ECONTENTS_TYPE::CRUSADER:
			HeroCrusaderConfigMap.Emplace(config.heroUD, config);
			break;
		case ECONTENTS_TYPE::WEEK_DUNGEON:
			HeroWeekDungeonConfigMap.Emplace(config.heroUD, config);
			break;
		case ECONTENTS_TYPE::COLOSSEUM:
			HeroColosseumConfigMap.Emplace(config.heroUD, config);
			break;
		}
	}
}

bool UUserInfo::GetHeroConfig(ECONTENTS_TYPE InContents, FString InHeroUD, FHERO_CONFIG& outHeroConfig)
{
	switch (InContents)
	{
	case ECONTENTS_TYPE::CAMPAIGN:
	{
		if (HeroCampaignConfigMap.Contains(InHeroUD))
		{
			outHeroConfig = HeroCampaignConfigMap[InHeroUD];
			return true;
		}
	}
	break;
// 	case ECONTENTS_TYPE::HERO_EPISODE:
// 	{
// 		if (HeroHeroEpisodeConfigMap.Contains(InHeroUD))
// 		{
// 			outHeroConfig = HeroHeroEpisodeConfigMap[InHeroUD];
// 			return true;
// 		}
// 	}
// 	break;
	case ECONTENTS_TYPE::RAID:
	{
		if (HeroRaidConfigMap.Contains(InHeroUD))
		{
			outHeroConfig = HeroRaidConfigMap[InHeroUD];
			return true;
		}
	}
	break;
	case ECONTENTS_TYPE::CRUSADER:
	{
		if (HeroCrusaderConfigMap.Contains(InHeroUD))
		{
			outHeroConfig = HeroCrusaderConfigMap[InHeroUD];
			return true;
		}
	}
	break;
	case ECONTENTS_TYPE::WEEK_DUNGEON:
	{
		if (HeroWeekDungeonConfigMap.Contains(InHeroUD))
		{
			outHeroConfig = HeroWeekDungeonConfigMap[InHeroUD];
			return true;
		}
	}
	break;
	case ECONTENTS_TYPE::COLOSSEUM:
	{
		if (HeroColosseumConfigMap.Contains(InHeroUD))
		{
			outHeroConfig = HeroColosseumConfigMap[InHeroUD];
			return true;
		}
	}
	break;
	}

	return false;
}

void UUserInfo::ApplyHeroConfig(EGamePlayModeTypeEnum ContentType, FString HeroUD, UBattleEntityComponent* BattleComp, USkillEntityComponent* SkillComp)
{
	bool IsValidContentType = false;
	ECONTENTS_TYPE DataContent = ECONTENTS_TYPE::CAMPAIGN;
	switch (ContentType)
	{
	case EGamePlayModeTypeEnum::VE_Campaign:		DataContent = ECONTENTS_TYPE::CAMPAIGN;		IsValidContentType = true;	break;
	case EGamePlayModeTypeEnum::VE_Colosseum:		DataContent = ECONTENTS_TYPE::COLOSSEUM;	IsValidContentType = true;	break;
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:		DataContent = ECONTENTS_TYPE::CRUSADER;		IsValidContentType = true;	break;
	case EGamePlayModeTypeEnum::VE_RaidKillBoss:		DataContent = ECONTENTS_TYPE::RAID;			IsValidContentType = true;	break;
	case EGamePlayModeTypeEnum::VE_HeroEpisode:		DataContent = ECONTENTS_TYPE::HERO_EPISODE;	IsValidContentType = true;	break;
	case EGamePlayModeTypeEnum::VE_WeekDungeon:		DataContent = ECONTENTS_TYPE::WEEK_DUNGEON;	IsValidContentType = true;	break;
	}

	if(IsValidContentType)
	{
		FHERO_CONFIG HeroConfig;
		if (GetHeroConfig(DataContent, HeroUD, HeroConfig))
		{
			// Hero AI PriorityType for target selection 
			EPriorityTypeEnum AIPriorityType = EPriorityTypeEnum::VE_None;
			EPREFER_TARGET_TYPE PreferType = HeroConfig.preferTarget;
			switch (PreferType)
			{
			case EPREFER_TARGET_TYPE::TANKER:		AIPriorityType = EPriorityTypeEnum::VE_Tanker;		break;
			case EPREFER_TARGET_TYPE::MELEE_DEALER:	AIPriorityType = EPriorityTypeEnum::VE_MeleeDealer;	break;
			case EPREFER_TARGET_TYPE::RANGE_DEALER:	AIPriorityType = EPriorityTypeEnum::VE_RangeDealer;	break;
			case EPREFER_TARGET_TYPE::MAGIC_DEALER:	AIPriorityType = EPriorityTypeEnum::VE_MagicDealer;	break;
			case EPREFER_TARGET_TYPE::SUPPORTER:	AIPriorityType = EPriorityTypeEnum::VE_Supporter;	break;
			}

			if (EPriorityTypeEnum::VE_None != AIPriorityType)
			{
				BattleComp->ChangeAIPriorityType(AIPriorityType);
			}

			// Hero AI automatically use skill On / Off
			SkillComp->SetSkillAutoModeOnOff(HeroConfig.skills);
		}
	}
}