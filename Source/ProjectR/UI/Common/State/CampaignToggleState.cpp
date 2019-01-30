// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CampaignToggleState.h"

#include "UI/Common/UC_QuickToggle.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"


void UCampaignToggleState::OnClick_Toggle(UUC_QuickToggle* OwnerToggle)
{
	if (IsValid(OwnerToggle) == false)
		return;

// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance->EventManager) == false)
// 		return;
// 
// 	RGameInstance->EventManager->OnClickCampaignChapter.Broadcast(OwnerToggle->ChapterID);
}

void UCampaignToggleState::SetActive(bool InbIsActive, UUC_QuickToggle* OwnerToggle)
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
