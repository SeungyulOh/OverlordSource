// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "CampaignStageButton.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_CampaignStageButton
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				EnterButton = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*					StageImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*				StageIndexText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UTextBlock*				StageTitleText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*					BGColor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UImage*					LineColor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<UImage*>			StarImageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UOverlay*				LockImage = nullptr;
};

UCLASS()
class PROJECTR_API UCampaignStageButton : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	UFUNCTION()
	void OnClickEnterButton();

	void SetData(int32 InFloorIndex, int32 InStageIndex, EStageDifficultyEnum InDifficulty);

	void RefreshUI();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCampaignStageButton")
	FVariables_CampaignStageButton		variables;

// 	일반컬러 : R:57  G : 134  B : 238  #3986ee
// 	정예컬러: R:86  G : 13   B : 16   #560d10
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_StageSelect")	// set UMG
	FLinearColor				ColorNormal = FLinearColor::FromSRGBColor(FColor(57, 134, 238));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_StageSelect")	// set UMG
	FLinearColor				ColorHard = FLinearColor::FromSRGBColor(FColor(86, 13, 16));


private:
	int32					FloorIndex = 0;	// 1 ~ 25
	int32					StageIndex = 0; // 1 ~ 6
	EStageDifficultyEnum	StageDifficulty = EStageDifficultyEnum::Normal;
	bool					bStageEnable = false;
};
