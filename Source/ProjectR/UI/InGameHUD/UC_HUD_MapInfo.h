// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_MapInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_MapInfo : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

private:
	void RefreshMapInfo();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HUD_Gathering)
	UTextBlock*				TextBlock_Title = nullptr;
};
