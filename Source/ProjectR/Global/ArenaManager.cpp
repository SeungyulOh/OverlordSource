// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ArenaManager.h"
#include "ProjectRGameMode.h"

#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketDataEnum.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "UI/RWidgetManager.h"
#include "UI/InGameHUD/UP_InGameHUD_Arena.h"
#include "UI/Arena/UP_Arena_Lobby.h"

#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"

#include "GameState/Arena/ArenaHeroPosition.h"

#include "ContentGameMode/Modes/ArenaGameMode.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "UI/UI_Common/TopBar.h"


/////////////////////////////////////////////////////////////////////
 void UArenaManager::Initialize()
 {
 	ResetArenaManagerData();



 }

void UArenaManager::ResetArenaManagerData()
{
	m_MyHeroSpawnPosList.Init(FTransform::Identity, MAX_HERO_POS_COUNT);
	m_MatchHeroSpawnPosList.Init(FTransform::Identity, MAX_HERO_POS_COUNT);

	m_MyWin = false;

	m_nMyHeroesMaxHP = 0;
	m_nTargetHeroesMaxHP = 0;

	m_nMyHeroesCurrentHP = 0;
	m_nTargetHeroesCurrentHP = 0;

	m_MyPower = 0;
	m_TargetPower = 0;
}

 bool UArenaManager::GetMatchTargetInfo(FBATTLE_USER& target_info)
 {
 	target_info = m_MatchList;
 	return true;
 }
 
// void UArenaManager::GetMyHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 
// 	icon_infos.Init(FHERO_ICON_INFO(), MAX_HERO_PLACE_COUNT);
// 
// 	FHERO_ICON_INFO icon_info;
// 	int32 hero_count = m_ArenaMyHeroUDs.Num();
// 	int32 Count = 0;
// 	for (int32 n = 0; n < hero_count; ++n)
// 	{
// 		if(icon_infos.IsValidIndex(Count) == false)
// 			continue;
// 
// 		FHERO* Hero = RGameInstance->RInventory->GetHeroWithUD(m_ArenaMyHeroUDs[n]);
// 
// 		if (Hero == nullptr)
// 			continue;
// 
// 		auto CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(Hero->heroId);
// 
// 		icon_info.heroUD = Hero->heroUD;
// 		icon_info.heroId = Hero->heroId;
// 		icon_info.level = Hero->level;
// 		icon_info.grade = CharacterInfo->Grade;
// 		icon_info.PortraitTextureRef = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, Hero->heroId);
// 
// 		icon_infos[Count] = icon_info;
// 		Count++;
// 	}
// }

// void UArenaManager::GetMatchHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 
// 	icon_infos.Init(FHERO_ICON_INFO(), MAX_HERO_PLACE_COUNT);
// 
// 	FHERO_ICON_INFO icon_info;
// 	int32 hero_count = m_MatchList.decks.defense.Num();
// 	int32 Count = 0;
// 	FEQUIPED_HERO	HeroInfo;
// 
// 	for (int32 n = 0; n < hero_count; ++n)
// 	{
// 		if (m_MatchList.decks.defense.IsValidIndex(n))
// 		{
// 			if(RGameInstance->OtherHeroManager->GetOtherHeroData(m_MatchList.kId, m_MatchList.decks.defense[n], HeroInfo) == false)
// 				continue;
// 
// 			auto CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInfo.heroId);
// 
// 			if (icon_infos.IsValidIndex(Count))
// 			{
// 				icon_info.heroUD = HeroInfo.heroUD;
// 				icon_info.heroId = HeroInfo.heroId;
// 				icon_info.level = HeroInfo.level;
// 				icon_info.grade = CharacterInfo->Grade;
// 				icon_info.PortraitTextureRef = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, HeroInfo.heroId);
// 
// 				icon_infos[Count] = icon_info;
// 				Count++;
// 			}
// 		}
// 	}
// }

// void UArenaManager::GetMatchTargetHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos)
// {
// 	icon_infos.Empty();
// 
// 	GetMatchHeroesIconInfo(icon_infos);
// }

void UArenaManager::SetRankerList(TArray<FARENA_RANK_USER>& user_list)
{
	int32 list_count = user_list.Num();
	if (0 < list_count)
	{
		m_RankList.Reset(list_count);

		FARENA_RANK_USER ranker;
		for (int32 n = 0; n < list_count; ++n)
		{
			ranker = user_list[n];
			m_RankList.Emplace(ranker);

			UE_LOG(LogTemp, Warning, TEXT("SetRankerList Rank(%d) Nick(%s)"), ranker.rank, *(ranker.nick));
		}
	}
}

void UArenaManager::ArenaLobbyUpdate()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	URWidgetManager* RWidgetManager = GameInstance->RWidgetManager;
	checkf(RWidgetManager != nullptr, TEXT("[%s]WidgetManager is Empty"), __FUNCTION__);

	UUserWidget* Widget = RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ArenaLobby);

	if (IsValid(Widget))
	{
		UUP_Arena_Lobby* UP_ArenaLobby = Cast<UUP_Arena_Lobby>(Widget);
		UP_ArenaLobby->Init();
	}
}

void UArenaManager::ArenaDeckSeetingUpdate()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	URWidgetManager* RWidgetManager = GameInstance->RWidgetManager;
	checkf(RWidgetManager != nullptr, TEXT("[%s]WidgetManager is Empty"), __FUNCTION__);

// 	UUserWidget* Widget = RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ArenaDeckSetting);
// 
// 	if (IsValid(Widget))
// 	{
// 		UUP_Arena_DeckSetting* UP_ArenaDeckSetting = Cast<UUP_Arena_DeckSetting>(Widget);
// 		UP_ArenaDeckSetting->Reset();
// 	}
	SetDefenceSetting(false);
	RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ArenaDeckSetting);
}

void UArenaManager::SetCameraModeAfterGameStart(EArenaCameraMode cameraMode)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	UUP_InGameHUD_Arena* in_game_hud = Cast<UUP_InGameHUD_Arena>(UUIFunctionLibrary::GetInGameHUD());
	if (in_game_hud)
	{
		int32 Index = (int32)cameraMode;
		in_game_hud->SetToggleCameraIcon(Index);
		in_game_hud->SetArenaCamera();
	}
}

void UArenaManager::RequestPlayStart()
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->PlayModeManager != nullptr, TEXT("RGameInstance->PlayModeManager is Empty"));

	if (RGameInstance->HttpClient->IsValidNetwork())
	{
		FBATTLE_USER& match_target = m_MatchList;
		//FTimerDelegate TimerCallback;
		if (!m_bIsAlreadyPlayArena)
		{
			UPacketFunctionLibrary::ARENA_PLAY_RQ(match_target.kId, m_ArenaMyHeroUDs, m_MyPower, m_TargetPower);
			m_bIsAlreadyPlayArena = true;
		}
	}
}

void UArenaManager::RequestPlayDone()
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 winner = 0;
	if (m_MyWin)
	{
		winner = RGameInstance->UserInfo->kId;
	}
	else
	{
		winner = m_MatchList.kId;
	}

	UPacketFunctionLibrary::ARENA_PLAY_DONE_RQ(m_ArenaPlayUD, m_MyWin);
}

void UArenaManager::RequestRankigList()
{	
 	UPacketFunctionLibrary::ARENA_RANKING_LIST_RQ();
}

void UArenaManager::RequestStatus()
{	
	UPacketFunctionLibrary::ARENA_STATUS_RQ();
}

void UArenaManager::RequestTargetInfo()
{
	UPacketFunctionLibrary::ARENA_TARGET_INFO_RQ(false);
}

void UArenaManager::RequestSeasonDone()
{
	UPacketFunctionLibrary::ARENA_SEASON_DONE_RQ();
}

void UArenaManager::RequestDefenceReward()
{
	UPacketFunctionLibrary::ARENA_DEFENSE_REWARD_RQ();
}

void UArenaManager::RequestDefenceReset()
{
	UPacketFunctionLibrary::ARENA_TARGET_INFO_RQ(true);
}

void UArenaManager::SetArenaHeroPosition(bool my_hero, int32 pos_index, FVector pos, FRotator rot)
{
	if (0 < pos_index && pos_index <= MAX_HERO_POS_COUNT)
	{
		FTransform spawn_pos;
		spawn_pos.SetRotation(FQuat(rot));
		spawn_pos.SetLocation(pos);
		int32 iIndex = pos_index - 1;
		if (m_MatchHeroSpawnPosList.IsValidIndex(iIndex) == false)
			return;

		if (my_hero)
		{
			m_MyHeroSpawnPosList[iIndex] = spawn_pos;
		}
		else
		{
			m_MatchHeroSpawnPosList[iIndex] = spawn_pos;
		}
	}
}

void UArenaManager::SetArenaHeroPosition()
{
	URGameInstance* RGameInstance	=	RGAMEINSTANCE(this);
	UWorld*			GameWorld		=	GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GameWorld, AArenaHeroPosition::StaticClass(), OutActors);

	if (OutActors.GetAllocatedSize() <= 0)
		return;

	for (int32 i = 0; i < OutActors.Num(); i++)
	{
		AArenaHeroPosition* ArenaHeroPosition =  Cast<AArenaHeroPosition>(OutActors[i]);
		if(ArenaHeroPosition)
			ArenaHeroPosition->SaveHeroPosition();
	}
}



bool UArenaManager::GetMyHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos)
{
	if (0 <= hero_deck_index && hero_deck_index < MAX_HERO_POS_COUNT)
	{
		if (m_MyHeroSpawnPosList.IsValidIndex(hero_deck_index))
		{
			spawn_pos = m_MyHeroSpawnPosList[hero_deck_index];
			return true;
		}
	}
	return false;
}

bool UArenaManager::GetMatchHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos)
{
	if (0 <= hero_deck_index && hero_deck_index < MAX_HERO_POS_COUNT)
	{
		if (m_MatchHeroSpawnPosList.IsValidIndex(hero_deck_index))
		{
			spawn_pos = m_MatchHeroSpawnPosList[hero_deck_index];
			return true;
		}
	}
	return false;
}



// //////////////////////////////////////////////////////////////////////////


void UArenaManager::AttachBattleRecordManager()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->BattleRecordManager != nullptr, TEXT("RGameInstance->BattleRecordManager is Empty"));

	RGameInstance->BattleRecordManager->OnBattleRecord_UpdateHP.RemoveDynamic(this, &UArenaManager::OnBattleRecord_UpdateHP);
	RGameInstance->BattleRecordManager->OnBattleRecord_UpdateHP.AddDynamic(this, &UArenaManager::OnBattleRecord_UpdateHP);
}

void UArenaManager::OnBattleRecord_UpdateHP()
{
	UpdateArenaGuage();
}

//////////////////////////////////////////////////////////////////////////

void UArenaManager::InitArenaGuage()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->BattleRecordManager != nullptr, TEXT("RGameInstance->BattleRecordManager is Empty"));

	m_nMyHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_MY);
	m_nTargetHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_ENEMY);

	m_nMyHeroesMaxHP = m_nMyHeroesCurrentHP;
	m_nTargetHeroesMaxHP = m_nTargetHeroesCurrentHP;
}

void UArenaManager::UpdateArenaGuage()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->BattleRecordManager != nullptr, TEXT("RGameInstance->BattleRecordManager is Empty"));
	
	m_nMyHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_MY);
	m_nTargetHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_ENEMY);
	
	float my_percent = (float)(m_nMyHeroesCurrentHP) / (float)(m_nMyHeroesMaxHP);
	float target_percent = (float)(m_nTargetHeroesCurrentHP) / (float)(m_nTargetHeroesMaxHP);

	UUP_InGameHUD_Arena* in_game_hud = Cast<UUP_InGameHUD_Arena>(UUIFunctionLibrary::GetInGameHUD());
	if (in_game_hud)
	{
		in_game_hud->UpdateMyTotalHP(m_nMyHeroesCurrentHP, m_nMyHeroesMaxHP, my_percent);
		in_game_hud->UpdateEnemyTotalHP(m_nTargetHeroesCurrentHP, m_nTargetHeroesMaxHP, target_percent);
	}
}

void UArenaManager::DecideWinner()
{
	if (m_nMyHeroesCurrentHP < m_nTargetHeroesCurrentHP)
		m_MyWin = false;
	else
		m_MyWin = true;
}

void UArenaManager::ResetCameraMode()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UUP_InGameHUD_Arena* in_game_hud = Cast<UUP_InGameHUD_Arena>(UUIFunctionLibrary::GetInGameHUD());
	if (in_game_hud)
	{
		in_game_hud->ResetCameraMode();
	}
}

void UArenaManager::ResetCamera()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ABasePlayerController* controller = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	if (controller)
	{
		ABaseCameraActor* camera = controller->GetCameraActor();
		if (camera)
		{
			//if (false == camera->IsDynamicCamera)
			{
				float dist = camera->GetCamLongDist();
				camera->ZoomInAndOut(dist, false);
				camera->SetDynamicCamera(true);
				camera->ChangeCameraOriginDistance(true, false);
			}
		}
	}
}

bool UArenaManager::IsCombatEnd()
{
	bool result = (GetAliveMyHero() == 0);
	if (result)
		return result;

	result = (GetAliveEnemy() == 0);
	if (result)
		return result;
	
	return result;
}

int32 UArenaManager::GetAliveMyHero()
{
	int32 aliveMyHero = 0;
	for (int32 index = 0; index < mMyHeroList.Num(); ++index)
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mMyHeroList[index]);
		if (IsValid(BattleEntityComponent)) 
		{
			if (!BattleEntityComponent->IsDead())
				aliveMyHero++;
		}
	}
	return aliveMyHero;
}

int32 UArenaManager::GetAliveEnemy()
{
	int32 aliveEnemyHero = 0;
	for (int32 index = 0; index < mEnemyList.Num(); ++index)
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mEnemyList[index]);
		if (IsValid(BattleEntityComponent)) 
		{
			if (!BattleEntityComponent->IsDead())
				aliveEnemyHero++;
		}
	}
	return aliveEnemyHero;
}

void UArenaManager::StartEnemyAI()
{
	for (int32 index = 0; index < mEnemyList.Num(); ++index) 
	{
		AActor* actor = mEnemyList[index];
		IEntityBaseProperty* pEntity = Cast<IEntityBaseProperty>(actor);
		if (pEntity != nullptr)
		{
			pEntity->SetSpawnEventForArena();
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(actor);
			if (IsValid(BattleEntityComponent)) {
				if (!BattleEntityComponent->IsDead())
				{
					UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(actor);
					if (IsValid(EntityAIComponent)) {
						EntityAIComponent->AIRestart();
					}
				}
			}
		}	
	}
}

void UArenaManager::StartHeroAI()
{
	for (int32 index = 0; index < mMyHeroList.Num(); ++index) 
	{
		AActor* actor = mMyHeroList[index];
		IEntityBaseProperty* pEntity = Cast<IEntityBaseProperty>(actor);
		if (pEntity != nullptr)
		{
			pEntity->SetSpawnEventForArena();
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(actor);
			if (IsValid(BattleEntityComponent))
			{
				if (!BattleEntityComponent->IsDead())
				{
					UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(actor);
					if (IsValid(EntityAIComponent)) 
					{
						EntityAIComponent->AIRestart();
					}
				}
			}
		}
	}
}

void UArenaManager::StopEnemyAI()
{
	for (auto Element : mEnemyList)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Element);
		if (IsValid(SkillEntityComponent)) 
		{
			SkillEntityComponent->EndSkillActionForced_V2();
			SkillEntityComponent->ForceClearSkillEffect();
		}

		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element);
		if (IsValid(BattleEntityComponent)) 
		{
			if (!BattleEntityComponent->IsDead())
			{
				BattleEntityComponent->StopBattleEntityForced();
			}
		}

		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Element);
		if (IsValid(EntityAIComponent)) 
		{
			EntityAIComponent->AIStop();
		}

	}
}

void UArenaManager::StopHeroAI()
{
	for (auto Element : mMyHeroList)
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Element);
		if (IsValid(SkillEntityComponent)) 
		{
			SkillEntityComponent->EndSkillActionForced_V2();
			SkillEntityComponent->ForceClearSkillEffect();
		}

		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element);
		if (IsValid(BattleEntityComponent)) 
		{
			if (!BattleEntityComponent->IsDead())
			{
				BattleEntityComponent->StopBattleEntityForced();
			}
		}
		
		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Element);
		if (IsValid(EntityAIComponent)) 
		{
			EntityAIComponent->AIStop();
		}

	}
}

void UArenaManager::ClearArenaInfo()
{
	mMyHeroList.Empty();
	mEnemyList.Empty();
}

void UArenaManager::ArenaDoneRP()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	AArenaRGameMode* ArenaMode = Cast<AArenaRGameMode>(UGameplayStatics::GetGameMode(RGameInstance->GetWorld()));
	if (ArenaMode)
		ArenaMode->OnPlayModeArenaDoneRpDelegate();
}

