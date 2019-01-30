// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "RInputLocationIndicator.h"

//	일단 안 쓰는 클래스 2017.12.28 엄재영

void URInputLocationIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (InputButton)
	{
		InputButton->OnClicked.RemoveDynamic(this, &URInputLocationIndicator::BtnEventOnClicked);
		InputButton->OnClicked.AddDynamic(this, &URInputLocationIndicator::BtnEventOnClicked);
	}*/
}

void URInputLocationIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//if(IsTickTemp)
	//{
	//	Super::NativeTick(MyGeometry, InDeltaTime);

	//	if (IsAnimationPlaying(HandAni_Ref) == false)
	//	{
	//		PlayAnimation(HandAni_Ref);
	//	}

	//	ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GetWorld());
	//	//FVector2D ComputeScreenLocation;
	//	BasePlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);
	//	//size
	//	ScreenLocation.X -= 50;
	//	ScreenLocation.Y -= 100;
	//	this->SetPositionInViewport(ScreenLocation);
	//}
}

//void URInputLocationIndicator::BtnEventOnClicked()
//{
//	
//}

//void URInputLocationIndicator::SetIconImageSize(FVector2D InSize)
//{
//	/*auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image_Icon);
//	CanvasSlot->SetSize(InSize);
//
//	auto ButtonCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InputButton);
//	ButtonCanvasSlot->SetSize(InSize);*/
//}

//void URInputLocationIndicator::SetLocation(FVector InLocation)
//{
//	Location = InLocation;
//	IsTickTemp = true;
//}