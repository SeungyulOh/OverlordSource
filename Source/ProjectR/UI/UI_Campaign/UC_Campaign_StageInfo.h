// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Campaign_StageInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Campaign_StageInfo : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Campaign_StageInfo|Ref")
	UTextBlock*			TitleTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Campaign_StageInfo|Ref")
	UTextBlock*			SubTitleTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_Campaign_StageInfo|Value")
	bool				IsLineType = false;

private:
	void NotVisible();
};
