// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/Common/UC_TopBar_Property.h"
#include "BasePropertyState.h"
#include "UtilFunctionIntegrated.h"
#include "Global/TableManager.h"
#include "UI/RWidgetManager.h"
#include "Global/RGameInstance.h"


void UBasePropertyState::RefreshInfo(UUC_TopBar_Property* StateOwner)
{
	SetImage(StateOwner);
	SetText(StateOwner);
}

void UBasePropertyState::SetImage(UUC_TopBar_Property* StateOwner)
{
	if (IsValid(StateOwner) == false)
		return;

	FWalletTableInfo*	Wallet_table = UTableManager::GetInstancePtr()->GetWalletRow(StateOwner->GetPropertyType());
	if (Wallet_table != nullptr && StateOwner->CurrencyImage != nullptr)
	{
		UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(Wallet_table->ItemIconKey), StateOwner->CurrencyImage);
		return;
	}
}

void UBasePropertyState::SetText(UUC_TopBar_Property* StateOwner)
{
}

void UBasePropertyState::Onclick_DirectLink(UUC_TopBar_Property* StateOwner)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager) == false)
	{
		return;
	}

	RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_Shop);
}
