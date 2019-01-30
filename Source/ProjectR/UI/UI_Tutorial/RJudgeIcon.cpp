// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
//

#include "GlobalIntegrated.h"
//
#include "UtilFunctionIntegrated.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

#include "RJudgeIcon.h"

void URJudgeIcon::NativeConstruct()
{
	Super::NativeConstruct();

	//BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
	RGameInstance = RGAMEINSTANCE(this);

	if(Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &URJudgeIcon::BtnEventOnClicked);
		Button_Click->OnClicked.AddDynamic(this, &URJudgeIcon::BtnEventOnClicked);
	}
}

void URJudgeIcon::NativeDestruct()
{
	//BasePlayerController = nullptr;
	//CancelButton = nullptr;

	Super::NativeDestruct();
}


void URJudgeIcon::BtnEventOnClicked()
{
	float RemainTime = RGameInstance->GetTimerManager().GetTimerRemaining(CheckTimerHandler);
	/*if (RemainTime > 0.5f
		&& RemainTime < 1.5f)
	{
		//RGameInstance->NewTutorialManager->SetCheckType();
	}
	else
	{
		//RGameInstance->NewTutorialManager->SetCheckType();
	}*/
// 	if (CheckIconType == IconType)
// 	{
// 		RGameInstance->NewTutorialManager->SetCurrentJudge(EJudgeType::VE_Good);
// 	}
// 	else
// 	{
// 		RGameInstance->NewTutorialManager->SetCurrentJudge(EJudgeType::VE_Miss);
// 	}

	ClearTimer();
}

void URJudgeIcon::ClearTimer()
{
	/*if (CheckTimerHandler.IsValid())
	{
		RGameInstance->GetTimerManager().ClearTimer(CheckTimerHandler);
	}*/
	SetVisibility(ESlateVisibility::Collapsed);
}


void URJudgeIcon::SetInfo(EHudIcon InIconType, float InTime)
{
	CheckIconType = InIconType;
	CheckTime = InTime;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//RGameInstance->GetTimerManager().SetTimer(CheckTimerHandler, this, &URJudgeIcon::ClearTimer, CheckTime);
}

void URJudgeIcon::SetWidgetInfo(FVector2D InPos, FVector2D InSize)
{
	if (Button_Click)
	{
		auto ButtonCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Button_Click);
		ButtonCanvasSlot->SetSize(InSize);
	}

	auto InCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (InCanvasSlot)
	{
		InCanvasSlot->SetPosition(InPos);
		InCanvasSlot->SetSize(InSize);
	}
}

void URJudgeIcon::SetTargetTexture(UTexture2D* InTexture)
{
	if (InTexture == nullptr)
		return;

	if (TargetImage)
	{
		TargetImage->SetBrushFromTexture(InTexture);
	}
}
