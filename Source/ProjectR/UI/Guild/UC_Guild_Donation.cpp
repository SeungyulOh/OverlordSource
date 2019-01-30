 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Guild_Donation.h"
#include "GlobalIntegrated.h"


#include "GameInfo/GuildInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"

void UUC_Guild_Donation::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->EventManager->OnRefreshGuildDonationUI.AddDynamic(this, &UUC_Guild_Donation::OnRefreshGuildDonationUI);
	}
}

void UUC_Guild_Donation::NativeDestruct()
{
 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
		RGameInstance->EventManager->OnRefreshGuildDonationUI.RemoveDynamic(this, &UUC_Guild_Donation::OnRefreshGuildDonationUI);
//	}

	Super::NativeDestruct();
}

void UUC_Guild_Donation::OpenDonation()
{
//	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UPacketFunctionLibrary::GUILD_CONTRIBUTION_STATUS_RQ();
}

void UUC_Guild_Donation::CloseDonation()
{
	this->SetVisibility(ESlateVisibility::Hidden);

 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	RGameInstance->EventManager->OnRequestGuildInfo.Broadcast();
}

void UUC_Guild_Donation::OnClick_Donation(EGUILD_CONTRIBUTION_TYPE SelectType)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (RGameInstance->GuildInfo->Donation_contributable == EREWARDABLE_STATUS::REWARDED)
// 		return;
// 	
// 	int32 SelectTypeToInt = (int32)SelectType;
// 	FName SelectTypeKey = FName(*FString::FromInt(SelectTypeToInt));
// 	FGuildContributeTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildContributeRow(SelectTypeKey);
// 
// 	if( UUIFunctionLibrary::IsEnableUsingCurrency(RGameInstance, SelectType == EGUILD_CONTRIBUTION_TYPE::TYPE1 ? EREWARD_TYPE::GOLD : EREWARD_TYPE::GEM, GuildTableInfo->consumeValue) )
// 		UPacketFunctionLibrary::GUILD_CONTRIBUTION_RQ(this, (int32)SelectType);
}

void UUC_Guild_Donation::OnRefreshGuildDonationUI()
{
	this->SetVisibility(ESlateVisibility::Visible);

	BP_RefreshDonation();
}

void UUC_Guild_Donation::RefreshDonationType(EGUILD_CONTRIBUTION_TYPE SelectType, UWidgetSwitcher* CompleteSwitcher, UCanvasPanel* CompletePanel,
								UUC_CurrencyDisplay* CurrencyDisplay, UTextBlock* TextPoint, UTextBlock* TextGold, UTextBlock* TextAvatarExp)
{
	if (!IsValid(CompleteSwitcher) || !IsValid(CompletePanel) || !IsValid(CurrencyDisplay))
		return;
	if (!IsValid(TextPoint) || !IsValid(TextGold) || !IsValid(TextAvatarExp))
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	int32 SelectTypeToInt = (int32)SelectType;
	FName SelectTypeKey = FName(*FString::FromInt(SelectTypeToInt));
	FGuildContributeTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildContributeRow(SelectTypeKey);
	if (GuildTableInfo)
	{
		if (EGUILD_CONTRIBUTION_TYPE::TYPE1 == SelectType)
		{
			CurrencyDisplay->SetCurrency(WALLET_GOLD, GuildTableInfo->consumeValue);
		}
		else
		{
			CurrencyDisplay->SetCurrency(WALLET_GEM, GuildTableInfo->consumeValue);
		}
		
		TextPoint->SetText(FText::AsNumber(GuildTableInfo->point));
		TextGold->SetText(FText::AsNumber(GuildTableInfo->gold));
		TextAvatarExp->SetText(FText::AsNumber(GuildTableInfo->avatarExp));
	}

	if (RGameInstance->GuildInfo->Donation_contributable == EREWARDABLE_STATUS::REWARDED)
	{
		CompleteSwitcher->SetActiveWidgetIndex(1);
		CompletePanel->SetVisibility(ESlateVisibility::Visible);
		/*
		if (RGameInstance->GuildInfo->Donation_contributionType == SelectType)
		{
			CompletePanel->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CompletePanel->SetVisibility(ESlateVisibility::Collapsed);
		}
		*/
	}
	else
	{
		CompleteSwitcher->SetActiveWidgetIndex(0);
		CompletePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// bool UUC_Guild_Donation::IsEnableContribute(EGUILD_CONTRIBUTION_TYPE SelectType)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return false;
// 
// 	int32 SelectTypeToInt = (int32)SelectType;
// 	FName SelectTypeKey = FName(*FString::FromInt(SelectTypeToInt));
// 	FGuildContributeTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildContributeRow(SelectTypeKey);
// 	FAVATAR Avatar = RGameInstance->UserInfo->GetAvatarData();
// 	bool isEnable = true;
// 
// 	if (EGUILD_CONTRIBUTION_TYPE::TYPE1 == SelectType && GuildTableInfo->consumeValue > Avatar.gold)
// 	{
// 		RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 
// 		auto Widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 		auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
// 		ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		
// 		float GoldValue = GuildTableInfo->consumeValue - Avatar.gold;
// 		ShortagePopup->SetNecessaryGold(GoldValue);
// 		auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY);
// 		auto Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_HeroManage_Notify2");
// 		FFormatOrderedArguments Args;
// 		auto DescTemp = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Result_gold");
// 		Args.Emplace(DescTemp);
// 		Args.Emplace(GoldValue);
// 		Desc = FText::Format(Desc, Args);
// 		ShortagePopup->LocalText_Caption->SetText(Caption);
// 		ShortagePopup->LocalText_Desc->SetText(Desc);
// 		return false;
// 	}
// 	
// 	if(EGUILD_CONTRIBUTION_TYPE::TYPE1 != SelectType && GuildTableInfo->consumeValue > Avatar.gems )
// 	{
// 		RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 
// 		auto Widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 		auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
// 		ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 
// 		float GemValue = GuildTableInfo->consumeValue - Avatar.gems;
// 		ShortagePopup->SetNecessaryGem(GemValue);
// 		ShortagePopup->LocalText_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY));
// 		ShortagePopup->LocalText_Desc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_ItemManage_OptionWarning"));
// 		return false;
// 	}
// 	return true;
// }
