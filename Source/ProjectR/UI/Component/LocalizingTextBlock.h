// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "SharedConstants/GlobalConstants.h"
#include "LocalizingTextBlock.generated.h"

/**
 * Looks up LocalStringTableManager to resolve string key.
 * If you update string key from code, call ApplyLocalizedText afterwards to apply to the native widget.
 */
UCLASS()
class PROJECTR_API ULocalizingTextBlock : public UTextBlock
{
	GENERATED_BODY()
	
public:
	void SynchronizeProperties() override;
	void ReleaseSlateResources(bool bReleaseChildren) override;

	void SetText(FText InText) override;
	void SetText(FName key, EStringTableType type);

	UFUNCTION(BlueprintCallable, Category = LocalizingTextBlock)
	void ApplyLocalizedText();

	  
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LocalizingTextBlock)
	FName LocalizingKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LocalizingTextBlock)
	EStringTableType StringTableType = EStringTableType::VE_UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LocalizingTextBlock)
	bool CustomControl = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LocalizingTextBlock)
	bool IsStartupContents = false;
};
