// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UP_ItemInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_ItemInfo : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetItemInfo_Left(FString InItemUD);
	void SetItemInfo_Right(FString InItemUD);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemInfo")
	class UUC_HeroItemInfo*						ItemInfo_L = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemInfo")
	class UUC_HeroItemInfo*						ItemInfo_R = nullptr;

	
};
