// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"

class URGameInstance;
class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API AHUDBase : public AHUD
{
	GENERATED_BODY()
	
	
public:
	/** Event when play begins for this actor. */
	void BeginPlay() override; 

	/*Use Debug*/
	void DrawHUD() override;

	/*Show Indicator Event*/
	UFUNCTION(BlueprintImplementableEvent, Category = HUDBase)
	void OnShowIndicatorText( const FText& DisplayText);

private:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	UPROPERTY()
	ABasePlayerController* BasePlayerController = nullptr;
};