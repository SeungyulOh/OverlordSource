// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RSystemNoti.generated.h"

class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URSystemNoti : public URBaseWidget
{
	GENERATED_BODY()
	

public:
	void NativeConstruct() override;
	//void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//void SetIconImageSize(FVector2D InSize);
	void SetDialogueString(const FName& InTableKey);
	void SetLocation(FVector InLocation);
	void SetLocation2D(FVector2D InLocation, bool InIsRight);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSystemNoti|Property")
	UButton*				InputButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSystemNoti|Property")
	UOverlay*				Overlay_Group = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSystemNoti|Property")
	UImage*					Image_BG = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSystemNoti|Property")
	UImage*					Image_Line = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RSystemNoti|Property")
	UTextBlock*				TextBlockDialogue = nullptr;
	
	bool					IsTick = false;
private:
	UPROPERTY()
	ABasePlayerController*	BasePlayerController = nullptr;

	FVector					Location;
	FVector2D				ScreenLocation;
};
