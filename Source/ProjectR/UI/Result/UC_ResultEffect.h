// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_ResultEffect.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EResultEffectEnum : uint8
{
	VE_Victory					UMETA(DisplayName = "Victory"),
	VE_Defeat					UMETA(DisplayName = "Defeat"),
	VE_Draw						UMETA(DisplayName = "Draw"),
	VE_Star1					UMETA(DisplayName = "Star1"),
	VE_Star2					UMETA(DisplayName = "Star2"),
	VE_Star3					UMETA(DisplayName = "Star3"),
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ResultEffect : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = UUC_ResultEffect)
	void SetState(EResultEffectEnum InState);

	UFUNCTION(BlueprintCallable, Category = UUC_ResultEffect)
	void ShowVictoryText(bool bShow);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UUC_ResultEffect)
	UTextBlock*			Text_Win = nullptr;
};