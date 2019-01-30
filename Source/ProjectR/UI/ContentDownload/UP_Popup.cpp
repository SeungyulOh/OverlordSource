// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Popup.h"


void UUP_Popup::NativeConstruct()
{
	OnClickOK.Clear();
	OnClickCancel.Clear();

	Super::NativeConstruct();
}

void UUP_Popup::SetPopup(FText InCaption, FText InContents, ECommonPopupType InPopupType)
{
	OnClickOK.Clear();
	OnClickCancel.Clear();

	Caption = InCaption;
	Contents = InContents;
	PopupType = InPopupType;
}

void UUP_Popup::RefreshUI()
{
	if (IsValid(CaptionTextBlock))
	{
		CaptionTextBlock->SetText(Caption);
	}
	if (IsValid(ContentsTextBlock))
	{
		ContentsTextBlock->SetText(Contents);
	}

	if (IsValid(OkButton))
	{
		OkButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(CancelButton))
	{
		if (PopupType == ECommonPopupType::VE_OKCANCEL)
		{
			SetCancelButtonVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SetCancelButtonVisibility(ESlateVisibility::Collapsed);
		}
	}
}