// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RBattleTimer.generated.h"

class AProjectRGameMode;
/**
 * 
 */
UCLASS()
class PROJECTR_API URBattleTimer : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool	IsValidData();
	void	UpdateTime();
	void	CheckWarningTime(int32 InMinute, int32 InSecond);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleTimer)
	UTextBlock*				Text_TimeCount = nullptr;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleTimer)
	//FSlateColor				DefaultColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBattleTimer)
	FSlateColor				OverTimeColor;

private:
	UPROPERTY()
	bool	IsOverTime = false;
	UPROPERTY()
	bool	IsMinuteColor = false;

	UPROPERTY()
	int32	CheckStateValue = 0;
};
