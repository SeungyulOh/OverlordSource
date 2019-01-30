// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_Currency_Icon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Currency_Icon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void Refresh(FName IconKey, int32 Amount);

public:
	UPROPERTY()
	UImage*			Image_Icon = nullptr;

	UPROPERTY()
	UTextBlock*		Text_StackNum = nullptr;
};
