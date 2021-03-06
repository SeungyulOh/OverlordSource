// Generated from PacketTypeEnum.h.handlebars
#pragma once

UENUM()
enum EPacketType
{
	PACKET_ERROR = 0,
	LOGIN_RQ = 8450,
	LOGIN_RP = -8450,
	ACCOUNT_CREATE_RQ = 8451,
	ACCOUNT_CREATE_RP = -8451,
	ACCOUNT_REMOVE_RQ = 8452,
	ACCOUNT_REMOVE_RP = -8452,
	INIT_CLIENT_KEY_RQ = 8453,
	INIT_CLIENT_KEY_RP = -8453,
	ADD_REFER_PUBLISHER_RQ = 8454,
	ADD_REFER_PUBLISHER_RP = -8454,
	GET_REFER_LIST_RQ = 8455,
	GET_REFER_LIST_RP = -8455,
	ACCOUNT_CHANGE_LANGUAGE_RQ = 8456,
	ACCOUNT_CHANGE_LANGUAGE_RP = -8456,
	CLIENT_SDK_ACTION_RQ = 8457,
	CLIENT_SDK_ACTION_RP = -8457,
	ARENA_STATUS_RQ = 4800,
	ARENA_STATUS_RP = -4800,
	ARENA_TARGET_INFO_RQ = 4801,
	ARENA_TARGET_INFO_RP = -4801,
	ARENA_PLAY_RQ = 4802,
	ARENA_PLAY_RP = -4802,
	ARENA_PLAY_DONE_RQ = 4803,
	ARENA_PLAY_DONE_RP = -4803,
	ARENA_DEFENSE_REWARD_RQ = 4804,
	ARENA_DEFENSE_REWARD_RP = -4804,
	ARENA_SEASON_DONE_RQ = 4805,
	ARENA_SEASON_DONE_RP = -4805,
	ARENA_RANKING_LIST_RQ = 4806,
	ARENA_RANKING_LIST_RP = -4806,
	ARENA_DEFENSE_DECK_SET_RQ = 4807,
	ARENA_DEFENSE_DECK_SET_RP = -4807,
	AVATAR_ABILITY_UP_RQ = 7650,
	AVATAR_ABILITY_UP_RP = -7650,
	AVATAR_ABILITY_RESET_RQ = 7651,
	AVATAR_ABILITY_RESET_RP = -7651,
	AVATAR_ICON_LIST_RQ = 7652,
	AVATAR_ICON_LIST_RP = -7652,
	AVATAR_ICON_SET_RQ = 7653,
	AVATAR_ICON_SET_RP = -7653,
	AVATAR_PROFILE_RQ = 7654,
	AVATAR_PROFILE_RP = -7654,
	AVATAR_LOBBY_RQ = 7655,
	AVATAR_LOBBY_RP = -7655,
	AVATAR_GAME_DATA_RQ = 7656,
	AVATAR_GAME_DATA_RP = -7656,
	AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ = 7657,
	AVATAR_SELECTIVE_TUTORIAL_UPDATE_RP = -7657,
	CHANNEL_INFO_LIST_RQ = 13150,
	CHANNEL_INFO_LIST_RP = -13150,
	CHANNEL_DETAIL_RQ = 13151,
	CHANNEL_DETAIL_RP = -13151,
	BILLING_TRANSACTION_CREATE_RQ = 7800,
	BILLING_TRANSACTION_CREATE_RP = -7800,
	BILLING_TRANSACTION_SUCCESS_RQ = 7801,
	BILLING_TRANSACTION_SUCCESS_RP = -7801,
	COLOSSEUM_STATUS_RQ = 10450,
	COLOSSEUM_STATUS_RP = -10450,
	COLOSSEUM_PLAY_RESULT_RQ = 10451,
	COLOSSEUM_PLAY_RESULT_RP = -10451,
	COLOSSEUM_SEASON_DONE_RQ = 10452,
	COLOSSEUM_SEASON_DONE_RP = -10452,
	COLOSSEUM_RANKING_LIST_RQ = 10453,
	COLOSSEUM_RANKING_LIST_RP = -10453,
	COMMUNITY_ARTICLE_WRITE_HERO_RQ = 11100,
	COMMUNITY_ARTICLE_WRITE_HERO_RP = -11100,
	COMMUNITY_ARTICLE_DELETE_HERO_RQ = 11101,
	COMMUNITY_ARTICLE_DELETE_HERO_RP = -11101,
	COMMUNITY_ARTICLE_LIST_HERO_RQ = 11102,
	COMMUNITY_ARTICLE_LIST_HERO_RP = -11102,
	COMMUNITY_ARTICLE_VOTE_HERO_RQ = 11103,
	COMMUNITY_ARTICLE_VOTE_HERO_RP = -11103,
	CONTENT_RESET_STATUS_RQ = 15650,
	CONTENT_RESET_STATUS_RP = -15650,
	CONTENT_RESET_RQ = 15651,
	CONTENT_RESET_RP = -15651,
	ATTENDANCE_GET_RQ = 5750,
	ATTENDANCE_GET_RP = -5750,
	GUILD_CREATE_RQ = 5050,
	GUILD_CREATE_RP = -5050,
	GUILD_CLOSE_RQ = 5051,
	GUILD_CLOSE_RP = -5051,
	GUILD_GET_RQ = 5052,
	GUILD_GET_RP = -5052,
	GUILD_GET_GUEST_RQ = 5053,
	GUILD_GET_GUEST_RP = -5053,
	GUILD_MEMBER_LIST_RQ = 5054,
	GUILD_MEMBER_LIST_RP = -5054,
	GUILD_AWAITER_LIST_RQ = 5055,
	GUILD_AWAITER_LIST_RP = -5055,
	GUILD_AWAITER_ACCEPT_RQ = 5056,
	GUILD_AWAITER_ACCEPT_RP = -5056,
	GUILD_AWAITER_CANCEL_RQ = 5057,
	GUILD_AWAITER_CANCEL_RP = -5057,
	GUILD_FIND_NAME_RQ = 5058,
	GUILD_FIND_NAME_RP = -5058,
	GUILD_CHANGE_NAME_RQ = 5059,
	GUILD_CHANGE_NAME_RP = -5059,
	GUILD_EDIT_DESC_RQ = 5060,
	GUILD_EDIT_DESC_RP = -5060,
	GUILD_EDIT_NOTICE_RQ = 5061,
	GUILD_EDIT_NOTICE_RP = -5061,
	GUILD_EDIT_AUTO_GRADE_UP_RQ = 5062,
	GUILD_EDIT_AUTO_GRADE_UP_RP = -5062,
	GUILD_JOIN_RQ = 5063,
	GUILD_JOIN_RP = -5063,
	GUILD_JOIN_AUTO_RQ = 5064,
	GUILD_JOIN_AUTO_RP = -5064,
	GUILD_MEMBER_GRADE_RQ = 5065,
	GUILD_MEMBER_GRADE_RP = -5065,
	GUILD_MEMBER_KICK_RQ = 5066,
	GUILD_MEMBER_KICK_RP = -5066,
	GUILD_CONTRIBUTION_RQ = 5067,
	GUILD_CONTRIBUTION_RP = -5067,
	GUILD_CONTRIBUTE_RANKING_RQ = 5068,
	GUILD_CONTRIBUTE_RANKING_RP = -5068,
	GUILD_CONTRIBUTION_STATUS_RQ = 5069,
	GUILD_CONTRIBUTION_STATUS_RP = -5069,
	GUILD_ATTENDANCE_STATUS_RQ = 5070,
	GUILD_ATTENDANCE_STATUS_RP = -5070,
	GUILD_ATTENDANCE_DAILY_REWARD_RQ = 5071,
	GUILD_ATTENDANCE_DAILY_REWARD_RP = -5071,
	GUILD_ATTENDANCE_WEEKLY_REWARD_RQ = 5072,
	GUILD_ATTENDANCE_WEEKLY_REWARD_RP = -5072,
	GUILD_RUNE_STATUS_RQ = 5073,
	GUILD_RUNE_STATUS_RP = -5073,
	GUILD_RUNE_REQUEST_RQ = 5074,
	GUILD_RUNE_REQUEST_RP = -5074,
	GUILD_RUNE_SUPPORT_RQ = 5075,
	GUILD_RUNE_SUPPORT_RP = -5075,
	GUILD_RUNE_GET_RQ = 5076,
	GUILD_RUNE_GET_RP = -5076,
	GUILD_HISTORY_RQ = 5077,
	GUILD_HISTORY_RP = -5077,
	GUILD_LEVEL_UP_RQ = 5078,
	GUILD_LEVEL_UP_RP = -5078,
	INVENTORY_HERO_UPGRADE_RQ = 11200,
	INVENTORY_HERO_UPGRADE_RP = -11200,
	INVENTORY_HERO_LEVELUP_RQ = 11201,
	INVENTORY_HERO_LEVELUP_RP = -11201,
	INVENTORY_HERO_GRIND_RQ = 11202,
	INVENTORY_HERO_GRIND_RP = -11202,
	INVENTORY_HERO_EQUIP_RQ = 11203,
	INVENTORY_HERO_EQUIP_RP = -11203,
	INVENTORY_HERO_UNEQUIP_RQ = 11204,
	INVENTORY_HERO_UNEQUIP_RP = -11204,
	INVENTORY_ITEM_GRIND_RQ = 11205,
	INVENTORY_ITEM_GRIND_RP = -11205,
	INVENTORY_RUNE_EQUIP_RQ = 11206,
	INVENTORY_RUNE_EQUIP_RP = -11206,
	INVENTORY_RUNE_UNEQUIP_RQ = 11207,
	INVENTORY_RUNE_UNEQUIP_RP = -11207,
	INVENTORY_RUNE_MAKE_RQ = 11208,
	INVENTORY_RUNE_MAKE_RP = -11208,
	INVENTORY_ITEM_LEVELUP_RQ = 11209,
	INVENTORY_ITEM_LEVELUP_RP = -11209,
	INVENTORY_ITEM_ENCHANT_RQ = 11210,
	INVENTORY_ITEM_ENCHANT_RP = -11210,
	INVENTORY_ITEM_HERO_LOCK_RQ = 11211,
	INVENTORY_ITEM_HERO_LOCK_RP = -11211,
	INVENTORY_HERO_SKILL_LEVELUP_RQ = 11212,
	INVENTORY_HERO_SKILL_LEVELUP_RP = -11212,
	INVENTORY_HERO_ENCHANT_CONSUME_RQ = 11213,
	INVENTORY_HERO_ENCHANT_CONSUME_RP = -11213,
	INVENTORY_HERO_ENCHANT_STAT_RQ = 11214,
	INVENTORY_HERO_ENCHANT_STAT_RP = -11214,
	INVENTORY_SELL_RQ = 11215,
	INVENTORY_SELL_RP = -11215,
	INVENTORY_ITEM_UPGRADE_RQ = 11216,
	INVENTORY_ITEM_UPGRADE_RP = -11216,
	INVENTORY_ITEM_OPTION_RESET_RQ = 11217,
	INVENTORY_ITEM_OPTION_RESET_RP = -11217,
	INVENTORY_TIMETREASURE_GET_RQ = 11218,
	INVENTORY_TIMETREASURE_GET_RP = -11218,
	INVENTORY_UPGRADE_RQ = 11219,
	INVENTORY_UPGRADE_RP = -11219,
	GIFT_LIST_RQ = 8600,
	GIFT_LIST_RP = -8600,
	GIFT_OPEN_RQ = 8601,
	GIFT_OPEN_RP = -8601,
	SPECIAL_GIFT_LIST_RQ = 8602,
	SPECIAL_GIFT_LIST_RP = -8602,
	SPECIAL_GIFT_OPEN_RQ = 8603,
	SPECIAL_GIFT_OPEN_RP = -8603,
	MIRROR_STATUS_RQ = 6950,
	MIRROR_STATUS_RP = -6950,
	MIRROR_PLAY_START_RQ = 6951,
	MIRROR_PLAY_START_RP = -6951,
	MIRROR_PLAY_DONE_RQ = 6952,
	MIRROR_PLAY_DONE_RP = -6952,
	MIRROR_RESET_RQ = 6953,
	MIRROR_RESET_RP = -6953,
	MIRROR_DEFENSE_DECK_SET_RQ = 6954,
	MIRROR_DEFENSE_DECK_SET_RP = -6954,
	GIVE_ACHIEVE_REWARD_RQ = 8100,
	GIVE_ACHIEVE_REWARD_RP = -8100,
	DAILY_QUEST_STATUS_RQ = 8101,
	DAILY_QUEST_STATUS_RP = -8101,
	DAILY_QUEST_REWARD_RQ = 8102,
	DAILY_QUEST_REWARD_RP = -8102,
	SHOP_USER_DATA_RQ = 4150,
	SHOP_USER_DATA_RP = -4150,
	SHOP_BUY_RQ = 4151,
	SHOP_BUY_RP = -4151,
	SHOP_CHARACTER_SUMMON_RQ = 4152,
	SHOP_CHARACTER_SUMMON_RP = -4152,
	SHOP_FLOORSHOP_STATUS_RQ = 4153,
	SHOP_FLOORSHOP_STATUS_RP = -4153,
	SHOP_FLOORSHOP_BUY_RQ = 4154,
	SHOP_FLOORSHOP_BUY_RP = -4154,
	SHOP_FLOORSHOP_OPEN_SLOT_RQ = 4155,
	SHOP_FLOORSHOP_OPEN_SLOT_RP = -4155,
	TOWER_STATUS_RQ = 6100,
	TOWER_STATUS_RP = -6100,
	TOWER_PLAY_START_RQ = 6101,
	TOWER_PLAY_START_RP = -6101,
	TOWER_PLAY_DONE_RQ = 6102,
	TOWER_PLAY_DONE_RP = -6102,
	TOWER_TREASURE_BOX_RQ = 6103,
	TOWER_TREASURE_BOX_RP = -6103,
	TOWER_BOSS_PLAY_CHECK_RQ = 6104,
	TOWER_BOSS_PLAY_CHECK_RP = -6104,
	TOWER_BOSS_PLAY_RESULT_RQ = 6105,
	TOWER_BOSS_PLAY_RESULT_RP = -6105,
	TOWER_FLOOR_MISSION_ACTIVE_RQ = 6106,
	TOWER_FLOOR_MISSION_ACTIVE_RP = -6106,
	TOWER_FLOOR_MISSION_REWARD_RQ = 6107,
	TOWER_FLOOR_MISSION_REWARD_RP = -6107,
	TREASURE_GET_REWARD_RQ = 8850,
	TREASURE_GET_REWARD_RP = -8850,
	GET_TARGET_SERVER_RQ = 17450,
	GET_TARGET_SERVER_RP = -17450,
	WORLDBOSS_STATUS_RQ = 10850,
	WORLDBOSS_STATUS_RP = -10850,
	WORLDBOSS_PLAY_CHECK_RQ = 10851,
	WORLDBOSS_PLAY_CHECK_RP = -10851,
	WORLDBOSS_PLAY_RESULT_RQ = 10852,
	WORLDBOSS_PLAY_RESULT_RP = -10852,
};
