#include "ProjectR.h"
#include "RTSReceiveHandler.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Skill_V2/NetSkillManager.h"
#include "UI/InGameHUD/PVP/UP_PVP_ReadyToStart.h"
#include "Network/Chat/ChatManager.h"
#include "UI/RWidgetManager.h"
#include "ProjectRGameMode.h"

void URTSReceiveHandler::NotifyConnectionEvent(int32 eventId, URGameInstance* instance) 
{
    switch((RTSState)eventId){
        case RTSState::RTSState_ErrorOnPlaying: // 플레이 중, 오류 발생
        case RTSState::RTSState_ErrorBeforeTryMatch: // 메치 신청 중, 오류 발생
        case RTSState::RTSState_FailToEstablish: // 소켓 연결 오류
            UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] NotifyConnectionEvent >> eventId %d"), eventId);
            // do something notify
            // 엡을 종료해줘.
            break;
        default:
            UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] NotifyConnectionEvent >> eventId %d"), eventId);
            // do something notify
            // 엡을 종료해줘.
			break;
    }
}

void URTSReceiveHandler::OnDisconnect()
{

}

void URTSReceiveHandler::ErrorShowPopup(std::string code, URGameInstance* instance)
{
	if (IsValid(instance->RWidgetManager))
	{
		FString CheckString		= code.c_str();
		FString ErrorStringCode;
		//Wrong Server code
		if (CheckString.IsValidIndex(0) && CheckString.Contains(FString(TEXT("N"))))
		{
			ErrorStringCode = CheckString;
		}
		else
		{
			ErrorStringCode = FString(TEXT("NE")) + CheckString;
		}
		//auto ErrorStringCode	= FString(TEXT("NE")) + code.c_str();
		FName ErrorStringKey	= FName(*ErrorStringCode);
		FText LocalizedText		= UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Error, ErrorStringKey);
		FText ErrorCaption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY);
		UUIFunctionLibrary::ShowCommonPopup(ErrorCaption, LocalizedText, ECommonPopupType::VE_Error);
		if (instance)
		{
			instance->RWidgetManager->ExceptionRTSError();
		}
		// UUIFunctionLibrary::ShowCommonPopupSplash(ErrorCaption, ErrorText);
		
	}
}

bool URTSReceiveHandler::ErrorProcess(RTSCE type, std::string code, URGameInstance* instance)
{
	switch(type){
		case RTSCE::RTSCE_SUCCESS: 
			return true;
		case RTSCE::RTSCE_SKILL_NOT_READY:
		case RTSCE::RTSCE_SKILL_BLOCKED_CAST:
		case RTSCE::RTSCE_SKILL_INVALID_TARGET:
			// 전투로직의 경우, 기존 코드에서 처리하도록 전달해 줌.
			return true; 
		case RTSCE::RTSCE_MATCH_FAIL:
		case RTSCE::RTSCE_MATCH_TIMEOUT:
			UUIFunctionLibrary::HideMatchingWaiting();
			URTSReceiveHandler::ErrorShowPopup(code, instance);
			return false;
		case RTSCE::RTSCE_HERO_SELECT_FAIL:
		case RTSCE::RTSCE_NEED_MORE_HERO:
		case RTSCE::RTSCE_MATCH_VS_EXIT:
		case RTSCE::RTSCE_VS_EXIT:
			URTSReceiveHandler::ErrorShowPopup(code, instance);
			return false;
		case RTSCE::RTSCE_UNKNOWN:
		case RTSCE::RTSCE_OPERATION_FAIL:
		case RTSCE::RTSCE_SYSTEM_FAULT:
		case RTSCE::RTSCE_AUTH_FAIL:
		case RTSCE::RTSCE_DISCONNECTED:
			UUIFunctionLibrary::HideMatchingWaiting();
			URTSReceiveHandler::ErrorShowPopup(code, instance);
			return false;
	}
	return false;
}

void URTSReceiveHandler::OnACK_AUTH(S2C::ACK_AUTHT* data, URGameInstance* instance) 
{
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_AUTH(data);
}

void URTSReceiveHandler::OnACK_BT_ACTOR_CREATE(S2C::ACK_BT_ACTOR_CREATET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_CREATE >> error %d"), data->error->type);
	instance->RealTimeModManager->OnACK_BT_ACTOR_CREATE(data);
}

void URTSReceiveHandler::OnACK_BT_ACTOR_DESPAWN(S2C::ACK_BT_ACTOR_DESPAWNT* data, URGameInstance* instance) 
{
	instance->RealTimeModManager->OnACK_BT_ACTOR_DESPAWN(data);
}

void URTSReceiveHandler::OnACK_BT_ACTOR_DISAPPEAR(S2C::ACK_BT_ACTOR_DISAPPEART* data, URGameInstance* instance) 
{
	instance->RealTimeModManager->OnACK_BT_ACTOR_DISAPPEAR(data);
}

void URTSReceiveHandler::OnACK_BT_ACTOR_FORMATION(S2C::ACK_BT_ACTOR_FORMATIONT* data, URGameInstance* instance)
{

}

void URTSReceiveHandler::OnACK_BT_ACTOR_SPAWN(S2C::ACK_BT_ACTOR_SPAWNT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_SPAWN >> error %d"), data->error->type);
	instance->RealTimeModManager->OnACK_BT_ACTOR_SPAWN(data);
}

void URTSReceiveHandler::OnACK_BT_ACTOR_TELEPORT(S2C::ACK_BT_ACTOR_TELEPORTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_TELEPORT >> actorUK %d"), data->actorUK);
}

void URTSReceiveHandler::OnACK_BT_ACTOR_UPDATE(S2C::ACK_BT_ACTOR_UPDATET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_UPDATE >> error %d"), data->error->type);
	instance->RealTimeModManager->OnACK_BT_ACTOR_UPDATE(data);
}

void URTSReceiveHandler::OnACK_BT_ATTACK_NORMAL(S2C::ACK_BT_ATTACK_NORMALT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ATTACK_NORMAL >> error %d"), data->error->type);
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
		instance->RealTimeModManager->OnACK_BT_ATTACK_NORMAL(data);
}

void URTSReceiveHandler::OnACK_BT_ATTACK_NOW(S2C::ACK_BT_ATTACK_NOWT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ATTACK_NOW >> error %d"), data->error->type);
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
	{
		instance->RealTimeModManager->OnACK_BT_ATTACK_NOW(data);
	}
}

void URTSReceiveHandler::OnACK_BT_CHANGE_CREW(S2C::ACK_BT_CHANGE_CREWT* data, URGameInstance* instance) 
{
	instance->RealTimeModManager->OnACK_BT_CHANGE_CREW(data);
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_CHANGE_CREW >> error %d"), data->error->type);
}

void URTSReceiveHandler::OnACK_BT_CHANGE_SPIRIT(S2C::ACK_BT_CHANGE_SPIRITT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_CHANGE_SPIRIT >> error %d"), data->error->type);
	instance->RealTimeModManager->OnACK_BT_CHANGE_SPIRIT(data);
}

void URTSReceiveHandler::OnACK_BT_DICE_COUNT(S2C::ACK_BT_DICE_COUNTT* data, URGameInstance* instance)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_DICE_COUNT >> error %d"), data->error->type);
}

void URTSReceiveHandler::OnACK_BT_DICE_NUM(S2C::ACK_BT_DICE_NUMT* data, URGameInstance* instance)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_DICE_NUM >> error %d"), data->error->type);

}

void URTSReceiveHandler::OnACK_BT_DICE_OPEN(S2C::ACK_BT_DICE_OPENT* data, URGameInstance* instance)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_DICE_OPEN >> error %d"), data->error->type);

}

void URTSReceiveHandler::OnACK_BT_DICE_RESULT(S2C::ACK_BT_DICE_RESULTT* data, URGameInstance* instance)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_DICE_RESULT >> error %d"), data->error->type);

}

void URTSReceiveHandler::OnACK_BT_DOOR_EVENT(S2C::ACK_BT_DOOR_EVENTT* data, URGameInstance* instance)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_DOOR_EVENT >> error %d"), data->error->type);
	instance->RealTimeModManager->OnACK_BT_DOOR_EVENT(data);
}

void URTSReceiveHandler::OnACK_BT_DROP_CREW(S2C::ACK_BT_DROP_CREWT* data, URGameInstance* instance)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_DOOR_EVENT >> error %d"), data->error->type);
	instance->RealTimeModManager->OnACK_BT_DROP_CREW(data);
}

void URTSReceiveHandler::OnACK_BT_NEW_PLAYER(S2C::ACK_BT_NEW_PLAYERT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_NEW_PLAYER >> error %d"), data->error->type);
}

void URTSReceiveHandler::OnACK_BT_SKILL_ACTIVATED(S2C::ACK_BT_SKILL_ACTIVATEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_SKILL_ACTIVATED >> error %d"), data->error->type);
	if (nullptr == instance)
		return;
	if (nullptr == instance->NetSkillManager)
		return;

	if (data->error->type == RTSCE::RTSCE_SUCCESS)
		instance->NetSkillManager->RecvRTS_UpdateSkillCool(data);
}

void URTSReceiveHandler::OnACK_BT_SKILL_BLOCKED(S2C::ACK_BT_SKILL_BLOCKEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ACK_BT_SKILL_BLOCKEDT >> error %d"), data->error->type);
	if (nullptr == instance)
		return;
	if (nullptr == instance->NetSkillManager)
		return;
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
	{
		instance->NetSkillManager->RecvRTS_SkillBlocked(data);
	}
}

void URTSReceiveHandler::OnACK_BT_SKILL_CASTED(S2C::ACK_BT_SKILL_CASTEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_SKILL_CASTED >> error %d"), data->error->type);
	if (nullptr == instance)
		return;
	if (nullptr == instance->NetSkillManager)
		return;
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
	{
		if (data->casting)
			instance->NetSkillManager->RecvRTS_CastSkill(data);
		else
			instance->NetSkillManager->RecvRTS_ApplySkill(data);
	}
	else
	{
		instance->NetSkillManager->RecvRTS_SkillFailed(data->actorUK , data->error->type);
	}
}

void URTSReceiveHandler::OnACK_BT_SKILL_CASTED_INTERVAL(S2C::ACK_BT_SKILL_CASTED_INTERVALT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_SKILL_CASTED_INTERVAL >> error %d"), data->error->type);
	if (nullptr == instance)
		return;
	if (nullptr == instance->NetSkillManager)
		return;
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
	{
		instance->NetSkillManager->RecvRTS_IntervalSkill(data);
	}
	else
	{
		
		instance->NetSkillManager->RecvRTS_SkillFailed(data->actorUK, data->error->type);
	}
}

void URTSReceiveHandler::OnACK_BT_SYNC_ACTOR_UPDATE(S2C::ACK_BT_SYNC_ACTOR_UPDATET* data, URGameInstance* instance) 
{
	instance->RealTimeModManager->OnACK_BT_SYNC_ACTOR_UPDATE(data);
}

void URTSReceiveHandler::OnACK_BT_SYNC_ACTOR_SCORE(S2C::ACK_BT_SYNC_ACTOR_SCORET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_SYNC_ACTOR_SCORE"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], frame[%d]"), data->error->type, data->frame);
	URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance);
}

void URTSReceiveHandler::OnACK_BT_SYNC_PLAYER_SCORE(S2C::ACK_BT_SYNC_PLAYER_SCORET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_SYNC_PLAYER_SCORE >> error %d"), data->error->type);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_MR_PLAYER_SCORE(data);
	/*
	for (auto& scoreData : data->scores)
	{
		FBattleRecord* BattleRecord = instance->BattleRecordManager->GetBattleRecord(scoreData->actorUK);
		if (!BattleRecord)
		{
			BattleRecord->nAmountOfNormalAttack = scoreData->damage;
			BattleRecord->nAmountOfSkillAttack = scoreData->damageSkill;
			BattleRecord->nAmountOfDamage = scoreData->damaged;
			BattleRecord->nAmountOfHeal = scoreData->heal;
			BattleRecord->nKillCount = scoreData->kill;
			BattleRecord->nSkillCount = scoreData->skill;
		}
	}
	instance->EventManager->OnRTS_BT_SYNC_PLAYER_SCORE_RP.Broadcast();
	*/
}

void URTSReceiveHandler::OnACK_BT_SYNC_SKILL_UPDATE(S2C::ACK_BT_SYNC_SKILL_UPDATET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_SYNC_SKILL_UPDATE >> error %d"), data->error->type);
	if (nullptr == instance)
		return;
	if (nullptr == instance->NetSkillManager)
		return;
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
		instance->NetSkillManager->RecvRTS_UpdateSkillSlot(data);
}

void URTSReceiveHandler::OnACK_COMMON_ERROR(S2C::ACK_COMMON_ERRORT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_COMMON_ERROR >> error %d"), data->error->type);
	URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance);
}

void URTSReceiveHandler::OnACK_DISCONNECTED(S2C::ACK_DISCONNECTEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_DISCONNECTED >> error %d"), data->error->type);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_DISCONNECTED(data->error->type);
}

void URTSReceiveHandler::OnACK_MATCHED(S2C::ACK_MATCHEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_MATCHED"));
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_MATCHED(data);
}

void URTSReceiveHandler::OnACK_MATCH_ROUND(S2C::ACK_MATCH_ROUNDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_MATCH_ROUND"));
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_MATCH_ROUND(data);
}

void URTSReceiveHandler::OnACK_PARTY_GO(S2C::ACK_PARTY_GOT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PARTY_GO"));
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_PARTY_GO(data);
}

void URTSReceiveHandler::OnACK_PARTY_LEADER_SLOT(S2C::ACK_PARTY_LEADER_SLOTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PARTY_LEADER_SLOT"));
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_LEADER_SLOT(data);
}

void URTSReceiveHandler::OnACK_PARTY_PLAYER(S2C::ACK_PARTY_PLAYERT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PARTY_PLAYER"));
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_PARTY_PLAYER(data);	
}

void URTSReceiveHandler::OnACK_PARTY_PLAYER_EXIT(S2C::ACK_PARTY_PLAYER_EXITT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PARTY_PLAYER_EXIT"));
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_PLAYER_EXIT(data);
}

void URTSReceiveHandler::OnACK_PARTY_UPDATE_PUBLIC(S2C::ACK_PARTY_UPDATE_PUBLICT* data, URGameInstance* instance) 
{
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_PARTY_UPDATE_PUBLIC(data);
}

//void URTSReceiveHandler::OnACK_MATCH(S2C::ACK_MATCHT* data, URGameInstance* instance) {
//
//#if LOG_NET_RTS
//	UE_LOG(LogNetwork, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_MATCH >> error %d"), data->error->type);
//#endif
//	instance->RealTimeModManager->OnACK_MATCH(data);
//}

void URTSReceiveHandler::OnACK_PLAY_COUNT(S2C::ACK_PLAY_COUNTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PLAY_COUNT >> error %d"), data->error->type);
	
	UUP_PVP_ReadyToStart* PVPWidget = Cast<UUP_PVP_ReadyToStart>(RGAMEINSTANCE(GEngine)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_PVP_ReadyToStart));
	if (PVPWidget && PVPWidget->IsInViewport())
	{
		PVPWidget->WidgetSwitcher_Count->SetActiveWidgetIndex(1);
		PVPWidget->TextBlock_PickTimer->SetText(FText::AsNumber(data->count));
		PVPWidget->PlayAni(TEXT("CountEffect"));
		if (data->count == 1)
		{
			PVPWidget->PlayAnimation(PVPWidget->Start_VersusAnim, 1.4f);
			PVPWidget->Duration = 1.f;
		}
			
	}
	
	/*UUserWidget* timecountUI = instance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TimeCount);
	if (!timecountUI->IsValidLowLevel())
	{
		timecountUI = instance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_TimeCount);
		instance->RWidgetManager->ShowUserWidget(EWidgetBluePrintEnum::WBP_TimeCount);
	}*/
}

void URTSReceiveHandler::OnACK_PLAY_READY(S2C::ACK_PLAY_READYT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PLAY_READY >> error %d"), data->error->type);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_PLAY_READY(data);
}

//void URTSReceiveHandler::OnACK_PLAY_READY_DONE(S2C::ACK_PLAY_READY_DONET* data, URGameInstance* instance) {
//
//#if LOG_NET_RTS
//	UE_LOG(LogNetwork, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PLAY_READY_DONE >> error %d"), data->error->type);
//#endif
//
//	if (data->error->type == RTSCE::RTSCE_SUCCESS)
//	{
//		instance->EventManager->OnRTS_PLAY_READY_DONE_RP.Broadcast();
//	}
//}

void URTSReceiveHandler::OnACK_PLAY_SEND_RESULT(S2C::ACK_PLAY_SEND_RESULTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PLAY_SEND_RESULT >> error %d"), data->error->type);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
	{
		if (URealTimeModManager::IsRealTimeGameMode())
		{
			instance->RealTimeModManager->OnACK_PLAY_SEND_RESULT(data);
			//instance->ColosseumManager->RequestColosseumInfo();
//			instance->EventManager->OnRTS_PLAY_SEND_RESULT_RP.Broadcast();
		}
	}
}

void URTSReceiveHandler::OnACK_PLAY_START(S2C::ACK_PLAY_STARTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PLAY_START >> error %d"), data->error->type);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_PLAY_START(data);
}

void URTSReceiveHandler::OnACK_PLAY_TIMEOUT(S2C::ACK_PLAY_TIMEOUTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PLAY_TIMEOUT >> error %d"), data->error->type);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_PLAY_TIMEOUT(data);
}

void URTSReceiveHandler::OnACK_STEP_GO(S2C::ACK_STEP_GOT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_STEP_GO"));
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_STEP_GO(data);
}

void URTSReceiveHandler::OnACK_BT_SYNC_TIME_REMAIN(S2C::ACK_BT_SYNC_TIME_REMAINT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_SYNC_TIME_REMAIN"));
	instance->RealTimeModManager->OnACK_BT_SYNC_TIME_REMAIN((float)data->remainSec, data->notify);
}

void URTSReceiveHandler::OnACK_BT_ZONE_EVENT(S2C::ACK_BT_ZONE_EVENTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ZONE_STATE"));
	OVERLORD::RTS::ENUM::RTSZoneEvent ZoneEvent = (OVERLORD::RTS::ENUM::RTSZoneEvent)data->event;
// 	switch (ZoneEvent)
// 	{
// 	case OVERLORD::RTS::ENUM::RTSZoneEvent::RTSZoneEvent_PeaceOn:
// 		break;
// 	case OVERLORD::RTS::ENUM::RTSZoneEvent::RTSZoneEvent_PeaceOff:
// 		break;
// 	case OVERLORD::RTS::ENUM::RTSZoneEvent::RTSZoneEvent_ExchangeStart:
// 	{
// 		if (instance->RealTimeModManager->IsMyPlayActorUK(data->actorUK))
// 		{
// 			instance->EventManager->OnModRoomProgressMedal.Broadcast(true);
// 		}
// 	}
// 	break;
// 	case OVERLORD::RTS::ENUM::RTSZoneEvent::RTSZoneEvent_ExchangeCancel:
// 	case OVERLORD::RTS::ENUM::RTSZoneEvent::RTSZoneEvent_ExchangeComplete:
// 	{
// 		if (instance->RealTimeModManager->IsMyPlayActorUK(data->actorUK))
// 		{
// 			instance->EventManager->OnModRoomProgressMedal.Broadcast(false);
// 		}
// 	}
// 	break;
// 	}
}

void URTSReceiveHandler::OnACK_BT_ZONE_STATE(S2C::ACK_BT_ZONE_STATET* data, URGameInstance* instance) 
{
 	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ZONE_STATE"));
// 
// 	switch (data->event)
// 	{
// 	case 1:	instance->RealTimeModManager->ExchangeZoneState(data->zone, true);	break;
// 	case 2:	instance->RealTimeModManager->ExchangeZoneState(data->zone, false);	break;
// 	}
}


void URTSReceiveHandler::OnACK_BT_TEAM_POINT_UPDATE(S2C::ACK_BT_TEAM_POINT_UPDATET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_TEAM_POINT_UPDATE"));
//	instance->RealTimeModManager->OnACK_BT_TEAM_POINT_UPDATE(data);
}

void URTSReceiveHandler::OnACK_BT_ACTOR_MEDAL_UPDATE(S2C::ACK_BT_ACTOR_MEDAL_UPDATET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_MEDAL_UPDATE"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], frame[%d], actorUK[%d], actorMedal[%d], victimUK[%d], victimMedal[%d]"), data->error->type, data->frame, data->actorUK, data->actorMedal, data->victimUK, data->victimMedal);
// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (!pGamePlayMode)
// 	{
// 		checkf(false, TEXT("[URTSReceiveHandler::OnACK_BT_ACTOR_MEDAL_UPDATE]"));
// 		return;
// 	}
// 	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
// 	if (!pGameMode)
// 	{
// 		checkf(false, TEXT("[URTSReceiveHandler::OnACK_BT_ACTOR_MEDAL_UPDATE]"));
// 		return;
// 	}
// 	TWeakObjectPtr<AActor> ActorPtr = pGamePlayMode->GetObjectByPlayerUK(data->actorUK);
// 	if (ActorPtr.IsValid())
// 	{
// 		auto Actor_BaseProp = Cast<IEntityBaseProperty>(ActorPtr.Get());
// 		if (Actor_BaseProp)
// 		{
// 			Actor_BaseProp->SetRealTimeParam(data->actorMedal);
// 		}
// 	}
// 
// 	instance->RealTimeModManager->Update_RTS_PlayerScore(data->actorScore.get());
}

void URTSReceiveHandler::OnACK_BT_ACTOR_RESPAWN(S2C::ACK_BT_ACTOR_RESPAWNT* data, URGameInstance* instance)
{
	instance->RealTimeModManager->OnACK_BT_ACTOR_RESPAWN(data);
}

void URTSReceiveHandler::OnACK_COST_UPDATE_AVATAR(S2C::ACK_COST_UPDATE_AVATART* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_COST_UPDATE_AVATAR")); 
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], avatar[%s]"), data->error->type, data->avatar.c_str());
//	instance->RealTimeModManager->OnACK_COST_UPDATE_AVATART(data);
}

void URTSReceiveHandler::OnACK_DECK_CACHED_DONE(S2C::ACK_DECK_CACHED_DONET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_DECK_CACHED_DONE"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d]"), data->error->type);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_DECK_CACHED_DONE(data);
}

void URTSReceiveHandler::OnACK_DECK_SCENE_LOADED(S2C::ACK_DECK_SCENE_LOADEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_DECK_SCENE_LOADED"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d]"), data->error->type);
	URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance);
}

void URTSReceiveHandler::OnACK_DECK_SELECT(S2C::ACK_DECK_SELECTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_DECK_SELECT"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], crewNo[%d], heroId[%s], heroUD[%s], level[%d], slot[%d]"), data->error->type, data->crewNo, data->heroId.c_str(), data->heroUD.c_str(), data->level, data->slot);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_DECK_SELECT(data);
}

void URTSReceiveHandler::OnACK_DECK_SELECT_READY(S2C::ACK_DECK_SELECT_READYT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_DECK_SELECT_READY"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], ready[%d], slot[%d]"), data->error->type, data->ready, data->slot);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_DECK_SELECT_READY(data);
}

void URTSReceiveHandler::OnCHAT_CHANNEL_ENTER(S2C::CHAT_CHANNEL_ENTERT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnCHAT_CHANNEL_ENTER"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], channel[%s], kId[%d], nick[%s]"), data->error->type, data->channel.c_str(), data->kId, data->nick.c_str());
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
		instance->ChatManager->OnReceive_UserChannelEnter(UTF8_TO_TCHAR(data->channel.c_str()), data->kId, UTF8_TO_TCHAR(data->nick.c_str()));
}
void URTSReceiveHandler::OnCHAT_CHANNEL_EXIT(S2C::CHAT_CHANNEL_EXITT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnCHAT_CHANNEL_EXIT")); 
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], channel[%s], kId[%d], nick[%s]"), data->error->type, data->channel.c_str(), data->kId, data->nick.c_str());
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
		instance->ChatManager->OnReceive_UserChannelLeave(UTF8_TO_TCHAR(data->channel.c_str()), data->kId, UTF8_TO_TCHAR(data->nick.c_str()));
}

void URTSReceiveHandler::OnCHAT_MESSAGE(S2C::CHAT_MESSAGET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnCHAT_MESSAGE")); 
//	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], channel[%s], chattype[%d], kId[%d] message[%s] nick[%s]"), data->error->type, data->channel.c_str(), data->chattype, data->kId, data->message.c_str(), data->nick.c_str());
	EChatListEnum type;
	TArray<FString> extraData;
	UChatManager* manager = instance->ChatManager;
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
	{
		type = manager->stringToEnum(data->channel.at(0));
		extraData = manager->SplitDataString(UTF8_TO_TCHAR(data->data.c_str()));
		manager->OnReceive_Message(type, data->chattype, data->kId, UTF8_TO_TCHAR(data->nick.c_str()), UTF8_TO_TCHAR(data->message.c_str()), extraData);
	}
}

void URTSReceiveHandler::OnCHAT_SUBSCRIBED(S2C::CHAT_SUBSCRIBEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnCHAT_SUBSCRIBED"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], channel[%s]"), data->error->type, data->channel.c_str());
	
	if (data->error->type == RTSCE::RTSCE_SUCCESS)
	{
		if (data->channel.at(0) == 'C')
			instance->ChatManager->OnReceive_OnChatSubscribed(FString(UTF8_TO_TCHAR(data->channel.c_str())));
		// else if( data->channel.at(0)=='G') instance->ChatManager->OnReceive_OnChatSubscribed(FString(UTF8_TO_TCHAR(data->channel.c_str())));
	}
}

void URTSReceiveHandler::OnCHAT_UNSUBSCRIBED(S2C::CHAT_UNSUBSCRIBEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnCHAT_UNSUBSCRIBED")); 
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], channel[%s]"), data->error->type, data->channel.c_str());
}

void URTSReceiveHandler::OnREQ_CINEMATIC_PLAY(S2C::REQ_CINEMATIC_PLAYT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnREQ_CINEMATIC_PLAY"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], sequence[%d]"), data->error->type, data->sequence);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnREQ_CINEMATIC_PLAY(data);
}

void URTSReceiveHandler::OnACK_CINEMATIC_DONE(S2C::ACK_CINEMATIC_DONET* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_CINEMATIC_DONE"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], sequence[%d]"), data->error->type, data->sequence);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_CINEMATIC_DONE(data);
}

void URTSReceiveHandler::OnREQ_STEP_READY(S2C::REQ_STEP_READYT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnREQ_STEP_READY")); 
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], sequence[%d]"), data->error->type, data->sequence);
	instance->RealTimeModManager->OnREQ_STEP_READY(data);
}
void URTSReceiveHandler::OnACK_CS_DECK_SELECT(S2C::ACK_CS_DECK_SELECTT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_CS_DECK_SELECT")); 
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], crewNo[%d], heroId[%s], heroUD[%s], kId[%d], level[%d]"), data->error->type, data->crewNo, data->heroId.c_str(), data->heroUD.c_str(), data->kId, data->level);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_CS_DECK_SELECT(data);
}
void URTSReceiveHandler::OnACK_CS_DECK_SELECT_READY(S2C::ACK_CS_DECK_SELECT_READYT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_CS_DECK_SELECT_READY"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], kId[%d], ready[%d]"), data->error->type, data->kId, data->ready);
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_CS_DECK_SELECT_READY(data);
}

void URTSReceiveHandler::OnACK_CS_DECK_TIME_REMAIN(S2C::ACK_CS_DECK_TIME_REMAINT* data, URGameInstance* instance)
{
	if(URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance))
		instance->RealTimeModManager->OnACK_CS_DECK_TIME_REMAIN(data);
}

void URTSReceiveHandler::OnACK_CS_MATCHED(S2C::ACK_CS_MATCHEDT* data, URGameInstance* instance) 
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_CS_MATCHED"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] ErrorType[%d], modId[%s], modType[%d], roomkey[%s]"), data->error->type, data->modId.c_str(), data->modType, data->roomkey.c_str());
	if(
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) &&
		URTSReceiveHandler::ErrorProcess((RTSCE)data->error->type, data->error->code, instance)
	) instance->RealTimeModManager->OnACK_CS_MATCHED(data);
}
