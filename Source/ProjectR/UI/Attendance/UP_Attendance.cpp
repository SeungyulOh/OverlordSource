// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Attendance.h"

#include "GlobalIntegrated.h"



#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"


#include "UI/Attendance/UC_AttendanceIcon.h"


void UUP_Attendance::NativeConstruct()
{
	Super::NativeConstruct();

	if (IconPanel)
	{
		int32 Max = IconPanel->GetChildrenCount();
		//AttendanceRewards.Reserve(Max);
		AttendanceRewards.Reset(Max);
		for (int32 i = 0; i < Max; ++i)
		{
			auto Widget = IconPanel->GetChildAt(i);
			if (Widget)
			{
				auto TargetWidget = Cast<UUC_AttendanceIcon>(Widget);
				if (TargetWidget)
				{
					AttendanceRewards.Emplace(TargetWidget);
				}
			}
		}
	}
	if (!IsValid(Button_Click))
		Button_Click = FindChildNameWidget<UButton>(this, TEXT("Button_Click"));

	if (IsValid(Button_Click))
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUP_Attendance::OnClick_OK);
		Button_Click->OnClicked.AddDynamic(this, &UUP_Attendance::OnClick_OK);
	}

	InvalidateData();
}

void UUP_Attendance::NativeDestruct()
{
	AttendanceRewards.Empty();

	Super::NativeDestruct();
}

void UUP_Attendance::InvalidateData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	auto AttendanceManager = RGameInstance->AttendanceManager;
	if (AttendanceManager == nullptr)
		return;
		
	int32 AttendanceNum = AttendanceManager->GetNumOfAttendanceDays();
	if (AttendanceManager->IsCheckStamp())
	{
		AttendanceNum -=  1;
	}
		
	FName RewardKey;
	int32 Day	= 1;
	bool bDone	= false;

	for (auto Icon : AttendanceRewards)
	{
		if (AttendanceNum > 0)
		{
			bDone = true;
			--AttendanceNum;
		}
		else
		{
			bDone = false;
		}

		RewardKey = AttendanceManager->GetRewardKeyOfAttendanceDay(Day);
		Icon->SetIconInfo(Day, RewardKey, bDone );			
		++Day;
	}
}

void UUP_Attendance::OnClick_OK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
	
	auto AttendanceManager = RGameInstance->AttendanceManager;

	bool bVisiblePopup = true;
	if (AttendanceManager)
	{
		int32 latestDay = AttendanceManager->GetNumOfAttendanceDays() - 1;

		if (AttendanceRewards.IsValidIndex(latestDay))
			AttendanceRewards[latestDay]->SetConfirmation();

		bVisiblePopup = AttendanceManager->IsCheckStamp();

		AttendanceManager->ClearCheckStamp();
	}

	if (bVisiblePopup)
	{
		if (RGameInstance->RWidgetManager)
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_login_000004"))));
			OnClick_Close();
// 			UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
// 				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_login_000004"))),
// 				ECommonPopupType::VE_OK);
// 
// 			if (CommonPopup)
// 				CommonPopup->OnClickOK.AddDynamic(this, &UUP_Attendance::OnClick_Close);
		}
	}
	else
	{
		OnClick_Close();
	}
}

void UUP_Attendance::OnClick_Close()
{
	//SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromViewport();
}

/*void UUP_Attendance::CallbackPopupOK()
{
	OnClick_Close();
}

void UUP_Attendance::InitRewardIcons()
{
	if (AttendanceRewards.Num() == 0)
		return;

	for (UUC_AttendanceIcon* Icon : AttendanceRewards)
	{
		if (IsValid(Icon))
		{
			Icon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}*/
