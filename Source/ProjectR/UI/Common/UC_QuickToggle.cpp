// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"

#include "Public/Components/WidgetSwitcher.h"
#include "UC_QuickToggle.h"
#include "UI/Common/State/BaseToggleState.h"	
#include "UI/Common/State/CampaignToggleState.h"
#include "UI/Common/State/RaidToggleState.h"

void UUC_QuickToggle::CreateState(EToggleType ToggleType, int32 InToggleIndex)
{
	BaseToggleState = nullptr;

	switch (ToggleType)
	{
	case EToggleType::VE_Campaign:
		BaseToggleState = NewObject<UCampaignToggleState>(this);
		break;
	case EToggleType::VE_Raid:
		BaseToggleState = NewObject<URaidToggleState>(this);
		break;
	default:
		break;
	}

	ToggleIndex = InToggleIndex;
}

void UUC_QuickToggle::OnClick_QuickToggle()
{
	if (IsValid(BaseToggleState) == false)
		return;

	BaseToggleState->OnClick_Toggle(this);

	
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance->EventManager) == false)
// 		return;
// 
// 	RGameInstance->EventManager->OnClickCampaignChapter.Broadcast(ChapterID);
}

void UUC_QuickToggle::SetActive(bool InbIsActive)
{
	if (IsValid(BaseToggleState) == false)
		return;

	BaseToggleState->SetActive(InbIsActive, this);
	
	if (WidgetSwitcher_Toggle)
	{
		if(InbIsActive)
			WidgetSwitcher_Toggle->SetActiveWidgetIndex(1);
		else
			WidgetSwitcher_Toggle->SetActiveWidgetIndex(0);
	}
}