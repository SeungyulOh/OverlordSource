// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"


#include "AttendanceManager.generated.h"

//class URGameInstance;

/**
 * 
 */
UCLASS()
class PROJECTR_API UAttendanceManager : public UObject
{
	GENERATED_BODY()
	
public:
	void			Initialize();
	
	void			SetAttendanceInfo(FEVENT_ATTENDANCE& InInfo);
	
	void			ClearCheckStamp();
	
	int32			GetAttendanceDay(int32 day);
	

	int32			GetNumOfAttendanceDays();

	FName			GetRewardKeyOfAttendanceDay(int32 day);

 	void			ShowAttendanceWidget();
	
	FORCEINLINE bool			IsCheckStamp()		{ return (Stamp > 0); }
	FORCEINLINE TArray<int32>&	GetAttendanceDays() { return AttendanceDays; }

private:
// 	UPROPERTY()
// 	URGameInstance*	RGameInstance = nullptr;
	
	UPROPERTY()
	FString				AttendanceType;
	UPROPERTY()
	TArray<int32>		AttendanceDays;
	
	int32				Stamp;
};
