// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Common/UC_Item_Icons.h"

#include "UC_Splash_FloorShop_Item.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Splash_FloorShop_Item : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetItem(FREWARD& InReward);

	FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	UPROPERTY()
	UUC_Item_Icons*			Item_Icon		=	nullptr;
	UPROPERTY()
	UTextBlock*				Text_ItemName	=	nullptr;
};