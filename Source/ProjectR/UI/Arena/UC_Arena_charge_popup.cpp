// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Utils/UtilFunctionLibrary.h"

#include "Network/PacketFunctionLibrary.h"

#include "UC_Arena_charge_popup.h"

void UUC_Arena_charge_popup::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_Arena_charge_popup::OnClickOk);
		Button_OK->OnClicked.AddDynamic(this, &UUC_Arena_charge_popup::OnClickOk);
	}
	if (IsValid(Button_Back))
	{
		Button_Back->OnClicked.RemoveDynamic(this, &UUC_Arena_charge_popup::OnClickCancel);
		Button_Back->OnClicked.AddDynamic(this, &UUC_Arena_charge_popup::OnClickCancel);
	}
	if (IsValid(Button_Back2))
	{
		Button_Back2->OnClicked.RemoveDynamic(this, &UUC_Arena_charge_popup::OnClickCancel);
		Button_Back2->OnClicked.AddDynamic(this, &UUC_Arena_charge_popup::OnClickCancel);
	}
}

void UUC_Arena_charge_popup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Arena_charge_popup::SetText(int32 InGem, int32 InCurency, bool InDaily)
{
	IsDaily = InDaily;
	int32	Max = IsDaily ? 10:3;

	if (IsValid(Text_TopCount))
		Text_TopCount->SetText(FText::AsNumber(InGem));
	if (IsValid(Text_Currency))
		Text_Currency->SetText(FText::AsNumber(InCurency));
	if(IsValid(Text_Change))
		Text_Change->SetText(FText::AsNumber(Max));

	if (IsDaily)
	{
		if (IsValid(Text_Topbar))
			Text_Topbar->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Notify"))));
		if (IsValid(Text_AddTooltip))
			Text_AddTooltip->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Arena_Popup_DailyBonusCharge_Desc03"))));
		if (IsValid(Text_DailyBonus))
			Text_DailyBonus->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Arena_Popup_DailyBonusCharge_Desc04"))));
	}
	else
	{
		if (IsValid(Text_Topbar))
			Text_Topbar->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Notify"))));
		if (IsValid(Text_AddTooltip))
			Text_AddTooltip->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Arena_Popup_DefResetCharge_Desc03"))));
		if (IsValid(Text_DailyBonus))
			Text_DailyBonus->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Arena_Popup_DefResetCharge_Desc04"))));
	}
}

void UUC_Arena_charge_popup::OnClickOk()
{
	FString		ReceiveString;
	if (IsDaily)
		ReceiveString = "ArenaDailyBonus";
	else
		ReceiveString = "ArenaDefenderReset";

	this->SetVisibility(ESlateVisibility::Collapsed);
	UPacketFunctionLibrary::CONTENT_RESET_RQ(ReceiveString, TEXT(""));
}

void UUC_Arena_charge_popup::OnClickCancel()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
