// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/UI_MailBox/MailListScrollData.h"
#include "GlobalIntegrated.h"

#include "GameInfo/UserInfo.h"

#include "MailManager.h"


void UMailManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
//	RGameInstance = InRGameInstance;
}

TArray<FMAIL>& UMailManager::GetMailList()
{
	return MailLists;
}

void UMailManager::SetMailList(TArray<FMAIL>& InMails)
{
	MailLists = InMails;

	bool bIsNotify = (InMails.Num() > 0);
	SetNotify(bIsNotify);
}

void UMailManager::UpdateMailNotify()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		bool bIsNotify = (RGameInstance->UserInfo->Avatar.mailN > 0);
// 		SetNotify(bIsNotify);
// 	}
}

void UMailManager::UpdateMailList(TArray<FMAIL>& InMails)
{
	//SetMailList(InMails);

	OnMailListRp.Broadcast(InMails);

	bool bIsNotify = (InMails.Num() > 0);
	SetNotify(bIsNotify);
}

void UMailManager::UpdateSpecialMailList(TArray<FMAIL>& InMails)
{
	OnSpecialMailListRp.Broadcast(InMails);

// 	bool bIsNotify = (InMails.Num() > 0);
// 	SetNotify(bIsNotify);
}

void UMailManager::UpdateMoreMailList(TArray<FMAIL>& InMails)
{
	//SetMailList(InMails);

	OnMoreMailListRp.Broadcast(InMails);

	bool bIsNotify = (InMails.Num() > 0);
	SetNotify(bIsNotify);
}

void UMailManager::SetNotify(bool InIsNotify)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->NotifyManager))
	{
		return;
	}

	if(InIsNotify)
	{
		RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_MailBox, true);
	}
	else
	{
		RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_MailBox, false);
	}
}