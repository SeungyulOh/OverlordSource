// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChatManager.h"
#include "GlobalIntegrated.h"

#include "UI/Chat/UC_ChatItem.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "UpdateService/Auth/RAuth.h"
#include "UtilFunctionIntegrated.h"
#include "Table/ChatStringAttributeTableInfo.h"
#include "../RTSManager.h"

#define MOD_TYPE 1
#define MOD_ID 2
#define MOD_NAME 3
#define ITEMID_OR_PARTYNO 4
#define SECRET_NO 5

UChatManager::UChatManager()
{

}

bool UChatManager::Initialize(URGameInstance* InRGameInstance)
{
	RGameInstance = InRGameInstance;
	//MyKId = RGameInstance->UserInfo->GetkId();
	//MyWho = RGameInstance->UserInfo->Avatar.nick;


	return true;
}

void UChatManager::Shutdown()
{

}

bool UChatManager::IsConnected() const
{
	return false;
}

int UChatManager::GetGeneralChannelNumber() const
{
	return GeneralChannelNumber;
}

void UChatManager::ChangeChannel(FString Channel)
{
	URTSManager* rtsManager = URTSManager::GetInstancePtr();
	rtsManager->CHAT_CHANNEL_MOVE(Channel);
}

void UChatManager::SendMessage(FString Message, EChatListEnum ChatListEnum)
{
	URTSManager* rtsManager = URTSManager::GetInstancePtr();
	const int32 MaxChatLen = 120;
	const int32 nLen = Message.Len();
	if(MaxChatLen < nLen)
	{
		int32 ndelete =  nLen - MaxChatLen;
		Message.RemoveAt(MaxChatLen, ndelete);
	}
	MyKId = RGameInstance->UserInfo->GetkId();
	MyWho = RGameInstance->UserInfo->Avatar.nick;
	rtsManager->CHAT_MESSAGE(
		currentChannel, 
		OVERLORD::RTS::ENUM::CHAT_TYPE::CHAT_TYPE_NORMAL, 
		MyKId, 
		MyWho, 
		Message, 
		TEXT("")
	);
}

bool UChatManager::SendMessageInner(FString Message, EChatListEnum ChatListEnum)
{
	return true;
}

void UChatManager::OnNumNewChange()
{
}

void UChatManager::BlockUser(const FChatUser& ChatUser)
{

}

void UChatManager::GetBlockedUsers(TArray<FChatUser>& OutBlocked)
{
}


bool UChatManager::IsUserBlocked(int32 kId) const
{
	return false;
}

void UChatManager::UnblockUser(int32 kId)
{

}

void UChatManager::SubscribeToRealTimeChannel()
{
}

void UChatManager::UnsubscribeFromRealTimeChannel()
{
}

void UChatManager::IssueMessage(FText Message, EChatItemType ItemType)
{
}

void UChatManager::OnConnectionChanged(bool bConnected)
{

}

void UChatManager::OnTimerTrigger()
{
}

bool UChatManager::GetOnInputCooldown() const
{
	return RemainingSeconds != 0;
}

int UChatManager::GetInputCooldownRemainingSeconds() const
{
	return RemainingSeconds;
}

void UChatManager::OnReceive_UserChannelEnter(FString channel, int32 id, FString nick)
{
	std::string string = TCHAR_TO_UTF8(*channel);
	AddUserMessage(stringToEnum(string.at(0)), id, nick, true);
	RGAMEINSTANCE(this)->EventManager->Receive_UserInOut.Broadcast();
}

void UChatManager::OnReceive_UserChannelLeave(FString channel, int32 id, FString nick)
{
	std::string string = TCHAR_TO_UTF8(*channel);
	AddUserMessage(stringToEnum(string.at(0)), id, nick, false);
	RGAMEINSTANCE(this)->EventManager->Receive_UserInOut.Broadcast();
}

void UChatManager::OnReceive_OnChatSubscribed(FString channel)
{
	currentChannel = channel;
	AddChannelMessage(channel);
	RGAMEINSTANCE(this)->EventManager->Receive_CurrentChannel.Broadcast(channel);
}

void UChatManager::OnReceive_Message(EChatListEnum channel, uint8 chattype, int32 id, FString nick, FString message, TArray<FString> data)
{
	FChatItem item;
	item.kId = id;
	item.GroupType = channel;
	item.UIType = (EChatItemUIType)chattype;
	item.ExtraData = data;

	//nick, message set by type
	switch (item.UIType)
	{
	case EChatItemUIType::VE_NORMAL:
	{
		if (channel == EChatListEnum::VE_GENERAL || channel == EChatListEnum::VE_PARTY || channel == EChatListEnum::VE_GUILD)
			item.Who = TEXT("<") + nick + TEXT("> :");
		else if (channel == EChatListEnum::VE_WHISPER)
			item.Who = (FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_Whisper")), FText::FromString(nick))).ToString();

		RGAMEINSTANCE(this)->EventManager->UpdateLastMessage.Broadcast(nick, message);
		item.Message = FText::FromString(message);
	}
	break;
	case EChatItemUIType::VE_GET:
	{
		item.Who = FString();
		FName itemName = UTableManager::GetInstancePtr()->GetCharacterItemRow(FName(*data[ITEMID_OR_PARTYNO]))->ItemName;
		item.Message = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_ItemGet")), FText::FromString(nick), FText::FromString(data[MOD_NAME]), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, itemName));
	}
	break;
	case EChatItemUIType::VE_EVENT:
	case EChatItemUIType::VE_GM:
		item.Who = FString();
		item.Message = FText::FromString(message);
		break;
	case EChatItemUIType::VE_INVITE:
	{
		item.Who = FString();
		item.Message = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_UserInvite")), FText::FromString(nick), FText::FromString(data[MOD_NAME]));
		FInviteData inviteData;
		
		inviteData.m_Nick = nick;
		inviteData.m_ModName = data[MOD_NAME];
		inviteData.m_ModType = FCString::Atoi(*data[MOD_TYPE]) ;
		inviteData.m_ModId = FName(*data[MOD_ID]);
		inviteData.m_PartyNo = FCString::Atoi(*data[ITEMID_OR_PARTYNO]);
		InviteLists.push(inviteData);
	}
		break;
	default:
		break;
	}

	SetQueueSizeConst();
	ChatLists.push(item);
	RGAMEINSTANCE(this)->EventManager->Receive_Message.Broadcast();
}

void UChatManager::AddChannelMessage(FString channel)
{
	FChatItem item;
	item.kId = ChatLists.size();
	item.UIType = EChatItemUIType::VE_SYSTEM;
	item.GroupType = EChatListEnum::VE_NONE;
	item.Who = channel;
	item.Message = FText::FromString(TEXT("enter the channel"));

	SetQueueSizeConst();
	ChatLists.push(item);
}

void UChatManager::AddUserMessage(EChatListEnum channel, int32 id, FString nick, bool inOut)
{
	FChatItem item;
	item.kId = id;
	item.Who = FString();
	item.GroupType = channel;
	item.UIType = item.UIType = EChatItemUIType::VE_INOUTALARM;
	
	if (inOut)
		item.Message = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_UserEnter")), FText::FromString(nick)) ;
	else
		item.Message = FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Chatting_UserLeave")), FText::FromString(nick));

	SetQueueSizeConst();
	ChatLists.push(item);
}

void UChatManager::SetQueueSizeConst()
{
	while (ChatLists.size() > 100)
	{
		ChatLists.pop();
	}
}

TArray<FString> UChatManager::SplitDataString(FString data)
{
	TArray<FString> out;
	data.ParseIntoArray(out, TEXT("\x1e"), true);
	return out;
}

EChatListEnum UChatManager::stringToEnum(char type)
{
	if (type == 'C')
		return EChatListEnum::VE_GENERAL;
	else if (type == 'P')
		return EChatListEnum::VE_PARTY;
	else if (type == 'G')
		return EChatListEnum::VE_GUILD;
	else if (type == 'N')
		return EChatListEnum::VE_NOTIFY;
	else if (type == 'W')
		return EChatListEnum::VE_WHISPER;
	else
		return EChatListEnum::VE_NONE;
}

void UChatManager::SubscribeToChannels(TArray<FString> channels)
{
}


void UChatManager::UnsubscribeFromChannels(TArray<FString> channels)
{
}


void UChatManager::ReportEnterEvent(int32 kId, FString who, FString channel)
{
}


void UChatManager::ReportExitEvent(int32 kId, FString who, FString channel)
{
}

EChatItemType UChatManager::MapMessageFieldsToItemType(FString ChannelTag, EChatType type) const
{
	EChatItemType ItemType = EChatItemType::VE_GENERAL;
	return ItemType;
}

TArray<EChatListEnum> UChatManager::MapItemTypeToChatLists(EChatItemType ItemType) const
{
	TArray<EChatListEnum> ChatListEnums;
	return ChatListEnums;
}


bool UChatManager::MapItemTypeToNew(EChatItemType ItemType) const
{
	bool bRes = true;
	return bRes;
}


bool UChatManager::ItemTypeAtHome(EChatItemType ItemType, EChatListEnum ChatListEnum) const
{
	bool bRes = false;
	return bRes;
}

FColor UChatManager::NormalMessageColor(EChatItemType ItemType, EChatListEnum ChatListEnum, EChatItemUIType ItemUIType) const
{
	return FColor::Black;
}


FColor UChatManager::FormatSystemMessage(FString& Message, EChatItemType ItemType) const
{
	return FColor::Red;
}

void UChatManager::HandleReceivedMessage(FString ChannelTag, int32 ChannelNumber, FString who, int32 kId, EChatType type, FString message)
{
}


void UChatManager::DispatchChatMessage(EChatItemType ItemType, FString who, int32 kId, FString message)
{
}


FText UChatManager::BuildErrorMessage(int32 code, FString name, TArray<FString> desc) const
{
	return FText::FromString(TEXT("NOT WORKING"));
}

FString UChatManager::EnumToChannel(EChatListEnum ChatListEnum) const
{
	return TEXT("NOT WORKING");
}


FString UChatManager::WriteGeneralChannelName(int Channel) const
{
	return TEXT("NOT WORKING");
}

bool UChatManager::ReadChannelName(FString channel, FString& OutTag, int32& OutNumber) const
{
	return true;
}
