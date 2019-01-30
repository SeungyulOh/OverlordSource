// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_Block.generated.h"


UENUM(BlueprintType)
enum class EUIBlockTypeEnum : uint8
{
	VE_Normal				UMETA(DisplayName = "Normal"),
	VE_Tooltip				UMETA(DisplayName = "Tooltip"),
};


/**
 * BP: UP_BlockScreen
 */
UCLASS()
class PROJECTR_API UUP_Block : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = UP_Block)
	void OnClick_Block();

	UFUNCTION(BlueprintCallable, Category = UP_Block)
	void ShowBackground();

	UFUNCTION(BlueprintCallable, Category = UP_Block)
	void FadeIn(float InTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Block|Ref")
	UBorder*				Border_Background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Block|Ref")
	UButton*				Button_Click = nullptr;

	UPROPERTY()
	EUIBlockTypeEnum		CurBlockType = EUIBlockTypeEnum::VE_Normal;

private:
	float					FadeInTime = 0.0f;
	float					CurTime = 0.0f;
	bool					bStartFadeIn = false;
};
