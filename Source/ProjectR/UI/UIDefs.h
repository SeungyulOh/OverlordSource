// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Table/CampaignTableInfo.h"
#include "UI/Control/RRichTextBlock.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "Network/PacketDataEnum.h"
#include "ToolTipList/ToolTipDefs.h"
#include "PaperSprite.h"
#include "UIDefs.generated.h"


/*
Exp  Count
*/
#define MAX_EXP_TYPE_COUNT 3

/*
Skill Texture Key
*/
#define HEROSKILLLEVELCIRCLE "Hero_Skill_Level_"
#define HEROSKILLCIRCLE		"Hero_Skill_Circle_"

/*
Color Info
*/
#define	OurTeamSkillAreaColor_Vector		FVector(0.0f, 1.0f, 0.0f)
#define EnemyTeamSkillAreaColor_Vector		FVector(1.0f, 0.0f, 0.0f)

/*
stage Clear Grade
todo : PacketDataStructure
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FSTAGE_CLEAR_GRADE_INFO
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataStructure")
	EPLAY_CLEAR_GRADE StageClearGrade;
};


#define SKILL_ICON_INDEX_1 0
#define SKILL_ICON_INDEX_2 1
#define SKILL_ICON_INDEX_3 2
#define SKILL_ICON_INDEX_4 3
#define SKILL_ICON_INDEX_5 4


/*
GameInfo
*/

#define ItemInventoryGrindMaxCount		9
#define DeckMemberMaxCount				5
#define RewardItemMaxCount				5
#define PDDeckMemberMaxCount			10
#define DeckCategoryCount				4
#define MAX_INVENTORY_AMOUNT_COUNT		60

#define MAX_HERO_ENCHANT_COUNT			30

/*
REWARD
*/

#define RewardMaxCount	2

/*
SkillUI Define Key
*/
#define StringID_SkillTargetMe					FName("SkillTargetMe")
#define StringID_SkillImmediately				FName("Skillimmediately")

#define ActiveSkillDescription_Prefix			FString("Active_")
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EUIMenuIDEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_DetailHeros			UMETA(DisplayName = "DetailHeros"),
	VE_Campaign				UMETA(DisplayName = "Campaign"),
	VE_Arena				UMETA(DisplayName = "Arena"),
	VE_FloatingWorld 		UMETA(DisplayName = "FloatingWorld"),
	VE_ItemManagement		UMETA(DisplayName = "ItemManagement"),
	VE_ItemManufacture		UMETA(DisplayName = "ItemManufacture"),
	VE_GigantRaid			UMETA(DisplayName = "GigantRaid"),
	VE_AkhanRaid			UMETA(DisplayName = "AkhanRaid"),
	VE_MorsRaid				UMETA(DisplayName = "MorsRaid"),
	VE_Shop					UMETA(DisplayName = "Shop"),
	VE_ShopHero				UMETA(DisplayName = "ShopHero"),
	VE_TimeTreasure			UMETA(DisplayName = "TimeTreasure"),
	VE_Colosseum			UMETA(DisplayName = "Colosseum"),
	VE_Crusader				UMETA(DisplayName = "Crusader"),
	VE_Guild				UMETA(DisplayName = "Guild"),
	VE_WeekDungeon			UMETA(DisplayName = "WeekDungeon"),
	VE_WeekStageSelect		UMETA(DisplayName = "WeekStageSelect"),
	VE_Raid_List			UMETA(DisplayName = "Raid_List"),
	VE_BattleField			UMETA(DisplayName = "BattleField"),
	VE_Mission				UMETA(DisplayName = "Mission"),
	VE_Quest				UMETA(DisplayName = "Quest"),
	VE_HeroRune				UMETA(DisplayName = "HeroRune"),
	VE_HeroRuneSelect		UMETA(DisplayName = "HeroRuneSelect"),
	VE_MainLobby			UMETA(DisplayName = "MainLobby"),
	VE_Mail					UMETA(DisplayName = "Mail"),
	VE_BattleGround			UMETA(DisplayName = "BattleGround"),
	VE_Avatar				UMETA(DisplayName = "Avatar"),
	VE_UltimateBattle		UMETA(DisplayName = "UltimateBattle"),
	VE_RankBattle			UMETA(DisplayName = "RankBattle"),
	VE_TreasureMap			UMETA(DisplayName = "TreasureMap"),
	VE_HeroGrind			UMETA(DisplayName = "HeroGrind"),
	VE_Item					UMETA(DisplayName = "Item"),
	VE_HeroInventory		UMETA(DisplayName = "HeroInventory"),
	VE_MirrorDungeon		UMETA(DisplayName = "MirrorDungeon"),
	VE_FloorShop			UMETA(DisplayName = "FloorShop"),
	VE_RaildWorldBoss		UMETA(DisplayName = "RaildWorldBoss"),
	VE_TowerBoss			UMETA(DisplayName = "TowerBoss"),
	VE_Achievements			UMETA(DisplayName = "Achievements"),
	VE_MiniGame				UMETA(DisplayName = "MiniGame"),
	VE_MissionSystem		UMETA(DisplayName = "MissionSystem"),
	VE_Attendance			UMETA(DisplayName = "Attendance"),
	VE_SingleRaid			UMETA(DisplayName = "SingleRaid"),
	VE_MultiRaid			UMETA(DisplayName = "MultiRaid"),

	VE_Max,
};

UENUM(BlueprintType)
enum class EBattleRepeatPopupState : uint8
{
	VE_GamePlay,
	VE_CountSelect,
	VE_ItemGainState,
	VE_FailResult_Stamina,
	VE_FailResult_Defeat,
	VE_SuccessResult,
};

UENUM(BlueprintType)
enum class EUIStackEventEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_LobbyQuest			UMETA(DisplayName = "LobbyQuest"),
};

// BP: Content/Data/DT_WidgetBluePrint
UENUM(BlueprintType)
enum class EWidgetBluePrintEnum : uint8
{
	WBP_None,

	// Login
	WBP_LoginMain,

	// Lobby
	WBP_TopBar,
	WBP_BottomBar,
	WBP_LobbyObjectName,	//	안 씀
	WBP_LobbyPDObjectName,	//	안 씀
	WBP_LobbyNotifyList,
	WBP_LobbyIndoor,
	WBP_LobbyOutdoor,
	WBP_LobbyOutdoorPortraitMode,

	// Common Widget
	WBP_WaitingPopup,
	WBP_CommonPopup,
	WBP_HeroPopup,
	WBP_RewardPopup,
	WBP_GrindRewardPopup,
	WBP_GachaRewardPopup,
	WBP_GachaRuneRewardPopup,
	WBP_BlockScreen,
	WBP_Setting,
	WBP_ContentsResetPopup,
	WBP_ChangeAttackPowerSplash,
	WBP_BottomHeroScrollView,
	WBP_GenericScrollview,
	
	// Chat
	WBP_Chat,
	WBP_Popup_ChangeChannel,

	// Campaign	
	WBP_CampaignReward,
	WBP_ResultPVE,
	WBP_ResultPVP,
	WBP_CampaignStart,
	WBP_Campaign,
	WBP_CampaignList,
	WBP_StageInfo,
	WBP_SweepResult,
	WBP_EpisodeDetail,

	WBP_WeekDungeon,
	WBP_WeekStageSelect,

	WBP_HeroRune_EpisodeDetail,	
	WBP_HeroRune_SelectHero,

	// Arena
	WBP_ArenaLobby,
	WBP_ArenaDeckSetting,
	WBP_ArenaResult,

	// HeroManagement
	WBP_HeroManagement,		//	안 씀
	WBP_Heromanagement_Renewal,
	WBP_SkillManagement,
	WBP_HeroDetail,
	WBP_HeroConfig,
	WBP_Popup_SpiritInfo,
	WBP_Popup_HeroEnchantResult,
	WBP_Popup_HeroEnchant_Modify,

	WBP_HeroRatingUp,
	WBP_HeroReinforce,

	// ItemManagement
	WBP_ItemManufacture,
	WBP_ItemOptionPopup,
	WBP_ItemEnchant,
	WBP_Bag,
	WBP_Popup_ItemInfo,
	WBP_Popup_ItemInventoryFilter,
	WBP_Popup_AmountSelect,
	WBP_Popup_BagExtend,
	WBP_Page_ItemEnchant,
	WBP_Page_ItemLevelUp,
	WBP_Page_ItemUpgrade,
	WBP_Page_ItemOptionReset,
	WBP_Page_RuneCombine,
	WBP_Page_RuneManagement,
	WBP_Splash_RuneEquipResult,
	WBP_Splash_BagExtendResult,

	// DeckSetting
	WBP_DeckManager,

	// MailBox
	WBP_MailBox,
	// Get Hero
	WBP_CaptureHero,

	// InGame
	WBP_ReadyToStart,
	WBP_PVP_ReadyToStart,
	WBP_InGameHUD_CampaignNew,
	WBP_InGameHUD_Arena,
	WBP_InGameHUD_Mirror,
	WBP_InGameSkillSetting,

	WBP_CW_HpBar,
	WBP_CW_HpBar_Small,	
	WBP_TextIndicator,
	WBP_SpeechBubble,

	// Boss
	WBP_CampaignBossSpawn,
	WBP_RaidBossSpawn,

	// Colosseum
	WBP_ColosseumLobby,
	WBP_ColosseumSetting,
	WBP_ColosseumResult,

	// Crusader
	WBP_CrusaderLobby,

	//Raid
	WBP_Raid_List,
	WBP_Raid_Select,
	WBP_Raid_Notify,

	//Quest
	WBP_Daily_Quest,


	//Shop
	WBP_Shop,
	WBP_ShopHero,

	WBP_TimeTreasure,

	//Matinee
	WBP_SkipButton,

	// BattleField
	WBP_BattleFieldLobby,
	WBP_BattleFieldResult,
	WBP_BattleFieldHeroList,

	// BattleGround
	WBP_BattleGroundLobby,
	WBP_BattleGroundHeroSelect,
	WBP_BattleGroundGuide,
	WBP_BattleGroundRewardPreview,
	// RealTime
	WBP_MatchingWaiting,
	WBP_ReSpawn,
	WBP_RealTimeResult,
	WBP_RealTimeGroup,
	WBP_TimeCount,
		
	// Unlock
	WBP_UnlockPopup,
	WBP_LockNotify,
	
	// Splash
	WBP_CommonPopupSplash,
	WBP_RewardPopupSplash,
	WBP_TreasurePopupSplash,

	// Avatar
	WBP_AvatarLevelupPopup,
	WBP_Avatar,

	// Guild
	WBP_GuildLobby,
	WBP_GuildRaid,
	WBP_GuildDonation,
	WBP_GuildModify,
	WBP_GuildLevelup,
	WBP_GuildGrade,
	WBP_GuildVisit,
	//ToolTip
	WBP_ToolTip,

	//Quest
	WBP_Quest,
	WBP_QuestArrow,

	// item popup
	WBP_Popup_ItemCraftResult,
	//WBP_Popup_ItemDetail,
	WBP_Popup_ItemEnchant,
	WBP_ItemEnchantResult,
	WBP_Popup_ItemObtain,	
	WBP_Popup_ItemSelectCategory,
	WBP_Popup_ItemSellGrindWarning,
	WBP_Popup_ItemShortage,
	WBP_Popup_ItemGrindNew,
	WBP_Popup_ItemGrind,
	WBP_Popup_ItemBatchGrindConfirm,
	WBP_Popup_WalletItemSell,
	WBP_ItemSortFilterPopup,
	WBP_ItemLevelUpPopup,
	WBP_ItemBatchGrind,
	WBP_Splash_RewardsSplash,
	WBP_Popup_WalletItemInfo,
	WBP_Splash_ItemEnchantResult,
	WBP_Splash_ItemLevelUpResult,
	WBP_Splash_ItemUpgradeResult,
	WBP_Splash_ItemOptionResetResult,

	// Enchant result 3D...
	WBP_Enchant_Result,	

	//Quick toggle button
	WBP_QuickToggle,

	// Result reward
	WBP_ResultReward,

	// Attendance
	WBP_Attendance,

	// BlockUI
	WBP_ControlBlock,
	WBP_InGame_Tuto,

	// Debug
	WBP_DebugPanel,

	// AutoPlay
	WBP_AutoPlayPopup,

	WBP_HeroGrind,
	WBP_HeroUpgradePopUp,
	WBP_ItemView,
//	WBP_HeroLevelUp,

	WBP_RuneInfo,
	WBP_RuneMix,
	WBP_HeroInventory,
	
	// mirror dungeon
	WBP_MirrorDungeon,
	WBP_MirrorDeckSetting,
	WBP_MirrorResult,

	WBP_FloorShop,

// Raid
	WBP_RaildWorldBoss,

	WBP_Campaign_One,
	WBP_Campaign_Three,
	WBP_Raid_Three,
	WBP_Raid_One,
	WBP_TowerBossLobby,


	WBP_MissionSystem,
	WBP_PartyRaidUI,

// Particle
	WBP_ParticleUI,

	WBP_Colosseum_Result,
	WBP_Raid_Result,

	WBP_GrindPopup,
	WBP_LevelUpCommonPopup,
	WBP_LevelUpCommonSplash,


	WBP_QuestCompletePopup,
	WBP_QuestPassingAlertPopup,

	WBP_DeckTest,
	WBP_SpiritDetail,
	WBP_ItemInfo,

	WBP_RaidLobby,
};

UENUM(BlueprintType)
enum class EUIPageEnum : uint8
{
	UIPage_None						UMETA(DisplayName = "None"),	
	UIPage_Campaign					UMETA(DisplayName = "Campaign"),	// deprecated
	UIPage_EpisodeDetail			UMETA(DisplayName = "EpisodeDetail"),
	UIPage_CampaignResult			UMETA(DisplayName = "CampaignResult"),
	UIPage_WeekDungeon				UMETA(DisplayName = "WeekDungeon"),
	UIPage_WeekStageSelect			UMETA(DisplayName = "WeekStageSelect"),

	UIPage_HeroRune_EpisodeDetail	UMETA(DisplayName = "UIPage_HeroRune_EpisodeDetail"),	
	UIPage_HeroRune_SelectHero		UMETA(DisplayName = "UIPage_HeroRune_SelectHero"),

	UIPage_Login					UMETA(DisplayName = "Login"),
	UIPage_LobbyOutdoor				UMETA(DisplayName = "LobbyOutdoor"),
	UIPage_LobbyOutdoorPortraitMode			UMETA(DisplayName = "LobbyOutdoorPortraitMode"),
	UIPage_LobbyIndoor				UMETA(DisplayName = "LobbyIndoor"),
	UIPage_ArenaLobby				UMETA(DisplayName = "ArenaLobby"),
	UIPage_ArenaDeckSetting			UMETA(DisplayName = "ArenaDeckSetting"),
	UIPage_HeroManagement			UMETA(DisplayName = "HeroManagement"),	//	안 씀
	UIPage_HeroManagement_Renewal	UMETA(DisplayName = "HeroManagement_Renewal"),
	UIPage_ItemManagement			UMETA(DisplayName = "ItemManagement"),
	UIPage_ItemManufacture			UMETA(DisplayName = "ItemManufacture"),
	UIPage_DeckSetting				UMETA(DisplayName = "DeckSetting"),
	UIPage_Setting					UMETA(DisplayName = "Setting"),
//	UIPage_InGameHUD_CampaignNew	UMETA(DisplayName = "InGameHUD_CampaignNew"),
	UIPage_InGameHUD_Arena			UMETA(DisplayName = "InGameHUD_Arena"),
	UIPage_InGameHUD_Mirror			UMETA(DisplayName = "InGameHUD_Mirror"),
	UIPage_ArenaResult				UMETA(DisplayName = "ArenaResult"),
	UIPage_CaptureHero				UMETA(DisplayName = "CaptureHero"),
	UIPage_MailBox					UMETA(DisplayName = "MailBox"),	
	UIPage_ColosseumLobby			UMETA(DisplayName = "ColosseumLobby"),
	UIPage_ColosseumSetting			UMETA(DisplayName = "ColosseumSetting"),
	UIPage_ColosseumResult			UMETA(DisplayName = "ColosseumResult"),
	UIPage_CrusaderLobby			UMETA(DisplayName = "CrusaderLobby"),
	UIPage_CrusaderResult			UMETA(DisplayName = "CrusaderResult"),
	UIPage_Raid_List				UMETA(DisplayName = "Raid_List"),
	UIPage_Raid_Select				UMETA(DisplayName = "Raid_Select"),
	UIPage_Daily_Quest				UMETA(DisplayName = "Daily_Quest"),
	UIPage_StageInfo				UMETA(DisplayName = "StageInfo"),
	UIPage_Shop						UMETA(DisplayName = "Shop"),
	UIPage_ShopHero					UMETA(DisplayName = "ShopHero"),
	UIPage_TimeTreasure				UMETA(DisplayName = "TimeTreasure"),
	UIPage_SweepResult				UMETA(DisplayName = "SweepResult"),
	UIPage_BattleFieldLobby			UMETA(DisplayName = "BattleFieldLobby"),
	UIPage_BattleFieldResult		UMETA(DisplayName = "BattleFieldResult"),
	UIPage_BattleFieldHeroList		UMETA(DisplayName = "BattleFieldHeroList"),
	UIPage_ContentsUnlockPopup		UMETA(DisplayName = "ContentsUnlockPopup"),
	UIPage_ContentsLockNotify		UMETA(DisplayName = "ContentsLockNotify"),
	UIPage_AvatarLevelup_Popup		UMETA(DisplayName = "AvatarLevelup_Popup"),	
	UIPage_GuildLobby				UMETA(DisplayName = "GuildLobby"),
	UIPage_GuildRaid				UMETA(DisplayName = "GuildRaid"),
	UIPage_GuildDonation			UMETA(DisplayName = "GuildDonation"),
	UIPage_EnchantResult			UMETA(DisplayName = "EnchantResult"),
	UIPage_ResultReward				UMETA(DisplayName = "ResultReward"),
	UIPage_QuestGuide				UMETA(DisplayName = "QuestGuide"),
	UIPage_BattleGroundLobby		UMETA(DisplayName = "BattleGroundLobby"),
	UIPage_BattleGroundHeroSelect	UMETA(DisplayName = "BattleGroundHeroSelect"),
	UIPage_BattleGroundGuide		UMETA(DisplayName = "BattleGroundGuide"),
	UIPage_RealTimeResult			UMETA(DisplayName = "RealTimeResult"),
	UIPage_RealTimeGroup			UMETA(DisplayName = "RealTimeGroup"),
	UIPage_Avatar					UMETA(DisplayName = "Avatar"),
	UIPage_GuildVisit				UMETA(DisplayName = "GuildVisit"),
	UIPage_HeroGrind				UMETA(DisplayName = "HeroGrind"),
	UIPage_RuneInfo					UMETA(DisplayName = "RuneInfo"),
	UIPage_RuneMix					UMETA(DisplayName = "RuneMix"),
	UIPage_HeroInventory			UMETA(DisplayName = "HeroInventory"),
	UIPage_LobbyOutside				UMETA(DisplayName = "LobbyOutside"),
	UIPage_Bag						UMETA(DisplayName = "Bag"),
	UIPage_ItemEnchant				UMETA(DisplayName = "ItemEnchant"),
	UIPage_ItemLevelUp				UMETA(DisplayName = "ItemLevelUp"),
	UIPage_ItemUpgrade				UMETA(DisplayName = "ItemUpgrade"),
	UIPage_ItemOptionReset			UMETA(DisplayName = "ItemOptionReset"),
	UIPage_ItemInfo					UMETA(DisplayName = "ItemInfo"),
	UIPage_RuneCombine				UMETA(DisplayName = "RuneCombine"),
	UIPage_RuneManagement			UMETA(DisplayName = "RuneManagement"),
	UIPage_MirrorDungeon			UMETA(DisplayName = "MirrorDungeon"),
	UIPage_MirrorDeckSetting		UMETA(DisplayName = "MirrorDeckSetting"),
	UIPage_MirrorResult				UMETA(DisplayName = "MirrorResult"),
	UIPage_FloorShop				UMETA(DisplayName = "FloorShop"),
	UIPage_RaidWorldBoss			UMETA(DisplayName = "RaidWorldBoss"),
	UIPage_InGameHUD_One			UMETA(DisplayName = "InGameHUDOne"),
	UIPage_InGameHUD_Three			UMETA(DisplayName = "InGameHUDThree"),
	UIPage_RaidHUD_Three			UMETA(DisplayName = "InRaidHUDThree"),
	UIPage_RaidHUD_One				UMETA(DisplayName = "InRaidHUDOne"),
	UIPage_TowerBossLobby			UMETA(DisplayName = "TowerBossLobby"),
	UIPage_MissionSystem			UMETA(DisplayName = "MissionSystem"),
	UIPage_PartyRaidUI				UMETA(DisplayName = "PartyRaidUI"),

	UIPage_HeroRatingUp				UMETA(DisplayName = "HeroRatingUp"),
	UIPage_HeroReinforce			UMETA(DisplayName = "HeroReinforce"),
	UIPage_SkillManagement			UMETA(DisplayName = "SkillManagement"),


	UIPage_ResultPageForRTS			UMETA(DisplayName = "RTSResult"),
	UIPage_ResultRaid				UMETA(DisplayName = "ResultRaid"),

	UIPage_MiniGame					UMETA(DisplayName = "MiniGame"),

	UIPage_GrindPopup				UMETA(DisplayName = "GrindPopup"),
	UIPage_LevelUpCommonPopup		UMETA(DisplayName = "LevelUpCommonPopup"),
	UIPage_LevelUpCommonSplash		UMETA(DisplayName = "LevelUpCommonSplash"),

	UIPage_QuestCompletePopup		UMETA(DisplayName = "QuestCompletePopup"),
	UIPage_QuestPassingAlertPopup	UMETA(DisplayName = "QuestPassingAlertPopup"),
	UIPage_DeckTest					UMETA(DisplayName = "DeckTest"),
	UIPage_SpiritDetail				UMETA(DisplayName = "SpiritDetail"),
	UIPage_MaterialDetail			UMETA(DisplayName = "MaterialDetail"),

	UIPage_RaidLobby				UMETA(DisplayName = "RaidLobby"),
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECommonSplashType : uint8
{
	VE_Normal		UMETA(DisplayName = "Normal"),
	VE_Hero			UMETA(DisplayName = "Hero"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ERewardSplashType : uint8
{
	VE_GuildRaid		UMETA(DisplayName = "GuildRaid"),
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETreasureSplashType : uint8
{
	VE_None			UMETA(DisplayName = "None"),
	VE_GetMap		UMETA(DisplayName = "GetMap"),
	VE_GetKey		UMETA(DisplayName = "GetKey"),
	VE_NeedKey		UMETA(DisplayName = "NeedKey"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGuildModifyPopupType : uint8
{
	VE_GuildName	UMETA(DisplayName = "GuildName"),
	VE_GuildDesc	UMETA(DisplayName = "GuildDesc"),
	VE_GuildNotice	UMETA(DisplayName = "GuildNotice"),
};


// USTRUCT(BlueprintType)
// struct PROJECTR_API FCampaignInfo
// {
// 	GENERATED_USTRUCT_BODY()
// 
// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCampaignInfo)
// 	FName										ChapterID;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCampaignInfo)
// 	FName										EpisodeID;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCampaignInfo)
// 	FName										StageID;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCampaignInfo)
// 	TArray<FName>								AllEpisodeIDInfo;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FCampaignInfo)
// 	TArray<FName>								AllStageIDInfo;
// 
// 
// 	FCampaignChapterTableInfo*					ChapterTableInfo = nullptr;
// 
// 	TMap<FName, FCampaignEpisodeTableInfo*>		EpisodeTableInfos;
// 	TMap<FName, FCampaignStageTableInfo*>		StageTableInfos;
// 	TMap<FName, bool>							OpendEpisodeInfos;
// 	TMap<FName, bool>							OpendStageInfos;
// 	int32										OpendStageCount = 0;
// 
// 	void Clear()
// 	{
// 		ChapterID = "";
// 		ChapterTableInfo = nullptr;
// 		EpisodeTableInfos.Empty();
// 		StageTableInfos.Empty();
// 		OpendEpisodeInfos.Empty();
// 
// 		AllEpisodeIDInfo.Empty();
// 		AllStageIDInfo.Empty();
// 	}
// };

UENUM(BlueprintType)
enum class EHeroIconSelectTypeEnum : uint8
{
	eHeroSelect_None,
	eHeroSelect_Selected,
	eHeroSelect_Using,
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_ICON_INFO
{
	GENERATED_USTRUCT_BODY()
public:
	// Server Data
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	FString		heroUD = "";
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	FName		heroId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		grade = 1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		level = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
		int32		enchantPoint = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		exp = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		acquiredExp = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		bIsOwned = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		bLock = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		bIsAbleSpawn = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		bIsDead = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		bIsUsed = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	EHeroIconSelectTypeEnum		eIconSelectType = EHeroIconSelectTypeEnum::eHeroSelect_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		bIsHp = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32					SubState = 0;
	// Table Data
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		MaxExp = 1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		MaxHP = 1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		MaxRune = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	UObject*	PortraitTextureRef = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	int32		EtcValue = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	float		HpRate = 1.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		bSpirit = false;
	/************************************************************************/
	/* ToolTip                                                              */
	/************************************************************************/
	UPROPERTY()
	EToolTipEventTypeEnum	ToolTipEventType = EToolTipEventTypeEnum::VE_None;
	UPROPERTY()
	FName					ToolTipID;

	/************************************************************************/
	/* Notify                                                              */
	/************************************************************************/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = HeroIconInfo)
	bool		IsNotify = false;
};

// For development purposes
USTRUCT(BlueprintType)
struct PROJECTR_API FOptionalUIStringKey
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOptionalUIStringKey")
	FName UIStringKey = NAME_None;
	
	// Used if UIStringKey is None
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOptionalUIStringKey")
	FText FallbackText;

	FText GetText();
};

USTRUCT(BlueprintType)
struct PROJECTR_API FITEM_ICON_INFO
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	EREWARD_TYPE	ItemCategory = EREWARD_TYPE::NONE;

	// Server Data
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	FString		itemUD = "";
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	FName		itemId = "";

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	int32		enchant = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	int32		enchantValue = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	int32		tuneCnt = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	int32		count = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	bool		bEquiped = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	bool		bCompare = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	bool		bCompareUp = true;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	int32		CompareValue = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	FName		heroId = "";

	// Table Data	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	EItemGradeEnum		itemGrade = EItemGradeEnum::Normal;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	UObject*			ItemIconRef = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	EItemEquipTypeEnum  ItemEquipType = EItemEquipTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemIconInfo)
	EItemEquipPositionEnum	ItemEquipPosition = EItemEquipPositionEnum::VE_Weapon;
	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemIconInfo)
	bool		bDisable = false;

	/************************************************************************/
	/* ToolTip                                                              */
	/************************************************************************/
	UPROPERTY()
	EToolTipEventTypeEnum	ToolTipEventType = EToolTipEventTypeEnum::VE_None;
	UPROPERTY()
	FName					ToolTipID;

	void Clear()
	{
		ItemCategory = EREWARD_TYPE::NONE;
		itemUD = "";
		itemId = FName();
		enchant = 0;
		tuneCnt = 0;
		count = 0;
		bEquiped = false;
		itemGrade = EItemGradeEnum::Normal;
		ItemIconRef = nullptr;
		ItemEquipType = EItemEquipTypeEnum::VE_None;
		ItemEquipPosition = EItemEquipPositionEnum::VE_Weapon;
	}

	bool IsValidItemData()
	{
		if (itemUD.IsEmpty() == true && itemId.IsNone() == true)
		{
			return false;
		}

		return true;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPlay_Heroes_Info
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Property)
	EDeckUsingTypeEnum	UsingType = EDeckUsingTypeEnum::VE_Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Property)
	int32	HeroMaxCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Property)
	TArray<FString> HeroUDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Property)
	int32			TotalCombatStat = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Property)
	TArray<FHERO_ICON_INFO>	HeroIconInfos;

};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBattleRecordType : uint8
{
	VE_KillCount		UMETA(DisplayName = "KillCount"),
	VE_AmountOfAttack	UMETA(DisplayName = "AmountOfAttack"),
	VE_AmountOfDamage	UMETA(DisplayName = "AmountOfDamage"),
	VE_AmountOfHeal		UMETA(DisplayName = "AmountOfHeal"),
	VE_SkillCount		UMETA(DisplayName = "SkillCount"),
	VE_RecordTypeMax	UMETA(DisplayName = "RecordTypeMax")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EStageStateType : uint8
{
	VE_None				UMETA(DisplayName = "None"),
	VE_OneStar			UMETA(DisplayName = "One star"),
	VE_TwoStar			UMETA(DisplayName = "Two star"),
	VE_ThreeStar		UMETA(DisplayName = "Three star"),
	VE_NotPlayed		UMETA(DisplayName = "Not played"),
	VE_Locked			UMETA(DisplayName = "Locked"),
};


// Mailbox Type
UENUM(BlueprintType)
enum class EMailBoxType : uint8
{
	VE_Gift		UMETA(DisplayNAme = "Gift"),
	VE_Event	UMETA(DisplayName = "Event"),
};

UENUM(BlueprintType)
enum class ESelectMaterialMode : uint8
{
	VE_HeroMaterial_Grade			UMETA("HeroMaterial_Grade"),
	VE_HeroMaterial_Land			UMETA("HeroMaterial_Land"),
};


//=====================================================================================================
// ContentsUnLock
//=====================================================================================================
UENUM(BlueprintType)
enum class EArenaCameraMode : uint8
{	
	VE_Default				UMETA(DisplayName = "Default"),
	VE_Sequence_1			UMETA(DisplayName = "Sequence_1"),
	VE_Sequence_2			UMETA(DisplayName = "Sequence_2"),	
};


//=====================================================================================================
// UIEvent
//=====================================================================================================
UENUM()
enum class EUIEventEnum : uint8
{
	VE_ItemPopup				UMETA(DisplayName = "ItemPopup"),
	VE_ItemEnchant				UMETA(DisplayName = "ItemEnchant"),
	VE_ItemGrind				UMETA(DisplayName = "ItemGrind"),
	VE_GoContent				UMETA(DisplayName = "GoContent"),
	VE_CampignDetail			UMETA(DisplayName = "CampignDetail"),
	VE_CampignDetail_Stage		UMETA(DisplayName = "CampignDetail_Stage"),
	VE_HeroDeckSettingPage		UMETA(DisplayName = "HeroDeckSettingPage"),
	VE_HeroManagement			UMETA(DisplayName = "HeroManagement"),
	VE_Shop						UMETA(DisplayName = "Shop"),
	VE_ItemInventoryPage		UMETA(DisplayName = "ItemInventoryPage"),
	VE_RuneInventoryPage		UMETA(DisplayName = "RuneInventoryPage"),
	VE_HeroShop					UMETA(DisplayName = "HeroShopPage"),
	VE_None,
};

UENUM()
enum class EShopBuyTypeEnum : uint8
{
	VE_UnLimited			= 1,
	VE_Limited_Quantity		= 2,
	VE_Limited_Daily		= 3,
	VE_Limited_Weekly		= 4,
	VE_Limited_Monthly		= 5,
};
UENUM()
enum class EStageFailReason : uint8
{
	VE_NONE,
	VE_TIMEOUT_FAIL,
	VE_LEADER_DIE_FAIL,
	VE_PROTECT_NPC_DIE_FAIL,
	VE_ALL_SURVIVAL_FAIL
};

UENUM()
enum class EStageDifficultyEnum : uint8
{
	Normal,
	Hard,
};


//////////////////////////////////////////////////////////////////////////
//Exp Info
//////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct PROJECTR_API FEXP_POTION_INFO
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEXP_POTION_INFO)
	EREWARD_TYPE	ExpPotionType = EREWARD_TYPE::NONE;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEXP_POTION_INFO)
	UObject*		Texture_Potion;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEXP_POTION_INFO)
	FText			Text_PotionName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEXP_POTION_INFO)
	int32			PotionCapacity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FEXP_POTION_INFO)
	int32			CurrentCount;
};


USTRUCT()
struct FPvPTotalHPData
{
	GENERATED_USTRUCT_BODY()

	bool		ChangedHP = false;

	int32		PrevTotalHP = 0;
	int32		CurrTotalHP = 0;
	int32		TotalHPMax = 1;
};

//////////////////////////////////////////////////////////////////////////
//Unit Def for UIFUnctionLibrary 
//////////////////////////////////////////////////////////////////////////
#define UNIT_SEC		TEXT("{sec}")
#define UNIT_YARD		TEXT("{yard}")
#define UNIT_COUNT		TEXT("{count}")
#define UNIT_PERCENT	TEXT("{percent}")

#define UNIT_SEC_CAL		TEXT("{sec_Cal}")
#define UNIT_YARD_CAL		TEXT("{yard_Cal}")
#define UNIT_COUNT_CAL		TEXT("{count_Cal}")
#define UNIT_Meter_CAL		TEXT("{m_Cal}")
#define UNIT_Percent_CAL	TEXT("{percent_Cal}")
#define UNIT_NONE_CAL		TEXT("{_Cal}")

//////////////////////////////////////////////////////////////////////////
//Texture2D RowName
//////////////////////////////////////////////////////////////////////////
#define PT_GOLD_REWARD	"PT_GOLD_REWARD"

//////////////////////////////////////////////////////////////////////////
//Texture2D RowName
//////////////////////////////////////////////////////////////////////////
#define CHARACTERUI_ORDER				-15

//////////////////////////////////////////////////////////////////////////
//UI ZOrder
//////////////////////////////////////////////////////////////////////////

#define  FORCETUTORIAL_BLOCK_ZORDER 22000
#define  TOOLTIP_ZORDER			21000
#define  COMMONPOPUP_ZORDER		20000
#define  WAITING_ZORDER			10000
#define  SPLASHPOPUP_ZORDER		 9100
#define  INGAME_NOTIFY_ZORDER	 9000
#define  AVATARLEVELUP_ZORDER	 9000
#define  REWARDPOPUP_ZORDER		 8000
#define  GACHAPOPUP_ZORDER		 8000
#define  ATTENDANCE_ZORDER		 1200
#define  MEDIA_ZOEDER			 1100
#define  DIALOGUE_ZORDER		 1000
#define  HEROSCROLLVIEW_ZORDER   900


//////////////////////////////////////////////////////////////////////////
//String
//////////////////////////////////////////////////////////////////////////
#define STRING_PATTACKPOWER		"PhysicalAttackPower"
#define STRING_MATTACKPOWER		"MagicAttackPower"


//////////////////////////////////////////////////////////////////////////
//Value
//////////////////////////////////////////////////////////////////////////
#define VALUE_TEMP				0.45f
#define VALUE_ONEHALF			0.5f


///////////////////////////////////////////////////////////////////////////
//floorshop
///////////////////////////////////////////////////////////////////////////
#define FLOORSHOP_SLOTNUM_INROW 6
#define FLOORSHOP_ROWNUM 5