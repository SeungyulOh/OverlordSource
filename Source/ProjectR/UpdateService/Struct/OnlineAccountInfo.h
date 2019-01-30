// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SharedConstants/GlobalConstants.h"
#include "OnlineSubsystem.h"
#include "UpdateService/LoginDefs.h"
#include "OnlineAccountInfo.generated.h"

/**
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FOnlineAccountInfo
{
	GENERATED_BODY()

public:

	////static void INIT_CLIENT_KEY_RQ(FString publisher, FString code, FString access_token, FString redirect_uri, FString id, FString pwd);
	UPROPERTY()
	bool bIsAcceptTerms = false;
	/** Type of account. Needed to identity the auth method to use (epic, internal, facebook, etc) */
	UPROPERTY()
	FString Type;
	/** Id of the user logging in (email, display name, facebook id, etc) */
	UPROPERTY()
	FString Id;
	/** Credentials of the user logging in (password or auth token) */
	UPROPERTY()
	FString Token;
	/** Check kokomo */
	UPROPERTY()
	FString ClientKey;
	/** Game User Nick */
	UPROPERTY()
	FString NickName;
	
	int32 PublisherType;
	bool IsCheckAccount = false;
	//FOnlineAccountCredentials UnrealAccountType;

	/**
	 * Equality operator
	 */
	bool operator==(const FOnlineAccountInfo& Other) const
	{
		return Other.Type == Type &&
			Other.Id == Id;
	}

	/**
	 * Constructor
	 */
	FOnlineAccountInfo(const FString& InType, const FString& InId,
		const FString& InToken, FString& InClientKey) :
		Type(InType),
		Id(InId),
		Token(InToken),
		ClientKey(InClientKey)
	{
	}

	/**
	 * Constructor
	 */
	FOnlineAccountInfo()
	{
	}

	FString& GetId()
	{
		return Id;
	}


	/*void MakePublisherName(EPublisher InType)
	{
		switch (InType)
		{
			case EPublisher::Kokomo:	Type = PUBLISHERID_DEV;			break;
			case EPublisher::Facebook:	Type = PUBLISHERID_FACEBOOK;	break;
			case EPublisher::Google:	Type = PUBLISHERID_GOOGLEPLUS;	break;
			case EPublisher::IOS:		Type = PUBLISHERID_IOS;			break;
			default: break;
		}
	}*/
};
