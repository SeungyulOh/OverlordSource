

#include "ProjectR.h"
#include "ProjectRGameMode.h"


#include "GlobalIntegrated.h"


#include "UI/InGameHUD/UP_InGameHUD_Mirror.h"



#include "UtilFunctionIntegrated.h"


#include "GameInfo/RInventory.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"

#include "MirrorDungeonManager.h"

#include "ContentGameMode/Modes/MirrorGameMode.h"

void UMirrorDungeonManager::Initialize()
{
	ResetMirrorDungeonManagerData();
}

void UMirrorDungeonManager::ResetMirrorDungeonManagerData()
{
	m_MyHeroSpawnPosList.Init(FTransform::Identity, MAX_HERO_POS_COUNT);
	m_MatchHeroSpawnPosList.Init(FTransform::Identity, MAX_HERO_POS_COUNT);

	m_MyWin = false;

	m_nMyHeroesMaxHP = 0;
	m_nTargetHeroesMaxHP = 0;

	m_nMyHeroesCurrentHP = 0;
	m_nTargetHeroesCurrentHP = 0;
}

void UMirrorDungeonManager::SetMirrorStatus(FMIRROR_STATUS InMirrorStatus)
{
	MirrorStatus = InMirrorStatus;
	OnMirrorDungeonStatusRp.Broadcast();
}

int32 UMirrorDungeonManager::FindRemainHP(FString InHeroUD)
{
	for (FMIRROR_HERO_HP& hpinfo : MirrorStatus.heroHPs)
	{
		if (hpinfo.heroUD == InHeroUD)
			return hpinfo.hp;
	}

	return -1;
}

void UMirrorDungeonManager::StartEnemyAI()
{
	for (int32 index = 0; index < mEnemyList.Num(); ++index) {
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

void UMirrorDungeonManager::StartHeroAI()
{
	for (int32 index = 0; index < mMyHeroList.Num(); ++index) {
		AActor* actor = mMyHeroList[index];
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

void UMirrorDungeonManager::StopEnemyAI()
{
	for (int32 index = 0; index < mEnemyList.Num(); ++index) {

		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(mEnemyList[index]);
		if (IsValid(SkillEntityComponent)) {
			SkillEntityComponent->EndSkillActionForced_V2();
			SkillEntityComponent->ForceClearSkillEffect();
		}

		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mEnemyList[index]);
		if (IsValid(BattleEntityComponent)) {
			if (!BattleEntityComponent->IsDead())
			{
				BattleEntityComponent->StopBattleEntityForced();
			}
		}

		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(mEnemyList[index]);
		if (IsValid(EntityAIComponent)) {
			EntityAIComponent->AIStop();
		}

	}
}

void UMirrorDungeonManager::StopHeroAI()
{
	for (int32 index = 0; index < mMyHeroList.Num(); ++index) {
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(mMyHeroList[index]);
		if (IsValid(SkillEntityComponent)) {
			SkillEntityComponent->EndSkillActionForced_V2();
			SkillEntityComponent->ForceClearSkillEffect();
		}
		
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mMyHeroList[index]);
		if (IsValid(BattleEntityComponent)) {
			if (!BattleEntityComponent->IsDead())
			{
				BattleEntityComponent->StopBattleEntityForced();

			}
		}

		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(mMyHeroList[index]);
		if (IsValid(EntityAIComponent)) {
			EntityAIComponent->AIStop();
		}
	}
}

void UMirrorDungeonManager::AttachBattleRecordManager()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->BattleRecordManager != nullptr, TEXT("RGameInstance->BattleRecordManager is Empty"));

	RGameInstance->BattleRecordManager->OnBattleRecord_UpdateHP.RemoveDynamic(this, &UMirrorDungeonManager::OnBattleRecord_UpdateHP);
	RGameInstance->BattleRecordManager->OnBattleRecord_UpdateHP.AddDynamic(this, &UMirrorDungeonManager::OnBattleRecord_UpdateHP);
}

void UMirrorDungeonManager::OnBattleRecord_UpdateHP()
{
	UpdateMirrorGuage();
}

void UMirrorDungeonManager::UpdateMirrorGuage()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->BattleRecordManager != nullptr, TEXT("RGameInstance->BattleRecordManager is Empty"));

	m_nMyHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_MY);
	m_nTargetHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_ENEMY);

	float my_percent = (float)(m_nMyHeroesCurrentHP) / (float)(m_nMyHeroesMaxHP);
	float target_percent = (float)(m_nTargetHeroesCurrentHP) / (float)(m_nTargetHeroesMaxHP);

	UUP_InGameHUD_Mirror* in_game_hud = Cast<UUP_InGameHUD_Mirror>(UUIFunctionLibrary::GetInGameHUD());
	if (in_game_hud)
	{
		in_game_hud->UpdateMyTotalHP(m_nMyHeroesCurrentHP, m_nMyHeroesMaxHP, my_percent);
		in_game_hud->UpdateEnemyTotalHP(m_nTargetHeroesCurrentHP, m_nTargetHeroesMaxHP, target_percent);
	}
}

void UMirrorDungeonManager::SetCameraModeAfterGameStart(EArenaCameraMode cameraMode)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	UUP_InGameHUD_Mirror* in_game_hud = Cast<UUP_InGameHUD_Mirror>(UUIFunctionLibrary::GetInGameHUD());
	if (in_game_hud)
	{
		int32 Index = (int32)cameraMode;
		in_game_hud->SetToggleCameraIcon(Index);
		in_game_hud->SetArenaCamera();
	}
}

void UMirrorDungeonManager::InitMirrorGuage()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->BattleRecordManager != nullptr, TEXT("RGameInstance->BattleRecordManager is Empty"));

	m_nMyHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_MY);
	m_nTargetHeroesCurrentHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_ENEMY);

	m_nMyHeroesMaxHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_MY, true);
	m_nTargetHeroesMaxHP = RGameInstance->BattleRecordManager->GetTeamTotalHP(GroupKey_SINGLE_ENEMY, true);

	UpdateMirrorGuage();
}

void UMirrorDungeonManager::ResetCameraMode()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UUP_InGameHUD_Mirror* in_game_hud = Cast<UUP_InGameHUD_Mirror>(UUIFunctionLibrary::GetInGameHUD());
	if (in_game_hud)
	{
		in_game_hud->ResetCameraMode();
	}
}

void UMirrorDungeonManager::RequestPlayStart()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance->HttpClient->IsValidNetwork())
	{
		FDECKS deck = RGameInstance->RInventory->GetHeroDeck();
		UPacketFunctionLibrary::MIRROR_PLAY_START_RQ(SelectedMirrorSlotData.mirrorUD, m_MirrorMyHeroUDs);
	}
}

void UMirrorDungeonManager::RequestPlayDone()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	TArray<int32> myHpList;
	TArray<int32> enemyHpList;
	GetMyHeroHpList(myHpList);
	GetEnemyHpList(enemyHpList);

	UPacketFunctionLibrary::MIRROR_PLAY_DONE_RQ(m_MirrorPlayUD, (m_MyWin ? 1 : 0), myHpList, enemyHpList);
}

void UMirrorDungeonManager::GetMyHeroHpList(TArray<int32>& InHpList)
{
	URGameInstance* RGameInstnace = RGAMEINSTANCE(this);
	int32 Count = 0;
	
	for (int32 i = 0; i < m_MirrorMyHeroUDs.Num(); ++i)
	{
		if (!m_MirrorMyHeroUDs[i].IsEmpty() && mMyHeroList.IsValidIndex(Count))
		{
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mMyHeroList[Count]);
			if (BattleComp)
			{
				InHpList.Emplace(BattleComp->GetHP());
			}
			else
			{
				InHpList.Emplace(0);
			}

			++Count;
		}
		else
		{
			InHpList.Add(0);
		}
	}
}

void UMirrorDungeonManager::GetMyHeroMaxHPList(TArray<int32>& InMaxHPList)
{
	URGameInstance* RGameInstnace = RGAMEINSTANCE(this);
	int32 Count = 0;

	for (int32 i = 0; i < m_MirrorMyHeroUDs.Num(); ++i)
	{
		if (!m_MirrorMyHeroUDs[i].IsEmpty() && mMyHeroList.IsValidIndex(Count))
		{
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mMyHeroList[Count]);
			if (BattleComp)
			{
				UCharacterBattleInfo* CharacterBattleInfo = BattleComp->GetBattleInfo();
				InMaxHPList.Add(CharacterBattleInfo->GetCharacterFinalProperty< int32 >(ECharacterPropertyTypeEnum::VE_MaxHP));
			}
			else
			{
				InMaxHPList.Add(0);
			}

			++Count;
		}
		else
		{
			InMaxHPList.Add(0);
		}
	}

}

void UMirrorDungeonManager::GetEnemyHpList(TArray<int32>& InHpList)
{
	URGameInstance* RGameInstnace = RGAMEINSTANCE(this);
	int32 Count = 0;

	for (int32 i = 0; i < m_MatchList.decks.defense.Num(); ++i)
	{
		if (!m_MatchList.decks.defense[i].IsEmpty() && mEnemyList.IsValidIndex(Count))
		{
			UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mEnemyList[Count]);
			if (BattleComp)
			{
				InHpList.Add(BattleComp->GetHP());
			}
			else
			{
				InHpList.Add(0);
			}

			++Count;
		}
		else
		{
			InHpList.Add(0);
		}
	}
}

void UMirrorDungeonManager::MirrorDoneRP()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	auto RGameMode = Cast<AMirrorGameMode>(UGameplayStatics::GetGameMode(RGameInstance));
	RGameMode->OnPlayModeMirrorDoneRpDelegate();
}

void UMirrorDungeonManager::ClearMirrorDungeonInfo()
{
	mMyHeroList.Empty();
	mEnemyList.Empty();
}

void UMirrorDungeonManager::DecideWinner()
{
	if (m_nMyHeroesCurrentHP < m_nTargetHeroesCurrentHP)
		m_MyWin = false;
	else
		m_MyWin = true;
}

bool UMirrorDungeonManager::IsCombatEnd()
{
	bool result = false;

	int32 index = 0;
	int32 aliveMyHero = GetAliveMyHero();
	int32 aliveEnemyHero = GetAliveEnemy();

	if (aliveMyHero == 0)
		result = true;
	else if (aliveEnemyHero == 0)
		result = true;

	return result;
}

int32 UMirrorDungeonManager::GetAliveMyHero()
{
	int32 aliveMyHero = 0;
	for (int32 index = 0; index < mMyHeroList.Num(); ++index) {
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mMyHeroList[index]);
		if (IsValid(BattleEntityComponent)) {
			if (!BattleEntityComponent->IsDead())
				aliveMyHero++;
		}
	}
	return aliveMyHero;
}

int32 UMirrorDungeonManager::GetAliveEnemy()
{
	int32 aliveEnemyHero = 0;
	for (int32 index = 0; index < mEnemyList.Num(); ++index) {
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(mEnemyList[index]);
		if (IsValid(BattleEntityComponent)) {
			if (!BattleEntityComponent->IsDead())
				aliveEnemyHero++;
		}
	}
	return aliveEnemyHero;
}

// void UMirrorDungeonManager::GetMyHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 
// 	icon_infos.Init(FHERO_ICON_INFO(), MAX_HERO_PLACE_COUNT);
// 
// 	FHERO_ICON_INFO icon_info;
// 	int32 hero_count = m_MirrorMyHeroUDs.Num();
// 	int32 Count = 0;
// 	for (int32 n = 0; n < hero_count; ++n)
// 	{
// 		FHERO* Hero = RGameInstance->RInventory->GetHeroWithUD(m_MirrorMyHeroUDs[n]);
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
// 		int32 nMaxHP = 0;
// 		UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(RGameInstance, Hero->heroUD);
// 		if (CharacterBattleInfo)
// 			nMaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
// 		icon_info.MaxHP = nMaxHP;
// 
// 		icon_infos[Count] = icon_info;
// 		Count++;
// 	}
// }
// 
// void UMirrorDungeonManager::GetMatchTargetHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos)
// {
// 	icon_infos.Empty();
// 
// 	GetMatchHeroesIconInfo(icon_infos);
// }
// 
// void UMirrorDungeonManager::GetMatchHeroesIconInfo(TArray<FHERO_ICON_INFO>& icon_infos)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 
// 	icon_infos.Init(FHERO_ICON_INFO(), MAX_HERO_PLACE_COUNT);
// 
// 	FHERO_ICON_INFO icon_info;
// 	int32 defenseIndex = 0;
// 	int32 hero_count = m_MatchList.decks.defense.Num();
// 	for (int32 n = 0; n < hero_count; ++n)
// 	{
// 		FString ud = m_MatchList.decks.defense[n];
// 		if(ud.IsEmpty())
// 			continue;
// 
// 		FEQUIPED_HERO defensehero;
// 
// 		if (RGameInstance->OtherHeroManager->GetOtherHeroData(m_MatchList.kId, m_MatchList.decks.defense[n], defensehero) == false)
// 			continue;
// 
// 		auto CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(defensehero.heroId);
// 
// 		icon_info.heroUD = defensehero.heroUD;
// 		icon_info.heroId = defensehero.heroId;
// 		icon_info.level = defensehero.level;
// 		icon_info.grade = CharacterInfo->Grade;
// 		icon_info.PortraitTextureRef = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, defensehero.heroId);
// 
// 		int32 nMaxHP = 0;
// 		UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(RGameInstance, defensehero);
// 		if (CharacterBattleInfo)
// 			nMaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
// 		icon_info.MaxHP = nMaxHP;
// 		icon_infos[defenseIndex] = icon_info;
// 		++defenseIndex;
// 	}
// }

bool UMirrorDungeonManager::GetMatchTargetInfo(FBATTLE_USER& target_info)
{
	target_info = m_MatchList;
	return true;
}

void UMirrorDungeonManager::SetMirrorHeroPosition(bool my_hero, int32 pos_index, FVector pos, FRotator rot)
{
	if (0 < pos_index && pos_index <= MAX_HERO_POS_COUNT)
	{
		FTransform spawn_pos;
		spawn_pos.SetRotation(FQuat(rot));
		spawn_pos.SetLocation(pos);

		if (m_MatchHeroSpawnPosList.IsValidIndex(pos_index - 1) == false)
			return;

		if (my_hero)
		{
			m_MyHeroSpawnPosList[pos_index - 1] = spawn_pos;
		}
		else
		{
			m_MatchHeroSpawnPosList[pos_index - 1] = spawn_pos;
		}
	}
}

bool UMirrorDungeonManager::GetMyHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos)
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

bool UMirrorDungeonManager::GetMatchHeroSpawnPosition(int32 hero_deck_index, FTransform& spawn_pos)
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

bool UMirrorDungeonManager::IsProgressMirrorSlot()
{
	for (FMIRROR& mirror : MirrorStatus.mirrors)
	{
		if(0 < mirror.hpList.Num())
			return true;
	}

	return false;
}