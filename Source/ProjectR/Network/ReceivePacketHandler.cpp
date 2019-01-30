#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"

#include "GameInfo/RewardInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/GuildInfo.h"
#include "GameState/TowerLobbyState.h"

#include "Global/ShopManager.h"

#include "UI/RWidgetManager.h"
#include "UI/UIDefs.h"
#include "UI/UI_Common/UP_RewardPopup.h"
#include "UI/Arena/UP_Arena_Lobby.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "UI/ItemManagement/UC_GenericPopup.h"
#include "UI/ItemManagement/UC_RewardsSplash.h"
#include "UI/ItemManagement/UP_GenericSplash.h"
#include "UI/UI_Campaign/UP_Campaign_Tower.h"
#include "UI/UI_Campaign/UP_CampaignTower_Scrollview.h"
#include "UI/UI_Campaign/UP_StageSelect.h"
#include "UI/UI_Battle/RCampaignHUD_RaidThreeControl.h"
#include "Global/ColosseumManager.h"

#include "ReceivePacketHandler.h"
#include "PacketFunctionLibrary.h"
#include "BinaryPacketFunctionLibrary.h"
#include "PacketDataStructures.h"
#include "UtilFunctionIntegrated.h"
#include "HttpRequestClient.h"
#include "Network/ClientProtocols.h"
#include "Lobby/TowerLobbyLevelScriptActor.h"

#include "UpdateService/Auth/RAuth.h"

#if WITH_EDITOR
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#endif
#include "UIFunctionLibrary.h"
#include "UI/UI_Lobby/UP_Lobby_Outdoor.h"
#include "UI/DeckManagement/ASyncPVPDeckSetting.h"


//////////////////////////////////////////////////////////////////////////////////////////
// Account
//////////////////////////////////////////////////////////////////////////////////////////

bool UReceivePacketHandler::OnLOGIN_RP(FLOGIN_RP& login_rp, URGameInstance* instance)
{
	bool loginReplySuccess = URAuth::GetInstancePtr()->OnLOGIN_RP(login_rp);

	if (!loginReplySuccess)
	{
		instance->HttpClient->SetBusy(false);
	}
	

	return loginReplySuccess;
}

bool UReceivePacketHandler::OnACCOUNT_CREATE_RP(FACCOUNT_CREATE_RP& account_create_rp, URGameInstance* instance)
{
	return URAuth::GetInstancePtr()->OnACCOUNT_CREATE_RP(account_create_rp);
}

bool UReceivePacketHandler::OnAVATAR_GAME_DATA_RP(FAVATAR_GAME_DATA_RP& avatar_game_data_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_game_data_rp.type, avatar_game_data_rp.is, avatar_game_data_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->ContentsUnLockManager->SetUnlockList(avatar_game_data_rp.unlockList);
	instance->HttpClient->SetKID(avatar_game_data_rp.kId);
	instance->UserInfo->kId = avatar_game_data_rp.kId;
	instance->UserInfo->SetAvatarData(avatar_game_data_rp.avatar);
	instance->GuildInfo->SetGuildMine(avatar_game_data_rp.guild_mine);
	instance->AttendanceManager->SetAttendanceInfo(avatar_game_data_rp.chart);
	instance->AchievementManager->SetAchievementStatus(avatar_game_data_rp.achieveStatus);
	//instance->ForceTutorialManager->nCurrentTutorialStep = avatar_game_data_rp.inventory.tut;
	instance->RInventory->Clear();
	instance->RInventory->InitInventory(avatar_game_data_rp.inventory);
	instance->QuestManager->SetDailyQuestStatus(avatar_game_data_rp.dailyQuestStatus);
	instance->ContentsUnLockManager->SetFirstEnterArr(avatar_game_data_rp.selectiveTutorial.flags);
	instance->CampaignManager->SetTowerStatus(avatar_game_data_rp.towerStatus);
	instance->MissionManager->SetMissionStatus(avatar_game_data_rp.towerStatus.floorMissionStatus);
	instance->ColosseumManager->ColosseumStatus.gradeId = avatar_game_data_rp.colosseumGradeId;
	return true;
}


bool UReceivePacketHandler::OnAVATAR_SELECTIVE_TUTORIAL_UPDATE_RP(FAVATAR_SELECTIVE_TUTORIAL_UPDATE_RP& avatar_selective_tutorial_update_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_selective_tutorial_update_rp.type, avatar_selective_tutorial_update_rp.is, avatar_selective_tutorial_update_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->ContentsUnLockManager->SetFirstEnterArr(avatar_selective_tutorial_update_rp.selectiveTutorial.flags);

	return true;
}

// bool UReceivePacketHandler::OnBATTLEFIELD_AUTH_RP(FBATTLEFIELD_AUTH_RP& battlefield_auth_rp, URGameInstance* instance)
// {
// 	return true;
// }

bool UReceivePacketHandler::OnACCOUNT_REMOVE_RP(FACCOUNT_REMOVE_RP& account_remove_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnINIT_CLIENT_KEY_RP(FINIT_CLIENT_KEY_RP& init_client_key_rp, URGameInstance* instance)
{
	return URAuth::GetInstancePtr()->OnINIT_CLIENT_KEY_RP(init_client_key_rp);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Hero
//////////////////////////////////////////////////////////////////////////////////////////
// bool UReceivePacketHandler::OnHERO_GET_RP(FHERO_GET_RP& hero_get_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_get_rp.type, hero_get_rp.is, hero_get_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(hero_get_rp.kId))
// 			return false;
// 
// 		instance->HeroInventory->SetInventoryData(hero_get_rp.heros);
// 	}
// 	return true;
// }
//
// bool UReceivePacketHandler::OnHERO_GET_ONE_RP(FHERO_GET_ONE_RP& hero_get_one_rp, URGameInstance* instance)
// {
// 	return true;
// }
//
// bool UReceivePacketHandler::OnHERO_SKILL_LEVEL_UP_RP(FHERO_SKILL_LEVEL_UP_RP& hero_skill_level_up_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_skill_level_up_rp.type, hero_skill_level_up_rp.is, hero_skill_level_up_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError(false);
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(hero_skill_level_up_rp.kId))
// 			return false;
// 
// 		instance->UserInfo->SetAvatarData(hero_skill_level_up_rp.avatar);
// 		instance->HeroInventory->UpdateInventoryData(hero_skill_level_up_rp.hero);
// 		instance->ItemInventory->UpdateInventoryData(hero_skill_level_up_rp.items);
// 		
// 		//instance->HttpClient->OnHeroSkillLevelUpRpDelegate.Broadcast();
// 
// 		instance->EventManager->OnResponse_SkillLevelup.Broadcast();
// 	}
// 	return true;
// }

/*
bool UReceivePacketHandler::OnHERO_ITEM_EQUIP_SET_RP(FHERO_ITEM_EQUIP_SET_RP& hero_item_equip_set_rp, URGameInstance* instance)
{
return true;
}
*/

// bool UReceivePacketHandler::OnHERO_ITEM_EQUIP_RP(FHERO_ITEM_EQUIP_RP& hero_item_equip_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_item_equip_rp.type, hero_item_equip_rp.is, hero_item_equip_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(hero_item_equip_rp.kId))
// 			return false;
// 
// 		instance->HeroInventory->UpdateInventoryData(hero_item_equip_rp.heros);
// 
// 		for (FITEM& item : hero_item_equip_rp.items)
// 		{
// 			instance->ItemInventory->ChangeItemEquipment(item.itemUD, item.equipedHeroUD);
// 		}
// 
// 		instance->HttpClient->OnItemEquipDelegate.Broadcast(hero_item_equip_rp.items);
// 
// 		//instance->ItemInventory->ChangeItemEquipment()
// // old item refresh
// // instance->ItemInventory->ChangeItemEquipment(hero_item_equip_rp.ueqItemUD, "");
// // 
// // equip item refresh
// // instance->ItemInventory->ChangeItemEquipment(hero_item_equip_rp.eqItemUD, hero_item_equip_rp.heroUD);
// // 
// // heroinventory refresh
// // instance->HeroInventory->ChangeItemEquipment(hero_item_equip_rp.heroUD, (EItemEquipPositionEnum)hero_item_equip_rp.slotNo, hero_item_equip_rp.eqItemUD);
// 	}
// 	return true;
// }



//////////////////////////////////////////////////////////////////////////////////////////
// Item
//////////////////////////////////////////////////////////////////////////////////////////
// bool UReceivePacketHandler::OnITEM_GET_RP(FITEM_GET_RP& item_get_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_get_rp.type, item_get_rp.is, item_get_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_get_rp.kId))
// 			return false;
// 
// 		instance->ItemInventory->SetInventoryData(item_get_rp.items);
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnITEM_RESET_OPTIONS_RP(FITEM_RESET_OPTIONS_RP& item_reset_options_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_reset_options_rp.type, item_reset_options_rp.is, item_reset_options_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	// Avatar Update
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_reset_options_rp.update);
// 
// 	// UserInfoEvent
// 	instance->EventManager->OnUpdateUserInfo.Broadcast();
// 
// 	instance->EventManager->OnResponse_EquipAllHeroItem_HeroManagement.Broadcast();
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnITEM_RESET_OPTION_VALUE_RP(FITEM_RESET_OPTION_VALUE_RP& item_reset_option_value_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_reset_option_value_rp.type, item_reset_option_value_rp.is, item_reset_option_value_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_reset_option_value_rp.update);
// 
// 	// UserInfoEvent
// 	instance->EventManager->OnUpdateUserInfo.Broadcast();
// 
// 	instance->EventManager->OnResponse_EquipAllHeroItem_HeroManagement.Broadcast();
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnITEM_ENCHANT_RP(FITEM_ENCHANT_RP& item_enchant_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_enchant_rp.type, item_enchant_rp.is, item_enchant_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_enchant_rp.kId))
// 			return false;
// 
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_enchant_rp.update);
// 
// 
// 		// UserInfoEvent
// 		instance->EventManager->OnUpdateUserInfo.Broadcast();
// 
// 		instance->HttpClient->OnItemEnchantRpDelegate.Broadcast(item_enchant_rp.enchantResult, item_enchant_rp.targetItem, item_enchant_rp.enchantFailedReturnMaterials);
// 	}
// 	return true;
// }

// bool UReceivePacketHandler::OnITEM_MANUFATURE_RP(FITEM_MANUFATURE_RP& item_manufature_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_manufature_rp.type, item_manufature_rp.is, item_manufature_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_manufature_rp.kId))
// 			return false;
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_manufature_rp.update);
// 
// 		// UserInfoEvent
// 		instance->EventManager->OnUpdateUserInfo.Broadcast();
// 
// 		instance->HttpClient->OnItemManufactureRpDelegate.Broadcast(item_manufature_rp.is, item_manufature_rp.manufaturedItem);
// 	}
// 	return true;
// }

bool UReceivePacketHandler::OnBILLING_TRANSACTION_CREATE_RP(FBILLING_TRANSACTION_CREATE_RP& billing_transaction_create_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)billing_transaction_create_rp.type, billing_transaction_create_rp.is, billing_transaction_create_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->BillingManager->OnBillingTransactionCreateRp(false, TEXT(""), TEXT(""), TEXT(""));

		return false;
	}
	else
	{
		instance->BillingManager->OnBillingTransactionCreateRp(true, billing_transaction_create_rp.product, billing_transaction_create_rp.transaction_id, billing_transaction_create_rp.payload);
	}
	return true;
}
// 
// bool UReceivePacketHandler::OnITEM_MULTI_GRIND_RP(FITEM_MULTI_GRIND_RP& item_multi_grind_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_multi_grind_rp.type, item_multi_grind_rp.is, item_multi_grind_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_multi_grind_rp.kId))
// 			return false;
// 
// 		if (item_multi_grind_rp.is == ECRUD::CRUD_OK)
// 		{
// 			// Item Delete
// 			for (auto& Item : item_multi_grind_rp.targetItems)
// 			{
// 				instance->ItemInventory->RemoveInventoryData(Item.itemUD);
// 			}
// 
// 			TArray<int32> Before_Materials_Count;
// 			// Material Update
// 			for (auto& Item : item_multi_grind_rp.grindReturnMaterials)
// 			{
// 				Before_Materials_Count.Emplace(instance->ItemInventory->GetInventoryDataCount(Item.itemId));
// 				instance->ItemInventory->UpdateInventoryData(Item);
// 			}
// 
// 			// Result Popup
// 			UUIFunctionLibrary::ShowRewardPopupByRewards(instance, item_multi_grind_rp.grindRewards);
// 
// 			instance->HttpClient->OnItemMultiGrindRpDelegate.Broadcast(item_multi_grind_rp.is, item_multi_grind_rp.targetItems, item_multi_grind_rp.grindReturnMaterials, Before_Materials_Count);
// 		}
// 	}
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnITEM_MULTI_SALE_RP(FITEM_MULTI_SALE_RP& item_multi_sale_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_multi_sale_rp.type, item_multi_sale_rp.is, item_multi_sale_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (item_multi_sale_rp.is == ECRUD::CRUD_OK)
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_multi_sale_rp.kId))
// 			return false;
// 
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_multi_sale_rp.update);
// 
// 		UUIFunctionLibrary::ShowRewardPopupByRewards(instance, item_multi_sale_rp.rewards);
// 	}
// 
// 	instance->HttpClient->OnItemMultiSaleRpDelegate.Broadcast((item_multi_sale_rp.is == ECRUD::CRUD_OK));
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnITEM_INSERT_JEWEL_RP(FITEM_INSERT_JEWEL_RP& item_insert_jewel_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_insert_jewel_rp.type, item_insert_jewel_rp.is, item_insert_jewel_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_insert_jewel_rp.kId))
// 			return false;
// 			
// 
// 		ECRUD IsSuccess = ECRUD::CRUD_OK;
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_insert_jewel_rp.update);
// 
// 		instance->HttpClient->OnItemInsertJewelRpDelegate.Broadcast(item_insert_jewel_rp.targetItem);
// 	}
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnITEM_REMOVE_JEWEL_RP(FITEM_REMOVE_JEWEL_RP& item_remove_jewel_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_remove_jewel_rp.type, item_remove_jewel_rp.is, item_remove_jewel_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_remove_jewel_rp.kId))
// 			return false;
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_remove_jewel_rp.update);
// 
// 		// UserInfoEvent
// 		instance->EventManager->OnUpdateUserInfo.Broadcast();
// 
// 		instance->HttpClient->OnItemRemoveJewelRpDelegate.Broadcast(item_remove_jewel_rp.targetItem);
// 	}
// 		
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnHERO_GET_ONE_WITH_ITEM_RP(FHERO_GET_ONE_WITH_ITEM_RP& hero_get_one_with_item_rp, URGameInstance* instance)
// {
// 	return true;
// }
//
//////////////////////////////////////////////////////////////////////////////////////////
// PlayCamp
//////////////////////////////////////////////////////////////////////////////////////////

// bool UReceivePacketHandler::OnPLAY_MODE_RAID_RP(FPLAY_MODE_RAID_RP& play_mode_raid_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)play_mode_raid_rp.type, play_mode_raid_rp.is, play_mode_raid_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
// 
// 
// 		if (!instance->HttpClient->IsValidKID(play_mode_raid_rp.kId))
// 			return false;
// 
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, play_mode_raid_rp.update);
// 
// 		BaseStageInfo->PlayUD = play_mode_raid_rp.playUD;
// 
// 		instance->RewardInfo->SetRewardData( play_mode_raid_rp.rewards);
// 	}
// 	return true;
// }

// bool UReceivePacketHandler::OnGET_RAID_STAGE_USER_RP(FGET_RAID_STAGE_USER_RP& get_raid_stage_user_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)get_raid_stage_user_rp.type, get_raid_stage_user_rp.is, get_raid_stage_user_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// // 		if (!instance->HttpClient->IsValidKID(get_raid_stage_user_rp.kId))
// // 			return false;
// // 
// // 		if (IsValid(instance) && IsValid(instance->RaidManager))
// // 		{
// // 			instance->RaidManager->SetModes(get_raid_stage_user_rp.modes);
// // 			instance->RaidManager->SetRaidChallengeCount(get_raid_stage_user_rp.raid);
// // 		}
// 	}
// 	return true;
// }

// bool UReceivePacketHandler::OnGET_RAID_STAGE_ALL_RP(FGET_RAID_STAGE_ALL_RP& get_raid_stage_all_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)get_raid_stage_all_rp.type, get_raid_stage_all_rp.is, get_raid_stage_all_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// // 	else
// // 	{
// // 		instance->RaidManager->SetRaidList(get_raid_stage_all_rp.raids);
// // 	}
// 	return true;
// }

// bool UReceivePacketHandler::OnPLAY_MODE_RAID_DONE_RP(FPLAY_MODE_RAID_DONE_RP& play_mode_raid_done_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)play_mode_raid_done_rp.type, play_mode_raid_done_rp.is, play_mode_raid_done_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(play_mode_raid_done_rp.kId))
// 			return false;
// 
// // 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, play_mode_raid_done_rp.update);
// // 
// // 		instance->RewardInfo->SetClearGrade(play_mode_raid_done_rp.clearGrade);
// // 		instance->RewardInfo->SetRewardData( play_mode_raid_done_rp.rewards);
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnCAMPAIGN_EPISODE_REWARD_RP(FCAMPAIGN_EPISODE_REWARD_RP& campaign_episode_reward_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)campaign_episode_reward_rp.type, campaign_episode_reward_rp.is, campaign_episode_reward_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, campaign_episode_reward_rp.update);
// 
// 		instance->CampaignManager->SetRewardEpisodeStatus(campaign_episode_reward_rp.campaign_status.episodeRewardStatus);
// 
// 		instance->EventManager->OnCampaignEpisodeReward.Broadcast();
// 
// 		UUIFunctionLibrary::ShowRewardPopupByRewards(instance, campaign_episode_reward_rp.rewards);
// 	}
// 
// 	return true;
// }

//bool UReceivePacketHandler::OnPLAY_MODE_ARENA_RP(FPLAY_MODE_ARENA_RP& play_mode_arena_rp, URGameInstance* instance)
//{
//	if (!instance->HttpClient->IsValidKID(play_mode_arena_rp.kId))
//		return false;
//
//	if (instance->PacketErrorManager->IsCheckError((EPacketType)play_mode_arena_rp.type, play_mode_arena_rp.error_info) == true)
//	{
//		instance->PacketErrorManager->ProcessError();
//
//		return false;
//	}
//	else
//	{
//		instance->UserInfo->SetAvatarData(play_mode_arena_rp.avatar);
//		instance->StageInfo->PlayUD = play_mode_arena_rp.playUD;
//
//		//instance->ArenaManager->SetArenaMatchTarget(play_mode_arena_rp.match);
//		//instance->OtherHeroManager->InsertOtherHeroData(play_mode_arena_rp.match.kId, play_mode_arena_rp.match.heros);
//
//		//instance->RewardInfo->SetRewardData(play_mode_arena_rp.playUD, play_mode_arena_rp.rewards);
//	}
//	return true;
//}

//bool UReceivePacketHandler::OnPLAY_MODE_ARENA_DONE_RP(FPLAY_MODE_ARENA_DONE_RP& play_mode_arena_done_rp, URGameInstance* instance)
//{
//	if (!instance->HttpClient->IsValidKID(play_mode_arena_done_rp.kId))
//		return false;
//
//	if (instance->PacketErrorManager->IsCheckError((EPacketType)play_mode_arena_done_rp.type, play_mode_arena_done_rp.error_info) == true)
//	{
//		instance->PacketErrorManager->ProcessError();
//
//		return false;
//	}
//	else
//	{
//		instance->UserInfo->SetAvatarData(play_mode_arena_done_rp.avatarChanged);
//		instance->HeroInventory->UpdateInventoryData(play_mode_arena_done_rp.heroChanged);
//		instance->ItemInventory->UpdateInventoryData(play_mode_arena_done_rp.itemChanged);
//
//		//instance->RewardInfo->SetClearGrade(play_mode_arena_done_rp.clearGrade);
//		instance->RewardInfo->SetRewardData(play_mode_arena_done_rp.playUD, play_mode_arena_done_rp.rewards);
//
//		//instance->DeckManager->SetDeck(play_mode_arena_done_rp.deck);
//	}
//	return true;
//}




//////////////////////////////////////////////////////////////////////////////////////////
// Arena
//////////////////////////////////////////////////////////////////////////////////////////
// bool UReceivePacketHandler::OnARENA_MATCH_LIST_RP(FARENA_MATCH_LIST_RP& arena_match_list_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_match_list_rp.type, arena_match_list_rp.is, arena_match_list_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(arena_match_list_rp.arenaUser.kId))
// 			return false;
// 
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, arena_match_list_rp.update);
// 
// 		instance->EventManager->OnUpdateUserInfo.Broadcast();
// 
// 		UArenaManager* arena_mgr = instance->ArenaManager;
// 		if (arena_mgr)
// 		{
// 			arena_mgr->SetSeasonInfo(arena_match_list_rp.seasonInfo);
// 			arena_mgr->SetMyArenaInfo(arena_match_list_rp.arenaUser);
// 			arena_mgr->SetMatchList(arena_match_list_rp.matchList);
// 			arena_mgr->DoneMatchInfoSetting();
// 			return true;
// 		}
// 	}
// 	return false;
// }

bool UReceivePacketHandler::OnARENA_PLAY_RP(FARENA_PLAY_RP& arena_play_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_play_rp.type, arena_play_rp.is, arena_play_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		instance->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_ArenaDeckSetting);

		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Init);

		//arena_play_rp.update.avatar.kId
		UArenaManager* arena_mgr = instance->ArenaManager;

		if (!instance->HttpClient->IsValidKID(arena_play_rp.update.avatar.kId))
			return false;

		instance->OtherHeroManager->InsertOtherHeroData(arena_mgr->m_MatchList.kId, arena_mgr->m_MatchList.heros);

		UUtilFunctionLibrary::UpdateGoodsInfo(instance, arena_play_rp.update);

		instance->UserInfo->SetAvatarData(arena_play_rp.update.avatar);
		instance->EventManager->OnUpdateUserInfo.Broadcast();

		arena_mgr->SetArenaPlayUD(arena_play_rp.playUD);
		instance->PlayModeManager->PlayModeArenaStart();
		return true;
	}
	return true;
}

bool UReceivePacketHandler::OnARENA_DEFENSE_REWARD_RP(FARENA_DEFENSE_REWARD_RP& arena_defense_reward_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_defense_reward_rp.type, arena_defense_reward_rp.is, arena_defense_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, arena_defense_reward_rp.update);

	UUIFunctionLibrary::ShowRewardPopupByRewards(arena_defense_reward_rp.rewards);

	instance->EventManager->OnUpdateUserInfo.Broadcast();

	UArenaManager* arena_mgr = instance->ArenaManager;
	if (arena_mgr)
	{
		arena_mgr->SetArenaInfo(arena_defense_reward_rp.arenaStatus);
		arena_mgr->ArenaLobbyUpdate();
	}

	
	return true;
}

bool UReceivePacketHandler::OnARENA_PLAY_DONE_RP(FARENA_PLAY_DONE_RP& arena_play_done_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_play_done_rp.type, arena_play_done_rp.is, arena_play_done_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		UArenaManager* ArenaManager = instance->ArenaManager;
		URInventory* RInventory = instance->RInventory;

		UTableManager* tblMgr = UTableManager::GetInstancePtr();

		int32 PVPTokenBefore =  RInventory->GetPVPToken();
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, arena_play_done_rp.update);
		int32 PVPTokenAfter = RInventory->GetPVPToken();
		ArenaManager->SetArenaInfoResult(arena_play_done_rp.arenaStatus, PVPTokenAfter-PVPTokenBefore);
		ArenaManager->SetReward(arena_play_done_rp.update.avatar, arena_play_done_rp.rewards);
		ArenaManager->ArenaDoneRP();
	}
	return true;
}

bool UReceivePacketHandler::OnARENA_SEASON_DONE_RP(FARENA_SEASON_DONE_RP& arena_season_done_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_season_done_rp.type, arena_season_done_rp.is, arena_season_done_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, arena_season_done_rp.update);

		UUIFunctionLibrary::ShowRewardPopupByRewards(arena_season_done_rp.rewards);

		instance->EventManager->OnUpdateUserInfo.Broadcast();

		UArenaManager* arena_mgr = instance->ArenaManager;
		if (arena_mgr)
		{
			arena_mgr->SetArenaInfo(arena_season_done_rp.arenaStatus);
			arena_mgr->ArenaLobbyUpdate();
		}
	}

	return true;
}

bool UReceivePacketHandler::OnARENA_RANKING_LIST_RP(FARENA_RANKING_LIST_RP& arena_ranking_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_ranking_list_rp.type, arena_ranking_list_rp.is, arena_ranking_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UArenaManager* arena_mgr = instance->ArenaManager;
	if (arena_mgr)
		arena_mgr->SetRankerList(arena_ranking_list_rp.rankingList);

	return true;
}

bool UReceivePacketHandler::OnARENA_DEFENSE_DECK_SET_RP(FARENA_DEFENSE_DECK_SET_RP& arena_defense_deck_set_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_defense_deck_set_rp.type, arena_defense_deck_set_rp.is, arena_defense_deck_set_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, arena_defense_deck_set_rp.update);

	return true;
}

// bool UReceivePacketHandler::OnARENA_HISTORY_RP(FARENA_HISTORY_RP& arena_history_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_history_rp.type, arena_history_rp.is, arena_history_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(arena_history_rp.kId))
// 			return false;
// 
// 		UArenaManager* arena_mgr = instance->ArenaManager;
// 		if (arena_mgr)
// 		{
// 			arena_mgr->SetArenaHistory(arena_history_rp.history);
// 		}
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnARENA_SET_DEFAULT_HERO_RP(FARENA_SET_DEFAULT_HERO_RP& arena_set_default_hero_rp, URGameInstance* instance)
// {
// 	return true;
// }

/*
bool UReceivePacketHandler::OnARENA_GET_RP(FARENA_GET_RP& arena_get_rp, URGameInstance* instance)
{
if (!instance->HttpClient->IsValidKID(arena_get_rp.kId))
return false;

if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_get_rp.type, arena_get_rp.error_info) == true)
{
instance->PacketErrorManager->ProcessError();

return false;
}
else
{
//My Deck, if not My deck, return default play deck1
instance->DeckManager->SetDeck(arena_get_rp.deck);
instance->ArenaManager->SetMatches(arena_get_rp.matches);
instance->ArenaManager->SetUserArenaInfo(arena_get_rp.arena);
//instance->ArenaManager->SetUserArenaDeckUd(arena_get_rp.deck.deckUD);
}
return true;
}*/

/*
bool UReceivePacketHandler::OnARENA_DECK_GET_RP(FARENA_DECK_GET_RP& arena_deck_get_rp, URGameInstance* instance)
{
if (!instance->HttpClient->IsValidKID(arena_deck_get_rp.kId))
return false;

instance->DeckManager->SetDeck(arena_deck_get_rp.deck);

return true;
}
*/

//bool UReceivePacketHandler::OnARENA_MATCHES_RP(FARENA_MATCHES_RP& arena_matches_rp, URGameInstance* instance)
//{
//	if (!instance->HttpClient->IsValidKID(arena_matches_rp.arena_user.kId))
//		return false;
//
//	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_matches_rp.type, arena_matches_rp.error_info) == true)
//	{
//		instance->PacketErrorManager->ProcessError();
//
//		return false;
//	}
//	else
//	{
//		instance->ArenaManager->SetMatches(arena_matches_rp.matches);
//	}
//	return true;
//}

//
/*
bool UReceivePacketHandler::OnARENA_GET_HIGH_RANK_USER_ARENA_INFO_RP(FARENA_GET_RP& arena_get_rp, URGameInstance* instance)
{
if (!instance->HttpClient->IsValidKID(arena_get_rp.kId))
return false;

instance->ArenaManager->SetHighRankUsersArenaInfo()
}*/


//////////////////////////////////////////////////////////////////////////////////////////
// Raid
//////////////////////////////////////////////////////////////////////////////////////////


// bool UReceivePacketHandler::OnGUILD_RAID_STATUS_RP(FGUILD_RAID_STATUS_RP& guild_raid_status_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_raid_status_rp.type, guild_raid_status_rp.is, guild_raid_status_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (instance->GuildInfo)
// 		{
// 			instance->GuildInfo->SetGuildRaidStatusInfos(guild_raid_status_rp.raids, guild_raid_status_rp.season_left);
// 		}
// 
// 		if( instance->EventManager)
// 			instance->EventManager->OnRefreshGuildRaidInfo.Broadcast();
// 
// 	}
// 
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnGUILD_RAID_CHALLENGE_RP(FGUILD_RAID_CHALLENGE_RP& guild_raid_challenge_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_raid_challenge_rp.type, guild_raid_challenge_rp.is, guild_raid_challenge_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (instance->GuildInfo)
// 		{
// 			instance->GuildInfo->SetGuildRaidStatusInfo(guild_raid_challenge_rp.history, guild_raid_challenge_rp.raid);
// 		}
// 
// 		if (instance->EventManager)
// 			instance->EventManager->OnRefreshGuildRaidHistory.Broadcast();
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnGUILD_RAID_SPAWN_RP(FGUILD_RAID_SPAWN_RP& guild_raid_spawn_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_raid_spawn_rp.type, guild_raid_spawn_rp.is, guild_raid_spawn_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (instance->GuildInfo)
// 		{
// 			instance->GuildInfo->SetGuildRaidStatusInfos(guild_raid_spawn_rp.raids, guild_raid_spawn_rp.season_left);
// 		}
// 
// 		if (instance->EventManager)
// 			instance->EventManager->OnRefreshGuildRaidInfo.Broadcast();
// 		UPacketFunctionLibrary::GUILD_GET_RQ();
// 	}
// 
// 
// 	return true;
// }
/*
bool UReceivePacketHandler::OnGUILD_RAID_UPGRADE_RP(FGUILD_RAID_UPGRADE_RP& guild_raid_upgrade_rp, URGameInstance* instance)
{

	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_raid_upgrade_rp.type, guild_raid_upgrade_rp.is, guild_raid_upgrade_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		if (instance->GuildInfo)
		{
			instance->GuildInfo->SetGuildRaidStatusInfos(guild_raid_upgrade_rp.raids, guild_raid_upgrade_rp.last_reward, guild_raid_upgrade_rp.last_level,
				guild_raid_upgrade_rp.season_reward, guild_raid_upgrade_rp.season_max, guild_raid_upgrade_rp.season_point, guild_raid_upgrade_rp.season_level,
				guild_raid_upgrade_rp.last_rewarded, guild_raid_upgrade_rp.season_left);
		}

		if (instance->EventManager)
			instance->EventManager->OnRefreshGuildRaidInfo.Broadcast();
	}


	return true;
}
*/
/*
bool UReceivePacketHandler::OnGUILD_RAID_REWARD_RP(FGUILD_RAID_REWARD_RP& guild_raid_reward_rp, URGameInstance* instance)
{

	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_raid_reward_rp.type, guild_raid_reward_rp.is, guild_raid_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		if (instance->RWidgetManager)
		{
			instance->RWidgetManager->ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRaid_SeasonReward")),
				ECommonPopupType::VE_OK);
		}


		if (instance->GuildInfo)
		{
			instance->GuildInfo->SetGuildRaidStatusInfos(instance->GuildInfo->RaidStatusInfos, guild_raid_reward_rp.last_reward, guild_raid_reward_rp.last_level,
				guild_raid_reward_rp.season_reward, guild_raid_reward_rp.season_max, guild_raid_reward_rp.season_point, guild_raid_reward_rp.season_level,
				guild_raid_reward_rp.last_rewarded, guild_raid_reward_rp.season_left);
		}

		if (instance->EventManager)
			instance->EventManager->OnRefreshGuildRaidInfo.Broadcast();
	}


	return true;
}
*/
// bool UReceivePacketHandler::OnGUILD_RAID_DSN_RP(FGUILD_RAID_DSN_RP& guild_raid_dsn_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_raid_dsn_rp.type, guild_raid_dsn_rp.is, guild_raid_dsn_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// // 		if (IsValid(instance->RealTimeModManager))
// // 		{
// // 			instance->RealTimeModManager->SetGuildRaidDSN(guild_raid_dsn_rp);
// // 		}
// // 
// // 		if (IsValid(instance->EventManager))
// // 		{
// // 			instance->EventManager->OnRefreshGuildRaidRealServerInfo.Broadcast();
// // 		}
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnGUILD_RAID_SPAWN_OPS_RP(FGUILD_RAID_SPAWN_OPS_RP& guild_raid_spawn_ops_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_raid_spawn_ops_rp.type, guild_raid_spawn_ops_rp.is, guild_raid_spawn_ops_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnBATTLE_GROUND_DSN_RP(FBATTLE_GROUND_DSN_RP& battle_ground_dsn_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)battle_ground_dsn_rp.type, battle_ground_dsn_rp.is, battle_ground_dsn_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(battle_ground_dsn_rp.kId))
// 			return false;
// 
// // 		if (IsValid(instance->RealTimeModManager))
// // 		{
// // 			instance->RealTimeModManager->SetBattleGroundDSN(battle_ground_dsn_rp);
// // 		}
// 	}
// 
// 	return true;
// }


//////////////////////////////////////////////////////////////////////////////////////////
// CAMP
//////////////////////////////////////////////////////////////////////////////////////////
// bool UReceivePacketHandler::OnCAMPAIGN_QUEST_REWARD_RP(FCAMPAIGN_QUEST_REWARD_RP& campaign_quest_reward_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)campaign_quest_reward_rp.type, campaign_quest_reward_rp.is, campaign_quest_reward_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (instance->HttpClient->IsResponseSuccess(campaign_quest_reward_rp.is)) {
// 			UUtilFunctionLibrary::UpdateGoodsInfo(instance, campaign_quest_reward_rp.update);
// 
// 			instance->CampaignDailyQuestManager->SetCampaignDailyQuestStatus(campaign_quest_reward_rp.campaign_status.Elite_1);
// // 			FText PopupCaption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_Success_Purchase"));
// 			
// 			instance->EventManager->OnCampaignQuestRewardRp.Broadcast();
// 
// 			UUIFunctionLibrary::ShowRewardPopupByRewards(instance, campaign_quest_reward_rp.rewards);
// 
// 			return true;
// 		}
// 		else return false;
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCAMPAIGN_STATUS_RP(FCAMPAIGN_STATUS_RP& campaign_status_rp, URGameInstance* instance)
// {
// 	if (IsValid(instance->CampaignManager))
// 	{
// 		instance->CampaignManager->SetCampaingStatus(campaign_status_rp.campaign_status);
// 		instance->CampaignDailyQuestManager->SetCampaignDailyQuestStatus(campaign_status_rp.campaign_status.Elite_1);
// 		instance->WeekDungeonManager->SetWeekDungeonStatus(campaign_status_rp.campaign_status);
// 
// 		instance->HttpClient->OnCampStatusRpDelegate.Broadcast();
// 		return true;
// 	}
// 	return false;
// }
// 
// bool UReceivePacketHandler::OnCAMPAIGN_PLAY_START_RP(FCAMPAIGN_PLAY_START_RP& campaign_play_start_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)campaign_play_start_rp.type, campaign_play_start_rp.is, campaign_play_start_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
// 
// 		if (instance->HttpClient->IsResponseSuccess(campaign_play_start_rp.is)) {
// 			UUtilFunctionLibrary::UpdateGoodsInfo(instance, campaign_play_start_rp.update);
// 
// 			BaseStageInfo->PlayUD = campaign_play_start_rp.playUD;
// 
// 			instance->HttpClient->OnPlayModeCampRpDelegate.Broadcast();
// 
// 			return true;
// 		}
// 		else return false;
// 	}
// 
// 
// 	return true;
// }
//
// bool UReceivePacketHandler::OnCAMPAIGN_PLAY_DONE_RP(FCAMPAIGN_PLAY_DONE_RP& campaign_play_done_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)campaign_play_done_rp.type, campaign_play_done_rp.is, campaign_play_done_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 		
// 
// 	//HeroInfos before Get Reward 
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, campaign_play_done_rp.update);
// 	instance->RewardInfo->SetPlayDeckHeroInfoBeforeGetReward(campaign_play_done_rp.update.inventoryUpdate.heros_update);
// 	instance->CampaignDailyQuestManager->SetCampaignDailyQuestStatus(campaign_play_done_rp.campaign_status.Elite_1);
// 	instance->RewardInfo->SetRewardData(campaign_play_done_rp.rewards);
// 	instance->RewardInfo->SetFirstReward(campaign_play_done_rp.firstRewards);
// 	instance->CampaignManager->SetRewardEpisodeStatus(campaign_play_done_rp.campaign_status.episodeRewardStatus);
// 	//instance->CampaignManager->InvalidateData();
// 	
// 	/*
// 	Update daily dungeon trial count
// 	*/
// 
// 	instance->WeekDungeonManager->UpdateEpisodeTrialCount(campaign_play_done_rp.campaign_status.dailyTicket);
// 	
// 
// 	//instance->EventManager->OnUpdateEpisodeTrialCount.Broadcast();
// 
// 	instance->HttpClient->OnPlayModeCampDoneRpDelegate.Broadcast();
// 
// 	//UUIFunctionLibrary::ShowHero2D(instance);
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnCAMPAIGN_PLAY_SWEEP_RP(FCAMPAIGN_PLAY_SWEEP_RP& campaign_play_sweep_rp, URGameInstance* instance)
// {
//     if (instance->PacketErrorManager->IsCheckError((EPacketType)campaign_play_sweep_rp.type, campaign_play_sweep_rp.is, campaign_play_sweep_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, campaign_play_sweep_rp.update);
// 
// 	instance->RewardInfo->SetRewardData(campaign_play_sweep_rp.rewardSet);
// 
// 	instance->HttpClient->OnPlayModeSweepRpDelegate.Broadcast(true);	
// 	return true;
// }

// bool UReceivePacketHandler::OnHERO_UNEQUIP_RP(FHERO_UNEQUIP_RP& hero_unequip_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_unequip_rp.type, hero_unequip_rp.is, hero_unequip_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (!instance->HttpClient->IsValidKID(hero_unequip_rp.kId))
// 		return false;
// 
// 	instance->HeroInventory->UpdateInventoryData(hero_unequip_rp.hero);
// 
// 	for (FITEM& item : hero_unequip_rp.items)
// 	{
// 		instance->ItemInventory->ChangeItemEquipment(item.itemUD, item.equipedHeroUD);
// 	}
// 
// 	instance->EventManager->OnResponse_UnEquipAllHeroItem_HeroManagement.Broadcast();
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnHERO_EQUIP_RP(FHERO_EQUIP_RP& hero_equip_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_equip_rp.type, hero_equip_rp.is, hero_equip_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (!instance->HttpClient->IsValidKID(hero_equip_rp.kId))
// 		return false;
// 
// 	for (auto& Heroelem : hero_equip_rp.heros)
// 	{
// 		instance->HeroInventory->UpdateInventoryData(Heroelem);
// 	}
// 	
// 
// 	for (FITEM& item : hero_equip_rp.items)
// 	{
// 		instance->ItemInventory->ChangeItemEquipment(item.itemUD, item.equipedHeroUD);
// 	}
// 
// 	instance->EventManager->OnResponse_EquipAllHeroItem_HeroManagement.Broadcast();
// 
// 	return true;
// }
//////////////////////////////////////////////////////////////////////////////////////////
// Hero episode
//////////////////////////////////////////////////////////////////////////////////////////

// bool UReceivePacketHandler::OnHERO_EPISODE_GET_RP(FHERO_EPISODE_GET_RP& hero_episode_get_rp, URGameInstance* instance)
// {
// 	return true;
// }

// bool UReceivePacketHandler::OnHERO_PLAY_STAGE_RP(FHERO_PLAY_STAGE_RP& hero_play_stage_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_play_stage_rp.type, hero_play_stage_rp.is, hero_play_stage_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (instance->HttpClient->IsResponseSuccess(hero_play_stage_rp.is)) {
// 			instance->UserInfo->SetAvatarData(hero_play_stage_rp.avatar);
// 			instance->StageInfo->PlayUD = hero_play_stage_rp.playUD;
// 
// 			instance->HttpClient->OnPlayModeCampRpDelegate.Broadcast();
// 
// 			return true;
// 		}
// 		else return false;
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnHERO_PLAY_DONE_RP(FHERO_PLAY_DONE_RP& hero_play_done_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_play_done_rp.type, hero_play_done_rp.is, hero_play_done_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 
// 	//HeroInfos before Get Reward 
// 	instance->RewardInfo->SetPlayDeckHeroInfoBeforeGetReward(hero_play_done_rp.update.heros);
// 	
// 	instance->HeroRuneDungeonManager->UpdateEpisode(hero_play_done_rp.episode);
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, hero_play_done_rp.update);
// 
// 	instance->RewardInfo->SetRewardData(hero_play_done_rp.rewards);
// 
// 	// Update stage grades
// 
// 	//instance->EventManager->OnUpdateEpisodeTrialCount.Broadcast();
// 
// 	instance->HttpClient->OnPlayModeCampDoneRpDelegate.Broadcast();
// 	return true;
// }




//////////////////////////////////////////////////////////////////////////////////////////
// ACHIEVE
//////////////////////////////////////////////////////////////////////////////////////////

bool UReceivePacketHandler::OnGIVE_ACHIEVE_REWARD_RP(FGIVE_ACHIEVE_REWARD_RP& give_achieve_reward_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)give_achieve_reward_rp.type, give_achieve_reward_rp.is, give_achieve_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		if (!instance->HttpClient->IsValidKID(give_achieve_reward_rp.kId))
			return false;
		
		//Reward Update
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, give_achieve_reward_rp.update);

		instance->EventManager->OnUpdateUserInfo.Broadcast();

		if (IsValid(instance->AchievementManager))
		{
			instance->AchievementManager->SetAchievementStatus(give_achieve_reward_rp.achieveStatus);
			instance->AchievementManager->GiveAchieveReward(give_achieve_reward_rp.rewards);
			instance->AchievementManager->OnUpdateAchievement.Broadcast();
		}
	}

	return true;
}

bool UReceivePacketHandler::OnGIFT_LIST_RP(FGIFT_LIST_RP& gift_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)gift_list_rp.type, gift_list_rp.is, gift_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		if (!instance->HttpClient->IsValidKID(gift_list_rp.kId))
			return false;

		instance->MailManager->UpdateMailList(gift_list_rp.mails);
	}

	return true;
}
/*
bool UReceivePacketHandler::OnGIFT_LIST_MORE_RP(FGIFT_LIST_MORE_RP& gift_list_more_rp, URGameInstance* instance)
{
if (!instance->HttpClient->IsValidKID(gift_list_more_rp.kId))
return false;

if (instance->PacketErrorManager->IsCheckError((EPacketType)gift_list_more_rp.type, gift_list_more_rp.error_info) == true)
{
instance->PacketErrorManager->ProcessError();

return false;
}
else
{
instance->MailManager->UpdateMoreMailList(gift_list_more_rp.mails);
}

return true;
}*/

/*
bool UReceivePacketHandler::OnSAVE_PDG_BINARY_RP(FSAVE_PDG_BINARY_RP& save_pdg_binary_rp, URGameInstance* instance)
{
return true;
}
*/
bool UReceivePacketHandler::OnGIFT_OPEN_RP(FGIFT_OPEN_RP& gift_open_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)gift_open_rp.type, gift_open_rp.is, gift_open_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		if (!instance->HttpClient->IsValidKID(gift_open_rp.kId))
			return false;

		instance->MailManager->OnGiftOpenRp.Broadcast(gift_open_rp.rewards);
		
		if (instance->MailManager->IsReceiveAll)
		{
			instance->MailManager->IsReceiveAll = false;

			UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Mail_AllReceive")),
				ECommonPopupType::VE_OK);
		}
		//Reward Update
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, gift_open_rp.update);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// SHOP
//////////////////////////////////////////////////////////////////////////

bool UReceivePacketHandler::OnSHOP_BUY_RP(FSHOP_BUY_RP& shop_buy_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_buy_rp.type, shop_buy_rp.is, shop_buy_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, shop_buy_rp.update);
	UUIFunctionLibrary::RefreshTopbarProperty_Simple(instance);

	instance->ShopManager->SetShopUserData(shop_buy_rp.shopUserData);

	FShopProductTableInfo* ShopProductInfo = UTableManager::GetInstancePtr()->GetShopProductRow( FName( *(shop_buy_rp.productId) ) );
	if (ShopProductInfo)
	{
		UUIFunctionLibrary::ShowCommonRewardPopupByRewardSet(/*PopupCaption,*/ shop_buy_rp.rewardSet);
	}

	return true;
}

bool UReceivePacketHandler::OnSHOP_CHARACTER_SUMMON_RP(FSHOP_CHARACTER_SUMMON_RP& shop_character_summon_rp, URGameInstance* instance)
{
	// TODO
	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_character_summon_rp.type, shop_character_summon_rp.is, shop_character_summon_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, shop_character_summon_rp.update);

		
	UWorld* World = instance->GetWorld();
	ATowerLobbyLevelScriptActor* LSA = Cast<ATowerLobbyLevelScriptActor>(World->GetLevelScriptActor());
	if(LSA)
		LSA->GachaHeroList = shop_character_summon_rp.update.inventoryUpdate.heros_update;

	if (instance->EventManager)
		instance->EventManager->OnSuccess_HeroSummon.Broadcast();

	return true;
}

// bool UReceivePacketHandler::OnSHOP_FLOORSTORE_STATUS_RP(FSHOP_FLOORSTORE_STATUS_RP& shop_floorstore_status_rp, URGameInstance* instance)
// {
// 	// TODO
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_floorstore_status_rp.type, shop_floorstore_status_rp.is, shop_floorstore_status_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	UShopManager*		ShopManager = instance->ShopManager;
// 	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::Init[ShopManager is Empty]"));
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, shop_floorstore_status_rp.update);
// 
// 	ShopManager->ResponFloorStoreStatus(shop_floorstore_status_rp.status);
// 
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnSHOP_FLOORSTORE_BUY_RP(FSHOP_FLOORSTORE_BUY_RP& shop_floorstore_buy_rp, URGameInstance* instance)
// {
// 	// TODO
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_floorstore_buy_rp.type, shop_floorstore_buy_rp.is, shop_floorstore_buy_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	UShopManager*		ShopManager = instance->ShopManager;
// 	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::Init[ShopManager is Empty]"));
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, shop_floorstore_buy_rp.update);
// 
// 	ShopManager->ResponFloorStoreItemBuy(shop_floorstore_buy_rp.status, shop_floorstore_buy_rp.rewards);
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnSHOP_FLOORSTORE_RESET_RP(FSHOP_FLOORSTORE_RESET_RP& shop_floorstore_reset_rp, URGameInstance* instance)
// {
// 	// TODO
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_floorstore_reset_rp.type, shop_floorstore_reset_rp.is, shop_floorstore_reset_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	UShopManager*		ShopManager = instance->ShopManager;
// 	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::Init[ShopManager is Empty]"));
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, shop_floorstore_reset_rp.update);
// 
// 	ShopManager->ResponFloorStoreReset(shop_floorstore_reset_rp.status);
// 
// 	return true;
// }

bool UReceivePacketHandler::OnSHOP_FLOORSHOP_STATUS_RP(FSHOP_FLOORSHOP_STATUS_RP& shop_floorshop_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_floorshop_status_rp.type, shop_floorshop_status_rp.is, shop_floorshop_status_rp.error_info))
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}
	UShopManager*		ShopManager = instance->ShopManager;
	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::Init[ShopManager is Empty]"));
	//current shopid
	ShopManager->CurrShopID = shop_floorshop_status_rp.status.shopId;
	//current item list
	ShopManager->SetFloorShopSlotArrData(shop_floorshop_status_rp.status.slots);
	//current shop's left time
	ShopManager->UpdateResetRemainTime(shop_floorshop_status_rp.status.timeLeft);
	return true;
}

bool UReceivePacketHandler::OnSHOP_FLOORSHOP_BUY_RP(FSHOP_FLOORSHOP_BUY_RP& shop_floorshop_buy_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_floorshop_buy_rp.type, shop_floorshop_buy_rp.is, shop_floorshop_buy_rp.error_info))
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, shop_floorshop_buy_rp.update);

	UShopManager*		ShopManager = instance->ShopManager;
	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::Init[ShopManager is Empty]"));

	//current shopid
	ShopManager->CurrShopID = shop_floorshop_buy_rp.status.shopId;
	//current item list
	ShopManager->SetFloorShopSlotArrData(shop_floorshop_buy_rp.status.slots);
	//current shop's left time
	ShopManager->UpdateResetRemainTime(shop_floorshop_buy_rp.status.timeLeft);
	UUIFunctionLibrary::ShowRewardPopupByRewards(shop_floorshop_buy_rp.rewards);
	return true;
}

bool UReceivePacketHandler::OnSHOP_FLOORSHOP_OPEN_SLOT_RP(FSHOP_FLOORSHOP_OPEN_SLOT_RP& shop_floorshop_open_slot_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_floorshop_open_slot_rp.type, shop_floorshop_open_slot_rp.is, shop_floorshop_open_slot_rp.error_info))
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, shop_floorshop_open_slot_rp.update);

	UShopManager*		ShopManager = instance->ShopManager;
	checkf(ShopManager != nullptr, TEXT("UUP_FloorShop::Init[ShopManager is Empty]"));

	//current shopid
	ShopManager->CurrShopID = shop_floorshop_open_slot_rp.status.shopId;
	//current item list
	ShopManager->SetFloorShopSlotArrData(shop_floorshop_open_slot_rp.status.slots);
	//current shop's left time
	ShopManager->UpdateResetRemainTime(shop_floorshop_open_slot_rp.status.timeLeft);
	UUIFunctionLibrary::ShowCommonPopupSplash(FText(), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Floor_Shop_Slot_Open")));
	return true;
}

bool UReceivePacketHandler::OnTOWER_STATUS_RP(FTOWER_STATUS_RP& tower_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_status_rp.type, tower_status_rp.is, tower_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}
	
	instance->CampaignManager->SetTowerStatus(tower_status_rp.tower_status);
	instance->MissionManager->SetMissionStatus(tower_status_rp.tower_status.floorMissionStatus);
	return true;
}

bool UReceivePacketHandler::OnTOWER_PLAY_START_RP(FTOWER_PLAY_START_RP& tower_play_start_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_play_start_rp.type, tower_play_start_rp.is, tower_play_start_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

		UUtilFunctionLibrary::UpdateGoodsInfo(instance, tower_play_start_rp.update);

		BaseStageInfo->PlayUD = tower_play_start_rp.playUD;
		BaseStageInfo->DropRewards = tower_play_start_rp.dropRewards;

		instance->PlayModeManager->CampignModeStart();

	}


	return true;
}

bool UReceivePacketHandler::OnTOWER_PLAY_DONE_RP(FTOWER_PLAY_DONE_RP& tower_play_done_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_play_done_rp.type, tower_play_done_rp.is, tower_play_done_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}


	//HeroInfos before Get Reward 
	const bool bCheckHeroUpdate = false;
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, tower_play_done_rp.update, bCheckHeroUpdate);

	//// hero »πµÊ »Æ¿Œ Ω√¿€
	TArray<FREWARD> HeroReward;
	HeroReward.Empty();
	for (FREWARD& reward : tower_play_done_rp.clearRewards)
	{
		if (reward.type == EREWARD_TYPE::HERO)
		{
			FCharacterTableInfo* pCharacter = UTableManager::GetInstancePtr()->GetCharacterRow(reward.rewardId);
			if(pCharacter)
			{
				// øµ»•¿Œ¡ˆ √º≈©
				if(pCharacter->IsHero)
					HeroReward.Emplace(reward);
			}
		}
	}

	for (FREWARD& reward : tower_play_done_rp.questRewards)
	{
		if (reward.type == EREWARD_TYPE::HERO)
			HeroReward.Emplace(reward);
	}
	
	if(0 < HeroReward.Num())
		instance->CampaignManager->SaveHeroRewardsForRewardPopup(HeroReward);
	//// hero »πµÊ »Æ¿Œ ≥°

	TArray<FREWARD> RewardArray = tower_play_done_rp.clearRewards;
	for (size_t i = 0; i < tower_play_done_rp.dropRewards.Num(); ++i)
		RewardArray.Emplace(tower_play_done_rp.dropRewards[i]);

	instance->CampaignManager->CampaignResult(RewardArray);

	if(tower_play_done_rp.questRewards.Num() > 0)
	{
		instance->CampaignManager->SavedQuestRewards.Empty();
		instance->CampaignManager->SavedQuestRewards = tower_play_done_rp.questRewards;
	}

	if (instance->CampaignManager->TowerStatus.floorQuestStatus.questId != tower_play_done_rp.tower_status.floorQuestStatus.questId)
	{
		instance->CampaignManager->bChangeQuest = true;
	}
	
	instance->CampaignManager->SetTowerStatus(tower_play_done_rp.tower_status);
	instance->MissionManager->SetMissionStatus(tower_play_done_rp.tower_status.floorMissionStatus);
	/*
	Update daily dungeon trial count
	*/

	//instance->WeekDungeonManager->UpdateEpisodeTrialCount(tower_play_done_rp.campaign_status.dailyTicket);


	//instance->EventManager->OnUpdateEpisodeTrialCount.Broadcast();
	instance->PlayModeManager->CampaignModeEnd();
//	instance->HttpClient->OnPlayModeCampDoneRpDelegate.Broadcast();

	//UUIFunctionLibrary::ShowHero2D(instance);

	return true;
}

//bool UReceivePacketHandler::OnTOWER_CHANGE_FLOOR_RP(FTOWER_CHANGE_FLOOR_RP& tower_change_floor_rp, URGameInstance* instance)
//{
//	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_change_floor_rp.type, tower_change_floor_rp.is, tower_change_floor_rp.error_info) == true)
//	{
//		instance->PacketErrorManager->ProcessError();
//
//		return false;
//	}
//
//	checkf(instance, TEXT("GameInstance Null!"));
//	checkf(instance->CampaignManager, TEXT("CampaignManager Null!"));
//	checkf(instance->EventManager, TEXT("EventManager Null!"));
//
//	instance->CampaignManager->TowerStatus = tower_change_floor_rp.tower_status;
//	instance->MissionManager->SetMissionStatus(tower_change_floor_rp.tower_status.floorMissionStatus);
//	instance->EventManager->OnUpdateCampaignUIPage.Broadcast(tower_change_floor_rp.tower_status.currentFloor);
//	instance->EventManager->OnUpdateTowerBossUIPage.Broadcast();
//	return true;
//}

bool UReceivePacketHandler::OnTOWER_TREASURE_BOX_RP(FTOWER_TREASURE_BOX_RP& tower_treasure_box_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_treasure_box_rp.type, tower_treasure_box_rp.is, tower_treasure_box_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->CampaignManager->SetTowerStatus(tower_treasure_box_rp.tower_status);
	instance->MissionManager->SetMissionStatus(tower_treasure_box_rp.tower_status.floorMissionStatus);
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, tower_treasure_box_rp.update);

	UUP_StageSelect* pStageSelect = instance->RWidgetManager->GetUserWidgetT<UUP_StageSelect>(EWidgetBluePrintEnum::WBP_Campaign);
	if (pStageSelect)
		pStageSelect->SetStageUI();
// 	UUP_Campaign_Tower* uptower = instance->RWidgetManager->GetUserWidgetT<UUP_Campaign_Tower>(EWidgetBluePrintEnum::WBP_Campaign);
// 	if (uptower)
// 		uptower->SetTreasureBox();
		

	//scrollview reward popup
	UUIFunctionLibrary::ShowCommonRewardPopupByRewards(tower_treasure_box_rp.rewards);
	
	return true;
}

bool UReceivePacketHandler::OnTOWER_BOSS_PLAY_CHECK_RP(FTOWER_BOSS_PLAY_CHECK_RP& tower_boss_play_check_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_boss_play_check_rp.type, tower_boss_play_check_rp.is, tower_boss_play_check_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	if (tower_boss_play_check_rp.isValid)
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		instance->RealTimeModManager->EnterTowerRaid(BaseStageInfo->StageID);
	}
	else
	{
		UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI,
			COMMONSTRING_NOTIFY), FText::FromString(TEXT("Unable to enter")), ECommonPopupType::VE_OK);
	}
		
	return true;
}

bool UReceivePacketHandler::OnTOWER_BOSS_PLAY_RESULT_RP(FTOWER_BOSS_PLAY_RESULT_RP& tower_boss_play_result_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_boss_play_result_rp.type, tower_boss_play_result_rp.is, tower_boss_play_result_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, tower_boss_play_result_rp.update);

	instance->RealTimeModManager->SetRTSResults(tower_boss_play_result_rp.rewards);
	instance->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ResultRaid);
	instance->MissionManager->SetMissionStatus(tower_boss_play_result_rp.tower_status.floorMissionStatus);

	return true;
}

bool UReceivePacketHandler::OnTOWER_FLOOR_MISSION_ACTIVE_RP(FTOWER_FLOOR_MISSION_ACTIVE_RP& tower_floor_mission_active_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_floor_mission_active_rp.type, tower_floor_mission_active_rp.is, tower_floor_mission_active_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, tower_floor_mission_active_rp.update);
	instance->MissionManager->SetMissionStatus(tower_floor_mission_active_rp.tower_status.floorMissionStatus);
	instance->MissionManager->ReceiveGetFloorMission.Broadcast();
	return true;
}

bool UReceivePacketHandler::OnTOWER_FLOOR_MISSION_REWARD_RP(FTOWER_FLOOR_MISSION_REWARD_RP& tower_floor_mission_reward_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)tower_floor_mission_reward_rp.type, tower_floor_mission_reward_rp.is, tower_floor_mission_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, tower_floor_mission_reward_rp.update);
	instance->MissionManager->SetMissionStatus(tower_floor_mission_reward_rp.tower_status.floorMissionStatus);
	instance->MissionManager->OnMissionStatusRp.Broadcast();
	instance->MissionManager->OnMissionRewardGetRp.Broadcast(tower_floor_mission_reward_rp.rewards, tower_floor_mission_reward_rp.complete_rewards);
	
	return true;
}

/*
bool UReceivePacketHandler::OnSHOP_RESET_COUNT_RP(FSHOP_RESET_COUNT_RP& shop_reset_count_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_reset_count_rp.type, shop_reset_count_rp.is, shop_reset_count_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->UserInfo->SetAvatarData(shop_reset_count_rp.avatar);
	instance->ShopManager->SetShopUserData(shop_reset_count_rp.shopUserData);
	return true;
}
*/
bool UReceivePacketHandler::OnSHOP_USER_DATA_RP(FSHOP_USER_DATA_RP& shop_user_data_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)shop_user_data_rp.type, shop_user_data_rp.is, shop_user_data_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->ShopManager->SetShopUserData(shop_user_data_rp.shopUserData);
	return true;
}

// bool UReceivePacketHandler::OnHERO_SHOP_PANEL_LIST_RP(FHERO_SHOP_PANEL_LIST_RP& hero_shop_panel_list_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_shop_panel_list_rp.type, hero_shop_panel_list_rp.is, hero_shop_panel_list_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ShopManager->SetHeroShopPanelList(hero_shop_panel_list_rp.panelList);
// 
// 	instance->EventManager->OnHeroShopListRp.Broadcast();
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnHERO_SHOP_PRODUCT_BUY_RP(FHERO_SHOP_PRODUCT_BUY_RP& hero_shop_product_buy_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_shop_product_buy_rp.type, hero_shop_product_buy_rp.is, hero_shop_product_buy_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ShopManager->UpdateHeroProduct(hero_shop_product_buy_rp.product);
// 	instance->EventManager->OnHeroShopBuyRp.Broadcast();
// 
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, hero_shop_product_buy_rp.update);
// 
// 	TArray<FREWARD> rewardList;
// 	if (hero_shop_product_buy_rp.rewardSet.Num() > 0)
// 	{		
// 		for (FREWARDSET& rewardset : hero_shop_product_buy_rp.rewardSet)
// 		{
// 			rewardList += rewardset.rewards;
// 		}
// 	}
// 
// 	UUIFunctionLibrary::ShowRewardPopupByRewardsHero(hero_shop_product_buy_rp.update.inventoryUpdate.heros_update, rewardList);
// 
// 	return true;
// }

bool UReceivePacketHandler::OnINVENTORY_HERO_UPGRADE_RP(FINVENTORY_HERO_UPGRADE_RP& inventory_hero_upgrade_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_upgrade_rp.type, inventory_hero_upgrade_rp.is, inventory_hero_upgrade_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_hero_upgrade_rp.update);

//	instance->EventManager->OnCharacterInfo.Broadcast();
	instance->EventManager->OnUpdateUserInfo.Broadcast();
	instance->EventManager->OnHeroUpgrade.Broadcast();
	
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_HERO_LEVELUP_RP(FINVENTORY_HERO_LEVELUP_RP& inventory_hero_levelup_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_levelup_rp.type, inventory_hero_levelup_rp.is, inventory_hero_levelup_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_hero_levelup_rp.update);
	instance->RInventory->UpdateInventory(inventory_hero_levelup_rp.update.inventoryUpdate);

	instance->EventManager->OnUpdateUserInfo.Broadcast();
	instance->EventManager->OnHeroLevelUp.Broadcast();
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_HERO_GRIND_RP(FINVENTORY_HERO_GRIND_RP& inventory_hero_grind_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_grind_rp.type, inventory_hero_grind_rp.is, inventory_hero_grind_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_hero_grind_rp.update);
// 	instance->RInventory->UpdateInventory(inventory_hero_grind_rp.update.inventoryUpdate);
// 
// 	for (FString HeroUD : inventory_hero_grind_rp.update.inventoryUpdate.heros_delete)
// 		instance->RInventory->RemoveHeroData(HeroUD);

	//instance->EventManager->OnHeroGrindRefresh.Broadcast();
	instance->EventManager->OnUpdateUserInfo.Broadcast();
	instance->HttpClient->OnInventoryUpdate.Broadcast(true);
	instance->EventManager->OnHeroGrind.Broadcast(inventory_hero_grind_rp.rewards);
	instance->EventManager->AfterHeroGrind.Broadcast(inventory_hero_grind_rp.rewards);
	UUIFunctionLibrary::ShowRewardPopupByRewards(inventory_hero_grind_rp.rewards);
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_HERO_EQUIP_RP(FINVENTORY_HERO_EQUIP_RP& inventory_hero_equip_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_equip_rp.type, inventory_hero_equip_rp.is, inventory_hero_equip_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryHeroEquip.Broadcast(false);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_hero_equip_rp.update);

	instance->HttpClient->OnInventoryHeroEquip.Broadcast(true);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_HERO_UNEQUIP_RP(FINVENTORY_HERO_UNEQUIP_RP& inventory_hero_unequip_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_unequip_rp.type, inventory_hero_unequip_rp.is, inventory_hero_unequip_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryHeroUnequip.Broadcast(false);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_hero_unequip_rp.update);

	instance->HttpClient->OnInventoryHeroUnequip.Broadcast(true);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_ITEM_GRIND_RP(FINVENTORY_ITEM_GRIND_RP& inventory_item_grind_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_item_grind_rp.type, inventory_item_grind_rp.is, inventory_item_grind_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryItemGrind.Broadcast(false, {});

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_item_grind_rp.update);

	instance->HttpClient->OnInventorySell.Broadcast(true, inventory_item_grind_rp.rewards);
	//instance->HttpClient->OnInventoryItemGrind.Broadcast(true, inventory_item_grind_rp.rewards);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_RUNE_EQUIP_RP(FINVENTORY_RUNE_EQUIP_RP& inventory_rune_equip_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_rune_equip_rp.type, inventory_rune_equip_rp.is, inventory_rune_equip_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryRuneEquip.Broadcast(false);

		return false;
	}

	// TODO
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_rune_equip_rp.update);
	//instance->EventManager->OnRuneItemUpdate.Broadcast();
	//instance->EventManager->OnRuneSlotUpdate.Broadcast();
	instance->HttpClient->OnInventoryRuneEquip.Broadcast(true);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_RUNE_UNEQUIP_RP(FINVENTORY_RUNE_UNEQUIP_RP& inventory_rune_unequip_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_rune_unequip_rp.type, inventory_rune_unequip_rp.is, inventory_rune_unequip_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryRuneUnequip.Broadcast(false);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_rune_unequip_rp.update);
	instance->HttpClient->OnInventoryRuneUnequip.Broadcast(true);
	//instance->EventManager->OnRuneItemUpdate.Broadcast();
	//instance->EventManager->OnRuneSlotUpdate.Broadcast();
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_RUNE_MAKE_RP(FINVENTORY_RUNE_MAKE_RP& inventory_rune_make_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_rune_make_rp.type, inventory_rune_make_rp.is, inventory_rune_make_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryRuneMake.Broadcast(false);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_rune_make_rp.update);
	//instance->EventManager->OnRuneItemMixUpdate.Broadcast();
	instance->HttpClient->OnInventoryRuneMake.Broadcast(true);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_ITEM_LEVELUP_RP(FINVENTORY_ITEM_LEVELUP_RP& inventory_item_levelup_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_item_levelup_rp.type, inventory_item_levelup_rp.is, inventory_item_levelup_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryItemLevelUp.Broadcast(false, false);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_item_levelup_rp.update);

	instance->HttpClient->OnInventoryItemLevelUp.Broadcast(true, inventory_item_levelup_rp.isLucky);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_ITEM_ENCHANT_RP(FINVENTORY_ITEM_ENCHANT_RP& inventory_item_enchant_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_item_enchant_rp.type, inventory_item_enchant_rp.is, inventory_item_enchant_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryItemEnchant.Broadcast(false, 0);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_item_enchant_rp.update);

	instance->HttpClient->OnInventoryItemEnchant.Broadcast(true, inventory_item_enchant_rp.affectedEnchant);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_ITEM_HERO_LOCK_RP(FINVENTORY_ITEM_HERO_LOCK_RP& inventory_item_hero_lock_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_item_hero_lock_rp.type, inventory_item_hero_lock_rp.is, inventory_item_hero_lock_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		
		return false;
	}
// 	instance->RInventory->UpdateInventory(inventory_item_hero_lock_rp.update.inventoryUpdate);
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_item_hero_lock_rp.update);
		// TODO
	instance->HttpClient->OnInventoryUpdate.Broadcast(true);
	instance->EventManager->OnHeroLock.Broadcast();
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_HERO_SKILL_LEVELUP_RP(FINVENTORY_HERO_SKILL_LEVELUP_RP& inventory_hero_skill_levelup_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_skill_levelup_rp.type, inventory_hero_skill_levelup_rp.is, inventory_hero_skill_levelup_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->RInventory->UpdateInventory(inventory_hero_skill_levelup_rp.update.inventoryUpdate);

	instance->EventManager->OnSkillUpgradeSuccess.Broadcast();
	instance->EventManager->OnUpdateUserInfo.Broadcast();

	// TODO

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_HERO_ENCHANT_CONSUME_RP(FINVENTORY_HERO_ENCHANT_CONSUME_RP& inventory_hero_enchant_consume_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_enchant_consume_rp.type, inventory_hero_enchant_consume_rp.is, inventory_hero_enchant_consume_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_hero_enchant_consume_rp.update);

	instance->HttpClient->OnInventoryUpdate.Broadcast(true);

	int32 nCurrentPoint = -1;
	if (inventory_hero_enchant_consume_rp.update.inventoryUpdate.heros_update.IsValidIndex(0))
		nCurrentPoint = inventory_hero_enchant_consume_rp.update.inventoryUpdate.heros_update[0].enchantPoint;

	instance->EventManager->OnHeroEnchant.Broadcast(inventory_hero_enchant_consume_rp.spiritResult, nCurrentPoint);
	
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_HERO_ENCHANT_STAT_RP(FINVENTORY_HERO_ENCHANT_STAT_RP& inventory_hero_enchant_stat_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_hero_enchant_stat_rp.type, inventory_hero_enchant_stat_rp.is, inventory_hero_enchant_stat_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_hero_enchant_stat_rp.update);
	instance->HttpClient->OnInventoryUpdate.Broadcast(true);
	instance->EventManager->OnHeroStatApply.Broadcast();
	
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_SELL_RP(FINVENTORY_SELL_RP& inventory_sell_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_sell_rp.type, inventory_sell_rp.is, inventory_sell_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventorySell.Broadcast(false, {});

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_sell_rp.update);

	instance->HttpClient->OnInventorySell.Broadcast(true, inventory_sell_rp.rewards);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_ITEM_UPGRADE_RP(FINVENTORY_ITEM_UPGRADE_RP& inventory_item_upgrade_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_item_upgrade_rp.type, inventory_item_upgrade_rp.is, inventory_item_upgrade_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_item_upgrade_rp.update);

	// (server) FINVENTORY_ITEM_UPGRADE_RP.update.items_update[0] is the item resulted of the ItemUpgrade

// 	if (!ensure(inventory_item_upgrade_rp.update.inventoryUpdate.items_update.IsValidIndex(0)))
// 	{
// 		// errornous
// 		instance->HttpClient->OnInventoryItemUpgrade.Broadcast(false, {}, TEXT(""));
// 		return false;
// 	}
	if(inventory_item_upgrade_rp.isSuccess)
		instance->HttpClient->OnInventoryItemUpgrade.Broadcast(true, inventory_item_upgrade_rp.rewards, inventory_item_upgrade_rp.update.inventoryUpdate.items_update[0].itemUD);
	else
		instance->HttpClient->OnInventoryItemUpgrade.Broadcast(false, {}, TEXT(""));

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_ITEM_OPTION_RESET_RP(FINVENTORY_ITEM_OPTION_RESET_RP& inventory_item_option_reset_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_item_option_reset_rp.type, inventory_item_option_reset_rp.is, inventory_item_option_reset_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryItemOptionReset.Broadcast(false);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_item_option_reset_rp.update);

	instance->HttpClient->OnInventoryItemOptionReset.Broadcast(true);

	return true;
}

bool UReceivePacketHandler::OnINVENTORY_TIMETREASURE_GET_RP(FINVENTORY_TIMETREASURE_GET_RP& inventory_timetreasure_get_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_timetreasure_get_rp.type, inventory_timetreasure_get_rp.is, inventory_timetreasure_get_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryUpgrade.Broadcast(false);

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_timetreasure_get_rp.update);

	//scrollview reward popup
	UUP_Popup_Reward_AfterGrind* RewardPopup = UUIFunctionLibrary::ShowRewardPopup_ScrollView(inventory_timetreasure_get_rp.rewards);
	if (!RewardPopup)
		return false;

	// TODO
	return true;
}

bool UReceivePacketHandler::OnINVENTORY_UPGRADE_RP(FINVENTORY_UPGRADE_RP& inventory_upgrade_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)inventory_upgrade_rp.type, inventory_upgrade_rp.is, inventory_upgrade_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->HttpClient->OnInventoryUpgrade.Broadcast(false);
		instance->HttpClient->OnHeroInventoryUpgrade.Broadcast(false);
		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, inventory_upgrade_rp.update);

	instance->HttpClient->OnInventoryUpgrade.Broadcast(true);
	instance->HttpClient->OnHeroInventoryUpgrade.Broadcast(true);

	return true;
}

// bool UReceivePacketHandler::OnHERO_SHOP_PANEL_RESET_RP(FHERO_SHOP_PANEL_RESET_RP& hero_shop_panel_reset_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_shop_panel_reset_rp.type, hero_shop_panel_reset_rp.is, hero_shop_panel_reset_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, hero_shop_panel_reset_rp.update);
// 
// 	instance->ShopManager->UpdateHeroShopPanel(hero_shop_panel_reset_rp.panel);
// 	instance->EventManager->OnHeroShopRefreshRp.Broadcast(hero_shop_panel_reset_rp.panel);
// 
// 	return true;
// }

//////////////////////////////////////////////////////////////////////////
// COLOSSEUM
//////////////////////////////////////////////////////////////////////////
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_HISTORY_RP(FCOLOSSEUM_HISTORY_RP& colosseum_history_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_history_rp.type, colosseum_history_rp.is, colosseum_history_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ColosseumManager->SetColosseumHistory(colosseum_history_rp.history);
// 
// 	instance->HttpClient->OnColosseumHistoryRpDelegate.Broadcast();
// 
// 	return true;
// }
//
// bool UReceivePacketHandler::OnCOLOSSEUM_LOBBY_INFO_RP(FCOLOSSEUM_LOBBY_INFO_RP& colosseum_lobby_info_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_lobby_info_rp.type, colosseum_lobby_info_rp.is, colosseum_lobby_info_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ColosseumManager->SetColosseumSeasonInfo(colosseum_lobby_info_rp.seasonInfo);
// 	instance->ColosseumManager->SetMyColosseumInfo(colosseum_lobby_info_rp.colosseumUser);
// 	instance->ColosseumManager->SetColosseumRankingList(colosseum_lobby_info_rp.rankingList);	
// 
// 	instance->HttpClient->OnColosseumLobbyInfoRpDelegate.Broadcast();
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_DECK_GET_RP(FCOLOSSEUM_DECK_GET_RP& colosseum_deck_get_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_deck_get_rp.type, colosseum_deck_get_rp.is, colosseum_deck_get_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ColosseumManager->SetColosseumRankingDeck(colosseum_deck_get_rp.kId, colosseum_deck_get_rp.deck);
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_DECK_SET_RP(FCOLOSSEUM_DECK_SET_RP& colosseum_deck_set_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_deck_set_rp.type, colosseum_deck_set_rp.is, colosseum_deck_set_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->UserInfo->SetAvatarData(colosseum_deck_set_rp.avatar);
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_TOP_RANK_RP(FCOLOSSEUM_TOP_RANK_RP& colosseum_top_rank_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_top_rank_rp.type, colosseum_top_rank_rp.is, colosseum_top_rank_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ColosseumManager->SetColosseumTopRank(colosseum_top_rank_rp.rankingList);
// 
// 	instance->HttpClient->OnSetColosseumTopRankRpDelegate.Broadcast();
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_HONOR_RANK_RP(FCOLOSSEUM_HONOR_RANK_RP& colosseum_honor_rank_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_honor_rank_rp.type, colosseum_honor_rank_rp.is, colosseum_honor_rank_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ColosseumManager->SetColosseumHonorRank(colosseum_honor_rank_rp.rankingList);
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_SEASON_DONE_RP(FCOLOSSEUM_SEASON_DONE_RP& colosseum_season_done_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_season_done_rp.type, colosseum_season_done_rp.is, colosseum_season_done_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	//Reward Update
// 	instance->UserInfo->SetAvatarData(colosseum_season_done_rp.avatarChanged);
// 	instance->RInventory->UpdateItemData(colosseum_season_done_rp.itemChanged);
// 
// 	UUIFunctionLibrary::ShowRewardPopupByRewardsHero(colosseum_season_done_rp.heroChanged, colosseum_season_done_rp.rewards);
// 
// 	ECOLOSSEUM_CLASS_VAL OldClassVal = instance->ColosseumManager->MyColosseumInfo.classVal;
// 	ECOLOSSEUM_CLASS_VAL NewClassVal = colosseum_season_done_rp.colosseumChanged.classVal;
// 	instance->ColosseumManager->SetMyColosseumInfo(colosseum_season_done_rp.colosseumChanged);
// 
// 	bool bChangeClass = false;
// 	if (OldClassVal != NewClassVal)
// 	{
// 		switch (NewClassVal)
// 		{
// 		case ECOLOSSEUM_CLASS_VAL::BRONZE_C:
// 			if (OldClassVal != ECOLOSSEUM_CLASS_VAL::BRONZE_C)
// 				bChangeClass = true;
// 			break;
// 		case ECOLOSSEUM_CLASS_VAL::SILVER_C:
// 			if (OldClassVal != ECOLOSSEUM_CLASS_VAL::SILVER_C)
// 				bChangeClass = true;
// 			break;
// 		case ECOLOSSEUM_CLASS_VAL::GOLD_C:
// 			if (OldClassVal != ECOLOSSEUM_CLASS_VAL::GOLD_C)
// 				bChangeClass = true;
// 			break;
// 		case ECOLOSSEUM_CLASS_VAL::DIAMOND_C:
// 			if (OldClassVal != ECOLOSSEUM_CLASS_VAL::DIAMOND_C)
// 				bChangeClass = true;
// 			break;
// 		case ECOLOSSEUM_CLASS_VAL::PLATINUM_S:
// 			if (OldClassVal != ECOLOSSEUM_CLASS_VAL::PLATINUM_S)
// 				bChangeClass = true;
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// 	
// 	instance->HttpClient->OnColosseumSeasonDoneRpDelegate.Broadcast(bChangeClass);
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnCOLOSSEUM_DSN_RP(FCOLOSSEUM_DSN_RP& colosseum_dsn_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_dsn_rp.type, colosseum_dsn_rp.is, colosseum_dsn_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// //		colosseum_dsn_rp.
// // 		if (IsValid(instance->RealTimeModManager))
// // 		{
// // 			instance->RealTimeModManager->SetColosseumDSN(colosseum_dsn_rp);
// // 		}
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnCOLOSSEUM_GET_AI_USER_RP(FCOLOSSEUM_GET_AI_USER_RP& colosseum_get_ai_user_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_get_ai_user_rp.type, colosseum_get_ai_user_rp.is, colosseum_get_ai_user_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (IsValid(instance->ColosseumManager))
// 	{
// 		instance->ColosseumManager->SetColosseumAIUserData(colosseum_get_ai_user_rp.avatar, colosseum_get_ai_user_rp.heros);
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_AI_PLAY_START_RP(FCOLOSSEUM_AI_PLAY_START_RP& colosseum_ai_play_start_rp, URGameInstance* instance)
// {
// // 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_ai_play_start_rp.type, colosseum_ai_play_start_rp.is, colosseum_ai_play_start_rp.error_info) == true)
// // 	{
// // 		instance->PacketErrorManager->ProcessError();
// // 
// // 		return false;
// // 	}
// // 
// // 	instance->UserInfo->SetAvatarData(colosseum_ai_play_start_rp.avatar);
// // 	instance->StageInfo->PlayUD = colosseum_ai_play_start_rp.playUD;
// // 	instance->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_ColosseumSetting);
// // 	instance->PlayModeManager->PlayModeColosseumStart();
// 	
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_AI_PLAY_DONE_RP(FCOLOSSEUM_AI_PLAY_DONE_RP& colosseum_ai_play_done_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_ai_play_done_rp.type, colosseum_ai_play_done_rp.is, colosseum_ai_play_done_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->ColosseumManager->SetColosseumPlayResult(true, colosseum_ai_play_done_rp.play_result);
// 	instance->EventManager->OnRTS_PLAY_SEND_RESULT_RP.Broadcast();
// 
// 	return true;
// }
// 
// // Not Used
// bool UReceivePacketHandler::OnCOLOSSEUM_PLAY_DONE_RP(FCOLOSSEUM_PLAY_DONE_RP& colosseum_play_done_rp, URGameInstance* instance)
// {
// 	return true;
// }
// // Not Used
// bool UReceivePacketHandler::OnCOLOSSEUM_USER_RP(FCOLOSSEUM_USER_RP& colosseum_user_rp, URGameInstance* instance)
// {
// 	return true;
// }
// // Not Used
// bool UReceivePacketHandler::OnCOLOSSEUM_TEST_RP(FCOLOSSEUM_TEST_RP& colosseum_test_rp, URGameInstance* instance)
// {
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCOLOSSEUM_AUTH_RP(FCOLOSSEUM_AUTH_RP& colosseum_auth_rp, URGameInstance* instance)
// {
// 	return true;
// }

bool UReceivePacketHandler::OnCOLOSSEUM_PLAY_RESULT_RP(FCOLOSSEUM_PLAY_RESULT_RP& colosseum_play_result_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_play_result_rp.type, colosseum_play_result_rp.is, colosseum_play_result_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}

	instance->ColosseumManager->CurrentToPrevStatus();
	instance->ColosseumManager->SetColosseumStatus(colosseum_play_result_rp.colosseumStatus);
	if (colosseum_play_result_rp.isWin)
	{
		instance->IsSuccess = true;
		instance->ColosseumManager->CS_SaveData.WinCount++;
		instance->ColosseumManager->CS_SaveData.WinningStreakCount++;
	}
	else
	{
		instance->IsSuccess = false;
		instance->ColosseumManager->CS_SaveData.LoseCount++;
		instance->ColosseumManager->CS_SaveData.WinningStreakCount = 0;
	}
	instance->ColosseumManager->SetPrevAvatarExp(instance->UserInfo->Avatar.exp);
	instance->ColosseumManager->SetMaxAvatarExp(instance->UserInfo->AvatarExpMax);
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, colosseum_play_result_rp.update);
	instance->ColosseumManager->m_rewards = colosseum_play_result_rp.rewards;
	UHUD_RaidThreeControl *RaidHUD = Cast<UHUD_RaidThreeControl>(UUIFunctionLibrary::GetInGameHUD());
	if (RaidHUD)
		RaidHUD->SetVisibility(ESlateVisibility::Collapsed);
	
	instance->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_ResultPageForRTS);
	UGameplayStatics::SetGamePaused(instance, true);

	return true;
}

bool UReceivePacketHandler::OnCOLOSSEUM_SEASON_DONE_RP(FCOLOSSEUM_SEASON_DONE_RP& colosseum_season_done_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_season_done_rp.type, colosseum_season_done_rp.is, colosseum_season_done_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}

	TArray<FREWARD>	Reward;

	Reward.Append(colosseum_season_done_rp.gradeRewards);
	Reward.Append(colosseum_season_done_rp.rankRewards);

	UUIFunctionLibrary::ShowCommonRewardPopupByRewards(Reward);

	instance->ColosseumManager->CS_SaveData.Clear();

	instance->ColosseumManager->ColosseumStatus = colosseum_season_done_rp.colosseumStatus;
	instance->ColosseumManager->ColosseumUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnCOLOSSEUM_RANKING_LIST_RP(FCOLOSSEUM_RANKING_LIST_RP& colosseum_ranking_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_ranking_list_rp.type, colosseum_ranking_list_rp.is, colosseum_ranking_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}

	instance->ColosseumManager->SetColoseumRanking(colosseum_ranking_list_rp.rankingList);
	instance->ColosseumManager->ColosseumUI.Broadcast();

	return true;
}

//////////////////////////////////////////////////////////////////////////
// BattleGround
//////////////////////////////////////////////////////////////////////////
// bool UReceivePacketHandler::OnPVP_BATTLE_GROUND_RANKING_RP(FPVP_BATTLE_GROUND_RANKING_RP& pvp_battle_ground_ranking_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)pvp_battle_ground_ranking_rp.type, pvp_battle_ground_ranking_rp.is, pvp_battle_ground_ranking_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 		return false;
// 	}
// // 	instance->RealTimeModManager->SetBattleGroundRanking(pvp_battle_ground_ranking_rp.bg_ranking);
// // 	instance->RealTimeModManager->SetBattleGroundMyRankinng(pvp_battle_ground_ranking_rp.bg_user);
// // 	instance->EventManager->OnBattleGroundRankList.Broadcast();
// 	return true;
// }


//////////////////////////////////////////////////////////////////////////

// bool UReceivePacketHandler::OnHERO_USE_EP_RP(FHERO_USE_EP_RP& hero_use_ep_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_use_ep_rp.type, hero_use_ep_rp.is, hero_use_ep_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->UserInfo->SetAvatarData(hero_use_ep_rp.avatar);
// 	instance->HeroInventory->UpdateInventoryData(hero_use_ep_rp.hero);
// 
// 	instance->EventManager->OnResponse_UseExpPotion.Broadcast();
// 	return true;
// }

bool UReceivePacketHandler::OnGET_TARGET_SERVER_RP(FGET_TARGET_SERVER_RP& get_target_server_rp, URGameInstance* instance)
{
	// This is a prelogin process so PacketErrorManager might be null
	if (get_target_server_rp.is != ECRUD::CRUD_OK)
	{
		URAuth::GetInstancePtr()->OnGetTargetServerRp(false, get_target_server_rp.error_info.code, nullptr);

		return false;
	}

	FReleaseTarget ReleaseTarget;
	ReleaseTarget.Env = get_target_server_rp.env;
	ReleaseTarget.ServerHost = get_target_server_rp.serverHost;
	ReleaseTarget.Cdn = get_target_server_rp.cdn;
	ReleaseTarget.TcpServerHost = get_target_server_rp.tcpServerHost;
	ReleaseTarget.TcpServerPort = get_target_server_rp.tcpServerPort;
	ReleaseTarget.ManifestBuildNumber = get_target_server_rp.manifestBuildNumber;
	UE_LOG(LogRTS, Log, TEXT("ENTER OnGET_TARGET_SERVER_RP[ TargetHost [%s], TargetPort [%s]"), *(get_target_server_rp.tcpServerHost), *(get_target_server_rp.tcpServerPort));
	URAuth::GetInstancePtr()->OnGetTargetServerRp(true, get_target_server_rp.error_info.code, &ReleaseTarget);

	return true;
}


bool UReceivePacketHandler::OnWORLDBOSS_STATUS_RP(FWORLDBOSS_STATUS_RP& worldboss_status_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnWORLDBOSS_PLAY_CHECK_RP(FWORLDBOSS_PLAY_CHECK_RP& worldboss_play_check_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnWORLDBOSS_PLAY_RESULT_RP(FWORLDBOSS_PLAY_RESULT_RP& worldboss_play_result_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnGET_REFER_LIST_RP(FGET_REFER_LIST_RP& get_refer_list_rp, URGameInstance* instance)
{
	return true;
}

// bool UReceivePacketHandler::OnGET_CONTENTS_UNLOCK_RP(FGET_CONTENTS_UNLOCK_RP& get_contents_unlock_rp, URGameInstance* instance)
// {
// 	instance->ContentsUnLockManager->ReceiveContentUnlockedList(get_contents_unlock_rp.unlocked);
// 	return true;
// }



//////////////////////////////////////////////////////////////////////////
// GUILD
//////////////////////////////////////////////////////////////////////////

bool UReceivePacketHandler::OnGUILD_CREATE_RP(FGUILD_CREATE_RP& guild_create_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_create_rp.type, guild_create_rp.is, guild_create_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, guild_create_rp.update);

	instance->GuildInfo->SetGuildData(guild_create_rp.guild_info);
	instance->GuildInfo->MyGuildExist = true;
	instance->HttpClient->OnGuildCreateRpDelegate.Broadcast();

	return true;
}

// bool UReceivePacketHandler::OnHERO_LIST_ENABLED_RP(FHERO_LIST_ENABLED_RP& hero_list_enabled_rp, URGameInstance* instance)
// {
// 	return true;
// }

bool UReceivePacketHandler::OnGUILD_CLOSE_RP(FGUILD_CLOSE_RP& guild_close_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_close_rp.type, guild_close_rp.is, guild_close_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	if (guild_close_rp.success)
	{
		instance->GuildInfo->ClearGuildData();

		instance->EventManager->OnRefreshGuildInfoUI.Broadcast();
	}

	return true;
}

bool UReceivePacketHandler::OnGUILD_GET_RP(FGUILD_GET_RP& guild_get_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_get_rp.type, guild_get_rp.is, guild_get_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->SetGuildMine(guild_get_rp.guild_mine);
	
	return true;
}

bool UReceivePacketHandler::OnGUILD_MEMBER_LIST_RP(FGUILD_MEMBER_LIST_RP& guild_member_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_member_list_rp.type, guild_member_list_rp.is, guild_member_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	bool ApplyData = instance->GuildInfo->SetGuildMemberList(guild_member_list_rp.guild_member_list);

	instance->EventManager->OnRefreshGuildMemberUI.Broadcast(ApplyData);
	return true;
}

bool UReceivePacketHandler::OnGUILD_AWAITER_LIST_RP(FGUILD_AWAITER_LIST_RP& guild_awaiter_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_awaiter_list_rp.type, guild_awaiter_list_rp.is, guild_awaiter_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	bool ApplyData = instance->GuildInfo->SetGuildAwaiterList(guild_awaiter_list_rp.guild_awaiter_list);

	instance->HttpClient->OnGuildAwaiterListRpDelegate.Broadcast(ApplyData);

	return true;
}

bool UReceivePacketHandler::OnGUILD_AWAITER_ACCEPT_RP(FGUILD_AWAITER_ACCEPT_RP& guild_awaiter_accept_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_awaiter_accept_rp.type, guild_awaiter_accept_rp.is, guild_awaiter_accept_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->RemoveAwaiter(guild_awaiter_accept_rp.target_kId);
	instance->HttpClient->OnGuildAwaiterListRpDelegate.Broadcast(true);

	return true;
}

bool UReceivePacketHandler::OnGUILD_AWAITER_CANCEL_RP(FGUILD_AWAITER_CANCEL_RP& guild_awaiter_cancel_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_awaiter_cancel_rp.type, guild_awaiter_cancel_rp.is, guild_awaiter_cancel_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->HttpClient->OnGuildAwaiterCancelRpDelegate.Broadcast(guild_awaiter_cancel_rp.guild_id);

	return true;
}

bool UReceivePacketHandler::OnGUILD_FIND_NAME_RP(FGUILD_FIND_NAME_RP& guild_find_name_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_find_name_rp.type, guild_find_name_rp.is, guild_find_name_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->SetFindGuildList(guild_find_name_rp.guild_list);
	instance->GuildInfo->SetWaitingGuildIDs(guild_find_name_rp.awaiting_guild_ids);

	instance->HttpClient->OnGuildFindNameRpDelegate.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_CHANGE_NAME_RP(FGUILD_CHANGE_NAME_RP& guild_change_name_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_change_name_rp.type, guild_change_name_rp.is, guild_change_name_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError(false);

		return false;
	}

	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_String0007"))));

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, guild_change_name_rp.update);

	instance->GuildInfo->SetGuildData(guild_change_name_rp.guild_info);

	instance->EventManager->OnRefreshGuildInfoUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_EDIT_DESC_RP(FGUILD_EDIT_DESC_RP& guild_edit_desc_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_edit_desc_rp.type, guild_edit_desc_rp.is, guild_edit_desc_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError(false);

		return false;
	}

	instance->GuildInfo->SetGuildData(guild_edit_desc_rp.guild_info);

	instance->EventManager->OnRefreshGuildInfoUI.Broadcast();

	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, TEXT("Notify")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_ChangeIntroduce")));

	return true;
}

bool UReceivePacketHandler::OnGUILD_EDIT_NOTICE_RP(FGUILD_EDIT_NOTICE_RP& guild_edit_notice_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_edit_notice_rp.type, guild_edit_notice_rp.is, guild_edit_notice_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError(false);

		return false;
	}

	instance->GuildInfo->SetGuildData(guild_edit_notice_rp.guild_info);

	instance->EventManager->OnRefreshGuildInfoUI.Broadcast();

	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, TEXT("Notify")),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_ChangeNotice")));

	return true;
}

bool UReceivePacketHandler::OnGUILD_EDIT_AUTO_GRADE_UP_RP(FGUILD_EDIT_AUTO_GRADE_UP_RP& guild_edit_auto_grade_up_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_edit_auto_grade_up_rp.type, guild_edit_auto_grade_up_rp.is, guild_edit_auto_grade_up_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_String0009"))));

	instance->GuildInfo->SetGuildData(guild_edit_auto_grade_up_rp.guild_info);

	instance->EventManager->OnRefreshGuildInfoUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_JOIN_RP(FGUILD_JOIN_RP& guild_join_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_join_rp.type, guild_join_rp.is, guild_join_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->HttpClient->OnGuildJoinRpDelegate.Broadcast(guild_join_rp.guild_id, guild_join_rp.auto_grade_up);

	return true;
}

bool UReceivePacketHandler::OnGUILD_JOIN_AUTO_RP(FGUILD_JOIN_AUTO_RP& guild_join_auto_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_join_auto_rp.type, guild_join_auto_rp.is, guild_join_auto_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->HttpClient->OnGuildJoinRpDelegate.Broadcast(guild_join_auto_rp.guild_id, guild_join_auto_rp.auto_grade_up);

	return true;
}

bool UReceivePacketHandler::OnGUILD_MEMBER_GRADE_RP(FGUILD_MEMBER_GRADE_RP& guild_member_grade_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_member_grade_rp.type, guild_member_grade_rp.is, guild_member_grade_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError(false);

		return false;
	}

	instance->GuildInfo->UpdateGuildMemberData(guild_member_grade_rp.guild_member);
	instance->GuildInfo->UpdateGuildMemberData(guild_member_grade_rp.target_member);
	instance->EventManager->OnRefreshGuildMemberUI.Broadcast(true);
	instance->EventManager->AfterChangeGrade.Broadcast(guild_member_grade_rp.target_member.nick, (int32)guild_member_grade_rp.target_member.grade);

	return true;
}

bool UReceivePacketHandler::OnGUILD_MEMBER_KICK_RP(FGUILD_MEMBER_KICK_RP& guild_member_kick_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_member_kick_rp.type, guild_member_kick_rp.is, guild_member_kick_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError(false);

		return false;
	}
	FGUILD_MEMBER* memberInfo = instance->GuildInfo->MyGuildMemberList.FindByPredicate([&](const FGUILD_MEMBER& GuildInfo) {
		return GuildInfo.kId == guild_member_kick_rp.target_kId;
	});
	if (memberInfo != nullptr) {
		FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_GuildReport_Kick"));
		FFormatArgumentValue FormatArg1 = FText::FromString(instance->UserInfo->Avatar.nick);
		FFormatArgumentValue FormatArg2 = FText::FromString(memberInfo->nick);
		LocalizedText = FText::Format(LocalizedText, FormatArg1, FormatArg2);

		if(instance->UserInfo->Avatar.nick == memberInfo->nick)
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), 
				FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error14")), FText::FromString(instance->UserInfo->Avatar.nick)));
		else
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),LocalizedText);
	}

	if (instance->HttpClient->IsValidKID(guild_member_kick_rp.target_kId))
	{
		instance->EventManager->OnRequestGuildInfo.Broadcast();
	}
	else
	{
		instance->EventManager->OnRequestGuildMember.Broadcast();
	}
	
	return true;
}

bool UReceivePacketHandler::OnGUILD_CONTRIBUTION_RP(FGUILD_CONTRIBUTION_RP& guild_contribution_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_contribution_rp.type, guild_contribution_rp.is, guild_contribution_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, guild_contribution_rp.update);

	instance->GuildInfo->SetGuildData(guild_contribution_rp.guild_info);
	instance->GuildInfo->UpdateGuildMemberData(guild_contribution_rp.guild_member);
	instance->GuildInfo->Donation_contributionType = guild_contribution_rp.contributionType;
	instance->GuildInfo->Donation_contributable = EREWARDABLE_STATUS::REWARDED;

	instance->EventManager->OnRefreshGuildDonationUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_CONTRIBUTE_RANKING_RP(FGUILD_CONTRIBUTE_RANKING_RP& guild_contribute_ranking_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_contribute_ranking_rp.type, guild_contribute_ranking_rp.is, guild_contribute_ranking_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	bool ApplyData = instance->GuildInfo->SetGuildRankingList(guild_contribute_ranking_rp.guild_list);

	instance->HttpClient->OnGuildContributeRankingRpDelegate.Broadcast(ApplyData);

	return true;
}

bool UReceivePacketHandler::OnGUILD_CONTRIBUTION_STATUS_RP(FGUILD_CONTRIBUTION_STATUS_RP& guild_contribution_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_contribution_status_rp.type, guild_contribution_status_rp.is, guild_contribution_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->Donation_contributable = guild_contribution_status_rp.contributable;
	instance->GuildInfo->Donation_contributionType = guild_contribution_status_rp.contributionType;

	instance->EventManager->OnRefreshGuildDonationUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_ATTENDANCE_STATUS_RP(FGUILD_ATTENDANCE_STATUS_RP& guild_attendance_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_attendance_status_rp.type, guild_attendance_status_rp.is, guild_attendance_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->Attendance_dailyRewardable = guild_attendance_status_rp.dailyRewardable;
	instance->GuildInfo->Attendance_weeklyRewardable = guild_attendance_status_rp.weeklyRewardable;
	instance->GuildInfo->Attendance_weeklyCount = guild_attendance_status_rp.weeklyCount;
	instance->GuildInfo->Attendance_weeklyTimeLeft = guild_attendance_status_rp.weeklyTimeLeft;

	instance->EventManager->OnRefreshGuildAttendanceUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_GET_GUEST_RP(FGUILD_GET_GUEST_RP& guild_get_guest_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_get_guest_rp.type, guild_get_guest_rp.is, guild_get_guest_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	instance->GuildInfo->SetGuestGuildData(guild_get_guest_rp.guild_info);
	instance->GuildInfo->SetGuestGuildMemberList(guild_get_guest_rp.guild_member_list);
	instance->EventManager->OnReceiveGuestGuildInfo.Broadcast();
	return true;
}

bool UReceivePacketHandler::OnADD_REFER_PUBLISHER_RP(FADD_REFER_PUBLISHER_RP& add_refer_publisher_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)add_refer_publisher_rp.type, add_refer_publisher_rp.is, add_refer_publisher_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	//add_refer_publisher_rp.clientkey;
	return true;
}

bool UReceivePacketHandler::OnGUILD_ATTENDANCE_DAILY_REWARD_RP(FGUILD_ATTENDANCE_DAILY_REWARD_RP& guild_attendance_daily_reward_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_attendance_daily_reward_rp.type, guild_attendance_daily_reward_rp.is, guild_attendance_daily_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->Attendance_dailyRewardable = guild_attendance_daily_reward_rp.dailyRewardable;
	instance->GuildInfo->Attendance_weeklyCount = guild_attendance_daily_reward_rp.weeklyCount;
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, guild_attendance_daily_reward_rp.update);

	UUIFunctionLibrary::ShowRewardPopupByRewards(guild_attendance_daily_reward_rp.rewards);

	instance->EventManager->OnRefreshGuildAttendanceUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_ATTENDANCE_WEEKLY_REWARD_RP(FGUILD_ATTENDANCE_WEEKLY_REWARD_RP& guild_attendance_weekly_reward_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_attendance_weekly_reward_rp.type, guild_attendance_weekly_reward_rp.is, guild_attendance_weekly_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->Attendance_weeklyRewardable = guild_attendance_weekly_reward_rp.weeklyRewardable;
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, guild_attendance_weekly_reward_rp.update);

	UUIFunctionLibrary::ShowRewardPopupByRewards(guild_attendance_weekly_reward_rp.rewards);

	instance->EventManager->OnRefreshGuildAttendanceUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_RUNE_STATUS_RP(FGUILD_RUNE_STATUS_RP& guild_rune_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_rune_status_rp.type, guild_rune_status_rp.is, guild_rune_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->SetGuildRuneRequesterList(guild_rune_status_rp.requests);

	instance->HttpClient->OnGuildRuneStatusRpDelegate.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnATTENDANCE_GET_RP(FATTENDANCE_GET_RP& attendance_get_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)attendance_get_rp.type, attendance_get_rp.is, attendance_get_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	
	instance->AttendanceManager->SetAttendanceInfo(attendance_get_rp.chart);

	return true;
}

bool UReceivePacketHandler::OnGUILD_RUNE_REQUEST_RP(FGUILD_RUNE_REQUEST_RP& guild_rune_request_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_rune_request_rp.type, guild_rune_request_rp.is, guild_rune_request_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->SetGuildRuneRequesterList(guild_rune_request_rp.requests);
	FGUILD_RUNE_REQUESTER MyRuneRequest;
	if (instance->GuildInfo->GetMyRuneRequest(MyRuneRequest))
	{
		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(MyRuneRequest.heroId);
		FText heroName;
		if (CharacterTableInfo)
		{
			heroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName);
		}
		FText Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Menu49"));
		Caption = FText::Format(Caption, heroName);
		FText Contents;
		UUIFunctionLibrary::ShowCommonPopupHeroRuneSplash(MyRuneRequest.heroId, Caption, Contents);
	}
	instance->HttpClient->OnGuildRuneStatusRpDelegate.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_RUNE_SUPPORT_RP(FGUILD_RUNE_SUPPORT_RP& guild_rune_support_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_rune_support_rp.type, guild_rune_support_rp.is, guild_rune_support_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	
	//UUIFunctionLibrary::ShowRewardPopupByRewardsHero(instance, guild_rune_support_rp.update.heros, guild_rune_support_rp.rewards);
	
// 	FName heroID;
// 	if (guild_rune_support_rp.update.heros.IsValidIndex(0))
// 	{
// 		heroID = guild_rune_support_rp.update.heros[0].heroId;
// 	}
// 	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(heroID);
// 	FText heroName;
// 
// 	if (CharacterTableInfo)
// 	{
// 		heroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName);
// 		FText Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Menu50"));
// 		Caption = FText::Format(Caption, heroName);
// 		FText Contents;
// 		instance->RWidgetManager->ShowCommonPopupHeroRuneSplash(heroID, Caption, Contents);
// 	}
// 
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, guild_rune_support_rp.update);
// 
// 	instance->GuildInfo->SetGuildRuneRequesterList(guild_rune_support_rp.requests);
// 
// 	instance->HttpClient->OnGuildRuneStatusRpDelegate.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_RUNE_GET_RP(FGUILD_RUNE_GET_RP& guild_rune_get_rp, URGameInstance* instance)
{
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_rune_get_rp.type, guild_rune_get_rp.is, guild_rune_get_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, guild_rune_get_rp.update);
// 
// 	instance->GuildInfo->SetGuildRuneRequesterList(guild_rune_get_rp.requests);
// 
// 	UUIFunctionLibrary::ShowRewardPopupByRewardsHero(instance, guild_rune_get_rp.update.heros, guild_rune_get_rp.rewards);
// 
// 	instance->HttpClient->OnGuildRuneStatusRpDelegate.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnGUILD_HISTORY_RP(FGUILD_HISTORY_RP& guild_history_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_history_rp.type, guild_history_rp.is, guild_history_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->GuildInfo->SetGuildHistory(guild_history_rp.historyList);
	
	instance->HttpClient->OnGuildHistoryRpDelegate.Broadcast();
	
	return true;
}

bool UReceivePacketHandler::OnGUILD_LEVEL_UP_RP(FGUILD_LEVEL_UP_RP& guild_level_up_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)guild_level_up_rp.type, guild_level_up_rp.is, guild_level_up_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError(false);

		return false;
	}

	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_LevelUp"));
	FFormatArgumentValue FormatArg1 = FText::AsNumber(guild_level_up_rp.guild_info.level);
	LocalizedText = FText::Format(LocalizedText, FormatArg1);
	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), LocalizedText);
	instance->GuildInfo->SetGuildData(guild_level_up_rp.guild_info);
	instance->EventManager->OnRefreshGuildInfoUI.Broadcast();

	return true;
}


//////////////////////////////////////////////////////////////////////////
// COMMUNITY
//////////////////////////////////////////////////////////////////////////

bool UReceivePacketHandler::OnCOMMUNITY_ARTICLE_WRITE_HERO_RP(FCOMMUNITY_ARTICLE_WRITE_HERO_RP& community_article_write_hero_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnBILLING_TRANSACTION_SUCCESS_RP(FBILLING_TRANSACTION_SUCCESS_RP& billing_transaction_success_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)billing_transaction_success_rp.type, billing_transaction_success_rp.is, billing_transaction_success_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		instance->BillingManager->OnBillingTransactionSuccessRp(false, billing_transaction_success_rp.update.avatar);

		return false;
	}
	else
	{
		instance->BillingManager->OnBillingTransactionSuccessRp(true, billing_transaction_success_rp.update.avatar);
	}
	return true;
}

bool UReceivePacketHandler::OnCOLOSSEUM_STATUS_RP(FCOLOSSEUM_STATUS_RP& colosseum_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)colosseum_status_rp.type, colosseum_status_rp.is, colosseum_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();
		return false;
	}
	instance->ColosseumManager->CurrentToPrevStatus();
	instance->ColosseumManager->SetColosseumStatus(colosseum_status_rp.colosseumStatus);
	instance->ColosseumManager->ColosseumUI.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnCOMMUNITY_ARTICLE_DELETE_HERO_RP(FCOMMUNITY_ARTICLE_DELETE_HERO_RP& community_article_delete_hero_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnCOMMUNITY_ARTICLE_VOTE_HERO_RP(FCOMMUNITY_ARTICLE_VOTE_HERO_RP& community_article_vote_hero_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnCOMMUNITY_ARTICLE_LIST_HERO_RP(FCOMMUNITY_ARTICLE_LIST_HERO_RP& community_article_list_hero_rp, URGameInstance* instance)
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
// BattleField
//////////////////////////////////////////////////////////////////////////

bool UReceivePacketHandler::OnCHANNEL_INFO_LIST_RP(FCHANNEL_INFO_LIST_RP& channel_info_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)channel_info_list_rp.type, channel_info_list_rp.is, channel_info_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

//	instance->BattleFieldManager->SetChannelInfoList(channel_info_list_rp.channel_list);
	instance->HttpClient->OnChannelInfoListRpDelegate.Broadcast();
	
	return true;
}

bool UReceivePacketHandler::OnCHANNEL_DETAIL_RP(FCHANNEL_DETAIL_RP& channel_detail_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)channel_detail_rp.type, channel_detail_rp.is, channel_detail_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

//	instance->BattleFieldManager->SetChannelDetail(channel_detail_rp.channel_detail);
	instance->HttpClient->OnChannelDetailRpDelegate.Broadcast();

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ROA

// bool UReceivePacketHandler::OnROA_RUNES_RP(FROA_RUNES_RP& roa_runes_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)roa_runes_rp.type, roa_runes_rp.is, roa_runes_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->UserInfo->SetAvatarData(roa_runes_rp.avatar);
// 	instance->HeroRuneDungeonManager->UpdateRuneDungeonStatus(roa_runes_rp.episodes, roa_runes_rp.rotation, roa_runes_rp.left );
// 	instance->HttpClient->OnRecvHeroRuneInfoDelegate.Broadcast();
// 
// 	UUP_Lobby* Lobby = Cast<UUP_Lobby>(instance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Lobby));
// 	if (IsValid(Lobby))
// 	{
// 		Lobby->SetHeroRuneEpisode(roa_runes_rp.episodes, roa_runes_rp.left);
// 	}
// 
// 	return true;
// }

// bool UReceivePacketHandler::OnITEM_SALE_STACK_RP(FITEM_SALE_STACK_RP& item_sale_stack_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_sale_stack_rp.type, item_sale_stack_rp.is, item_sale_stack_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 
// 
// 	if (item_sale_stack_rp.is == ECRUD::CRUD_OK)
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_sale_stack_rp.kId))
// 			return false;
// 
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_sale_stack_rp.update);
// 
// 		UUIFunctionLibrary::ShowRewardPopupByRewards(instance, item_sale_stack_rp.rewards);
// 	}
// 
// 	instance->HttpClient->OnItemSaleStackRpDelegate.Broadcast((item_sale_stack_rp.is == ECRUD::CRUD_OK));
// 
// 
// 	return true;
// }


// bool UReceivePacketHandler::OnROA_TUTORIAL_PROGRESS_RP(FROA_TUTORIAL_PROGRESS_RP& roa_tutorial_progress_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)roa_tutorial_progress_rp.type, roa_tutorial_progress_rp.is, roa_tutorial_progress_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (!instance->HttpClient->IsValidKID(roa_tutorial_progress_rp.kId))
// 		return false;
// 
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, roa_tutorial_progress_rp.update);
// // 	instance->ForceTutorialManager->nCurrentTutorialStep = roa_tutorial_progress_rp.update.inventoryUpdate.tut;
// 
// 	return true;
// }

bool UReceivePacketHandler::OnACCOUNT_CHANGE_LANGUAGE_RP(FACCOUNT_CHANGE_LANGUAGE_RP& account_change_language_rp, URGameInstance* instance)
{
	return true;
}

bool UReceivePacketHandler::OnCLIENT_SDK_ACTION_RP(FCLIENT_SDK_ACTION_RP& client_sdk_action_rp, URGameInstance* instance)
{
	UE_LOG(LogRTS, Log, TEXT("ENTER OnCLIENT_SDK_ACTION_RP"));

	if (instance->PacketErrorManager->IsCheckError((EPacketType)client_sdk_action_rp.type, client_sdk_action_rp.is, client_sdk_action_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UE_LOG(LogRTS, Log, TEXT("OnCLIENT_SDK_ACTION_RP: SetAvatarData"));

	instance->UserInfo->SetAvatarData(client_sdk_action_rp.avatar);

	return true;
}

bool UReceivePacketHandler::OnARENA_STATUS_RP(FARENA_STATUS_RP& arena_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_status_rp.type, arena_status_rp.is, arena_status_rp.error_info) == true)
	{
		//instance->PacketErrorManager->ProcessError();

		return false;
	}
	instance->ArenaManager->SetArenaInfo(arena_status_rp.arenaStatus);

	instance->ArenaManager->onACK_ARENA_STATUS.ExecuteIfBound();
	return true;
}

bool UReceivePacketHandler::OnARENA_TARGET_INFO_RP(FARENA_TARGET_INFO_RP& arena_target_info_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)arena_target_info_rp.type, arena_target_info_rp.is, arena_target_info_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, arena_target_info_rp.update);

	instance->ArenaManager->SetTargetInfo(arena_target_info_rp.target);
	instance->ArenaManager->SetDefenceSetting(false);

	if (instance->RWidgetManager->GetCurrentUIPage() == EUIPageEnum::UIPage_ArenaDeckSetting)
	{
		auto widget = instance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ArenaDeckSetting);
		UASyncPVPDeckSetting* ArenaDeckSetting = Cast<UASyncPVPDeckSetting>(widget);
		ArenaDeckSetting->Renderer.Prepare();
		ArenaDeckSetting->Renderer.Render();
	}
	else
		instance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ArenaDeckSetting);

	return true;
}

// bool UReceivePacketHandler::OnAUTH_CHAT_RP(FAUTH_CHAT_RP& auth_chat_rp, URGameInstance* instance)
// {
// 	return true;
// }




// bool UReceivePacketHandler::OnROA_STATUS_RP(FROA_STATUS_RP& roa_status_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)roa_status_rp.type, roa_status_rp.is, roa_status_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	instance->QuestManager->SetQuestData(roa_status_rp.roa);
// //	instance->ForceTutorialManager->SetROA_STATUS_RP(roa_status_rp.roa);
// 	
// 	return true;
// }

// bool UReceivePacketHandler::OnHERO_UNLOCK_RP(FHERO_UNLOCK_RP& hero_unlock_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_unlock_rp.type, hero_unlock_rp.is, hero_unlock_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError(false);
// 
// 		return false;
// 	}
// 
// 	if (hero_unlock_rp.update.heros.IsValidIndex(0))
// 		UUIFunctionLibrary::ShowHero(instance, hero_unlock_rp.update.heros[0], true);
// 
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, hero_unlock_rp.update);
// 	
// 
// 	return true;
// }

bool UReceivePacketHandler::OnCONTENT_RESET_STATUS_RP(FCONTENT_RESET_STATUS_RP& content_reset_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)content_reset_status_rp.type, content_reset_status_rp.is, content_reset_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	instance->HttpClient->OnContentResetStatusRpDelegate.Broadcast(content_reset_status_rp.contentResetCount);

	return true;
}

bool UReceivePacketHandler::OnCONTENT_RESET_RP(FCONTENT_RESET_RP& content_reset_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)content_reset_rp.type, content_reset_rp.is, content_reset_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, content_reset_rp.update);

	return true;
}


// bool UReceivePacketHandler::OnDAILYMISSION_STATUS_RP(FDAILYMISSION_STATUS_RP& dailymission_status_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)dailymission_status_rp.type, dailymission_status_rp.is, dailymission_status_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 		return false;
// 	}
// 
// 	instance->MissionManager->SetMissionStatus(dailymission_status_rp.status);
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnDAILYMISSION_REWARD_RP(FDAILYMISSION_REWARD_RP& dailymission_reward_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)dailymission_reward_rp.type, dailymission_reward_rp.is, dailymission_reward_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 		return false;
// 	}
// 
// 	UUIFunctionLibrary::ShowRewardPopupByRewards(dailymission_reward_rp.rewards);
// 	instance->MissionManager->SetMissionStatus(dailymission_reward_rp.status);
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, dailymission_reward_rp.update);
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnDAILYMISSION_ADD_RP(FDAILYMISSION_ADD_RP& dailymission_add_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)dailymission_add_rp.type, dailymission_add_rp.is, dailymission_add_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 		return false;
// 	}
// 	
// 	instance->MissionManager->SetMissionStatus(dailymission_add_rp.status);
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, dailymission_add_rp.update);
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnDAILYMISSION_RESET_RP(FDAILYMISSION_RESET_RP& dailymission_reset_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)dailymission_reset_rp.type, dailymission_reset_rp.is, dailymission_reset_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 		return false;
// 	}
// 
// 	instance->MissionManager->SetMissionStatus(dailymission_reset_rp.status);
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, dailymission_reset_rp.update);
// 
// 	return true;
// }

bool UReceivePacketHandler::OnAVATAR_PROFILE_RP(FAVATAR_PROFILE_RP& avatar_profile_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_profile_rp.type, avatar_profile_rp.is, avatar_profile_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->UserInfo->SetAvatarProfile(avatar_profile_rp.records, avatar_profile_rp.heroUse);

	return true;
}

bool UReceivePacketHandler::OnAVATAR_ABILITY_UP_RP(FAVATAR_ABILITY_UP_RP& avatar_ability_up_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_ability_up_rp.type, avatar_ability_up_rp.is, avatar_ability_up_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError(false);

		return false;
	}
	
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, avatar_ability_up_rp.update);
	instance->EventManager->OnRequestReceive.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnAVATAR_ABILITY_RESET_RP(FAVATAR_ABILITY_RESET_RP& avatar_ability_reset_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_ability_reset_rp.type, avatar_ability_reset_rp.is, avatar_ability_reset_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	UUtilFunctionLibrary::UpdateGoodsInfo(instance, avatar_ability_reset_rp.update);
	instance->SetIsResetSkill(true);
	instance->EventManager->OnUpdateUserInfo.Broadcast();

	return true;
}

bool UReceivePacketHandler::OnAVATAR_ICON_SET_RP(FAVATAR_ICON_SET_RP& avatar_icon_set_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_icon_set_rp.type, avatar_icon_set_rp.is, avatar_icon_set_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, avatar_icon_set_rp.update);

	return true;
}

bool UReceivePacketHandler::OnAVATAR_ICON_LIST_RP(FAVATAR_ICON_LIST_RP& avatar_icon_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_icon_list_rp.type, avatar_icon_list_rp.is, avatar_icon_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->UserInfo->SetAvatarIconList(avatar_icon_list_rp.iconList);

	return true;
}

// bool UReceivePacketHandler::OnHERO_LEVEL_UP_RP(FHERO_LEVEL_UP_RP& hero_level_up_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_level_up_rp.type, hero_level_up_rp.is, hero_level_up_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	
// 	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero_level_up_rp.hero.heroId);
// 	if (CharacterTableInfo)
// 	{
// 		FText heroText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName);
// 		FText text = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("HeroLevelUp_PopUp_02")), heroText, hero_level_up_rp.hero.level);
// 
// 		instance->RWidgetManager->ShowCommonPopupHeroRuneSplash(hero_level_up_rp.hero.heroId, text, FText::FromString(TEXT("")), true);
// 	}
// 	instance->HeroInventory->UpdateInventoryData(hero_level_up_rp.hero);
// 
// 	instance->EventManager->OnResponse_UseExpPotion.Broadcast();
// 
// 	return true;
// }

bool UReceivePacketHandler::OnDAILY_QUEST_STATUS_RP(FDAILY_QUEST_STATUS_RP& daily_quest_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)daily_quest_status_rp.type, daily_quest_status_rp.is, daily_quest_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}

	instance->QuestManager->SetDailyQuestStatus(daily_quest_status_rp.daily_quest_status);

	return true;
}

bool UReceivePacketHandler::OnDAILY_QUEST_REWARD_RP(FDAILY_QUEST_REWARD_RP& daily_quest_reward_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)daily_quest_reward_rp.type, daily_quest_reward_rp.is, daily_quest_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, daily_quest_reward_rp.update);

// 	UUIFunctionLibrary::ShowCommonPopupSplash(
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_RewardMessage")));

	/*UUIFunctionLibrary::ShowCommonPopup(instance->GetWorld(),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_RewardMessage")),
		ECommonPopupType::VE_OK);*/

	UUIFunctionLibrary::ShowCommonRewardPopupByRewards(daily_quest_reward_rp.rewards);

	instance->QuestManager->SetDailyQuestStatus(daily_quest_reward_rp.daily_quest_status);
	UUP_Lobby_Outdoor* OutdoorUMG = Cast<UUP_Lobby_Outdoor>(instance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_LobbyOutdoor));
	if(OutdoorUMG)
		OutdoorUMG->SetDailyQuest();
	return true;
}

//=============================================================================================
// CRUSADER
//=============================================================================================
// bool UReceivePacketHandler::OnCRUSADER_STATUS_RP(FCRUSADER_STATUS_RP& crusader_status_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)crusader_status_rp.type, crusader_status_rp.is, crusader_status_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (IsValid(instance->CrusaderManager))
// 	{
// 		instance->CrusaderManager->OnCRUSADER_STATUS_RP(crusader_status_rp);
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCRUSADER_GET_PLAYER_RP(FCRUSADER_GET_PLAYER_RP& crusader_get_player_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)crusader_get_player_rp.type, crusader_get_player_rp.is, crusader_get_player_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (IsValid(instance->CrusaderManager))
// 	{
// 		instance->CrusaderManager->OnCRUSADER_GET_PLAYER_RP(crusader_get_player_rp);
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCRUSADER_PLAY_START_RP(FCRUSADER_PLAY_START_RP& crusader_play_start_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)crusader_play_start_rp.type, crusader_play_start_rp.is, crusader_play_start_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 
// 	if (IsValid(instance->CrusaderManager))
// 	{
// 		instance->CrusaderManager->OnCRUSADER_PLAY_START_RP(crusader_play_start_rp);
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCRUSADER_PLAY_DONE_RP(FCRUSADER_PLAY_DONE_RP& crusader_play_done_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)crusader_play_done_rp.type, crusader_play_done_rp.is, crusader_play_done_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, crusader_play_done_rp.update);
// 
// 	instance->RewardInfo->SetRewardData(crusader_play_done_rp.rewards);
// 
// 	if (IsValid(instance->CrusaderManager))
// 	{
// 		instance->CrusaderManager->OnCRUSADER_PLAY_DONE_RP(crusader_play_done_rp);
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCRUSADER_REWARD_RP(FCRUSADER_REWARD_RP& crusader_reward_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)crusader_reward_rp.type, crusader_reward_rp.is, crusader_reward_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, crusader_reward_rp.update);
// 
// 	UUIFunctionLibrary::ShowRewardPopupByRewards(instance, crusader_reward_rp.rewards);
// 
// 	if (IsValid(instance->CrusaderManager))
// 	{
// 		instance->CrusaderManager->OnCRUSADER_REWARD_RP(crusader_reward_rp);
// 	}
// 
// 	return true;
// }
// 
// bool UReceivePacketHandler::OnCRUSADER_RESET_RP(FCRUSADER_RESET_RP& crusader_reset_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)crusader_reset_rp.type, crusader_reset_rp.is, crusader_reset_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	UUtilFunctionLibrary::UpdateGoodsInfo(instance, crusader_reset_rp.update);
// 
// 	if (IsValid(instance->CrusaderManager))
// 	{
// 		instance->CrusaderManager->OnCRUSADER_RESET_RP(crusader_reset_rp);
// 	}
// 
// 	return true;
// }

bool UReceivePacketHandler::OnSPECIAL_GIFT_LIST_RP(FSPECIAL_GIFT_LIST_RP& special_gift_list_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)special_gift_list_rp.type, special_gift_list_rp.is, special_gift_list_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		if (!instance->HttpClient->IsValidKID(special_gift_list_rp.kId))
			return false;
		instance->MailManager->UpdateSpecialMailList(special_gift_list_rp.mails);
	}

	return true;
}

bool UReceivePacketHandler::OnSPECIAL_GIFT_OPEN_RP(FSPECIAL_GIFT_OPEN_RP& special_gift_open_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)special_gift_open_rp.type, special_gift_open_rp.is, special_gift_open_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		if (!instance->HttpClient->IsValidKID(special_gift_open_rp.kId))
			return false;

		UUtilFunctionLibrary::UpdateGoodsInfo(instance, special_gift_open_rp.update);

		UUIFunctionLibrary::ShowRewardPopupByRewards(special_gift_open_rp.rewards);
		
		instance->MailManager->OnSpecialGiftOpenRp.Broadcast(special_gift_open_rp.rewards);
	}

	return true;
}

bool UReceivePacketHandler::OnMIRROR_STATUS_RP(FMIRROR_STATUS_RP& mirror_status_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)mirror_status_rp.type, mirror_status_rp.is, mirror_status_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		instance->MirrorDungeonManager->SetMirrorStatus(mirror_status_rp.mirror_status);
	}

	return true;
}

bool UReceivePacketHandler::OnMIRROR_PLAY_START_RP(FMIRROR_PLAY_START_RP& mirror_play_start_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)mirror_play_start_rp.type, mirror_play_start_rp.is, mirror_play_start_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		UMirrorDungeonManager* MirrorManager = instance->MirrorDungeonManager;
		
		if (!instance->HttpClient->IsValidKID(mirror_play_start_rp.update.avatar.kId))
			return false;
		
		instance->OtherHeroManager->InsertOtherHeroData(MirrorManager->SelectedMirrorSlotData.user.kId, MirrorManager->SelectedMirrorSlotData.user.heros);

		UUtilFunctionLibrary::UpdateGoodsInfo(instance, mirror_play_start_rp.update);

		instance->UserInfo->SetAvatarData(mirror_play_start_rp.update.avatar);
		instance->EventManager->OnUpdateUserInfo.Broadcast();


		if (false == mirror_play_start_rp.playUD.IsEmpty())
		{
			MirrorManager->m_MirrorPlayUD = mirror_play_start_rp.playUD;
			instance->PlayModeManager->PlayModeMirrorDungeonStart();
			return true;
		}


	}

	return true;
}

bool UReceivePacketHandler::OnMIRROR_PLAY_DONE_RP(FMIRROR_PLAY_DONE_RP& mirror_play_done_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)mirror_play_done_rp.type, mirror_play_done_rp.is, mirror_play_done_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		UMirrorDungeonManager* MirrorManager = instance->MirrorDungeonManager;
		//UArenaManager* ArenaManager = instance->ArenaManager;
		//URInventory* RInventory = instance->RInventory;
		
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, mirror_play_done_rp.update);

		MirrorManager->SetMirrorStatus(mirror_play_done_rp.mirror_status);
		MirrorManager->SetGameResultRewards(mirror_play_done_rp.rewards);
		//MirrorManager->SetMirrorInfoResult(mirror_play_done_rp.mirror_status, PVPTokenAfter - PVPTokenBefore);
		MirrorManager->MirrorDoneRP();

	}

	return true;
}

bool UReceivePacketHandler::OnMIRROR_RESET_RP(FMIRROR_RESET_RP& mirror_reset_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)mirror_reset_rp.type, mirror_reset_rp.is, mirror_reset_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, mirror_reset_rp.update);
		UMirrorDungeonManager* MirrorManager = instance->MirrorDungeonManager;
		MirrorManager->SetMirrorStatus(mirror_reset_rp.mirror_status);
// 		mirror_reset_rp.mirror_status;
// 		mirror_reset_rp.update;
// 		mirror_reset_rp.
	}

	return true;
}

bool UReceivePacketHandler::OnMIRROR_DEFENSE_DECK_SET_RP(FMIRROR_DEFENSE_DECK_SET_RP& mirror_defense_deck_set_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)mirror_defense_deck_set_rp.type, mirror_defense_deck_set_rp.is, mirror_defense_deck_set_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	UUtilFunctionLibrary::UpdateGoodsInfo(instance, mirror_defense_deck_set_rp.update);

	instance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_MirrorDungeon);

	instance->MirrorDungeonManager->SetMirrorDefenceSetting(false);

	return true;
}

// bool UReceivePacketHandler::OnITEM_LEVEL_UP_RP(FITEM_LEVEL_UP_RP& item_level_up_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)item_level_up_rp.type, item_level_up_rp.is, item_level_up_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		if (!instance->HttpClient->IsValidKID(item_level_up_rp.kId))
// 			return false;
// 
// 		UUtilFunctionLibrary::UpdateGoodsInfo(instance, item_level_up_rp.update);
// 
// 		// UserInfoEvent
// 		instance->EventManager->OnUpdateUserInfo.Broadcast();
// 
// 		instance->EventManager->OnResponse_EquipAllHeroItem_HeroManagement.Broadcast();
// 	}
// 	return true;
// }

bool UReceivePacketHandler::OnAVATAR_LOBBY_RP(FAVATAR_LOBBY_RP& avatar_lobby_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)avatar_lobby_rp.type, avatar_lobby_rp.is, avatar_lobby_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
// 		instance->AchievementManager->SetAchievementStatus(avatar_lobby_rp.achieveStatus);
		instance->QuestManager->SetDailyQuestStatus(avatar_lobby_rp.daily_quest_status);
//		instance->ForceTutorialManager->SetROA_STATUS_RP(avatar_lobby_rp.roa);
// 		instance->QuestManager->SetQuestData(avatar_lobby_rp.roa);

// 		avatar_lobby_rp.treasure;
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, avatar_lobby_rp.update);
		// TODO: lobby
		//UUP_Lobby* Lobby = instance->RWidgetManager->GetUserWidgetT<UUP_Lobby>(EWidgetBluePrintEnum::WBP_Lobby);
		//if (Lobby)
		//{
		//	bool bGetTreasureMap = !(avatar_lobby_rp.treasure.treasureId.IsEmpty());
		//	instance->UserInfo->SetIsTreasureMap(bGetTreasureMap);
		//	
		//	bool bGetTreasureKey = (avatar_lobby_rp.treasure.key == 1);
		//	instance->UserInfo->SetIsTreasureKey(bGetTreasureKey);

		//	if (bGetTreasureMap)
		//	{
		//		int32 Second = avatar_lobby_rp.treasure.left;
		//		instance->UserInfo->SetTreasureTime(Second);

		//		FString TreasureID = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(*avatar_lobby_rp.treasure.treasureId)).ToString();
		//		instance->UserInfo->SetTreasureID(TreasureID);
		//	}

		//	Lobby->ChangeSwitcher_Quest(int32(bGetTreasureMap));
		//}
	}
	return true;
}

bool UReceivePacketHandler::OnTREASURE_GET_REWARD_RP(FTREASURE_GET_REWARD_RP& treasure_get_reward_rp, URGameInstance* instance)
{
	if (instance->PacketErrorManager->IsCheckError((EPacketType)treasure_get_reward_rp.type, treasure_get_reward_rp.is, treasure_get_reward_rp.error_info) == true)
	{
		instance->PacketErrorManager->ProcessError();

		return false;
	}
	else
	{
// 		treasure_get_reward_rp.treasure;
		//UUIFunctionLibrary::ShowRewardPopupByRewards(instance, treasure_get_reward_rp.rewards);
		
		UUtilFunctionLibrary::UpdateGoodsInfo(instance, treasure_get_reward_rp.update);

		TArray<FREWARDSET> RewardSets;
		FREWARDSET Element;
		Element.rewards = treasure_get_reward_rp.rewards;
		RewardSets.Emplace(Element);

		UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_TreasureMap, 0);
	}
	return true;
}

// bool UReceivePacketHandler::OnHERO_EPISODE_ROBBY_RP(FHERO_EPISODE_ROBBY_RP& hero_episode_robby_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_episode_robby_rp.type, hero_episode_robby_rp.is, hero_episode_robby_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		instance->HeroRuneDungeonManager->SetHeroEpisodesInfo(hero_episode_robby_rp.recent, hero_episode_robby_rp.episodes);
// 
// 		instance->EventManager->OnReceiveHeroRuneInfo.Broadcast();
// 	}
// 	return true;
// }

// bool UReceivePacketHandler::OnHERO_CONFIG_LIST_SET_RP(FHERO_CONFIG_LIST_SET_RP& hero_config_list_set_rp, URGameInstance* instance)
// {
// 	if (instance->PacketErrorManager->IsCheckError((EPacketType)hero_config_list_set_rp.type, hero_config_list_set_rp.is, hero_config_list_set_rp.error_info) == true)
// 	{
// 		instance->PacketErrorManager->ProcessError();
// 
// 		return false;
// 	}
// 	else
// 	{
// 		instance->UserInfo->UpdateHeroConfig(hero_config_list_set_rp.hero_config_list);
// 		//hero_config_list_set_rp.hero_config_list;
// 	}
// 	return true;
// }
