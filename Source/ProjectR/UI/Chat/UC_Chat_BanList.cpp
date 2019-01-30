// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Network/Chat/ChatManager.h"
#include "UC_Chat_BanList_ScrollItem.h"
#include "UC_Chat_BanList_ScrollData.h"
#include "UC_Chat_BanList.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


void UUC_Chat_BanList::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ChatManager = RGameInstance->ChatManager;

	ChatManager->OnBlockedUsersChange.AddDynamic(this, &UUC_Chat_BanList::InvalidateData);
	
	if (!IsValid(Scroll_BanList))
	{
		return;
	}

	Scroll_BanList->Init();

	InvalidateData();
}


void UUC_Chat_BanList::NativeDestruct()
{
	ChatManager->OnBlockedUsersChange.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_Chat_BanList::InvalidateData()
{
	TArray<FChatUser> BlockedUsers;
	ChatManager->GetBlockedUsers(BlockedUsers);

	Scroll_BanList->ClearScrollItemData();

	for (int i = 0; i < BlockedUsers.Num(); i++)
	{
		auto ScrollData = Cast<UUC_Chat_BanList_ScrollData>(Scroll_BanList->AddScrollItemData(UUC_Chat_BanList_ScrollData::StaticClass()));

		ScrollData->kId = BlockedUsers[i].kId;
		ScrollData->who = BlockedUsers[i].who;
	}

	Scroll_BanList->InvalidateData();
}
