// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"
//
#include "GlobalIntegrated.h"
//
#include "ContentGameMode/Modes/StageGameMode.h"
#include "ContentGameMode/Modes/ArenaGameMode.h"
#include "ContentGameMode/Modes/MirrorGameMode.h"
//#include "Table/CampaignTableInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "UC_Campaign_StageInfo.h"

void UUC_Campaign_StageInfo::NativeConstruct()
{
	Super::NativeConstruct();
	
	auto RGameInstance = RGAMEINSTANCE(this);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	bool bIsValid = ( IsValid(BaseStageInfo));
	if (bIsValid == false)
	{
		NotVisible();
		return;
	}

	AStageGameMode* RGamePlayMode = Cast<AStageGameMode>(UUtilFunctionLibrary::GetGamePlayMode());
	if (RGamePlayMode == nullptr)
	{
		NotVisible();
		return;
	}

	bool bExe = true;
	//FName chapterName = UGamePlayBluePrintFunction::GetBaseStageInfo()->ChapterID;
	//FName episodeName = UGamePlayBluePrintFunction::GetBaseStageInfo()->EpisodeID;
	FName stageName = BaseStageInfo->StageID;

	if (stageName.IsNone() == false)
	{
		if (IsValid(TitleTextBlock) && IsValid(SubTitleTextBlock))
		{
// 			FCampaignStageTableInfo* StageTableInfo = UTableManager::GetInstancePtr()->GetCampaignStageRow(stageName);
// 			FFloorStageDetail* StageTableInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(UGamePlayBluePrintFunction::GetBaseStageInfo()->StageID);
// 			if (StageTableInfo)
// 			{
// 				if (IsLineType)
// 				{
// 					auto TitleText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->InGameTitle);
// 					auto SubTitleText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->InGameTitleName);
// 					auto ResultString = TitleText.ToString() + " "+ SubTitleText.ToString();
// 					TitleTextBlock->SetVisibility(ESlateVisibility::Collapsed);
// 					SubTitleTextBlock->SetText(FText::FromString(ResultString));
// 				}
// 				else
// 				{
// 					TitleTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->InGameTitle));
// 					SubTitleTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->InGameTitleName));
// 				}
// 				this->SetVisibility(ESlateVisibility::HitTestInvisible);
// 				bExe = false;
// 			}
		}
	}
	

	if (bExe)
	{
		NotVisible();
	}
}

void UUC_Campaign_StageInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Campaign_StageInfo::NotVisible()
{
	if (this->GetVisibility() != ESlateVisibility::Collapsed)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}
