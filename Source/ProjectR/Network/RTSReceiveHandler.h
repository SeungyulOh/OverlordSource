#pragma once

#include "./RTSProtocol/rts_generated.h"

using namespace OVERLORD;
using namespace OVERLORD::RTS;
using namespace OVERLORD::RTS::ENUM;

class URGameInstance;

class URTSReceiveHandler
{
    public:
        static void OnACK_AUTH(S2C::ACK_AUTHT* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_CREATE(S2C::ACK_BT_ACTOR_CREATET* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_DESPAWN(S2C::ACK_BT_ACTOR_DESPAWNT* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_DISAPPEAR(S2C::ACK_BT_ACTOR_DISAPPEART* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_FORMATION(S2C::ACK_BT_ACTOR_FORMATIONT* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_MEDAL_UPDATE(S2C::ACK_BT_ACTOR_MEDAL_UPDATET* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_RESPAWN(S2C::ACK_BT_ACTOR_RESPAWNT* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_SPAWN(S2C::ACK_BT_ACTOR_SPAWNT* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_TELEPORT(S2C::ACK_BT_ACTOR_TELEPORTT* data, URGameInstance* instance);
        static void OnACK_BT_ACTOR_UPDATE(S2C::ACK_BT_ACTOR_UPDATET* data, URGameInstance* instance);
        static void OnACK_BT_ATTACK_NORMAL(S2C::ACK_BT_ATTACK_NORMALT* data, URGameInstance* instance);
        static void OnACK_BT_ATTACK_NOW(S2C::ACK_BT_ATTACK_NOWT* data, URGameInstance* instance);
        static void OnACK_BT_CHANGE_CREW(S2C::ACK_BT_CHANGE_CREWT* data, URGameInstance* instance);
        static void OnACK_BT_CHANGE_SPIRIT(S2C::ACK_BT_CHANGE_SPIRITT* data, URGameInstance* instance);
        static void OnACK_BT_DICE_COUNT(S2C::ACK_BT_DICE_COUNTT* data, URGameInstance* instance);
        static void OnACK_BT_DICE_NUM(S2C::ACK_BT_DICE_NUMT* data, URGameInstance* instance);
        static void OnACK_BT_DICE_OPEN(S2C::ACK_BT_DICE_OPENT* data, URGameInstance* instance);
        static void OnACK_BT_DICE_RESULT(S2C::ACK_BT_DICE_RESULTT* data, URGameInstance* instance);
        static void OnACK_BT_DOOR_EVENT(S2C::ACK_BT_DOOR_EVENTT* data, URGameInstance* instance);
        static void OnACK_BT_DROP_CREW(S2C::ACK_BT_DROP_CREWT* data, URGameInstance* instance);
        static void OnACK_BT_NEW_PLAYER(S2C::ACK_BT_NEW_PLAYERT* data, URGameInstance* instance);
        static void OnACK_BT_SKILL_ACTIVATED(S2C::ACK_BT_SKILL_ACTIVATEDT* data, URGameInstance* instance);
        static void OnACK_BT_SKILL_BLOCKED(S2C::ACK_BT_SKILL_BLOCKEDT* data, URGameInstance* instance);
        static void OnACK_BT_SKILL_CASTED(S2C::ACK_BT_SKILL_CASTEDT* data, URGameInstance* instance);
        static void OnACK_BT_SKILL_CASTED_INTERVAL(S2C::ACK_BT_SKILL_CASTED_INTERVALT* data, URGameInstance* instance);
        static void OnACK_BT_SYNC_ACTOR_SCORE(S2C::ACK_BT_SYNC_ACTOR_SCORET* data, URGameInstance* instance);
        static void OnACK_BT_SYNC_ACTOR_UPDATE(S2C::ACK_BT_SYNC_ACTOR_UPDATET* data, URGameInstance* instance);
        static void OnACK_BT_SYNC_PLAYER_SCORE(S2C::ACK_BT_SYNC_PLAYER_SCORET* data, URGameInstance* instance);
        static void OnACK_BT_SYNC_SKILL_UPDATE(S2C::ACK_BT_SYNC_SKILL_UPDATET* data, URGameInstance* instance);
        static void OnACK_BT_SYNC_TIME_REMAIN(S2C::ACK_BT_SYNC_TIME_REMAINT* data, URGameInstance* instance);
        static void OnACK_BT_TEAM_POINT_UPDATE(S2C::ACK_BT_TEAM_POINT_UPDATET* data, URGameInstance* instance);
        static void OnACK_BT_ZONE_EVENT(S2C::ACK_BT_ZONE_EVENTT* data, URGameInstance* instance);
        static void OnACK_BT_ZONE_STATE(S2C::ACK_BT_ZONE_STATET* data, URGameInstance* instance);
        static void OnACK_CINEMATIC_DONE(S2C::ACK_CINEMATIC_DONET* data, URGameInstance* instance);
        static void OnACK_COMMON_ERROR(S2C::ACK_COMMON_ERRORT* data, URGameInstance* instance);
        static void OnACK_COST_UPDATE_AVATAR(S2C::ACK_COST_UPDATE_AVATART* data, URGameInstance* instance);
        static void OnACK_CS_DECK_SELECT(S2C::ACK_CS_DECK_SELECTT* data, URGameInstance* instance);
        static void OnACK_CS_DECK_SELECT_READY(S2C::ACK_CS_DECK_SELECT_READYT* data, URGameInstance* instance);
        static void OnACK_CS_DECK_TIME_REMAIN(S2C::ACK_CS_DECK_TIME_REMAINT* data, URGameInstance* instance);
        static void OnACK_CS_MATCHED(S2C::ACK_CS_MATCHEDT* data, URGameInstance* instance);
        static void OnACK_DECK_CACHED_DONE(S2C::ACK_DECK_CACHED_DONET* data, URGameInstance* instance);
        static void OnACK_DECK_SCENE_LOADED(S2C::ACK_DECK_SCENE_LOADEDT* data, URGameInstance* instance);
        static void OnACK_DECK_SELECT(S2C::ACK_DECK_SELECTT* data, URGameInstance* instance);
        static void OnACK_DECK_SELECT_READY(S2C::ACK_DECK_SELECT_READYT* data, URGameInstance* instance);
        static void OnACK_DISCONNECTED(S2C::ACK_DISCONNECTEDT* data, URGameInstance* instance);
        static void OnACK_MATCHED(S2C::ACK_MATCHEDT* data, URGameInstance* instance);
        static void OnACK_MATCH_ROUND(S2C::ACK_MATCH_ROUNDT* data, URGameInstance* instance);
        static void OnACK_PARTY_GO(S2C::ACK_PARTY_GOT* data, URGameInstance* instance);
        static void OnACK_PARTY_LEADER_SLOT(S2C::ACK_PARTY_LEADER_SLOTT* data, URGameInstance* instance);
        static void OnACK_PARTY_PLAYER(S2C::ACK_PARTY_PLAYERT* data, URGameInstance* instance);
        static void OnACK_PARTY_PLAYER_EXIT(S2C::ACK_PARTY_PLAYER_EXITT* data, URGameInstance* instance);
        static void OnACK_PARTY_UPDATE_PUBLIC(S2C::ACK_PARTY_UPDATE_PUBLICT* data, URGameInstance* instance);
        static void OnACK_PLAY_COUNT(S2C::ACK_PLAY_COUNTT* data, URGameInstance* instance);
        static void OnACK_PLAY_READY(S2C::ACK_PLAY_READYT* data, URGameInstance* instance);
        static void OnACK_PLAY_SEND_RESULT(S2C::ACK_PLAY_SEND_RESULTT* data, URGameInstance* instance);
        static void OnACK_PLAY_START(S2C::ACK_PLAY_STARTT* data, URGameInstance* instance);
        static void OnACK_PLAY_TIMEOUT(S2C::ACK_PLAY_TIMEOUTT* data, URGameInstance* instance);
        static void OnACK_STEP_GO(S2C::ACK_STEP_GOT* data, URGameInstance* instance);
        static void OnCHAT_CHANNEL_ENTER(S2C::CHAT_CHANNEL_ENTERT* data, URGameInstance* instance);
        static void OnCHAT_CHANNEL_EXIT(S2C::CHAT_CHANNEL_EXITT* data, URGameInstance* instance);
        static void OnCHAT_MESSAGE(S2C::CHAT_MESSAGET* data, URGameInstance* instance);
        static void OnCHAT_SUBSCRIBED(S2C::CHAT_SUBSCRIBEDT* data, URGameInstance* instance);
        static void OnCHAT_UNSUBSCRIBED(S2C::CHAT_UNSUBSCRIBEDT* data, URGameInstance* instance);
        static void OnREQ_CINEMATIC_PLAY(S2C::REQ_CINEMATIC_PLAYT* data, URGameInstance* instance);
        static void OnREQ_STEP_READY(S2C::REQ_STEP_READYT* data, URGameInstance* instance);

		static void NotifyConnectionEvent(int32 eventId, URGameInstance* instance);
	    static bool ErrorProcess(RTSCE type, std::string code, URGameInstance* instance);
		static void ErrorShowPopup(std::string code, URGameInstance* instance);
		static void OnDisconnect();
};