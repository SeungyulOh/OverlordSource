// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_PVP_ReadyToStart.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/UserInfo.h"

#include "GlobalIntegrated.h"

#include "UI/Common/HeroBaseScrollItem.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "Network/RTSManager.h"

#include "UtilFunctionIntegrated.h"

void UUP_PVP_ReadyToStart::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);
	if (false == RGameInstance.IsValid())
		return;

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (IsValid(BaseStageInfo))
		CheckGameMode = BaseStageInfo->GetGameMode();

	RGameInstance->EventManager->OnUI_ReadyToStart.RemoveDynamic(this, &UUP_PVP_ReadyToStart::OnReadyToStartEvent);
	RGameInstance->EventManager->OnUI_ReadyToStart.AddDynamic(this, &UUP_PVP_ReadyToStart::OnReadyToStartEvent);

	Start_VersusAnim = GetWidgetAnimFromName(TEXT("Start_Versus"));
	if (IsValid(Start_VersusAnim))
	{
		Start_VersusAnim->OnAnimationFinished.RemoveDynamic(this, &UUP_PVP_ReadyToStart::EndAnimationAction);
		Start_VersusAnim->OnAnimationFinished.AddDynamic(this, &UUP_PVP_ReadyToStart::EndAnimationAction);
	}
}

void UUP_PVP_ReadyToStart::NativeDestruct()
{
	RGameInstance = RGAMEINSTANCE(this);
	if (false == RGameInstance.IsValid())
		return;

	RGameInstance->EventManager->OnUI_ReadyToStart.RemoveDynamic(this, &UUP_PVP_ReadyToStart::OnReadyToStartEvent);

	Super::NativeDestruct();
}

void UUP_PVP_ReadyToStart::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Duration > 0)
	{
		Duration -= InDeltaTime;
		if (Duration < 0)
		{
			Duration = 0.f;
			WidgetSwitcher_Count->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUP_PVP_ReadyToStart::PrepareWidget()
{
	SetUIPvPSummary(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_9"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_10"))), Cast<ULocalizingTextBlock>(GetWidgetFromName(TEXT("LocalizingTextBlock_10"))), true);
	SetUIPvPSummary(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_11"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_12"))), Cast<ULocalizingTextBlock>(GetWidgetFromName(TEXT("LocalizingTextBlock_12"))), false);

	TArray<UHeroBaseScrollItem*> HeroScrollArrayTemp;
	FindChildWidgets<UHeroBaseScrollItem>(this, HeroScrollArrayTemp);

	for (UHeroBaseScrollItem* item : HeroScrollArrayTemp)
		item->HeroIcon->HideIcon();

	if (RGameInstance->RealTimeModManager->IsRealTimeGameMode() && URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		RTSColosseumSet(HeroScrollArrayTemp);
	else
		ArenaAndMirrorSet(HeroScrollArrayTemp);
}

void UUP_PVP_ReadyToStart::RTSColosseumSet(TArray<UHeroBaseScrollItem*>& HeroScrollArrayTemp)
{
	TArray<FString> rBlueUDs;
	TArray<FString> rRedUDs;

	int32 BlueAttackPower = 0;
	int32 RedAttackPower = 0;
	for (auto Actor : RGameInstance->RealTimeModManager->ActorList)
	{
		if (Actor.Value.actorCategory != 1)
			continue;

		if (Actor.Value.team == 1)
		{
			rBlueUDs.Emplace(Actor.Value.heroUD);
			BlueAttackPower += Actor.Value.power;
		}
		else
		{
			rRedUDs.Emplace(Actor.Value.heroUD);
			RedAttackPower += Actor.Value.power;
		}
			
	}

	if (IsValid(Text_AttackPowerLeft))
	{
		Text_AttackPowerLeft->SetText(FText::AsNumber(BlueAttackPower));
	}
	if (IsValid(Text_AttackPowerRight))
	{
		Text_AttackPowerRight->SetText(FText::AsNumber(RedAttackPower));
	}


	int32 nMyHeroIndex = 0;
	for (int32 i = 0; i < rBlueUDs.Num(); ++i)
	{
		if (rBlueUDs.IsValidIndex(i) == false)
			continue;
		HeroScrollArrayTemp[nMyHeroIndex]->HeroIcon->SetHeroIconInfoWithUD(rBlueUDs[i], EHeroOwnershipType::VE_Colosseum);
		AllyHeroItemArray.Emplace(HeroScrollArrayTemp[nMyHeroIndex]);
		nMyHeroIndex++;
	}
	int32 nEnemyIndex = 5;
	for (int32 i = 0; i < rRedUDs.Num(); ++i)
	{
		if (rRedUDs.IsValidIndex(i) == false)
			continue;
		HeroScrollArrayTemp[nEnemyIndex]->HeroIcon->SetHeroIconInfoWithUD(rRedUDs[i], EHeroOwnershipType::VE_Colosseum);
		EnemyHeroItemArray.Emplace(HeroScrollArrayTemp[nEnemyIndex]);
		nEnemyIndex++;
	}
}

void UUP_PVP_ReadyToStart::ArenaAndMirrorSet(TArray<UHeroBaseScrollItem*>& HeroScrollArrayTemp)
{
	TArray<FString> rMyHeroUDs;
	TArray<FString> rEnemyHeroUDs;

	bool bArenaMode = CheckGameMode == EGamePlayModeTypeEnum::VE_Arena;

	rMyHeroUDs = bArenaMode ? RGameInstance->ArenaManager->m_ArenaMyHeroUDs : RGameInstance->MirrorDungeonManager->m_MirrorMyHeroUDs;
	rEnemyHeroUDs = bArenaMode ? RGameInstance->ArenaManager->m_MatchList.decks.defense : RGameInstance->MirrorDungeonManager->m_MatchList.decks.defense;

	int32 nMyHeroIndex = 0;
	for (int32 i = 0; i < rMyHeroUDs.Num(); ++i)
	{
		if (rMyHeroUDs[i].IsEmpty())
			continue;
		HeroScrollArrayTemp[nMyHeroIndex]->HeroIcon->SetHeroIconInfoWithUD(rMyHeroUDs[i], EHeroOwnershipType::VE_Me);
		AllyHeroItemArray.Emplace(HeroScrollArrayTemp[nMyHeroIndex]);
		nMyHeroIndex++;
	}

	int32 nEnemyIndex = 5;
	for (int32 i = 0; i < rEnemyHeroUDs.Num(); ++i)
	{
		if (rEnemyHeroUDs[i].IsEmpty())
			continue;
		HeroScrollArrayTemp[nEnemyIndex]->HeroIcon->SetHeroIconInfoWithUD(rEnemyHeroUDs[i], bArenaMode ? EHeroOwnershipType::VE_Arena : EHeroOwnershipType::VE_Mirror);
		EnemyHeroItemArray.Emplace(HeroScrollArrayTemp[nEnemyIndex]);
		nEnemyIndex++;
	}
}

void UUP_PVP_ReadyToStart::OnReadyToStartEvent()
{
	BP_ReadyToStartEvent();
}

void UUP_PVP_ReadyToStart::BP_ReadyToStartEvent()
{
	PrepareWidget();

	PlayAni("Start_Versus");
}

void UUP_PVP_ReadyToStart::EndAnimationAction()
{
	if(CheckGameMode == EGamePlayModeTypeEnum::VE_Arena)
	{
		// 	RGameInstance->EventManager->OnReadyToStartEvent.RemoveDynamic(this, &UUP_PVP_ReadyToStart::OnReadyToStartEvent);
		// 	RGameInstance->EventManager->OnReadyToStart.Broadcast();
		RGameInstance->ArenaManager->StartEnemyAI();
		RGameInstance->ArenaManager->StartHeroAI();
	}
	else if (CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
	{
		RGameInstance->MirrorDungeonManager->StartEnemyAI();
		RGameInstance->MirrorDungeonManager->StartHeroAI();
	}
	else if (CheckGameMode == EGamePlayModeTypeEnum::VE_Colosseum)
	{
		RemoveFromViewport();
	}
}

void UUP_PVP_ReadyToStart::PauseAnim()
{
	if (Start_VersusAnim && URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
	{
		PauseAnimation(Start_VersusAnim);
	}
}

void UUP_PVP_ReadyToStart::SetUIPvPSummary(UTextBlock* wdg_level, UTextBlock* wdg_nick, UTextBlock* wdg_power, bool my_summary)
{
 	if (false == IsValid(wdg_level) || false == IsValid(wdg_nick) || false == IsValid(wdg_power))
 		return;
 
 	int32 level = 0;
 	FString nick;
 	int32 power = 0;
 
 	if (CheckGameMode == EGamePlayModeTypeEnum::VE_Arena)
 	{
		UArenaManager* arena_mgr = RGameInstance.Get()->ArenaManager;
 		if (my_summary)
 		{
			UUserInfo* UserInfo = RGameInstance.Get()->UserInfo;
			FAVATAR& avatar = UserInfo->GetAvatarData();

 			level = avatar.level;
 			nick = avatar.nick;
 			power = arena_mgr->m_MyPower;
 		}
 		else
 		{
			FBATTLE_USER match_target;
 			arena_mgr->GetMatchTargetInfo(match_target);
 
 			level = match_target.level;
 			nick = match_target.nick;
 			power = arena_mgr->m_TargetPower;
 		}
 	}
	else if (CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
	{
		UMirrorDungeonManager* Mirror_mgr = RGameInstance.Get()->MirrorDungeonManager;
		if (my_summary)
		{
			UUserInfo* UserInfo = RGameInstance.Get()->UserInfo;
			FAVATAR& avatar = UserInfo->GetAvatarData();
			level = avatar.level;
			nick = avatar.nick;
			power = Mirror_mgr->m_MyPower;
		}
		else
		{
			UMirrorDungeonManager* MirrorManager = RGameInstance.Get()->MirrorDungeonManager;
			FBATTLE_USER match_target;
			MirrorManager->GetMatchTargetInfo(match_target);
			level = match_target.level;
			nick = match_target.nick;
			power = Mirror_mgr->m_TargetPower;
		}
	}
 	else if (RGameInstance->RealTimeModManager->IsRealTimeGameMode() && URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
 	{
		TMap<int16, FRTS_PLAYER>		RTSPlayerList = RGameInstance->RealTimeModManager->RTSPlayerList;

		for (auto Player : RTSPlayerList)
		{
			if (my_summary)
			{
				if (Player.Value.team == 1)
				{
					level = Player.Value.level;
					nick = Player.Value.nick;
					power = 0;
					break;
				}
			}
			else
			{
				if (Player.Value.team == 2)
				{
					level = Player.Value.level;
					nick = Player.Value.nick;
					power = 0;
					break;
				}
			}
		}

 	}
//  	else if (CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
//  	{
//  		UCrusaderManager* crusader_mgr = RGameInstance.Get()->CrusaderManager;
//  		if (IsValid(crusader_mgr))
//  		{
//  			power = 0;
//  			if (my_summary)
//  			{
//  				if (IsValid(RGameInstance->UserInfo))
//  				{
//  					level = RGameInstance->UserInfo->Avatar.level;
//  					nick = RGameInstance->UserInfo->Avatar.nick;
//  				}
//  				
//  				TArray<FString> deckUdList = RGameInstance->CrusaderManager->GetCurPlayDeck();
//  				for (FString ud : deckUdList)
//  				{
//  					power += UClient_BattleFunctionLibrary::GetHeroCombatStat(ud);
//  				}
//  			}
//  			else
//  			{
//  				level = RGameInstance->CrusaderManager->TargetAvatar.level;
//  				nick = RGameInstance->CrusaderManager->TargetAvatar.nick;
//  
//  				TArray<FEQUIPED_HERO> targetList = RGameInstance->CrusaderManager->TargetHeros;
//  				for (FEQUIPED_HERO hero : targetList)
//  				{
//  					power += UClient_BattleFunctionLibrary::GetEquipedHeroCombatStat(hero);
//  				}
//  			}
//  		}
//  	}
 
 	FText info_text;
 	info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(level));
 	wdg_level->SetText(info_text);
 
 	info_text = FText::FromString(nick);
 	wdg_nick->SetText(info_text);
 
 	info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(power));
 	wdg_power->SetText(info_text);
	
}

// void UUP_PVP_ReadyToStart::GetPvPHeroIconInfo(TArray<FHERO_ICON_INFO>& hero_icon_infos, bool my_heroes)
// {
// 	if (CheckGameMode == EGamePlayModeTypeEnum::VE_Arena)
// 	{
// 		UArenaManager* arena_mgr = RGameInstance.Get()->ArenaManager;
// 		if (IsValid(arena_mgr))
// 		{
// 			if (my_heroes)
// 			{
// 				arena_mgr->GetMyHeroesIconInfo(hero_icon_infos);
// 			}
// 			else
// 			{
// 				arena_mgr->GetMatchTargetHeroesIconInfo(hero_icon_infos);
// 			}
// 		}
// 	}
// 	else if (CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
// 	{
// 		UMirrorDungeonManager* MirrorManager = RGameInstance.Get()->MirrorDungeonManager;
// 		if (IsValid(MirrorManager))
// 		{
// 			if (my_heroes)
// 			{
// 				MirrorManager->GetMyHeroesIconInfo(hero_icon_infos);
// 			}
// 			else
// 			{
// 				MirrorManager->GetMatchTargetHeroesIconInfo(hero_icon_infos);
// 			}
// 		}
// 	}
// 	else if (CheckGameMode == EGamePlayModeTypeEnum::VE_Colosseum)
// 	{
// 		UColosseumManager* colosseum_mgr = RGameInstance.Get()->ColosseumManager;
// 		if (IsValid(colosseum_mgr))
// 		{
// 			FColosseumTeamInfo* ColosseumTeamInfo = colosseum_mgr->GetColosseumTeamInfoByMyTeamToggle(my_heroes);
// 			if (ColosseumTeamInfo)
// 			{
// 				for (FColosseumHero& ColosseumHero : ColosseumTeamInfo->ColosseumHeroList)
// 				{
// 					FHERO_ICON_INFO HeroIconInfo;
// 					UUIFunctionLibrary::GetHeroIconInfo(HeroIconInfo, ColosseumTeamInfo->UserKID, ColosseumHero.HeroUD);
// 					hero_icon_infos.Emplace(HeroIconInfo);
// 				}
// 			}
// 		}
// 	}
// 	else if (CheckGameMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
// 	{
// 		UCrusaderManager* crusader_mgr = RGameInstance.Get()->CrusaderManager;
// 		if (IsValid(crusader_mgr))
// 		{
// 			if (my_heroes)
// 			{
// 				TArray<FString> deckUdList = RGameInstance->CrusaderManager->GetCurPlayDeck();
// 				for (FString ud : deckUdList)
// 				{
// 					FHERO_ICON_INFO HeroIconInfo;
// 					UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, ud, EHeroOwnershipType::VE_Me);
// 					hero_icon_infos.Emplace(HeroIconInfo);
// 				}
// 			}
// 			else
// 			{
// 				TArray<FEQUIPED_HERO> targetList = RGameInstance->CrusaderManager->TargetHeros;
// 				for (FEQUIPED_HERO hero : targetList)
// 				{
// 					FHERO_ICON_INFO HeroIconInfo;
// 					UUIFunctionLibrary::GetHeroIconInfoByEquipedHero(HeroIconInfo, hero);
// 					hero_icon_infos.Emplace(HeroIconInfo);
// 				}
// 			}
// 		}
// 	}
// }
