// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_DailyContentsList.generated.h"


/**
*
*/
UCLASS()
class PROJECTR_API UUC_DailyContentsList : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = DailyQuest)
	void OnClickDirectLink();

	UFUNCTION(BlueprintCallable, Category = DailyQuest)
	void SetContents(EGamePlayModeTypeEnum InContentType, int32 InContentsScore);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyQuest)
	ULocalizingTextBlock*		ContentsNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyQuest)
	UTextBlock*					ContentsScore = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DailyQuest)
	UButton*					Button_Proceed = nullptr;

private:
	EGamePlayModeTypeEnum				eContentType = EGamePlayModeTypeEnum::VE_None;
};
