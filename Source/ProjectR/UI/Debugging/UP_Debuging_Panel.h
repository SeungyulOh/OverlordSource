// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/CharacterUI/CharacterUserWidget.h"
#include "UP_Debuging_Panel.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Debuging_Panel : public UCharacterUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void InvalidateData() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debuging_Panel_Control )
	UTextBlock*						TextBlock_DebugText = nullptr;

	/*
	flags
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debuging_Panel_Property)
	bool							bShowForceTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debuging_Panel_Property)
	bool							bShowAnimState = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debuging_Panel_Property)
	bool							bShowAIState = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debuging_Panel_Property)
	bool							bShowBuffList = true;
};
