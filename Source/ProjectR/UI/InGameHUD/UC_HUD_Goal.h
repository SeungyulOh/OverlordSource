// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HUD_Goal.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HUD_Goal : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_HUD_Goal)
	UTextBlock*				Text_Goal = nullptr;

private:
	void SetGoalText();
};