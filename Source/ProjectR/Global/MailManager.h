// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataEnum.h"
#include "Network/PacketDataStructures.h"
#include "Network/ClientProtocols.h"
#include "MailManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnUpdateMailList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnMailListRp, const TArray<FMAIL>&, InMails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnGiftOpenRp, const TArray<FREWARD>&, InRewards);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnMoreMailListRp, const TArray<FMAIL>&, InMails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnEventRewardOpen, const FString&, InMailUD, const TArray<FREWARD>&, InRewards);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnEventRewardOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnEventRewardRp, const FString&, InRewardId);
class URScrollItemData;
//class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UMailManager : public UObject
{
	GENERATED_BODY()
	
	
public:
	void Initialize(/*URGameInstance* InRGameInstance*/);
	
	TArray<FMAIL>& GetMailList();
	void SetMailList(TArray<FMAIL>& InMails);
	//Receive Data From Server
	void UpdateMailNotify();
	void UpdateMailList(TArray<FMAIL>& InMails);
	void UpdateMoreMailList(TArray<FMAIL>& InMails);

	void UpdateSpecialMailList(TArray<FMAIL>& InMails);

	TArray<URScrollItemData*>& GetReceiveList()		{ return ReceiveList; }
	void CleanReceiveList()							{ ReceiveList.Empty(); }
	void AddReceiveList(URScrollItemData* InItem)	{ ReceiveList.Emplace(InItem); }

	void SetNotify(bool InIsNotify);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnUpdateMailList	OnUpdateMailList;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnMailListRp		OnMailListRp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnMailListRp		OnSpecialMailListRp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnGiftOpenRp		OnGiftOpenRp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnGiftOpenRp		OnSpecialGiftOpenRp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnMoreMailListRp	OnMoreMailListRp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnEventRewardRp	OnEventRewardRp;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = MailManager)
	FDelegate_OnEventRewardOpen OnEventRewardOpen;
	
	UPROPERTY()
	TArray<FMAIL>				MailLists;
	UPROPERTY()
	TArray<URScrollItemData*>	ReceiveList;

public:
	bool						IsReceiveAll = false;

private:
// 	UPROPERTY()
// 	URGameInstance*				RGameInstance = nullptr;


};
