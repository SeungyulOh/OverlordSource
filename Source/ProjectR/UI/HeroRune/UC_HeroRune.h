// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HeroRune.generated.h"





/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroRune : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void	NativeConstruct() override;
	virtual void	NativeDestruct() override;
	
	void			SetHeroID(FName InHeroID);
		
	virtual void	InvalidateData();
	

public:
	UPROPERTY()
	FName			HeroID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroRune)
	UImage*			HeroIconImage;
	
};
