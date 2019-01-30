// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UP_InGame_Tutorial_Popup.generated.h"

struct FGuideResourceTableInfo;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_InGame_Tutorial_Popup : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	void Init();
	void SetTextureData();

	FName GetLocalTextureName(FGuideResourceTableInfo* InGuideTable);

	//property
	UPROPERTY()
		USizeBox* tutorial_btn_prev;
	UPROPERTY()
		USizeBox* tutorial_btn_next;
	UPROPERTY()
		UImage* Image_ForTexture;
	UPROPERTY()
		UButton* Button_Close;
	UPROPERTY()
		UButton* Button_Prev;
	UPROPERTY()
		UButton* Button_Next;
	//delegate
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickPrev();
	UFUNCTION()
		void OnClickNext();

private:
	uint8 pageIndex;
	
	UPROPERTY()
	TArray<UTexture2D*> NormalTextureArray;

	UPROPERTY()
	UTexture2D* HardTexture;
};