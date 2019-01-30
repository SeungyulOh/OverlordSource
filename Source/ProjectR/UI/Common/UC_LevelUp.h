// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_LevelUp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_LevelUp : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UTextBlock*		TextBlock_Level;
};
