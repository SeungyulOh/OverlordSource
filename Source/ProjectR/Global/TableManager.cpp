// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "TableManager.h"
#include "UtilFunctionIntegrated.h"

// #if WITH_EDITOR

// #endif

#define USE_INVALID_TABLE_KEY_WARN 0

UTableManager* UTableManager::InstancePtr = nullptr;

#define LOAD_RTABLE_PIE( Table, Path )\
	static ConstructorHelpers::FObjectFinder<UDataTable> Table##_BP(TEXT(Path));\
	Table = Table##_BP.Object;

#if UE_BUILD_SHIPPING
#define LOAD_RTABLE( Table, Path )\
	Table = LoadTableFromPath( Path );
#else
#define LOAD_RTABLE( Table, Path )\
	Table = LoadTableFromPath( Path );\
	double Table##MyTime = FPlatformTime::Seconds();\
	double Table##ElapsedTime = Table##MyTime - CurSecond;\
	CurSecond = Table##MyTime;\
	FResourceSizeEx Table##CumulativeResourceSize;\
	Table->GetResourceSizeEx(Table##CumulativeResourceSize);\
	Total_CumulativeResourceSize += Table##CumulativeResourceSize;\
	UE_LOG(LogGamePlay, Log, TEXT( "%f - %s : Size - %d"), Table##ElapsedTime, TEXT( #Table ), Table##CumulativeResourceSize.GetTotalMemoryBytes() );
#endif

//#define LOAD_RJSONSTRING_PIE( Table, Path )\
//	static ConstructorHelpers::FObjectFinder<UJSONString> Table##_BP(TEXT(Path));\
//	Table = Table##_BP.Object;
//
//#define LOAD_RJSONSTRING( Table, Path )\
//	Table = LoadJSONStringFromPath( Path );

static const FString ContextString(TEXT("GENERAL"));

UTableManager::UTableManager()
{	
	
}

UDataTable* UTableManager::LoadTableFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;
	
	UObject* DataTableObject = (StaticLoadObject(UDataTable::StaticClass(), NULL, *Path.ToString()));
	UDataTable* DataTablePtr = Cast< UDataTable >(DataTableObject);
	LoadedTables.Emplace(DataTablePtr);

	return DataTablePtr;
}



UTableManager* UTableManager::GetInstancePtr()
{
	if (InstancePtr == nullptr)
	{
		InstancePtr = NewObject< UTableManager >((UObject*)GetTransientPackage(), UTableManager::StaticClass());
		InstancePtr->InitDataTable();
		InstancePtr->AddToRoot();
	}
	
	return InstancePtr;
}

void UTableManager::ReleaseSingleton()
{
	if (InstancePtr)
	{
		InstancePtr->RemoveFromRoot();
	}
	InstancePtr = nullptr;
	//UnloadAllTables();
}

void UTableManager::ReloadAllTables()
{
	InitDataTable();
}

void UTableManager::UnloadTable(UDataTable*& table)
{
	if (table != nullptr)
	{
		table->ConditionalBeginDestroy();
		table = nullptr;
	}
}

void UTableManager::UnloadAllTables()
{
	for (auto DataTablePtr : LoadedTables)
	{
		if( IsValid(DataTablePtr ) )
			UnloadTable(DataTablePtr);
	}
}

void UTableManager::UnMemAllTables()
{
	for (auto Element : LoadedTables)
	{
		if (IsValid(Element))
			Element->EmptyTable();
	}
}

void UTableManager::InitDataTable()
{
	//miss match
	int32 NumTable = 90; //table add(++) del(--)
	LoadedTables.Empty(NumTable);

//#if !WITH_EDITOR	
	double CurSecond = FPlatformTime::Seconds();	
	FResourceSizeEx Total_CumulativeResourceSize;

	LOAD_RTABLE(DTSkillTable_V2, "DataTable'/Game/Data/DT_Skill_V2.DT_Skill_V2'");

//	자동 리임포트
// 	if (FPaths::FileExists(TEXT("C:/overlord/DesignDocument/Tables/DT_Skill_V2.csv")))
// 	{
// 		FString FileContent;
// 		//Read the csv file
// 		FFileHelper::LoadFileToString(FileContent, TEXT("C:/overlord/DesignDocument/Tables/DT_Skill_V2.csv"));
// 		if (DTSkillTable_V2)
// 		{
// 			TArray<FString> problems = DTSkillTable_V2->CreateTableFromCSVString(FileContent);
// 		}
// 	}

	LOAD_RTABLE(DTItemPassiveSkill, "DataTable'/Game/Data/DT_ItemPassiveSkill.DT_ItemPassiveSkill'");
	LOAD_RTABLE(SkillLevelUpCostTable, "DataTable'/Game/Data/DT_SkillLevelUpCost.DT_SkillLevelUpCost'");
	LOAD_RTABLE(DTSkillResourceTable, "DataTable'/Game/Data/DT_SkillResource.DT_SkillResource'");	
	LOAD_RTABLE(DTSkillConditionTable, "DataTable'/Game/Data/DT_SkillCondition.DT_SkillCondition'");
	LOAD_RTABLE(DTDailyMissionTable, "DataTable'/Game/Data/DT_DailyMission.DT_DailyMission'");
	LOAD_RTABLE(DTFloorMissionTable, "DataTable'/Game/Data/DT_FloorMission.DT_FloorMission'");
	LOAD_RTABLE(DTBulletTable, "DataTable'/Game/Data/DT_Bullet.DT_Bullet'");
	LOAD_RTABLE(DTCharacterTable, "DataTable'/Game/Data/DT_Character.DT_Character'");
	TArray<FStringFormatArg> Args;
	HeroGrowthTables.Empty();
	
	TArray<FName> CharacterIds = DTCharacterTable->GetRowNames();
	for (FName CharacterId : CharacterIds)
	{
		FCharacterTableInfo* ChracterInfo = GetCharacterRow(CharacterId);
		if (false == ChracterInfo->IsHero)
			continue;
		if (false == HeroGrowthTables.Contains(ChracterInfo->GrowthTableID) && false == ChracterInfo->GrowthTableID.IsNone())
		{
			Args.Reset();
			Args.Push(ChracterInfo->GrowthTableID.ToString());
			UDataTable* GrowthTable = nullptr;
			FString GrowPath = FString::Format(TEXT("DataTable'/Game/Data/GrowthTables/HeroGrowth/{0}.{0}'"), Args);
			LOAD_RTABLE(GrowthTable, *GrowPath);
			HeroGrowthTables.Emplace(ChracterInfo->GrowthTableID, GrowthTable);
		}
	}
	{
		HeroEnchantTables.Empty(5);
		UDataTable* DT_HeroEnchant_Tanker = nullptr;
		LOAD_RTABLE(DT_HeroEnchant_Tanker, "DataTable'/Game/Data/GrowthTables/HeroEnchant/DT_HeroEnchant_Tanker.DT_HeroEnchant_Tanker'");
		HeroEnchantTables.Emplace(EClassTypeEnum::VE_Tanker, DT_HeroEnchant_Tanker);
		UDataTable* DT_HeroEnchant_Supporter = nullptr;
		LOAD_RTABLE(DT_HeroEnchant_Supporter, "DataTable'/Game/Data/GrowthTables/HeroEnchant/DT_HeroEnchant_Supporter.DT_HeroEnchant_Supporter'");
		HeroEnchantTables.Emplace(EClassTypeEnum::VE_Supporter, DT_HeroEnchant_Supporter);
		UDataTable* DT_HeroEnchant_RangeDealer = nullptr;
		LOAD_RTABLE(DT_HeroEnchant_RangeDealer, "DataTable'/Game/Data/GrowthTables/HeroEnchant/DT_HeroEnchant_RangeDealer.DT_HeroEnchant_RangeDealer'");
		HeroEnchantTables.Emplace(EClassTypeEnum::VE_RangeDealer, DT_HeroEnchant_RangeDealer);
		UDataTable* DT_HeroEnchant_MeleeDealer = nullptr;
		LOAD_RTABLE(DT_HeroEnchant_MeleeDealer, "DataTable'/Game/Data/GrowthTables/HeroEnchant/DT_HeroEnchant_MeleeDealer.DT_HeroEnchant_MeleeDealer'");
		HeroEnchantTables.Emplace(EClassTypeEnum::VE_MeleeDealer, DT_HeroEnchant_MeleeDealer);
		UDataTable* DT_HeroEnchant_MagicDealer = nullptr;
		LOAD_RTABLE(DT_HeroEnchant_MagicDealer, "DataTable'/Game/Data/GrowthTables/HeroEnchant/DT_HeroEnchant_MagicDealer.DT_HeroEnchant_MagicDealer'");
		HeroEnchantTables.Emplace(EClassTypeEnum::VE_MagicDealer, DT_HeroEnchant_MagicDealer);
	}
	
	LOAD_RTABLE(DTCharacterNpcTable, "DataTable'/Game/Data/DT_CharacterNpc.DT_CharacterNpc'");
	LOAD_RTABLE(DTCharacterCostEnchant, "DataTable'/Game/Data/DT_CharacterCostEnchant.DT_CharacterCostEnchant'");
	LOAD_RTABLE(DTCharacterCostEnchantGrade, "DataTable'/Game/Data/DT_CharacterEnchantGrade.DT_CharacterEnchantGrade'");
	
	LOAD_RTABLE(DTBattleObjectTable, "DataTable'/Game/Data/DT_BattleObject.DT_BattleObject'");
	//10

	LOAD_RTABLE(DTMapTable, "DataTable'/Game/Data/DT_Map.DT_Map'");
	LOAD_RTABLE(DTMapPostProcessSettingTable, "DataTable'/Game/Data/DT_MapPostProcessSetting.DT_MapPostProcessSetting'");
	LOAD_RTABLE(DTGameModeSequenceTableInfo, "DataTable'/Game/Data/DT_GameModeSequenceTableInfo.DT_GameModeSequenceTableInfo'");
	LOAD_RTABLE(DTAbnormalStateGroupTable_V2, "DataTable'/Game/Data/DT_AbnormalStateGroup_V2.DT_AbnormalStateGroup_V2'");
	LOAD_RTABLE(DTCharacterItemTable, "DataTable'/Game/Data/DT_CharacterItem.DT_CharacterItem'");
	LOAD_RTABLE(DTItemRandomOptionTable, "DataTable'/Game/Data/DT_ItemRandomOption_reformatted.DT_ItemRandomOption_reformatted'");
	LOAD_RTABLE(DTItemUpgradeCostTable, "DataTable'/Game/Data/DT_ItemUpgradeCost.DT_ItemUpgradeCost'");
	LOAD_RTABLE(DTItemLevelUpTable, "DataTable'/Game/Data/DT_ItemLevelUp.DT_ItemLevelUp'");
	LOAD_RTABLE(DTForceTutorialDetailTable, "DataTable'/Game/Data/DT_ForceTutorialDetail.DT_ForceTutorialDetail'");
	LOAD_RTABLE(DTGuildLevelupTable, "DataTable'/Game/Data/DT_GuildLevelup.DT_GuildLevelup'");
	//20

	LOAD_RTABLE(DTGuildContributeTable, "DataTable'/Game/Data/DT_GuildContribute.DT_GuildContribute'");

	LOAD_RTABLE(DTRaidStageTableInfo, "DataTable'/Game/Data/DT_RaidStageTable.DT_RaidStageTable'");

	LOAD_RTABLE(FloorStageTable, "DataTable'/Game/Data/DT_FloorStage.DT_FloorStage'");
	LOAD_RTABLE(FloorInfoTable, "DataTable'/Game/Data/DT_FloorInfo.DT_FloorInfo'");


	//	Attendance
	LOAD_RTABLE(DTAttendanceTableInfo, "DataTable'/Game/Data/DT_Attendance.DT_Attendance'");
//#ifndef RULE_TODOMEMORY
//#endif
	//30

	LOAD_RTABLE(RewardPreviewTable, "DataTable'/Game/Data/DT_RewardPreview.DT_RewardPreview'");
	LOAD_RTABLE(RewardPreviewItemTable, "DataTable'/Game/Data/DT_RewardPreviewItem.DT_RewardPreviewItem'");
	LOAD_RTABLE(FloorQuestTable, "DataTable'/Game/Data/DT_FloorQuestTable.DT_FloorQuestTable'");
	LOAD_RTABLE(WidgetBluePrintTable, "DataTable'/Game/Data/DT_WidgetBluePrint.DT_WidgetBluePrint'");
	LOAD_RTABLE(DTForceTutorialTable, "DataTable'/Game/Data/DT_ForceTutorial.DT_ForceTutorial'");
	LOAD_RTABLE(DTAchievementTable, "DataTable'/Game/Data/DT_Achievement.DT_Achievement'");
	LOAD_RTABLE(DTContentsUnlockTable, "DataTable'/Game/Data/DT_Unlock.DT_Unlock'");
	//	Growth tables
	LOAD_RTABLE(NPCGrowTables, "DataTable'/Game/Data/GrowthTables/DT_HeroGrowth.DT_HeroGrowth'");
	LOAD_RTABLE(HeroGrowthMultiplierTable, "DataTable'/Game/Data/GrowthTables/DT_HeroGrowthMultiplier.DT_HeroGrowthMultiplier'");
	LOAD_RTABLE(LevelGrowthTable, "DataTable'/Game/Data/GrowthTables/DT_LevelGrowth.DT_LevelGrowth'");
	//	40

	LOAD_RTABLE(DTItemEnchantTable, "DataTable'/Game/Data/DT_ItemEnchant.DT_ItemEnchant'");
	LOAD_RTABLE(DTItemManufactureTable, "DataTable'/Game/Data/DT_ItemManufature.DT_ItemManufature'");
	//	RaidBoss table
	LOAD_RTABLE(DTRaidBossTableInfo, "DataTable'/Game/Data/DT_Raid.DT_Raid'");
	LOAD_RTABLE(DTRaidBossAttackTableInfo, "DataTable'/Game/Data/DT_Raid_Attack.DT_Raid_Attack'");
	LOAD_RTABLE(DTRaidBossTermsTableInfo, "DataTable'/Game/Data/DT_Raid_Terms.DT_Raid_Terms'");
	//	BattleGround
	LOAD_RTABLE(DTBattleGroundTableInfo, "DataTable'/Game/Data/DT_BattleGround.DT_BattleGround'");
	//	Battle GuildRaid
	LOAD_RTABLE(DTBattleGuildRaidTableInfo, "DataTable'/Game/Data/DT_BattleGuildRaid.DT_BattleGuildRaid'");
	//	GuildRaid Reward
	LOAD_RTABLE(DTGuildRaidRewardTableInfo, "DataTable'/Game/Data/DT_GuildRaidReward.DT_GuildRaidReward'");
	//	50

	//	Content Reset
	LOAD_RTABLE(DTContentResetTableInfo, "DataTable'/Game/Data/DT_ContentReset.DT_ContentReset'");
	//	Battle Logic Config
	LOAD_RTABLE(DTBattleLogicConfigTable, "DataTable'/Game/Data/DT_BattleLogicConfig.DT_BattleLogicConfig'");
	LOAD_RTABLE(DTBattlePowerTable, "DataTable'/Game/Data/GrowthTables/DT_BattlePower.DT_BattlePower'");
	LOAD_RTABLE(HPProgressInfoTable, "DataTable'/Game/Data/DT_HPProgress.DT_HPProgress'");
	//	PVP StaminaUse table
	LOAD_RTABLE(PVPStaminaUseTable, "DataTable'/Game/Data/DT_StaminaUse.DT_StaminaUse'");
	//	Avatar Ability table
	LOAD_RTABLE(DTAvatarAbilityTableInfo, "DataTable'/Game/Data/DT_AvatarAbility.DT_AvatarAbility'");
	LOAD_RTABLE(DTConstantVariablesTable, "DataTable'/Game/Data/DT_ConstantVariables.DT_ConstantVariables'");
	//	56

	//	Resource table
	LOAD_RTABLE(ResourceCacheTable, "DataTable'/Game/Data/ResourceTables/DT_ResourceCache.DT_ResourceCache'");
	LOAD_RTABLE(ResourceParticleTable, "DataTable'/Game/Data/ResourceTables/DT_ParticleResource.DT_ParticleResource'");
	LOAD_RTABLE(ResourceMaterialTable, "DataTable'/Game/Data/ResourceTables/DT_MaterialResource.DT_MaterialResource'");
	LOAD_RTABLE(ResourceTexture2DTable, "DataTable'/Game/Data/ResourceTables/DT_Texture2DResource.DT_Texture2DResource'");
	LOAD_RTABLE(ResourceDamageFloaterTex2DTable, "DataTable'/Game/Data/ResourceTables/DT_DamageFloaterTexture2D.DT_DamageFloaterTexture2D'");
	LOAD_RTABLE(ResourceSpriteTableInfo, "DataTable'/Game/Data/ResourceTables/DT_SpriteResource.DT_SpriteResource'");
	LOAD_RTABLE(ResourceStaticMeshTable, "DataTable'/Game/Data/ResourceTables/DT_StaticMeshResource.DT_StaticMeshResource'");
	LOAD_RTABLE(ResourceFXTable, "DataTable'/Game/Data/ResourceTables/DT_FXResource.DT_FXResource'");
	LOAD_RTABLE(ResourceSoundTableInfo, "DataTable'/Game/Data/ResourceTables/DT_SoundResource.DT_SoundResource'");
	LOAD_RTABLE(ResourceSequenceTableInfo, "DataTable'/Game/Data/ResourceTables/DT_SequenceResource.DT_SequenceResource'");
	LOAD_RTABLE(ResourceCustomActorTableInfo, "DataTable'/Game/Data/ResourceTables/DT_CustomActorResource.DT_CustomActorResource'");
	LOAD_RTABLE(DropItemResourceTableInfo, "DataTable'/Game/Data/DT_DropItemResource.DT_DropItemResource'");
	LOAD_RTABLE(DropItemParticleTableInfo, "DataTable'/Game/Data/DT_DropItemParticle.DT_DropItemParticle'");
	LOAD_RTABLE(ResourceCurveTableInfo, "DataTable'/Game/Data/ResourceTables/DT_CurveResource.DT_CurveResource'");

	LOAD_RTABLE(SkillCastingEffectTableInfo, "DataTable'/Game/Data/GameSetting/DT_SkillCastingEffect.DT_SkillCastingEffect'");
	LOAD_RTABLE(InventoryFilterTableInfo, "DataTable'/Game/Data/DT_InventoryFilter.DT_InventoryFilter'");
	//	67
	
	LOAD_RTABLE(DialogueAnimationTableInfo, "DataTable'/Game/Data/DT_DialogueAnimation.DT_DialogueAnimation'");
	LOAD_RTABLE(DialogueTable, "DataTable'/Game/Data/DT_DialogueInfo.DT_DialogueInfo'");
	LOAD_RTABLE(DialogueResourceTable, "DataTable'/Game/Data/DT_DialogueResource.DT_DialogueResource'");


	LOAD_RTABLE(DTLobbyCharacterTableInfo, "DataTable'/Game/Data/DT_LobbyCharacter.DT_LobbyCharacter'");
	LOAD_RTABLE(DTIndoorHeroManagementViewTableInfo, "DataTable'/Game/Data/GameSetting/DT_IndoorHeroManagementView.DT_IndoorHeroManagementView'");
	LOAD_RTABLE(DTTowerOptionTable, "DataTable'/Game/Data/DT_TowerOption.DT_TowerOption'");
	LOAD_RTABLE(CurrencyPropertyTableInfo, "DataTable'/Game/Data/DT_CurrencyProperty.DT_CurrencyProperty'");
	LOAD_RTABLE(ShopProductTableInfo, "DataTable'/Game/Data/DT_ShopProduct.DT_ShopProduct'");
	LOAD_RTABLE(InAppPurchaseTableInfo, "DataTable'/Game/Data/DT_InAppPurchase.DT_InAppPurchase'");
	LOAD_RTABLE(ShopDisplayGroupTableInfo, "DataTable'/Game/Data/DT_ShopDisplayGroup.DT_ShopDisplayGroup'");

	LOAD_RTABLE(DTShopCharacterSummon, "DataTable'/Game/Data/DT_ShopCharacterSummon.DT_ShopCharacterSummon'");
	LOAD_RTABLE(TimeTreasureTable, "DataTable'/Game/Data/DT_TimeTreasure.DT_TimeTreasure'");


	LOAD_RTABLE(LoadingTableInfo, "DataTable'/Game/Data/DT_Loading.DT_Loading'");
	LOAD_RTABLE(TopbarTableInfo, "DataTable'/Game/Data/DT_TopbarUI.DT_TopbarUI'");
	// Chat
	LOAD_RTABLE(DTChatStringAttributeTableInfo, "DataTable'/Game/Data/GameSetting/DT_ChatStringAttribute.DT_ChatStringAttribute'");
	//	77

	LOAD_RTABLE(DTItemGradeTableInfo, "DataTable'/Game/Data/GameSetting/DT_ItemGrade.DT_ItemGrade'");
	LOAD_RTABLE(DTContentsUITableInfo, "DataTable'/Game/Data/GameSetting/DT_ContentsUI.DT_ContentsUI'");
	LOAD_RTABLE(DTItemEquipTypeTableInfo, "DataTable'/Game/Data/GameSetting/DT_ItemEquipType.DT_ItemEquipType'");

	//	by danzang
	LOAD_RTABLE(HeroManagementCameraTableInfo, "DataTable'/Game/Data/DT_HeromanagementCamera.DT_HeromanagementCamera'");
	//	Media FrameWork
	LOAD_RTABLE(DTMediaTableInfo, "DataTable'/Game/Data/DT_Media.DT_Media'");
	//	Media Subtitle
	LOAD_RTABLE(DTSubtitleTableInfo, "DataTable'/Game/Data/DT_Subtitles.DT_Subtitles'");

	//	HeroShop
	LOAD_RTABLE(DTHeroShopProductTableInfo, "DataTable'/Game/Data/DT_HeroShopProduct.DT_HeroShopProduct'");
	LOAD_RTABLE(DTHeroShopSynergyTableInfo, "DataTable'/Game/Data/DT_HeroShopSynergy.DT_HeroShopSynergy'");
	LOAD_RTABLE(DTHeroMovieTableInfo, "DataTable'/Game/Data/DT_HeroMovie.DT_HeroMovie'");
	//	84

	LOAD_RTABLE(DTCrewSpotTableInfo, "DataTable'/Game/Data/DT_CrewSpot.DT_CrewSpot'");
	LOAD_RTABLE(DTCharacterCostUpgrade, "DataTable'/Game/Data/DT_CharacterCostUpgrade.DT_CharacterCostUpgrade'");
	LOAD_RTABLE(DTCharacterCostGrowth, "DataTable'/Game/Data/DT_CharacterCostGrowth.DT_CharacterCostGrowth'");
	LOAD_RTABLE(DTWallet, "DataTable'/Game/Data/DT_Wallet.DT_Wallet'");
	LOAD_RTABLE(DTRuneOption, "DataTable'/Game/Data/DT_RuneOption.DT_RuneOption'");
	LOAD_RTABLE(DTRuneWord, "DataTable'/Game/Data/DT_RuneWord.DT_RuneWord'");	//	
	LOAD_RTABLE(DTRuneWordSkill, "DataTable'/Game/Data/DT_RuneWordSkill.DT_RuneWordSkill'");	//	
	LOAD_RTABLE(DTTowerSkill, "DataTable'/Game/Data/DT_TowerSkill.DT_TowerSkill'");
	LOAD_RTABLE(DTWorldBossStage, "DataTable'/Game/Data/DT_WorldBossStage.DT_WorldBossStage'");
	LOAD_RTABLE(DTShopFloorStore, "DataTable'/Game/Data/DT_ShopFloorStore.DT_ShopFloorStore'");	//	
	LOAD_RTABLE(DTBossStage, "DataTable'/Game/Data/DT_BossStage.DT_BossStage'");	//
	LOAD_RTABLE(DTColosseumModes, "DataTable'/Game/Data/DT_ColosseumModes.DT_ColosseumModes'");	//
	LOAD_RTABLE(DTColosseumGrade, "DataTable'/Game/Data/DT_ColosseumGrade.DT_ColosseumGrade'");	//
	LOAD_RTABLE(DTColosseumRank, "DataTable'/Game/Data/DT_ColosseumRank.DT_ColosseumRank'");	//
	LOAD_RTABLE(DTGuideResource, "DataTable'/Game/Data/DT_GuideResource.DT_GuideResource'");	//
	LOAD_RTABLE(DTDailyQuestRewards, "DataTable'/Game/Data/DT_DailyQuestRewards.DT_DailyQuestRewards'");	//
	LOAD_RTABLE(DTDailyQuestContents, "DataTable'/Game/Data/DT_DailyQuestContents.DT_DailyQuestContents'");	//

	UE_LOG(LogGamePlay, Log, TEXT(" Total table size  - %d"), Total_CumulativeResourceSize.GetTotalMemoryBytes());

//#endif
}

FCharacterTableInfo* UTableManager::GetCharacterRow(FName TableKey)
{
	FCharacterTableInfo* RowInfo = DTCharacterTable->FindRow< FCharacterTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	return RowInfo;
}

FCharacterTableInfo* UTableManager::GetCharacterNpcRow(FName TableKey)
{
	FCharacterTableInfo* RowInfo = DTCharacterNpcTable->FindRow< FCharacterTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FBattleObjectTableInfo*	UTableManager::GetBattleObjectRow(FName TableKey)
{
	FBattleObjectTableInfo* RowInfo = DTBattleObjectTable->FindRow< FBattleObjectTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FSkillTableInfo_V2* UTableManager::GetSkillRow_V2(FName TableKey)
{
	FSkillTableInfo_V2* RowInfo = DTSkillTable_V2->FindRow< FSkillTableInfo_V2 >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemPassiveSkillTableInfo* UTableManager::GetItemPassiveSkillRow(FName TableKey)
{
	FItemPassiveSkillTableInfo* RowInfo = DTItemPassiveSkill->FindRow< FItemPassiveSkillTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FSkillResourceTableInfo* UTableManager::GetSkillResourceRow(FName TableKey)
{	
	FSkillResourceTableInfo* RowInfo = DTSkillResourceTable->FindRow< FSkillResourceTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}


FSkillConditionTableInfo* UTableManager::GetSkillConditionRow(FName TableKey)
{
	FSkillConditionTableInfo* RowInfo = DTSkillConditionTable->FindRow< FSkillConditionTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}


FDailyMissionTableInfo* UTableManager::GetDailyMissionTable(FName TableKey)
{
	FDailyMissionTableInfo* RowInfo = DTDailyMissionTable->FindRow< FDailyMissionTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FFloorMissionTable* UTableManager::GetFloorMissionTable(FName TableKey)
{
	FFloorMissionTable* rowInfo = DTFloorMissionTable->FindRow< FFloorMissionTable >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	return rowInfo;
}

FBulletTableInfo* UTableManager::GetBulletRow(FName TableKey)
{
	FBulletTableInfo* RowInfo = DTBulletTable->FindRow< FBulletTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FMapTableInfo* UTableManager::GetMapRow(FName TableKey)
{
	FMapTableInfo* RowInfo = DTMapTable->FindRow< FMapTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FMapPostProcessSettingInfo* UTableManager::GetMapPostProcessSettingRow(FName TableKey)
{
	FMapPostProcessSettingInfo* RowInfo = DTMapPostProcessSettingTable->FindRow< FMapPostProcessSettingInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FGameModeSequenceTableInfo* UTableManager::GetGameModeSequenceRow(FName TableKey)
{
	FGameModeSequenceTableInfo* RowInfo = DTGameModeSequenceTableInfo->FindRow< FGameModeSequenceTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FAbnormalStateGroupTableInfo_V2* UTableManager::GetAbnormalStateGroupRow_V2(FName TableKey)
{
	FAbnormalStateGroupTableInfo_V2* RowInfo = DTAbnormalStateGroupTable_V2->FindRow< FAbnormalStateGroupTableInfo_V2 >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemTableInfo* UTableManager::GetCharacterItemRow(FName TableKey)
{
	FItemTableInfo* RowInfo = DTCharacterItemTable->FindRow< FItemTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemRandomOptionTableInfo* UTableManager::GetItemRandomOptionRow(FName TableKey)
{
	FItemRandomOptionTableInfo* RowInfo = DTItemRandomOptionTable->FindRow< FItemRandomOptionTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemUpgradeCostInfo* UTableManager::GetItemUpgradeCostRow(FName TableKey)
{
	FItemUpgradeCostInfo* RowInfo = DTItemUpgradeCostTable->FindRow< FItemUpgradeCostInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemLevelUpTableInfo* UTableManager::GetItemLevelUpTableRow(FName TableKey)
{
	FItemLevelUpTableInfo* RowInfo = DTItemLevelUpTable->FindRow< FItemLevelUpTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FGachaInfo* UTableManager::GetShopCharacterSummonTableRow(FName TableKey)
{
	FGachaInfo* RowInfo = DTShopCharacterSummon->FindRow< FGachaInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FFloorStageDetail* UTableManager::GetFloorStageDetailRow(FName TableKey)
{
	FFloorStageDetail* RowInfo = FloorStageTable->FindRow< FFloorStageDetail >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FFloorInfo* UTableManager::GetFloorInfoRow(FName TableKey)
{
	FFloorInfo* RowInfo = FloorInfoTable->FindRow< FFloorInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FTimeTreasureTable* UTableManager::GetTimeTreasureTableRow(FName TableKey)
{
	FTimeTreasureTable* RowInfo = TimeTreasureTable->FindRow< FTimeTreasureTable >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FRewardPreviewTableInfo* UTableManager::GetRewardPreviewRow(FName TableKey)
{
	FRewardPreviewTableInfo* RowInfo = RewardPreviewTable->FindRow< FRewardPreviewTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FRewardPreviewItemTableInfo* UTableManager::GetRewardPreviewItemRow(FName TableKey)
{
	FRewardPreviewItemTableInfo* RowInfo = RewardPreviewItemTable->FindRow< FRewardPreviewItemTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}



FFloorQuestTableInfo* UTableManager::GetFloorQuestRow(FName TableKey)
{
	FFloorQuestTableInfo* RowInfo = FloorQuestTable->FindRow< FFloorQuestTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	return RowInfo;
}

FWidgetBluePrintTableInfo* UTableManager::GetWidgetBluePrintRow(FName TableKey)
{
	FWidgetBluePrintTableInfo* RowInfo = WidgetBluePrintTable->FindRow< FWidgetBluePrintTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FAchievementTableInfo* UTableManager::GetAchievementRow(FName TableKey)
{
	FAchievementTableInfo* RowInfo = DTAchievementTable->FindRow<FAchievementTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

TArray<FName> UTableManager::GetAchievementRowsNames()
{
	auto RowNames = DTAchievementTable->GetRowNames();
	return RowNames;
}

FLevelGrowthTableInfo* UTableManager::GetLevelGrowthRow(FName TableKey)
{
	FLevelGrowthTableInfo* RowInfo = LevelGrowthTable->FindRow< FLevelGrowthTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FSkillLevelUpCostTableInfo*	UTableManager::GetSkillLevelUpCostRow(FName TableKey)
{
	FSkillLevelUpCostTableInfo* RowInfo = SkillLevelUpCostTable->FindRow< FSkillLevelUpCostTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	return RowInfo;
}

FItemEnchantTableInfo* UTableManager::GetItemEnchantRow(FName TableKey)
{
	FItemEnchantTableInfo* RowInfo = DTItemEnchantTable->FindRow< FItemEnchantTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemManufatureTableInfo* UTableManager::GetItemManufactureRow(FName TableKey)
{
	FItemManufatureTableInfo* RowInfo = DTItemManufactureTable->FindRow< FItemManufatureTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FGuildLevelUpTableInfo* UTableManager::GetGuildLevelUpRow(FName TableKey)
{
	FGuildLevelUpTableInfo* RowInfo = DTGuildLevelupTable->FindRow< FGuildLevelUpTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FGuildContributeTableInfo* UTableManager::GetGuildContributeRow(FName TableKey)
{
	FGuildContributeTableInfo* RowInfo = DTGuildContributeTable->FindRow< FGuildContributeTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}


FRaidStageTableInfo* UTableManager::GetRaidStageTableRow(FName TableKey)
{
	FRaidStageTableInfo* RowInfo = DTRaidStageTableInfo->FindRow< FRaidStageTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );

	return RowInfo;
}

/*
Growth tables
*/

const UDataTable* UTableManager::GetHeroGrowthTable(FName TableKey)
{
	UDataTable** ptr = HeroGrowthTables.Find(TableKey);
	checkf(ptr != nullptr, TEXT("GetHeroGrowthRow {0} is Empty"), *TableKey.ToString());
	return *ptr;
}

const UDataTable* UTableManager::GetHeroEnchantTable(EClassTypeEnum TableKey)
{
	UDataTable** ptr = this->HeroEnchantTables.Find(TableKey);
	FString	stringAnimType = UUtilFunctionLibrary::GetEnumToString(TEXT("EClassTypeEnum"), (int32)TableKey);
	checkf(ptr != nullptr, TEXT("HeroEnchantTables {0} is Empty"), *stringAnimType);
	return *ptr;
}

FGrowthHeroInfo_V2* UTableManager::GetHeroGrowthInfo(const UDataTable* InHeroGrowTable, int32 level)
{
	checkf(InHeroGrowTable != nullptr, TEXT("GetHeroGrowthInfo {0} is Empty"), *FString::FromInt(level));
	FGrowthHeroInfo_V2* RowInfo = InHeroGrowTable->FindRow< FGrowthHeroInfo_V2 >(FName(*FString::FromInt(level)), ContextString, USE_INVALID_TABLE_KEY_WARN);
	return RowInfo;
}

FHeroEnchant_V2* UTableManager::GetHeroEnchantInfo(const UDataTable* InHeroEnchantTable, int32 point)
{
	checkf(InHeroEnchantTable != nullptr, TEXT("InHeroEnchantTable {0} is Empty"), *FString::FromInt(point));
	FHeroEnchant_V2* RowInfo = InHeroEnchantTable->FindRow< FHeroEnchant_V2 >(FName(*FString::FromInt(point)), ContextString, USE_INVALID_TABLE_KEY_WARN);

	return RowInfo;
}

FGrowthHeroInfo* UTableManager::GetNPCGrowInfo(FName TableKey)
{
	FGrowthHeroInfo* RowInfo = NPCGrowTables->FindRow< FGrowthHeroInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FGrowthHeroMultiplier* UTableManager::GetHeroGrowthMultiplierRow(FName TableKey)
{
	FGrowthHeroMultiplier* RowInfo = HeroGrowthMultiplierTable->FindRow< FGrowthHeroMultiplier >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FCharacterCostEnchant* UTableManager::GetCharacterCostEnchant(FName TableKey)
{
	FCharacterCostEnchant* RowInfo = DTCharacterCostEnchant->FindRow< FCharacterCostEnchant >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}


FCharacterCostEnchantGrade* UTableManager::GetCharacterCostEnchantGradeInfo(FName TableKey)
{
	FCharacterCostEnchantGrade* RowInfo = DTCharacterCostEnchantGrade->FindRow<FCharacterCostEnchantGrade>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	return RowInfo;
}
FMediaTableInfo* UTableManager::GetMediaTableRow(FName TableKey)
{
	FMediaTableInfo* RowInfo = DTMediaTableInfo->FindRow< FMediaTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;

}

FSubtitleTableInfo* UTableManager::GetSubtitleTableRow(FName TableKey)
{
	FSubtitleTableInfo* RowInfo = DTSubtitleTableInfo->FindRow< FSubtitleTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FRaidBossTableInfo* UTableManager::GetRaidBossTableRow(FName TableKey)
{
	FRaidBossTableInfo* RowInfo = DTRaidBossTableInfo->FindRow< FRaidBossTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FRaidBossAttackTableInfo* UTableManager::GetRaidBossAttackTableRow(FName TableKey)
{
	FRaidBossAttackTableInfo* RowInfo = DTRaidBossAttackTableInfo->FindRow< FRaidBossAttackTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FRaidBossTermsTableInfo* UTableManager::GetRaidBossTermsTableRow(FName TableKey)
{
	FRaidBossTermsTableInfo* RowInfo = DTRaidBossTermsTableInfo->FindRow< FRaidBossTermsTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FBattleGroundTableInfo* UTableManager::GetBattleGroundTableRow(FName TableKey)
{
	FBattleGroundTableInfo* RowInfo = DTBattleGroundTableInfo->FindRow< FBattleGroundTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FBattleGuildRaidTableInfo* UTableManager::GetBattleGuildRaidTableRow(FName TableKey)
{
	FBattleGuildRaidTableInfo* RowInfo = DTBattleGuildRaidTableInfo->FindRow< FBattleGuildRaidTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FGuildRaidRewardTableInfo* UTableManager::GetGuildRaidRewardTableRow(FName TableKey)
{
	FGuildRaidRewardTableInfo* RowInfo = DTGuildRaidRewardTableInfo->FindRow< FGuildRaidRewardTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FAttendanceTableInfo* UTableManager::GetAttendanceTableRow(FName TableKey)
{
	FAttendanceTableInfo* RowInfo = DTAttendanceTableInfo->FindRow< FAttendanceTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FStaminaUseTableInfo* UTableManager::GetStaminaUseTableRow(FName TableKey)
{
	FStaminaUseTableInfo* RowInfo = PVPStaminaUseTable->FindRow<FStaminaUseTableInfo>(TableKey, ContextString);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FForceTutorialTableInfo* UTableManager::GetForceTutorialTableRow(FName TableKey)
{
	FForceTutorialTableInfo* RowInfo = DTForceTutorialTable->FindRow< FForceTutorialTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FForceTutorialDetailTableInfo* UTableManager::GetForceTutorialDetailTableRow(FName TableKey)
{
	FForceTutorialDetailTableInfo* RowInfo = DTForceTutorialDetailTable->FindRow< FForceTutorialDetailTableInfo >(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}


FHPProgressInfo* UTableManager::GetHPProgressInfoTableRow(FName TableKey)
{
	FHPProgressInfo*	RowInfo = HPProgressInfoTable->FindRow<FHPProgressInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

/*
Avatar Ability table
*/
FAvatarAbilityTableInfo* UTableManager::GetAvatarAbilityTableRow(FName TableKey)
{
	FAvatarAbilityTableInfo* RowInfo = DTAvatarAbilityTableInfo->FindRow<FAvatarAbilityTableInfo>(TableKey, ContextString);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

/*
Content Reset
*/

FContentResetTableInfo*	UTableManager::GetContentResetTableRow(FName TableKey)
{
	FContentResetTableInfo* RowInfo = DTContentResetTableInfo->FindRow<FContentResetTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FBattleLogicConfigTable* UTableManager::GetBattleLogicConfigTableRow(FName TableKey)
{
	FBattleLogicConfigTable* RowInfo = DTBattleLogicConfigTable->FindRow<FBattleLogicConfigTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

float UTableManager::GetBattleLogicConfigValue(FName TableKey)
{
	FBattleLogicConfigTable* RowInfo = GetBattleLogicConfigTableRow(TableKey);
	if (RowInfo)
		return RowInfo->BattleConstant;

	return 0.0f;
}

FConstantVariablesTableInfo* UTableManager::GetConstantVariablesRow(FName TableKey)
{
	FConstantVariablesTableInfo* RowInfo = DTConstantVariablesTable->FindRow<FConstantVariablesTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FBattlePowerTable* UTableManager::GetBattlePowerRow(int32 InLevel)
{
	FName TableKey = FName(*FText::AsNumber(InLevel).ToString());

	FBattlePowerTable* RowInfo = DTBattlePowerTable->FindRow<FBattlePowerTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);

	return RowInfo;
}

FChatStringAttributeTableInfo* UTableManager::GetChatStringAttributeTableRow(FName TableKey)
{
	FChatStringAttributeTableInfo* RowInfo = DTChatStringAttributeTableInfo->FindRow<FChatStringAttributeTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemGradeTableInfo* UTableManager::GetItemGradeTableRow(FName TableKey)
{
	FItemGradeTableInfo* RowInfo = DTItemGradeTableInfo->FindRow<FItemGradeTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FContentsUITableInfo* UTableManager::GetContentsUITableRow(FName TableKey)
{
	FContentsUITableInfo* RowInfo = DTContentsUITableInfo->FindRow<FContentsUITableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FItemEquipTypeTableInfo* UTableManager::GetItemEquipTypeTableRow(FName TableKey)
{
	FItemEquipTypeTableInfo* RowInfo = DTItemEquipTypeTableInfo->FindRow<FItemEquipTypeTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceMaterialTableInfo* UTableManager::GetResourceMaterialRow(FName TableKey)
{
	FResourceMaterialTableInfo*	RowInfo = ResourceMaterialTable->FindRow<FResourceMaterialTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceCacheTable* UTableManager::GetResourceCacheRow(FName TableKey)
{
	FResourceCacheTable*	RowInfo = ResourceCacheTable->FindRow<FResourceCacheTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceParticleTableInfo*	UTableManager::GetResourceParticleRow(FName TableKey)
{
	FResourceParticleTableInfo*	RowInfo = ResourceParticleTable->FindRow<FResourceParticleTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceTexture2DTableInfo* UTableManager::GetResourceTexture2DRow(FName TableKey)
{
	FResourceTexture2DTableInfo*	RowInfo = ResourceTexture2DTable->FindRow<FResourceTexture2DTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceTexture2DTableInfo* UTableManager::GetResourceDamageFloaterTex2DRow(FName TableKey)
{
	FResourceTexture2DTableInfo*	RowInfo = ResourceDamageFloaterTex2DTable->FindRow<FResourceTexture2DTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	return RowInfo;
}

FResourceSpriteTableInfo* UTableManager::GetResourceSpriteRow(FName TableKey)
{
	FResourceSpriteTableInfo*	RowInfo = ResourceSpriteTableInfo->FindRow<FResourceSpriteTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceStaticMeshTableInfo* UTableManager::GetResourceStaticMeshRow(FName TableKey)
{
	FResourceStaticMeshTableInfo*	RowInfo = ResourceStaticMeshTable->FindRow<FResourceStaticMeshTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceFXTableInfo* UTableManager::GetResourceFXRow(FName TableKey)
{
	FResourceFXTableInfo*	RowInfo = ResourceFXTable->FindRow<FResourceFXTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceSoundTableInfo* UTableManager::GetResourceSoundRow(FName TableKey)
{
	FResourceSoundTableInfo*	RowInfo = ResourceSoundTableInfo->FindRow<FResourceSoundTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceSequenceTableInfo* UTableManager::GetResourceSequenceRow(FName TableKey)
{
	FResourceSequenceTableInfo*	RowInfo = ResourceSequenceTableInfo->FindRow<FResourceSequenceTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceCustomActorTableInfo* UTableManager::GetResourceCustomActorRow(FName TableKey)
{
	FResourceCustomActorTableInfo*	RowInfo = ResourceCustomActorTableInfo->FindRow<FResourceCustomActorTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FResourceCurveTableInfo* UTableManager::GetResourceCurveRow(FName TableKey)
{
	FResourceCurveTableInfo*	RowInfo = ResourceCurveTableInfo->FindRow<FResourceCurveTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FSkillCastingEffectTableInfo* UTableManager::GetSkillCastingEffectRow(FName TableKey)
{
	FSkillCastingEffectTableInfo*	RowInfo = SkillCastingEffectTableInfo->FindRow<FSkillCastingEffectTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FInventoryFilterTableInfo* UTableManager::GetInventoryFilterRow(FName TableKey)
{
	FInventoryFilterTableInfo*	RowInfo = InventoryFilterTableInfo->FindRow<FInventoryFilterTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

void UTableManager::GetDataTableRowNames(UDataTable* Table, TArray<FName>& OutRowNames)
{
	//switch
	UDataTableFunctionLibrary::GetDataTableRowNames(Table, OutRowNames);
}

void UTableManager::GetDataTableRowNamesByType(const EDataTableType InDataTableType, TArray<FName>& OutRowNames)
{
	switch (InDataTableType)
	{
	case EDataTableType::VE_CharacterTable:
		UDataTableFunctionLibrary::GetDataTableRowNames(DTCharacterTable, OutRowNames);
		break;
// 	case EDataTableType::VE_WeekDungeonTable:
// 		UDataTableFunctionLibrary::GetDataTableRowNames(CampaignEpisodeChallengeTable, OutRowNames);
// 		break;
// 	case EDataTableType::VE_AtlasmapTable:
// 		UDataTableFunctionLibrary::GetDataTableRowNames(DTAtlasmapTableInfo, OutRowNames);
// 		break;
	case EDataTableType::VE_AbnormalStateGroupTable:
		UDataTableFunctionLibrary::GetDataTableRowNames(DTAbnormalStateGroupTable_V2, OutRowNames);
		break;
	default:
		break;
	}
}

FCurrencyPropertyTableInfo*	 UTableManager::GetCurrencyPropertyRow(FName TableKey)
{
	FCurrencyPropertyTableInfo*	RowInfo = CurrencyPropertyTableInfo->FindRow<FCurrencyPropertyTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FShopProductTableInfo* UTableManager::GetShopProductRow(FName TableKey)
{
	FShopProductTableInfo*	RowInfo = ShopProductTableInfo->FindRow<FShopProductTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );

	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FInAppPurchaseTableInfo* UTableManager::GetInAppPurchaseRow(FName TableKey)
{
	FInAppPurchaseTableInfo*	RowInfo = InAppPurchaseTableInfo->FindRow<FInAppPurchaseTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	return RowInfo;
}

FShopDisplayGroupTableInfo*	UTableManager::GetShopDisplayGroupRow(FName TableKey)
{
	FShopDisplayGroupTableInfo*	RowInfo = ShopDisplayGroupTableInfo->FindRow<FShopDisplayGroupTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );

	return RowInfo;
}

FDropItemResourceInfo* UTableManager::GetDropItemResourceRow(FName TableKey)
{
	FDropItemResourceInfo* RowInfo = DropItemResourceTableInfo->FindRow<FDropItemResourceInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FDropItemParticleInfo* UTableManager::GetDropItemParticleRow(int32 nGrade)
{
	FName TableKey = FName(*FText::AsNumber(nGrade).ToString());
	FDropItemParticleInfo* RowInfo = DropItemParticleTableInfo->FindRow<FDropItemParticleInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FHeroManagementCameraTableInfo*	UTableManager::GetHeroManagementCameraRow(FName TableKey)
{
	return HeroManagementCameraTableInfo->FindRow<FHeroManagementCameraTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
}

FContentsUnlockTableInfo* UTableManager::GetContentsUnlockRow(FName TableKey)
{
	FContentsUnlockTableInfo* RowInfo =  DTContentsUnlockTable->FindRow<FContentsUnlockTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FDialogueAnimationTableInfo* UTableManager::GetDialogueAnimationTableRow(FName TableKey)
{
	return DialogueAnimationTableInfo->FindRow<FDialogueAnimationTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
}

FDialogueInfo* UTableManager::GetDialogueTableRow(FName TableKey)
{
	return DialogueTable->FindRow<FDialogueInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
}

FDialogueResource* UTableManager::GetDialogueResourceTableRow(FName TableKey)
{
	return DialogueResourceTable->FindRow<FDialogueResource>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
}

FLobbyCharacterTableInfo* UTableManager::GetLobbyCharacterTableRow(FName TableKey)
{
	return DTLobbyCharacterTableInfo->FindRow<FLobbyCharacterTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
}

FIndoorHeroManagementViewTableInfo* UTableManager::GetIndoorHeroManagementViewTableRow(FName TableKey)
{
	return DTIndoorHeroManagementViewTableInfo->FindRow<FIndoorHeroManagementViewTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
}


FTowerOptionInfo* UTableManager::GetTowerOptionTableRow(FName TableKey)
{
	return DTTowerOptionTable->FindRow<FTowerOptionInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
}

FLoadingTableInfo*	UTableManager::GetLoadingRow(FName TableKey)
{
	FLoadingTableInfo* RowInfo = LoadingTableInfo->FindRow<FLoadingTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));

	return RowInfo;
}

FTopbarTableInfo* UTableManager::GetTopbarRow(FName TableKey)
{
	FTopbarTableInfo* RowInfo = TopbarTableInfo->FindRow<FTopbarTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );

	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

//---------------------------------------------------------------------------------
// Reset cached resources
//---------------------------------------------------------------------------------
void UTableManager::ClearCachedTableAsset()
{
	TArray< FName > RowNames = DTCharacterTable->GetRowNames();
	for (FName rowName : RowNames)
	{
		FCharacterTableInfo* CharacterTableInfo = DTCharacterTable->FindRow<FCharacterTableInfo>(rowName, ContextString, USE_INVALID_TABLE_KEY_WARN );
		if (CharacterTableInfo)
			CharacterTableInfo->ResetLoadedAssetPtr();
	}
}

TArray<FName> UTableManager::GetSpiritSkillRowNames()
{
	TArray<FName> SpiritSkillArray = DTSkillTable_V2->GetRowNames();
	TArray<FName> OutArray;
	for (size_t i = 0; i < SpiritSkillArray.Num(); ++i)
	{
		if (SpiritSkillArray[i].ToString().Contains(TEXT("UTIL_Spirit")))
			OutArray.Emplace(SpiritSkillArray[i]);
	}

	return OutArray;
}

FHeroShopProductTableInfo* UTableManager::GetHeroShopProductTableInfo(FName TableKey)
{
	FHeroShopProductTableInfo* RowInfo = DTHeroShopProductTableInfo->FindRow<FHeroShopProductTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FHeroShopSynergyTableInfo* UTableManager::GetHeroShopSynergyTableInfo(FName TableKey)
{
	FHeroShopSynergyTableInfo* RowInfo = DTHeroShopSynergyTableInfo->FindRow<FHeroShopSynergyTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FHeroMovieTableInfo* UTableManager::GetHeroMovieTableInfo(FName TableKey)
{
	FHeroMovieTableInfo* RowInfo = DTHeroMovieTableInfo->FindRow<FHeroMovieTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN );
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FCrewSpotTableInfo*	UTableManager::GetCrewSpotTableRow(FName TableKey)
{
	FCrewSpotTableInfo* RowInfo = DTCrewSpotTableInfo->FindRow<FCrewSpotTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FCharacterCostUpgrade* UTableManager::GetCharacterCostUpgradeRow(FName TableKey)
{
	FCharacterCostUpgrade* RowInfo = DTCharacterCostUpgrade->FindRow<FCharacterCostUpgrade>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

void UTableManager::GetCharacterCostUpgradeWithID(FName HeroID, TArray<FCharacterCostUpgrade*>& list)
{
	TArray< FName > RowNames = DTCharacterCostUpgrade->GetRowNames();
	list.Empty(RowNames.Num());
	for (FName rowName : RowNames)
	{
		FCharacterCostUpgrade* CharacterTableInfo = DTCharacterCostUpgrade->FindRow<FCharacterCostUpgrade>(rowName, ContextString, USE_INVALID_TABLE_KEY_WARN);
		if (CharacterTableInfo)
			list.Emplace(CharacterTableInfo);
	}
}

FCharacterCostGrowth* UTableManager::GetCharacterCostGrowthRow(FName TableKey)
{
	FCharacterCostGrowth* RowInfo = DTCharacterCostGrowth->FindRow<FCharacterCostGrowth>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FWalletTableInfo* UTableManager::GetWalletRow(FName TableKey)
{
	FWalletTableInfo* RowInfo = DTWallet->FindRow<FWalletTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FWalletTableInfo* UTableManager::GetWalletRowEnum(FName TableKey)
{
	TArray< FName > RowNames = DTWallet->GetRowNames();
	for (FName rowName : RowNames)
	{
		FWalletTableInfo* Wallet = DTWallet->FindRow<FWalletTableInfo>(rowName, ContextString, USE_INVALID_TABLE_KEY_WARN);
		if (Wallet && Wallet->Enum.IsEqual(TableKey))
			return Wallet;
	}

	return nullptr;
}

FRuneTable* UTableManager::GetRuneOptionRow(FName TableKey)
{
	FRuneTable* RowInfo = DTRuneOption->FindRow<FRuneTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FRuneTable* UTableManager::GetRuneOptionRowInt(int TableKey)
{
	TArray< FName > RowNames = DTRuneOption->GetRowNames();
	for (FName rowName : RowNames)
	{
		FRuneTable* RowInfo = DTRuneOption->FindRow<FRuneTable>(rowName, ContextString, USE_INVALID_TABLE_KEY_WARN);

		if (RowInfo == nullptr)
			return nullptr;

		if(RowInfo->ID == TableKey)
			return RowInfo;
	}
	
	return nullptr;
}

FRuneWordTable* UTableManager::GetRuneWordRow(FName TableKey)
{
	FRuneWordTable* RowInfo = DTRuneWord->FindRow<FRuneWordTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}


FRuneWordSkillTable* UTableManager::GetRuneWordSkillRow(FName TableKey)
{
	FRuneWordSkillTable* RowInfo = DTRuneWordSkill->FindRow<FRuneWordSkillTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}


FRuneWordTable* UTableManager::GetRuneWordFind(TArray<FName>& TableKey)
{
	if (TableKey.Num() != 4)
		return nullptr;

	TArray< FName > RowNames = DTRuneWord->GetRowNames();
	TArray<FName> RuneWord;
	for (FName rowName : RowNames)
	{
		FRuneWordTable* RowInfo = DTRuneWord->FindRow<FRuneWordTable>(rowName, ContextString, USE_INVALID_TABLE_KEY_WARN);

		if (RowInfo == nullptr)
			return nullptr;

		bool			isReturn = true;
		
		RuneWord.Reset(4);
		RuneWord.Emplace(RowInfo->Slot01);
		RuneWord.Emplace(RowInfo->Slot02);
		RuneWord.Emplace(RowInfo->Slot03);
		RuneWord.Emplace(RowInfo->Slot04);

		for (int32 i = 0; i < 4; i++)
		{
			if (TableKey[i] != RuneWord[i])
			{
				isReturn = false;
				break;
			}
		}
		if (isReturn)
			return RowInfo;
	}
	return nullptr;
}

FTowerSkillTable* UTableManager::GetTowerSkillRow(FName TableKey)
{
	FTowerSkillTable* RowInfo = DTTowerSkill->FindRow<FTowerSkillTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FWorldBossStageInfo* UTableManager::GetWorldBossStage(FName TableKey)
{
	FWorldBossStageInfo* RowInfo = DTWorldBossStage->FindRow<FWorldBossStageInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FShopFloorStore* UTableManager::GetDTShopFloorStore(FName TableKey)
{
	FShopFloorStore* RowInfo = DTShopFloorStore->FindRow<FShopFloorStore>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FColosseumModes* UTableManager::GetColosseumModes(FName TableKey)
{
	FColosseumModes* RowInfo = DTColosseumModes->FindRow<FColosseumModes>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FBossStage* UTableManager::GetBossStage(FName TableKey)
{
	FBossStage* RowInfo = DTBossStage->FindRow<FBossStage>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FColosseumGrade* UTableManager::GetColosseumGrade(FName TableKey)
{
	FColosseumGrade* RowInfo = DTColosseumGrade->FindRow<FColosseumGrade>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FColosseumRank* UTableManager::GetColosseumRank(FName TableKey)
{
	FColosseumRank* RowInfo = DTColosseumRank->FindRow<FColosseumRank>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FGuideResourceTableInfo* UTableManager::GetGuideResourceTableRow(FName TableKey)
{
	FGuideResourceTableInfo* RowInfo = DTGuideResource->FindRow<FGuideResourceTableInfo>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	//checkf(RowInfo != nullptr, TEXT("TableKey Invalid(%s - key : %s)"), __FUNCTION__, *(TableKey.ToString()));
	return RowInfo;
}

FDailyQuestRewardsTable* UTableManager::GetDailyQuestRewardsTableRow(FName TableKey)
{
	FDailyQuestRewardsTable* RowInfo = DTDailyQuestRewards->FindRow<FDailyQuestRewardsTable>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	return RowInfo;
}

FDailyQuestContents* UTableManager::GetDailyQuestContentsTableRow(FName TableKey)
{
	FDailyQuestContents* RowInfo = DTDailyQuestContents->FindRow<FDailyQuestContents>(TableKey, ContextString, USE_INVALID_TABLE_KEY_WARN);
	return RowInfo;
}
