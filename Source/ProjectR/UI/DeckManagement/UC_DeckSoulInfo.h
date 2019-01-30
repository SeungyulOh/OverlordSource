// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_DeckSoulInfo.generated.h"

class UUC_Soul_Column;
/**
*
*/
UCLASS()
class PROJECTR_API UUC_DeckSoulInfo : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void	NativeConstruct()	override;

public:
	void SetSoulInfo(int32 InIndex, FString InSoulUD);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_DeckSoulInfo)
	TArray<UUC_Soul_Column*>		CrewSouls;
};