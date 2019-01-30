// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ToolTipBaseEvent.h"

#include "GlobalIntegrated.h"

#include "UI/RWidgetManager.h"

#include "UI/Common/UC_Tooltip.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "UtilFunctionIntegrated.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

#include "Table/ItemTableInfo.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "Table/CharacterTableInfo.h"
#include "Table/ShopProductTableInfo.h"
#include "Table/RewardPreviewItemTableInfo.h"

bool UTooltipBaseEvent::IsShowUI()
{
	return bShowUI;
}

void UTooltipBaseEvent::ShowUI(FVector2D pos)
{
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		if (!ToolTipUI.IsValid())
		{
			ToolTipUI = Cast<UUC_Tooltip>(RGameInstance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_ToolTip));
		}

		if (ToolTipUI.IsValid())
		{
			RGameInstance->RWidgetManager->AddToViewport(EWidgetBluePrintEnum::WBP_ToolTip, ToolTipUI.Get(), TOOLTIP_ZORDER);
			ToolTipUI->SetTooltipEvent(this, pos);

			bShowUI = true;
		}		
	}
}

void UTooltipBaseEvent::HideUI()
{
	if (ToolTipUI.IsValid())
	{
		ToolTipUI->HideTooltip();
		ToolTipUI->RemoveFromViewport();
		ToolTipUI = nullptr;
		bShowUI = false;
	}
}

void UTooltipBaseEvent::HideTooltip()
{
	HideUI();
}

FText UTooltipBaseEvent::GetNameText()
{
	return UUIFunctionLibrary::GetToolTipText(EventType, EventID);
}

FText UTooltipBaseEvent::GetToolTipTypeText()
{
	return UUIFunctionLibrary::GetToolTipTypeText(EventType, EventID);
}

FText UTooltipBaseEvent::GetDescriptionText()
{
	return UUIFunctionLibrary::GetToolTipDescriptionText(EventType, EventID);
}
