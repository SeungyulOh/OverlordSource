// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UP_TimeTreasure.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_TimeTreasure : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClick_ButtonClose();
	UFUNCTION()
	void OnClick_ButtonOK();
	UFUNCTION()
	void OnClick_ButtonPreview();
	
public:
	UPROPERTY()
	UButton* Button_Close;
	UPROPERTY()
	UButton* Button_OK;
	UPROPERTY()
	UButton* Button_Preview;
	
};
