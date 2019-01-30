// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UI_Common/UP_ReadyToStart.h"
#include "UC_Campaign_Start.generated.h"

class UWidgetSwitcher;;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Campaign_Start : public UUP_ReadyToStart
{
	GENERATED_BODY()
	
public:
	void Init(FName stageId);

	virtual void BP_ReadyToStartEvent() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Campaign_Start)
	UTextBlock*			LocalizingTextBlock_TitleText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Campaign_Start)
	UTextBlock*			LocalizingTextBlock_SubTitle;
};
