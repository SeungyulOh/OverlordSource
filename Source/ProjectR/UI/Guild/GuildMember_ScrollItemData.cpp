// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildMember_ScrollItemData.h"

#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/UI_Common/UP_CommonPopup.h"



void UGuildMember_ScrollItemData::SetGuildMemberData(UObject* WorldContextObject, UTextBlock* MemberName, UTextBlock* MemberLevel, UWidgetSwitcher* MamberGrade, UTextBlock* MemberContribute,
										UTextBlock* Text_Day, UTextBlock* Text_DayWord, UTextBlock* Text_Hour, UTextBlock* Text_HourWord, UTextBlock* Text_Minute, UTextBlock* Text_MinuteWord)
{
	if (WorldContextObject == nullptr)
		return;
	if (!IsValid(MemberName) || !IsValid(MemberLevel) || !IsValid(MamberGrade) || !IsValid(MemberContribute))
		return;
	if (!IsValid(Text_Day) || !IsValid(Text_DayWord) || !IsValid(Text_Hour) || !IsValid(Text_HourWord) || !IsValid(Text_Minute) || !IsValid(Text_MinuteWord))
		return;

	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

	MemberName->SetText(FText::FromString(GuildMemberData.nick));
	MemberLevel->SetText(FText::AsNumber(GuildMemberData.level));
	MemberContribute->SetText(FText::AsNumber(GuildMemberData.point));
	

	switch (GuildMemberData.grade)
	{
	case  EGUILD_GRADE::MEMBER: MamberGrade->SetActiveWidgetIndex(2);	break;
	case  EGUILD_GRADE::ELDER:	MamberGrade->SetActiveWidgetIndex(1);	break;
	case  EGUILD_GRADE::MASTER:	MamberGrade->SetActiveWidgetIndex(0);	break;
	}


	int64 InTicks = ETimespan::TicksPerSecond * GuildMemberData.last_update;
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
}

void UGuildMember_ScrollItemData::SetGuildMemberButton(UObject* WorldContextObject, UButton* GradeUp, UButton* GradeDown, UButton* MemberKick, UButton* ChangeMaster, UButton* AddFriend, UButton* ViewInfo, UButton* SendMail)
{
	if (WorldContextObject == nullptr)
		return;
	if (!IsValid(GradeUp) || !IsValid(GradeDown) || !IsValid(MemberKick) || !IsValid(ChangeMaster) || !IsValid(AddFriend) || !IsValid(ViewInfo) || !IsValid(SendMail))
		return;

	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

	AddFriend->SetVisibility(ESlateVisibility::Visible);
	ViewInfo->SetVisibility(ESlateVisibility::Visible);
	SendMail->SetVisibility(ESlateVisibility::Visible);

	ESlateVisibility GradeUpVisibility = ESlateVisibility::Collapsed;
	ESlateVisibility GradeDownVisibility = ESlateVisibility::Collapsed;
	ESlateVisibility KickVisibility = ESlateVisibility::Collapsed;
	ESlateVisibility MasterVisibility = ESlateVisibility::Collapsed;

	if (MyGuildGrade == EGUILD_GRADE::MASTER)
	{
		if (GuildMemberData.grade == EGUILD_GRADE::MEMBER)
			GradeUpVisibility = ESlateVisibility::Visible;
		if (GuildMemberData.grade == EGUILD_GRADE::ELDER)
			GradeDownVisibility = ESlateVisibility::Visible;

		MasterVisibility = ESlateVisibility::Visible;
		KickVisibility = ESlateVisibility::Visible;
	}
	else if (MyGuildGrade == EGUILD_GRADE::ELDER)
	{
		if (GuildMemberData.grade == EGUILD_GRADE::MEMBER)
			KickVisibility = ESlateVisibility::Visible;
	}

	GradeUp->SetVisibility(GradeUpVisibility);
	GradeDown->SetVisibility(GradeDownVisibility);
	MemberKick->SetVisibility(KickVisibility);
	ChangeMaster->SetVisibility(MasterVisibility);
}

void UGuildMember_ScrollItemData::OnClick_SetVisiblePanel(UObject* WorldContextObject, UCanvasPanel* Panel, ESlateVisibility PanelVisibility)
{
	if (WorldContextObject == nullptr)
		return;
	if (!IsValid(Panel))
		return;

	PopupPanel = Panel;

	if (GuildMemberData.kId == MyAccountKid)
		return;

	SetVisiblePanel(PanelVisibility);
}

void UGuildMember_ScrollItemData::SetVisiblePanel(ESlateVisibility PanelVisibility)
{
	if (PopupPanel)
	{
		PopupPanel->SetVisibility(PanelVisibility);
	}
}

void UGuildMember_ScrollItemData::OnClick_GradeUp(UObject* WorldContextObject)
{
	SetVisiblePanel(ESlateVisibility::Collapsed);

	if (WorldContextObject == nullptr)
		return;
	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	if (GuildMemberData.grade != EGUILD_GRADE::MEMBER)
		return;

	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(GuildMemberData.kId, (int32)EGUILD_GRADE::ELDER);
}

void UGuildMember_ScrollItemData::OnClick_GradeDown(UObject* WorldContextObject)
{
	SetVisiblePanel(ESlateVisibility::Collapsed);

	if (WorldContextObject == nullptr)
		return;
	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	if (GuildMemberData.grade != EGUILD_GRADE::ELDER)
		return;

	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(GuildMemberData.kId, (int32)EGUILD_GRADE::MEMBER);
}

void UGuildMember_ScrollItemData::OnClick_Kick(UObject* WorldContextObject)
{
	SetVisiblePanel(ESlateVisibility::Collapsed);

	if (WorldContextObject == nullptr)
		return;
	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	FFormatArgumentValue FormatArg = FText::FromString(GuildMemberData.nick);
	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error10"));
	LocalizedText = FText::Format(LocalizedText, FormatArg);

	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
		LocalizedText,
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(UP_CommonPopup))
	{
		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UGuildMember_ScrollItemData::MemberKick);
		UP_CommonPopup->OnClickOK.AddDynamic(this, &UGuildMember_ScrollItemData::MemberKick);
	}
}

void UGuildMember_ScrollItemData::MemberKick()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	UPacketFunctionLibrary::GUILD_MEMBER_KICK_RQ(GuildMemberData.kId);
}

void UGuildMember_ScrollItemData::OnClick_ChangeMaster(UObject* WorldContextObject)
{
	SetVisiblePanel(ESlateVisibility::Collapsed);

	if (WorldContextObject == nullptr)
		return;
	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	FFormatArgumentValue FormatArg = FText::FromString(GuildMemberData.nick);
	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error13"));
	LocalizedText = FText::Format(LocalizedText, FormatArg);

	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
		LocalizedText,
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(UP_CommonPopup))
	{
		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UGuildMember_ScrollItemData::ChangeMaster);
		UP_CommonPopup->OnClickOK.AddDynamic(this, &UGuildMember_ScrollItemData::ChangeMaster);
	}
}

void UGuildMember_ScrollItemData::ChangeMaster()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(GuildMemberData.kId, (int32)EGUILD_GRADE::MASTER);
}

void UGuildMember_ScrollItemData::OnClick_AddFriend(UObject* WorldContextObject)
{
	SetVisiblePanel(ESlateVisibility::Collapsed);

	if (WorldContextObject == nullptr)
		return;
	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
}

void UGuildMember_ScrollItemData::OnClick_View(UObject* WorldContextObject)
{
	SetVisiblePanel(ESlateVisibility::Collapsed);

	if (WorldContextObject == nullptr)
		return;
	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
}

void UGuildMember_ScrollItemData::OnClick_Mail(UObject* WorldContextObject)
{
	SetVisiblePanel(ESlateVisibility::Collapsed);

	if (WorldContextObject == nullptr)
		return;
	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;
}