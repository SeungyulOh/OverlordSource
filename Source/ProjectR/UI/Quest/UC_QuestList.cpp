// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_QuestList.h"

#include "Public/Components/Image.h"
#include "Public/Components/TextBlock.h"
#include "Public/Components/ProgressBar.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"

#include "UI/Common/UC_Item_Icons.h"
#include "UI/Quest/UC_QuestList_Data.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UI/UIDefs.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataEnum.h"
#include "Network/PacketDataStructures.h"
#include "GameInfo/UserInfo.h"




void UUC_QuestList::NativeConstruct()
{
	Super::NativeConstruct();

	//if (IsValidData() == false)
	//	return;

	FindChildWidgets<UUC_Item_Icons>(this, RewardItemIcons);

	TArray<UCanvasPanel*> CanvasArray;
	FindChildWidgets<UCanvasPanel>(this, CanvasArray);

	if (Btn_Move)
	{
		Btn_Move->OnClicked.RemoveDynamic(this, &UUC_QuestList::OnClick_GotoContent);
		Btn_Move->OnClicked.AddDynamic(this, &UUC_QuestList::OnClick_GotoContent);
	}

	if (Btn_Success)
	{
		Btn_Success->OnClicked.RemoveDynamic(this, &UUC_QuestList::OnClick_AcceptReward);
		Btn_Success->OnClicked.AddDynamic(this, &UUC_QuestList::OnClick_AcceptReward);
	}
}

bool UUC_QuestList::IsValidData()
{
	if (IsValid(Image_QuestType) == false)
		return false;
	if (IsValid(LocalizingText_QuestType) == false)
		return false;

	if (IsValid(LocalizingText_QuestDescription) == false)
		return false;
	if (IsValid(Text_CurrentSuccessValue) == false)
		return false;
	if (IsValid(Text_SuccessValue) == false)
		return false;
	if (IsValid(ProgressBar_Success) == false)
		return false;
	if (IsValid(Btn_Move) == false)
		return false;
	if (IsValid(Btn_Success) == false)
		return false;
	if (IsValid(WidgetSwitcher_SuccessAndMove) == false)
		return false;

	for (auto& RewardItemIconELem : RewardItemIcons)
	{
		if (IsValid(RewardItemIconELem) == false)
			return false;
	}
	return true;
}

void UUC_QuestList::SetQuestList()
{
	if (IsValidData() == false)
		return;

	if (IsValid(ItemData))
	{
		auto QuestList_Data = Cast<UUC_QuestList_Data>(ItemData);
		if (IsValid(QuestList_Data))
		{
			// type image
			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(QuestList_Data->type_pt), Image_QuestType);
			

			//Quest Name
			LocalizingText_QuestType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Name, QuestList_Data->NameString_Key));
			//Quest Description
			LocalizingText_QuestDescription->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Description, QuestList_Data->DescriptionKey));


			//Quest Current Value
			Text_CurrentSuccessValue->SetText(FText::AsNumber(QuestList_Data->CurrentSuccessValue));

			//Quest Success Value
			Text_SuccessValue->SetText(FText::AsNumber(QuestList_Data->SuccessValue));

			//ProgressBar
			ProgressBar_Success->SetPercent(QuestList_Data->SuccessRateProgressBarValue);

			//Success & Move Btn 
			if (QuestList_Data->CurrentSuccessValue >= QuestList_Data->SuccessValue)
			{
				WidgetSwitcher_SuccessAndMove->SetActiveWidgetIndex(1);
			}
			else
			{
				WidgetSwitcher_SuccessAndMove->SetActiveWidgetIndex(0);
				
				if (Btn_Move)
				{
					if (QuestList_Data->Shortcut == EUIMenuIDEnum::VE_None)
					{
						//Btn_Move->SetIsEnabled(false);
						Btn_Move->SetVisibility(ESlateVisibility::Collapsed);
					}						
					else
					{
						//Btn_Move->SetIsEnabled(true);
						Btn_Move->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					}						
				}
			}

			//SetIsEnabled(QuestList_Data->bIsActive);

			if (QuestList_Data->bIsActive == true)
			{
				//accept
				if (IsValid(WidgetSwitcher_SuccessAndMove))
				{
					WidgetSwitcher_SuccessAndMove->SetActiveWidgetIndex(1);
				}
				SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				//move
				if (IsValid(WidgetSwitcher_SuccessAndMove))
				{
					WidgetSwitcher_SuccessAndMove->SetActiveWidgetIndex(0);
				}
				//SetColorAndOpacity(FLinearColor(0.25f, 0.25f, 0.25f, 1.0f));
			}

			//RewardItemIcon
			for (auto& RewardItemIconElem : RewardItemIcons)
			{
				RewardItemIconElem->SetVisibility(ESlateVisibility::Collapsed);
			}

			int32 RewardItemIconIndex = 0;
			for (auto& RewardItemIconInfoELem : QuestList_Data->RewardItemIconInfo)
			{
				if (RewardItemIcons.IsValidIndex(RewardItemIconIndex))
				{
					RewardItemIcons[RewardItemIconIndex]->SetVisibility(ESlateVisibility::Visible);
					RewardItemIcons[RewardItemIconIndex]->SetItemIconStructure(RewardItemIconInfoELem);					
					++RewardItemIconIndex;
				}
			}
		}
	}
}

void UUC_QuestList::OnClick_AcceptReward()
{
	AcceptAchievementReward();
}

void UUC_QuestList::OnClick_GotoContent()
{
	UUC_QuestList_Data* QuestList_Data = Cast<UUC_QuestList_Data>(ItemData);
	if (IsValid(QuestList_Data))
	{		
		if(IsUnlockContents(QuestList_Data->Shortcut))
			UUIFunctionLibrary::GoContents( QuestList_Data->Shortcut );
	}
}

bool UUC_QuestList::IsUnlockContents(EUIMenuIDEnum InContents)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if(InContents == EUIMenuIDEnum::VE_Campaign)
	{
		if (!RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Campaign, 1))
		{
			FText title;
			FText desc;

			RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, title, desc, 1);
			UUIFunctionLibrary::ShowCommonPopupSplash(title, desc);
			return false;
		}
	}
	else if (InContents == EUIMenuIDEnum::VE_Arena)
	{
		if (!RGameInstance->ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Arena))
		{
			FText title;
			FText desc;

			RGameInstance->ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Arena, title, desc);
			UUIFunctionLibrary::ShowCommonPopupSplash(title, desc);

			return false;
		}
	}

	return true;
}

void UUC_QuestList::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	SetQuestList();
}

void UUC_QuestList::AcceptAchievementReward()
{
	auto AchievementItemData = Cast<UUC_QuestList_Data>(ItemData);
	UPacketFunctionLibrary::GIVE_ACHIEVE_REWARD_RQ(AchievementItemData->srl, AchievementItemData->passed_level + 1);
}