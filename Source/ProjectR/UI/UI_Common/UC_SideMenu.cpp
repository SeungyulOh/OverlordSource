// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "public/Components/Image.h"
#include "UI/RWidgetManager.h"
#include "UC_SideMenu.h"

//#define HEROMANAGEMENT_RENEWAL



void UUC_SideMenu::ClearNotify(ENotifyStateFlagEnum InNotifyState)
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->NotifyManager))
	{
		RGameInstance->NotifyManager->SetNotify(InNotifyState, false);
	}
}

FReply UUC_SideMenu::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UUC_SideMenu::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UUC_SideMenu::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}
