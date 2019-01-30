 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_RuneSupport.h"
#include "GlobalIntegrated.h"


#include "GameInfo/GuildInfo.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/RWidgetManager.h"
#include "UI/Control/RScrollView.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/Guild/GuildRuneRequest_ScrollItemData.h"

void UUC_Guild_RuneSupport::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->HttpClient->OnGuildRuneStatusRpDelegate.AddDynamic(this, &UUC_Guild_RuneSupport::OnGuildRuneStatusRpDelegate);
	}

	if (ScrollView_RuneRequestList)
	{
		ScrollView_RuneRequestList->Init();
		ScrollView_RuneRequestList->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUC_Guild_RuneSupport::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
		RGameInstance->HttpClient->OnGuildRuneStatusRpDelegate.RemoveDynamic(this, &UUC_Guild_RuneSupport::OnGuildRuneStatusRpDelegate);
//		RGameInstance->EventManager->OnClick_HeroIcon_HeroManagement_HeroList.RemoveDynamic(this, &UUC_Guild_RuneSupport::OnClick_HeroIcon);
//	}

	Super::NativeDestruct();
}

void UUC_Guild_RuneSupport::OpenRuneSupport()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UPacketFunctionLibrary::GUILD_RUNE_STATUS_RQ();
}

void UUC_Guild_RuneSupport::CloseRuneSupport()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_Guild_RuneSupport::OnGuildRuneStatusRpDelegate()
{
	this->SetVisibility(ESlateVisibility::Visible);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (!IsValid(ScrollView_RuneRequestList))
		return;

	ScrollView_RuneRequestList->ClearScrollItemData();
	ScrollView_RuneRequestList->SetVisibility(ESlateVisibility::Visible);

	// My Data First
	FGUILD_RUNE_REQUESTER MyRuneRequest;
	if (RGameInstance->GuildInfo->GetMyRuneRequest(MyRuneRequest))
	{
		UGuildRuneRequest_ScrollItemData* ScrollItemData = Cast<UGuildRuneRequest_ScrollItemData>(ScrollView_RuneRequestList->AddScrollItemData(UGuildRuneRequest_ScrollItemData::StaticClass()));
		if (ScrollItemData)
		{
			ScrollItemData->RuneRequester = MyRuneRequest;
		}
	}

	TArray<FGUILD_RUNE_REQUESTER> GuildRuneRequestList = RGameInstance->GuildInfo->RuneRequests;
	for (FGUILD_RUNE_REQUESTER& RuneRequest : GuildRuneRequestList)
	{
		if (RGameInstance->HttpClient->IsValidKID(RuneRequest.kId))
			continue;
		
		UGuildRuneRequest_ScrollItemData* ScrollItemData = Cast<UGuildRuneRequest_ScrollItemData>(ScrollView_RuneRequestList->AddScrollItemData(UGuildRuneRequest_ScrollItemData::StaticClass()));
		if (ScrollItemData)
		{
			ScrollItemData->RuneRequester = RuneRequest;
		}
	}

	ScrollView_RuneRequestList->InvalidateData();
	
	RefreshTimer();
}

void UUC_Guild_RuneSupport::OnClick_HeroIcon(FString InHeroUD)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	FGUILD_RUNE_REQUESTER MyRuneRequest;
	if (RGameInstance->GuildInfo->GetMyRuneRequest(MyRuneRequest))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////

	FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(InHeroUD);
// 	if (false == RGameInstance->HeroInventory->GetInventoryData(InHeroUD, HeroData))
// 		return;
	if (HeroData == nullptr)
		return;
	SelectHeroID = HeroData->heroId;

	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(SelectHeroID);
	if (CharacterTableInfo == nullptr)
		return;

	FFormatArgumentValue FormatArg = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName);
	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_AskRuneMsg"));
	LocalizedText = FText::Format(LocalizedText, FormatArg);

	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_AskRune")),
		LocalizedText,
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(UP_CommonPopup))
	{
		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UUC_Guild_RuneSupport::GuildRuneRequest);
		UP_CommonPopup->OnClickOK.AddDynamic(this, &UUC_Guild_RuneSupport::GuildRuneRequest);
	}
}

void UUC_Guild_RuneSupport::GuildRuneRequest()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	UPacketFunctionLibrary::GUILD_RUNE_REQUEST_RQ(SelectHeroID);
}

void UUC_Guild_RuneSupport::RefreshTimer()
{
	if (!IsValid(Text_RemainHour) || !IsValid(Text_RemainMinute))
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	FGUILD_RUNE_REQUESTER MyRuneRequest;
	if (RGameInstance->GuildInfo->GetMyRuneRequest(MyRuneRequest))
	{
		int32 hour = (int32)(MyRuneRequest.timeLeft / 3600);
		int32 minute = (int32)(MyRuneRequest.timeLeft / 60) % 60;
		Text_RemainHour->SetText(FText::AsNumber(hour));
		Text_RemainMinute->SetText(FText::AsNumber(minute));
	}
	else
	{
		Text_RemainHour->SetText(FText::FromString(TEXT("00")));
		Text_RemainMinute->SetText(FText::FromString(TEXT("00")));
	}
}