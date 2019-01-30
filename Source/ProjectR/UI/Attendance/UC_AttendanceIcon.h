// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_AttendanceIcon.generated.h"

/**
 * BP: UC_AttendanceDay
 * embedded in BP UP_Attendance
 */
UCLASS()
class PROJECTR_API UUC_AttendanceIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void InvalidateData();

	void SetIconInfo(int32 day, FName reward, bool isDone);
	void SetConfirmation();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_AttendanceIcon|Ref")
	UTextBlock*			Text_Day = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_AttendanceIcon|Ref")
	UTextBlock*			Text_Reward = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_AttendanceIcon|Ref")
	UImage*				Image_RewardIcon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_AttendanceIcon|Ref")
	UImage*				Image_RewardDone = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_AttendanceIcon|Ref")
	UWidgetSwitcher*	PriceTypeSwitcher = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UC_AttendanceIcon|Ref")
	UImage*				ArrowImage = nullptr;

};
