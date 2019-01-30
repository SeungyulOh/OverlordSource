// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RInputIndicator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API URInputIndicator : public URBaseWidget
{
	GENERATED_BODY()
	

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void BtnEventOnClicked();
	UFUNCTION()
	void BtnEventOnPressed();
	UFUNCTION()
	void BtnEventOnRelease();

	UFUNCTION()
	void TempCheck(int32 HeroSlotIndex, AActor* SelectedCharacter);

	void SetIconImageSize(FVector2D InSize, bool InIsMovePad = false);
	void SetMovePad(bool InActive);
	void TempSlide();

	//inline
	FORCEINLINE bool GetIsRefleshSize() { return IsRefreshSize; }
	FORCEINLINE bool GetIsClicked()		{ return IsClicked; }

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
	UPROPERTY()
	bool					IsClicked = false;
	UPROPERTY()
	bool					IndicatorTypeClicker = false;
	UPROPERTY()
	bool					IsPressIcon = false;
	UPROPERTY()
	bool					IsCheckRelease = false;
	UPROPERTY()
	bool					IsTempAttack = false;

private:
	UPROPERTY()
	FVector2D				DownPos;
	UPROPERTY()
	int32					FingerIndex = 0;
	UPROPERTY()
	bool					IsRefreshSize = false;
};
