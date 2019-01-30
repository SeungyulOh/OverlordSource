 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_JoinType_Change.h"
#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"

#define DEF_SELECT_FREE_INDEX 0
#define DEF_SELECT_ACCEPT_INDEX 1

void UUC_Guild_JoinType_Change::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<UImage>(this, SelectImageList, TEXT("Select_"));

	Button_Free->OnClicked.AddDynamic(this, &UUC_Guild_JoinType_Change::OnClickJoinTypeFree);
	Button_Accept->OnClicked.AddDynamic(this, &UUC_Guild_JoinType_Change::OnClickJoinTypeAccept);
	Button_OK->OnClicked.AddDynamic(this, &UUC_Guild_JoinType_Change::OnClickOK);
	Button_Cancel->OnClicked.AddDynamic(this, &UUC_Guild_JoinType_Change::CloseJoinType);
}

void UUC_Guild_JoinType_Change::NativeDestruct()
{
	Button_Free->OnClicked.RemoveDynamic(this, &UUC_Guild_JoinType_Change::OnClickJoinTypeFree);
	Button_Accept->OnClicked.RemoveDynamic(this, &UUC_Guild_JoinType_Change::OnClickJoinTypeAccept);
	Button_OK->OnClicked.RemoveDynamic(this, &UUC_Guild_JoinType_Change::OnClickOK);
	Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Guild_JoinType_Change::CloseJoinType);

	Super::NativeDestruct();
}

void UUC_Guild_JoinType_Change::OnClickJoinTypeFree()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

// 	if (RGameInstance->GuildInfo->MyMemberData.grade != EGUILD_GRADE::MASTER)
// 	{
// 		
// 		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
// 		CloseJoinType();
// 		return;
// 	}
	SelectJoinType = 0;
	RefreshSelectImage(SelectJoinType);
}

void UUC_Guild_JoinType_Change::OnClickJoinTypeAccept()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	if (RGameInstance->GuildInfo->MyMemberData.grade != EGUILD_GRADE::MASTER)
// 	{
// 		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
// 		return;
// 	}
	SelectJoinType = 1;
	RefreshSelectImage(SelectJoinType);
}

void UUC_Guild_JoinType_Change::OnClickOK()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	if (RGameInstance->GuildInfo->MyMemberData.grade != EGUILD_GRADE::MASTER)
// 	{
// 		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
// 		return;
// 	}

// 	if ((RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MASTER)
// 	{
// 		FText LocalizedText = FText();
// 
// 		auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 			LocalizedText, ECommonPopupType::VE_OKCANCEL);
// 		UP_CommonPopup->OkTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_Transfer")));
// 		UP_CommonPopup->CancelTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRaid_0032")));
// 		if (IsValid(UP_CommonPopup))
// 		{
// 			UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UUC_Guild_JoinType_Change::SendGuildAutoGradeUp);
// 			UP_CommonPopup->OnClickOK.AddDynamic(this, &UUC_Guild_JoinType_Change::SendGuildAutoGradeUp);
// 		}
// 	}
// 	else
// 	{
// 		SendGuildAutoGradeUp();
// 	}
	SendGuildAutoGradeUp();
}

void UUC_Guild_JoinType_Change::OpenJoinType(uint8 JoinType)
{
	SelectJoinType = JoinType;
	
	RefreshSelectImage(SelectJoinType);
}

void UUC_Guild_JoinType_Change::CloseJoinType()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Guild_JoinType_Change::SendGuildAutoGradeUp()
{
	UPacketFunctionLibrary::GUILD_EDIT_AUTO_GRADE_UP_RQ(SelectJoinType == 0);
	CloseJoinType();
}

void UUC_Guild_JoinType_Change::RefreshSelectImage(uint8 iSelectJoinType)
{
	if (iSelectJoinType == 0)
	{
		SelectImageList[DEF_SELECT_FREE_INDEX]->SetVisibility(ESlateVisibility::Visible);
		SelectImageList[DEF_SELECT_ACCEPT_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (iSelectJoinType == 1)
	{
		SelectImageList[DEF_SELECT_FREE_INDEX]->SetVisibility(ESlateVisibility::Collapsed);
		SelectImageList[DEF_SELECT_ACCEPT_INDEX]->SetVisibility(ESlateVisibility::Visible);
	}
}