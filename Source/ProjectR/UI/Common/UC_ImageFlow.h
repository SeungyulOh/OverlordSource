// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_ImageFlow.generated.h"

class UUC_QuickToggle;
class UImage;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ImageFlow : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()	override;
	virtual void NativeDestruct()	override;
	void Init(const TArray<FName>& InImageKeys);

	int32 GetCurrentImageIndex()	{ return CurrentImageIndex; }
	int32 GetTotalImageCount()		{ return TotalImageNumber; }

	UFUNCTION(BlueprintCallable, Category = UUC_ImageFlow)
	void OnClick_MoveImage(bool bIsLeft);

	UFUNCTION()
	void OnClick_ButtonImage();

	UFUNCTION()
	void OnClick_ButtonTipRight();
	UFUNCTION()
	void OnClick_ButtonTipLeft();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ImageFlow)
	UImage*		BP_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ImageFlow)
	UHorizontalBox*	BP_HBox_Toggles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ImageFlow)
	UButton*	Button_Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ImageFlow)
	TArray<UUC_QuickToggle*>	QuickToggles;


private:
	void CreateTogglesInHorizontalBox();
	void SetQuickToggle();
	void Refresh();
	void RefreshToggles();
	void Clear();

	UFUNCTION()
	void OnClick_QuickToggle(int32 InToggleIndex);
private:
	UPROPERTY()
	TArray<FName> ImageKeys;
	UPROPERTY()
	int32 TotalImageNumber;
	UPROPERTY()
	int32 CurrentImageIndex;
};
