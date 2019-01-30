#pragma once

#include "GlobalConstants.h"

// #include "UObject/NoExportTypes.h"
#include "IPAddress.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "PacketDataEnum.h"
#include "Networking.h"

#include "./RTSProtocol/rts_generated.h"
using namespace OVERLORD;
using namespace OVERLORD::RTS;
using namespace OVERLORD::RTS::ENUM;
using namespace flatbuffers;

// #define RECV_BUFFER_SIZE 2048
#define PACKET_BUFFER_SIZE 65535
#define RTS_THREAD_WAIT_IN_SEC 10.f
#define RTS_PACKET_HEADER_SIZE 6

#define RTS_PACKET_HEADER_MAGIC0 82
#define RTS_PACKET_HEADER_MAGIC1 86
#define RTS_AUTH_CODE 76
#define RTS_PACKET_MAX_READ 20 // 0.5 sec


class URGameInstance;

class URTSManager : public FRunnable
{
	public:
		static URTSManager *GetInstancePtr();
		static void ReleaseSingleton();
		void SetConnectionParam(FString &IP, int32 Port);
		bool Connect();
		void Disconnect();
		bool IsConnected();
		bool IsConnectedMode(EPVP modType);
        EPVP getConnectedMode();
		void setCurrentFrame(long frame);
		long getCurrentFrame();
		void Initialize(URGameInstance* instance);
		void FrameMove(float DeltaTime);
        void NPC_CACHE_LIST(FName StageID, TArray<FName>& actorIds);

    protected:
    	FRunnableThread* SocketProcedureThread;	
        FEvent* WorkEvent;
        double CheckTimeTemp	= 0.f;
        double LastTimeUpdate	= 0.f;

        /** FRunnable implementation */
        bool Init() override;
        uint32 Run() override;
        void Stop() override;
        void Exit() override;
        /** FRunnable implementation */

	private:
		URTSManager();
		URTSManager(const URTSManager &other);
		~URTSManager();
		
		static URTSManager* InstancePtr;
		URGameInstance*		RGameInstance = nullptr;

		class ISocketSubsystem* SSS;
		TSharedPtr<FInternetAddr> Addr = nullptr;
		TUniquePtr<FSocket> Socket = nullptr;
		TArray<uint8> Chunk;
		TQueue<std::shared_ptr<CMD_RTST>> ReceiveBuffer;
		long currentFrame = 0;
		double accumulatedTime = 0.f;
		EPVP currentMode = EPVP::NONE;
		// long attackedFrame = 0; // 공격중!
		// bool attackNow = false;
		// uint32 selectedActor = 0;
		// uint32 countFrame = 0;

		UNION_RTSUnion U;
        bool bIsReady = false;
		ESocketConnectionState SocketState = ESocketConnectionState::SCS_NotConnected; // SCS_ConnectionError SCS_Connected
		SessionState sessionState = SessionState::SessionState_None; // SCS_ConnectionError SCS_Connected
		LazyCondition lazy = LazyCondition::LazyCondition_Healthy;

		// TArray<TArray<uint8>> SendQueue;
		// bool EnqueueSend(const TArray<uint8> &Data);
		// uint8 ReceiveBuffer[RECV_BUFFER_SIZE];

		// Private Methods
		void authorize();
		//void sendPing();
		// void NormalAttack(uint32 actorUK);
		void ReadSocketData();
        void checkSocketStatus();
		void SendPacket(UNION_RTS type, flatbuffers::NativeTable *table);
        // void RecvPacket(const uint8* Packet, int32 Size);
		// void ChangeNormalAttackState(bool state);
		void PacketParser(const uint8* packet);
		void PropagatePacket(std::shared_ptr<CMD_RTST> CMD);

	public: // For Request API
		void ACK_CINEMATIC_DONE(uint8 sequence);
		void ACK_STEP_AND_GO(uint8 sequence);
		void CHAT_CHANNEL_MOVE(FString channel);
		void CHAT_MESSAGE(FString channel, uint8 chattype, int32 kId, FString nick, FString message, FString data);
		void CHAT_SUBSCRIBE(FString channel);
		void REQ_AUTH(FString clientkey, FString deviceid, uint32 t);
		void REQ_BT_ACTOR_CREATE(uint8 category, FString actorId, uint8 role, TArray<FString> option);
		void REQ_BT_ACTOR_FORMATION(uint16 actorUK);
		void REQ_BT_ACTOR_MOVE(uint16 actorUK, bool isStop, FVector location, FVector direction);
		void REQ_BT_ACTOR_SCORE();
		void REQ_BT_ACTOR_SPAWN(uint16 actorUK, FVector location, FVector direction);
		void REQ_BT_ACTOR_TELEPORT(uint16 actorUK, uint8 portalNo);
		void REQ_BT_ATTACK_NOW(uint16 actorUK, FVector actorLocation, uint16 targetUK, bool bAttack, uint8 sequence);
		void REQ_BT_CHANGE_CREW(uint16 actorUK, uint16 targetUK, FVector location, FVector direction);
		void REQ_BT_CHANGE_SPIRIT(uint16 actorUK, uint16 spiritIndex);
		void REQ_BT_DICE(bool participate);
		void REQ_BT_DROP_CREW(uint16 actorUK, FVector location, FVector direction);
		void REQ_BT_ENTER();
		void REQ_BT_EXIT();
		void REQ_BT_PLAYER_SCORE();
		void REQ_BT_SKILL_CAST(uint16 actorUK, FVector actorLocation, uint8 slot, uint16 targetUK, FVector location, FVector direction);
		void REQ_CS_DECK_SELECT(uint8 crewNo, FString heroUD, FString heroId, int32 level);
		void REQ_CS_DECK_SELECT_READY(bool ready);
		void REQ_CS_PARTY_UI_READY();
		void REQ_DECK_SELECT(uint8 slot, uint8 crewNo, FString heroUD, FString heroId, int32 level);
		void REQ_DECK_SELECT_READY(uint8 slot, bool ready);
		void REQ_MATCH(EPVP modType, FString modId, bool bCreate, bool bPublic);
		void REQ_MATCH_CANCEL();
		void REQ_MATCH_JOIN(EPVP modType, FString modId, int32 partyNo, FString secret);
		void REQ_PARTY_GO(int32 partyNo);
		void REQ_PARTY_UI_READY(int32 partyNo);
		void REQ_PARTY_UPDATE_PUBLIC(int32 partyNo, bool bPublic);
		void REQ_PING();
		void REQ_PLAY_READY();

	// private:
};
