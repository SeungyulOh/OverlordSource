
#include "ProjectR.h"
#include "ChatProtocols.h"
#include "chat_generated.h"
#include "flatbuffers/flatbuffers.h"
// #include "Utils/FlatBuffersHelper.h"

#define CHAT_HEADER_SIZE 6
#define CHAT_HEADER_MAGIC0 67
#define CHAT_HEADER_MAGIC1 84

#define BUILDER_INITIAL_SIZE 1024



bool FChatProtocols::SerializeChatSubscribe(TArray<uint8>& OutBuf, FString clientkey, FString deviceid, TArray<FString> channels)
{

	flatbuffers::FlatBufferBuilder Builder(BUILDER_INITIAL_SIZE);

	// push channels strings
	TArray<flatbuffers::Offset<flatbuffers::String>> channelsOffsets;
	for (int32 Idx = 0; Idx < channels.Num(); Idx++)
	{
		flatbuffers::Offset<flatbuffers::String> c = Builder.CreateString((const char *)TCHAR_TO_UTF8(*channels[Idx]));
		channelsOffsets.Add(c);
	}

	// create channels vector
	Builder.StartVector(channels.Num(), sizeof(flatbuffers::uoffset_t));
	for (int32 Idx = channelsOffsets.Num() - 1; Idx >= 0; Idx--)
	{
		Builder.PushElement<flatbuffers::String>(channelsOffsets[Idx]);
	}
	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> channelsOffset = Builder.EndVector(channels.Num());

	auto clientkeyOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*clientkey));
	auto deviceidOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*deviceid));

	// ChatSubscribe
	auto ChatSubscribeOffset = OVERLORD::CHAT::CreateChatSubscribe(Builder, clientkeyOffset, deviceidOffset, channelsOffset);

	// CHATAPI
	auto COMMANDStart = Builder.StartTable();
	Builder.AddElement<uint8_t>(4, OVERLORD::UNIONS_OVERLORD_CHAT_ChatSubscribe, 0);
	Builder.AddOffset(6, ChatSubscribeOffset);
	auto COMMAND = Builder.EndTable(COMMANDStart, 2);

	Builder.Finish<OVERLORD::COMMAND>(COMMAND);

	MakePacket(OutBuf, Builder.GetBufferPointer(), Builder.GetSize());

	return true;
}


bool FChatProtocols::SerializeChatChannelMove(TArray<uint8>& OutBuf, FString clientkey, FString deviceid, FString channel)
{
	flatbuffers::FlatBufferBuilder Builder(BUILDER_INITIAL_SIZE);

	auto clientkeyOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*clientkey));
	auto deviceidOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*deviceid));
	auto channelOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*channel));

	auto ChatChannelMoveOffset = OVERLORD::CHAT::CreateChatChannelMove(Builder, clientkeyOffset, deviceidOffset, channelOffset);

	auto COMMANDStart = Builder.StartTable();
	Builder.AddElement<uint8_t>(4, OVERLORD::UNIONS_OVERLORD_CHAT_ChatChannelMove, 0);
	Builder.AddOffset(6, ChatChannelMoveOffset);
	auto COMMANDOffset = Builder.EndTable(COMMANDStart, 2);

	Builder.Finish<OVERLORD::COMMAND>(COMMANDOffset);

	MakePacket(OutBuf, Builder.GetBufferPointer(), Builder.GetSize());

	return true;
}

bool FChatProtocols::SerializeChatMessage(TArray<uint8>& OutBuf, FString who, int32 kId, uint8 type, FString channel, FString message, int64 frame)
{
	flatbuffers::FlatBufferBuilder Builder(BUILDER_INITIAL_SIZE);

	auto whoOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*who));
	auto channelOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*channel));
	auto messageOffset = Builder.CreateString((const char *)TCHAR_TO_UTF8(*message));

	auto ChatMessageOffset = OVERLORD::CHAT::CreateChatMessage(Builder, whoOffset, kId, (OVERLORD::CHAT::CHAT_TYPE)type, channelOffset, messageOffset, frame);

	auto COMMANDStart = Builder.StartTable();
	Builder.AddElement<uint8_t>(4, OVERLORD::UNIONS_OVERLORD_CHAT_ChatMessage, 0);
	Builder.AddOffset(6, ChatMessageOffset);
	auto COMMANDOffset = Builder.EndTable(COMMANDStart, 2);

	Builder.Finish<OVERLORD::COMMAND>(COMMANDOffset);

	MakePacket(OutBuf, Builder.GetBufferPointer(), Builder.GetSize());

	return true;
}


void FChatProtocols::MakePacket(TArray<uint8>& OutBuf, const uint8* Data, int32 DataSize)
{
	OutBuf.AddDefaulted(CHAT_HEADER_SIZE);
	OutBuf[0] = CHAT_HEADER_MAGIC0;
	OutBuf[1] = CHAT_HEADER_MAGIC1;
	
	flatbuffers::WriteScalar<uint32>(&OutBuf[2], DataSize);

	OutBuf.Append(Data, DataSize);
}


bool FChatProtocols::VerifyCOMMAND(const uint8* Buf, size_t BufSize)
{
	auto verifier = flatbuffers::Verifier(Buf, BufSize);
	bool ok = OVERLORD::VerifyCOMMANDBuffer(verifier);
	return ok;
}

bool FChatProtocols::GetCOMMANDApiType(const uint8* Buf, size_t BufSize, OVERLORD::UNIONS& OutApiType)
{
	auto COMMAND = OVERLORD::GetCOMMAND(Buf);
	OutApiType = COMMAND->api_type();

	return true;
}


bool FChatProtocols::DeterminePacket(const uint8* Buf, size_t BufSize, size_t& OutDataSize)
{
	if (BufSize < CHAT_HEADER_SIZE)
	{
		// Incomplete header
		OutDataSize = 0;
		return true;
	}

	// Check magic bytes [0,1]
	uint8 Magic0 = Buf[0];
	uint8 Magic1 = Buf[1];

	if (Magic0 != CHAT_HEADER_MAGIC0 || Magic1 != CHAT_HEADER_MAGIC1)
	{
		UE_LOG(LogChatProtocols, Log, TEXT("Incorrect magic bytes: %d %d"), (int32)Magic0, (int32)Magic1);
		return false;
	}

	// Read DataSize [2,5]
	uint32 DataSize = flatbuffers::ReadScalar<uint32>(&Buf[2]);

	if (BufSize - CHAT_HEADER_SIZE < DataSize)
	{
		// Incomplete data
		OutDataSize = 0;
	}
	else
	{
		OutDataSize = DataSize;
	}
	return true;
}

bool FChatProtocols::GetCOMMANDApiData(const uint8* Buf, size_t BufSize, uint8*& OutPointer)
{
	auto COMMAND = flatbuffers::GetRoot<flatbuffers::Table>(Buf);

	OutPointer = (uint8*)COMMAND->GetAddressOf(OVERLORD::COMMAND::VT_API);

	return true;
}
