// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GuildManagerListScrollItem.h"

#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "GuildManagerListScrollItemData.h"
#include "UI/Avatar/Item/UC_ScrollItem_AvatarIcon.h"
#include "UI/UI_Common/UP_CommonPopup.h"


void UGuildManagerListScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Banish->OnClicked.AddDynamic(this, &UGuildManagerListScrollItem::OnClick_Kick);
	BtnGrade->OnClicked.AddDynamic(this, &UGuildManagerListScrollItem::OnClick_Grade);
}

void UGuildManagerListScrollItem::NativeDestruct()
{
	Btn_Banish->OnClicked.RemoveDynamic(this, &UGuildManagerListScrollItem::OnClick_Kick);
	BtnGrade->OnClicked.RemoveDynamic(this, &UGuildManagerListScrollItem::OnClick_Grade);
	Super::NativeDestruct();
}

void UGuildManagerListScrollItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGuildManagerListScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UGuildManagerListScrollItemData* pGuildData = Cast<UGuildManagerListScrollItemData>(item);
	if (!IsValid(TextBlockLevel) || !IsValid(TextBlockName) || !IsValid(TextBlockContribute))
		return;
	GuildMemberData = pGuildData->GuildMemberData;
	TextBlockName->SetText(FText::FromString(GuildMemberData.nick));
	TextBlockLevel->SetText(FText::AsNumber(GuildMemberData.level));
	TextBlockContribute->SetText(FText::AsNumber(GuildMemberData.point));
	
	int64 InTicks = ETimespan::TicksPerSecond * GuildMemberData.last_update;
	bool LessOneMinute = true;
	FTimespan RemainTimespan = FTimespan(InTicks);
	int32 CheckDay = RemainTimespan.GetDays();
	if (TextBlockDay && LocalizingTextBlockDay)
	{
		if (0 < CheckDay)
		{
			TextBlockDay->SetText(FText::AsNumber(CheckDay));
			TextBlockDay->SetVisibility(ESlateVisibility::Visible);
			LocalizingTextBlockDay->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
			TextBlockDay->SetVisibility(ESlateVisibility::Collapsed);
			LocalizingTextBlockDay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckHour = RemainTimespan.GetHours();
	if (TextBlockHour && LocalizingTextBlockHour)
	{
		if (0 < CheckHour)
		{
			TextBlockHour->SetText(FText::AsNumber(CheckHour));
			TextBlockHour->SetVisibility(ESlateVisibility::Visible);
			LocalizingTextBlockHour->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
			TextBlockHour->SetVisibility(ESlateVisibility::Collapsed);
			LocalizingTextBlockHour->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckMinute = RemainTimespan.GetMinutes();
	if (TextBlockMinute && LocalizingTextBlockMinute)
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
			TextBlockMinute->SetText(FText::AsNumber(CheckMinute));
			TextBlockMinute->SetVisibility(ESlateVisibility::Visible);
			LocalizingTextBlockMinute->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			TextBlockMinute->SetVisibility(ESlateVisibility::Collapsed);
			LocalizingTextBlockMinute->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	GradeIconSwitcher->SetVisibility(ESlateVisibility::Visible);
	if (GuildMemberData.grade == EGUILD_GRADE::MASTER)
	{
		GradeIconSwitcher->SetActiveWidgetIndex(0);
		AvatarIcon->SetGuildGradeIcon(0);
	}
	else if (GuildMemberData.grade == EGUILD_GRADE::ELDER)
	{
		GradeIconSwitcher->SetActiveWidgetIndex(1);
		AvatarIcon->SetGuildGradeIcon(1);
	}
	else
	{
		GradeIconSwitcher->SetVisibility(ESlateVisibility::Collapsed);
		AvatarIcon->SetGuildGradeIcon(2);
	}
	if (pGuildData->IsBtnOn && pGuildData->MyAccountKid != pGuildData->GuildMemberData.kId)
	{
		GradeBox->SetVisibility(ESlateVisibility::Visible);
		BanishBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GradeBox->SetVisibility(ESlateVisibility::Collapsed);
		BanishBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (pGuildData->MyAccountKid != GuildMemberData.kId)
		backgroundSwitcher->SetActiveWidgetIndex(0);
	else
		backgroundSwitcher->SetActiveWidgetIndex(1);
// 	GuildMemberData
// 	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
// 
// 	if (AvatarIcon)
// 	{
// 		//todo avataricon table
// 		FName NameID;
// 		int32 TestValue = Avatar.icon - 1;
// 		if (TestValue == 0)
// 		{
// 			NameID = "PT_Avatar_Default";
// 		}
// 		else
// 		{
// 			auto ID = FString::FromInt(TestValue);
// 			auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(FName(*ID));
// 			NameID = Table->PortraitTextureKey;
// 		}
// 		auto temp = AvatarIcon->Image_Hero;
// 		if (temp)
// 		{
// 			auto Image = UUIFunctionLibrary::GetIconImageFromTable(NameID);
// 			UUIFunctionLibrary::SetImageBrush(Image, temp);
// 		}
// 	}
}

void UGuildManagerListScrollItem::OnClick_Grade()
{
	UGuildManagerListScrollItemData* pGuildData = Cast<UGuildManagerListScrollItemData>(ItemData);
	uint8 grade = 0;

	if (pGuildData->MyAccountKid != GuildMemberData.kId)
	{
		if (pGuildData->MyGuildGrade == EGUILD_GRADE::MASTER)
		{
			RGAMEINSTANCE(this)->EventManager->OnOpenGradeChangePop.Broadcast((uint8)pGuildData->GuildMemberData.grade, pGuildData->GuildMemberData.kId, pGuildData->GuildMemberData.nick);
		}
		else
		{
		}
	}
}

void UGuildManagerListScrollItem::OnClick_Kick()
{
	FFormatArgumentValue FormatArg = FText::FromString(GuildMemberData.nick);
	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error10"));
	LocalizedText = FText::Format(LocalizedText, FormatArg);

	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
		LocalizedText,
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(UP_CommonPopup))
	{
		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UGuildManagerListScrollItem::MemberKick);
		UP_CommonPopup->OnClickOK.AddDynamic(this, &UGuildManagerListScrollItem::MemberKick);
	}
}

void UGuildManagerListScrollItem::MemberKick()
{
	UPacketFunctionLibrary::GUILD_MEMBER_KICK_RQ(GuildMemberData.kId);
}

// 
// void GuildManagerListScrollItem::SetGuildMemberData(UObject* WorldContextObject, UTextBlock* MemberName, UTextBlock* MemberLevel, UTextBlock* MemberContribute,
// 										UTextBlock* Text_Hour, UTextBlock* Text_HourWord)
// {
// 	if (WorldContextObject == nullptr)
// 		return;
// 	if (!IsValid(MemberName) || !IsValid(MemberLevel) || !IsValid(MemberContribute) || !IsValid(Text_Hour) || !IsValid(Text_HourWord))
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	MemberName->SetText(FText::FromString(GuildMemberData.nick));
// 	MemberLevel->SetText(FText::AsNumber(GuildMemberData.level));
// 	MemberContribute->SetText(FText::AsNumber(GuildMemberData.point));
// // 	switch (GuildMemberData.grade)
// // 	{
// // 	case  EGUILD_GRADE::MEMBER: MamberGrade->SetActiveWidgetIndex(2);	break;
// // 	case  EGUILD_GRADE::ELDER:	MamberGrade->SetActiveWidgetIndex(1);	break;
// // 	case  EGUILD_GRADE::MASTER:	MamberGrade->SetActiveWidgetIndex(0);	break;
// // 	}
// 	int64 InTicks = ETimespan::TicksPerSecond * GuildMemberData.last_update;
// 	bool LessOneMinute = true;
// 	FTimespan RemainTimespan = FTimespan(InTicks);
// // 	int32 CheckDay = RemainTimespan.GetDays();
// // 	if (Text_Day && Text_DayWord)
// // 	{
// // 		if (0 < CheckDay)
// // 		{
// // 			Text_Day->SetText(FText::AsNumber(CheckDay));
// // 			Text_Day->SetVisibility(ESlateVisibility::Visible);
// // 			Text_DayWord->SetVisibility(ESlateVisibility::Visible);
// // 			LessOneMinute = false;
// // 		}
// // 		else
// // 		{
// // 			Text_Day->SetVisibility(ESlateVisibility::Collapsed);
// // 			Text_DayWord->SetVisibility(ESlateVisibility::Collapsed);
// // 		}
// // 	}
// 	int32 CheckHour = RemainTimespan.GetHours();
// 	if (Text_Hour && Text_HourWord)
// 	{
// 		if (0 < CheckHour)
// 		{
// 			Text_Hour->SetText(FText::AsNumber(CheckHour));
// 			Text_Hour->SetVisibility(ESlateVisibility::Visible);
// 			Text_HourWord->SetVisibility(ESlateVisibility::Visible);
// 			LessOneMinute = false;
// 		}
// 		else
// 		{
// 			Text_Hour->SetVisibility(ESlateVisibility::Collapsed);
// 			Text_HourWord->SetVisibility(ESlateVisibility::Collapsed);
// 		}
// 	}
// // 	int32 CheckMinute = RemainTimespan.GetMinutes();
// // 	if (Text_Minute && Text_MinuteWord)
// // 	{
// // 		if (LessOneMinute)
// // 		{
// // 			if (0 == CheckMinute)
// // 			{
// // 				CheckMinute = 1;
// // 			}
// // 		}
// // 
// // 		if (0 < CheckMinute)
// // 		{
// // 			Text_Minute->SetText(FText::AsNumber(CheckMinute));
// // 			Text_Minute->SetVisibility(ESlateVisibility::Visible);
// // 			Text_MinuteWord->SetVisibility(ESlateVisibility::Visible);
// // 		}
// // 		else
// // 		{
// // 			Text_Minute->SetVisibility(ESlateVisibility::Collapsed);
// // 			Text_MinuteWord->SetVisibility(ESlateVisibility::Collapsed);
// // 		}
// // 	}
// }
// 
// void GuildManagerListScrollItem::SetGuildMemberButton(UObject* WorldContextObject, UButton* btnGrade, UButton* MemberKick)
// {
// 	if (WorldContextObject == nullptr)
// 		return;
// 	if (!IsValid(MemberKick) || !IsValid(btnGrade))
// 		return;
// 
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 
// 	//ViewInfo->SetVisibility(ESlateVisibility::Visible);
// 
// 	ESlateVisibility GradeUpVisibility = ESlateVisibility::Collapsed;
// 	ESlateVisibility GradeDownVisibility = ESlateVisibility::Collapsed;
// 	ESlateVisibility KickVisibility = ESlateVisibility::Collapsed;
// 	ESlateVisibility MasterVisibility = ESlateVisibility::Collapsed;
// 
// 	if (MyGuildGrade == EGUILD_GRADE::MASTER)
// 	{
// 		if (GuildMemberData.grade == EGUILD_GRADE::MEMBER)
// 			GradeUpVisibility = ESlateVisibility::Visible;
// 		if (GuildMemberData.grade == EGUILD_GRADE::ELDER)
// 			GradeDownVisibility = ESlateVisibility::Visible;
// 
// 		MasterVisibility = ESlateVisibility::Visible;
// 		KickVisibility = ESlateVisibility::Visible;
// 	}
// 	else if (MyGuildGrade == EGUILD_GRADE::ELDER)
// 	{
// 		if (GuildMemberData.grade == EGUILD_GRADE::MEMBER)
// 			KickVisibility = ESlateVisibility::Visible;
// 	}
// 
// 	MemberKick->SetVisibility(KickVisibility);
// }
// 
// void GuildManagerListScrollItem::OnClick_SetVisiblePanel(UObject* WorldContextObject, UCanvasPanel* Panel, ESlateVisibility PanelVisibility)
// {
// 	if (WorldContextObject == nullptr)
// 		return;
// 	if (!IsValid(Panel))
// 		return;
// 
// 	PopupPanel = Panel;
// 
// 	if (GuildMemberData.kId == MyAccountKid)
// 		return;
// 
// 	SetVisiblePanel(PanelVisibility);
// }
// 
// void GuildManagerListScrollItem::SetVisiblePanel(ESlateVisibility PanelVisibility)
// {
// 	if (PopupPanel)
// 	{
// 		PopupPanel->SetVisibility(PanelVisibility);
// 	}
// }
// 
// void GuildManagerListScrollItem::OnClick_Grade(UObject* WorldContextObject)
// {
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 	RGameInstance->RWidgetManager->ShowGuildGradePopup();
// }
// 
// void GuildManagerListScrollItem::OnClick_Kick(UObject* WorldContextObject)
// {
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 	RGameInstance->RWidgetManager->HideGuildGradePopup();
// }
// void UGuildManager_ScrollItemData::OnClick_GradeUp(UObject* WorldContextObject)
// {
// 	SetVisiblePanel(ESlateVisibility::Collapsed);
// 
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	if (GuildMemberData.grade != EGUILD_GRADE::MEMBER)
// 		return;
// 
// 	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(RGameInstance, GuildMemberData.kId, (int32)EGUILD_GRADE::ELDER);
// }
// 
// void UGuildManager_ScrollItemData::OnClick_GradeDown(UObject* WorldContextObject)
// {
// 	SetVisiblePanel(ESlateVisibility::Collapsed);
// 
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	if (GuildMemberData.grade != EGUILD_GRADE::ELDER)
// 		return;
// 
// 	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(RGameInstance, GuildMemberData.kId, (int32)EGUILD_GRADE::MEMBER);
// }
// 
// void UGuildManager_ScrollItemData::OnClick_Kick(UObject* WorldContextObject)
// {
// 	SetVisiblePanel(ESlateVisibility::Collapsed);
// 
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	FFormatArgumentValue FormatArg = FText::FromString(GuildMemberData.nick);
// 	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error10"));
// 	LocalizedText = FText::Format(LocalizedText, FormatArg);
// 
// 	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 		LocalizedText,
// 		ECommonPopupType::VE_OKCANCEL);
// 
// 	if (IsValid(UP_CommonPopup))
// 	{
// 		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UGuildManager_ScrollItemData::MemberKick);
// 		UP_CommonPopup->OnClickOK.AddDynamic(this, &UGuildManager_ScrollItemData::MemberKick);
// 	}
// }
// 
// void UGuildManager_ScrollItemData::MemberKick()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	UPacketFunctionLibrary::GUILD_MEMBER_KICK_RQ(RGameInstance, GuildMemberData.kId);
// }
// 
// void UGuildManager_ScrollItemData::OnClick_ChangeMaster(UObject* WorldContextObject)
// {
// 	SetVisiblePanel(ESlateVisibility::Collapsed);
// 
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	FFormatArgumentValue FormatArg = FText::FromString(GuildMemberData.nick);
// 	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error13"));
// 	LocalizedText = FText::Format(LocalizedText, FormatArg);
// 
// 	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 		LocalizedText,
// 		ECommonPopupType::VE_OKCANCEL);
// 
// 	if (IsValid(UP_CommonPopup))
// 	{
// 		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UGuildManager_ScrollItemData::ChangeMaster);
// 		UP_CommonPopup->OnClickOK.AddDynamic(this, &UGuildManager_ScrollItemData::ChangeMaster);
// 	}
// }
// 
// void UGuildManager_ScrollItemData::ChangeMaster()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(RGameInstance, GuildMemberData.kId, (int32)EGUILD_GRADE::MASTER);
// }
// 
// void UGuildManager_ScrollItemData::OnClick_AddFriend(UObject* WorldContextObject)
// {
// 	SetVisiblePanel(ESlateVisibility::Collapsed);
// 
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// }
// 
// void UGuildManager_ScrollItemData::OnClick_View(UObject* WorldContextObject)
// {
// 	SetVisiblePanel(ESlateVisibility::Collapsed);
// 
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// }
// 
// void UGuildManager_ScrollItemData::OnClick_Mail(UObject* WorldContextObject)
// {
// 	SetVisiblePanel(ESlateVisibility::Collapsed);
// 
// 	if (WorldContextObject == nullptr)
// 		return;
// 	OwnedWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// }