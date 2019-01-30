// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RInputLocationIndicator.generated.h"

/**
 * 
 */

 //	일단 안 쓰는 클래스 2017.12.28 엄재영
UCLASS()
class PROJECTR_API URInputLocationIndicator : public URBaseWidget
{
	GENERATED_BODY()
	

public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//UFUNCTION()
	//void BtnEventOnClicked();

	//void SetIconImageSize(FVector2D InSize);
	/*void SetLocation(FVector InLocation);*/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RInputIndicator_Ref)
	UButton*				InputButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RInputIndicator_Ref)
	UImage*					Image_Icon = nullptr;

	//Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RInputIndicator_Ref)
	UWidgetAnimation*		HandAni_Ref = nullptr;

	UPROPERTY()
	bool					IsTickTemp = false;
	
private:
	UPROPERTY()
	FVector					Location;
	UPROPERTY()
	FVector2D				ScreenLocation;
};
