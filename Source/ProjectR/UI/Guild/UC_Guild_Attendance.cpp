 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_Attendance.h"
#include "GlobalIntegrated.h"


#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Common/UC_Item_Icons.h"

void UUC_Guild_Attendance::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->EventManager->OnRefreshGuildAttendanceUI.AddDynamic(this, &UUC_Guild_Attendance::OnRefreshGuildAttendanceUI);
	}
}

void UUC_Guild_Attendance::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->EventManager->OnRefreshGuildAttendanceUI.RemoveDynamic(this, &UUC_Guild_Attendance::OnRefreshGuildAttendanceUI);
	}

	Super::NativeDestruct();
}

void UUC_Guild_Attendance::OpenAttendance()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UPacketFunctionLibrary::GUILD_ATTENDANCE_STATUS_RQ();
}

void UUC_Guild_Attendance::CloseAttendance()
{
	this->SetVisibility(ESlateVisibility::Hidden);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	RGameInstance->EventManager->OnRequestGuildInfo.Broadcast();
}

void UUC_Guild_Attendance::OnClick_DailyReward()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (RGameInstance->GuildInfo->Attendance_dailyRewardable == EREWARDABLE_STATUS::REWARDED)
		return;

	UPacketFunctionLibrary::GUILD_ATTENDANCE_DAILY_REWARD_RQ();
}

void UUC_Guild_Attendance::OnClick_WeeklyReward()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UPacketFunctionLibrary::GUILD_ATTENDANCE_WEEKLY_REWARD_RQ();
}

void UUC_Guild_Attendance::OnRefreshGuildAttendanceUI()
{
	this->SetVisibility(ESlateVisibility::Visible);

	BP_RefreshAttendance();
}

void UUC_Guild_Attendance::RefreshAttendanceDaily(UWidgetSwitcher* CompleteSwitcher)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	ESlateVisibility RewardVisible = ESlateVisibility::Collapsed;

	if (CompleteSwitcher)
	{
		if (RGameInstance->GuildInfo->Attendance_dailyRewardable == EREWARDABLE_STATUS::REWARDED)
		{
			RewardVisible = ESlateVisibility::SelfHitTestInvisible;
			CompleteSwitcher->SetActiveWidgetIndex(1);
		}
		else
		{
			CompleteSwitcher->SetActiveWidgetIndex(0);
		}
	}

	FName GuildLevelUpKey = FName(*FString::FromInt(RGameInstance->GuildInfo->MyGuildInfo.level));
	FGuildLevelUpTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildLevelUpRow(GuildLevelUpKey);
	if (GuildTableInfo == nullptr)
		return;

	TArray<FITEM_ICON_INFO> RewardsIconInfoArray;
	UUIFunctionLibrary::GetRewardsIconInfoByPreview(GuildTableInfo->attendanceDailyRewardKey, RewardsIconInfoArray);

	int32 MaxIndex = ItemIconArray.Num();
	for (int32 CheckIndex = 0; CheckIndex < MaxIndex; ++CheckIndex)
	{
		if (ReceiveImageArray.IsValidIndex(CheckIndex))
		{
			ReceiveImageArray[CheckIndex]->SetVisibility(RewardsIconInfoArray.IsValidIndex(CheckIndex) ? RewardVisible : ESlateVisibility::Collapsed);
		}

		if (ItemIconArray.IsValidIndex(CheckIndex))
		{
			UUC_Item_Icons* ItemIcon = ItemIconArray[CheckIndex];

			if (RewardsIconInfoArray.IsValidIndex(CheckIndex))
			{
				ItemIcon->SetItemIconStructure(RewardsIconInfoArray[CheckIndex]);
			}
		}
		
		if (ItemSizeBoxArray.IsValidIndex(CheckIndex))
		{
			USizeBox* ItemSizeBox = ItemSizeBoxArray[CheckIndex];

			if (RewardsIconInfoArray.IsValidIndex(CheckIndex))
			{
				ItemSizeBox->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				ItemSizeBox->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUC_Guild_Attendance::RefreshAttendanceWeekly(UTextBlock* Text_Day, UTextBlock* Text_DayWord, UTextBlock* Text_Hour, UTextBlock* Text_HourWord,
	UTextBlock* Text_Minute, UTextBlock* Text_MinuteWord, UTextBlock* Text_WeekCount, UProgressBar* Progress_AttendanceCount, UButton* Btn_WeeklyAttendance)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (Text_WeekCount)
	{
		Text_WeekCount->SetText(FText::AsNumber(RGameInstance->GuildInfo->Attendance_weeklyCount));
	}
	if (Progress_AttendanceCount)
	{
		float CalcPercent = 0.0f;
		if (0 < RGameInstance->GuildInfo->Attendance_weeklyCount)
		{
			CalcPercent = (float)RGameInstance->GuildInfo->Attendance_weeklyCount / 300.0f;
		}
		Progress_AttendanceCount->SetPercent(CalcPercent);
	}
	if (Btn_WeeklyAttendance)
	{
		Btn_WeeklyAttendance->SetIsEnabled((RGameInstance->GuildInfo->Attendance_weeklyRewardable == EREWARDABLE_STATUS::READY));
	}

	int64 InTicks = ETimespan::TicksPerSecond * RGameInstance->GuildInfo->Attendance_weeklyTimeLeft;
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


	TArray<FName> WeeklyRewardKeyArray;
	WeeklyRewardKeyArray.Reserve(5);
	WeeklyRewardKeyArray.Emplace(FName("RGU000210"));
	WeeklyRewardKeyArray.Emplace(FName("RGU000220"));
	WeeklyRewardKeyArray.Emplace(FName("RGU000230"));
	WeeklyRewardKeyArray.Emplace(FName("RGU000240"));
	WeeklyRewardKeyArray.Emplace(FName("RGU000250"));

	TArray<FITEM_ICON_INFO> RewardsIconInfoArray;

	int32 MaxIndex = WeeklyItemIconArray.Num();
	for (int32 CheckIndex = 0; CheckIndex < MaxIndex; ++CheckIndex)
	{
		if (false == WeeklyItemIconArray.IsValidIndex(CheckIndex))
			continue;

		UUC_Item_Icons* ItemIcon = WeeklyItemIconArray[CheckIndex];
		if (false == IsValid(ItemIcon))
			continue;

		ESlateVisibility RewardVisibility = ESlateVisibility::Collapsed;

		if (WeeklyRewardKeyArray.IsValidIndex(CheckIndex))
		{
			UUIFunctionLibrary::GetRewardsIconInfoByPreview(WeeklyRewardKeyArray[CheckIndex], RewardsIconInfoArray);

			if (RewardsIconInfoArray.IsValidIndex(0))
			{
				ItemIcon->SetItemIconStructure(RewardsIconInfoArray[0]);
				RewardVisibility = ESlateVisibility::Visible;
			}
		}

		ItemIcon->SetVisibility(RewardVisibility);
	}
}
