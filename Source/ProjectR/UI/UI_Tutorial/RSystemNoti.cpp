// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"
//
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"
#include "Blueprint/WidgetLayoutLibrary.h"
//

#include "RSystemNoti.h"

void URSystemNoti::NativeConstruct()
{
	Super::NativeConstruct();
	this->bCanEverTick = true;
	this->bCanEverPaint = true;

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GetWorld());
}
/*
void URSystemNoti::NativeDestruct()
{
	Super::NativeDestruct();
}*/

void URSystemNoti::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsTick)
	{
		//FVector2D ComputeScreenLocation;
		BasePlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);
		//size
		//ScreenLocation.X -= 150;
		//ScreenLocation.Y -= 30;
		this->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		this->SetPositionInViewport(ScreenLocation);
	}
}

//void URSystemNoti::SetIconImageSize(FVector2D InSize)
//{
//	if (Image_Line)
//	{
//		auto CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Image_Line);
//		CanvasSlot->SetSize(InSize);
//	}
//
//	auto ButtonCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InputButton);
//	ButtonCanvasSlot->SetSize(InSize);
//}

void URSystemNoti::SetLocation(FVector InLocation)
{
	Location = InLocation;
	
	IsTick = true;
}

void URSystemNoti::SetLocation2D(FVector2D InLocation, bool InIsRight)
{
	ScreenLocation = InLocation;
	if (InIsRight)
	{
		//ScreenLocation.X += 50;
	}
	else
	{
		ScreenLocation.X -= 300;
		//ScreenLocation.Y -= 30 * 5;
	}
	this->SetPositionInViewport(ScreenLocation);
}

void URSystemNoti::SetDialogueString(const FName& InTableKey)
{
	FText Text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Dialogue, InTableKey);
	TextBlockDialogue->SetText(Text);
}