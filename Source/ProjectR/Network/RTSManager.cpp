#include "ProjectR.h"
#include "RTSManager.h"
// #include "Utils/FlatBuffersHelper.h"

// #include "Global/RGameInstance.h"
// #include "Global/EventManager.h"
// #include "Global/GameObjectManager.h"
#include "UpdateService/Auth/RAuth.h"
#include "PacketDataEnum.h"
#include "RTSReceiveHandler.h"

URTSManager *URTSManager::InstancePtr = nullptr;

/*
#pragma warning(default :  warning_number )
#pragma warning( warning_level  :  warning_number )
C4946
*/

URTSManager::URTSManager()
{
    SocketProcedureThread = nullptr;
    WorkEvent = nullptr;
	SocketState = ESocketConnectionState::SCS_NotConnected;
	SSS = ISocketSubsystem::Get();
	if (SSS == nullptr)
	{
		UE_LOG(LogNetwork, Log, TEXT("Initialize: ISocketSubsystem didn't Get"));
	}
}

URTSManager::~URTSManager()
{
	// delete something
    Disconnect();
}

URTSManager *URTSManager::GetInstancePtr()
{
	if (InstancePtr == NULL)
	{
		InstancePtr = new URTSManager();
	}

	return InstancePtr;
}

void URTSManager::ReleaseSingleton()
{
	if (InstancePtr)
	{
		delete InstancePtr;
		InstancePtr = NULL;
	}
}

void URTSManager::Initialize(URGameInstance* instance)
{
	RGameInstance = instance;
}

void URTSManager::SetConnectionParam(FString &Dn, int32 Port)
{
    /*
	UE_LOG(LogNetwork, Log, TEXT("RTS: SetConnectionParam DNS: %s:%d"), *Dn, Port);
	Addr = SSS->CreateInternetAddr(0, Port);

    ESocketErrors ResolveError = SSS->GetHostByName(TCHAR_TO_ANSI(*Dn), *Addr.Get());
	if (ResolveError != SE_NO_ERROR)
	{
        UE_LOG(LogNetwork, Log, TEXT("RTS: couldn't resolve host name %s: %d"), *Dn, (int32) ResolveError);
        Addr = nullptr;
    } 
    else 
    {
        Addr->SetPort(Port);
        UE_LOG(LogNetwork, Log, TEXT("RTS: SetConnectionParam:resolved %s: %d"), *Addr.Get()->ToString(true));
        bIsReady = Connect();
    }
    */

	FResolveInfo* pResolveInfo = SSS->GetHostByName(TCHAR_TO_ANSI(*Dn));
	while (!pResolveInfo->IsComplete())
	{
		// waiting
	}
	const FInternetAddr* pAddr = &pResolveInfo->GetResolvedAddress();
	UE_LOG(LogNetwork, Log, TEXT("RTS: SetConnectionParam:resolve host name %s"), *pAddr->ToString(true));

	Addr = SSS->CreateInternetAddr();

	uint32 OutIP = 0;
	pAddr->GetIp(OutIP);
	
	Addr->SetIp(OutIP);
	Addr->SetPort(Port);

	UE_LOG(LogNetwork, Log, TEXT("RTS: SetConnectionParam:resolve host name %s"), *Addr.Get()->ToString(true));
	bIsReady = Connect();
}

bool URTSManager::IsConnected() {
	return sessionState >= SessionState::SessionState_Connecting && 
        Socket.IsValid() && 
        Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected;
}

bool URTSManager::IsConnectedMode(EPVP modType){
    if(IsConnected())
    {
        if(modType == EPVP::NONE) 
            return currentMode != modType;
        else 
            return currentMode == modType;
    }
	else 
		return false;
}

EPVP URTSManager::getConnectedMode(){
    return currentMode;
}

void URTSManager::setCurrentFrame(long frame) {
	currentFrame = frame;
	accumulatedTime = FPlatformTime::Seconds();
}

long URTSManager::getCurrentFrame() {
	/*
	UE_LOG(LogNetwork, Log, TEXT("RTS: FrameCheck: %d + %f :: Cycles: %f: %d"),
		currentFrame,
		accumulatedTime,
		FPlatformTime::Seconds() - accumulatedTime,
		currentFrame + (long)((FPlatformTime::Seconds() - accumulatedTime) * 1000)
	);
	*/
	return currentFrame + (long)((FPlatformTime::Seconds() - accumulatedTime) * 1000);
}

bool URTSManager::Connect()
{
	// Close exiting socket
	if (Socket.IsValid())
	{
		if (!Socket->Close())
		{
			UE_LOG(LogNetwork, Log, TEXT("RTS: Closing existing Socket."));
			return false;
		}
	}

	Socket.Reset(SSS->CreateSocket(NAME_Stream, TEXT("RTS_Socket_Connection")));
	if (!Socket.IsValid())
	{
		UE_LOG(LogNetwork, Log, TEXT("RTS: CreateSocket Invalid"));
		return false;
	}

	if (!Socket->SetNonBlocking(true))
	{
		UE_LOG(LogNetwork, Log, TEXT("RTS: SetNonBlocking failed"));
		return false;
	}

	if (!Socket->Connect(*Addr)) {
		UE_LOG(LogNetwork, Warning, TEXT("RTS: Connect RTS Failed. %s"), *Addr.Get()->ToString(true));
        URTSReceiveHandler::ErrorProcess(RTSCE::RTSCE_SYSTEM_FAULT, "1000", RGameInstance);
        return false;
	}

	if (WorkEvent != nullptr) {
        FPlatformProcess::ReturnSynchEventToPool(WorkEvent);
        WorkEvent = nullptr;
    }
    WorkEvent = FPlatformProcess::GetSynchEventFromPool();

    if(SocketProcedureThread == nullptr)
        SocketProcedureThread = FRunnableThread::Create(this, TEXT("FETcpSocketProcedure"), 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());

	sessionState = SessionState::SessionState_Connecting;
	bIsReady = true; //temp

	UE_LOG(LogNetwork, Log, TEXT("RTS: Connecting to RTS"));
	return true;
}

void URTSManager::Disconnect()
{
	//rev16684 - ?§Ïãú ?∞Í≤∞ ?úÎèÑÎ•??òÎ†§Î©?trueÎ°??¨Ïïº ??
	//19.01.05 - Framemove Error
    bIsReady = false;

	if (Socket.IsValid())
	{
		Socket->Close();
		// ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}

    if (nullptr != WorkEvent)
    {
        FPlatformProcess::ReturnSynchEventToPool(WorkEvent);
        WorkEvent = nullptr;
    }

	SocketState = ESocketConnectionState::SCS_NotConnected;
	sessionState = SessionState::SessionState_None;
	lazy = LazyCondition::LazyCondition_Healthy;
	currentMode = EPVP::NONE;

    if (SocketProcedureThread != nullptr)
    {
        SocketProcedureThread->Kill(true);
        delete SocketProcedureThread;
        SocketProcedureThread = nullptr;
    }

	UE_LOG(LogNetwork, Log, TEXT("RTS: ShutDown"));
}

void URTSManager::authorize() {
	if (sessionState == SessionState::SessionState_Authorizing) return;
	sessionState = SessionState::SessionState_Authorizing;

	URAuth* RAuth = URAuth::GetInstancePtr();
	REQ_AUTH(RAuth->GetClientKey(), RAuth->GetDeviceId(), RTS_AUTH_CODE);
}

/**
void URTSManager::sendPing(){ REQ_PING(currentFrame); }

void URTSManager::NormalAttack(uint32 actorUK){
	attackedFrame = currentFrame;
	selectedActor = actorUK;
}

void URTSManager::ChangeNormalAttackState(bool state){
	if( attackNow == state ) return; // ?åÎ¶¥ ?ÑÏöîÍ∞Ä ?ÜÏùå.
	attackNow = state;
	// REQ_BT_ATTACK_STATE_CHANGED(selectedActor, attackNow);
}
*/

/** FRunnable implementation */
bool URTSManager::Init()
{
	return true;
}

uint32 URTSManager::Run()
{
    UE_LOG(LogNetwork, Log, TEXT("RTS: RTS-Socket-Runner Enabled."));
	for (;;)
	{
		if (WorkEvent == nullptr)
			break;

        CheckTimeTemp += FPlatformTime::Seconds() - LastTimeUpdate;
        LastTimeUpdate = FPlatformTime::Seconds();

        if(RTS_THREAD_WAIT_IN_SEC < CheckTimeTemp)
        {
            UE_LOG(LogNetwork, Log, TEXT("RTS: RTS-Socket-Runner Waits. Awake: %g"), CheckTimeTemp);
            WorkEvent->Trigger();
            CheckTimeTemp = 0.f;
        }
        
        if (true == WorkEvent->Wait(FTimespan::FromMilliseconds(RTS_THREAD_WAIT_IN_SEC))) {
            // 10Ï¥àÏóê ?úÎ≤à??
            if(bIsReady==true) REQ_PING();
        }
        ReadSocketData(); 
	}
	return 0;
}

void URTSManager::Stop()
{

}

void URTSManager::Exit()
{

}

void URTSManager::checkSocketStatus()
{
    ESocketConnectionState ConnectionState = Socket.IsValid() ? Socket->GetConnectionState() : ESocketConnectionState::SCS_NotConnected;
    switch(ConnectionState){
        case ESocketConnectionState::SCS_Connected:
            {
                switch(SocketState){
                    case ESocketConnectionState::SCS_NotConnected:
                        // ?∞Í≤∞???ÑÎ£å ?òÏóà?ÑÎïå ÏµúÏ¥à 1??AuthorizeÎ•??§Ìñâ?òÍ∏∞ ?ÑÌï¥, NotConnectedÎ•??ïÏù∏?úÎã§.
                        if (sessionState == SessionState::SessionState_Connecting) {
                            authorize();
                            SocketState = ESocketConnectionState::SCS_Connected;
                        }
                        break;
                    case ESocketConnectionState::SCS_ConnectionError:
                        // ?êÎûò??SCS_ConnectionError?êÏÑú ?°Ì????òÎÇò, FSocket?Ä ?ÅÌÉúÍ∞Ä Î∞îÎÄåÏ? ?äÏïÑ. ?¥Ï™Ω?ºÎ°ú ?§Ïñ¥??
                        // ?∞Í≤∞ - ?∏Ï¶ù - Î©îÏπ≠ÍπåÏ????úÎèÑ ?®Í≥Ñ?¥Î?Î°?Ï∞ΩÏùÑ ?¥Î¶¨?ÑÎ°ù!
                        // Îß§Ïπ≠?ÑÎ£å - ?åÎ†à??Ï§ëÏûÑ. ?åÏºì ?§Î•òÍ∞Ä ?òÎ©¥ Î™®ÎìúÎ•?Ï¢ÖÎ£å?¥Ïïº ??
                        UE_LOG(LogNetwork, Log, TEXT("RTS: SCS_Connected:SCS_ConnectionError - %d"), (int32) SocketState);
                        if(sessionState <= SessionState::SessionState_MatchWaiting)
                            URTSReceiveHandler::NotifyConnectionEvent(RTSState::RTSState_ErrorBeforeTryMatch, RGameInstance);
                        else if(sessionState <= SessionState::SessionState_Playing)
                            URTSReceiveHandler::NotifyConnectionEvent(RTSState::RTSState_ErrorOnPlaying, RGameInstance);
                        Disconnect();
                        break;
                }
            }
            break;

        case ESocketConnectionState::SCS_NotConnected:
            if(sessionState == SessionState::SessionState_None && SocketState == ESocketConnectionState::SCS_NotConnected) 
                Connect();
            break;

        case ESocketConnectionState::SCS_ConnectionError:
            {
                // Connected ?ÑÏóê ???§Î•ò?¥Î?Î°??∞Í≤∞ Î∂àÍ????òÎ?!
                // ?∞Í≤∞ - ?∏Ï¶ù - Î©îÏπ≠ÍπåÏ????úÎèÑ ?®Í≥Ñ?¥Î?Î°?Ï∞ΩÏùÑ ?¥Î¶¨?ÑÎ°ù!
                // Îß§Ïπ≠?ÑÎ£å - ?åÎ†à??Ï§ëÏûÑ. ?åÏºì ?§Î•òÍ∞Ä ?òÎ©¥ Î™®ÎìúÎ•?Ï¢ÖÎ£å?¥Ïïº ??
                switch(sessionState){
                    case SessionState::SessionState_Connecting:
                        // URTSReceiveHandler::ErrorProcess(RTSCE::RTSCE_DISCONNECTED, "3021", RGameInstance);
                        break;
                    case SessionState::SessionState_MatchWaiting:
                        URTSReceiveHandler::NotifyConnectionEvent(RTSState::RTSState_ErrorBeforeTryMatch, RGameInstance);
                        // URTSReceiveHandler::ErrorProcess(RTSCE::RTSCE_DISCONNECTED, "3021", RGameInstance);
                        break;
                    case SessionState::SessionState_Playing:
                        URTSReceiveHandler::NotifyConnectionEvent(RTSState::RTSState_ErrorOnPlaying, RGameInstance);
                        // URTSReceiveHandler::ErrorProcess(RTSCE::RTSCE_DISCONNECTED, "3021", RGameInstance);
                        break;
                }
                Disconnect();
            }
            break;
    }
}

void URTSManager::ACK_CINEMATIC_DONE(uint8 sequence){
    C2S::ACK_CINEMATIC_DONET* table = new C2S::ACK_CINEMATIC_DONET;
    table->sequence = sequence;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_ACK_CINEMATIC_DONE, table);
}

void URTSManager::ACK_STEP_AND_GO(uint8 sequence){
    C2S::ACK_STEP_AND_GOT* table = new C2S::ACK_STEP_AND_GOT;
    table->sequence = sequence;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_ACK_STEP_AND_GO, table);
}

void URTSManager::CHAT_CHANNEL_MOVE(FString channel){
    C2S::CHAT_CHANNEL_MOVET* table = new C2S::CHAT_CHANNEL_MOVET;
    table->channel = TCHAR_TO_UTF8(*channel);
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_CHAT_CHANNEL_MOVE, table);
}

void URTSManager::CHAT_MESSAGE(FString channel, uint8 chattype, int32 kId, FString nick, FString message, FString data){
    C2S::CHAT_MESSAGET* table = new C2S::CHAT_MESSAGET;
    table->channel = TCHAR_TO_UTF8(*channel);
    table->chattype = chattype;
    table->kId = kId;
    table->nick = TCHAR_TO_UTF8(*nick);
    table->message = TCHAR_TO_UTF8(*message);
    table->data = TCHAR_TO_UTF8(*data);
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_CHAT_MESSAGE, table);
}

void URTSManager::CHAT_SUBSCRIBE(FString channel){
    C2S::CHAT_SUBSCRIBET* table = new C2S::CHAT_SUBSCRIBET;
    table->channel = TCHAR_TO_UTF8(*channel);
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_CHAT_SUBSCRIBE, table);
}

void URTSManager::REQ_AUTH(FString clientkey, FString deviceid, uint32 t){
    C2S::REQ_AUTHT* table = new C2S::REQ_AUTHT;
    table->clientkey = TCHAR_TO_UTF8(*clientkey);
    table->deviceid = TCHAR_TO_UTF8(*deviceid);
    table->t = t;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_AUTH, table);
}

void URTSManager::REQ_BT_ACTOR_CREATE(uint8 category, FString actorId, uint8 role, TArray<FString> option){
    C2S::REQ_BT_ACTOR_CREATET* table = new C2S::REQ_BT_ACTOR_CREATET;
    table->category = category;
    table->actorId = TCHAR_TO_UTF8(*actorId);
    table->role = role;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ACTOR_CREATE, table);
}

void URTSManager::REQ_BT_ACTOR_FORMATION(uint16 actorUK){
    C2S::REQ_BT_ACTOR_FORMATIONT* table = new C2S::REQ_BT_ACTOR_FORMATIONT;
    table->actorUK = actorUK;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ACTOR_FORMATION, table);
}

void URTSManager::REQ_BT_ACTOR_MOVE(uint16 actorUK, bool isStop, FVector location, FVector direction){
    C2S::REQ_BT_ACTOR_MOVET* table = new C2S::REQ_BT_ACTOR_MOVET;
    table->frame = getCurrentFrame();
    table->actorUK = actorUK;
    table->isStop = isStop;
    table->location = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->location->X = location.X;
    table->location->Y = location.Y;
    table->location->Z = location.Z;
    table->direction = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->direction->X = direction.X;
    table->direction->Y = direction.Y;
    table->direction->Z = direction.Z;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ACTOR_MOVE, table);
}

void URTSManager::REQ_BT_ACTOR_SCORE(){
    C2S::REQ_BT_ACTOR_SCORET* table = new C2S::REQ_BT_ACTOR_SCORET;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ACTOR_SCORE, table);
}

void URTSManager::REQ_BT_ACTOR_SPAWN(uint16 actorUK, FVector location, FVector direction){
    C2S::REQ_BT_ACTOR_SPAWNT* table = new C2S::REQ_BT_ACTOR_SPAWNT;
    table->actorUK = actorUK;
    table->location = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->location->X = location.X;
    table->location->Y = location.Y;
    table->location->Z = location.Z;
    table->direction = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->direction->X = direction.X;
    table->direction->Y = direction.Y;
    table->direction->Z = direction.Z;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ACTOR_SPAWN, table);
}

void URTSManager::REQ_BT_ACTOR_TELEPORT(uint16 actorUK, uint8 portalNo){
    C2S::REQ_BT_ACTOR_TELEPORTT* table = new C2S::REQ_BT_ACTOR_TELEPORTT;
    table->actorUK = actorUK;
    table->portalNo = portalNo;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ACTOR_TELEPORT, table);
}

void URTSManager::REQ_BT_ATTACK_NOW(uint16 actorUK, FVector actorLocation, uint16 targetUK, bool bAttack, uint8 sequence){
    C2S::REQ_BT_ATTACK_NOWT* table = new C2S::REQ_BT_ATTACK_NOWT;
    table->frame = getCurrentFrame();
    table->actorUK = actorUK;
    table->actorLocation = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->actorLocation->X = actorLocation.X;
    table->actorLocation->Y = actorLocation.Y;
    table->actorLocation->Z = actorLocation.Z;
    table->targetUK = targetUK;
    table->bAttack = bAttack;
    table->sequence = sequence;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ATTACK_NOW, table);
}

void URTSManager::REQ_BT_CHANGE_CREW(uint16 actorUK, uint16 targetUK, FVector location, FVector direction){
    C2S::REQ_BT_CHANGE_CREWT* table = new C2S::REQ_BT_CHANGE_CREWT;
    table->actorUK = actorUK;
    table->targetUK = targetUK;
    table->location = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->location->X = location.X;
    table->location->Y = location.Y;
    table->location->Z = location.Z;
    table->direction = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->direction->X = direction.X;
    table->direction->Y = direction.Y;
    table->direction->Z = direction.Z;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_CHANGE_CREW, table);
}

void URTSManager::REQ_BT_CHANGE_SPIRIT(uint16 actorUK, uint16 spiritIndex){
    C2S::REQ_BT_CHANGE_SPIRITT* table = new C2S::REQ_BT_CHANGE_SPIRITT;
    table->actorUK = actorUK;
    table->spiritIndex = spiritIndex;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_CHANGE_SPIRIT, table);
}

void URTSManager::REQ_BT_DICE(bool participate){
    C2S::REQ_BT_DICET* table = new C2S::REQ_BT_DICET;
    table->participate = participate;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_DICE, table);
}

void URTSManager::REQ_BT_DROP_CREW(uint16 actorUK, FVector location, FVector direction){
    C2S::REQ_BT_DROP_CREWT* table = new C2S::REQ_BT_DROP_CREWT;
    table->actorUK = actorUK;
    table->location = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->location->X = location.X;
    table->location->Y = location.Y;
    table->location->Z = location.Z;
    table->direction = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->direction->X = direction.X;
    table->direction->Y = direction.Y;
    table->direction->Z = direction.Z;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_DROP_CREW, table);
}

void URTSManager::REQ_BT_ENTER(){
    C2S::REQ_BT_ENTERT* table = new C2S::REQ_BT_ENTERT;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_ENTER, table);
}

void URTSManager::REQ_BT_EXIT(){
    C2S::REQ_BT_EXITT* table = new C2S::REQ_BT_EXITT;
    currentMode = EPVP::NONE;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_EXIT, table);
}

void URTSManager::REQ_BT_PLAYER_SCORE(){
    C2S::REQ_BT_PLAYER_SCORET* table = new C2S::REQ_BT_PLAYER_SCORET;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_PLAYER_SCORE, table);
}

void URTSManager::REQ_BT_SKILL_CAST(uint16 actorUK, FVector actorLocation, uint8 slot, uint16 targetUK, FVector location, FVector direction){
    C2S::REQ_BT_SKILL_CASTT* table = new C2S::REQ_BT_SKILL_CASTT;
    table->frame = getCurrentFrame();
    table->actorUK = actorUK;
    table->actorLocation = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->actorLocation->X = actorLocation.X;
    table->actorLocation->Y = actorLocation.Y;
    table->actorLocation->Z = actorLocation.Z;
    table->slot = slot;
    table->targetUK = targetUK;
    table->location = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->location->X = location.X;
    table->location->Y = location.Y;
    table->location->Z = location.Z;
    table->direction = std::make_unique<OVERLORD::RTS::STRUCT::RTS_VECTORT>();
    table->direction->X = direction.X;
    table->direction->Y = direction.Y;
    table->direction->Z = direction.Z;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_BT_SKILL_CAST, table);
}

void URTSManager::REQ_CS_DECK_SELECT(uint8 crewNo, FString heroUD, FString heroId, int32 level){
    C2S::REQ_CS_DECK_SELECTT* table = new C2S::REQ_CS_DECK_SELECTT;
    table->crewNo = crewNo;
    table->heroUD = TCHAR_TO_UTF8(*heroUD);
    table->heroId = TCHAR_TO_UTF8(*heroId);
    table->level = level;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_CS_DECK_SELECT, table);
}

void URTSManager::REQ_CS_DECK_SELECT_READY(bool ready){
    C2S::REQ_CS_DECK_SELECT_READYT* table = new C2S::REQ_CS_DECK_SELECT_READYT;
    table->ready = ready;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_CS_DECK_SELECT_READY, table);
}

void URTSManager::REQ_CS_PARTY_UI_READY(){
    C2S::REQ_CS_PARTY_UI_READYT* table = new C2S::REQ_CS_PARTY_UI_READYT;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_CS_PARTY_UI_READY, table);
}

void URTSManager::REQ_DECK_SELECT(uint8 slot, uint8 crewNo, FString heroUD, FString heroId, int32 level){
    C2S::REQ_DECK_SELECTT* table = new C2S::REQ_DECK_SELECTT;
    table->slot = slot;
    table->crewNo = crewNo;
    table->heroUD = TCHAR_TO_UTF8(*heroUD);
    table->heroId = TCHAR_TO_UTF8(*heroId);
    table->level = level;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_DECK_SELECT, table);
}

void URTSManager::REQ_DECK_SELECT_READY(uint8 slot, bool ready){
    C2S::REQ_DECK_SELECT_READYT* table = new C2S::REQ_DECK_SELECT_READYT;
    table->slot = slot;
    table->ready = ready;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_DECK_SELECT_READY, table);
}

void URTSManager::REQ_MATCH(EPVP modType, FString modId, bool bCreate, bool bPublic){
    C2S::REQ_MATCHT* table = new C2S::REQ_MATCHT;
    currentMode = modType;
    if(IsConnected())
    {
        table->modType = (int32)modType;
        table->modId = TCHAR_TO_UTF8(*modId);
        table->bCreate = bCreate;
        table->bPublic = bPublic;
        SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_MATCH, table);
    }
    else
        URTSReceiveHandler::ErrorProcess(RTSCE::RTSCE_DISCONNECTED, "3021", RGameInstance);
}

void URTSManager::REQ_MATCH_CANCEL(){
    C2S::REQ_MATCH_CANCELT* table = new C2S::REQ_MATCH_CANCELT;
    currentMode = EPVP::NONE;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_MATCH_CANCEL, table);
}

void URTSManager::REQ_MATCH_JOIN(EPVP modType, FString modId, int32 partyNo, FString secret){
    C2S::REQ_MATCH_JOINT* table = new C2S::REQ_MATCH_JOINT;
    currentMode = modType;
    if(IsConnected())
    {
        table->modType = (int32)modType;
        table->modId = TCHAR_TO_UTF8(*modId);
        table->partyNo = partyNo;
        table->secret = TCHAR_TO_UTF8(*secret);
        SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_MATCH_JOIN, table);
    }
    else
        URTSReceiveHandler::ErrorProcess(RTSCE::RTSCE_DISCONNECTED, "3021", RGameInstance);
}

void URTSManager::REQ_PARTY_GO(int32 partyNo){
    C2S::REQ_PARTY_GOT* table = new C2S::REQ_PARTY_GOT;
    table->partyNo = partyNo;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_PARTY_GO, table);
}

void URTSManager::REQ_PARTY_UI_READY(int32 partyNo){
    C2S::REQ_PARTY_UI_READYT* table = new C2S::REQ_PARTY_UI_READYT;
    table->partyNo = partyNo;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_PARTY_UI_READY, table);
}

void URTSManager::REQ_PARTY_UPDATE_PUBLIC(int32 partyNo, bool bPublic){
    C2S::REQ_PARTY_UPDATE_PUBLICT* table = new C2S::REQ_PARTY_UPDATE_PUBLICT;
    table->partyNo = partyNo;
    table->bPublic = bPublic;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_PARTY_UPDATE_PUBLIC, table);
}

void URTSManager::REQ_PING(){
    C2S::REQ_PINGT* table = new C2S::REQ_PINGT;
    table->frame = getCurrentFrame();
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_PING, table);
}

void URTSManager::REQ_PLAY_READY(){
    C2S::REQ_PLAY_READYT* table = new C2S::REQ_PLAY_READYT;
    SendPacket(UNION_RTS_OVERLORD_RTS_C2S_REQ_PLAY_READY, table);
}


void URTSManager::PacketParser(const uint8 *packet) {
	{
		// packet ?¥Ïóê frame or elapsedÎ•?Í∏∞Ï??ºÎ°ú ?òÏã† Î≤ÑÌçºÎ•?Ï±ÑÏõå ?¥Îùº?¥Ïñ∏?∏Ïùò ?êÎ¶Ñ(tick)Í≥??ÑÏû¨ ?úÎ≤Ñ???¥Ïö©??Î¶¨ÏãúÎ≤ÑÎ°ú Î≥¥ÎÇ¥???ëÏóÖ???úÎã§.
		std::shared_ptr<CMD_RTST> CMD = UnPackCMD_RTS(packet);
        ReceiveBuffer.Enqueue(CMD); // PropagatePacket(CMD);
	}
}

void URTSManager::FrameMove(float DeltaTime) {
    std::shared_ptr<CMD_RTST> CMD; 
    while(ReceiveBuffer.Dequeue(CMD))
        PropagatePacket(CMD);
        
    // check socket state
    if(bIsReady==true) checkSocketStatus();
}

void URTSManager::PropagatePacket(std::shared_ptr<CMD_RTST> CMD) {
	{
		switch (CMD->api.type) {
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_AUTH:
                {
                    S2C::ACK_AUTHT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_AUTH();
                    URTSReceiveHandler::OnACK_AUTH(data, RGameInstance);
                    sessionState = SessionState::SessionState_Authorized;
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_CREATE:
                {
                    S2C::ACK_BT_ACTOR_CREATET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_CREATE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ACTOR_CREATE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_DESPAWN:
                {
                    S2C::ACK_BT_ACTOR_DESPAWNT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_DESPAWN();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ACTOR_DESPAWN(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_DISAPPEAR:
                {
                    S2C::ACK_BT_ACTOR_DISAPPEART* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_DISAPPEAR();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ACTOR_DISAPPEAR(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_FORMATION:
                {
                    S2C::ACK_BT_ACTOR_FORMATIONT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_FORMATION();
                    URTSReceiveHandler::OnACK_BT_ACTOR_FORMATION(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_MEDAL_UPDATE:
                {
                    S2C::ACK_BT_ACTOR_MEDAL_UPDATET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_MEDAL_UPDATE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ACTOR_MEDAL_UPDATE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_RESPAWN:
                {
                    S2C::ACK_BT_ACTOR_RESPAWNT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_RESPAWN();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ACTOR_RESPAWN(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_SPAWN:
                {
                    S2C::ACK_BT_ACTOR_SPAWNT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_SPAWN();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ACTOR_SPAWN(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_TELEPORT:
                {
                    S2C::ACK_BT_ACTOR_TELEPORTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_TELEPORT();
                    URTSReceiveHandler::OnACK_BT_ACTOR_TELEPORT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ACTOR_UPDATE:
                {
                    S2C::ACK_BT_ACTOR_UPDATET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ACTOR_UPDATE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ACTOR_UPDATE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ATTACK_NORMAL:
                {
                    S2C::ACK_BT_ATTACK_NORMALT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ATTACK_NORMAL();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ATTACK_NORMAL(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ATTACK_NOW:
                {
                    S2C::ACK_BT_ATTACK_NOWT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ATTACK_NOW();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ATTACK_NOW(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_CHANGE_CREW:
                {
                    S2C::ACK_BT_CHANGE_CREWT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_CHANGE_CREW();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_CHANGE_CREW(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_CHANGE_SPIRIT:
                {
                    S2C::ACK_BT_CHANGE_SPIRITT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_CHANGE_SPIRIT();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_CHANGE_SPIRIT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_DICE_COUNT:
                {
                    S2C::ACK_BT_DICE_COUNTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_DICE_COUNT();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_DICE_COUNT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_DICE_NUM:
                {
                    S2C::ACK_BT_DICE_NUMT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_DICE_NUM();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_DICE_NUM(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_DICE_OPEN:
                {
                    S2C::ACK_BT_DICE_OPENT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_DICE_OPEN();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_DICE_OPEN(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_DICE_RESULT:
                {
                    S2C::ACK_BT_DICE_RESULTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_DICE_RESULT();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_DICE_RESULT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_DOOR_EVENT:
                {
                    S2C::ACK_BT_DOOR_EVENTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_DOOR_EVENT();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_DOOR_EVENT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_DROP_CREW:
                {
                    S2C::ACK_BT_DROP_CREWT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_DROP_CREW();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_DROP_CREW(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_NEW_PLAYER:
                {
                    S2C::ACK_BT_NEW_PLAYERT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_NEW_PLAYER();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_NEW_PLAYER(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SKILL_ACTIVATED:
                {
                    S2C::ACK_BT_SKILL_ACTIVATEDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SKILL_ACTIVATED();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SKILL_ACTIVATED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SKILL_BLOCKED:
                {
                    S2C::ACK_BT_SKILL_BLOCKEDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SKILL_BLOCKED();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SKILL_BLOCKED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SKILL_CASTED:
                {
                    S2C::ACK_BT_SKILL_CASTEDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SKILL_CASTED();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SKILL_CASTED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SKILL_CASTED_INTERVAL:
                {
                    S2C::ACK_BT_SKILL_CASTED_INTERVALT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SKILL_CASTED_INTERVAL();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SKILL_CASTED_INTERVAL(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SYNC_ACTOR_SCORE:
                {
                    S2C::ACK_BT_SYNC_ACTOR_SCORET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SYNC_ACTOR_SCORE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SYNC_ACTOR_SCORE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SYNC_ACTOR_UPDATE:
                {
                    S2C::ACK_BT_SYNC_ACTOR_UPDATET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SYNC_ACTOR_UPDATE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SYNC_ACTOR_UPDATE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SYNC_PLAYER_SCORE:
                {
                    S2C::ACK_BT_SYNC_PLAYER_SCORET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SYNC_PLAYER_SCORE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SYNC_PLAYER_SCORE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SYNC_SKILL_UPDATE:
                {
                    S2C::ACK_BT_SYNC_SKILL_UPDATET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SYNC_SKILL_UPDATE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SYNC_SKILL_UPDATE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_SYNC_TIME_REMAIN:
                {
                    S2C::ACK_BT_SYNC_TIME_REMAINT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_SYNC_TIME_REMAIN();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_SYNC_TIME_REMAIN(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_TEAM_POINT_UPDATE:
                {
                    S2C::ACK_BT_TEAM_POINT_UPDATET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_TEAM_POINT_UPDATE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_TEAM_POINT_UPDATE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ZONE_EVENT:
                {
                    S2C::ACK_BT_ZONE_EVENTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ZONE_EVENT();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ZONE_EVENT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_BT_ZONE_STATE:
                {
                    S2C::ACK_BT_ZONE_STATET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_BT_ZONE_STATE();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_BT_ZONE_STATE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_CINEMATIC_DONE:
                {
                    S2C::ACK_CINEMATIC_DONET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_CINEMATIC_DONE();
                    URTSReceiveHandler::OnACK_CINEMATIC_DONE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_COMMON_ERROR:
                {
                    S2C::ACK_COMMON_ERRORT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_COMMON_ERROR();
                    URTSReceiveHandler::OnACK_COMMON_ERROR(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_COST_UPDATE_AVATAR:
                {
                    S2C::ACK_COST_UPDATE_AVATART* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_COST_UPDATE_AVATAR();
                    URTSReceiveHandler::OnACK_COST_UPDATE_AVATAR(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_CS_DECK_SELECT:
                {
                    S2C::ACK_CS_DECK_SELECTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_CS_DECK_SELECT();
                    URTSReceiveHandler::OnACK_CS_DECK_SELECT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_CS_DECK_SELECT_READY:
                {
                    S2C::ACK_CS_DECK_SELECT_READYT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_CS_DECK_SELECT_READY();
                    URTSReceiveHandler::OnACK_CS_DECK_SELECT_READY(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_CS_DECK_TIME_REMAIN:
                {
                    S2C::ACK_CS_DECK_TIME_REMAINT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_CS_DECK_TIME_REMAIN();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_CS_DECK_TIME_REMAIN(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_CS_MATCHED:
                {
                    S2C::ACK_CS_MATCHEDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_CS_MATCHED();
                    URTSReceiveHandler::OnACK_CS_MATCHED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_DECK_CACHED_DONE:
                {
                    S2C::ACK_DECK_CACHED_DONET* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_DECK_CACHED_DONE();
                    URTSReceiveHandler::OnACK_DECK_CACHED_DONE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_DECK_SCENE_LOADED:
                {
                    S2C::ACK_DECK_SCENE_LOADEDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_DECK_SCENE_LOADED();
                    URTSReceiveHandler::OnACK_DECK_SCENE_LOADED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_DECK_SELECT:
                {
                    S2C::ACK_DECK_SELECTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_DECK_SELECT();
                    URTSReceiveHandler::OnACK_DECK_SELECT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_DECK_SELECT_READY:
                {
                    S2C::ACK_DECK_SELECT_READYT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_DECK_SELECT_READY();
                    URTSReceiveHandler::OnACK_DECK_SELECT_READY(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_DISCONNECTED:
                {
                    S2C::ACK_DISCONNECTEDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_DISCONNECTED();
                    URTSReceiveHandler::OnACK_DISCONNECTED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_MATCHED:
                {
                    S2C::ACK_MATCHEDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_MATCHED();
                    URTSReceiveHandler::OnACK_MATCHED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_MATCH_ROUND:
                {
                    S2C::ACK_MATCH_ROUNDT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_MATCH_ROUND();
                    URTSReceiveHandler::OnACK_MATCH_ROUND(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PARTY_GO:
                {
                    S2C::ACK_PARTY_GOT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PARTY_GO();
                    URTSReceiveHandler::OnACK_PARTY_GO(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PARTY_LEADER_SLOT:
                {
                    S2C::ACK_PARTY_LEADER_SLOTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PARTY_LEADER_SLOT();
                    URTSReceiveHandler::OnACK_PARTY_LEADER_SLOT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PARTY_PLAYER:
                {
                    S2C::ACK_PARTY_PLAYERT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PARTY_PLAYER();
                    URTSReceiveHandler::OnACK_PARTY_PLAYER(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PARTY_PLAYER_EXIT:
                {
                    S2C::ACK_PARTY_PLAYER_EXITT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PARTY_PLAYER_EXIT();
                    URTSReceiveHandler::OnACK_PARTY_PLAYER_EXIT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PARTY_UPDATE_PUBLIC:
                {
                    S2C::ACK_PARTY_UPDATE_PUBLICT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PARTY_UPDATE_PUBLIC();
                    URTSReceiveHandler::OnACK_PARTY_UPDATE_PUBLIC(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PLAY_COUNT:
                {
                    S2C::ACK_PLAY_COUNTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PLAY_COUNT();
                    URTSReceiveHandler::OnACK_PLAY_COUNT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PLAY_READY:
                {
                    S2C::ACK_PLAY_READYT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PLAY_READY();
                    URTSReceiveHandler::OnACK_PLAY_READY(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PLAY_SEND_RESULT:
                {
                    S2C::ACK_PLAY_SEND_RESULTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PLAY_SEND_RESULT();
                    URTSReceiveHandler::OnACK_PLAY_SEND_RESULT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PLAY_START:
                {
                    S2C::ACK_PLAY_STARTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PLAY_START();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_PLAY_START(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_PLAY_TIMEOUT:
                {
                    S2C::ACK_PLAY_TIMEOUTT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_PLAY_TIMEOUT();
                    setCurrentFrame(data->frame);
                    URTSReceiveHandler::OnACK_PLAY_TIMEOUT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_ACK_STEP_GO:
                {
                    S2C::ACK_STEP_GOT* data = CMD->api.AsOVERLORD_RTS_S2C_ACK_STEP_GO();
                    URTSReceiveHandler::OnACK_STEP_GO(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_CHAT_CHANNEL_ENTER:
                {
                    S2C::CHAT_CHANNEL_ENTERT* data = CMD->api.AsOVERLORD_RTS_S2C_CHAT_CHANNEL_ENTER();
                    URTSReceiveHandler::OnCHAT_CHANNEL_ENTER(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_CHAT_CHANNEL_EXIT:
                {
                    S2C::CHAT_CHANNEL_EXITT* data = CMD->api.AsOVERLORD_RTS_S2C_CHAT_CHANNEL_EXIT();
                    URTSReceiveHandler::OnCHAT_CHANNEL_EXIT(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_CHAT_MESSAGE:
                {
                    S2C::CHAT_MESSAGET* data = CMD->api.AsOVERLORD_RTS_S2C_CHAT_MESSAGE();
                    URTSReceiveHandler::OnCHAT_MESSAGE(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_CHAT_SUBSCRIBED:
                {
                    S2C::CHAT_SUBSCRIBEDT* data = CMD->api.AsOVERLORD_RTS_S2C_CHAT_SUBSCRIBED();
                    URTSReceiveHandler::OnCHAT_SUBSCRIBED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_CHAT_UNSUBSCRIBED:
                {
                    S2C::CHAT_UNSUBSCRIBEDT* data = CMD->api.AsOVERLORD_RTS_S2C_CHAT_UNSUBSCRIBED();
                    URTSReceiveHandler::OnCHAT_UNSUBSCRIBED(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_REQ_CINEMATIC_PLAY:
                {
                    S2C::REQ_CINEMATIC_PLAYT* data = CMD->api.AsOVERLORD_RTS_S2C_REQ_CINEMATIC_PLAY();
                    URTSReceiveHandler::OnREQ_CINEMATIC_PLAY(data, RGameInstance);
                }
                break;
            case UNION_RTS_OVERLORD_RTS_S2C_REQ_STEP_READY:
                {
                    S2C::REQ_STEP_READYT* data = CMD->api.AsOVERLORD_RTS_S2C_REQ_STEP_READY();
                    URTSReceiveHandler::OnREQ_STEP_READY(data, RGameInstance);
                }
                break;
		}
	}
}

void URTSManager::NPC_CACHE_LIST(FName StageID, TArray<FName>& actorIds) {
    if(StageID==TEXT("M_AKHAN_EASY")){
    }
    if(StageID==TEXT("M_AKHAN_HARD")){
    }
    if(StageID==TEXT("M_AKHAN_NORMAL")){
    }
    if(StageID==TEXT("M_AKHAN_VERYHARD")){
    }
    if(StageID==TEXT("M_WEEK_TANK_AKHAN_EASY")){
    }
    if(StageID==TEXT("M_WEEK_TANK_AKHAN_HARD")){
    }
    if(StageID==TEXT("M_WEEK_TANK_AKHAN_NORMAL")){
    }
    if(StageID==TEXT("M_WEEK_TANK_AKHAN_VERYHARD")){
    }
    if(StageID==TEXT("M_WEEK_TANK_GIGANT_EASY")){
    }
    if(StageID==TEXT("M_WEEK_TANK_GIGANT_HARD")){
    }
    if(StageID==TEXT("M_WEEK_TANK_GIGANT_NORMAL")){
    }
    if(StageID==TEXT("M_WEEK_TANK_GIGANT_VERYHARD")){
    }
    if(StageID==TEXT("M_WEEK_TANK_MORS_EASY")){
    }
    if(StageID==TEXT("M_WEEK_TANK_MORS_HARD")){
    }
    if(StageID==TEXT("M_WEEK_TANK_MORS_NORMAL")){
    }
    if(StageID==TEXT("M_WEEK_TANK_MORS_VERYHARD")){
    }
    if(StageID==TEXT("S_AKHAN_EASY")){
    }
    if(StageID==TEXT("S_AKHAN_HARD")){
    }
    if(StageID==TEXT("S_AKHAN_NORMAL")){
    }
    if(StageID==TEXT("S_AKHAN_VERYHARD")){
    }
    if(StageID==TEXT("S_FRI_MAGIC_AMON_EASY")){
            actorIds.Emplace(FName(TEXT("30196")));
            actorIds.Emplace(FName(TEXT("30197")));
            actorIds.Emplace(FName(TEXT("30198")));
            actorIds.Emplace(FName(TEXT("30199")));
            actorIds.Emplace(FName(TEXT("30200")));
            actorIds.Emplace(FName(TEXT("30201")));
            actorIds.Emplace(FName(TEXT("30202")));
            actorIds.Emplace(FName(TEXT("30203")));
            actorIds.Emplace(FName(TEXT("30204")));
            actorIds.Emplace(FName(TEXT("30205")));
            actorIds.Emplace(FName(TEXT("30206")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_AMON_HARD")){
            actorIds.Emplace(FName(TEXT("32113")));
            actorIds.Emplace(FName(TEXT("32114")));
            actorIds.Emplace(FName(TEXT("32115")));
            actorIds.Emplace(FName(TEXT("32116")));
            actorIds.Emplace(FName(TEXT("32117")));
            actorIds.Emplace(FName(TEXT("32119")));
            actorIds.Emplace(FName(TEXT("32120")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_AMON_NORMAL")){
            actorIds.Emplace(FName(TEXT("31279")));
            actorIds.Emplace(FName(TEXT("31281")));
            actorIds.Emplace(FName(TEXT("31282")));
            actorIds.Emplace(FName(TEXT("31283")));
            actorIds.Emplace(FName(TEXT("31284")));
            actorIds.Emplace(FName(TEXT("31285")));
            actorIds.Emplace(FName(TEXT("31286")));
            actorIds.Emplace(FName(TEXT("31287")));
            actorIds.Emplace(FName(TEXT("31290")));
            actorIds.Emplace(FName(TEXT("31291")));
            actorIds.Emplace(FName(TEXT("31292")));
            actorIds.Emplace(FName(TEXT("31293")));
            actorIds.Emplace(FName(TEXT("31295")));
            actorIds.Emplace(FName(TEXT("31296")));
            actorIds.Emplace(FName(TEXT("31300")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_AMON_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33155")));
            actorIds.Emplace(FName(TEXT("33156")));
            actorIds.Emplace(FName(TEXT("33157")));
            actorIds.Emplace(FName(TEXT("33158")));
            actorIds.Emplace(FName(TEXT("33159")));
            actorIds.Emplace(FName(TEXT("33160")));
            actorIds.Emplace(FName(TEXT("33161")));
            actorIds.Emplace(FName(TEXT("33163")));
            actorIds.Emplace(FName(TEXT("33165")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_SCARLET_EASY")){
            actorIds.Emplace(FName(TEXT("30159")));
            actorIds.Emplace(FName(TEXT("30164")));
            actorIds.Emplace(FName(TEXT("30168")));
            actorIds.Emplace(FName(TEXT("30169")));
            actorIds.Emplace(FName(TEXT("30170")));
            actorIds.Emplace(FName(TEXT("30171")));
            actorIds.Emplace(FName(TEXT("30172")));
            actorIds.Emplace(FName(TEXT("30174")));
            actorIds.Emplace(FName(TEXT("30175")));
            actorIds.Emplace(FName(TEXT("30176")));
            actorIds.Emplace(FName(TEXT("30177")));
            actorIds.Emplace(FName(TEXT("30178")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_SCARLET_HARD")){
            actorIds.Emplace(FName(TEXT("32097")));
            actorIds.Emplace(FName(TEXT("32098")));
            actorIds.Emplace(FName(TEXT("32099")));
            actorIds.Emplace(FName(TEXT("32100")));
            actorIds.Emplace(FName(TEXT("32101")));
            actorIds.Emplace(FName(TEXT("32103")));
            actorIds.Emplace(FName(TEXT("32104")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_SCARLET_NORMAL")){
            actorIds.Emplace(FName(TEXT("31224")));
            actorIds.Emplace(FName(TEXT("31241")));
            actorIds.Emplace(FName(TEXT("31242")));
            actorIds.Emplace(FName(TEXT("31243")));
            actorIds.Emplace(FName(TEXT("31245")));
            actorIds.Emplace(FName(TEXT("31246")));
            actorIds.Emplace(FName(TEXT("31247")));
            actorIds.Emplace(FName(TEXT("31250")));
            actorIds.Emplace(FName(TEXT("31251")));
            actorIds.Emplace(FName(TEXT("31252")));
            actorIds.Emplace(FName(TEXT("31253")));
            actorIds.Emplace(FName(TEXT("31255")));
            actorIds.Emplace(FName(TEXT("31256")));
            actorIds.Emplace(FName(TEXT("31259")));
            actorIds.Emplace(FName(TEXT("31260")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_SCARLET_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33133")));
            actorIds.Emplace(FName(TEXT("33134")));
            actorIds.Emplace(FName(TEXT("33135")));
            actorIds.Emplace(FName(TEXT("33136")));
            actorIds.Emplace(FName(TEXT("33137")));
            actorIds.Emplace(FName(TEXT("33138")));
            actorIds.Emplace(FName(TEXT("33139")));
            actorIds.Emplace(FName(TEXT("33141")));
            actorIds.Emplace(FName(TEXT("33143")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_YOSEPH_EASY")){
            actorIds.Emplace(FName(TEXT("30182")));
            actorIds.Emplace(FName(TEXT("30183")));
            actorIds.Emplace(FName(TEXT("30184")));
            actorIds.Emplace(FName(TEXT("30185")));
            actorIds.Emplace(FName(TEXT("30186")));
            actorIds.Emplace(FName(TEXT("30187")));
            actorIds.Emplace(FName(TEXT("30188")));
            actorIds.Emplace(FName(TEXT("30189")));
            actorIds.Emplace(FName(TEXT("30190")));
            actorIds.Emplace(FName(TEXT("30191")));
            actorIds.Emplace(FName(TEXT("30192")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_YOSEPH_HARD")){
            actorIds.Emplace(FName(TEXT("32105")));
            actorIds.Emplace(FName(TEXT("32106")));
            actorIds.Emplace(FName(TEXT("32107")));
            actorIds.Emplace(FName(TEXT("32108")));
            actorIds.Emplace(FName(TEXT("32109")));
            actorIds.Emplace(FName(TEXT("32111")));
            actorIds.Emplace(FName(TEXT("32112")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_YOSEPH_NORMAL")){
            actorIds.Emplace(FName(TEXT("31261")));
            actorIds.Emplace(FName(TEXT("31262")));
            actorIds.Emplace(FName(TEXT("31263")));
            actorIds.Emplace(FName(TEXT("31264")));
            actorIds.Emplace(FName(TEXT("31265")));
            actorIds.Emplace(FName(TEXT("31266")));
            actorIds.Emplace(FName(TEXT("31267")));
            actorIds.Emplace(FName(TEXT("31270")));
            actorIds.Emplace(FName(TEXT("31271")));
            actorIds.Emplace(FName(TEXT("31272")));
            actorIds.Emplace(FName(TEXT("31273")));
            actorIds.Emplace(FName(TEXT("31275")));
            actorIds.Emplace(FName(TEXT("31276")));
            actorIds.Emplace(FName(TEXT("31279")));
            actorIds.Emplace(FName(TEXT("31280")));
    }
    if(StageID==TEXT("S_FRI_MAGIC_YOSEPH_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33144")));
            actorIds.Emplace(FName(TEXT("33145")));
            actorIds.Emplace(FName(TEXT("33146")));
            actorIds.Emplace(FName(TEXT("33147")));
            actorIds.Emplace(FName(TEXT("33148")));
            actorIds.Emplace(FName(TEXT("33149")));
            actorIds.Emplace(FName(TEXT("33150")));
            actorIds.Emplace(FName(TEXT("33152")));
            actorIds.Emplace(FName(TEXT("33154")));
    }
    if(StageID==TEXT("S_MON_TANK_BARTOR_EASY")){
            actorIds.Emplace(FName(TEXT("30000")));
            actorIds.Emplace(FName(TEXT("30001")));
            actorIds.Emplace(FName(TEXT("30002")));
            actorIds.Emplace(FName(TEXT("30003")));
            actorIds.Emplace(FName(TEXT("30004")));
            actorIds.Emplace(FName(TEXT("30005")));
            actorIds.Emplace(FName(TEXT("30006")));
            actorIds.Emplace(FName(TEXT("30007")));
            actorIds.Emplace(FName(TEXT("30008")));
            actorIds.Emplace(FName(TEXT("30009")));
            actorIds.Emplace(FName(TEXT("30010")));
    }
    if(StageID==TEXT("S_MON_TANK_BARTOR_HARD")){
            actorIds.Emplace(FName(TEXT("32001")));
            actorIds.Emplace(FName(TEXT("32002")));
            actorIds.Emplace(FName(TEXT("32003")));
            actorIds.Emplace(FName(TEXT("32004")));
            actorIds.Emplace(FName(TEXT("32006")));
            actorIds.Emplace(FName(TEXT("32007")));
            actorIds.Emplace(FName(TEXT("32008")));
    }
    if(StageID==TEXT("S_MON_TANK_BARTOR_NORMAL")){
            actorIds.Emplace(FName(TEXT("10302")));
            actorIds.Emplace(FName(TEXT("10303")));
            actorIds.Emplace(FName(TEXT("10306")));
            actorIds.Emplace(FName(TEXT("31001")));
            actorIds.Emplace(FName(TEXT("31004")));
            actorIds.Emplace(FName(TEXT("31005")));
            actorIds.Emplace(FName(TEXT("31007")));
            actorIds.Emplace(FName(TEXT("31008")));
            actorIds.Emplace(FName(TEXT("31010")));
            actorIds.Emplace(FName(TEXT("31011")));
            actorIds.Emplace(FName(TEXT("31012")));
            actorIds.Emplace(FName(TEXT("31013")));
            actorIds.Emplace(FName(TEXT("31014")));
            actorIds.Emplace(FName(TEXT("31015")));
            actorIds.Emplace(FName(TEXT("31016")));
            actorIds.Emplace(FName(TEXT("31019")));
            actorIds.Emplace(FName(TEXT("31020")));
    }
    if(StageID==TEXT("S_MON_TANK_BARTOR_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33001")));
            actorIds.Emplace(FName(TEXT("33002")));
            actorIds.Emplace(FName(TEXT("33003")));
            actorIds.Emplace(FName(TEXT("33004")));
            actorIds.Emplace(FName(TEXT("33005")));
            actorIds.Emplace(FName(TEXT("33006")));
            actorIds.Emplace(FName(TEXT("33007")));
            actorIds.Emplace(FName(TEXT("33009")));
            actorIds.Emplace(FName(TEXT("33011")));
    }
    if(StageID==TEXT("S_MON_TANK_MURMUR_EASY")){
            actorIds.Emplace(FName(TEXT("30014")));
            actorIds.Emplace(FName(TEXT("30015")));
            actorIds.Emplace(FName(TEXT("30016")));
            actorIds.Emplace(FName(TEXT("30017")));
            actorIds.Emplace(FName(TEXT("30018")));
            actorIds.Emplace(FName(TEXT("30019")));
            actorIds.Emplace(FName(TEXT("30020")));
            actorIds.Emplace(FName(TEXT("30021")));
            actorIds.Emplace(FName(TEXT("30022")));
            actorIds.Emplace(FName(TEXT("30023")));
            actorIds.Emplace(FName(TEXT("30024")));
    }
    if(StageID==TEXT("S_MON_TANK_MURMUR_HARD")){
            actorIds.Emplace(FName(TEXT("32009")));
            actorIds.Emplace(FName(TEXT("32010")));
            actorIds.Emplace(FName(TEXT("32011")));
            actorIds.Emplace(FName(TEXT("32012")));
            actorIds.Emplace(FName(TEXT("32013")));
            actorIds.Emplace(FName(TEXT("32015")));
            actorIds.Emplace(FName(TEXT("32016")));
    }
    if(StageID==TEXT("S_MON_TANK_MURMUR_NORMAL")){
            actorIds.Emplace(FName(TEXT("31021")));
            actorIds.Emplace(FName(TEXT("31022")));
            actorIds.Emplace(FName(TEXT("31023")));
            actorIds.Emplace(FName(TEXT("31024")));
            actorIds.Emplace(FName(TEXT("31025")));
            actorIds.Emplace(FName(TEXT("31026")));
            actorIds.Emplace(FName(TEXT("31027")));
            actorIds.Emplace(FName(TEXT("31028")));
            actorIds.Emplace(FName(TEXT("31030")));
            actorIds.Emplace(FName(TEXT("31031")));
            actorIds.Emplace(FName(TEXT("31032")));
            actorIds.Emplace(FName(TEXT("31033")));
            actorIds.Emplace(FName(TEXT("31034")));
            actorIds.Emplace(FName(TEXT("31035")));
            actorIds.Emplace(FName(TEXT("31036")));
            actorIds.Emplace(FName(TEXT("31039")));
            actorIds.Emplace(FName(TEXT("31040")));
    }
    if(StageID==TEXT("S_MON_TANK_MURMUR_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33012")));
            actorIds.Emplace(FName(TEXT("33013")));
            actorIds.Emplace(FName(TEXT("33014")));
            actorIds.Emplace(FName(TEXT("33015")));
            actorIds.Emplace(FName(TEXT("33016")));
            actorIds.Emplace(FName(TEXT("33017")));
            actorIds.Emplace(FName(TEXT("33018")));
            actorIds.Emplace(FName(TEXT("33020")));
            actorIds.Emplace(FName(TEXT("33022")));
    }
    if(StageID==TEXT("S_MON_TANK_ZAGAN_EASY")){
            actorIds.Emplace(FName(TEXT("30019")));
            actorIds.Emplace(FName(TEXT("30020")));
            actorIds.Emplace(FName(TEXT("30021")));
            actorIds.Emplace(FName(TEXT("30024")));
            actorIds.Emplace(FName(TEXT("30028")));
            actorIds.Emplace(FName(TEXT("30029")));
            actorIds.Emplace(FName(TEXT("30030")));
            actorIds.Emplace(FName(TEXT("30031")));
            actorIds.Emplace(FName(TEXT("30032")));
            actorIds.Emplace(FName(TEXT("30036")));
            actorIds.Emplace(FName(TEXT("30037")));
            actorIds.Emplace(FName(TEXT("30038")));
    }
    if(StageID==TEXT("S_MON_TANK_ZAGAN_HARD")){
            actorIds.Emplace(FName(TEXT("32010")));
            actorIds.Emplace(FName(TEXT("32017")));
            actorIds.Emplace(FName(TEXT("32019")));
            actorIds.Emplace(FName(TEXT("32020")));
            actorIds.Emplace(FName(TEXT("32021")));
            actorIds.Emplace(FName(TEXT("32023")));
            actorIds.Emplace(FName(TEXT("32024")));
    }
    if(StageID==TEXT("S_MON_TANK_ZAGAN_NORMAL")){
            actorIds.Emplace(FName(TEXT("31023")));
            actorIds.Emplace(FName(TEXT("31024")));
            actorIds.Emplace(FName(TEXT("31025")));
            actorIds.Emplace(FName(TEXT("31027")));
            actorIds.Emplace(FName(TEXT("31034")));
            actorIds.Emplace(FName(TEXT("31035")));
            actorIds.Emplace(FName(TEXT("31036")));
            actorIds.Emplace(FName(TEXT("31039")));
            actorIds.Emplace(FName(TEXT("31041")));
            actorIds.Emplace(FName(TEXT("31042")));
            actorIds.Emplace(FName(TEXT("31046")));
            actorIds.Emplace(FName(TEXT("31048")));
            actorIds.Emplace(FName(TEXT("31051")));
            actorIds.Emplace(FName(TEXT("31052")));
            actorIds.Emplace(FName(TEXT("31053")));
            actorIds.Emplace(FName(TEXT("31054")));
            actorIds.Emplace(FName(TEXT("31060")));
    }
    if(StageID==TEXT("S_MON_TANK_ZAGAN_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33023")));
            actorIds.Emplace(FName(TEXT("33024")));
            actorIds.Emplace(FName(TEXT("33025")));
            actorIds.Emplace(FName(TEXT("33026")));
            actorIds.Emplace(FName(TEXT("33027")));
            actorIds.Emplace(FName(TEXT("33028")));
            actorIds.Emplace(FName(TEXT("33029")));
            actorIds.Emplace(FName(TEXT("33031")));
            actorIds.Emplace(FName(TEXT("33033")));
    }
    if(StageID==TEXT("S_SAT_MELEE_JIN_EASY")){
            actorIds.Emplace(FName(TEXT("30224")));
            actorIds.Emplace(FName(TEXT("30225")));
            actorIds.Emplace(FName(TEXT("30226")));
            actorIds.Emplace(FName(TEXT("30227")));
            actorIds.Emplace(FName(TEXT("30228")));
            actorIds.Emplace(FName(TEXT("30229")));
            actorIds.Emplace(FName(TEXT("30230")));
            actorIds.Emplace(FName(TEXT("30231")));
            actorIds.Emplace(FName(TEXT("30232")));
            actorIds.Emplace(FName(TEXT("30233")));
            actorIds.Emplace(FName(TEXT("30234")));
    }
    if(StageID==TEXT("S_SAT_MELEE_JIN_HARD")){
            actorIds.Emplace(FName(TEXT("32122")));
            actorIds.Emplace(FName(TEXT("32123")));
            actorIds.Emplace(FName(TEXT("32127")));
            actorIds.Emplace(FName(TEXT("32128")));
            actorIds.Emplace(FName(TEXT("32129")));
            actorIds.Emplace(FName(TEXT("32132")));
            actorIds.Emplace(FName(TEXT("32133")));
    }
    if(StageID==TEXT("S_SAT_MELEE_JIN_NORMAL")){
            actorIds.Emplace(FName(TEXT("31316")));
            actorIds.Emplace(FName(TEXT("31321")));
            actorIds.Emplace(FName(TEXT("31322")));
            actorIds.Emplace(FName(TEXT("31323")));
            actorIds.Emplace(FName(TEXT("31324")));
            actorIds.Emplace(FName(TEXT("31325")));
            actorIds.Emplace(FName(TEXT("31326")));
            actorIds.Emplace(FName(TEXT("31327")));
            actorIds.Emplace(FName(TEXT("31330")));
            actorIds.Emplace(FName(TEXT("31331")));
            actorIds.Emplace(FName(TEXT("31332")));
            actorIds.Emplace(FName(TEXT("31333")));
            actorIds.Emplace(FName(TEXT("31335")));
            actorIds.Emplace(FName(TEXT("31336")));
            actorIds.Emplace(FName(TEXT("31339")));
            actorIds.Emplace(FName(TEXT("31340")));
    }
    if(StageID==TEXT("S_SAT_MELEE_JIN_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33177")));
            actorIds.Emplace(FName(TEXT("33178")));
            actorIds.Emplace(FName(TEXT("33179")));
            actorIds.Emplace(FName(TEXT("33180")));
            actorIds.Emplace(FName(TEXT("33181")));
            actorIds.Emplace(FName(TEXT("33182")));
            actorIds.Emplace(FName(TEXT("33183")));
            actorIds.Emplace(FName(TEXT("33185")));
            actorIds.Emplace(FName(TEXT("33187")));
    }
    if(StageID==TEXT("S_SAT_RANGE_BUDDY_EASY")){
            actorIds.Emplace(FName(TEXT("30238")));
            actorIds.Emplace(FName(TEXT("30239")));
            actorIds.Emplace(FName(TEXT("30240")));
            actorIds.Emplace(FName(TEXT("30241")));
            actorIds.Emplace(FName(TEXT("30242")));
            actorIds.Emplace(FName(TEXT("30243")));
            actorIds.Emplace(FName(TEXT("30244")));
            actorIds.Emplace(FName(TEXT("30245")));
            actorIds.Emplace(FName(TEXT("30246")));
            actorIds.Emplace(FName(TEXT("30247")));
            actorIds.Emplace(FName(TEXT("30248")));
    }
    if(StageID==TEXT("S_SAT_RANGE_BUDDY_HARD")){
            actorIds.Emplace(FName(TEXT("32137")));
            actorIds.Emplace(FName(TEXT("32138")));
            actorIds.Emplace(FName(TEXT("32139")));
            actorIds.Emplace(FName(TEXT("32140")));
            actorIds.Emplace(FName(TEXT("32141")));
            actorIds.Emplace(FName(TEXT("32143")));
            actorIds.Emplace(FName(TEXT("32144")));
    }
    if(StageID==TEXT("S_SAT_RANGE_BUDDY_NORMAL")){
            actorIds.Emplace(FName(TEXT("31341")));
            actorIds.Emplace(FName(TEXT("31342")));
            actorIds.Emplace(FName(TEXT("31343")));
            actorIds.Emplace(FName(TEXT("31344")));
            actorIds.Emplace(FName(TEXT("31345")));
            actorIds.Emplace(FName(TEXT("31346")));
            actorIds.Emplace(FName(TEXT("31347")));
            actorIds.Emplace(FName(TEXT("31350")));
            actorIds.Emplace(FName(TEXT("31351")));
            actorIds.Emplace(FName(TEXT("31352")));
            actorIds.Emplace(FName(TEXT("31353")));
            actorIds.Emplace(FName(TEXT("31355")));
            actorIds.Emplace(FName(TEXT("31356")));
            actorIds.Emplace(FName(TEXT("31359")));
            actorIds.Emplace(FName(TEXT("31360")));
    }
    if(StageID==TEXT("S_SAT_RANGE_BUDDY_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33188")));
            actorIds.Emplace(FName(TEXT("33189")));
            actorIds.Emplace(FName(TEXT("33190")));
            actorIds.Emplace(FName(TEXT("33191")));
            actorIds.Emplace(FName(TEXT("33192")));
            actorIds.Emplace(FName(TEXT("33193")));
            actorIds.Emplace(FName(TEXT("33194")));
            actorIds.Emplace(FName(TEXT("33196")));
            actorIds.Emplace(FName(TEXT("33198")));
    }
    if(StageID==TEXT("S_SAT_TANK_PANG_EASY")){
            actorIds.Emplace(FName(TEXT("30210")));
            actorIds.Emplace(FName(TEXT("30211")));
            actorIds.Emplace(FName(TEXT("30212")));
            actorIds.Emplace(FName(TEXT("30213")));
            actorIds.Emplace(FName(TEXT("30214")));
            actorIds.Emplace(FName(TEXT("30215")));
            actorIds.Emplace(FName(TEXT("30216")));
            actorIds.Emplace(FName(TEXT("30217")));
            actorIds.Emplace(FName(TEXT("30218")));
            actorIds.Emplace(FName(TEXT("30219")));
            actorIds.Emplace(FName(TEXT("30220")));
    }
    if(StageID==TEXT("S_SAT_TANK_PANG_HARD")){
            actorIds.Emplace(FName(TEXT("32121")));
            actorIds.Emplace(FName(TEXT("32122")));
            actorIds.Emplace(FName(TEXT("32123")));
            actorIds.Emplace(FName(TEXT("32124")));
            actorIds.Emplace(FName(TEXT("32125")));
            actorIds.Emplace(FName(TEXT("32127")));
            actorIds.Emplace(FName(TEXT("32128")));
    }
    if(StageID==TEXT("S_SAT_TANK_PANG_NORMAL")){
            actorIds.Emplace(FName(TEXT("31301")));
            actorIds.Emplace(FName(TEXT("31302")));
            actorIds.Emplace(FName(TEXT("31303")));
            actorIds.Emplace(FName(TEXT("31304")));
            actorIds.Emplace(FName(TEXT("31305")));
            actorIds.Emplace(FName(TEXT("31306")));
            actorIds.Emplace(FName(TEXT("31307")));
            actorIds.Emplace(FName(TEXT("31310")));
            actorIds.Emplace(FName(TEXT("31311")));
            actorIds.Emplace(FName(TEXT("31312")));
            actorIds.Emplace(FName(TEXT("31313")));
            actorIds.Emplace(FName(TEXT("31315")));
            actorIds.Emplace(FName(TEXT("31316")));
            actorIds.Emplace(FName(TEXT("31319")));
            actorIds.Emplace(FName(TEXT("31320")));
    }
    if(StageID==TEXT("S_SAT_TANK_PANG_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33166")));
            actorIds.Emplace(FName(TEXT("33167")));
            actorIds.Emplace(FName(TEXT("33168")));
            actorIds.Emplace(FName(TEXT("33169")));
            actorIds.Emplace(FName(TEXT("33170")));
            actorIds.Emplace(FName(TEXT("33171")));
            actorIds.Emplace(FName(TEXT("33172")));
            actorIds.Emplace(FName(TEXT("33174")));
            actorIds.Emplace(FName(TEXT("33176")));
    }
    if(StageID==TEXT("S_SUN_MAGIC_RAYNA_EASY")){
            actorIds.Emplace(FName(TEXT("30266")));
            actorIds.Emplace(FName(TEXT("30267")));
            actorIds.Emplace(FName(TEXT("30268")));
            actorIds.Emplace(FName(TEXT("30269")));
            actorIds.Emplace(FName(TEXT("30270")));
            actorIds.Emplace(FName(TEXT("30271")));
            actorIds.Emplace(FName(TEXT("30272")));
            actorIds.Emplace(FName(TEXT("30273")));
            actorIds.Emplace(FName(TEXT("30274")));
            actorIds.Emplace(FName(TEXT("30275")));
            actorIds.Emplace(FName(TEXT("30276")));
    }
    if(StageID==TEXT("S_SUN_MAGIC_RAYNA_HARD")){
            actorIds.Emplace(FName(TEXT("32153")));
            actorIds.Emplace(FName(TEXT("32154")));
            actorIds.Emplace(FName(TEXT("32155")));
            actorIds.Emplace(FName(TEXT("32156")));
            actorIds.Emplace(FName(TEXT("32157")));
            actorIds.Emplace(FName(TEXT("32159")));
            actorIds.Emplace(FName(TEXT("32160")));
    }
    if(StageID==TEXT("S_SUN_MAGIC_RAYNA_NORMAL")){
            actorIds.Emplace(FName(TEXT("31370")));
            actorIds.Emplace(FName(TEXT("31381")));
            actorIds.Emplace(FName(TEXT("31382")));
            actorIds.Emplace(FName(TEXT("31383")));
            actorIds.Emplace(FName(TEXT("31384")));
            actorIds.Emplace(FName(TEXT("31385")));
            actorIds.Emplace(FName(TEXT("31386")));
            actorIds.Emplace(FName(TEXT("31387")));
            actorIds.Emplace(FName(TEXT("31388")));
            actorIds.Emplace(FName(TEXT("31390")));
            actorIds.Emplace(FName(TEXT("31391")));
            actorIds.Emplace(FName(TEXT("31392")));
            actorIds.Emplace(FName(TEXT("31393")));
            actorIds.Emplace(FName(TEXT("31395")));
            actorIds.Emplace(FName(TEXT("31396")));
            actorIds.Emplace(FName(TEXT("31399")));
            actorIds.Emplace(FName(TEXT("31400")));
    }
    if(StageID==TEXT("S_SUN_MAGIC_RAYNA_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33210")));
            actorIds.Emplace(FName(TEXT("33211")));
            actorIds.Emplace(FName(TEXT("33212")));
            actorIds.Emplace(FName(TEXT("33213")));
            actorIds.Emplace(FName(TEXT("33214")));
            actorIds.Emplace(FName(TEXT("33215")));
            actorIds.Emplace(FName(TEXT("33216")));
            actorIds.Emplace(FName(TEXT("33218")));
            actorIds.Emplace(FName(TEXT("33220")));
    }
    if(StageID==TEXT("S_SUN_MELEE_LUCY_EASY")){
            actorIds.Emplace(FName(TEXT("30252")));
            actorIds.Emplace(FName(TEXT("30253")));
            actorIds.Emplace(FName(TEXT("30254")));
            actorIds.Emplace(FName(TEXT("30255")));
            actorIds.Emplace(FName(TEXT("30256")));
            actorIds.Emplace(FName(TEXT("30257")));
            actorIds.Emplace(FName(TEXT("30258")));
            actorIds.Emplace(FName(TEXT("30259")));
            actorIds.Emplace(FName(TEXT("30260")));
            actorIds.Emplace(FName(TEXT("30261")));
            actorIds.Emplace(FName(TEXT("30262")));
    }
    if(StageID==TEXT("S_SUN_MELEE_LUCY_HARD")){
            actorIds.Emplace(FName(TEXT("32145")));
            actorIds.Emplace(FName(TEXT("32146")));
            actorIds.Emplace(FName(TEXT("32147")));
            actorIds.Emplace(FName(TEXT("32148")));
            actorIds.Emplace(FName(TEXT("32149")));
            actorIds.Emplace(FName(TEXT("32151")));
            actorIds.Emplace(FName(TEXT("32152")));
    }
    if(StageID==TEXT("S_SUN_MELEE_LUCY_NORMAL")){
            actorIds.Emplace(FName(TEXT("31361")));
            actorIds.Emplace(FName(TEXT("31362")));
            actorIds.Emplace(FName(TEXT("31363")));
            actorIds.Emplace(FName(TEXT("31364")));
            actorIds.Emplace(FName(TEXT("31365")));
            actorIds.Emplace(FName(TEXT("31366")));
            actorIds.Emplace(FName(TEXT("31367")));
            actorIds.Emplace(FName(TEXT("31370")));
            actorIds.Emplace(FName(TEXT("31371")));
            actorIds.Emplace(FName(TEXT("31372")));
            actorIds.Emplace(FName(TEXT("31373")));
            actorIds.Emplace(FName(TEXT("31375")));
            actorIds.Emplace(FName(TEXT("31376")));
            actorIds.Emplace(FName(TEXT("31379")));
            actorIds.Emplace(FName(TEXT("31380")));
    }
    if(StageID==TEXT("S_SUN_MELEE_LUCY_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33199")));
            actorIds.Emplace(FName(TEXT("33200")));
            actorIds.Emplace(FName(TEXT("33201")));
            actorIds.Emplace(FName(TEXT("33202")));
            actorIds.Emplace(FName(TEXT("33203")));
            actorIds.Emplace(FName(TEXT("33204")));
            actorIds.Emplace(FName(TEXT("33205")));
            actorIds.Emplace(FName(TEXT("33207")));
            actorIds.Emplace(FName(TEXT("33209")));
    }
    if(StageID==TEXT("S_SUN_SUPPORT_SOPHIA_EASY")){
            actorIds.Emplace(FName(TEXT("30280")));
            actorIds.Emplace(FName(TEXT("30281")));
            actorIds.Emplace(FName(TEXT("30282")));
            actorIds.Emplace(FName(TEXT("30283")));
            actorIds.Emplace(FName(TEXT("30284")));
            actorIds.Emplace(FName(TEXT("30285")));
            actorIds.Emplace(FName(TEXT("30286")));
            actorIds.Emplace(FName(TEXT("30287")));
            actorIds.Emplace(FName(TEXT("30288")));
            actorIds.Emplace(FName(TEXT("30289")));
            actorIds.Emplace(FName(TEXT("30290")));
    }
    if(StageID==TEXT("S_SUN_SUPPORT_SOPHIA_HARD")){
            actorIds.Emplace(FName(TEXT("32161")));
            actorIds.Emplace(FName(TEXT("32162")));
            actorIds.Emplace(FName(TEXT("32163")));
            actorIds.Emplace(FName(TEXT("32164")));
            actorIds.Emplace(FName(TEXT("32165")));
            actorIds.Emplace(FName(TEXT("32167")));
            actorIds.Emplace(FName(TEXT("32168")));
    }
    if(StageID==TEXT("S_SUN_SUPPORT_SOPHIA_NORMAL")){
            actorIds.Emplace(FName(TEXT("31387")));
            actorIds.Emplace(FName(TEXT("31401")));
            actorIds.Emplace(FName(TEXT("31402")));
            actorIds.Emplace(FName(TEXT("31403")));
            actorIds.Emplace(FName(TEXT("31404")));
            actorIds.Emplace(FName(TEXT("31405")));
            actorIds.Emplace(FName(TEXT("31406")));
            actorIds.Emplace(FName(TEXT("31408")));
            actorIds.Emplace(FName(TEXT("31410")));
            actorIds.Emplace(FName(TEXT("31411")));
            actorIds.Emplace(FName(TEXT("31412")));
            actorIds.Emplace(FName(TEXT("31413")));
            actorIds.Emplace(FName(TEXT("31415")));
            actorIds.Emplace(FName(TEXT("31416")));
            actorIds.Emplace(FName(TEXT("31419")));
            actorIds.Emplace(FName(TEXT("31420")));
    }
    if(StageID==TEXT("S_SUN_SUPPORT_SOPHIA_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33221")));
            actorIds.Emplace(FName(TEXT("33222")));
            actorIds.Emplace(FName(TEXT("33223")));
            actorIds.Emplace(FName(TEXT("33224")));
            actorIds.Emplace(FName(TEXT("33225")));
            actorIds.Emplace(FName(TEXT("33226")));
            actorIds.Emplace(FName(TEXT("33227")));
            actorIds.Emplace(FName(TEXT("33229")));
            actorIds.Emplace(FName(TEXT("33231")));
    }
    if(StageID==TEXT("S_THU_MELEE_BALT_EASY")){
            actorIds.Emplace(FName(TEXT("30140")));
            actorIds.Emplace(FName(TEXT("30141")));
            actorIds.Emplace(FName(TEXT("30142")));
            actorIds.Emplace(FName(TEXT("30143")));
            actorIds.Emplace(FName(TEXT("30144")));
            actorIds.Emplace(FName(TEXT("30145")));
            actorIds.Emplace(FName(TEXT("30146")));
            actorIds.Emplace(FName(TEXT("30147")));
            actorIds.Emplace(FName(TEXT("30148")));
            actorIds.Emplace(FName(TEXT("30149")));
            actorIds.Emplace(FName(TEXT("30150")));
    }
    if(StageID==TEXT("S_THU_MELEE_BALT_HARD")){
            actorIds.Emplace(FName(TEXT("32081")));
            actorIds.Emplace(FName(TEXT("32082")));
            actorIds.Emplace(FName(TEXT("32083")));
            actorIds.Emplace(FName(TEXT("32084")));
            actorIds.Emplace(FName(TEXT("32085")));
            actorIds.Emplace(FName(TEXT("32087")));
            actorIds.Emplace(FName(TEXT("32088")));
    }
    if(StageID==TEXT("S_THU_MELEE_BALT_NORMAL")){
            actorIds.Emplace(FName(TEXT("31201")));
            actorIds.Emplace(FName(TEXT("31202")));
            actorIds.Emplace(FName(TEXT("31203")));
            actorIds.Emplace(FName(TEXT("31204")));
            actorIds.Emplace(FName(TEXT("31205")));
            actorIds.Emplace(FName(TEXT("31206")));
            actorIds.Emplace(FName(TEXT("31207")));
            actorIds.Emplace(FName(TEXT("31210")));
            actorIds.Emplace(FName(TEXT("31211")));
            actorIds.Emplace(FName(TEXT("31212")));
            actorIds.Emplace(FName(TEXT("31213")));
            actorIds.Emplace(FName(TEXT("31215")));
            actorIds.Emplace(FName(TEXT("31216")));
            actorIds.Emplace(FName(TEXT("31219")));
            actorIds.Emplace(FName(TEXT("31220")));
    }
    if(StageID==TEXT("S_THU_MELEE_BALT_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33111")));
            actorIds.Emplace(FName(TEXT("33112")));
            actorIds.Emplace(FName(TEXT("33113")));
            actorIds.Emplace(FName(TEXT("33114")));
            actorIds.Emplace(FName(TEXT("33115")));
            actorIds.Emplace(FName(TEXT("33116")));
            actorIds.Emplace(FName(TEXT("33117")));
            actorIds.Emplace(FName(TEXT("33119")));
            actorIds.Emplace(FName(TEXT("33121")));
    }
    if(StageID==TEXT("S_THU_MELEE_FLAME_EASY")){
            actorIds.Emplace(FName(TEXT("30126")));
            actorIds.Emplace(FName(TEXT("30127")));
            actorIds.Emplace(FName(TEXT("30128")));
            actorIds.Emplace(FName(TEXT("30129")));
            actorIds.Emplace(FName(TEXT("30130")));
            actorIds.Emplace(FName(TEXT("30131")));
            actorIds.Emplace(FName(TEXT("30132")));
            actorIds.Emplace(FName(TEXT("30133")));
            actorIds.Emplace(FName(TEXT("30134")));
            actorIds.Emplace(FName(TEXT("30135")));
            actorIds.Emplace(FName(TEXT("30136")));
    }
    if(StageID==TEXT("S_THU_MELEE_FLAME_HARD")){
            actorIds.Emplace(FName(TEXT("32073")));
            actorIds.Emplace(FName(TEXT("32074")));
            actorIds.Emplace(FName(TEXT("32075")));
            actorIds.Emplace(FName(TEXT("32076")));
            actorIds.Emplace(FName(TEXT("32077")));
            actorIds.Emplace(FName(TEXT("32079")));
            actorIds.Emplace(FName(TEXT("32080")));
    }
    if(StageID==TEXT("S_THU_MELEE_FLAME_NORMAL")){
            actorIds.Emplace(FName(TEXT("31181")));
            actorIds.Emplace(FName(TEXT("31182")));
            actorIds.Emplace(FName(TEXT("31183")));
            actorIds.Emplace(FName(TEXT("31184")));
            actorIds.Emplace(FName(TEXT("31185")));
            actorIds.Emplace(FName(TEXT("31186")));
            actorIds.Emplace(FName(TEXT("31187")));
            actorIds.Emplace(FName(TEXT("31190")));
            actorIds.Emplace(FName(TEXT("31191")));
            actorIds.Emplace(FName(TEXT("31192")));
            actorIds.Emplace(FName(TEXT("31193")));
            actorIds.Emplace(FName(TEXT("31195")));
            actorIds.Emplace(FName(TEXT("31196")));
            actorIds.Emplace(FName(TEXT("31199")));
            actorIds.Emplace(FName(TEXT("31200")));
    }
    if(StageID==TEXT("S_THU_MELEE_FLAME_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33100")));
            actorIds.Emplace(FName(TEXT("33101")));
            actorIds.Emplace(FName(TEXT("33102")));
            actorIds.Emplace(FName(TEXT("33103")));
            actorIds.Emplace(FName(TEXT("33104")));
            actorIds.Emplace(FName(TEXT("33105")));
            actorIds.Emplace(FName(TEXT("33106")));
            actorIds.Emplace(FName(TEXT("33108")));
            actorIds.Emplace(FName(TEXT("33110")));
    }
    if(StageID==TEXT("S_THU_MELEE_THAN_EASY")){
            actorIds.Emplace(FName(TEXT("30154")));
            actorIds.Emplace(FName(TEXT("30155")));
            actorIds.Emplace(FName(TEXT("30156")));
            actorIds.Emplace(FName(TEXT("30157")));
            actorIds.Emplace(FName(TEXT("30158")));
            actorIds.Emplace(FName(TEXT("30159")));
            actorIds.Emplace(FName(TEXT("30160")));
            actorIds.Emplace(FName(TEXT("30161")));
            actorIds.Emplace(FName(TEXT("30162")));
            actorIds.Emplace(FName(TEXT("30163")));
            actorIds.Emplace(FName(TEXT("30164")));
    }
    if(StageID==TEXT("S_THU_MELEE_THAN_HARD")){
            actorIds.Emplace(FName(TEXT("32089")));
            actorIds.Emplace(FName(TEXT("32090")));
            actorIds.Emplace(FName(TEXT("32091")));
            actorIds.Emplace(FName(TEXT("32092")));
            actorIds.Emplace(FName(TEXT("32093")));
            actorIds.Emplace(FName(TEXT("32095")));
            actorIds.Emplace(FName(TEXT("32096")));
    }
    if(StageID==TEXT("S_THU_MELEE_THAN_NORMAL")){
            actorIds.Emplace(FName(TEXT("31215")));
            actorIds.Emplace(FName(TEXT("31221")));
            actorIds.Emplace(FName(TEXT("31222")));
            actorIds.Emplace(FName(TEXT("31223")));
            actorIds.Emplace(FName(TEXT("31224")));
            actorIds.Emplace(FName(TEXT("31225")));
            actorIds.Emplace(FName(TEXT("31226")));
            actorIds.Emplace(FName(TEXT("31227")));
            actorIds.Emplace(FName(TEXT("31230")));
            actorIds.Emplace(FName(TEXT("31231")));
            actorIds.Emplace(FName(TEXT("31232")));
            actorIds.Emplace(FName(TEXT("31233")));
            actorIds.Emplace(FName(TEXT("31236")));
            actorIds.Emplace(FName(TEXT("31239")));
            actorIds.Emplace(FName(TEXT("31240")));
    }
    if(StageID==TEXT("S_THU_MELEE_THAN_VERYHARD")){
            actorIds.Emplace(FName(TEXT("10502")));
            actorIds.Emplace(FName(TEXT("10503")));
            actorIds.Emplace(FName(TEXT("10505")));
            actorIds.Emplace(FName(TEXT("33122")));
            actorIds.Emplace(FName(TEXT("33125")));
            actorIds.Emplace(FName(TEXT("33127")));
            actorIds.Emplace(FName(TEXT("33128")));
            actorIds.Emplace(FName(TEXT("33130")));
            actorIds.Emplace(FName(TEXT("33132")));
    }
    if(StageID==TEXT("S_TUE_RANGE_BANTONIO_EASY")){
            actorIds.Emplace(FName(TEXT("30056")));
            actorIds.Emplace(FName(TEXT("30057")));
            actorIds.Emplace(FName(TEXT("30058")));
            actorIds.Emplace(FName(TEXT("30059")));
            actorIds.Emplace(FName(TEXT("30060")));
            actorIds.Emplace(FName(TEXT("30061")));
            actorIds.Emplace(FName(TEXT("30062")));
            actorIds.Emplace(FName(TEXT("30063")));
            actorIds.Emplace(FName(TEXT("30064")));
            actorIds.Emplace(FName(TEXT("30065")));
            actorIds.Emplace(FName(TEXT("30066")));
    }
    if(StageID==TEXT("S_TUE_RANGE_BANTONIO_HARD")){
            actorIds.Emplace(FName(TEXT("32033")));
            actorIds.Emplace(FName(TEXT("32034")));
            actorIds.Emplace(FName(TEXT("32035")));
            actorIds.Emplace(FName(TEXT("32036")));
            actorIds.Emplace(FName(TEXT("32037")));
            actorIds.Emplace(FName(TEXT("32039")));
            actorIds.Emplace(FName(TEXT("32040")));
    }
    if(StageID==TEXT("S_TUE_RANGE_BANTONIO_NORMAL")){
            actorIds.Emplace(FName(TEXT("31054")));
            actorIds.Emplace(FName(TEXT("31081")));
            actorIds.Emplace(FName(TEXT("31082")));
            actorIds.Emplace(FName(TEXT("31083")));
            actorIds.Emplace(FName(TEXT("31084")));
            actorIds.Emplace(FName(TEXT("31085")));
            actorIds.Emplace(FName(TEXT("31086")));
            actorIds.Emplace(FName(TEXT("31087")));
            actorIds.Emplace(FName(TEXT("31088")));
            actorIds.Emplace(FName(TEXT("31091")));
            actorIds.Emplace(FName(TEXT("31092")));
            actorIds.Emplace(FName(TEXT("31093")));
            actorIds.Emplace(FName(TEXT("31094")));
            actorIds.Emplace(FName(TEXT("31095")));
            actorIds.Emplace(FName(TEXT("31096")));
            actorIds.Emplace(FName(TEXT("31099")));
            actorIds.Emplace(FName(TEXT("31100")));
    }
    if(StageID==TEXT("S_TUE_RANGE_BANTONIO_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33045")));
            actorIds.Emplace(FName(TEXT("33046")));
            actorIds.Emplace(FName(TEXT("33047")));
            actorIds.Emplace(FName(TEXT("33048")));
            actorIds.Emplace(FName(TEXT("33049")));
            actorIds.Emplace(FName(TEXT("33050")));
            actorIds.Emplace(FName(TEXT("33051")));
            actorIds.Emplace(FName(TEXT("33053")));
            actorIds.Emplace(FName(TEXT("33055")));
    }
    if(StageID==TEXT("S_TUE_RANGE_SIRIA_EASY")){
            actorIds.Emplace(FName(TEXT("30042")));
            actorIds.Emplace(FName(TEXT("30043")));
            actorIds.Emplace(FName(TEXT("30044")));
            actorIds.Emplace(FName(TEXT("30045")));
            actorIds.Emplace(FName(TEXT("30046")));
            actorIds.Emplace(FName(TEXT("30047")));
            actorIds.Emplace(FName(TEXT("30048")));
            actorIds.Emplace(FName(TEXT("30049")));
            actorIds.Emplace(FName(TEXT("30050")));
            actorIds.Emplace(FName(TEXT("30051")));
            actorIds.Emplace(FName(TEXT("30052")));
    }
    if(StageID==TEXT("S_TUE_RANGE_SIRIA_HARD")){
            actorIds.Emplace(FName(TEXT("32025")));
            actorIds.Emplace(FName(TEXT("32026")));
            actorIds.Emplace(FName(TEXT("32027")));
            actorIds.Emplace(FName(TEXT("32028")));
            actorIds.Emplace(FName(TEXT("32029")));
            actorIds.Emplace(FName(TEXT("32031")));
            actorIds.Emplace(FName(TEXT("32032")));
    }
    if(StageID==TEXT("S_TUE_RANGE_SIRIA_NORMAL")){
            actorIds.Emplace(FName(TEXT("31061")));
            actorIds.Emplace(FName(TEXT("31062")));
            actorIds.Emplace(FName(TEXT("31063")));
            actorIds.Emplace(FName(TEXT("31064")));
            actorIds.Emplace(FName(TEXT("31065")));
            actorIds.Emplace(FName(TEXT("31066")));
            actorIds.Emplace(FName(TEXT("31067")));
            actorIds.Emplace(FName(TEXT("31071")));
            actorIds.Emplace(FName(TEXT("31072")));
            actorIds.Emplace(FName(TEXT("31073")));
            actorIds.Emplace(FName(TEXT("31074")));
            actorIds.Emplace(FName(TEXT("31075")));
            actorIds.Emplace(FName(TEXT("31076")));
            actorIds.Emplace(FName(TEXT("31079")));
            actorIds.Emplace(FName(TEXT("31080")));
    }
    if(StageID==TEXT("S_TUE_RANGE_SIRIA_VERYHARD")){
            actorIds.Emplace(FName(TEXT("10504")));
            actorIds.Emplace(FName(TEXT("33034")));
            actorIds.Emplace(FName(TEXT("33035")));
            actorIds.Emplace(FName(TEXT("33036")));
            actorIds.Emplace(FName(TEXT("33038")));
            actorIds.Emplace(FName(TEXT("33039")));
            actorIds.Emplace(FName(TEXT("33040")));
            actorIds.Emplace(FName(TEXT("33042")));
            actorIds.Emplace(FName(TEXT("33044")));
    }
    if(StageID==TEXT("S_TUE_RANGE_TIERA_EASY")){
            actorIds.Emplace(FName(TEXT("30070")));
            actorIds.Emplace(FName(TEXT("30071")));
            actorIds.Emplace(FName(TEXT("30072")));
            actorIds.Emplace(FName(TEXT("30073")));
            actorIds.Emplace(FName(TEXT("30074")));
            actorIds.Emplace(FName(TEXT("30075")));
            actorIds.Emplace(FName(TEXT("30076")));
            actorIds.Emplace(FName(TEXT("30077")));
            actorIds.Emplace(FName(TEXT("30078")));
            actorIds.Emplace(FName(TEXT("30079")));
            actorIds.Emplace(FName(TEXT("30080")));
    }
    if(StageID==TEXT("S_TUE_RANGE_TIERA_HARD")){
            actorIds.Emplace(FName(TEXT("32041")));
            actorIds.Emplace(FName(TEXT("32042")));
            actorIds.Emplace(FName(TEXT("32043")));
            actorIds.Emplace(FName(TEXT("32044")));
            actorIds.Emplace(FName(TEXT("32045")));
            actorIds.Emplace(FName(TEXT("32047")));
            actorIds.Emplace(FName(TEXT("32048")));
    }
    if(StageID==TEXT("S_TUE_RANGE_TIERA_NORMAL")){
            actorIds.Emplace(FName(TEXT("31066")));
            actorIds.Emplace(FName(TEXT("31101")));
            actorIds.Emplace(FName(TEXT("31102")));
            actorIds.Emplace(FName(TEXT("31103")));
            actorIds.Emplace(FName(TEXT("31104")));
            actorIds.Emplace(FName(TEXT("31105")));
            actorIds.Emplace(FName(TEXT("31107")));
            actorIds.Emplace(FName(TEXT("31110")));
            actorIds.Emplace(FName(TEXT("31111")));
            actorIds.Emplace(FName(TEXT("31112")));
            actorIds.Emplace(FName(TEXT("31113")));
            actorIds.Emplace(FName(TEXT("31115")));
            actorIds.Emplace(FName(TEXT("31116")));
            actorIds.Emplace(FName(TEXT("31119")));
            actorIds.Emplace(FName(TEXT("31120")));
    }
    if(StageID==TEXT("S_TUE_RANGE_TIERA_VERYHARD")){
            actorIds.Emplace(FName(TEXT("10502")));
            actorIds.Emplace(FName(TEXT("10505")));
            actorIds.Emplace(FName(TEXT("33056")));
            actorIds.Emplace(FName(TEXT("33058")));
            actorIds.Emplace(FName(TEXT("33059")));
            actorIds.Emplace(FName(TEXT("33061")));
            actorIds.Emplace(FName(TEXT("33062")));
            actorIds.Emplace(FName(TEXT("33064")));
            actorIds.Emplace(FName(TEXT("33066")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_IOLA_EASY")){
            actorIds.Emplace(FName(TEXT("30084")));
            actorIds.Emplace(FName(TEXT("30085")));
            actorIds.Emplace(FName(TEXT("30086")));
            actorIds.Emplace(FName(TEXT("30087")));
            actorIds.Emplace(FName(TEXT("30088")));
            actorIds.Emplace(FName(TEXT("30089")));
            actorIds.Emplace(FName(TEXT("30090")));
            actorIds.Emplace(FName(TEXT("30091")));
            actorIds.Emplace(FName(TEXT("30092")));
            actorIds.Emplace(FName(TEXT("30093")));
            actorIds.Emplace(FName(TEXT("30094")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_IOLA_HARD")){
            actorIds.Emplace(FName(TEXT("32049")));
            actorIds.Emplace(FName(TEXT("32050")));
            actorIds.Emplace(FName(TEXT("32051")));
            actorIds.Emplace(FName(TEXT("32052")));
            actorIds.Emplace(FName(TEXT("32053")));
            actorIds.Emplace(FName(TEXT("32055")));
            actorIds.Emplace(FName(TEXT("32056")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_IOLA_NORMAL")){
            actorIds.Emplace(FName(TEXT("31121")));
            actorIds.Emplace(FName(TEXT("31122")));
            actorIds.Emplace(FName(TEXT("31123")));
            actorIds.Emplace(FName(TEXT("31124")));
            actorIds.Emplace(FName(TEXT("31125")));
            actorIds.Emplace(FName(TEXT("31126")));
            actorIds.Emplace(FName(TEXT("31127")));
            actorIds.Emplace(FName(TEXT("31130")));
            actorIds.Emplace(FName(TEXT("31131")));
            actorIds.Emplace(FName(TEXT("31132")));
            actorIds.Emplace(FName(TEXT("31133")));
            actorIds.Emplace(FName(TEXT("31135")));
            actorIds.Emplace(FName(TEXT("31136")));
            actorIds.Emplace(FName(TEXT("31139")));
            actorIds.Emplace(FName(TEXT("31140")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_IOLA_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33067")));
            actorIds.Emplace(FName(TEXT("33068")));
            actorIds.Emplace(FName(TEXT("33069")));
            actorIds.Emplace(FName(TEXT("33070")));
            actorIds.Emplace(FName(TEXT("33071")));
            actorIds.Emplace(FName(TEXT("33072")));
            actorIds.Emplace(FName(TEXT("33073")));
            actorIds.Emplace(FName(TEXT("33075")));
            actorIds.Emplace(FName(TEXT("33077")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_MOMENT_EASY")){
            actorIds.Emplace(FName(TEXT("30098")));
            actorIds.Emplace(FName(TEXT("30099")));
            actorIds.Emplace(FName(TEXT("30100")));
            actorIds.Emplace(FName(TEXT("30101")));
            actorIds.Emplace(FName(TEXT("30102")));
            actorIds.Emplace(FName(TEXT("30103")));
            actorIds.Emplace(FName(TEXT("30104")));
            actorIds.Emplace(FName(TEXT("30105")));
            actorIds.Emplace(FName(TEXT("30106")));
            actorIds.Emplace(FName(TEXT("30107")));
            actorIds.Emplace(FName(TEXT("30108")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_MOMENT_HARD")){
            actorIds.Emplace(FName(TEXT("32057")));
            actorIds.Emplace(FName(TEXT("32058")));
            actorIds.Emplace(FName(TEXT("32059")));
            actorIds.Emplace(FName(TEXT("32060")));
            actorIds.Emplace(FName(TEXT("32061")));
            actorIds.Emplace(FName(TEXT("32063")));
            actorIds.Emplace(FName(TEXT("32064")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_MOMENT_NORMAL")){
            actorIds.Emplace(FName(TEXT("31122")));
            actorIds.Emplace(FName(TEXT("31124")));
            actorIds.Emplace(FName(TEXT("31139")));
            actorIds.Emplace(FName(TEXT("31141")));
            actorIds.Emplace(FName(TEXT("31142")));
            actorIds.Emplace(FName(TEXT("31143")));
            actorIds.Emplace(FName(TEXT("31145")));
            actorIds.Emplace(FName(TEXT("31146")));
            actorIds.Emplace(FName(TEXT("31147")));
            actorIds.Emplace(FName(TEXT("31150")));
            actorIds.Emplace(FName(TEXT("31151")));
            actorIds.Emplace(FName(TEXT("31152")));
            actorIds.Emplace(FName(TEXT("31153")));
            actorIds.Emplace(FName(TEXT("31155")));
            actorIds.Emplace(FName(TEXT("31156")));
            actorIds.Emplace(FName(TEXT("31160")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_MOMENT_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33078")));
            actorIds.Emplace(FName(TEXT("33079")));
            actorIds.Emplace(FName(TEXT("33080")));
            actorIds.Emplace(FName(TEXT("33081")));
            actorIds.Emplace(FName(TEXT("33082")));
            actorIds.Emplace(FName(TEXT("33083")));
            actorIds.Emplace(FName(TEXT("33084")));
            actorIds.Emplace(FName(TEXT("33086")));
            actorIds.Emplace(FName(TEXT("33088")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_RHEA_EASY")){
            actorIds.Emplace(FName(TEXT("30112")));
            actorIds.Emplace(FName(TEXT("30113")));
            actorIds.Emplace(FName(TEXT("30114")));
            actorIds.Emplace(FName(TEXT("30115")));
            actorIds.Emplace(FName(TEXT("30116")));
            actorIds.Emplace(FName(TEXT("30117")));
            actorIds.Emplace(FName(TEXT("30118")));
            actorIds.Emplace(FName(TEXT("30119")));
            actorIds.Emplace(FName(TEXT("30120")));
            actorIds.Emplace(FName(TEXT("30121")));
            actorIds.Emplace(FName(TEXT("30122")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_RHEA_HARD")){
            actorIds.Emplace(FName(TEXT("32065")));
            actorIds.Emplace(FName(TEXT("32066")));
            actorIds.Emplace(FName(TEXT("32067")));
            actorIds.Emplace(FName(TEXT("32068")));
            actorIds.Emplace(FName(TEXT("32069")));
            actorIds.Emplace(FName(TEXT("32071")));
            actorIds.Emplace(FName(TEXT("32072")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_RHEA_NORMAL")){
            actorIds.Emplace(FName(TEXT("31161")));
            actorIds.Emplace(FName(TEXT("31162")));
            actorIds.Emplace(FName(TEXT("31163")));
            actorIds.Emplace(FName(TEXT("31164")));
            actorIds.Emplace(FName(TEXT("31165")));
            actorIds.Emplace(FName(TEXT("31166")));
            actorIds.Emplace(FName(TEXT("31167")));
            actorIds.Emplace(FName(TEXT("31170")));
            actorIds.Emplace(FName(TEXT("31171")));
            actorIds.Emplace(FName(TEXT("31172")));
            actorIds.Emplace(FName(TEXT("31173")));
            actorIds.Emplace(FName(TEXT("31175")));
            actorIds.Emplace(FName(TEXT("31176")));
            actorIds.Emplace(FName(TEXT("31179")));
            actorIds.Emplace(FName(TEXT("31180")));
    }
    if(StageID==TEXT("S_WED_SUPPORT_RHEA_VERYHARD")){
            actorIds.Emplace(FName(TEXT("33089")));
            actorIds.Emplace(FName(TEXT("33090")));
            actorIds.Emplace(FName(TEXT("33091")));
            actorIds.Emplace(FName(TEXT("33092")));
            actorIds.Emplace(FName(TEXT("33093")));
            actorIds.Emplace(FName(TEXT("33094")));
            actorIds.Emplace(FName(TEXT("33095")));
            actorIds.Emplace(FName(TEXT("33097")));
            actorIds.Emplace(FName(TEXT("33099")));
    }
}

void URTSManager::SendPacket(UNION_RTS type, flatbuffers::NativeTable *table) {
	if (!Socket.IsValid())
	{
		UE_LOG(LogNetwork, Log, TEXT("RTS: Socket Invalid"));
		return;
	}
	
	// FB Î∞îÏù¥???¥Î†à??ÎßåÎì§Í∏?
	FlatBufferBuilder fbb;
	U.type = type;
	U.value = table; //  reinterpret_cast<NativeTable*>(table);
	fbb.ForceDefaults(true);
	auto offset = U.Pack(fbb);
	auto CMD_offset = CreateCMD_RTS(fbb, type, offset);
	FinishCMD_RTSBuffer(fbb, CMD_offset);

	TArray<uint8> sendData;
	sendData.AddDefaulted(RTS_PACKET_HEADER_SIZE);
	sendData[0] = RTS_PACKET_HEADER_MAGIC0;
	sendData[1] = RTS_PACKET_HEADER_MAGIC1;

	flatbuffers::WriteScalar<uint32>(&sendData[2], fbb.GetSize());
	sendData.Append(fbb.GetBufferPointer(), fbb.GetSize());

	int32 Sent;
    if(!Socket->Send(sendData.GetData(), sendData.Num(), Sent))
    {
		SocketState = ESocketConnectionState::SCS_ConnectionError;
		UE_LOG(LogNetwork, Log, TEXT("RTS: Socket failed to send: %d"), Sent);
    }
}

void URTSManager::ReadSocketData() {
    if (!Socket.IsValid()) 
        return;
        
	// Socket?¥ÎÇò authrorized ?ÅÌÉú???¥Î? ?ïÏù∏???ÅÌô©?êÏÑú FrameMove???òÌï¥ ?∏Ï∂ú??
	// ?ÑÏöî?òÎã§Î©?Socket???ÅÌÉú ?ïÎèÑ???ïÏù∏??Ï∂îÍ??????àÏúº??.. ?¥Ï©åÍµ??ÄÏ©åÍµ¨...
    // bool HasPendingConnection = false;

    uint32 Size = 0;
    // uint8 RecvBuffers[PACKET_BUFFER_SIZE];
	TArray<uint8> bytes;
    TArray<uint8> ReceivedData;

	if (Chunk.Num() > 0) {
		ReceivedData.Append(Chunk);
		Chunk.Empty();
	}

	while (Socket->HasPendingData(Size))
	{
        /*
        int32 Read = 0;
        memset(&RecvBuffers, 0, sizeof(uint8) * PACKET_BUFFER_SIZE);
        Socket->Recv(RecvBuffers, Size, Read);
        ??
        */

        int32 Read = 0;
        bytes.SetNumZeroed(FMath::Min(Size, 65507u));
		Socket->Recv(bytes.GetData(), bytes.Num(), Read);
        ReceivedData.Append(bytes);
	}

	if (ReceivedData.Num() <= 0) return;
	else {
		int32 pos = 0, packetLength, readCount = 0;
		bool bContinue = true;
		TArray<uint8> packet;
		do {
			if (ReceivedData.Num() < RTS_PACKET_HEADER_SIZE + pos) {
				// ?ΩÏùÑ ?§Îçî??Í∏∏Ïù¥ Ï°∞Ï∞® ?ÜÎã§Î©?
				bContinue = false;
			}
			else if (
				ReceivedData[pos] != RTS_PACKET_HEADER_MAGIC0 || 
				((ReceivedData[pos + 1] ^ RTS_PACKET_HEADER_MAGIC1) != 0 && (ReceivedData[pos + 1] ^ RTS_PACKET_HEADER_MAGIC1) != 32)
			) {
				// Transition Error : ?§Îçî Î∂ÑÏÑù Í≤∞Í≥º ?¥ÏÉÅ???Ä?ùÏù¥?ºÎ©¥.
				// UE_LOG(LogNetwork, Log, TEXT("RTS: RECV Header T-Error %d %d %d"), pos, (int32)ReceivedData[pos], (int32)ReceivedData[pos + 1]);
				bContinue = false;
			}
			else {
				// ?§Îçî?êÏÑú ?®ÌÇ∑??Í∏∏Ïù¥Î•??ΩÍ≥†
				packetLength = flatbuffers::ReadScalar<uint32>(&ReceivedData[pos + 2]);
				// UE_LOG(LogNetwork, Log, TEXT("RTS: RECV Header %d %d %d: %d"), (int32)ReceivedData[pos], (int32)ReceivedData[pos + 1], packetLength, ReceivedData.Num());

				if (ReceivedData.Num() >= RTS_PACKET_HEADER_SIZE + packetLength + pos) { // ?ΩÏùÑ ?®ÌÇ∑???àÎã§Î©?
					packet.Append(&ReceivedData[RTS_PACKET_HEADER_SIZE + pos], packetLength);
					// UE_LOG(LogNetwork, Log, TEXT("RTS: RECV CHUNK %d : %d - %d: %d"), packet.Num(), ReceivedData.Num(), (int32)ReceivedData[pos + 6], (int32)packet[0]);
					pos += RTS_PACKET_HEADER_SIZE + packetLength;
					// throw Packet!!
					PacketParser(packet.GetData());
					packet.Empty();

					// if(++readCount >= RTS_PACKET_MAX_READ) bContinue = false;
				} else bContinue = false;
			}
		} while (bContinue);

		if (ReceivedData.IsValidIndex(pos)) {
            Chunk.Append(&ReceivedData[pos], ReceivedData.Num() - pos);
            packet.Empty();
            ReceivedData.Empty();
        }
		// UE_LOG(LogNetwork, Log, TEXT("RTS: RESET CHUNK %d"), Chunk.Num());
	}
}
