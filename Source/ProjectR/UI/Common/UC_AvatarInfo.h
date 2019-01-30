// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_AvatarInfo.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_AvatarInfo : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_AvatarInfo)
	int32			Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_AvatarInfo)
	FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_AvatarInfo)
	FString			Guild;
};