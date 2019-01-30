// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"







#include "GameInfo/UserInfo.h"
#include "ProjectRGameMode.h"
#include "Network/RTSManager.h"


/*
Game mode
*/
void UGamePlayFunctionLibrary::PauseInGamePlayTimer(bool InbPause)
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (InbPause)
		BaseStageInfo->PauseTimer();
	else
		BaseStageInfo->UnPauseTimer();
}


// FEPISODE* UGamePlayFunctionLibrary::GetEpisode(EDungeonTypeEnum InEpisodeType, FName InEpisodeID )
// {
// 	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(GameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);
// 	checkf(GameInstance->CampaignManager != nullptr, TEXT("[%s]URGameInstance->CampaignManager is Empty"), __FUNCTION__);
// 	checkf(GameInstance->HeroRuneDungeonManager != nullptr, TEXT("[%s]URGameInstance->HeroRuneDungeonManager is Empty"), __FUNCTION__);
// 	checkf(GameInstance->WeekDungeonManager != nullptr, TEXT("[%s]URGameInstance->WeekDungeonManager is Empty"), __FUNCTION__);
// 
// 
// 	switch (InEpisodeType)
// 	{
// 		case EDungeonTypeEnum::VE_Normal:
// 		return GameInstance->CampaignManager->GetEPISODE(InEpisodeID);
// 		case EDungeonTypeEnum::VE_HeroCard:
// 		return GameInstance->HeroRuneDungeonManager->GetEPISODE(InEpisodeID);
// 		case EDungeonTypeEnum::VE_WeekDungeon:
// 		return GameInstance->WeekDungeonManager->GetEPISODE(InEpisodeID);
// 	}
// 
// 	return nullptr;
// }
//
// void UGamePlayFunctionLibrary::UpdateEpisode(EDungeonTypeEnum InEpisodeType, const FEPISODE& EpisodeInfo)
// {
// 	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(GameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);
// 	checkf(GameInstance->CampaignManager != nullptr, TEXT("[%s]URGameInstance->CampaignManager is Empty"), __FUNCTION__);
// 	checkf(GameInstance->HeroRuneDungeonManager != nullptr, TEXT("[%s]URGameInstance->HeroRuneDungeonManager is Empty"), __FUNCTION__);
// 	checkf(GameInstance->WeekDungeonManager != nullptr, TEXT("[%s]URGameInstance->WeekDungeonManager is Empty"), __FUNCTION__);
// 
// 	switch (InEpisodeType)
// 	{
// 		case EDungeonTypeEnum::VE_Normal:
// 		return GameInstance->CampaignManager->UpdateEpisode(EpisodeInfo);
// 		case EDungeonTypeEnum::VE_HeroCard:
// 		return GameInstance->HeroRuneDungeonManager->UpdateEpisode(EpisodeInfo);
// 		case EDungeonTypeEnum::VE_WeekDungeon:
// 		return GameInstance->WeekDungeonManager->UpdateEpisode(EpisodeInfo);
// 	}
// }


/*
Stage
*/


ECharacterCrewStateEnum UGamePlayFunctionLibrary::IndexToCrewState(int32 index)
{
	switch (index)
	{
		case ROLE_LEADER_INDEX:
		case ROLE_CREW1_BATTLE_INDEX:
		case ROLE_CREW2_BATTLE_INDEX:
		return ECharacterCrewStateEnum::VE_Battle;
		break;
		case ROLE_CREW1_REST_INDEX:
		case ROLE_CREW2_REST_INDEX:
		return ECharacterCrewStateEnum::VE_Rest;
		break;
		default:
		return ECharacterCrewStateEnum::VE_None;
		break;
	}
}

ECharacterRoleEnum UGamePlayFunctionLibrary::IndexToRoleEnum(int32 index)
{
	switch (index)
	{
		case ROLE_LEADER_INDEX:
		return ECharacterRoleEnum::VE_Leader;
		break;
		case ROLE_CREW1_BATTLE_INDEX:
		case ROLE_CREW1_REST_INDEX:
		return ECharacterRoleEnum::VE_Crew1;
		break;
		case ROLE_CREW2_BATTLE_INDEX:
		case ROLE_CREW2_REST_INDEX:
		return ECharacterRoleEnum::VE_Crew2;
		break;
		default:
		return ECharacterRoleEnum::VE_None;
		break;
	}
}

bool UGamePlayFunctionLibrary::IsValidDeckHeroUDs(const TArray<FString>& heroUDs)
{
	int32 DeckEmptyCnt = 0;
	for (const FString& UD : heroUDs)
	{
		if (UD.IsEmpty() == true)
		{
			++DeckEmptyCnt;
		}
	}

	// Deck Empty
	if (DeckEmptyCnt == heroUDs.Num())
	{
		return false;
	}

	return true;
}

void UGamePlayFunctionLibrary::RequestGameStart(FPlay_Heroes_Info PlayHeroInfo)
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	bool bPlaySuccess = false;
	EGamePlayModeTypeEnum contentType = EGamePlayModeTypeEnum::VE_None;
	BaseStageInfo->bPlayAlone = false;

	switch (PlayHeroInfo.UsingType)
	{

	case EDeckUsingTypeEnum::VE_PlayAlone:
		BaseStageInfo->bPlayAlone = true;
		//skip break;
	case EDeckUsingTypeEnum::VE_Campaign:
		contentType = EGamePlayModeTypeEnum::VE_Campaign;
		bPlaySuccess = RGameInstance->CampaignManager->RequestGameStart(PlayHeroInfo);// ReqeustPlayModeCampStart(PlayHeroInfo);
		break;

	case EDeckUsingTypeEnum::VE_Raid:
		contentType = EGamePlayModeTypeEnum::VE_SingleRaid;
		URTSManager::GetInstancePtr()->REQ_DECK_SELECT_READY(RGameInstance->RealTimeModManager->RTSInfo.mSlotNo, true);
		break;
// 	case EDeckUsingTypeEnum::VE_Arena:
// 		contentType = EGamePlayModeTypeEnum::VE_Arena;
// 		break;
// 	case EDeckUsingTypeEnum::VE_WeekDungeon:
// 		contentType = EGamePlayModeTypeEnum::VE_WeekDungeon;
// 		bPlaySuccess = ReqeustPlayModeCampStart(PlayHeroInfo);
// 		break;
// 	case EDeckUsingTypeEnum::VE_HeroCard:
// 		contentType = EGamePlayModeTypeEnum::VE_Campaign;
// 		bPlaySuccess = ReqeustPlayModeCampStart(PlayHeroInfo);
// 		break;
// 	case EDeckUsingTypeEnum::VE_Crusader:
// 		contentType = EGamePlayModeTypeEnum::VE_MirrorDungeon;
// 		bPlaySuccess = PlayModeCrusaderStart(PlayHeroInfo);
// 		break;
// 	case EDeckUsingTypeEnum::VE_MirrorDungeon:
// 		contentType = EGamePlayModeTypeEnum::VE_MirrorDungeon;
// 		//bPlaySuccess = PlayModeMirrorDungeonStart(PlayHeroInfo);
// 		break;
	}

	RGameInstance->CurrentGameMode = contentType;

	if (PlayHeroInfo.UsingType == EDeckUsingTypeEnum::VE_HeroCard)
	{
		RGameInstance->CurrentGameMode = EGamePlayModeTypeEnum::VE_HeroEpisode;
	}

	if (bPlaySuccess)
	{
		RGameInstance->PlayModeManager->SetPlayHeroInfo(PlayHeroInfo);		
	}
}

void UGamePlayFunctionLibrary::RequestGameEnd(int32 StarGrade)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	EGamePlayModeTypeEnum GamePlayTypeEnum = UUtilFunctionLibrary::GetGamePlayModeEnum();
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (UGamePlayFunctionLibrary::IsCampaignMode(GamePlayTypeEnum))
	{
		RGameInstance->CampaignManager->RequestGameResult(StarGrade);
	}

}

bool UGamePlayFunctionLibrary::IsCampaignMode(EGamePlayModeTypeEnum InGameMode)
{
	return (InGameMode == EGamePlayModeTypeEnum::VE_Campaign);
}

