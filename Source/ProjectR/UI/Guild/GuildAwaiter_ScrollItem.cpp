// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildAwaiter_ScrollItem.h"
#include "GuildAwaiter_ScrollItemData.h"
#include "Network/PacketFunctionLibrary.h"



void UGuildAwaiter_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	Button_View->OnClicked.AddDynamic(this, &UGuildAwaiter_ScrollItem::OnClick_Button_UserInfo);
	Button_Approve->OnClicked.AddDynamic(this, &UGuildAwaiter_ScrollItem::OnClick_Button_Approve);
	Button_Reject->OnClicked.AddDynamic(this, &UGuildAwaiter_ScrollItem::OnClick_Button_Reject);
}

void UGuildAwaiter_ScrollItem::NativeDestruct()
{
	Button_View->OnClicked.RemoveDynamic(this, &UGuildAwaiter_ScrollItem::OnClick_Button_UserInfo);
	Button_Approve->OnClicked.RemoveDynamic(this, &UGuildAwaiter_ScrollItem::OnClick_Button_Approve);
	Button_Reject->OnClicked.RemoveDynamic(this, &UGuildAwaiter_ScrollItem::OnClick_Button_Reject);

	Super::NativeDestruct();
}

void UGuildAwaiter_ScrollItem::InvalidateData()
{
	UGuildAwaiter_ScrollItemData* WaiterData = Cast<UGuildAwaiter_ScrollItemData>(ItemData);
	SetNickName(WaiterData->GuildAwaiterData.nick);
	SetLevel(WaiterData->GuildAwaiterData.level);
	SetLastAccessTime(WaiterData->GuildAwaiterData.elapsedSec);
}

void UGuildAwaiter_ScrollItem::OnClick_Button_UserInfo()
{

}

void UGuildAwaiter_ScrollItem::OnClick_Button_Approve()
{
	UGuildAwaiter_ScrollItemData* WaiterData = Cast<UGuildAwaiter_ScrollItemData>(ItemData);
	if (WaiterData)
	{
		UPacketFunctionLibrary::GUILD_AWAITER_ACCEPT_RQ(WaiterData->GuildAwaiterData.kId, true);
	}
}

void UGuildAwaiter_ScrollItem::OnClick_Button_Reject()
{
	UGuildAwaiter_ScrollItemData* WaiterData = Cast<UGuildAwaiter_ScrollItemData>(ItemData);
	if (WaiterData)
	{
		UPacketFunctionLibrary::GUILD_AWAITER_ACCEPT_RQ(WaiterData->GuildAwaiterData.kId, false);
	}
}

void UGuildAwaiter_ScrollItem::SetNickName(FString nick)
{
	Text_NickName->SetText(FText::FromString(nick));
}

void UGuildAwaiter_ScrollItem::SetLevel(int32 level)
{
	Text_Level->SetText(FText::AsNumber(level));
}

void UGuildAwaiter_ScrollItem::SetLastAccessTime(int32 elapseTime)
{
	int64 InTicks = ETimespan::TicksPerSecond * elapseTime;
	bool LessOneMinute = true;
	FTimespan RemainTimespan = FTimespan(InTicks);
	int32 CheckDay = RemainTimespan.GetDays();
	if (Text_Day && Text_Day_Unit)
	{
		if (0 < CheckDay)
		{
			Text_Day->SetText(FText::AsNumber(CheckDay));
			Text_Day->SetVisibility(ESlateVisibility::Visible);
			Text_Day_Unit->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
			Text_Day->SetVisibility(ESlateVisibility::Collapsed);
			Text_Day_Unit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckHour = RemainTimespan.GetHours();
	if (Text_Hour && Text_Hour_Unit)
	{
		if (0 < CheckHour)
		{
			Text_Hour->SetText(FText::AsNumber(CheckHour));
			Text_Hour->SetVisibility(ESlateVisibility::Visible);
			Text_Hour_Unit->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
			Text_Hour->SetVisibility(ESlateVisibility::Collapsed);
			Text_Hour_Unit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckMinute = RemainTimespan.GetMinutes();
	if (Text_Minute && Text_Minute_Unit)
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
			Text_Minute_Unit->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Text_Minute->SetVisibility(ESlateVisibility::Collapsed);
			Text_Minute_Unit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
