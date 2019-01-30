// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_StageButton.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "UI/UI_Campaign/UP_Campaign_Tower.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/RInventory.h"
#include "UI/UI_Common/UP_CommonPopup.h"

//#define STAGE01 Btn_ScenarioStage01__Selected_Sprite

void UUC_StageButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (ButtonInput)
	{
		ButtonInput->SetClickMethod(EButtonClickMethod::MouseDown);
		ButtonInput->SetTouchMethod(EButtonTouchMethod::PreciseTap);
		ButtonInput->OnClicked.RemoveDynamic(this, &UUC_StageButton::OnButtonClick);
		ButtonInput->OnClicked.AddDynamic(this, &UUC_StageButton::OnButtonClick);
	}
	
	
	FindChildWidgetsWithCertainName(this, WidgetSwitcher_GradeStar, TEXT("GradeStar"));
}

void UUC_StageButton::NativeDestruct()
{
	Super::NativeDestruct();
}


void UUC_StageButton::OnButtonClick()
{
	if (bSelected == false)
	{
		for (size_t i = 0; i < UP_Campaign_Tower->NormalStageButtonArray.Num(); ++i)
			UP_Campaign_Tower->NormalStageButtonArray[i]->SetSelected(false);

		for (size_t i = 0; i < UP_Campaign_Tower->EliteStageButtonArray.Num(); ++i)
			UP_Campaign_Tower->EliteStageButtonArray[i]->SetSelected(false);

		SetSelected(true);

		UP_Campaign_Tower->CurrentSelectedStageIdx = StageIdx;

		UUC_TowerStageInfo* TowerStageInfo = UP_Campaign_Tower->TowerStageInfo;
		if (TowerStageInfo)
		{
			TowerStageInfo->Invalidate_ScrollView();

			FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;
			
			TArray<FName> FloorStageIDs = UP_Campaign_Tower->GetFloorStageIds();
			FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[StageIdx]);
			if (FloorStageTable)
			{
				TowerStageInfo->Title->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorStageTable->Title));

				TowerStageInfo->TopText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FloorStageTable->clearInfo2));
				TowerStageInfo->BottomText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FloorStageTable->clearInfo1));
			}
		}
	}
	else
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->RWidgetManager))
		{
			TArray<FName> FloorStageIDs = UP_Campaign_Tower->GetFloorStageIds();
			FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageIDs[StageIdx]);
			checkf(FloorStageTable, TEXT("Failed to find FloorStageDetail Table"));

			bool IsEnoughStamina = !(RGameInstance->RInventory->GetStamina() == 0 || FloorStageTable->Stamina > RGameInstance->RInventory->GetStamina());
			if (IsEnoughStamina)
			{

				if (IsValid(UGamePlayBluePrintFunction::GetBaseStageInfo()))
				{

					UBaseStageInfo* CurrentStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
					checkf(CurrentStageInfo, TEXT("CurrentStageInfo is Null"));

					CurrentStageInfo->ClearRewardTableKey = FloorStageTable->ClearRewardTableKey;
					CurrentStageInfo->MapTableKey = FName(*FString::FromInt(FloorStageTable->MapTableKey));
					CurrentStageInfo->FloorInfoId = FName(*FString::FromInt(FloorStageTable->FloorInfoId));
					CurrentStageInfo->StageIndex = FloorStageTable->StageIdx;
					CurrentStageInfo->StageID = FloorStageIDs[StageIdx];

					if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
						CurrentStageInfo->bPlayAlone = true;
					else
						CurrentStageInfo->bPlayAlone = false;

					EDeckUsingTypeEnum DeckUsingType = CurrentStageInfo->bPlayAlone ? EDeckUsingTypeEnum::VE_PlayAlone : EDeckUsingTypeEnum::VE_Campaign;
					RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_DeckSetting);
				}
				
			}
			else
			{
				UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, TEXT("Notify")),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Popup_Campaign_Start_StaminaCondition")), ECommonPopupType::VE_OKCANCEL);

				CommonPopup->OnClickOK.RemoveAll(this);
				CommonPopup->OnClickOK.AddDynamic(this, &UUC_StageButton::GotoCashShop);
			}
		}
	}
	
		
}

void UUC_StageButton::SetEliteMode()
{
	bEliteMode = true;
	if (WidgetSwitcher_Elite)
		WidgetSwitcher_Elite->SetActiveWidgetIndex(1);

}

void UUC_StageButton::SetStageState(int32 InStageIdx, EStageStateType InState)
{
	StageIdx = InStageIdx;
	StageState = InState;

	InvalidateData();
}

void UUC_StageButton::SetSelected(bool InSelected)
{
	if (bSelected == InSelected)
		return;

	bSelected = InSelected;

	if (InSelected)
		UP_Campaign_Tower->Toggle_ControlMode(bEliteMode);

	InvalidateData();
}

void UUC_StageButton::HideGradePanel(bool bHide)
{
	if (PanelGrade == nullptr)
		return;

	if (bHide)
	{
		PanelGrade->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PanelGrade->SetVisibility(ESlateVisibility::Visible);
	}
}


void UUC_StageButton::GotoCashShop()
{
	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_Shop);
}

void UUC_StageButton::InvalidateData()
{
	if (bSelected)
	{
		SetRenderScale(FVector2D(1.f, 1.f));
		if (VerticalBox_Play)
			VerticalBox_Play->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (VerticalBox_Play_Elite)
			VerticalBox_Play_Elite->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (PanelGrade)
			PanelGrade->SetVisibility(ESlateVisibility::Collapsed);
		if (PanelGrade_Elite)
			PanelGrade_Elite->SetVisibility(ESlateVisibility::Collapsed);

		if (ImageHead_Selected)
			ImageHead_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (ImageBody_Selected)
			ImageBody_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (ImageHead_Selected_Elite)
			ImageHead_Selected_Elite->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (ImageBody_Selected_Elite)
			ImageBody_Selected_Elite->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetRenderScale(FVector2D(0.79f, 0.79f));
		if (VerticalBox_Play)
			VerticalBox_Play->SetVisibility(ESlateVisibility::Collapsed);
		if (VerticalBox_Play_Elite)
			VerticalBox_Play_Elite->SetVisibility(ESlateVisibility::Collapsed);
		if (PanelGrade)
			PanelGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (PanelGrade_Elite)
			PanelGrade_Elite->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (ImageHead_Selected)
			ImageHead_Selected->SetVisibility(ESlateVisibility::Collapsed);
		if (ImageBody_Selected)
			ImageBody_Selected->SetVisibility(ESlateVisibility::Collapsed);
		if (ImageHead_Selected_Elite)
			ImageHead_Selected_Elite->SetVisibility(ESlateVisibility::Collapsed);
		if (ImageBody_Selected_Elite)
			ImageBody_Selected_Elite->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetStateImage();

	if (IsValid(Text_StageNumber) && IsValid(Text_StageNumber_Elite))
	{
		checkf(RGAMEINSTANCE(this), TEXT("GameInst Null"));
		checkf(RGAMEINSTANCE(this)->CampaignManager, TEXT("CampaignManager null"));
			
		int32 CurrentFloor = UP_Campaign_Tower->CurrentFloor;
		FText StageNumber = FText::FromString(FString::FormatAsNumber(CurrentFloor) + "-" + FString::FormatAsNumber(int32(StageDisplayInfo) + 1));
		Text_StageNumber->SetText(StageNumber);
		Text_StageNumber_Elite->SetText(StageNumber);
	}


	
		
}

void UUC_StageButton::SetStateImage()
{
	switch (StageState)
	{
	case EStageStateType::VE_OneStar:
	case EStageStateType::VE_TwoStar:
	case EStageStateType::VE_ThreeStar:
		SetGrade();
		break;
	case EStageStateType::VE_NotPlayed:
		SetNotPlayed();
		break;
	case EStageStateType::VE_Locked:
		SetLock();
		break;
	}
}

void UUC_StageButton::SetLock()
{
	if (ButtonInput)
	{
		ButtonInput->SetIsEnabled(false);
	}

	if (Image_Lock)
		Image_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (Image_Lock_Elite)
		Image_Lock_Elite->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (PanelGrade)
		PanelGrade->SetVisibility(ESlateVisibility::Collapsed);
	if (PanelGrade_Elite)
		PanelGrade_Elite->SetVisibility(ESlateVisibility::Collapsed);

	if (VerticalBox_Play)
		VerticalBox_Play->SetVisibility(ESlateVisibility::Collapsed);
	if (VerticalBox_Play_Elite)
		VerticalBox_Play_Elite->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_StageButton::SetGrade()
{
	if (ButtonInput)
	{
		if (ButtonInput->GetIsEnabled() == false)
		{
			ButtonInput->SetIsEnabled(true);
		}
	}

	if (!bEliteMode)
	{
		if (PanelGrade && !bSelected)
			PanelGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (Image_Lock)
			Image_Lock->SetVisibility(ESlateVisibility::Collapsed);
		
	}
	else
	{
		if (PanelGrade_Elite && !bSelected)
			PanelGrade_Elite->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (Image_Lock_Elite)
			Image_Lock_Elite->SetVisibility(ESlateVisibility::Collapsed);
	
	}

	int32 GradeNumber = static_cast<int32>(StageState);
	for (int32 i = 0; i < 3; ++i)
	{
		int Idx = !bEliteMode ? i : i + 3;

		if (WidgetSwitcher_GradeStar[Idx] == nullptr)
			continue;

		if (i < GradeNumber)
			WidgetSwitcher_GradeStar[Idx]->SetActiveWidgetIndex(0);
		else
			WidgetSwitcher_GradeStar[Idx]->SetActiveWidgetIndex(1);
	}
}

void UUC_StageButton::SetNotPlayed()
{
	if (ButtonInput)
	{
		if (ButtonInput->GetIsEnabled() == false)
			ButtonInput->SetIsEnabled(true);
	}

	if (Image_Lock)
		Image_Lock->SetVisibility(ESlateVisibility::Collapsed);
	if (Image_Lock_Elite)
		Image_Lock_Elite->SetVisibility(ESlateVisibility::Collapsed);
	
}

