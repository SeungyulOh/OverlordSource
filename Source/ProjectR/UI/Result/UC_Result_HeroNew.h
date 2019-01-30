// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Global/BattleRecordManager.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "UI/UIDefs.h"

#include "UC_Result_HeroNew.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Result_HeroNew : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = UUC_Result_HeroNew)
	void UpdateInfo(FHERO_ICON_INFO& InHeroIcon, FHeroSlotInfo& InSlotInfo);

	void StartProgress(float InTime);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	FHERO_ICON_INFO			ResultHeroIconInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	UImage*				Image_HeroCard = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	UTextBlock*			TextBlock_Level = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	UPanelWidget*		HorizontalBox_Exp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	UTextBlock*			TextBlock_Exp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	UTextBlock*			TextBlock_ExpProgress = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	UProgressBar*		BP_ProgressBar_Success = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUC_Result_HeroNew)
	UImage*				Image_Up = nullptr;

private:
	bool								bProgressShowStart = false;
	float								ShowTime = 1.0f;
	float								CurrentTime = 0.0f;
	float								DiffValue = 0.0f;
	float								StartValue = 0.0f;
	float								EndValue = 0.0f;
	float								MinusValue = 0.0f;
	int32								CurrentLevel = 0;
	float								CurrentMax = 0.0f;
};
