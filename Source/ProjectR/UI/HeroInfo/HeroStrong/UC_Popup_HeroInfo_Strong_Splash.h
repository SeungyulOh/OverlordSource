// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UC_Popup_HeroInfo_Strong_Splash.generated.h"

UCLASS()
class PROJECTR_API UUC_Popup_HeroInfo_Strong_Splash : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();

	void SetEnchant(TArray<int32>& InPrev, TArray<int32>& InEnchant);

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	UTextBlock*					Text_PrevEhchant	=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_NextEhchant	=	nullptr;
protected:
private:
};