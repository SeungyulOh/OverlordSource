// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_Attendance.generated.h"

class UUC_Item_Icons;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Guild_Attendance : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void			NativeConstruct()	override;
	virtual	void			NativeDestruct()	override;

	void OpenAttendance();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void CloseAttendance();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void OnClick_DailyReward();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void OnClick_WeeklyReward();

	UFUNCTION()
	void OnRefreshGuildAttendanceUI();
	UFUNCTION(BlueprintImplementableEvent, Category = UUC_Guild_Attendance)
	void BP_RefreshAttendance();

	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void RefreshAttendanceDaily(UWidgetSwitcher* CompleteSwitcher);
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_Attendance)
	void RefreshAttendanceWeekly(UTextBlock* Text_Day, UTextBlock* Text_DayWord, UTextBlock* Text_Hour, UTextBlock* Text_HourWord, 
		UTextBlock* Text_Minute, UTextBlock* Text_MinuteWord, UTextBlock* Text_WeekCount, UProgressBar* Progress_AttendanceCount, UButton* Btn_WeeklyAttendance);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Attendance)
	TArray<UImage*>			ReceiveImageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Attendance)
	TArray<UUC_Item_Icons*> ItemIconArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Attendance)
	TArray<USizeBox*>		ItemSizeBoxArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_Attendance)
	TArray<UUC_Item_Icons*> WeeklyItemIconArray;
	
};
