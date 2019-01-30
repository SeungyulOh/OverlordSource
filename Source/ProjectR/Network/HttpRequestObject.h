#pragma once

#include "Runtime/Online/HTTP/Public/Http.h"
#include "HttpRequestObject.generated.h"


USTRUCT()
struct PROJECTR_API FHttpRequestObject
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY()
	FString			Dest;

	UPROPERTY()
	FString			Data;

	UPROPERTY()
	FString			mID;

	UPROPERTY()
	FString			qID;

	UPROPERTY()
	FString			Delim;

	UPROPERTY()
	FString			BinAddData;

	UPROPERTY()
	TArray<uint8>	ArData;

	void	AddURLPair(FString Param, FString Value)
	{
		Data  += (Delim + Param + "=" + Value);
		Delim = "&";
	}

	void	SetDestination(FString& NewDest)
	{
		Dest = NewDest;
	}

	void	SetModuleInfo(FString& NewMID, FString& NewQID)
	{
		mID = NewMID;
		qID = NewQID;
	}

	FHttpRequestObject()
	{
		Delim	= "";
		Dest = "";
		Data = "";
		mID = "";
		qID = "";

		BinAddData = "";
		ArData.Empty();
	}
};