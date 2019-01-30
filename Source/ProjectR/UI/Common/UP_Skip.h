// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_Skip.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Skip : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = UP_Skip)
	void OnClick_Skip();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Skip|Ref")
	UButton*			Button_Skip;

public:
	UPROPERTY()
	FName SkipSequenceName;

	FTimerHandle				AutoSkipTimeHandler;
};
