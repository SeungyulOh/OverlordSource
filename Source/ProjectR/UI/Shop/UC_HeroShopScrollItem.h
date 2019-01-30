// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UC_HeroShopScrollItem.generated.h"


class UUC_Item_Icons;
class UUC_CurrencyDisplay;
class UUC_HeroLimiteProduct;
class UUC_HeroSynergyProduct;
class UUC_HeroBoxsProduct;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroShopScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void SetParentScrollView(URScrollView* InScrollView) override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void InvalidateData() override;
	//UFUNCTION(BlueprintCallable, Category = UShopScrollItem)
	//void SetShopScrollItem(URScrollItemData* InOtherData = nullptr );

private:
	void ChangeSize();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroShopScrollItem)
	UUC_HeroLimiteProduct*		HeroLimiteProduct = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroShopScrollItem)
	UUC_HeroSynergyProduct*		HeroSynergyProduct = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroShopScrollItem)
	UUC_HeroBoxsProduct*		HeroBoxsProduct = nullptr;

private:
	FMargin					PrevWidgetSize;
};