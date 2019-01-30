// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_FloorMissionReward_Popup.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UIFunctionLibrary.h"



void UUC_FloorMissionReward_Popup::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, rewardPreviewIconList, TEXT("RewardIcon_"));
	FindChildWidgetsWithCertainName<USizeBox>(this, SizeBoxList, TEXT("Reward_Box_"));
	Button_Close->OnClicked.RemoveDynamic(this, &UUC_FloorMissionReward_Popup::OnClicked_ButtonClose);
	Button_Close->OnClicked.AddDynamic(this, &UUC_FloorMissionReward_Popup::OnClicked_ButtonClose);
	Button_OK->OnClicked.RemoveDynamic(this, &UUC_FloorMissionReward_Popup::OnClicked_ButtonClose);
	Button_OK->OnClicked.AddDynamic(this, &UUC_FloorMissionReward_Popup::OnClicked_ButtonClose);

	SetRewardPreviewList();
}

void UUC_FloorMissionReward_Popup::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_FloorMissionReward_Popup::OnClicked_ButtonClose()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_FloorMissionReward_Popup::SetRewardPreviewList()
{
	TArray<FITEM_ICON_INFO> IconArray;
	UUIFunctionLibrary::GetRewardsIconInfoByPreview(TEXT("FQComplete"), IconArray);
	for (int32 i = 0; i < rewardPreviewIconList.Num(); i++)
	{
		if (i < IconArray.Num())
			rewardPreviewIconList[i]->SetItemIconStructure(IconArray[i]);
		else
			SizeBoxList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}
