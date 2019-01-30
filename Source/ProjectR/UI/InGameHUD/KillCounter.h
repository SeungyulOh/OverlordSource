// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "KillCounter.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTR_API UKillCounter : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()	override;
	virtual void NativeDestruct()	override;

	UFUNCTION()
	void Init();

public:
	UPROPERTY()
	UTextBlock* CurrentCounter;

	UPROPERTY()
	UTextBlock* NeedCounter;

private:
	UFUNCTION()
	void SetKillCounter();
private:
	int32 BossSpawnCount = 0;

	
};
