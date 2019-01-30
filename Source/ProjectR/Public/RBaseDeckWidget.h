// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "RBaseDeckWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URBaseDeckWidget : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE FString GetCurHeroUD() { return CurHeroUD; }
	
	virtual void			SetAvailable(bool inAvailable) {}
	virtual void			SetSelecting(bool bSelecting) {}

	virtual void			SetHero(FString heroUD) {}
	
	UFUNCTION()
	void					OnClick_Deck();
	
protected:
	FString					CurHeroUD;
	
};
