// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Colosseum_Class.generated.h"

UCLASS()
class PROJECTR_API UUC_Colosseum_Class : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetColosseumClassIndex(int32 SelectIndex);

public:
	UPROPERTY()
	UWidgetSwitcher*	Switcher_Tier	=	nullptr;
	UPROPERTY()
	UTextBlock*			Text_Tier		=	nullptr;
};
