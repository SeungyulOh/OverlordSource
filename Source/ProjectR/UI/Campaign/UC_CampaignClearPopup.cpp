// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_CampaignClearPopup.h"

#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "UI/Common/Rewards_Scrollview.h"

void UUC_CampaignClearPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_Close))
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_CampaignClearPopup::OnClick_Close);
		Button_Close->OnClicked.AddDynamic(this, &UUC_CampaignClearPopup::OnClick_Close);
	}

	if (IsValid(Button_Close2))
	{
		Button_Close2->OnClicked.RemoveDynamic(this, &UUC_CampaignClearPopup::OnClick_Close);
		Button_Close2->OnClicked.AddDynamic(this, &UUC_CampaignClearPopup::OnClick_Close);
	}

	if (IsValid(Button_Enter))
	{
		Button_Enter->OnClicked.RemoveDynamic(this, &UUC_CampaignClearPopup::OnClick_Enter);
		Button_Enter->OnClicked.AddDynamic(this, &UUC_CampaignClearPopup::OnClick_Enter);
	}


// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnCampaignEpisodeClearPopup.RemoveDynamic(this, &UUC_CampaignClearPopup::OnCampaignEpisodeClearPopup);
// 		RGameInstance->EventManager->OnCampaignEpisodeClearPopup.AddDynamic(this, &UUC_CampaignClearPopup::OnCampaignEpisodeClearPopup);
// 	}
}

void UUC_CampaignClearPopup::NativeDestruct()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnCampaignEpisodeClearPopup.RemoveDynamic(this, &UUC_CampaignClearPopup::OnCampaignEpisodeClearPopup);
// 	}

	Super::NativeDestruct();
}

void UUC_CampaignClearPopup::OnCampaignEpisodeClearPopup(FName EpisodeID, int32 EpisodeIdx)
{
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	CurEpisodeID = EpisodeID;
	CurEpisodeIdx = EpisodeIdx;

	if (IsValid(Text_Caption))
	{
		FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Reward_UI_01"));
		FFormatArgumentValue FormatArg1 = FText::AsNumber(CurEpisodeIdx + 1);
		LocalizedText = FText::Format(LocalizedText, FormatArg1);

		Text_Caption->SetText(LocalizedText);
	}

// 	if(IsValid(UC_RewardsScrollView))
// 	{
// 		FCampaignEpisodeTableInfo* EpisodeTableInfo = UTableManager::GetInstancePtr()->GetCampaignEpisodeRow(CurEpisodeID);
// 		if (EpisodeTableInfo)
// 		{
// 			UC_RewardsScrollView->InvalidateByRewardKey(EpisodeTableInfo->StarRewardKey);
// 		}
// 	}

	/*
	if (IsValid(TextEnterButton))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		
		{
			int32 state = RGameInstance->CampaignManager->GetEpisodeRewardStatus(CurEpisodeIdx);

			switch (state)
			{
			case 0:	//play
				TextEnterButton->LocalizingKey = FName("USK_uc_Start");
				break;
			case 1: //clear
				TextEnterButton->LocalizingKey = FName("UI_Quest_Button1");
				break;
			case 2: //clear & get
				TextEnterButton->LocalizingKey = FName("UI_Quest_Desc_7");
				break;
			}

			TextEnterButton->ApplyLocalizedText();
		}
	}
	*/
}

void UUC_CampaignClearPopup::OnClick_Close()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_CampaignClearPopup::OnClick_Enter()
{
	this->SetVisibility(ESlateVisibility::Collapsed);

// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		RGameInstance->EventManager->OnForceSelectEpisodeDataIdx.Broadcast(CurEpisodeIdx);
// 	}
}