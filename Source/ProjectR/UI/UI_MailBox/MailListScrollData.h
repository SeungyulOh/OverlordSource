// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "MailListScrollData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UMailListScrollData : public URScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	bool		bEventMail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	FString		MailUD;

	//int32		kId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	int32		kId = 0;
	
	//FString		subject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	FText		Text_Title;

	//FString		from;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	FText		Text_NickName;

	//FString		created;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	FText		Text_ReceiveDay;

	//FString		expire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	FText		Text_RemainDay;
	
	//TArray<FREWARD>		attach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	TArray<FITEM_ICON_INFO> ScrollItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MailListScrollData)
	TArray<FREWARD>			RewardInfo;

	//FString		desc;
};