// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollView.h"

#include "Input/Reply.h"

#include "UC_FloorShop_FloorSelect.generated.h"


#define TOTAL_FLOOR 25

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_FloorShop_FloorSelect : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;


	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	UPROPERTY()
	UCanvasPanel*	CanvasPanel_Cursor	=	nullptr;
	UPROPERTY()
	UTextBlock*		Text_CurFloor		=	nullptr;
	UPROPERTY()
	UImage*			Image_Cursor		=	nullptr;
	UPROPERTY()
	UOverlay*		Overlay_Drag		=	nullptr;
	
private:
	bool		isShow					=	false;
	float		ButtonSizeY				=	0.f;
	float		Height_1Floor			=	0.f;
	float		FirstPosY				=	0.f;
	float		PosY					=	0.f;
	float		LeftTopPos				=	0.f;
	float		TargetTextFloorPos		=	0.f;
	int32		TargetFloor				=	0;
};