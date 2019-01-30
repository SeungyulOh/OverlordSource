// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UC_HeroLink.generated.h"

class UUC_CurrencyDisplay;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroLink : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION()
	void OnClickSelected(URScrollItem* InSelectScrollItem);

	void Refresh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UImage*					BackImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UImage*					JobImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				RuneText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				FrontHeroText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				BackHeroText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UWidgetSwitcher*		Switcher_HeroOwn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UWidgetSwitcher*		Switcher_HeroUnlockType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UUC_CurrencyDisplay*	CurrencyValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UUC_CurrencyDisplay*	CurrencyValue2 = nullptr;

};