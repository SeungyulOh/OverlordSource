// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_Goal.h"
#include "Global//RGameInstance.h"
//


#include "UtilFunctionIntegrated.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"

//#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "Global/TableManager.h"
#include "Global/LocalStringTableManager.h"

void UUC_HUD_Goal::NativeConstruct()
{
	Super::NativeConstruct();

	SetGoalText();
}

void UUC_HUD_Goal::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_HUD_Goal::SetGoalText()
{	
	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance == nullptr)
		return;

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	FMapTableInfo* MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(BaseStageInfo->GetSelectedMapID());
	if (MapTableInfo == nullptr)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (MapTableInfo->StarCondition.IsNone())
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FText Text;
	ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(EStringTableType::VE_Campaign, MapTableInfo->StarCondition, Text);

	if (Text_Goal)
	{
		Text_Goal->SetText(Text);
	}
}