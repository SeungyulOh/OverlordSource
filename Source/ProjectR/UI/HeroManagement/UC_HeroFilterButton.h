// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_HeroFilterButton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroFilterButton : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroFilterButton)
	UButton*						SortingBtn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroFilterButton)
	UWidgetSwitcher*				CheckStateSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroFilterButton)
	UWidgetSwitcher*				SortTypeSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroFilterButton)
	ULocalizingTextBlock*			Text_Type;
};
