// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UP_Popup_Reward_AfterGrind.h"
#include "UI/Control/RScrollView.h"
#include "UI/ItemManagement/UC_RewardAfterGrindScrollView.h"
#include "UI/RWidgetManager.h"



void UUP_Popup_Reward_AfterGrind::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAni(TEXT("Popup_Appear"));

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUP_Popup_Reward_AfterGrind::OnClickClose);
		Button_Close->OnClicked.AddDynamic(this, &UUP_Popup_Reward_AfterGrind::OnClickClose);
	}

	Refresh();
}

void UUP_Popup_Reward_AfterGrind::OnClickClose()
{
	if (this->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
		return;

	PlayAni(TEXT("Popup_Appear"), true);

	OnClickCloseEvent.ExecuteIfBound();

	//SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromViewport();

// 	URGameInstance* RGameInst = RGAMEINSTANCE(this);
// 	if (RGameInst)
// 	{
// 		if (RGameInst->RWidgetManager)
// 		{
// 			RGameInst->RWidgetManager->DeleteWidgetEtc(EWidgetBluePrintEnum::WBP_GrindRewardPopup);
// 		}
// 	}
	
}

void UUP_Popup_Reward_AfterGrind::Refresh()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	ScrollView->SetItemDataintoScrollItem();
	
}
