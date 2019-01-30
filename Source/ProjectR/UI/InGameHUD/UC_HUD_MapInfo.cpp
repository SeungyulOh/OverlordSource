// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_MapInfo.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "Network/RTSManager.h"

void UUC_HUD_MapInfo::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshMapInfo();
}

void UUC_HUD_MapInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HUD_MapInfo::RefreshMapInfo()
{
	if (false == IsValid(TextBlock_Title))
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;

// 	FText TitleText;
// 	auto RealTimeModDSN = RGameInstance->RealTimeModManager->GetRealTimeModDSN();
// 	FName ModeID = FName(*(RealTimeModDSN.modId));
// 
// 	if( URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG) )
// 	{
// 		auto TableInfo = UTableManager::GetInstancePtr()->GetBattleGroundTableRow(ModeID);
// 		if (TableInfo)
// 		{
// 			TitleText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TableInfo->MapNameUIStringKey);
// 		}
// 	}
// 	else if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
// 	{
// 		auto TableInfo = UTableManager::GetInstancePtr()->GetBattleGuildRaidTableRow(ModeID);
// 		if (TableInfo)
// 		{
// 			TitleText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TableInfo->MapNameUIStringKey);
// 		}
// 	}
// 
// 	TextBlock_Title->SetText(TitleText);
}