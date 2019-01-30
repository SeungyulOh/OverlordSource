// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildAwaiter_ScrollItemData.h"

#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"


void UGuildAwaiter_ScrollItemData::SetAwaiterData(UObject* WorldContextObject, UTextBlock* MemberName, UTextBlock* MemberLevel, UButton* BtnApprove, UButton* BtnReject,
	UTextBlock* Text_Day, UTextBlock* Text_DayWord, UTextBlock* Text_Hour, UTextBlock* Text_HourWord, UTextBlock* Text_Minute, UTextBlock* Text_MinuteWord)
{
	if (WorldContextObject == nullptr)
		return;
	if (!IsValid(MemberName) || !IsValid(MemberLevel) || !IsValid(BtnApprove) || !IsValid(BtnReject))
		return;
	if (!IsValid(Text_Day) || !IsValid(Text_DayWord) || !IsValid(Text_Hour) || !IsValid(Text_HourWord) || !IsValid(Text_Minute) || !IsValid(Text_MinuteWord))
		return;

	MemberName->SetText(FText::FromString(GuildAwaiterData.nick));
	MemberLevel->SetText(FText::AsNumber(GuildAwaiterData.level));
	
	if (MyGuildGrade == EGUILD_GRADE::MEMBER)
	{
		BtnApprove->SetVisibility(ESlateVisibility::Collapsed);
		BtnReject->SetVisibility(ESlateVisibility::Collapsed); 
	}
	else
	{
		BtnApprove->SetVisibility(ESlateVisibility::Visible);
		BtnReject->SetVisibility(ESlateVisibility::Visible);
	}

	/*
	int64 InTicks = ETimespan::TicksPerSecond * GuildMamberData.last_update;
	bool LessOneMinute = true;
	FTimespan RemainTimespan = FTimespan(InTicks);
	int32 CheckDay = RemainTimespan.GetDays();
	if (Text_Day && Text_DayWord)
	{
		if (0 < CheckDay)
		{
			Text_Day->SetText(FText::AsNumber(CheckDay));
			Text_Day->SetVisibility(ESlateVisibility::Visible);
			Text_DayWord->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
			Text_Day->SetVisibility(ESlateVisibility::Collapsed);
			Text_DayWord->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckHour = RemainTimespan.GetHours();
	if (Text_Hour && Text_HourWord)
	{
		if (0 < CheckHour)
		{
			Text_Hour->SetText(FText::AsNumber(CheckHour));
			Text_Hour->SetVisibility(ESlateVisibility::Visible);
			Text_HourWord->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
			Text_Hour->SetVisibility(ESlateVisibility::Collapsed);
			Text_HourWord->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckMinute = RemainTimespan.GetMinutes();
	if (Text_Minute && Text_MinuteWord)
	{
		if (LessOneMinute)
		{
			if (0 == CheckMinute)
			{
				CheckMinute = 1;
			}
		}

		if (0 < CheckMinute)
		{
			Text_Minute->SetText(FText::AsNumber(CheckMinute));
			Text_Minute->SetVisibility(ESlateVisibility::Visible);
			Text_MinuteWord->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Text_Minute->SetVisibility(ESlateVisibility::Collapsed);
			Text_MinuteWord->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	*/
}

void UGuildAwaiter_ScrollItemData::OnClick_AWaiterAccept(UObject* WorldContextObject, bool accept)
{
	if (WorldContextObject == nullptr)
		return;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
	
	UPacketFunctionLibrary::GUILD_AWAITER_ACCEPT_RQ(GuildAwaiterData.kId, accept);
}