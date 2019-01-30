// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AttendanceManager.h"

#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "Network/PacketFunctionLibrary.h"

#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"

void UAttendanceManager::Initialize()
{

}

void UAttendanceManager::SetAttendanceInfo(FEVENT_ATTENDANCE& InInfo)
{
	Stamp = InInfo.stamp;
	AttendanceType = InInfo.attId;
	AttendanceDays = InInfo.days;
}

void UAttendanceManager::ClearCheckStamp()
{	
	Stamp = 0;
}

int32 UAttendanceManager::GetAttendanceDay(int32 day)
{
	if( !AttendanceDays.IsValidIndex(day -1) )
		return 0;

	return AttendanceDays[day-1];
}

int32 UAttendanceManager::GetNumOfAttendanceDays()
{
	return AttendanceDays.Num();
}

FName UAttendanceManager::GetRewardKeyOfAttendanceDay(int32 day)
{
	FName rewardKey;

	FAttendanceTableInfo* AttendanceTable = UTableManager::GetInstancePtr()->GetAttendanceTableRow( FName(*AttendanceType) );
	checkf(AttendanceTable != nullptr, TEXT("AttendanceTable is Empty"));

	if (AttendanceTable->RewardList.IsValidIndex(day - 1))
		rewardKey = AttendanceTable->RewardList[day - 1];

	return rewardKey;
}

 void UAttendanceManager::ShowAttendanceWidget()
 {
 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("RGameInstance->RWidgetManager is Empty"));
 	UUserWidget* Widget =  RGameInstance->RWidgetManager->GetUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Attendance);
 	if (IsValid(Widget))
 	{
 		if (!Widget->IsInViewport())
 		{
 			RGameInstance->RWidgetManager->AddToViewport(EWidgetBluePrintEnum::WBP_Attendance, Widget, ATTENDANCE_ZORDER);
 		}
 	}
 }
