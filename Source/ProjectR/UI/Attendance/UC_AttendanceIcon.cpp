// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_AttendanceIcon.h"

//#include "GlobalIntegrated.h"
//


#include "UtilFunctionIntegrated.h"
#include "UI/UIDefs.h"
#include "Table/RewardPreviewTableInfo.h"
#include "Global/TableManager.h"



void UUC_AttendanceIcon::NativeConstruct()
{
	Super::NativeConstruct();

	//InvalidateData();
}

void UUC_AttendanceIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_AttendanceIcon::InvalidateData()
{
	if (IsValid(Text_Day))
	{
		Text_Day->SetText( FText::AsNumber(0));
	}

	if (IsValid(Text_Reward))
	{
		Text_Reward->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (IsValid(Image_RewardDone))
	{
		Image_RewardDone->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_AttendanceIcon::SetIconInfo(int32 day, FName reward, bool isDone)
{
	if (IsValid(Text_Day))
	{
		Text_Day->SetText(FText::AsNumber(day));
	}

	if (IsValid(ArrowImage))
	{
		ArrowImage->SetVisibility((day % 7 == 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IsValid(PriceTypeSwitcher))
	{
		PriceTypeSwitcher->SetActiveWidgetIndex((day % 7 == 0) ? 1 : 0);
	}

	if (IsValid(Image_RewardDone))
	{
		if (isDone)
			Image_RewardDone->SetVisibility(ESlateVisibility::HitTestInvisible);
		else
			Image_RewardDone->SetVisibility(ESlateVisibility::Collapsed);
	}

	FRewardPreviewTableInfo* RewardPreviewTable = UTableManager::GetInstancePtr()->GetRewardPreviewRow(reward);
	if (RewardPreviewTable == nullptr)
	{
		UE_LOG(LogUI, Warning, TEXT("UUC_AttendanceIcon :: FRewardPreviewTableInfo table is null ====> %s"), *reward.ToString());
		return;
	}

	if (IsValid(Text_Reward))
	{
		Text_Reward->SetVisibility(ESlateVisibility::HitTestInvisible);
		Text_Reward->SetText(FText::AsNumber(RewardPreviewTable->min_amount1));
	}

//#ifndef RULE_TESTDELETE
//	FCurrencyPropertyTableInfo* CurrencyProperty = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(RewardPreviewTable->CodeName1);
//	if (CurrencyProperty == nullptr)
//	{
//		UE_LOG(LogUI, Warning, TEXT("UUC_AttendanceIcon :: FCurrencyPropertyTableInfo table is null ===> %s"), *RewardPreviewTable->CodeName1.ToString());
//	}
//#endif
	if (IsValid(Image_RewardIcon))
	{	
		TArray<FITEM_ICON_INFO> RewardsItemIconInfo;
		UUIFunctionLibrary::GetRewardsIconInfoByPreview(reward, RewardsItemIconInfo);

		if (RewardsItemIconInfo.Num() > 0)
		{
			UUIFunctionLibrary::SetImageBrush(RewardsItemIconInfo[0].ItemIconRef, Image_RewardIcon);
		}
	}
}

void UUC_AttendanceIcon::SetConfirmation()
{
	if (IsValid(Image_RewardDone))
	{
		Image_RewardDone->SetVisibility(ESlateVisibility::HitTestInvisible);			
	}
}
