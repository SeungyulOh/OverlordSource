// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GlobalConstants.h"
#include "ChatList.generated.h"

// TODO Adjust
#define NUM_CHAT_ITEMS 100

// Should I be concerned about Id value rollover?
typedef int CHATID_T;

// ChatItem/ChatList Styling should be data-ized



UENUM(BlueprintType)
enum class EChatItemUIType: uint8
{
	VE_NORMAL = 0,
	VE_GET = 1,
	VE_EVENT = 2,
	VE_GM = 3,
	VE_INVITE = 4,
	VE_WHISPER = 5,
	VE_INOUTALARM = 6,
	VE_SYSTEM = 7,
};

// Represents chat messages or system messages
// Statically exist in ChatList
USTRUCT(Blueprintable)
struct PROJECTR_API FChatItem
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY()
		EChatItemUIType UIType = EChatItemUIType::VE_NORMAL;

	UPROPERTY()
		EChatListEnum GroupType = EChatListEnum::VE_GENERAL;

	UPROPERTY()
		int32 kId = 0;

	UPROPERTY()
		FString Who;

	UPROPERTY()
		FText Message;

	UPROPERTY()
		FDateTime Timestamp;

	UPROPERTY()
		TArray<FString> ExtraData;
};


DECLARE_MULTICAST_DELEGATE(FChatListOnNumNewChange);
typedef FChatListOnNumNewChange::FDelegate FChatListOnNumNewChangeDelegate;

/**
 * ring structured ChatItem store
 * Statically exist in ChatManager
 */
class PROJECTR_API FChatList
{
public:
	FChatList();
	~FChatList();

	////// Frontend interface - UC_Chat

	bool GetIsActive() const;

	const FChatItem& GetChatItem(CHATID_T Id) const;

	// For UI arranging
	void GetWindow(CHATID_T& OutHead, int& OutSize) const;

	//// Newness
	int GetNumNewItems();
	void SetAsSeen();

	////// Backend interface - ChatManager

	void SetIsActive(bool InIsActive);

	// For ChatManager to hook to
	FChatListOnNumNewChangeDelegate OnNumNewChangeDelegate;

	void AddChatItem(const FChatItem& InChatItem, bool bNew);
	// In case channel change
	void Reset();

private:
	bool IsActive = false;

	// ChatItems store
	FChatItem ChatItems[NUM_CHAT_ITEMS];
	FChatItem& Add(); // Not Emptied
	FChatItem& GetAtId(CHATID_T Id);
	const FChatItem& GetAtId(CHATID_T Id) const;

	// Window
	int HeadIndex = -1;
	int ChatItemsSize = 0;

	CHATID_T HeadId = -1;

	// Newness
	int NumNewItems = 0;
};
