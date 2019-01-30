 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_Grade.h"
#include "GlobalIntegrated.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "Global/EventManager.h"

#define DEF_SELECT_MASTER_INDEX 0
#define DEF_SELECT_ELDER_INDEX 1
#define DEF_SELECT_MEMBER_INDEX 2

void UUC_Guild_Grade::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<UImage>(this, SelectImageList, TEXT("Select_"));

	ButtonMaster->OnClicked.AddDynamic(this, &UUC_Guild_Grade::OnClick_MasterBtn);
	ButtonElder->OnClicked.AddDynamic(this, &UUC_Guild_Grade::OnClick_ElderBtn);
	ButtonMember->OnClicked.AddDynamic(this, &UUC_Guild_Grade::OnClick_MemberBtn);

	Button_OK->OnClicked.AddDynamic(this, &UUC_Guild_Grade::OnClickGradeChange);
	Button_Cancel->OnClicked.AddDynamic(this, &UUC_Guild_Grade::CloseGrade);

	RGAMEINSTANCE(this)->EventManager->AfterChangeGrade.RemoveDynamic(this, &UUC_Guild_Grade::GradeChangeRp);
	RGAMEINSTANCE(this)->EventManager->AfterChangeGrade.AddDynamic(this, &UUC_Guild_Grade::GradeChangeRp);
}

void UUC_Guild_Grade::NativeDestruct()
{
	ButtonMaster->OnClicked.RemoveDynamic(this, &UUC_Guild_Grade::OnClick_MasterBtn);
	ButtonElder->OnClicked.RemoveDynamic(this, &UUC_Guild_Grade::OnClick_ElderBtn);
	ButtonMember->OnClicked.RemoveDynamic(this, &UUC_Guild_Grade::OnClick_MemberBtn);

	Button_OK->OnClicked.RemoveDynamic(this, &UUC_Guild_Grade::OnClickGradeChange);
	Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Guild_Grade::CloseGrade);

	Super::NativeDestruct();
}

void UUC_Guild_Grade::OpenGrade(uint8 InMyGuildGrade, int32 Inkid, const FString& InNick)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	CurrGrade = InMyGuildGrade;
	ChangeGrade = InMyGuildGrade;
	kid = Inkid;
	nick = InNick;

	this->SetVisibility(ESlateVisibility::Visible);
	FText Content = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UP_Guild_Management_PositionChange02");
	FString Type = "Gem";

	FFormatOrderedArguments Args;
	Args.Emplace(FText::FromString(nick));
	Text_Desc->SetText(FText::Format(Content, Args));

	RefreshSelectImage(ChangeGrade);
}

void UUC_Guild_Grade::CloseGrade()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Guild_Grade::OnClickGrade(uint8 grade)
{
	ChangeGrade = grade;
	RefreshSelectImage(ChangeGrade);
}

void UUC_Guild_Grade::OnClickGradeChange()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (CurrGrade == ChangeGrade)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_GuildReport_GradeUpError")), FText());
		return;
	}

	if (ChangeGrade == (uint8)EGUILD_GRADE::MASTER)
	{
		FFormatArgumentValue FormatArg = FText::FromString(nick);
		FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error13"));
		LocalizedText = FText::Format(LocalizedText, FormatArg);

		auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			LocalizedText, ECommonPopupType::VE_OKCANCEL);
		UP_CommonPopup->LocalizingTextBlock_OK->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Guild_Management_TransferButton")));
		UP_CommonPopup->LocalizingTextBlock_Cancel->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRaid_0032")));
		if (IsValid(UP_CommonPopup))
		{
			UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UUC_Guild_Grade::GradeTransfer);
			UP_CommonPopup->OnClickOK.AddDynamic(this, &UUC_Guild_Grade::GradeTransfer);
		}
	}
	else
	{
		GradeTransfer();
	}
}

void UUC_Guild_Grade::GradeChangeRp(FString _nick, int32 grade)
{
	FFormatOrderedArguments Args;
	Args.Add(FText::FromString(_nick));
	Args.Add(UResourceFunctionLibrary::GetGuildGradeTypeName((EGUILD_GRADE)grade));
	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_GuildReport_GradeUp"));
	LocalizedText = FText::Format(LocalizedText, Args);
	UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), LocalizedText);

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Guild_Grade::OnClick_MasterBtn()
{
	OnClickGrade(10);
}

void UUC_Guild_Grade::OnClick_ElderBtn()
{
	OnClickGrade(7);
}

void UUC_Guild_Grade::OnClick_MemberBtn()
{
	OnClickGrade(5);
}

void UUC_Guild_Grade::GradeTransfer()
{
	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(kid, (int32)ChangeGrade);
}

void UUC_Guild_Grade::RefreshSelectImage(uint8 grade)
{
	if ((EGUILD_GRADE)grade == EGUILD_GRADE::MASTER)
	{
		SelectImageList[DEF_SELECT_MASTER_INDEX]->SetVisibility(ESlateVisibility::Visible);
		SelectImageList[DEF_SELECT_ELDER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
		SelectImageList[DEF_SELECT_MEMBER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if ((EGUILD_GRADE)grade == EGUILD_GRADE::ELDER)
	{
		SelectImageList[DEF_SELECT_MASTER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
		SelectImageList[DEF_SELECT_ELDER_INDEX]->SetVisibility(ESlateVisibility::Visible);
		SelectImageList[DEF_SELECT_MEMBER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SelectImageList[DEF_SELECT_MASTER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
		SelectImageList[DEF_SELECT_ELDER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
		SelectImageList[DEF_SELECT_MEMBER_INDEX]->SetVisibility(ESlateVisibility::Visible);
	}
}

// void UUC_Guild_Grade::OnGuildHistoryRpDelegate()
// {
// 	this->SetVisibility(ESlateVisibility::Visible);
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (!IsValid(ScrollView_HistoryList))
// 		return;
// 
// 	ScrollView_HistoryList->ClearScrollItemData();
// 	ScrollView_HistoryList->SetVisibility(ESlateVisibility::Visible);
// 
// 	TArray<FGUILD_HISTORY> GuildHistoryList = RGameInstance->GuildInfo->HistoryList;
// 	
// 	for (FGUILD_HISTORY& GuildHistory : GuildHistoryList)
// 	{
// 		UGuildHistory_ScrollItemData* ScrollItemData = Cast<UGuildHistory_ScrollItemData>(ScrollView_HistoryList->AddScrollItemData(UGuildHistory_ScrollItemData::StaticClass()));
// 		if (ScrollItemData)
// 		{
// 			ScrollItemData->GuildHistory = GuildHistory;
// 		}
// 	}
// 
// 	ScrollView_HistoryList->InvalidateData();
// }
