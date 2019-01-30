// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ChatList.h"
#include "ProjectR.h"
#include <queue>
#include "ChatManager.generated.h"

//// Frontend definitions

UENUM(BlueprintType)
enum class EChatItemType: uint8
{
	VE_GENERAL = 0,
	VE_BATTLE,
	VE_PARTY,
	VE_GUILD,
	VE_SYSTEM_CHAT_GENERAL,
	VE_SYSTEM_CHAT_BATTLE,
	VE_SYSTEM_CHAT_PARTY,
	VE_SYSTEM_CHAT_GUILD,
	VE_SYSTEM_CHAT_CRITICAL,
	VE_SYSTEM_GET,
	VE_SYSTEM_BATTLE,
	VE_SYSTEM_EVENT,
	VE_SYSTEM_GM
};

// Params: channel that received the message, in order to be informative
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatManagerOnNewMessage, TArray<EChatListEnum>, AffectedChatLists);

// Params: channel that received the message, in order to be informative
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatManagerSubscriptionChange, TArray<EChatListEnum>, AffectedChatLists);

// Params: channel that received the message, in order to be informative
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatManagerPopupMessage, FText, Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChatManagerBlockedUsersChange);

// Will be called for 3, 2, 1, off cool down
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChatManagerInputCooldownChange, bool, bOnCoolDown, int, RemainingSeconds);


//// Game client definitions

//// Backend definitions

UENUM(BlueprintType)
enum class EChatType: uint8
{
	VE_NORMAL = 0,
	VE_GET = 1,
	VE_EVENT = 2,
	VE_GM = 3,
	VE_ENUM_END = 4
};

USTRUCT(BlueprintType)
struct PROJECTR_API FInviteData
{
	GENERATED_USTRUCT_BODY()
public:
	FString m_Nick;
	FString m_ModName;
	int32 m_ModType;
	FName m_ModId;
	int32 m_PartyNo;
	FString m_Secret;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FChatUser
{
	GENERATED_USTRUCT_BODY()

public:
	FChatUser(): kId(0), who(TEXT("")) {}
	FChatUser(int32 InKId, FString InWho): kId(InKId), who(InWho) {}

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		int32 kId = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString who;
};

class URGameInstance;

/**
 * Currently statically holding ChatList for each channel
 */
UCLASS(BlueprintType)
class PROJECTR_API UChatManager : public UObject
{
	GENERATED_BODY()
	
public:
	UChatManager();

	// Return value of false means ChattingSystem is permanently disabled; internal error (e.g. can't create socket)
	bool Initialize(URGameInstance* InRGameInstance);
	void Shutdown();


	////// Frontend interface - UC_Chat

	bool IsConnected() const;

	int GetGeneralChannelNumber() const;

	// Return value of false means channel out of range
	void ChangeChannel(FString Channel);

	UPROPERTY(BlueprintAssignable, Category = ChatManager)
	FChatManagerOnNewMessage OnNewMessage;

	UPROPERTY(BlueprintAssignable, Category = ChatManager)
	FChatManagerSubscriptionChange OnSubscriptionChange;

	UPROPERTY(BlueprintAssignable, Category = ChatManager)
	FChatManagerPopupMessage OnPopupMessage;

	UPROPERTY(BlueprintAssignable, Category = ChatManager)
	FChatManagerInputCooldownChange OnInputCooldownChange;

	//FChatList& GetChatList(EChatListEnum ChatListEnum);

	void SendMessage(FString Message, EChatListEnum ChatListEnum);

	// Delegate to FChatList.OnNumNewChangeDelegate
	void OnNumNewChange();

	// Blocking user
	// UI is responsible for refreshing its views
	void BlockUser(const FChatUser& ChatUser);
	void GetBlockedUsers(TArray<FChatUser>& OutBlocked); // TODO: should be const
	bool IsUserBlocked(int32 kId) const;
	void UnblockUser(int32 kId);

	UPROPERTY(BlueprintAssignable, Category = ChatManager)
	FChatManagerBlockedUsersChange OnBlockedUsersChange;
	

	////// Game client interface
	void SubscribeToRealTimeChannel();
	void UnsubscribeFromRealTimeChannel();

	// Battle
	void DispatchChatMessage(EChatItemType ItemType, FString who, int32 kId, FString message);

	////// Backend interface - TcpConnManager

	// Insert System Message

	void IssueMessage(FText Message, EChatItemType ItemType);

	// delegates to TcpConnManager

	UFUNCTION()
	void OnConnectionChanged(bool bConnected);

	UFUNCTION()
	void OnTimerTrigger();

	bool GetOnInputCooldown() const;
	int GetInputCooldownRemainingSeconds() const;
	/////////////////////
	UFUNCTION()
		void OnReceive_UserChannelEnter(FString channel, int32 id, FString nick);
	UFUNCTION()
		void OnReceive_UserChannelLeave(FString channel, int32 id, FString nick);
	UFUNCTION()
	void OnReceive_OnChatSubscribed(FString channel);
	UFUNCTION()
		void OnReceive_Message(EChatListEnum channel, uint8 chattype, int32 id, FString nick, FString message, TArray<FString> inviteData);
	//set chat item
	void AddChannelMessage(FString channel);
	//set user log item
	void AddUserMessage(EChatListEnum channel, int32 id, FString nick, bool inOut);
	//
	void SetQueueSizeConst();
	TArray<FString> SplitDataString(FString data);

	FString m_nick = "";
	FString m_message = "";

	EChatListEnum stringToEnum(char type);
	//chat list data
	std::queue<FChatItem> ChatLists;
	std::queue<FInviteData> InviteLists;
	//channel data
	FString currentChannel;

private:
	bool SendMessageInner(FString Message, EChatListEnum ChatListEnum);

//	bool CheckRealtimeContentMessage(EChatListEnum ChatListEnum, FString Message);

	// Subscribe/unsubscribe
	void SubscribeToGeneralChannel(FString channel, int32 ChannelNumber);
	void UnsubscribeFromGeneralChannel(FString channel);
	void SubscribeToGuildChannel(FString channel, int32 ChannelNumber);

	// Client chat system channel tag B for realtime game modes
	void SubscribeToChannels(TArray<FString> channels);
	void UnsubscribeFromChannels(TArray<FString> channels);

	// Events
	void ReportEnterEvent(int32 kId, FString who, FString channel);
	void ReportExitEvent(int32 kId, FString who, FString channel);

	// Dispatch helpers
	EChatItemType MapMessageFieldsToItemType(FString ChannelTag, EChatType type) const;
	TArray<EChatListEnum> MapItemTypeToChatLists(EChatItemType ItemType) const;
	bool MapItemTypeToNew(EChatItemType ItemType) const;

	bool ItemTypeAtHome(EChatItemType ItemType, EChatListEnum ChatListEnum) const;

	// Styling/formatting helpers
	FColor NormalMessageColor(EChatItemType ItemType, EChatListEnum ChatListEnum, EChatItemUIType ItemUIType) const;
	FColor FormatSystemMessage(FString& Message, EChatItemType ItemType) const;

	int32 MyKId = 0;
	FString MyWho;

	void HandleReceivedMessage(FString ChannelTag, int32 ChannelNumber, FString who, int32 kId, EChatType type, FString message);

	FText BuildErrorMessage(int32 code, FString name, TArray<FString> desc) const;

	//// Subscription state
	// Empty string for channel name mean not subscribed
	FString NoticeChannelName;

	FString PrivateChannelName;

	FString GuildChannelName;
	int32 GuildId = 0;

	FString PartyChannelName;
	int32 PartyId = 0;

	FString GeneralChannelName;
	int GeneralChannelNumber = 0;

	// Returns empty string for not subscribed channel
	FString EnumToChannel(EChatListEnum ChatListEnum) const;

	FString WriteGeneralChannelName(int Channel) const;

	// WARNING: Number part better be not 0
	bool ReadChannelName(FString channel, FString& OutTag, int32& OutNumber) const;

	//// Blocked users
	bool bBlockUsers = true;
	TArray<FChatUser> BlockedUsers;


	

	URGameInstance* RGameInstance = nullptr;

	// Input cooldown
	FTimerHandle TimerHandle;
	/*
	Zero means cooldown not active
	*/
	int RemainingSeconds = 0;
};
