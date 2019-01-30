// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HeroCard.generated.h"

class UImage;
class UPanelWidget;
class UTextBlock;
class UUC_HeroGrade;

/**
 * Represents a hero in hero inventory
 */
UCLASS()
class PROJECTR_API UUC_HeroCard : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	//// Interface
	UPROPERTY()
	FString HeroUD;

	void InvalidateData() override;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroCard)
	UImage* Image_HeroCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroCard)
	UPanelWidget* Panel_HeroLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroCard)
	UTextBlock* Text_HeroLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroCard)
	UUC_HeroGrade* HeroGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroCard)
	UTextBlock* Text_HeroName;

	
};
