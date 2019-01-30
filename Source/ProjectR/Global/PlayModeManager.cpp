// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "PlayModeManager.h"
#include "HUDBase.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/RewardInfo.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"
#include "GlobalIntegrated.h"
//
//#include "Global/BattleFieldManager.h"

#include "UtilFunctionIntegrated.h"

#include "Network/HttpRequestClient.h"
#include "Network/PacketDataEnum.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"
#include "ProjectRGameMode.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Table/CampaignTableInfo.h"
#include "UI/RWidgetManager.h"
#include "GameState/PlayState.h"



void UPlayModeManager::Initialize(/*URGameInstance* Instance*/)
{
//	RGameInstance = Instance;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->HttpClient)
	{
		RGameInstance->HttpClient->OnPlayModeRaidRpDelegate.Clear();
		RGameInstance->HttpClient->OnPlayModeRaidRpDelegate.AddDynamic(this, &UPlayModeManager::PlayModeRaidStartReply);
		RGameInstance->HttpClient->OnPlayModeRaidDoneRpDelegate.Clear();
		RGameInstance->HttpClient->OnPlayModeRaidDoneRpDelegate.AddDynamic(this, &UPlayModeManager::PlayModeRaidEndReply);
	}	
}



//////////////////////////////////////////////////////////////////////////
// Campaign


void UPlayModeManager::CampignModeStart()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//RGameInstance->DeckManager->SetDeckTypeForDeckUsingType(EDeckUsingTypeEnum::VE_Campaign, DeckInfo.pno);
	RGameInstance->PlayStateInfo->MakeHeroSlot(CurrentPlayHeroInfo.HeroUDs);
	

	if (RGameInstance->CurState == EGameStateEnum::VE_Play)
	{
		UPlayState* PlayState = Cast<UPlayState>(RGameInstance->CurrentState);
		if (PlayState)
			PlayState->OnReceivedPlayPacket.ExecuteIfBound();
	}
	else
	{
		RGameInstance->ChangeState(EGameStateEnum::VE_Play);
	}
}


void UPlayModeManager::CampaignModeEnd()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	auto gameMode = UGameplayStatics::GetGameMode(RGameInstance);
	if (IsValid(gameMode))
	{
		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGameInstance->GetWorld()));
		if (RGameMode)
			RGameMode->ResultUI();
	}

	TArray<FHeroSlotInfo> HeroSlots = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
	for (size_t i = 0; i < HeroSlots.Num(); ++i)
	{
		if (HeroSlots[i].SpawnedTargetActor)
		{
			UEntityAIComponent* AiComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(HeroSlots[i].SpawnedTargetActor);
			if (AiComp)
				AiComp->AIStop();

			UAnimStateComponent* AnimComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(HeroSlots[i].SpawnedTargetActor);
			if (AnimComp)
			{
				AnimComp->SetAnimComponentState(EAnimStateEnum::VE_Idle);
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////
// Raid
//bool UPlayModeManager::PlayModeRaidStart(const FPlay_Heroes_Info& PlayHeroInfo)
bool UPlayModeManager::PlayModeRaidStart()
{
 	auto			RGameInstance = RGAMEINSTANCE(this);
 	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (RGameInstance->CurState == EGameStateEnum::VE_Play)
	{
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Prepare);
	}
	else
	{
		RGameInstance->ChangeState(EGameStateEnum::VE_Play);
	}

	return true;
}

void UPlayModeManager::PlayModeRaidStartReply()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//RGameInstance->DeckManager->GetDeckInfoByDeckUsingType(EDeckUsingTypeEnum::VE_Campaign, DeckInfo);
	RGameInstance->PlayStateInfo->MakeHeroSlot(CurrentPlayHeroInfo.HeroUDs);
	if (RGameInstance->CurState == EGameStateEnum::VE_Play)
	{
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Prepare);
	}
	else
	{
		RGameInstance->ChangeState(EGameStateEnum::VE_Play);
	}
}

void UPlayModeManager::PlayModeRaidEnd()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
// 
// 	EPLAY_CLEAR_GRADE PlayClearGrade = URenewal_BattleFunctionLibrary::GetCampaignStageResultType(RGameInstance);
// 	if ( IsValid(RGameInstance->PlayStateInfo))
// 	{
// 		auto heroSlotList = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
// 
// 		TArray<FString>		deadList;
// 		for (FHeroSlotInfo& slot : heroSlotList)
// 		{
// 			if (slot.SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
// 			{
// 				deadList.Emplace(slot.HeroUD);
// 			}
// 		}
// 
// 		UPacketFunctionLibrary::PLAY_MODE_RAID_DONE_RQ(BaseStageInfo->PlayUD, (int32)PlayClearGrade, deadList);
// 	} 	
}

void UPlayModeManager::PlayModeRaidEndReply()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	AProjectRGameMode* GameMode = UUtilFunctionLibrary::GetGamePlayMode();
	if (IsValid(GameMode))
	{
		GameMode->ResultUI();
	}
}

//////////////////////////////////////////////////////////////////////////
// Arena

void UPlayModeManager::PlayModeArenaStart()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();


	RGameInstance->CurrentGameMode = EGamePlayModeTypeEnum::VE_Arena;
	BaseStageInfo->SetSelectedMapInfo(TEXT("5"));
	
	if (RGameInstance->CurState == EGameStateEnum::VE_Play)
	{
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Prepare);
	}
	else
	{
		RGameInstance->ChangeState(EGameStateEnum::VE_Play);
		RGameInstance->PlayStateInfo->MakeHeroSlot(RGameInstance->ArenaManager->m_ArenaMyHeroUDs);
	}
}

void UPlayModeManager::PlayModeMirrorDungeonStart()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();


	RGameInstance->CurrentGameMode = EGamePlayModeTypeEnum::VE_MirrorDungeon;
// 	int32 RandomValue = FMath::RandRange(0, 1);
	// 	if (0 == RandomValue)	
	// 		UGamePlayBluePrintFunction::GetBaseStageInfo()->SetSelectedMapInfo(TEXT("5"));
	// 	else					
	BaseStageInfo->SetSelectedMapInfo(TEXT("3002"));// DT_MAP keyvalue

	if (RGameInstance->CurState == EGameStateEnum::VE_Play)
	{
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Prepare);
	}
	else
	{
		RGameInstance->ChangeState(EGameStateEnum::VE_Play);
		RGameInstance->PlayStateInfo->MakeHeroSlot(RGameInstance->MirrorDungeonManager->m_MirrorMyHeroUDs);
	}
}

void UPlayModeManager::PlayModeMirrorDungeonEnd()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (IsValid(RGameInstance->RWidgetManager))
			RGameInstance->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_MirrorResult);

		if (IsValid(RGameInstance->MirrorDungeonManager))
			RGameInstance->MirrorDungeonManager->ClearMirrorDungeonInfo();
	}
}



void UPlayModeManager::PlayModeArenaEnd()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (IsValid(RGameInstance->RWidgetManager))
		{
			RGameInstance->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ArenaResult);
			RGameInstance->ArenaManager->ClearArenaInfo();
		}
	}
}

