// Generated from PacketFunctionLibrary.h.handlebars
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PacketDataEnum.h"
#include "Utils/JsonHelper.h"
#include "PacketFunctionLibrary.generated.h"


UCLASS()
class PROJECTR_API UPacketFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	template<typename OutStructType>
	static bool GetUStructFromJsonObject(const TSharedRef<FJsonObject>& JsonObject, OutStructType* OutStruct)
	{
		if (!FJsonHelper::JsonObjectToUStruct( JsonObject, OutStructType::StaticStruct(), OutStruct, 0, 0))
		{
			return false;
		}
		return true;
	}

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void DispatchNetMessage(URGameInstance* instance, const FString& JsonString, int32& PacketType, bool& PacketLogicalResult);


	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void LOGIN_RQ(int32 flag);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ACCOUNT_CREATE_RQ(FString nick, int32 language);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ACCOUNT_REMOVE_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INIT_CLIENT_KEY_RQ(FString publisher, FString code, FString access_token, FString redirect_uri, FString id, FString pwd);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ADD_REFER_PUBLISHER_RQ(FString publisher, FString code, FString access_token, FString redirect_uri, FString id, FString pwd);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GET_REFER_LIST_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ACCOUNT_CHANGE_LANGUAGE_RQ(int32 language);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void CLIENT_SDK_ACTION_RQ(int32 flag);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_TARGET_INFO_RQ(bool refresh);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_PLAY_RQ(int32 target_kId, TArray<FString> heroUDs, int32 power, int32 target_power);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_PLAY_DONE_RQ(FString playUD, bool isWin);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_DEFENSE_REWARD_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_SEASON_DONE_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_RANKING_LIST_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ARENA_DEFENSE_DECK_SET_RQ(TArray<FString> defense);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_ABILITY_UP_RQ(int32 abilityIndex, int32 point);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_ABILITY_RESET_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_ICON_LIST_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_ICON_SET_RQ(int32 icon);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_PROFILE_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_LOBBY_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_GAME_DATA_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ(int32 idx, int32 value);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void CHANNEL_INFO_LIST_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void CHANNEL_DETAIL_RQ(int32 no);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void BILLING_TRANSACTION_CREATE_RQ(FString purchase_system, FString product, FString redirect);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void BILLING_TRANSACTION_SUCCESS_RQ(FString purchase_system, FString receipt, FString product, FString transaction_id);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COLOSSEUM_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COLOSSEUM_PLAY_RESULT_RQ(FString roomKey);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COLOSSEUM_SEASON_DONE_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COLOSSEUM_RANKING_LIST_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COMMUNITY_ARTICLE_WRITE_HERO_RQ(FString heroId, FString contents);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COMMUNITY_ARTICLE_DELETE_HERO_RQ(FString articleId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COMMUNITY_ARTICLE_LIST_HERO_RQ(FString heroId, int32 sortType);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void COMMUNITY_ARTICLE_VOTE_HERO_RQ(FString articleId, bool recommend);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void CONTENT_RESET_STATUS_RQ(FString resetType, FString resetKey);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void CONTENT_RESET_RQ(FString resetType, FString resetKey);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void ATTENDANCE_GET_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_CREATE_RQ(FString name, FString desc, FString notice, bool auto_grade_up);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_CLOSE_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_GET_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_GET_GUEST_RQ(int32 guild_id, int32 page);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_MEMBER_LIST_RQ(int32 page);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_AWAITER_LIST_RQ(int32 page);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_AWAITER_ACCEPT_RQ(int32 target_kId, bool accept);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_AWAITER_CANCEL_RQ(int32 guild_id);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_FIND_NAME_RQ(FString name);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_CHANGE_NAME_RQ(FString name);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_EDIT_DESC_RQ(FString desc);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_EDIT_NOTICE_RQ(FString notice);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_EDIT_AUTO_GRADE_UP_RQ(bool auto_grade_up);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_JOIN_RQ(int32 guild_id);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_JOIN_AUTO_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_MEMBER_GRADE_RQ(int32 target_kId, int32 grade);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_MEMBER_KICK_RQ(int32 target_kId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_CONTRIBUTION_RQ(int32 contributionType);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_CONTRIBUTE_RANKING_RQ(int32 page);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_CONTRIBUTION_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_ATTENDANCE_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_ATTENDANCE_DAILY_REWARD_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_ATTENDANCE_WEEKLY_REWARD_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_RUNE_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_RUNE_REQUEST_RQ(FName heroId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_RUNE_SUPPORT_RQ(int32 requester_kId, FName heroId, int32 amount);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_RUNE_GET_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_HISTORY_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GUILD_LEVEL_UP_RQ(int32 level);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_UPGRADE_RQ(FString heroUD, TArray<FString> materials1, TArray<FString> materials2, TArray<FString> materials3, TArray<FString> materials4);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_LEVELUP_RQ(FString heroUD, int32 level);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_GRIND_RQ(TArray<FString> heroUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_EQUIP_RQ(FString heroUD, FString itemUD);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_UNEQUIP_RQ(FString heroUD, FString itemUD);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_ITEM_GRIND_RQ(TArray<FString> itemUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_RUNE_EQUIP_RQ(FString itemUD, TArray<FString> equipRunes);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_RUNE_UNEQUIP_RQ(FString itemUD, TArray<bool> unequipSlots);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_RUNE_MAKE_RQ(FString rune, int32 amount);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_ITEM_LEVELUP_RQ(FString itemUD, int32 amount);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_ITEM_ENCHANT_RQ(FString itemUD, bool specialEnchant, bool useProtection);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_ITEM_HERO_LOCK_RQ(TArray<FString> itemUDs_lock, TArray<FString> itemUDs_unlock, TArray<FString> heroUDs_lock, TArray<FString> heroUDs_unlock);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_SKILL_LEVELUP_RQ(FString heroUD, int32 slot);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_ENCHANT_CONSUME_RQ(FString heroUD, TArray<FString> spiritUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_HERO_ENCHANT_STAT_RQ(FString heroUD, TArray<int32> enchantStat);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_SELL_RQ(TArray<FString> itemUDs, TArray<FString> walletIdList, TArray<int32> walletQtyList);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_ITEM_UPGRADE_RQ(FString itemUD, TArray<FString> material_itemUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_ITEM_OPTION_RESET_RQ(FString itemUD, TArray<FString> material_itemUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_TIMETREASURE_GET_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void INVENTORY_UPGRADE_RQ(int32 upgrade, int32 upgradeType);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GIFT_LIST_RQ(int32 holds, int32 size);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GIFT_OPEN_RQ(TArray<FString> mailUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SPECIAL_GIFT_LIST_RQ(int32 holds, int32 size);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SPECIAL_GIFT_OPEN_RQ(TArray<FString> mailUD, int32 giftIdx);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void MIRROR_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void MIRROR_PLAY_START_RQ(FString mirrorUD, TArray<FString> heroUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void MIRROR_PLAY_DONE_RQ(FString playUD, int32 clearGrade, TArray<int32> heroHPs_hp, TArray<int32> target_heroHPs_hp);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void MIRROR_RESET_RQ(bool useGem);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void MIRROR_DEFENSE_DECK_SET_RQ(TArray<FString> defense);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GIVE_ACHIEVE_REWARD_RQ(int32 srl, int32 target_level);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void DAILY_QUEST_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void DAILY_QUEST_REWARD_RQ(int32 idx);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SHOP_USER_DATA_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SHOP_BUY_RQ(FString productId, int32 productType, int32 amount, FString currencyType, int32 price);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SHOP_CHARACTER_SUMMON_RQ(int32 summonId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SHOP_FLOORSHOP_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SHOP_FLOORSHOP_BUY_RQ(int32 shopId, int32 slotId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void SHOP_FLOORSHOP_OPEN_SLOT_RQ(int32 slotId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_PLAY_START_RQ(FString stageId, TArray<FString> heroUDs);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_PLAY_DONE_RQ(FString playUD, int32 clearGrade, int32 playTime);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_TREASURE_BOX_RQ(int32 boxIdx);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_BOSS_PLAY_CHECK_RQ(FString stageId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_BOSS_PLAY_RESULT_RQ(FString roomKey);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_FLOOR_MISSION_ACTIVE_RQ(int32 floor);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TOWER_FLOOR_MISSION_REWARD_RQ(FString id);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void TREASURE_GET_REWARD_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void GET_TARGET_SERVER_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void WORLDBOSS_STATUS_RQ();

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void WORLDBOSS_PLAY_CHECK_RQ(FString worldBossStageId);

	UFUNCTION(BlueprintCallable, Category = PacketFunction)
	static void WORLDBOSS_PLAY_RESULT_RQ(FString roomKey);

};
