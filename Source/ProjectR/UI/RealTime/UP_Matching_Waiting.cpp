// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Matching_Waiting.h"

#include "GlobalIntegrated.h"

#include "UI/RWidgetManager.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UtilFunctionIntegrated.h"

#include "Network/RTSManager.h"

void UUP_Matching_Waiting::NativeConstruct()
{
	if (IsValid(Button_MatchingCancel))
		Button_MatchingCancel->OnClicked.AddDynamic(this, &UUP_Matching_Waiting::OnClick_MatchingWaitingCancel);
	Super::NativeConstruct();
}

void UUP_Matching_Waiting::NativeDestruct()
{
	if (IsValid(Button_MatchingCancel))
		Button_MatchingCancel->OnClicked.RemoveDynamic(this, &UUP_Matching_Waiting::OnClick_MatchingWaitingCancel);
	Super::NativeDestruct();
}

void UUP_Matching_Waiting::RefreshUI(EPVP InConnecMode)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (IsValid(ContentsTextBlock))
	{
		FName ContentsKeyName;
		switch (InConnecMode)
		{
		case EPVP::BG:
			ContentsKeyName = FName(TEXT("UIPage_Matching_Title2"));
			break;
		case EPVP::GR:
			ContentsKeyName = FName(TEXT("UIPage_Matching_Title3"));
			break;
		case EPVP::CS:
			ContentsKeyName = FName(TEXT("UIPage_Matching_Title1"));
			break;
		case EPVP::BF:
			ContentsKeyName = FName(TEXT("UIPage_Matching_Title4"));
			break;
		}

		ContentsTextBlock->LocalizingKey = ContentsKeyName;
		ContentsTextBlock->ApplyLocalizedText();
	}
	PlayAni("NewAnimation_1", false, 100);
}

void UUP_Matching_Waiting::OnClick_MatchingWaitingCancel()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UUIFunctionLibrary::HideMatchingWaiting();
	URTSManager::GetInstancePtr()->REQ_MATCH_CANCEL();
}