// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ChatList.h"


FChatList::FChatList()
{
	Reset();
}

FChatList::~FChatList()
{
}

bool FChatList::GetIsActive() const
{
	return IsActive;
}

const FChatItem& FChatList::GetChatItem(CHATID_T Id) const
{
	// Check if the id is within the window
	// TODO
	//assert(Id > HeadId || Id <= HeadId - ChatItemsSize);

	return GetAtId(Id);
}

void FChatList::GetWindow(CHATID_T& OutHead, int& OutSize) const
{
	OutHead = HeadId;
	OutSize = ChatItemsSize;
}

int FChatList::GetNumNewItems()
{
	return NumNewItems;
}

void FChatList::AddChatItem(const FChatItem& InChatItem, bool bNew)
{
	FChatItem& ChatItem = Add();
	ChatItem = InChatItem;

	// // chat system messages do not count as new message...

	if (bNew)
	{
		if (NumNewItems < NUM_CHAT_ITEMS)
		{
			NumNewItems++;

			// Prompt update Chat Notify State
			OnNumNewChangeDelegate.ExecuteIfBound();
		}
	}
}

void FChatList::Reset()
{
	HeadIndex = -1;
	ChatItemsSize = 0;

	HeadId = -1;

	NumNewItems = 0;

	// Prompt update Chat Notify State
	OnNumNewChangeDelegate.ExecuteIfBound();
}

void FChatList::SetAsSeen()
{
	NumNewItems = 0;

	// Prompt update Chat Notify State
	OnNumNewChangeDelegate.ExecuteIfBound();

}

void FChatList::SetIsActive(bool InIsActive)
{
	IsActive = InIsActive;
}

FChatItem& FChatList::Add()
{
	if (ChatItemsSize != NUM_CHAT_ITEMS)
	{
		ChatItemsSize++;
	}

	HeadIndex = (HeadIndex + 1) % NUM_CHAT_ITEMS;
	HeadId++;
	return ChatItems[HeadIndex];
}

FChatItem& FChatList::GetAtId(CHATID_T Id)
{
	check(Id <= HeadId);

	int Index = (HeadIndex + (HeadId - Id) + NUM_CHAT_ITEMS) % NUM_CHAT_ITEMS;
	return ChatItems[Index];
}

const FChatItem& FChatList::GetAtId(CHATID_T Id) const
{
	check(Id <= HeadId);

	int Index = (HeadIndex - (HeadId - Id) + NUM_CHAT_ITEMS) % NUM_CHAT_ITEMS;
	return ChatItems[Index];
}
