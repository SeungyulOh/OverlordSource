// Copyright(C) 2018 KOKOMO Games. All rights reserved.

#include "ProjectR.h"
#include "UC_HUD_Header_Areana.h"


void UUC_HUD_Header_Areana::NativeConstruct()
{
	Super::NativeConstruct();

	NickName.Reset();
	GuildName.Reset();

	for (int32 i = 0; i < 2; i++)
	{
		NickName.Emplace(TEXT(""));
		GuildName.Emplace(TEXT(""));
	}
}

void UUC_HUD_Header_Areana::InitAvatarInfo(const FString& nick, const FString& guild, bool isMe)
{
	NickName[(int32)isMe] = nick;
	GuildName[(int32)isMe] = guild;
}

void UUC_HUD_Header_Areana::SetAvatarInfo(bool isMe)
{
	variables.TB_AvatarNames[(int32)isMe]->SetText(FText::FromString(NickName[(int32)isMe]));
	variables.TB_GuildNames[(int32)isMe]->SetText(FText::FromString(GuildName[(int32)isMe]));
}
