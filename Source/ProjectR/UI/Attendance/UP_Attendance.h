// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_Attendance.generated.h"


class UUC_AttendanceIcon;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Attendance : public URBaseWidget
{
	GENERATED_BODY()
	

public:
	void				NativeConstruct() override;
	void				NativeDestruct() override;

	void				InvalidateData();

	UFUNCTION(BlueprintCallable, Category = UP_Attendance)
	void				OnClick_OK();

	UFUNCTION(BlueprintCallable, Category = UP_Attendance)
	void				OnClick_Close();

	//UFUNCTION()
	//void				CallbackPopupOK();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Attendance)
	UGridPanel*				IconPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Attendance)
	UButton*				Button_Click = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_Attendance)
	TArray<	UUC_AttendanceIcon* >			AttendanceRewards;

private:
	//void				InitRewardIcons();
};
