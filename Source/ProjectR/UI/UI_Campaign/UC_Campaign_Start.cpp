// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"

#include "Global/TableManager.h"
#include "Global/EventManager.h"

#include "Table/CampaignTableInfo.h"
#include "UC_Campaign_Start.h"

void UUC_Campaign_Start::Init(FName stageId)
{
 	FFloorStageDetail* StageTableInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(stageId);
	if (StageTableInfo)
	{
		if (IsValid(LocalizingTextBlock_TitleText))
		{
			LocalizingTextBlock_TitleText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->IngameSubTitle));
		}

		if (IsValid(LocalizingTextBlock_SubTitle))
		{
			LocalizingTextBlock_SubTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->Title));
		}
	}
}

void UUC_Campaign_Start::BP_ReadyToStartEvent()
{
	Super::BP_ReadyToStartEvent();

	if (RGameInstance->EventManager)
		RGameInstance->EventManager->OnHideDialogueEvent.Broadcast();

	RemoveFromParent();
}
