

#include "ProjectR.h"
#include "CampaignStageButton.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"
#include "Global/CampaignManager.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/GamePlayBluePrintFunction.h"

// #include "GameInfo/RInventory.h"

#include "UI/RWidgetManager.h"
#include "UI/UIDefs.h"

void UCampaignStageButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(variables.EnterButton))
	{
		variables.EnterButton->OnClicked.RemoveDynamic(this, &UCampaignStageButton::OnClickEnterButton);
		variables.EnterButton->OnClicked.AddDynamic(this, &UCampaignStageButton::OnClickEnterButton);
	}

}

void UCampaignStageButton::NativeDestruct()
{
	if (IsValid(variables.EnterButton))
	{
		variables.EnterButton->OnClicked.RemoveDynamic(this, &UCampaignStageButton::OnClickEnterButton);
	}

	Super::NativeDestruct();
}

void UCampaignStageButton::OnClickEnterButton()
{
	if (!bStageEnable)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	const int32 StageCountperFloor = 12;

	int32 nCurrntFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;
	int32 nNormalIndex = ((nCurrntFloor - 1) * StageCountperFloor) + StageIndex;
	int32 nHardIndex = nNormalIndex + 6;
	int32 nStageIndex = (StageDifficulty == EStageDifficultyEnum::Normal) ? nNormalIndex : nHardIndex;

	const TArray<FName>& FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
	FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[nStageIndex]);
	checkf(FloorStageTable, TEXT("Failed to find FloorStageDetail Table"));

// 	bool IsEnoughStamina = !(RGameInstance->RInventory->GetStamina() == 0 || FloorStageTable->Stamina > RGameInstance->RInventory->GetStamina());
// 	if (IsEnoughStamina)
	{

		if (IsValid(UGamePlayBluePrintFunction::GetBaseStageInfo()))
		{

			UBaseStageInfo* CurrentStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
			//checkf(CurrentStageInfo, TEXT("CurrentStageInfo is Null"));

			CurrentStageInfo->ClearRewardTableKey = FloorStageTable->ClearRewardTableKey;
			CurrentStageInfo->MapTableKey = FName(*FString::FromInt(FloorStageTable->MapTableKey));
			CurrentStageInfo->FloorInfoId = FName(*FString::FromInt(FloorStageTable->FloorInfoId));
			CurrentStageInfo->StageIndex = FloorStageTable->StageIdx;
			CurrentStageInfo->StageID = FloorStageIDs[nStageIndex];

			if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
				CurrentStageInfo->bPlayAlone = true;
			else
				CurrentStageInfo->bPlayAlone = false;

// 			EDeckUsingTypeEnum DeckUsingType = CurrentStageInfo->bPlayAlone ? EDeckUsingTypeEnum::VE_PlayAlone : EDeckUsingTypeEnum::VE_Campaign;
			RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_DeckSetting);
		}

	}

}

void UCampaignStageButton::SetData(int32 InFloorIndex, int32 InStageIndex, EStageDifficultyEnum InDifficulty)
{
	FloorIndex = InFloorIndex;
	StageIndex = InStageIndex;
	StageDifficulty = InDifficulty;

	RefreshUI();
}

void UCampaignStageButton::RefreshUI()
{
	if (IsValid(variables.StageIndexText))
	{
		variables.StageIndexText->SetText(FText::AsNumber(StageIndex + 1));
	}

	if (IsValid(variables.StageTitleText))
	{
		const int32 MaxStageCount = 12;
		int32 iCurrentStageIdx = ((FloorIndex - 1) * MaxStageCount) + StageIndex;
		
		const TArray<FName>& FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
		if (FloorStageIDs.Num() <= iCurrentStageIdx)
			iCurrentStageIdx = iCurrentStageIdx - 1;

		if(FloorStageIDs.IsValidIndex(iCurrentStageIdx))
		{
			FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[iCurrentStageIdx]);

			variables.StageTitleText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorStageTable->Title));

			if(IsValid(variables.StageImage))
				variables.StageImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(FloorStageTable->ImageCardKey));
		}
	}
	
	switch (StageDifficulty)
	{
	case EStageDifficultyEnum::Normal:
	{
		if (IsValid(variables.BGColor))
			variables.BGColor->SetColorAndOpacity(ColorNormal);
		if (IsValid(variables.LineColor))
			variables.LineColor->SetColorAndOpacity(ColorNormal);
	}
	break;
	case EStageDifficultyEnum::Hard:
	{
		if (IsValid(variables.BGColor))
			variables.BGColor->SetColorAndOpacity(ColorHard);
		if (IsValid(variables.LineColor))
			variables.LineColor->SetColorAndOpacity(ColorHard);
	}
	break;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	const int32 StageCountperFloor = 12;

// 	int32 nCurrntFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;

	int32 nNormalIndex = ((FloorIndex - 1) * StageCountperFloor) + StageIndex;
	int32 nHardIndex = nNormalIndex + 6;
	int32 nStageIndex = (StageDifficulty == EStageDifficultyEnum::Normal) ? nNormalIndex : nHardIndex;
	
	for(int32 i = 0; i < variables.StarImageList.Num(); ++i)
	{
		if (variables.StarImageList.IsValidIndex(i))
			variables.StarImageList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	bStageEnable = false;

	if (RGameInstance->CampaignManager->TowerStatus.clearGrades.IsValidIndex(nStageIndex))
	{
		int32 nClearStarCount = RGameInstance->CampaignManager->TowerStatus.clearGrades[nStageIndex];
		for (int32 i = 0; i < nClearStarCount; ++i)
		{
			if (variables.StarImageList.IsValidIndex(i))
				variables.StarImageList[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		bStageEnable = (0 < nClearStarCount);

	}

	if (!bStageEnable)
	{
		int32 nProgressIndex = (StageDifficulty == EStageDifficultyEnum::Normal) ? RGameInstance->CampaignManager->GetNormalProgressIndex() : RGameInstance->CampaignManager->GetHardProgressIndex();
		if (StageIndex == nProgressIndex)
		{
			bStageEnable = true;
		}
	}

	if(IsValid(variables.EnterButton))
	{
		variables.EnterButton->SetIsEnabled(bStageEnable);
	}

	if (IsValid(variables.LockImage))
	{
		variables.LockImage->SetVisibility(bStageEnable ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}