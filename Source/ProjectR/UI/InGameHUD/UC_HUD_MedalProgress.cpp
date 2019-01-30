// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_MedalProgress.h"

#include "GlobalIntegrated.h"

//#include "Global/BattleFieldManager.h"
#include "UtilFunctionIntegrated.h"
#include "Network/RTSManager.h"



void UUC_HUD_MedalProgress::NativeConstruct()
{
	Super::NativeConstruct();

	ActiveProgress = false;

	SetVisibility(ESlateVisibility::Collapsed);

// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// // 	if (!RGameInstance->RealTimeModManager->IsRealTimeConnectMode(EPVP::BG))
// // 		return;
// 	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
// 		return;
// 	RGameInstance->EventManager->OnModRoomProgressMedal.RemoveDynamic(this, &UUC_HUD_MedalProgress::OnModRoomProgressMedal);
// 	RGameInstance->EventManager->OnModRoomProgressMedal.AddDynamic(this, &UUC_HUD_MedalProgress::OnModRoomProgressMedal);
// 
// 	auto RealTimeModDSN = RGameInstance->RealTimeModManager->GetRealTimeModDSN();
// 	FName ModeID = FName(*(RealTimeModDSN.modId));
// 
// 	auto TableInfo = UTableManager::GetInstancePtr()->GetBattleGroundTableRow(ModeID);
// 	if (TableInfo)
// 	{
// 		ProgressMaxTime = TableInfo->ExchangeTime;
// 	}
}

void UUC_HUD_MedalProgress::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnModRoomProgressMedal.RemoveDynamic(this, &UUC_HUD_MedalProgress::OnModRoomProgressMedal);
// 	}

	Super::NativeDestruct();
}

void UUC_HUD_MedalProgress::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ActiveProgress)
		return;

	if (false == IsValid(BP_ProgressBar_Success))
		return;

// 	if (1.0f <= PercentValue)
// 	{
// 		OnModRoomProgressMedal(false);
// 		return;
// 	}

	ProgressCheckTime += InDeltaTime;
	PercentValue = ProgressCheckTime / ProgressMaxTime;
	if (1.0f < PercentValue)
		PercentValue = 1.0f;

	BP_ProgressBar_Success->SetPercent(PercentValue);
}
// 
// void UUC_HUD_MedalProgress::OnModRoomProgressMedal(bool bActive)
// {
// 	if (false == IsValid(BP_ProgressBar_Success))
// 		return;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	if (false == IsValid(RGameInstance))
// 		return;
// 	
// // 	if (!RGameInstance->RealTimeModManager->IsRealTimeConnectMode(EPVP::BG))
// // 		return;
// 	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG))
// 		return;
// 	ActiveProgress = bActive;
// 
// 	if (ActiveProgress)
// 	{
// 		ProgressCheckTime = 0.0f;
// 		PercentValue = 0.0f;
// 
// 		BP_ProgressBar_Success->SetPercent(PercentValue);
// 
// 		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 	}
// 	else
// 	{
// 		this->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// 
// }