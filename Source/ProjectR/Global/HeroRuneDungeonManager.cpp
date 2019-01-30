// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "HeroRuneDungeonManager.h"
#include "GlobalIntegrated.h"

#include "GameInfo/UserInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "UtilFunctionIntegrated.h"

#define HERO_RUNE_EPISODEID_PREFIX	TEXT("H_")
#define HERO_DEFAULT_ROW			FName("1")
#define EPISODE_DEFAULT_ROW			FName("Campaign_BG_Hero_Agathon")


void UHeroRunekDungeonManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;
}

void UHeroRunekDungeonManager::ClearLastPlayInfos()
{
	SetLastPlayStageIndex(-1);
	SetInitState(true);
}

int32 UHeroRunekDungeonManager::GetRestRefreshTime()
{
	FDateTime nowTime = FDateTime::UtcNow();
	if ((nowTime - CurDateTime) >= FTimespan::FromSeconds(NewRuneRefreshRemainTime))
	{
		return 0;
	}
	else
	{
		FTimespan timeSpan = nowTime - CurDateTime;
		return NewRuneRefreshRemainTime - (int32)timeSpan.GetTotalSeconds();
	}
}

void UHeroRunekDungeonManager::UpdateRuneDungeonStatus(TArray< FEPISODE >& InEpisodes, TArray<FString>& InRotations, int32 InRuneRefreshRemainTime )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	NewRuneRefreshRemainTime = InRuneRefreshRemainTime;

	CurDateTime = FDateTime::UtcNow();

	HeroRuneDungoenEpisoides.Empty(InEpisodes.Num());
	for (auto& EpisodeInst : InEpisodes)
	{
		HeroRuneDungoenEpisoides.Emplace(EpisodeInst.episodeId, EpisodeInst );
	}

	EpisodeIDs.Empty();
	HeroRuneDungoenEpisoides.GetKeys(EpisodeIDs);

	HeroRuneRotations = InRotations;
}

void UHeroRunekDungeonManager::UpdateEpisode(const FEPISODE& EpisodeInfo)
{
// 	FEPISODE* pEpisode = FindEpisodebyID(EpisodeInfo.episodeId);
// 	*pEpisode = EpisodeInfo;
}

void UHeroRunekDungeonManager::UpdateStageGrade(FName InEpisodeID, FName InStageID, EPLAY_CLEAR_GRADE InClearGrade)
{	
// 	FEPISODE* Episode = GetEPISODE(InEpisodeID);
// 	if (Episode == nullptr)
// 		return;
// 
// 	FCampaignEpisodeTableInfo* EpisodeTableInfo = UTableManager::GetInstancePtr()->GetCampaignEpisodeHeroRow(InEpisodeID);
// 	if (EpisodeTableInfo)
// 	{
// 		int32 index = 0;
// 		for (auto& stageID : EpisodeTableInfo->StageKeys)
// 		{
// 			if (stageID == InStageID)
// 			{
// 				if( Episode->stages.IsValidIndex(index) && 
// 					Episode->stages[index] < (int32)InClearGrade )
// 				{
// 					Episode->stages[index] = (int32)InClearGrade;
// 				}
// 				break;
// 			}			
// 			++index;
// 		}
// 	}
}

FEPISODE* UHeroRunekDungeonManager::GetEPISODE(FName InEpisodeID)
{
	return FindEpisodebyID(InEpisodeID);
}

TArray< FEPISODE > UHeroRunekDungeonManager::GetHeroRuneEpisodes()
{
	return HeroEpisodes;
}

bool UHeroRunekDungeonManager::ExistTargetStage(int32 StageIndex)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	auto EpisodeInfo = GetEPISODE(UGamePlayBluePrintFunction::GetBaseStageInfo()->EpisodeID);
// 	if (EpisodeInfo == nullptr)
// 	{
// 		return false;
// 	}
// 
// 	if (EpisodeInfo->stages.IsValidIndex(StageIndex) == false)
// 	{
// 		return false;
// 	}
// 
// 	if (EpisodeInfo->stages[StageIndex] <= 0)
// 	{
// 		return false;
// 	}

	return true;
}

FText UHeroRunekDungeonManager::GetHeroDisplayText()
{
	FText ResultText;
// 	if(SelectHeroID.ToString().Len() > 0)
// 	{
// 		auto HeroRuneLeftEpisode = FName(*FText::Format(FText::FromString("H_{0}"), FText::FromString(SelectHeroID.ToString())).ToString());
// 		FCampaignEpisodeTableInfo* EpisodeLeftTableInfo = UTableManager::GetInstancePtr()->GetCampaignEpisodeHeroRow(HeroRuneLeftEpisode);
// 		if (EpisodeLeftTableInfo)
// 		{
// 			FCharacterTableInfo* CharacterLeftTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(EpisodeLeftTableInfo->RuneHeroID);
// 			if (CharacterLeftTableInfo)
// 			{
// 				ResultText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterLeftTableInfo->DisplayName);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		FCharacterTableInfo* CharacterLeftTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HERO_DEFAULT_ROW);
// 		if (CharacterLeftTableInfo)
// 		{
// 			ResultText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterLeftTableInfo->DisplayName);
// 		}
// 	}

	return ResultText;
}


FName UHeroRunekDungeonManager::GetHeroTextureKey()
{
//	TArray< FEPISODE > EpisodeList = GetHeroRuneEpisodes();
	FName ResultText;
// 	if (EpisodeList.IsValidIndex(0))
// 	{
// 		auto HeroRuneLeftEpisode = EpisodeList[0].episodeId;
// 		FCampaignEpisodeTableInfo* EpisodeLeftTableInfo = UTableManager::GetInstancePtr()->GetCampaignEpisodeHeroRow(HeroRuneLeftEpisode);
// 		if (EpisodeLeftTableInfo)
// 		{
// 			ResultText = EpisodeLeftTableInfo->EpisodeKey_Texture;
// 		}
// 	}
// 	else
// 	{
// 		return EPISODE_DEFAULT_ROW;
// 		/*//auto row = UTableManager::GetInstancePtr()->GetCampaignEpisodeRowNames();
// 		FCampaignEpisodeTableInfo* EpisodeLeftTableInfo = UTableManager::GetInstancePtr()->GetCampaignEpisodeHeroRow(EPISODE_DEFAULT_ROW);
// 		if (EpisodeLeftTableInfo)
// 		{
// 			//ResultText = EpisodeLeftTableInfo->EpisodeKey_Texture;
// 			return FName("Campaign_Episode_Image1");
// 		}*/
// 	}

	return ResultText;
}

void UHeroRunekDungeonManager::SetHeroEpisodesInfo(TArray<FEPISODE> recent, TArray<FEPISODE> episodes)
{
	if (recent.IsValidIndex(0))
		RecentlyEpisode = recent[0];
	
	bRecentRecordExist = (0 < recent.Num());

	HeroEpisodes = episodes;
}

bool UHeroRunekDungeonManager::FindEpisodebyID(FName InEpisodeID, FEPISODE& outEpisode)
{
	for (FEPISODE& episode : HeroEpisodes)
	{
		if (episode.episodeId == InEpisodeID)
		{
			outEpisode = episode;
			return true;
		}
	}

	return false;
}
FEPISODE* UHeroRunekDungeonManager::FindEpisodebyID(FName InEpisodeID)
{
// 	for (FEPISODE& episode : HeroEpisodes)
// 	{
// 		if (episode.episodeId == InEpisodeID)
// 		{
// 			return &episode;
// 		}
// 	}

	return nullptr;
}

void UHeroRunekDungeonManager::SetHeroRuneDungeonRetry()
{
	bInitState = false;
	bHeroRunekDungeonRetry = true;
}