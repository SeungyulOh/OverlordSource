 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_Create.h"
#include "GlobalIntegrated.h"

#include "UI/RWidgetManager.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "GameInfo/UserInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/RWidgetManager.h"

#define DEF_SELECT_MASTER_INDEX 0
#define DEF_SELECT_ELDER_INDEX 1
#define DEF_SELECT_MEMBER_INDEX 2

void UUC_Guild_Create::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Guild_Create::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Guild_Create::Open()
{
	Init();
	this->SetVisibility(ESlateVisibility::Visible);
	Create_Backscreen->SetVisibility(ESlateVisibility::Visible);
	Create_PopupPanel->SetVisibility(ESlateVisibility::Visible);
}

void UUC_Guild_Create::Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	Create_Backscreen->SetVisibility(ESlateVisibility::Collapsed);
	Create_PopupPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Guild_Create::Init()
{
	Create_GuildName->SetText(FText());
	Create_GuildDesc->SetText(FText());
	Create_JoinTypeConfirm->SetCheckedState(ECheckBoxState::Checked);
	Create_JoinTypeAuto->SetCheckedState(ECheckBoxState::Unchecked);

	GuildCreateGold = 0;
	if (UC_CurrencyDisplay)
	{
		FName GuildLevelUpKey = FName(*FString::FromInt(1));
		FGuildLevelUpTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildLevelUpRow(GuildLevelUpKey);
		if (GuildTableInfo)
		{
			GuildCreateGold = GuildTableInfo->gold;
		}
		UC_CurrencyDisplay->SetCurrency(WALLET_GOLD, GuildCreateGold);
	}
}

void UUC_Guild_Create::OnClickCancel()
{
	Close();
}

void UUC_Guild_Create::OnClickCreate()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (Create_GuildName->GetText().IsEmpty())
	{
		UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Search")),
			ECommonPopupType::VE_OK);
		return;
	}

// 	if (RGameInstance->RInventory->GetGold() < GuildCreateGold)
// 	{
// 		RGameInstance->RWidgetManager->ShowCurrencyShortagePopup(
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")), 
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error5")), 
// 			EREWARD_TYPE::GOLD, GuildCreateGold - RGameInstance->RInventory->GetGold());
//
// 		UUIFunctionLibrary::ShowCommonPopup(
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error5")),
// 			ECommonPopupType::VE_OK);
// 		return;
// 	}

	bool isAuto = Create_JoinTypeAuto->IsChecked();

	UPacketFunctionLibrary::GUILD_CREATE_RQ(Create_GuildName->GetText().ToString(), Create_GuildDesc->GetText().ToString(), "", isAuto);
	Close();
}

void UUC_Guild_Create::OnClickJoinType(int32 iAuto)
{
	if (iAuto == 0) {
		Create_JoinTypeAuto->SetCheckedState(ECheckBoxState::Checked);
		Create_JoinTypeConfirm->SetCheckedState(ECheckBoxState::Unchecked);
	} else {
		Create_JoinTypeAuto->SetCheckedState(ECheckBoxState::Unchecked);
		Create_JoinTypeConfirm->SetCheckedState(ECheckBoxState::Checked);
	}
}

// void UUC_Guild_Create::OpenGrade(uint8 InMyGuildGrade, int32 Inkid, const FString& InNick, uint8 InGuildGrade)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 	
// // 	MyGuildGrade = InMyGuildGrade;
// // 	kid = Inkid;
// // 	nick = InNick;
// // 	GuildGrade = InGuildGrade;
// // 
// // 	this->SetVisibility(ESlateVisibility::Visible);
// // 	FText Content = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UP_Guild_Management_PositionChange02");
// // 	FString Type = "Gem";
// // 
// // 	FFormatOrderedArguments Args;
// // 	Args.Emplace(FText::FromString(nick));
// // 	TextBlock_Desc->SetText(FText::Format(Content, Args));
// // 
// // 	RefreshSelectImage(GuildGrade);
// }
// 
// void UUC_Guild_Create::CloseGrade()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// //	RGameInstance->RWidgetManager->HideGuildGradePopup();
// }
// 
// void UUC_Guild_Create::OnClickGrade(uint8 grade)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// // 	if (grade > MyGuildGrade)
// // 	{
// // 		RGameInstance->RWidgetManager->HideGuildGradePopup();
// // 		RGameInstance->RWidgetManager->ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// // 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
// // 		return;
// // 	}
// // 	SelectGrade = grade;
// // 	RefreshSelectImage(SelectGrade);
// }
// 
// void UUC_Guild_Create::OnClickGradeChange()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// // 	if ((EGUILD_GRADE)MyGuildGrade != EGUILD_GRADE::MEMBER)
// // 	{
// // 		RGameInstance->RWidgetManager->ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// // 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
// // 		return;
// // 	}
// // 	if (SelectGrade == (uint8)EGUILD_GRADE::MASTER)
// // 	{
// // 		FFormatArgumentValue FormatArg = FText::FromString(nick);
// // 		FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error13"));
// // 		LocalizedText = FText::Format(LocalizedText, FormatArg);
// // 
// // 		auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// // 			LocalizedText, ECommonPopupType::VE_OKCANCEL);
// // 		UP_CommonPopup->OkTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Transfer")));
// // 		UP_CommonPopup->CancelTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRaid_0032")));
// // 		if (IsValid(UP_CommonPopup))
// // 		{
// // 			UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UUC_Guild_Create::GradeTransfer);
// // 			UP_CommonPopup->OnClickOK.AddDynamic(this, &UUC_Guild_Create::GradeTransfer);
// // 		}
// // 	}
// // 	else
// // 	{
// // 		GradeTransfer();
// // 	}
// }
// 
// void UUC_Guild_Create::GradeTransfer()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 	UPacketFunctionLibrary::GUILD_MEMBER_GRADE_RQ(RGameInstance, kid, (int32)SelectGrade);
// 	RGameInstance->RWidgetManager->HideGuildGradePopup();
//}
// void UUC_Guild_Create::RefreshSelectImage(uint8 grade)
// {
// 	if ((EGUILD_GRADE)grade == EGUILD_GRADE::MASTER)
// 	{
// 		SelectImageList[DEF_SELECT_MASTER_INDEX]->SetVisibility(ESlateVisibility::Visible);
// 		SelectImageList[DEF_SELECT_ELDER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
// 		SelectImageList[DEF_SELECT_MEMBER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// 	else if ((EGUILD_GRADE)grade == EGUILD_GRADE::ELDER)
// 	{
// 		SelectImageList[DEF_SELECT_MASTER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
// 		SelectImageList[DEF_SELECT_ELDER_INDEX]->SetVisibility(ESlateVisibility::Visible);
// 		SelectImageList[DEF_SELECT_MEMBER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// 	else
// 	{
// 		SelectImageList[DEF_SELECT_MASTER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
// 		SelectImageList[DEF_SELECT_ELDER_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
// 		SelectImageList[DEF_SELECT_MEMBER_INDEX]->SetVisibility(ESlateVisibility::Visible);
// 	}
// }
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
