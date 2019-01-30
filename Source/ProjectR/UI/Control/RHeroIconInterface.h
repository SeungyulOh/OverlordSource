// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/UIDefs.h"
#include "UI/RBaseWidget.h"
#include "RHeroIconInterface.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URHeroIconInterface : public URBaseWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = URHeroIconInterface)
	void OnSetHeroIconInfo(FHERO_ICON_INFO InHeroInfo);

	
	
	
};
