// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_HeroRuneResult.generated.h"


class UUC_HeroRune;


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_HeroRuneResult : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void	NativeConstruct() override;
	virtual void	NativeDestruct() override;
	
		
	virtual void	InvalidateData();
	
	UFUNCTION(BlueprintCallable, Category = EpisodeDetail)
	void			OnClick_Close();


public:
	UPROPERTY()
	FName									AcquiredHeroID;
	int32									AcquiredRuneCount = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroRuneDungeon)
	UTextBlock*								RuneHeroName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroRuneDungeon)
	UTextBlock*								RuneHeroAcquireCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroRuneDungeon)
	UUC_HeroRune*							HeroRune;
};
