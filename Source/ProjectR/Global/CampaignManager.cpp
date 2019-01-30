// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"

#include "GlobalIntegrated.h"

#include "ProjectRGameMode.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"

// #include "Utils/GamePlayFunctionLibrary.h"

#include "UtilFunctionIntegrated.h"
#include "Network/PacketFunctionLibrary.h"

#include "CampaignManager.h"

#include "UI/RWidgetManager.h"
#include "Network/PacketDataStructures.h"


UCampaignManager::UCampaignManager()
{
	//todo qsmname - It should be erased. Who did it? This code is wrong.
	TresureboxTexArray.Reset(8);
	UPaperSprite* TresureTex = nullptr;
	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox1_Sprite.Treasurebox1_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);
	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox1_Press_Sprite.Treasurebox1_Press_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);

	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox2_Sprite.Treasurebox2_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);
	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox2_Press_Sprite.Treasurebox2_Press_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);

	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox3_Sprite.Treasurebox3_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);
	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox3_Press_Sprite.Treasurebox3_Press_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);

	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox4_Sprite.Treasurebox4_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);
	TresureTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/Scenario/Treasurebox4_Press_Sprite.Treasurebox4_Press_Sprite'")));
	if (TresureTex)
		TresureboxTexArray.Emplace(TresureTex);

	



}

void UCampaignManager::Initialize()
{
	NextCampaignStageID = FName("");


	ClearSelectedCampaign();
	
	InvalidateData();
}

void UCampaignManager::InvalidateData()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	int32 CompletedStageSequence = BaseStageInfo->StageIndex;
	if( bIsInitState )
		CompletedStageSequence = GetCompletedStageLastIndex();

	InvalidateNextCampaignData(FName(TEXT("")), FName(TEXT("")), NextCampaignStageID, CompletedStageSequence);
		
	BaseStageInfo->FloorInfoId = FName(TEXT(""));
//	BaseStageInfo->EpisodeID = NextCampaignEpisodeID;
	BaseStageInfo->StageID = NextCampaignStageID;
	BaseStageInfo->StageIndex = CompletedStageSequence;

 } 

bool UCampaignManager::InvalidateNextCampaignData(FName ChapterID, FName EpisodeID, FName StageID, int32 StageSequence)
{
	ChapterID = FName(*(FText::AsNumber(1).ToString()));

	return false;	
}

int32 UCampaignManager::GetCompletedStageLastIndex()
{
	int32 index = 0;
	for (auto& stageClearGrade : StageStateInfo)
	{
		if (stageClearGrade == 0)
			break;
		++index;
	}

	return index;
}


void UCampaignManager::ClearSelectedCampaign()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	NextCampaignStageID = FName("");
	bIsInitState = true;

	BaseStageInfo->FloorInfoId = FName("1");
	BaseStageInfo->StageID = FName("0");
	BaseStageInfo->StageIndex = 0;
	
	InvalidateData();
}

//---------------------------------------------------------------------------------------------------
// complete episode reward
//---------------------------------------------------------------------------------------------------


bool UCampaignManager::RequestGameStart(FPlay_Heroes_Info& PlayHeroInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (BaseStageInfo->StageID.IsNone() || BaseStageInfo->MapTableKey.IsNone())
		return false;

	if (UGamePlayFunctionLibrary::IsValidDeckHeroUDs(PlayHeroInfo.HeroUDs) == false)
		return false;

	UPacketFunctionLibrary::TOWER_PLAY_START_RQ(BaseStageInfo->StageID.ToString(), PlayHeroInfo.HeroUDs);
	return true;
}

bool UCampaignManager::RequestGameResult(int32 StarGrade)
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	EPLAY_CLEAR_GRADE PlayClearGrade = EPLAY_CLEAR_GRADE::CLEAR_FAILED;
	switch (StarGrade)
	{
	case 1:
		PlayClearGrade = EPLAY_CLEAR_GRADE::CLEAR_STAR_ONE;
		break;
	case 2:
		PlayClearGrade = EPLAY_CLEAR_GRADE::CLEAR_STAR_TWO;
		break;
	case 3:
		PlayClearGrade = EPLAY_CLEAR_GRADE::CLEAR_STAR_THR;
		break;
	}
	//RGameInstance->RewardInfo->SetClearGrade(PlayClearGrade);
	UE_LOG(LogClass, Log, TEXT("Sending PlayDone Packet"));
	
	if(!BaseStageInfo->PlayUD.IsEmpty() && PrevPlayUD != BaseStageInfo->PlayUD)
		UPacketFunctionLibrary::TOWER_PLAY_DONE_RQ(BaseStageInfo->PlayUD, StarGrade, BaseStageInfo->GetElapsedGameTime());

	PrevPlayUD = BaseStageInfo->PlayUD;
	return true;
}

void UCampaignManager::CampaignResult(const TArray<FREWARD>& Reward)
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	m_ResultAvatar	=	GameInstance->UserInfo->GetAvatarData();
	m_ResultReward	=	Reward;

}

void UCampaignManager::SaveHeroRewardsForRewardPopup(const TArray<FREWARD>& InRewards)
{
	ClearHeroRewardsForRewardPopup();
	SavedHeroRewards = InRewards;
}

void UCampaignManager::ClearHeroRewardsForRewardPopup()
{
	SavedHeroRewards.Empty();
}

bool UCampaignManager::IsUnlockTowerBoss(int32 InBossFloor)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	//////////////////////////////////////////////////////////////////////////
	// ver.1
	//////////////////////////////////////////////////////////////////////////
	/*
	UTableManager* TableManager = UTableManager::GetInstancePtr();
	FName SingleKey = FName("S");
	TArray<FName> StageNames = TableManager->GetFloorStageRowNames();
	int32 nTopFloor = 0;
	for (FName Stage : StageNames)
	{
		FFloorStageDetail* StageDetail = TableManager->GetFloorStageDetailRow(Stage);
		if(!StageDetail)
			continue;

		if ((StageDetail->FloorInfoId - 1) == InBossFloor && StageDetail->PlayType == SingleKey)
		{
			FBossStage*	BossStageTable = TableManager->GetBossStage(Stage);
			if (BossStageTable)
			{
				if (StageDetail->OpenKey.IsNone())
					continue;

				int32 openKey = FCString::Atoi(*(StageDetail->OpenKey.ToString()));
				if (TowerStatus.clearGrades.IsValidIndex(openKey))
				{
					if (TowerStatus.clearGrades[openKey] > 0)
						return true;
					else
						return false;
				}
			}
		}
	}
	return false;
	//*/

	//////////////////////////////////////////////////////////////////////////
	// ver.2
	//////////////////////////////////////////////////////////////////////////
	//*
	int32 iCurrentStageIdx = (InBossFloor+1) * 14 - 1;
	TArray<FName> FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
	FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[iCurrentStageIdx]);

	TArray<int32> StageClearArray = TowerStatus.clearGrades;
	FString OpenKeyStr = FloorStageTable->OpenKey.ToString();
	if (StageClearArray.IsValidIndex(FCString::Atoi(*OpenKeyStr)) && StageClearArray[FCString::Atoi(*OpenKeyStr)] != 0)
		return true;
	else
		return false;
	//*/
}

void UCampaignManager::CalculateFloorStageIndex()
{
	int32 currNormalFloor = 1;
	int32 currHardFloor = 1;
	int32 currNormalStage = 0;
	int32 currHardStage = 0;
	bool NormalCalcEnd = false;
	bool HardCalcEnd = false;

	TArray<int32>& arr = TowerStatus.clearGrades;

	for (int32 i = 0; i < arr.Num(); i++)
	{
		if ((i % 12) < 6)
		{
			if (!NormalCalcEnd)
			{

				if (arr[i] > 0)
					currNormalStage++;
				else
				{
					NormalCalcEnd = true;
					continue;
				}

				if ((i % 6) == 5)
					currNormalFloor++;
			}
		}
		else
		{
			if (!HardCalcEnd)
			{
				if (arr[i] > 0)
					currHardStage++;
				else
				{
					HardCalcEnd = true;
					continue;
				}

				if ((i % 12) == 11)
					currHardFloor++;
			}
		}
	}

	SetNormalProgressIndex(currNormalStage % 6);
	SetHardProgressIndex(currHardStage % 6);
	SetCurrentNormalFloor(FMath::Clamp(currNormalFloor, 1, 25));
	SetCurrentHardFloor(FMath::Clamp(currHardFloor, 1, 25));
}

void UCampaignManager::SetTowerStatus(FTOWER_STATUS& InTowerStatus)
{
	if( TowerStatus.currentFloor == 0 )
		TowerStatus.currentFloor = InTowerStatus.currentFloor;
	
	TowerStatus.clearGrades = InTowerStatus.clearGrades;
	TowerStatus.floorMissionStatus = InTowerStatus.floorMissionStatus;
	TowerStatus.floorQuestStatus = InTowerStatus.floorQuestStatus;
	TowerStatus.treasureBoxes = InTowerStatus.treasureBoxes;
	
	CalculateFloorStageIndex();
}
