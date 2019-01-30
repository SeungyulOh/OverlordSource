// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Table/CharacterTableInfo.h"
#include "Table/RaidTableInfo.h"
#include "Table/BattleObjectTableInfo.h"
#include "Table/SkillTableInfo_V2.h"
#include "Table/SkillUnlockLevelTableInfo.h"
#include "Table/SkillResourceTableInfo.h"
#include "Table/SkillConditionTableInfo.h"
#include "Table/DailyMissionTableInfo.h"
#include "Table/BulletTableInfo.h"
#include "Table/AbnormalStateGroupTableInfo_V2.h"
#include "Table/MapTableInfo.h"
#include "Table/MapPostProcessSettingInfo.h"
#include "Table/ItemTableInfo.h"
#include "Table/CampaignTableInfo.h"
#include "Table/RewardPreviewTableInfo.h"
#include "Table/RewardPreviewItemTableInfo.h"
#include "Table/FloorQuestTableInfo.h"
#include "Table/WidgetBluePrintTableInfo.h"
#include "Table/LevelGrowth.h"
#include "Table/StringTableInfo.h"
#include "Table/SkillLevelUpCostTableInfo.h"
#include "Table/AchievementTableInfo.h"
#include "Table/GrowthTables/GrowthHeroInfo.h"
#include "Table/GrowthTables/GrowthHeroMultiplier.h"
#include "Table/GrowthTables/BattlePowerTableInfo.h"
#include "Table/ResourceMaterialTableInfo.h"
#include "Table/ResourceParticleTableInfo.h"
#include "Table/ResourceTexture2DTableInfo.h"
#include "Table/ResourceSpriteTableInfo.h"
#include "Table/ResourceStaticMeshTableInfo.h"
#include "Table/ResourceFXTableInfo.h"
#include "Table/ResourceSoundTableInfo.h"
#include "Table/ResourceSequenceTableInfo.h"
#include "Table/ResourceCustomActorTableInfo.h"
#include "Table/ResourceCurveTableInfo.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "Table/HPProgressInfo.h"
#include "Table/HeroManagementCameraTableInfo.h"
#include "Table/ShopProductTableInfo.h"
#include "Table/InAppPurchaseTableInfo.h"
#include "Table/ShopDisplayGroupTableInfo.h"
#include "Table/ContentsUnlockTableInfo.h"
#include "Table/ChatStringAttributeTableInfo.h"
#include "Table/ItemGradeTableInfo.h"
#include "Table/ContentsUITableInfo.h"
#include "Table/ItemEquipTypeTableInfo.h"
#include "Table/ResourceDialogueTableInfo.h"
#include "Table/LoadingTableInfo.h"
#include "Table/TopbarTableInfo.h"
#include "Table/GrindReturnMaterial.h"
#include "Table/MediaTableInfo.h"
#include "Table/GameModeSequenceTableInfo.h"
#include "Table/GuildTableInfo.h"
#include "Table/RaidBossTableInfo.h"
#include "Table/RaidBossAttackTableInfo.h"
#include "Table/RaidBossTermsTableInfo.h"
#include "Table/AttendanceTableInfo.h"
#include "Table/HeroShopTableInfo.h"
#include "Table/HeroMovieTableInfo.h"
#include "Table/BattleGroundTableInfo.h"
#include "Table/BattleGuildRaidTableInfo.h"
#include "Table/GuildRaidRewardTableInfo.h"
#include "Table/SubtitleTableInfo.h"
#include "Table/ContentResetTableInfo.h"
#include "Table/BattleLogicConfigTable.h"
#include "Table/ConstantVariablesTableInfo.h"
#include "Table/StaminaUseTableInfo.h"
#include "Table/ForceTutorialTableInfo.h"
#include "Table/AvatarAbilityTableInfo.h"
#include "Table/DialogueAnimationTableInfo.h"
#include "Table/LobbyCharacterTableInfo.h"
#include "Table/IndoorHeroManagementViewTableInfo.h"
#include "Table/ItemLevelUpTableInfo.h"
#include "Table/ItemPassiveSkillTableInfo.h"
#include "Table/CrewSpotTableInfo.h"
#include "Table/SkillCastingEffectTableInfo.h"
#include "Table/InventoryFilterTableInfo.h"
#include "Table/SpiritSkill.h"
#include "Table/TimeTreasureInfos.h"
#include "Table/DropItemResourceInfo.h"
#include "Table/ColosseumTableInfo.h"
#include "Table/TowerSkillTableInfo.h"
#include "Table/GuideResourceTableInfo.h"
#include "Table/DailyQuestRewardsTable.h"
#include "Table/DailyQuestContentsTable.h"
#include "Table/FloorMissionTable.h"
#include "Table/CharacterCostEnchantGrade.h"
#include "Table/RaidStageTableInfo.h"

#include "CustomStructures/DialogueInfos.h"
#include "CustomStructures/GachaInfos.h"

#include "ResourceCacheManager.h"


#include "TableManager.generated.h"

UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	VE_CharacterTable			UMETA(DisPlayname = "CharacterTable"),
	VE_HeroCardTable			UMETA(DisPlayname = "Hero card"),
	VE_WeekDungeonTable			UMETA(DisPlayname = "WeekDungeon"),
	VE_AtlasmapTable			UMETA(DisPlayname = "Atlas map"),
	VE_AbnormalStateGroupTable	UMETA(DisPlayname = "Abnormal state group"),
	VE_None,
};


/**
 * 

 !!!!!!!! Check calling order!!!!!


 */ 

UCLASS(BlueprintType)
class PROJECTR_API UTableManager : public UObject
{
	GENERATED_BODY()
		
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTSkillTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTSkillTable_V2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTItemPassiveSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTSkillResourceTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTSkillNPCTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTSkillConditionTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTDailyMissionTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTDailyQuestRewards;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTFloorMissionTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTBulletTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTCharacterTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTCharacterNpcTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTCharacterCostEnchant;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
		UDataTable* DTCharacterCostEnchantGrade;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTBattleObjectTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTMapTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTMapPostProcessSettingTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTGameModeSequenceTableInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTAbnormalStateGroupTable_V2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTCharacterItemTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTItemRandomOptionTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTItemUpgradeCostTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTItemLevelUpTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTShopCharacterSummon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* FloorStageTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* FloorInfoTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* TimeTreasureTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* RewardPreviewTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* RewardPreviewItemTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tablemanager)
	UDataTable* FloorQuestTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* WidgetBluePrintTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* LevelGrowthTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* SkillLevelUpCostTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTItemManufactureTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTItemEnchantTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTAchievementTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTContentsUnlockTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTGuildLevelupTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTGuildContributeTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTRaidStageTableInfo;

	/*
	RaidBoss Table
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTRaidBossTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTRaidBossAttackTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTRaidBossTermsTableInfo;

	/*
	BattleGround
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTBattleGroundTableInfo;

	/*
	Guild raid
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTBattleGuildRaidTableInfo;

	/*
	Guild raid reward
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTGuildRaidRewardTableInfo;

	/*
	Character growth tables
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	NPCGrowTables;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	HeroGrowthMultiplierTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	TMap<FName, UDataTable*> HeroGrowthTables;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	TMap<EClassTypeEnum, UDataTable*> HeroEnchantTables;
	
	/*
	PVP StaminaUse table
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* PVPStaminaUseTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* HPProgressInfoTable;

	/*
	Avatar Ability table
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTAvatarAbilityTableInfo;

	/*
	Resource table
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* ResourceCacheTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* ResourceParticleTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* ResourceMaterialTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* ResourceTexture2DTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* ResourceDamageFloaterTex2DTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* ResourceStaticMeshTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* ResourceFXTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ResourceSoundTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ResourceSequenceTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ResourceCustomActorTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ResourceCurveTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	CurrencyPropertyTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ShopProductTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	InAppPurchaseTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ShopDisplayGroupTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ResourceDialogueTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	ResourceSpriteTableInfo;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	SkillCastingEffectTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	InventoryFilterTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DropItemResourceTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DropItemParticleTableInfo;
	/*
	attendance
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTAttendanceTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTHeroShopProductTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTHeroShopSynergyTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTHeroMovieTableInfo;

	
	//by cowspirit
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	LoadingTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* TopbarTableInfo;

	/*
	Chat
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTChatStringAttributeTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTItemGradeTableInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTContentsUITableInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTItemEquipTypeTableInfo;

	//by danzang
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	HeroManagementCameraTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTMediaTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTSubtitleTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTContentResetTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTBattleLogicConfigTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTConstantVariablesTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTBattlePowerTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTForceTutorialTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTForceTutorialDetailTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DialogueAnimationTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DialogueTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DialogueResourceTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTLobbyCharacterTableInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTIndoorHeroManagementViewTableInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTTowerOptionTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTCrewSpotTableInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTCharacterCostUpgrade;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTCharacterCostGrowth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTWallet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTRuneOption;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTRuneWord;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTRuneWordSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTTowerSkill;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTWorldBossStage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTColosseumModes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTShopFloorStore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTBossStage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTColosseumGrade;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTColosseumRank;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable* DTGuideResource;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TableManager)
	UDataTable*	DTDailyQuestContents;

public:
	UFUNCTION(BlueprintCallable, Category = TableManager)
	static UTableManager* GetInstancePtr();

	UFUNCTION(BlueprintCallable, Category = TableManager)
	static void	ReleaseSingleton();

	// use blueprint function.
	UFUNCTION(BlueprintCallable, Category = TableManager)
	void InitDataTable();
		
	UDataTable* LoadTableFromPath(const FName& Path);
	

	FCharacterTableInfo*			GetCharacterRow(FName TableKey);
	FCharacterTableInfo*			GetCharacterNpcRow(FName TableKey);
	FBattleObjectTableInfo*			GetBattleObjectRow(FName TableKey);	
	
	FSkillTableInfo_V2*				GetSkillRow_V2(FName TableKey);
	FItemPassiveSkillTableInfo*		GetItemPassiveSkillRow(FName TableKey);
	FSkillResourceTableInfo*		GetSkillResourceRow(FName TableKey);	
	FSkillConditionTableInfo*		GetSkillConditionRow(FName TableKey);

	FDailyMissionTableInfo*			GetDailyMissionTable(FName TableKey);
	FFloorMissionTable*			GetFloorMissionTable(FName TableKey);
	FBulletTableInfo*				GetBulletRow(FName TableKey);	
	FMapTableInfo*					GetMapRow(FName TableKey);
	FMapPostProcessSettingInfo*		GetMapPostProcessSettingRow(FName TableKey);
	FGameModeSequenceTableInfo*		GetGameModeSequenceRow(FName TableKey);
	FAbnormalStateGroupTableInfo_V2*	GetAbnormalStateGroupRow_V2(FName TableKey);
	FItemTableInfo*					GetCharacterItemRow(FName TableKey);
	FItemRandomOptionTableInfo*		GetItemRandomOptionRow(FName TableKey);
	FItemUpgradeCostInfo*		GetItemUpgradeCostRow(FName TableKey);
	FItemLevelUpTableInfo*			GetItemLevelUpTableRow(FName TableKey);
	FGachaInfo*						GetShopCharacterSummonTableRow(FName TableKey);

	FFloorStageDetail*				GetFloorStageDetailRow(FName TableKey);
	FFloorInfo*						GetFloorInfoRow(FName TableKey);
	FTimeTreasureTable*				GetTimeTreasureTableRow(FName TableKey);
	
	FRewardPreviewTableInfo*		GetRewardPreviewRow(FName TableKey);
	FRewardPreviewItemTableInfo*    GetRewardPreviewItemRow(FName TableKey);

	FFloorQuestTableInfo*			GetFloorQuestRow(FName TableKey);

	FWidgetBluePrintTableInfo*		GetWidgetBluePrintRow(FName TableKey);

	FAchievementTableInfo*			GetAchievementRow(FName TableKey);
	TArray<FName>					GetAchievementRowsNames();

	FSkillLevelUpCostTableInfo*		GetSkillLevelUpCostRow(FName TableKey);
	FItemEnchantTableInfo*			GetItemEnchantRow(FName TableKey);
	FItemManufatureTableInfo*		GetItemManufactureRow(FName TableKey);
	
	FGuildLevelUpTableInfo*			GetGuildLevelUpRow(FName TableKey);
	FGuildContributeTableInfo*		GetGuildContributeRow(FName TableKey);

	FRaidStageTableInfo*			GetRaidStageTableRow(FName TableKey);

	FResourceMaterialTableInfo*		GetResourceMaterialRow(FName TableKey);
	FResourceCacheTable*			GetResourceCacheRow(FName TableKey);
	FResourceParticleTableInfo*		GetResourceParticleRow(FName TableKey);
	FResourceTexture2DTableInfo*	GetResourceTexture2DRow(FName TableKey);
	FResourceTexture2DTableInfo*	GetResourceDamageFloaterTex2DRow(FName TableKey);
	FResourceSpriteTableInfo*		GetResourceSpriteRow(FName TableKey);
	FSkillCastingEffectTableInfo*	GetSkillCastingEffectRow(FName TableKey);
	FInventoryFilterTableInfo*		GetInventoryFilterRow(FName TableKey);
	FResourceStaticMeshTableInfo*	GetResourceStaticMeshRow(FName TableKey);
	FResourceFXTableInfo*			GetResourceFXRow(FName TableKey);
	FResourceSoundTableInfo*		GetResourceSoundRow(FName TableKey);
	FResourceSequenceTableInfo*		GetResourceSequenceRow(FName TableKey);
	FResourceCustomActorTableInfo*	GetResourceCustomActorRow(FName TableKey);
	FResourceCurveTableInfo*		GetResourceCurveRow(FName TableKey);
	FCurrencyPropertyTableInfo*		GetCurrencyPropertyRow(FName TableKey);
	FShopProductTableInfo*			GetShopProductRow(FName TableKey);
	FInAppPurchaseTableInfo*		GetInAppPurchaseRow(FName TableKey);
	FShopDisplayGroupTableInfo*		GetShopDisplayGroupRow(FName TableKey);

	FDropItemResourceInfo*			GetDropItemResourceRow(FName TableKey);
	FDropItemParticleInfo*			GetDropItemParticleRow(int32 nGrade);

	FHeroManagementCameraTableInfo*	GetHeroManagementCameraRow(FName TableKey);
	
	FDialogueAnimationTableInfo*	GetDialogueAnimationTableRow(FName TableKey);
	FDialogueInfo*					GetDialogueTableRow(FName TableKey);
	FDialogueResource*				GetDialogueResourceTableRow(FName TableKey);



	FLobbyCharacterTableInfo*		GetLobbyCharacterTableRow(FName TableKey);
	FIndoorHeroManagementViewTableInfo* GetIndoorHeroManagementViewTableRow(FName TableKey);

	FTowerOptionInfo*				GetTowerOptionTableRow(FName TableKey);
	
	FContentsUnlockTableInfo*		GetContentsUnlockRow(FName TableKey);
	//by cowspirit
	FLoadingTableInfo*				GetLoadingRow(FName TableKey);	

	FTopbarTableInfo*				GetTopbarRow(FName TableKey);

	FContentResetTableInfo*			GetContentResetTableRow(FName TableKey);

	FBattleLogicConfigTable*		GetBattleLogicConfigTableRow(FName TableKey);
	float							GetBattleLogicConfigValue(FName TableKey);

	FConstantVariablesTableInfo*	GetConstantVariablesRow(FName TableKey);

	FBattlePowerTable*				GetBattlePowerRow(int32 InLevel);

	// Chat
	FChatStringAttributeTableInfo*	GetChatStringAttributeTableRow(FName TableKey);

	FItemGradeTableInfo*			GetItemGradeTableRow(FName TableKey);

	FContentsUITableInfo*			GetContentsUITableRow(FName TableKey);
	FItemEquipTypeTableInfo*		GetItemEquipTypeTableRow(FName TableKey);

	/*
	Character growth tables
	*/
	FLevelGrowthTableInfo*			GetLevelGrowthRow(FName TableKey);
	const UDataTable*				GetHeroGrowthTable(FName TableKey);
	const UDataTable*				GetHeroEnchantTable(EClassTypeEnum TableKey);
	
	static FGrowthHeroInfo_V2*		GetHeroGrowthInfo(const UDataTable* InHeroGrowTable, int32 level);
	static FHeroEnchant_V2*			GetHeroEnchantInfo(const UDataTable* InHeroEnchantTable, int32 point);
	FGrowthHeroInfo*				GetNPCGrowInfo(FName TableKey);
	FGrowthHeroMultiplier*			GetHeroGrowthMultiplierRow(FName TableKey);
	FCharacterCostEnchant*			GetCharacterCostEnchant(FName TableKey);
	FCharacterCostEnchantGrade*		GetCharacterCostEnchantGradeInfo(FName TableKey);
	
	/*
	Media Path Table
	*/
	FMediaTableInfo*			GetMediaTableRow(FName TableKey);

	/*
	Media Subtitle Table
	*/
	FSubtitleTableInfo*			GetSubtitleTableRow(FName TableKey);


	/*
	Raid Boss table
	*/
	FRaidBossTableInfo*			GetRaidBossTableRow(FName TableKey);
	FRaidBossAttackTableInfo*	GetRaidBossAttackTableRow(FName TableKey);
	FRaidBossTermsTableInfo*	GetRaidBossTermsTableRow(FName TableKey);

	/*
	BattleGround
	*/
	FBattleGroundTableInfo*			GetBattleGroundTableRow(FName TableKey);

	/*
	Battle GuildRaid
	*/
	FBattleGuildRaidTableInfo*	GetBattleGuildRaidTableRow(FName TableKey);

	/*
	GuildRaid Raid
	*/
	FGuildRaidRewardTableInfo*	GetGuildRaidRewardTableRow(FName TableKey);


	/*
	Attendance
	*/
	FAttendanceTableInfo*		GetAttendanceTableRow(FName TableKey);



	/*
	Boss table
	*/
	FHPProgressInfo* GetHPProgressInfoTableRow(FName TableKey);
	
	/*
	PVP StaminaUse table
	*/
	FStaminaUseTableInfo* GetStaminaUseTableRow(FName TableKey);
	
	FForceTutorialTableInfo*		GetForceTutorialTableRow(FName TableKey);
	FForceTutorialDetailTableInfo*	GetForceTutorialDetailTableRow(FName TableKey);

	/*
	Avatar Ability table
	*/
	FAvatarAbilityTableInfo*	GetAvatarAbilityTableRow(FName TableKey);

	FCrewSpotTableInfo*			GetCrewSpotTableRow(FName TableKey);

	FCharacterCostUpgrade*		GetCharacterCostUpgradeRow(FName TableKey);
	void GetCharacterCostUpgradeWithID(FName HeroID, TArray<FCharacterCostUpgrade*>& list);
	FCharacterCostGrowth*		GetCharacterCostGrowthRow(FName TableKey);
	FWalletTableInfo*			GetWalletRow(FName TableKey);
	FWalletTableInfo*			GetWalletRowEnum(FName TableKey);

	FRuneTable*					GetRuneOptionRow(FName TableKey);
	FRuneTable*					GetRuneOptionRowInt(int TableKey);

	FRuneWordTable*				GetRuneWordRow(FName TableKey);
	FRuneWordSkillTable*		GetRuneWordSkillRow(FName TableKey);
	FRuneWordTable*				GetRuneWordFind(TArray<FName>& TableKey);
	FTowerSkillTable*			GetTowerSkillRow(FName TableKey);

	FWorldBossStageInfo*		GetWorldBossStage(FName TableKey);
	FShopFloorStore*			GetDTShopFloorStore(FName TableKey);
	FColosseumModes*			GetColosseumModes(FName TableKey);
	FBossStage*					GetBossStage(FName TableKey);
	FColosseumGrade*			GetColosseumGrade(FName TableKey);
	FColosseumRank*				GetColosseumRank(FName TableKey);
	
	FGuideResourceTableInfo*	GetGuideResourceTableRow(FName TableKey);
	FDailyQuestRewardsTable*	GetDailyQuestRewardsTableRow(FName TableKey);
	FDailyQuestContents*		GetDailyQuestContentsTableRow(FName TableKey);

	FORCEINLINE TArray<FName> GetWorldBossStageNames()				{ return DTWorldBossStage->GetRowNames(); }
	FORCEINLINE TArray<FName> GetCharacterRowNames()				{ return DTCharacterTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetCharacterNpcRowNames()				{ return DTCharacterNpcTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetHeroCostEnchantRowNames()			{ return DTCharacterCostEnchant->GetRowNames(); }
	FORCEINLINE TArray<FName> GetBattleObjectRowNames()				{ return DTBattleObjectTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetCharacterItemRowNames()			{ return DTCharacterItemTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetMapRowNames()						{ return DTMapTable->GetRowNames();	}
	FORCEINLINE TArray<FName> GetMapPostProcessSettingRowNames()	{ return DTMapPostProcessSettingTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetGameModeSequenceRowNames()			{ return DTGameModeSequenceTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetHPProgressInfoRowNames()			{ return HPProgressInfoTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceStaticMeshRowNames()		{ return ResourceStaticMeshTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceCacheRowNames()			{ return ResourceCacheTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceParticleRowNames()			{ return ResourceParticleTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceMaterialRowNames()			{ return ResourceMaterialTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceSequenceRowNames()			{ return ResourceSequenceTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceCustomActorRowNames()		{ return ResourceCustomActorTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceSpriteRowNames()			{ return ResourceSpriteTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceDamageFloater2DRowNames()  { return ResourceDamageFloaterTex2DTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetWidgetBluePrintRowNames()			{ return WidgetBluePrintTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetContentResetRowNames()				{ return DTContentResetTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetBattleLogicRowNames()				{ return DTBattleLogicConfigTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetConstantVariablesRowNames()		{ return DTConstantVariablesTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetSkillLvUpCostRowNames()			{ return SkillLevelUpCostTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetShopCharacterSummonRowNames()		{ return DTShopCharacterSummon->GetRowNames(); }
	FORCEINLINE TArray<FName> GetDropItemRowNames()					{ return DropItemResourceTableInfo->GetRowNames(); }

	FORCEINLINE	TArray<FName> GetDailyQuestRewardsRowsNames()		{ return DTDailyQuestRewards->GetRowNames(); }

	FORCEINLINE TArray<FName> GetFloorStageRowNames()				{ return FloorStageTable->GetRowNames(); }
	
	FORCEINLINE TArray<FName> GetItemManufactureRowNames()			{ return DTItemManufactureTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceDialogueTableRowNames()	{ return ResourceDialogueTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetContentsUnlockRowNames()			{ return DTContentsUnlockTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetLoadingRowsNames()					{ return LoadingTableInfo->GetRowNames();}
	FORCEINLINE TArray<FName> GetFloorQuestRowsNames()				{ return FloorQuestTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetGuildLevelupRowsNames()			{ return DTGuildLevelupTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetGuildContributeRowsNames()			{ return DTGuildContributeTable->GetRowNames(); }
	
	FORCEINLINE TArray<FName> GetRaidStageRowNames()				{ return DTRaidStageTableInfo->GetRowNames(); }

	FORCEINLINE TArray<FName> GetMediaTableRowsNames()				{ return DTMediaTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetSubtitleTableRowsNames()			{ return DTSubtitleTableInfo->GetRowNames(); }


	FORCEINLINE TArray<FName> GetCurrencyPropertyRowNames()			{ return CurrencyPropertyTableInfo->GetRowNames(); }

	FORCEINLINE TArray<FName> GetLobbyCharacterTableRowNames()		{ return DTLobbyCharacterTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetIndoorHeroManagementViewTableRowNames() { return DTIndoorHeroManagementViewTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetResourceFXTableRowNames() { return ResourceFXTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetBulletTableInfoRowNames() { return DTBulletTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetAbnormalTableInfoRowNames() { return DTAbnormalStateGroupTable_V2->GetRowNames(); }
	FORCEINLINE TArray<FName> GetSkillCastingTableInfoRowNames() { return SkillCastingEffectTableInfo->GetRowNames(); }
	FORCEINLINE	TArray<FName> GetWalletRowNames() { return DTWallet->GetRowNames(); }
	
	/*
	Raid Boss table
	*/
	FORCEINLINE TArray<FName> GetRaidBossRowsNames() { return DTRaidBossTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetRaidBossAttackRowsNames() { return DTRaidBossAttackTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetRaidBossTermsRowsNames() { return DTRaidBossTermsTableInfo->GetRowNames(); }


	/*
	BattleGround
	*/
	FORCEINLINE TArray<FName> GetBattleGroundRowsNames() { return DTBattleGroundTableInfo->GetRowNames(); }

	/*
	Battle GuildRaid
	*/
	FORCEINLINE TArray<FName> GetBattleGuildRaidRowsNames() { return DTBattleGuildRaidTableInfo->GetRowNames(); }


	/*
	GuildRaid Reward
	*/
	FORCEINLINE TArray<FName> GetGuildRaidRewardRowsNames() { return DTGuildRaidRewardTableInfo->GetRowNames(); }


	/*
	Attendance
	*/
	FORCEINLINE TArray<FName> GetAttendanceRowsNames() { return DTAttendanceTableInfo->GetRowNames(); }

	FORCEINLINE TArray<FName> GetForceTutorialRowsNames() { return DTForceTutorialTable->GetRowNames(); }
	FORCEINLINE TArray<FName> GetForceTutorialDetailRowsNames() { return DTForceTutorialDetailTable->GetRowNames(); }

	TArray<FName> GetSpiritSkillRowNames();

	
	/*
	HeroShop
	*/
	FHeroShopProductTableInfo*	GetHeroShopProductTableInfo(FName TableKey);
	FHeroShopSynergyTableInfo*	GetHeroShopSynergyTableInfo(FName TableKey);
	FHeroMovieTableInfo*		GetHeroMovieTableInfo(FName TableKey);

	FORCEINLINE TArray<FName> GetHeroShopProductRowsNames() { return DTHeroShopProductTableInfo->GetRowNames(); }
	FORCEINLINE TArray<FName> GetHeroShopSynergyRowsNames() { return DTHeroShopSynergyTableInfo->GetRowNames(); }
	TArray<FName> GetHeroMovieRowsNames() { return DTHeroMovieTableInfo->GetRowNames(); }
	
	FORCEINLINE TArray<FName> GetDailyQuestContentsRowsNames() { return DTDailyQuestContents->GetRowNames(); }

	void GetDataTableRowNames(UDataTable* Table, TArray<FName>& OutRowNames);
	void GetDataTableRowNamesByType(const EDataTableType InDataTableType, TArray<FName>& OutRowNames);

	void UnloadTable(UDataTable*& table);
	void UnloadAllTables();
	void UnMemAllTables();
	void ReloadAllTables();
	//---------------------------------------------------------------------------------
	// Reset cached resources
	//---------------------------------------------------------------------------------
	void ClearCachedTableAsset();

private:
	UTableManager();

	static UTableManager*	InstancePtr;

	TArray< UDataTable* >	LoadedTables;
};
