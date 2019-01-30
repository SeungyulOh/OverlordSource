// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RaidToggleState.h"

#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"


#include "UI/Common/UC_QuickToggle.h"


void URaidToggleState::OnClick_Toggle(UUC_QuickToggle* OwnerToggle)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
	{
		return;
	}

	RGameInstance->EventManager->OnClick_QuickToggle.Broadcast(OwnerToggle->GetToggleIndex());
}

void URaidToggleState::SetActive(bool InbIsActive, UUC_QuickToggle* OwnerToggle)
{
	if (IsValid(OwnerToggle) == false)
		return;

	if (OwnerToggle->WidgetSwitcher_Toggle)
	{
		if (InbIsActive)
			OwnerToggle->WidgetSwitcher_Toggle->SetActiveWidgetIndex(1);
		else
			OwnerToggle->WidgetSwitcher_Toggle->SetActiveWidgetIndex(0);
	}
}