// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Shop/UC_HeroProduct.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroLimiteProduct.generated.h"

class UUC_CurrencyDisplay;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroLimiteProduct : public UUC_HeroProduct
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void RefreshUI() override;

	UFUNCTION(BlueprintCallable, Category = UUC_HeroLimiteProduct)
	void Buy();

	UFUNCTION()
	void OnBuy();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UUC_CurrencyDisplay*	CurrencyValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UImage*					JobImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UImage*					BackImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UTextBlock*				HeroNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UTextBlock*				HeroValueText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UTextBlock*				ValueText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UButton*				ClickBtn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UButton*				Button_Refresh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroLimiteProduct)
	UPanelWidget*			LockPanel = nullptr;

private: 
	int32					TotalTime = 0;

};