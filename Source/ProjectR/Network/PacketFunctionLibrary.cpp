// Generated from PacketFunctionLibrary.cpp.handlebars
#include "ProjectR.h"
#include "PacketFunctionLibrary.h"
#include "ClientProtocols.h"
#include "ReceivePacketHandler.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Global/RGameInstance.h"
#include "HttpRequestClient.h"


void UPacketFunctionLibrary::DispatchNetMessage(URGameInstance* instance, const FString& JsonString, int32& PacketType, bool& PacketLogicalResult)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogJson, Log, TEXT("FJsonSerializer - Unable to deserialize. json=[%s]"), *JsonString);
		return;
	}

	PacketType = JsonObject->GetNumberField("Type");

	switch (PacketType)
	{
	case EPacketType::PACKET_ERROR:
	{
		UE_LOG(LogJson, Log, TEXT("JsonObject - PacketType::PACKET_ERROR. json=[%s]"), *JsonString);
	}
	break;
	case EPacketType::LOGIN_RP:
	{
		FLOGIN_RP rpData;
		if (GetUStructFromJsonObject< FLOGIN_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::LOGIN_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnLOGIN_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ACCOUNT_CREATE_RP:
	{
		FACCOUNT_CREATE_RP rpData;
		if (GetUStructFromJsonObject< FACCOUNT_CREATE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ACCOUNT_CREATE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnACCOUNT_CREATE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ACCOUNT_REMOVE_RP:
	{
		FACCOUNT_REMOVE_RP rpData;
		if (GetUStructFromJsonObject< FACCOUNT_REMOVE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ACCOUNT_REMOVE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnACCOUNT_REMOVE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INIT_CLIENT_KEY_RP:
	{
		FINIT_CLIENT_KEY_RP rpData;
		if (GetUStructFromJsonObject< FINIT_CLIENT_KEY_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INIT_CLIENT_KEY_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINIT_CLIENT_KEY_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ADD_REFER_PUBLISHER_RP:
	{
		FADD_REFER_PUBLISHER_RP rpData;
		if (GetUStructFromJsonObject< FADD_REFER_PUBLISHER_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ADD_REFER_PUBLISHER_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnADD_REFER_PUBLISHER_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GET_REFER_LIST_RP:
	{
		FGET_REFER_LIST_RP rpData;
		if (GetUStructFromJsonObject< FGET_REFER_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GET_REFER_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGET_REFER_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ACCOUNT_CHANGE_LANGUAGE_RP:
	{
		FACCOUNT_CHANGE_LANGUAGE_RP rpData;
		if (GetUStructFromJsonObject< FACCOUNT_CHANGE_LANGUAGE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ACCOUNT_CHANGE_LANGUAGE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnACCOUNT_CHANGE_LANGUAGE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::CLIENT_SDK_ACTION_RP:
	{
		FCLIENT_SDK_ACTION_RP rpData;
		if (GetUStructFromJsonObject< FCLIENT_SDK_ACTION_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::CLIENT_SDK_ACTION_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCLIENT_SDK_ACTION_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_STATUS_RP:
	{
		FARENA_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FARENA_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_TARGET_INFO_RP:
	{
		FARENA_TARGET_INFO_RP rpData;
		if (GetUStructFromJsonObject< FARENA_TARGET_INFO_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_TARGET_INFO_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_TARGET_INFO_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_PLAY_RP:
	{
		FARENA_PLAY_RP rpData;
		if (GetUStructFromJsonObject< FARENA_PLAY_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_PLAY_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_PLAY_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_PLAY_DONE_RP:
	{
		FARENA_PLAY_DONE_RP rpData;
		if (GetUStructFromJsonObject< FARENA_PLAY_DONE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_PLAY_DONE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_PLAY_DONE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_DEFENSE_REWARD_RP:
	{
		FARENA_DEFENSE_REWARD_RP rpData;
		if (GetUStructFromJsonObject< FARENA_DEFENSE_REWARD_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_DEFENSE_REWARD_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_DEFENSE_REWARD_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_SEASON_DONE_RP:
	{
		FARENA_SEASON_DONE_RP rpData;
		if (GetUStructFromJsonObject< FARENA_SEASON_DONE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_SEASON_DONE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_SEASON_DONE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_RANKING_LIST_RP:
	{
		FARENA_RANKING_LIST_RP rpData;
		if (GetUStructFromJsonObject< FARENA_RANKING_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_RANKING_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_RANKING_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ARENA_DEFENSE_DECK_SET_RP:
	{
		FARENA_DEFENSE_DECK_SET_RP rpData;
		if (GetUStructFromJsonObject< FARENA_DEFENSE_DECK_SET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ARENA_DEFENSE_DECK_SET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnARENA_DEFENSE_DECK_SET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_ABILITY_UP_RP:
	{
		FAVATAR_ABILITY_UP_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_ABILITY_UP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_ABILITY_UP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_ABILITY_UP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_ABILITY_RESET_RP:
	{
		FAVATAR_ABILITY_RESET_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_ABILITY_RESET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_ABILITY_RESET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_ABILITY_RESET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_ICON_LIST_RP:
	{
		FAVATAR_ICON_LIST_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_ICON_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_ICON_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_ICON_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_ICON_SET_RP:
	{
		FAVATAR_ICON_SET_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_ICON_SET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_ICON_SET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_ICON_SET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_PROFILE_RP:
	{
		FAVATAR_PROFILE_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_PROFILE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_PROFILE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_PROFILE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_LOBBY_RP:
	{
		FAVATAR_LOBBY_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_LOBBY_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_LOBBY_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_LOBBY_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_GAME_DATA_RP:
	{
		FAVATAR_GAME_DATA_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_GAME_DATA_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_GAME_DATA_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_GAME_DATA_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RP:
	{
		FAVATAR_SELECTIVE_TUTORIAL_UPDATE_RP rpData;
		if (GetUStructFromJsonObject< FAVATAR_SELECTIVE_TUTORIAL_UPDATE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnAVATAR_SELECTIVE_TUTORIAL_UPDATE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::CHANNEL_INFO_LIST_RP:
	{
		FCHANNEL_INFO_LIST_RP rpData;
		if (GetUStructFromJsonObject< FCHANNEL_INFO_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::CHANNEL_INFO_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCHANNEL_INFO_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::CHANNEL_DETAIL_RP:
	{
		FCHANNEL_DETAIL_RP rpData;
		if (GetUStructFromJsonObject< FCHANNEL_DETAIL_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::CHANNEL_DETAIL_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCHANNEL_DETAIL_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::BILLING_TRANSACTION_CREATE_RP:
	{
		FBILLING_TRANSACTION_CREATE_RP rpData;
		if (GetUStructFromJsonObject< FBILLING_TRANSACTION_CREATE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::BILLING_TRANSACTION_CREATE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnBILLING_TRANSACTION_CREATE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::BILLING_TRANSACTION_SUCCESS_RP:
	{
		FBILLING_TRANSACTION_SUCCESS_RP rpData;
		if (GetUStructFromJsonObject< FBILLING_TRANSACTION_SUCCESS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::BILLING_TRANSACTION_SUCCESS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnBILLING_TRANSACTION_SUCCESS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COLOSSEUM_STATUS_RP:
	{
		FCOLOSSEUM_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FCOLOSSEUM_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COLOSSEUM_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOLOSSEUM_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COLOSSEUM_PLAY_RESULT_RP:
	{
		FCOLOSSEUM_PLAY_RESULT_RP rpData;
		if (GetUStructFromJsonObject< FCOLOSSEUM_PLAY_RESULT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COLOSSEUM_PLAY_RESULT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOLOSSEUM_PLAY_RESULT_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COLOSSEUM_SEASON_DONE_RP:
	{
		FCOLOSSEUM_SEASON_DONE_RP rpData;
		if (GetUStructFromJsonObject< FCOLOSSEUM_SEASON_DONE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COLOSSEUM_SEASON_DONE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOLOSSEUM_SEASON_DONE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COLOSSEUM_RANKING_LIST_RP:
	{
		FCOLOSSEUM_RANKING_LIST_RP rpData;
		if (GetUStructFromJsonObject< FCOLOSSEUM_RANKING_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COLOSSEUM_RANKING_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOLOSSEUM_RANKING_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COMMUNITY_ARTICLE_WRITE_HERO_RP:
	{
		FCOMMUNITY_ARTICLE_WRITE_HERO_RP rpData;
		if (GetUStructFromJsonObject< FCOMMUNITY_ARTICLE_WRITE_HERO_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COMMUNITY_ARTICLE_WRITE_HERO_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOMMUNITY_ARTICLE_WRITE_HERO_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COMMUNITY_ARTICLE_DELETE_HERO_RP:
	{
		FCOMMUNITY_ARTICLE_DELETE_HERO_RP rpData;
		if (GetUStructFromJsonObject< FCOMMUNITY_ARTICLE_DELETE_HERO_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COMMUNITY_ARTICLE_DELETE_HERO_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOMMUNITY_ARTICLE_DELETE_HERO_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COMMUNITY_ARTICLE_LIST_HERO_RP:
	{
		FCOMMUNITY_ARTICLE_LIST_HERO_RP rpData;
		if (GetUStructFromJsonObject< FCOMMUNITY_ARTICLE_LIST_HERO_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COMMUNITY_ARTICLE_LIST_HERO_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOMMUNITY_ARTICLE_LIST_HERO_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::COMMUNITY_ARTICLE_VOTE_HERO_RP:
	{
		FCOMMUNITY_ARTICLE_VOTE_HERO_RP rpData;
		if (GetUStructFromJsonObject< FCOMMUNITY_ARTICLE_VOTE_HERO_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::COMMUNITY_ARTICLE_VOTE_HERO_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCOMMUNITY_ARTICLE_VOTE_HERO_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::CONTENT_RESET_STATUS_RP:
	{
		FCONTENT_RESET_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FCONTENT_RESET_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::CONTENT_RESET_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCONTENT_RESET_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::CONTENT_RESET_RP:
	{
		FCONTENT_RESET_RP rpData;
		if (GetUStructFromJsonObject< FCONTENT_RESET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::CONTENT_RESET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnCONTENT_RESET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::ATTENDANCE_GET_RP:
	{
		FATTENDANCE_GET_RP rpData;
		if (GetUStructFromJsonObject< FATTENDANCE_GET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::ATTENDANCE_GET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnATTENDANCE_GET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_CREATE_RP:
	{
		FGUILD_CREATE_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_CREATE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_CREATE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_CREATE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_CLOSE_RP:
	{
		FGUILD_CLOSE_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_CLOSE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_CLOSE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_CLOSE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_GET_RP:
	{
		FGUILD_GET_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_GET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_GET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_GET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_GET_GUEST_RP:
	{
		FGUILD_GET_GUEST_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_GET_GUEST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_GET_GUEST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_GET_GUEST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_MEMBER_LIST_RP:
	{
		FGUILD_MEMBER_LIST_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_MEMBER_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_MEMBER_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_MEMBER_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_AWAITER_LIST_RP:
	{
		FGUILD_AWAITER_LIST_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_AWAITER_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_AWAITER_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_AWAITER_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_AWAITER_ACCEPT_RP:
	{
		FGUILD_AWAITER_ACCEPT_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_AWAITER_ACCEPT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_AWAITER_ACCEPT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_AWAITER_ACCEPT_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_AWAITER_CANCEL_RP:
	{
		FGUILD_AWAITER_CANCEL_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_AWAITER_CANCEL_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_AWAITER_CANCEL_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_AWAITER_CANCEL_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_FIND_NAME_RP:
	{
		FGUILD_FIND_NAME_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_FIND_NAME_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_FIND_NAME_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_FIND_NAME_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_CHANGE_NAME_RP:
	{
		FGUILD_CHANGE_NAME_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_CHANGE_NAME_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_CHANGE_NAME_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_CHANGE_NAME_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_EDIT_DESC_RP:
	{
		FGUILD_EDIT_DESC_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_EDIT_DESC_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_EDIT_DESC_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_EDIT_DESC_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_EDIT_NOTICE_RP:
	{
		FGUILD_EDIT_NOTICE_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_EDIT_NOTICE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_EDIT_NOTICE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_EDIT_NOTICE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_EDIT_AUTO_GRADE_UP_RP:
	{
		FGUILD_EDIT_AUTO_GRADE_UP_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_EDIT_AUTO_GRADE_UP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_EDIT_AUTO_GRADE_UP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_EDIT_AUTO_GRADE_UP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_JOIN_RP:
	{
		FGUILD_JOIN_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_JOIN_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_JOIN_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_JOIN_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_JOIN_AUTO_RP:
	{
		FGUILD_JOIN_AUTO_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_JOIN_AUTO_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_JOIN_AUTO_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_JOIN_AUTO_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_MEMBER_GRADE_RP:
	{
		FGUILD_MEMBER_GRADE_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_MEMBER_GRADE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_MEMBER_GRADE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_MEMBER_GRADE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_MEMBER_KICK_RP:
	{
		FGUILD_MEMBER_KICK_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_MEMBER_KICK_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_MEMBER_KICK_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_MEMBER_KICK_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_CONTRIBUTION_RP:
	{
		FGUILD_CONTRIBUTION_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_CONTRIBUTION_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_CONTRIBUTION_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_CONTRIBUTION_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_CONTRIBUTE_RANKING_RP:
	{
		FGUILD_CONTRIBUTE_RANKING_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_CONTRIBUTE_RANKING_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_CONTRIBUTE_RANKING_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_CONTRIBUTE_RANKING_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_CONTRIBUTION_STATUS_RP:
	{
		FGUILD_CONTRIBUTION_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_CONTRIBUTION_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_CONTRIBUTION_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_CONTRIBUTION_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_ATTENDANCE_STATUS_RP:
	{
		FGUILD_ATTENDANCE_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_ATTENDANCE_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_ATTENDANCE_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_ATTENDANCE_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_ATTENDANCE_DAILY_REWARD_RP:
	{
		FGUILD_ATTENDANCE_DAILY_REWARD_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_ATTENDANCE_DAILY_REWARD_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_ATTENDANCE_DAILY_REWARD_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_ATTENDANCE_DAILY_REWARD_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_ATTENDANCE_WEEKLY_REWARD_RP:
	{
		FGUILD_ATTENDANCE_WEEKLY_REWARD_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_ATTENDANCE_WEEKLY_REWARD_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_ATTENDANCE_WEEKLY_REWARD_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_ATTENDANCE_WEEKLY_REWARD_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_RUNE_STATUS_RP:
	{
		FGUILD_RUNE_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_RUNE_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_RUNE_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_RUNE_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_RUNE_REQUEST_RP:
	{
		FGUILD_RUNE_REQUEST_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_RUNE_REQUEST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_RUNE_REQUEST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_RUNE_REQUEST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_RUNE_SUPPORT_RP:
	{
		FGUILD_RUNE_SUPPORT_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_RUNE_SUPPORT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_RUNE_SUPPORT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_RUNE_SUPPORT_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_RUNE_GET_RP:
	{
		FGUILD_RUNE_GET_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_RUNE_GET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_RUNE_GET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_RUNE_GET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_HISTORY_RP:
	{
		FGUILD_HISTORY_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_HISTORY_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_HISTORY_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_HISTORY_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GUILD_LEVEL_UP_RP:
	{
		FGUILD_LEVEL_UP_RP rpData;
		if (GetUStructFromJsonObject< FGUILD_LEVEL_UP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GUILD_LEVEL_UP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGUILD_LEVEL_UP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_UPGRADE_RP:
	{
		FINVENTORY_HERO_UPGRADE_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_UPGRADE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_UPGRADE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_UPGRADE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_LEVELUP_RP:
	{
		FINVENTORY_HERO_LEVELUP_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_LEVELUP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_LEVELUP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_LEVELUP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_GRIND_RP:
	{
		FINVENTORY_HERO_GRIND_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_GRIND_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_GRIND_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_GRIND_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_EQUIP_RP:
	{
		FINVENTORY_HERO_EQUIP_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_EQUIP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_EQUIP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_EQUIP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_UNEQUIP_RP:
	{
		FINVENTORY_HERO_UNEQUIP_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_UNEQUIP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_UNEQUIP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_UNEQUIP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_ITEM_GRIND_RP:
	{
		FINVENTORY_ITEM_GRIND_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_ITEM_GRIND_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_ITEM_GRIND_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_ITEM_GRIND_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_RUNE_EQUIP_RP:
	{
		FINVENTORY_RUNE_EQUIP_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_RUNE_EQUIP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_RUNE_EQUIP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_RUNE_EQUIP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_RUNE_UNEQUIP_RP:
	{
		FINVENTORY_RUNE_UNEQUIP_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_RUNE_UNEQUIP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_RUNE_UNEQUIP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_RUNE_UNEQUIP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_RUNE_MAKE_RP:
	{
		FINVENTORY_RUNE_MAKE_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_RUNE_MAKE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_RUNE_MAKE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_RUNE_MAKE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_ITEM_LEVELUP_RP:
	{
		FINVENTORY_ITEM_LEVELUP_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_ITEM_LEVELUP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_ITEM_LEVELUP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_ITEM_LEVELUP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_ITEM_ENCHANT_RP:
	{
		FINVENTORY_ITEM_ENCHANT_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_ITEM_ENCHANT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_ITEM_ENCHANT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_ITEM_ENCHANT_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_ITEM_HERO_LOCK_RP:
	{
		FINVENTORY_ITEM_HERO_LOCK_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_ITEM_HERO_LOCK_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_ITEM_HERO_LOCK_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_ITEM_HERO_LOCK_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_SKILL_LEVELUP_RP:
	{
		FINVENTORY_HERO_SKILL_LEVELUP_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_SKILL_LEVELUP_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_SKILL_LEVELUP_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_SKILL_LEVELUP_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_ENCHANT_CONSUME_RP:
	{
		FINVENTORY_HERO_ENCHANT_CONSUME_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_ENCHANT_CONSUME_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_ENCHANT_CONSUME_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_ENCHANT_CONSUME_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_HERO_ENCHANT_STAT_RP:
	{
		FINVENTORY_HERO_ENCHANT_STAT_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_HERO_ENCHANT_STAT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_HERO_ENCHANT_STAT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_HERO_ENCHANT_STAT_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_SELL_RP:
	{
		FINVENTORY_SELL_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_SELL_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_SELL_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_SELL_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_ITEM_UPGRADE_RP:
	{
		FINVENTORY_ITEM_UPGRADE_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_ITEM_UPGRADE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_ITEM_UPGRADE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_ITEM_UPGRADE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_ITEM_OPTION_RESET_RP:
	{
		FINVENTORY_ITEM_OPTION_RESET_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_ITEM_OPTION_RESET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_ITEM_OPTION_RESET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_ITEM_OPTION_RESET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_TIMETREASURE_GET_RP:
	{
		FINVENTORY_TIMETREASURE_GET_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_TIMETREASURE_GET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_TIMETREASURE_GET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_TIMETREASURE_GET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::INVENTORY_UPGRADE_RP:
	{
		FINVENTORY_UPGRADE_RP rpData;
		if (GetUStructFromJsonObject< FINVENTORY_UPGRADE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::INVENTORY_UPGRADE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnINVENTORY_UPGRADE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GIFT_LIST_RP:
	{
		FGIFT_LIST_RP rpData;
		if (GetUStructFromJsonObject< FGIFT_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GIFT_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGIFT_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GIFT_OPEN_RP:
	{
		FGIFT_OPEN_RP rpData;
		if (GetUStructFromJsonObject< FGIFT_OPEN_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GIFT_OPEN_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGIFT_OPEN_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SPECIAL_GIFT_LIST_RP:
	{
		FSPECIAL_GIFT_LIST_RP rpData;
		if (GetUStructFromJsonObject< FSPECIAL_GIFT_LIST_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SPECIAL_GIFT_LIST_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSPECIAL_GIFT_LIST_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SPECIAL_GIFT_OPEN_RP:
	{
		FSPECIAL_GIFT_OPEN_RP rpData;
		if (GetUStructFromJsonObject< FSPECIAL_GIFT_OPEN_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SPECIAL_GIFT_OPEN_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSPECIAL_GIFT_OPEN_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::MIRROR_STATUS_RP:
	{
		FMIRROR_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FMIRROR_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::MIRROR_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnMIRROR_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::MIRROR_PLAY_START_RP:
	{
		FMIRROR_PLAY_START_RP rpData;
		if (GetUStructFromJsonObject< FMIRROR_PLAY_START_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::MIRROR_PLAY_START_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnMIRROR_PLAY_START_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::MIRROR_PLAY_DONE_RP:
	{
		FMIRROR_PLAY_DONE_RP rpData;
		if (GetUStructFromJsonObject< FMIRROR_PLAY_DONE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::MIRROR_PLAY_DONE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnMIRROR_PLAY_DONE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::MIRROR_RESET_RP:
	{
		FMIRROR_RESET_RP rpData;
		if (GetUStructFromJsonObject< FMIRROR_RESET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::MIRROR_RESET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnMIRROR_RESET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::MIRROR_DEFENSE_DECK_SET_RP:
	{
		FMIRROR_DEFENSE_DECK_SET_RP rpData;
		if (GetUStructFromJsonObject< FMIRROR_DEFENSE_DECK_SET_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::MIRROR_DEFENSE_DECK_SET_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnMIRROR_DEFENSE_DECK_SET_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GIVE_ACHIEVE_REWARD_RP:
	{
		FGIVE_ACHIEVE_REWARD_RP rpData;
		if (GetUStructFromJsonObject< FGIVE_ACHIEVE_REWARD_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GIVE_ACHIEVE_REWARD_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGIVE_ACHIEVE_REWARD_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::DAILY_QUEST_STATUS_RP:
	{
		FDAILY_QUEST_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FDAILY_QUEST_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::DAILY_QUEST_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnDAILY_QUEST_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::DAILY_QUEST_REWARD_RP:
	{
		FDAILY_QUEST_REWARD_RP rpData;
		if (GetUStructFromJsonObject< FDAILY_QUEST_REWARD_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::DAILY_QUEST_REWARD_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnDAILY_QUEST_REWARD_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SHOP_USER_DATA_RP:
	{
		FSHOP_USER_DATA_RP rpData;
		if (GetUStructFromJsonObject< FSHOP_USER_DATA_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SHOP_USER_DATA_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSHOP_USER_DATA_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SHOP_BUY_RP:
	{
		FSHOP_BUY_RP rpData;
		if (GetUStructFromJsonObject< FSHOP_BUY_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SHOP_BUY_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSHOP_BUY_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SHOP_CHARACTER_SUMMON_RP:
	{
		FSHOP_CHARACTER_SUMMON_RP rpData;
		if (GetUStructFromJsonObject< FSHOP_CHARACTER_SUMMON_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SHOP_CHARACTER_SUMMON_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSHOP_CHARACTER_SUMMON_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SHOP_FLOORSHOP_STATUS_RP:
	{
		FSHOP_FLOORSHOP_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FSHOP_FLOORSHOP_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SHOP_FLOORSHOP_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSHOP_FLOORSHOP_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SHOP_FLOORSHOP_BUY_RP:
	{
		FSHOP_FLOORSHOP_BUY_RP rpData;
		if (GetUStructFromJsonObject< FSHOP_FLOORSHOP_BUY_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SHOP_FLOORSHOP_BUY_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSHOP_FLOORSHOP_BUY_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::SHOP_FLOORSHOP_OPEN_SLOT_RP:
	{
		FSHOP_FLOORSHOP_OPEN_SLOT_RP rpData;
		if (GetUStructFromJsonObject< FSHOP_FLOORSHOP_OPEN_SLOT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::SHOP_FLOORSHOP_OPEN_SLOT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnSHOP_FLOORSHOP_OPEN_SLOT_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_STATUS_RP:
	{
		FTOWER_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_PLAY_START_RP:
	{
		FTOWER_PLAY_START_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_PLAY_START_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_PLAY_START_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_PLAY_START_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_PLAY_DONE_RP:
	{
		FTOWER_PLAY_DONE_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_PLAY_DONE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_PLAY_DONE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_PLAY_DONE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_TREASURE_BOX_RP:
	{
		FTOWER_TREASURE_BOX_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_TREASURE_BOX_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_TREASURE_BOX_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_TREASURE_BOX_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_BOSS_PLAY_CHECK_RP:
	{
		FTOWER_BOSS_PLAY_CHECK_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_BOSS_PLAY_CHECK_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_BOSS_PLAY_CHECK_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_BOSS_PLAY_CHECK_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_BOSS_PLAY_RESULT_RP:
	{
		FTOWER_BOSS_PLAY_RESULT_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_BOSS_PLAY_RESULT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_BOSS_PLAY_RESULT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_BOSS_PLAY_RESULT_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_FLOOR_MISSION_ACTIVE_RP:
	{
		FTOWER_FLOOR_MISSION_ACTIVE_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_FLOOR_MISSION_ACTIVE_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_FLOOR_MISSION_ACTIVE_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_FLOOR_MISSION_ACTIVE_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TOWER_FLOOR_MISSION_REWARD_RP:
	{
		FTOWER_FLOOR_MISSION_REWARD_RP rpData;
		if (GetUStructFromJsonObject< FTOWER_FLOOR_MISSION_REWARD_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TOWER_FLOOR_MISSION_REWARD_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTOWER_FLOOR_MISSION_REWARD_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::TREASURE_GET_REWARD_RP:
	{
		FTREASURE_GET_REWARD_RP rpData;
		if (GetUStructFromJsonObject< FTREASURE_GET_REWARD_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::TREASURE_GET_REWARD_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnTREASURE_GET_REWARD_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::GET_TARGET_SERVER_RP:
	{
		FGET_TARGET_SERVER_RP rpData;
		if (GetUStructFromJsonObject< FGET_TARGET_SERVER_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::GET_TARGET_SERVER_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnGET_TARGET_SERVER_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::WORLDBOSS_STATUS_RP:
	{
		FWORLDBOSS_STATUS_RP rpData;
		if (GetUStructFromJsonObject< FWORLDBOSS_STATUS_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::WORLDBOSS_STATUS_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnWORLDBOSS_STATUS_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::WORLDBOSS_PLAY_CHECK_RP:
	{
		FWORLDBOSS_PLAY_CHECK_RP rpData;
		if (GetUStructFromJsonObject< FWORLDBOSS_PLAY_CHECK_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::WORLDBOSS_PLAY_CHECK_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnWORLDBOSS_PLAY_CHECK_RP( rpData, instance );
		}
	}
	break;
	case EPacketType::WORLDBOSS_PLAY_RESULT_RP:
	{
		FWORLDBOSS_PLAY_RESULT_RP rpData;
		if (GetUStructFromJsonObject< FWORLDBOSS_PLAY_RESULT_RP >(JsonObject.ToSharedRef(), &rpData) == false)
		{
			UE_LOG(LogJson, Log, TEXT("GetUStructFromJsonObject - EPacketType::WORLDBOSS_PLAY_RESULT_RP : Unable to parse json=[%s]"), *JsonString);
		}
		else
		{
			PacketLogicalResult = UReceivePacketHandler::OnWORLDBOSS_PLAY_RESULT_RP( rpData, instance );
		}
	}
	break;
	};
}



void UPacketFunctionLibrary::LOGIN_RQ(int32 flag)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FLOGIN_RQ data;
	data.type = EPacketType::LOGIN_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.flag = flag;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FLOGIN_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ACCOUNT_CREATE_RQ(FString nick, int32 language)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FACCOUNT_CREATE_RQ data;
	data.type = EPacketType::ACCOUNT_CREATE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.language = language;
	data.nick = nick;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FACCOUNT_CREATE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ACCOUNT_REMOVE_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FACCOUNT_REMOVE_RQ data;
	data.type = EPacketType::ACCOUNT_REMOVE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FACCOUNT_REMOVE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INIT_CLIENT_KEY_RQ(FString publisher, FString code, FString access_token, FString redirect_uri, FString id, FString pwd)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINIT_CLIENT_KEY_RQ data;
	data.type = EPacketType::INIT_CLIENT_KEY_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.pwd = pwd;
	data.id = id;
	data.redirect_uri = redirect_uri;
	data.access_token = access_token;
	data.code = code;
	data.publisher = publisher;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINIT_CLIENT_KEY_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ADD_REFER_PUBLISHER_RQ(FString publisher, FString code, FString access_token, FString redirect_uri, FString id, FString pwd)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FADD_REFER_PUBLISHER_RQ data;
	data.type = EPacketType::ADD_REFER_PUBLISHER_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.pwd = pwd;
	data.id = id;
	data.redirect_uri = redirect_uri;
	data.access_token = access_token;
	data.code = code;
	data.publisher = publisher;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FADD_REFER_PUBLISHER_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GET_REFER_LIST_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGET_REFER_LIST_RQ data;
	data.type = EPacketType::GET_REFER_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGET_REFER_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ACCOUNT_CHANGE_LANGUAGE_RQ(int32 language)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FACCOUNT_CHANGE_LANGUAGE_RQ data;
	data.type = EPacketType::ACCOUNT_CHANGE_LANGUAGE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.language = language;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FACCOUNT_CHANGE_LANGUAGE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::CLIENT_SDK_ACTION_RQ(int32 flag)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCLIENT_SDK_ACTION_RQ data;
	data.type = EPacketType::CLIENT_SDK_ACTION_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.flag = flag;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCLIENT_SDK_ACTION_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_STATUS_RQ data;
	data.type = EPacketType::ARENA_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_TARGET_INFO_RQ(bool refresh)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_TARGET_INFO_RQ data;
	data.type = EPacketType::ARENA_TARGET_INFO_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.refresh = refresh;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_TARGET_INFO_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_PLAY_RQ(int32 target_kId, TArray<FString> heroUDs, int32 power, int32 target_power)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_PLAY_RQ data;
	data.type = EPacketType::ARENA_PLAY_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.target_power = target_power;
	data.power = power;
	data.heroUDs = heroUDs;
	data.target_kId = target_kId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_PLAY_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_PLAY_DONE_RQ(FString playUD, bool isWin)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_PLAY_DONE_RQ data;
	data.type = EPacketType::ARENA_PLAY_DONE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.isWin = isWin;
	data.playUD = playUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_PLAY_DONE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_DEFENSE_REWARD_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_DEFENSE_REWARD_RQ data;
	data.type = EPacketType::ARENA_DEFENSE_REWARD_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_DEFENSE_REWARD_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_SEASON_DONE_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_SEASON_DONE_RQ data;
	data.type = EPacketType::ARENA_SEASON_DONE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_SEASON_DONE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_RANKING_LIST_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_RANKING_LIST_RQ data;
	data.type = EPacketType::ARENA_RANKING_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_RANKING_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ARENA_DEFENSE_DECK_SET_RQ(TArray<FString> defense)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FARENA_DEFENSE_DECK_SET_RQ data;
	data.type = EPacketType::ARENA_DEFENSE_DECK_SET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.defense = defense;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FARENA_DEFENSE_DECK_SET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_ABILITY_UP_RQ(int32 abilityIndex, int32 point)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_ABILITY_UP_RQ data;
	data.type = EPacketType::AVATAR_ABILITY_UP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.point = point;
	data.abilityIndex = abilityIndex;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_ABILITY_UP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_ABILITY_RESET_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_ABILITY_RESET_RQ data;
	data.type = EPacketType::AVATAR_ABILITY_RESET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_ABILITY_RESET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_ICON_LIST_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_ICON_LIST_RQ data;
	data.type = EPacketType::AVATAR_ICON_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_ICON_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_ICON_SET_RQ(int32 icon)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_ICON_SET_RQ data;
	data.type = EPacketType::AVATAR_ICON_SET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.icon = icon;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_ICON_SET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_PROFILE_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_PROFILE_RQ data;
	data.type = EPacketType::AVATAR_PROFILE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_PROFILE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_LOBBY_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_LOBBY_RQ data;
	data.type = EPacketType::AVATAR_LOBBY_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_LOBBY_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_GAME_DATA_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_GAME_DATA_RQ data;
	data.type = EPacketType::AVATAR_GAME_DATA_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_GAME_DATA_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ(int32 idx, int32 value)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FAVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ data;
	data.type = EPacketType::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.value = value;
	data.idx = idx;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FAVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::CHANNEL_INFO_LIST_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCHANNEL_INFO_LIST_RQ data;
	data.type = EPacketType::CHANNEL_INFO_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCHANNEL_INFO_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::CHANNEL_DETAIL_RQ(int32 no)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCHANNEL_DETAIL_RQ data;
	data.type = EPacketType::CHANNEL_DETAIL_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.no = no;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCHANNEL_DETAIL_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::BILLING_TRANSACTION_CREATE_RQ(FString purchase_system, FString product, FString redirect)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FBILLING_TRANSACTION_CREATE_RQ data;
	data.type = EPacketType::BILLING_TRANSACTION_CREATE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.redirect = redirect;
	data.product = product;
	data.purchase_system = purchase_system;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FBILLING_TRANSACTION_CREATE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::BILLING_TRANSACTION_SUCCESS_RQ(FString purchase_system, FString receipt, FString product, FString transaction_id)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FBILLING_TRANSACTION_SUCCESS_RQ data;
	data.type = EPacketType::BILLING_TRANSACTION_SUCCESS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.transaction_id = transaction_id;
	data.product = product;
	data.receipt = receipt;
	data.purchase_system = purchase_system;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FBILLING_TRANSACTION_SUCCESS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COLOSSEUM_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOLOSSEUM_STATUS_RQ data;
	data.type = EPacketType::COLOSSEUM_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOLOSSEUM_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COLOSSEUM_PLAY_RESULT_RQ(FString roomKey)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOLOSSEUM_PLAY_RESULT_RQ data;
	data.type = EPacketType::COLOSSEUM_PLAY_RESULT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.roomKey = roomKey;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOLOSSEUM_PLAY_RESULT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COLOSSEUM_SEASON_DONE_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOLOSSEUM_SEASON_DONE_RQ data;
	data.type = EPacketType::COLOSSEUM_SEASON_DONE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOLOSSEUM_SEASON_DONE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COLOSSEUM_RANKING_LIST_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOLOSSEUM_RANKING_LIST_RQ data;
	data.type = EPacketType::COLOSSEUM_RANKING_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOLOSSEUM_RANKING_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COMMUNITY_ARTICLE_WRITE_HERO_RQ(FString heroId, FString contents)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOMMUNITY_ARTICLE_WRITE_HERO_RQ data;
	data.type = EPacketType::COMMUNITY_ARTICLE_WRITE_HERO_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.contents = contents;
	data.heroId = heroId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOMMUNITY_ARTICLE_WRITE_HERO_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COMMUNITY_ARTICLE_DELETE_HERO_RQ(FString articleId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOMMUNITY_ARTICLE_DELETE_HERO_RQ data;
	data.type = EPacketType::COMMUNITY_ARTICLE_DELETE_HERO_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.articleId = articleId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOMMUNITY_ARTICLE_DELETE_HERO_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COMMUNITY_ARTICLE_LIST_HERO_RQ(FString heroId, int32 sortType)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOMMUNITY_ARTICLE_LIST_HERO_RQ data;
	data.type = EPacketType::COMMUNITY_ARTICLE_LIST_HERO_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.sortType = sortType;
	data.heroId = heroId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOMMUNITY_ARTICLE_LIST_HERO_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::COMMUNITY_ARTICLE_VOTE_HERO_RQ(FString articleId, bool recommend)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCOMMUNITY_ARTICLE_VOTE_HERO_RQ data;
	data.type = EPacketType::COMMUNITY_ARTICLE_VOTE_HERO_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.recommend = recommend;
	data.articleId = articleId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCOMMUNITY_ARTICLE_VOTE_HERO_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(FString resetType, FString resetKey)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCONTENT_RESET_STATUS_RQ data;
	data.type = EPacketType::CONTENT_RESET_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.resetKey = resetKey;
	data.resetType = resetType;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCONTENT_RESET_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::CONTENT_RESET_RQ(FString resetType, FString resetKey)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FCONTENT_RESET_RQ data;
	data.type = EPacketType::CONTENT_RESET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.resetKey = resetKey;
	data.resetType = resetType;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FCONTENT_RESET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::ATTENDANCE_GET_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FATTENDANCE_GET_RQ data;
	data.type = EPacketType::ATTENDANCE_GET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FATTENDANCE_GET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_CREATE_RQ(FString name, FString desc, FString notice, bool auto_grade_up)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_CREATE_RQ data;
	data.type = EPacketType::GUILD_CREATE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.auto_grade_up = auto_grade_up;
	data.notice = notice;
	data.desc = desc;
	data.name = name;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_CREATE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_CLOSE_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_CLOSE_RQ data;
	data.type = EPacketType::GUILD_CLOSE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_CLOSE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_GET_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_GET_RQ data;
	data.type = EPacketType::GUILD_GET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_GET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_GET_GUEST_RQ(int32 guild_id, int32 page)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_GET_GUEST_RQ data;
	data.type = EPacketType::GUILD_GET_GUEST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.page = page;
	data.guild_id = guild_id;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_GET_GUEST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_MEMBER_LIST_RQ(int32 page)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_MEMBER_LIST_RQ data;
	data.type = EPacketType::GUILD_MEMBER_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.page = page;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_MEMBER_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_AWAITER_LIST_RQ(int32 page)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_AWAITER_LIST_RQ data;
	data.type = EPacketType::GUILD_AWAITER_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.page = page;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_AWAITER_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_AWAITER_ACCEPT_RQ(int32 target_kId, bool accept)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_AWAITER_ACCEPT_RQ data;
	data.type = EPacketType::GUILD_AWAITER_ACCEPT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.accept = accept;
	data.target_kId = target_kId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_AWAITER_ACCEPT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_AWAITER_CANCEL_RQ(int32 guild_id)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_AWAITER_CANCEL_RQ data;
	data.type = EPacketType::GUILD_AWAITER_CANCEL_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.guild_id = guild_id;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_AWAITER_CANCEL_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_FIND_NAME_RQ(FString name)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_FIND_NAME_RQ data;
	data.type = EPacketType::GUILD_FIND_NAME_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.name = name;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_FIND_NAME_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_CHANGE_NAME_RQ(FString name)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_CHANGE_NAME_RQ data;
	data.type = EPacketType::GUILD_CHANGE_NAME_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.name = name;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_CHANGE_NAME_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_EDIT_DESC_RQ(FString desc)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_EDIT_DESC_RQ data;
	data.type = EPacketType::GUILD_EDIT_DESC_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.desc = desc;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_EDIT_DESC_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_EDIT_NOTICE_RQ(FString notice)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_EDIT_NOTICE_RQ data;
	data.type = EPacketType::GUILD_EDIT_NOTICE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.notice = notice;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_EDIT_NOTICE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_EDIT_AUTO_GRADE_UP_RQ(bool auto_grade_up)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_EDIT_AUTO_GRADE_UP_RQ data;
	data.type = EPacketType::GUILD_EDIT_AUTO_GRADE_UP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.auto_grade_up = auto_grade_up;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_EDIT_AUTO_GRADE_UP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_JOIN_RQ(int32 guild_id)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_JOIN_RQ data;
	data.type = EPacketType::GUILD_JOIN_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.guild_id = guild_id;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_JOIN_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_JOIN_AUTO_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_JOIN_AUTO_RQ data;
	data.type = EPacketType::GUILD_JOIN_AUTO_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_JOIN_AUTO_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(int32 target_kId, int32 grade)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_MEMBER_GRADE_RQ data;
	data.type = EPacketType::GUILD_MEMBER_GRADE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.grade = grade;
	data.target_kId = target_kId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_MEMBER_GRADE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_MEMBER_KICK_RQ(int32 target_kId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_MEMBER_KICK_RQ data;
	data.type = EPacketType::GUILD_MEMBER_KICK_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.target_kId = target_kId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_MEMBER_KICK_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_CONTRIBUTION_RQ(int32 contributionType)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_CONTRIBUTION_RQ data;
	data.type = EPacketType::GUILD_CONTRIBUTION_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.contributionType = contributionType;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_CONTRIBUTION_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_CONTRIBUTE_RANKING_RQ(int32 page)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_CONTRIBUTE_RANKING_RQ data;
	data.type = EPacketType::GUILD_CONTRIBUTE_RANKING_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.page = page;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_CONTRIBUTE_RANKING_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_CONTRIBUTION_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_CONTRIBUTION_STATUS_RQ data;
	data.type = EPacketType::GUILD_CONTRIBUTION_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_CONTRIBUTION_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_ATTENDANCE_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_ATTENDANCE_STATUS_RQ data;
	data.type = EPacketType::GUILD_ATTENDANCE_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_ATTENDANCE_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_ATTENDANCE_DAILY_REWARD_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_ATTENDANCE_DAILY_REWARD_RQ data;
	data.type = EPacketType::GUILD_ATTENDANCE_DAILY_REWARD_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_ATTENDANCE_DAILY_REWARD_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_ATTENDANCE_WEEKLY_REWARD_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_ATTENDANCE_WEEKLY_REWARD_RQ data;
	data.type = EPacketType::GUILD_ATTENDANCE_WEEKLY_REWARD_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_ATTENDANCE_WEEKLY_REWARD_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_RUNE_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_RUNE_STATUS_RQ data;
	data.type = EPacketType::GUILD_RUNE_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_RUNE_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_RUNE_REQUEST_RQ(FName heroId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_RUNE_REQUEST_RQ data;
	data.type = EPacketType::GUILD_RUNE_REQUEST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.heroId = heroId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_RUNE_REQUEST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_RUNE_SUPPORT_RQ(int32 requester_kId, FName heroId, int32 amount)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_RUNE_SUPPORT_RQ data;
	data.type = EPacketType::GUILD_RUNE_SUPPORT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.amount = amount;
	data.heroId = heroId;
	data.requester_kId = requester_kId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_RUNE_SUPPORT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_RUNE_GET_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_RUNE_GET_RQ data;
	data.type = EPacketType::GUILD_RUNE_GET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_RUNE_GET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_HISTORY_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_HISTORY_RQ data;
	data.type = EPacketType::GUILD_HISTORY_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_HISTORY_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GUILD_LEVEL_UP_RQ(int32 level)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGUILD_LEVEL_UP_RQ data;
	data.type = EPacketType::GUILD_LEVEL_UP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.level = level;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGUILD_LEVEL_UP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_UPGRADE_RQ(FString heroUD, TArray<FString> materials1, TArray<FString> materials2, TArray<FString> materials3, TArray<FString> materials4)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_UPGRADE_RQ data;
	data.type = EPacketType::INVENTORY_HERO_UPGRADE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.materials4 = materials4;
	data.materials3 = materials3;
	data.materials2 = materials2;
	data.materials1 = materials1;
	data.heroUD = heroUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_UPGRADE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_LEVELUP_RQ(FString heroUD, int32 level)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_LEVELUP_RQ data;
	data.type = EPacketType::INVENTORY_HERO_LEVELUP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.level = level;
	data.heroUD = heroUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_LEVELUP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_GRIND_RQ(TArray<FString> heroUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_GRIND_RQ data;
	data.type = EPacketType::INVENTORY_HERO_GRIND_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.heroUDs = heroUDs;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_GRIND_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_EQUIP_RQ(FString heroUD, FString itemUD)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_EQUIP_RQ data;
	data.type = EPacketType::INVENTORY_HERO_EQUIP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.itemUD = itemUD;
	data.heroUD = heroUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_EQUIP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_UNEQUIP_RQ(FString heroUD, FString itemUD)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_UNEQUIP_RQ data;
	data.type = EPacketType::INVENTORY_HERO_UNEQUIP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.itemUD = itemUD;
	data.heroUD = heroUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_UNEQUIP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_ITEM_GRIND_RQ(TArray<FString> itemUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_ITEM_GRIND_RQ data;
	data.type = EPacketType::INVENTORY_ITEM_GRIND_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.itemUDs = itemUDs;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_ITEM_GRIND_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_RUNE_EQUIP_RQ(FString itemUD, TArray<FString> equipRunes)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_RUNE_EQUIP_RQ data;
	data.type = EPacketType::INVENTORY_RUNE_EQUIP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.equipRunes = equipRunes;
	data.itemUD = itemUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_RUNE_EQUIP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_RUNE_UNEQUIP_RQ(FString itemUD, TArray<bool> unequipSlots)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_RUNE_UNEQUIP_RQ data;
	data.type = EPacketType::INVENTORY_RUNE_UNEQUIP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.unequipSlots = unequipSlots;
	data.itemUD = itemUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_RUNE_UNEQUIP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_RUNE_MAKE_RQ(FString rune, int32 amount)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_RUNE_MAKE_RQ data;
	data.type = EPacketType::INVENTORY_RUNE_MAKE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.amount = amount;
	data.rune = rune;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_RUNE_MAKE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_ITEM_LEVELUP_RQ(FString itemUD, int32 amount)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_ITEM_LEVELUP_RQ data;
	data.type = EPacketType::INVENTORY_ITEM_LEVELUP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.amount = amount;
	data.itemUD = itemUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_ITEM_LEVELUP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_ITEM_ENCHANT_RQ(FString itemUD, bool specialEnchant, bool useProtection)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_ITEM_ENCHANT_RQ data;
	data.type = EPacketType::INVENTORY_ITEM_ENCHANT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.useProtection = useProtection;
	data.specialEnchant = specialEnchant;
	data.itemUD = itemUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_ITEM_ENCHANT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ(TArray<FString> itemUDs_lock, TArray<FString> itemUDs_unlock, TArray<FString> heroUDs_lock, TArray<FString> heroUDs_unlock)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_ITEM_HERO_LOCK_RQ data;
	data.type = EPacketType::INVENTORY_ITEM_HERO_LOCK_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.heroUDs_unlock = heroUDs_unlock;
	data.heroUDs_lock = heroUDs_lock;
	data.itemUDs_unlock = itemUDs_unlock;
	data.itemUDs_lock = itemUDs_lock;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_ITEM_HERO_LOCK_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_SKILL_LEVELUP_RQ(FString heroUD, int32 slot)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_SKILL_LEVELUP_RQ data;
	data.type = EPacketType::INVENTORY_HERO_SKILL_LEVELUP_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.slot = slot;
	data.heroUD = heroUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_SKILL_LEVELUP_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_ENCHANT_CONSUME_RQ(FString heroUD, TArray<FString> spiritUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_ENCHANT_CONSUME_RQ data;
	data.type = EPacketType::INVENTORY_HERO_ENCHANT_CONSUME_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.spiritUDs = spiritUDs;
	data.heroUD = heroUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_ENCHANT_CONSUME_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_HERO_ENCHANT_STAT_RQ(FString heroUD, TArray<int32> enchantStat)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_HERO_ENCHANT_STAT_RQ data;
	data.type = EPacketType::INVENTORY_HERO_ENCHANT_STAT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.enchantStat = enchantStat;
	data.heroUD = heroUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_HERO_ENCHANT_STAT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_SELL_RQ(TArray<FString> itemUDs, TArray<FString> walletIdList, TArray<int32> walletQtyList)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_SELL_RQ data;
	data.type = EPacketType::INVENTORY_SELL_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.walletQtyList = walletQtyList;
	data.walletIdList = walletIdList;
	data.itemUDs = itemUDs;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_SELL_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_ITEM_UPGRADE_RQ(FString itemUD, TArray<FString> material_itemUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_ITEM_UPGRADE_RQ data;
	data.type = EPacketType::INVENTORY_ITEM_UPGRADE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.material_itemUDs = material_itemUDs;
	data.itemUD = itemUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_ITEM_UPGRADE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_ITEM_OPTION_RESET_RQ(FString itemUD, TArray<FString> material_itemUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_ITEM_OPTION_RESET_RQ data;
	data.type = EPacketType::INVENTORY_ITEM_OPTION_RESET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.material_itemUDs = material_itemUDs;
	data.itemUD = itemUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_ITEM_OPTION_RESET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_TIMETREASURE_GET_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_TIMETREASURE_GET_RQ data;
	data.type = EPacketType::INVENTORY_TIMETREASURE_GET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_TIMETREASURE_GET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::INVENTORY_UPGRADE_RQ(int32 upgrade, int32 upgradeType)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FINVENTORY_UPGRADE_RQ data;
	data.type = EPacketType::INVENTORY_UPGRADE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.upgradeType = upgradeType;
	data.upgrade = upgrade;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FINVENTORY_UPGRADE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GIFT_LIST_RQ(int32 holds, int32 size)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGIFT_LIST_RQ data;
	data.type = EPacketType::GIFT_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.size = size;
	data.holds = holds;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGIFT_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GIFT_OPEN_RQ(TArray<FString> mailUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGIFT_OPEN_RQ data;
	data.type = EPacketType::GIFT_OPEN_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.mailUDs = mailUDs;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGIFT_OPEN_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SPECIAL_GIFT_LIST_RQ(int32 holds, int32 size)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSPECIAL_GIFT_LIST_RQ data;
	data.type = EPacketType::SPECIAL_GIFT_LIST_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.size = size;
	data.holds = holds;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSPECIAL_GIFT_LIST_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SPECIAL_GIFT_OPEN_RQ(TArray<FString> mailUD, int32 giftIdx)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSPECIAL_GIFT_OPEN_RQ data;
	data.type = EPacketType::SPECIAL_GIFT_OPEN_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.giftIdx = giftIdx;
	data.mailUD = mailUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSPECIAL_GIFT_OPEN_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::MIRROR_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FMIRROR_STATUS_RQ data;
	data.type = EPacketType::MIRROR_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FMIRROR_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::MIRROR_PLAY_START_RQ(FString mirrorUD, TArray<FString> heroUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FMIRROR_PLAY_START_RQ data;
	data.type = EPacketType::MIRROR_PLAY_START_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.heroUDs = heroUDs;
	data.mirrorUD = mirrorUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FMIRROR_PLAY_START_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::MIRROR_PLAY_DONE_RQ(FString playUD, int32 clearGrade, TArray<int32> heroHPs_hp, TArray<int32> target_heroHPs_hp)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FMIRROR_PLAY_DONE_RQ data;
	data.type = EPacketType::MIRROR_PLAY_DONE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.target_heroHPs_hp = target_heroHPs_hp;
	data.heroHPs_hp = heroHPs_hp;
	data.clearGrade = clearGrade;
	data.playUD = playUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FMIRROR_PLAY_DONE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::MIRROR_RESET_RQ(bool useGem)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FMIRROR_RESET_RQ data;
	data.type = EPacketType::MIRROR_RESET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.useGem = useGem;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FMIRROR_RESET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::MIRROR_DEFENSE_DECK_SET_RQ(TArray<FString> defense)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FMIRROR_DEFENSE_DECK_SET_RQ data;
	data.type = EPacketType::MIRROR_DEFENSE_DECK_SET_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.defense = defense;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FMIRROR_DEFENSE_DECK_SET_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GIVE_ACHIEVE_REWARD_RQ(int32 srl, int32 target_level)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGIVE_ACHIEVE_REWARD_RQ data;
	data.type = EPacketType::GIVE_ACHIEVE_REWARD_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.target_level = target_level;
	data.srl = srl;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGIVE_ACHIEVE_REWARD_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::DAILY_QUEST_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FDAILY_QUEST_STATUS_RQ data;
	data.type = EPacketType::DAILY_QUEST_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FDAILY_QUEST_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::DAILY_QUEST_REWARD_RQ(int32 idx)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FDAILY_QUEST_REWARD_RQ data;
	data.type = EPacketType::DAILY_QUEST_REWARD_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.idx = idx;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FDAILY_QUEST_REWARD_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SHOP_USER_DATA_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSHOP_USER_DATA_RQ data;
	data.type = EPacketType::SHOP_USER_DATA_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSHOP_USER_DATA_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SHOP_BUY_RQ(FString productId, int32 productType, int32 amount, FString currencyType, int32 price)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSHOP_BUY_RQ data;
	data.type = EPacketType::SHOP_BUY_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.price = price;
	data.currencyType = currencyType;
	data.amount = amount;
	data.productType = productType;
	data.productId = productId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSHOP_BUY_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SHOP_CHARACTER_SUMMON_RQ(int32 summonId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSHOP_CHARACTER_SUMMON_RQ data;
	data.type = EPacketType::SHOP_CHARACTER_SUMMON_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.summonId = summonId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSHOP_CHARACTER_SUMMON_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SHOP_FLOORSHOP_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSHOP_FLOORSHOP_STATUS_RQ data;
	data.type = EPacketType::SHOP_FLOORSHOP_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSHOP_FLOORSHOP_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SHOP_FLOORSHOP_BUY_RQ(int32 shopId, int32 slotId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSHOP_FLOORSHOP_BUY_RQ data;
	data.type = EPacketType::SHOP_FLOORSHOP_BUY_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.slotId = slotId;
	data.shopId = shopId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSHOP_FLOORSHOP_BUY_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::SHOP_FLOORSHOP_OPEN_SLOT_RQ(int32 slotId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FSHOP_FLOORSHOP_OPEN_SLOT_RQ data;
	data.type = EPacketType::SHOP_FLOORSHOP_OPEN_SLOT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.slotId = slotId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FSHOP_FLOORSHOP_OPEN_SLOT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_STATUS_RQ data;
	data.type = EPacketType::TOWER_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_PLAY_START_RQ(FString stageId, TArray<FString> heroUDs)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_PLAY_START_RQ data;
	data.type = EPacketType::TOWER_PLAY_START_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.heroUDs = heroUDs;
	data.stageId = stageId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_PLAY_START_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_PLAY_DONE_RQ(FString playUD, int32 clearGrade, int32 playTime)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_PLAY_DONE_RQ data;
	data.type = EPacketType::TOWER_PLAY_DONE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.playTime = playTime;
	data.clearGrade = clearGrade;
	data.playUD = playUD;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_PLAY_DONE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_TREASURE_BOX_RQ(int32 boxIdx)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_TREASURE_BOX_RQ data;
	data.type = EPacketType::TOWER_TREASURE_BOX_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.boxIdx = boxIdx;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_TREASURE_BOX_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(FString stageId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_BOSS_PLAY_CHECK_RQ data;
	data.type = EPacketType::TOWER_BOSS_PLAY_CHECK_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.stageId = stageId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_BOSS_PLAY_CHECK_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_BOSS_PLAY_RESULT_RQ(FString roomKey)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_BOSS_PLAY_RESULT_RQ data;
	data.type = EPacketType::TOWER_BOSS_PLAY_RESULT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.roomKey = roomKey;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_BOSS_PLAY_RESULT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_FLOOR_MISSION_ACTIVE_RQ(int32 floor)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_FLOOR_MISSION_ACTIVE_RQ data;
	data.type = EPacketType::TOWER_FLOOR_MISSION_ACTIVE_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.floor = floor;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_FLOOR_MISSION_ACTIVE_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TOWER_FLOOR_MISSION_REWARD_RQ(FString id)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTOWER_FLOOR_MISSION_REWARD_RQ data;
	data.type = EPacketType::TOWER_FLOOR_MISSION_REWARD_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.id = id;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTOWER_FLOOR_MISSION_REWARD_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::TREASURE_GET_REWARD_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FTREASURE_GET_REWARD_RQ data;
	data.type = EPacketType::TREASURE_GET_REWARD_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FTREASURE_GET_REWARD_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::GET_TARGET_SERVER_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FGET_TARGET_SERVER_RQ data;
	data.type = EPacketType::GET_TARGET_SERVER_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FGET_TARGET_SERVER_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::WORLDBOSS_STATUS_RQ()
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FWORLDBOSS_STATUS_RQ data;
	data.type = EPacketType::WORLDBOSS_STATUS_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FWORLDBOSS_STATUS_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::WORLDBOSS_PLAY_CHECK_RQ(FString worldBossStageId)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FWORLDBOSS_PLAY_CHECK_RQ data;
	data.type = EPacketType::WORLDBOSS_PLAY_CHECK_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.worldBossStageId = worldBossStageId;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FWORLDBOSS_PLAY_CHECK_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}

void UPacketFunctionLibrary::WORLDBOSS_PLAY_RESULT_RQ(FString roomKey)
{
    URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	FWORLDBOSS_PLAY_RESULT_RQ data;
	data.type = EPacketType::WORLDBOSS_PLAY_RESULT_RQ;
	data.clientkey = RGameInstance->HttpClient->clientkey;
	data.deviceid = RGameInstance->HttpClient->deviceid;
	data.protocol = PROTOCOL_VERSION;
	data.version = URAuth::GetInstancePtr()->GetBuildVersion();
	data.roomKey = roomKey;
	

	FString json_str;
	FJsonHelper::UStructToJsonObjectString(FWORLDBOSS_PLAY_RESULT_RQ::StaticStruct(), &data, json_str, 0, CPF_BlueprintReadOnly);

	json_str = json_str.Replace(TEXT("\r"), TEXT(""));
	json_str = json_str.Replace(TEXT("\t"), TEXT(""));
	json_str = json_str.Replace(TEXT("\n"), TEXT(""));

	RGameInstance->HttpClient->TransmitRequest(json_str);
}
