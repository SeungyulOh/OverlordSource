
#pragma once

#include "chat_generated.h"
#include "Utils/FlatBuffersHelper.h"
#include "ProjectR.h"
#include "ChatProtocols.generated.h"

// Manually written to reflect BuildTools/fbs/chat.fbs and header.fbs


USTRUCT(BlueprintType)
struct PROJECTR_API FChatException
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		bool suc;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		int32 code;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		TArray<FString> desc;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FChatSubscribed
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		int32 kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString who;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		TArray<FString> channels;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FChatException error;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FChatChannelMoved
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		int32 kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString channel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FChatException error;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FChatChannelEnter
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		int32 kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString who;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString channel;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FChatChannelExit
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		int32 kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString who;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString channel;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRChatMessage
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString who;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		int32 kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		uint8 type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString channel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChatProtocols")
		FString message;
//	UPROPERTY()
//		int64 frame;
};


// None of these can handle error
// TODO: FIX: SO INSECURE
class PROJECTR_API FChatProtocols
{
public:

	////// Serialize ChatApi

	// These create packets

	// Serialize ChatSubscribe into buffer/CHATAPI
	static bool SerializeChatSubscribe(TArray<uint8>& OutBuf, FString clientkey, FString deviceid, TArray<FString> channels);

	static bool SerializeChatChannelMove(TArray<uint8>& OutBuf, FString clientkey, FString deviceid, FString channel);

	// Serialize ChatMessage into buffer/CHATAPI
	static bool SerializeChatMessage(TArray<uint8>& OutBuf, FString who, int32 kId, uint8 type, FString channel, FString message, int64 frame);

private:
	static void MakePacket(TArray<uint8>& OutBuf, const uint8* Data, int32 DataSize);


	////// Deserialize ChatApi
public:
	static bool VerifyCOMMAND(const uint8* Buf, size_t BufSize);

	// Read UNION_CHAT type of Api data
	static bool GetCOMMANDApiType(const uint8* Buf, size_t BufSize, OVERLORD::UNIONS& OutApiType);

	// Deserialize api data given its type
	template<typename T>
	static bool DeserializeCOMMANDApi(const uint8* Buf, size_t BufSize, T& OutApiData)
	{
		uint8* ApiData;
		GetCOMMANDApiData(Buf, BufSize, ApiData);
		if (!FFlatBuffersHelper::FlatBufferToUStruct(ApiData, Buf + BufSize - ApiData, T::StaticStruct(), &OutApiData))
		{
			UE_LOG(LogChatProtocols, Log, TEXT("ChatApiToWhatever: FlatBufferToUStruct failed for %s"), *(T::StaticStruct()->GetName()));
			return false;
		}
		return true;
	}

	////// Header operations

	// OutPacketSize of zero means incomplete packet
	static bool DeterminePacket(const uint8* Buf, size_t BufSize, size_t& OutDataSize);


private:

	// Get pointer into the data
	static bool GetCOMMANDApiData(const uint8* Buf, size_t BufSize, uint8*& OutPointer);

};