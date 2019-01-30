// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Quest.h"

#include "GlobalIntegrated.h"






#include "UI/RWidgetManager.h"
#include "GameInfo/UserInfo.h"

#include "GameInfo/UserInfo.h"

#include "Public/Components/CanvasPanel.h"
#include "Public/Components/HorizontalBox.h"
#include "Public/Components/SizeBox.h"
#include "UtilFunctionIntegrated.h"

#include "UI/UIDefs.h"
#include "UI/Control/RScrollView.h"
#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Common/UC_Notify.h"

#include "UI/UI_Common/UP_RewardPopup.h"

#include "UI/Component/CheckBox_Radio.h"
#include "UI/Quest/UC_QuestList.h"
#include "UI/Quest/UC_QuestList_Data.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataEnum.h"
#include "Global/RGameInstance.h"



bool UUP_Quest::IsValidDatas()
{
	if (IsValid(BP_ScrollView_Quest) == false)
		return false;

	if (IsValid(CanvasPanel_AllPassQuest) == false)
		return false;

	for (auto& CheckBoxRadioElem : CheckboxRadio_Category)
	{
		if (IsValid(CheckBoxRadioElem) == false)
			return false;
	}

	for (auto& BoxElem : TwoDepth_SizeBoxDatas)
	{
		if (IsValid(BoxElem.Box) == false)
			return false;
	}

	return true;
}

void UUP_Quest::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (IsValidDatas() == false)
		return;

	PrepareWidget();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	const int32 FLOORSCROLLDATAMAX = RGameInstance->QuestManager->ImportantQuestList.Num();
	const int32 FLOORSCROLLITEMMAX = 10;

	if(!FloorScrollView->ItemDataList.Num())
		FloorScrollView->Init_NoClear(FLOORSCROLLITEMMAX);

	if (!CachedFloorScrollDataList.Num())
	{
		for (int32 i = 0; i < FLOORSCROLLDATAMAX; ++i)
		{
			UFloorScrollItemData* FloorScrollItemData = Cast<UFloorScrollItemData>(FloorScrollView->AddScrollItemData(UFloorScrollItemData::StaticClass()));
			FloorScrollItemData->WidgetSize = FloorScrollView->GetItemInitSize();

			CachedFloorScrollDataList.Emplace(FloorScrollItemData);
		}
	}

	if (!MissionScrollView->ItemDataList.Num())
		MissionScrollView->Init_NoClear(MISSIONSCROLLITEMMAX);

	if (!CachedMissionScrollDataList.Num())
	{
		for (int32 i = 0; i < MISSIONSCROLLDATAMAX; ++i)
		{
			UMissionScrollItemData* MissionScrollData = Cast<UMissionScrollItemData>(MissionScrollView->AddScrollItemData(UMissionScrollItemData::StaticClass()));
			MissionScrollData->WidgetSize = MissionScrollView->GetItemInitSize();

			CachedMissionScrollDataList.Emplace(MissionScrollData);
		}
	}
// 	MissionScrollView->Init();

	DetailMissionCategoryType	= EMissionCategoryType::VE_None;

// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if ( IsValid(RGameInstance->AchievementManager))
	{
		InvalidateData_Achievement();
		RGameInstance->AchievementManager->OnAchievementRewardRp.RemoveDynamic(this, &UUP_Quest::GetRewardRp);
		RGameInstance->AchievementManager->OnAchievementRewardRp.AddDynamic(this, &UUP_Quest::GetRewardRp);
		RGameInstance->AchievementManager->OnUpdateAchievement.RemoveDynamic(this, &UUP_Quest::InvalidateData_Achievement);
		RGameInstance->AchievementManager->OnUpdateAchievement.AddDynamic(this, &UUP_Quest::InvalidateData_Achievement);
	}
	
	if (RGameInstance->QuestManager)
	{
		RGameInstance->QuestManager->OnFloorSelect.RemoveDynamic(this, &UUP_Quest::OnFloorSelect);
		RGameInstance->QuestManager->OnFloorSelect.AddDynamic(this, &UUP_Quest::OnFloorSelect);
	}

	UPacketFunctionLibrary::DAILY_QUEST_STATUS_RQ();
	RefreshCheckBoxRadio_DetailMissionCategory();
//	InvalidateData_Achievement();
	nCurrentFloor = 0;
	InvalidateData_ImportantMission();
	QuestTypeSwitcher->SetActiveWidgetIndex((int32)EQuestTabType::DailyQuest);
	Tab_Quest->SetActiveRadioIndex((int32)EQuestTabType::DailyQuest);
}

void UUP_Quest::SetDetailCategoryNotify()
{
	//Set All
	int32 Index = 0;
	for (auto& DetailCategoryNotifyElem : DetailCategory_Notify)
	{
		if (!DetailCategory_Notify_ShowingValue.IsValidIndex(Index) || !DetailCategory_Notify.IsValidIndex(Index))
		{
			continue;
		}

		if (DetailCategory_Notify_ShowingValue[Index])
		{
			DetailCategory_Notify[Index]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			DetailCategory_Notify[Index]->SetVisibility(ESlateVisibility::Hidden);
		}
		++Index;
	}
}

void UUP_Quest::SelectActiveFloor()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nbaseIndex = 0;
	int32 nCurrentQuest = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
	int32 nFloor = 0;

	for (; nFloor < RGameInstance->QuestManager->ImportantQuestList.Num(); ++nFloor)
	{
		nbaseIndex += RGameInstance->QuestManager->ImportantQuestList[nFloor].Num();
		if (nbaseIndex >= nCurrentQuest)
			break;
	}
	URScrollItemData* SeekItem = nullptr;

	for (URScrollItemData* ItemData : FloorScrollView->ItemDataList)
	{
		UFloorScrollItemData* FloorData = Cast<UFloorScrollItemData>(ItemData);
		if (FloorData)
		{
			if (FloorData->Floor == nFloor)
			{
				SeekItem = ItemData;
				break;
			}
		}
	}

	if (SeekItem)
	{
		FloorScrollView->InvalidateData();
		FloorScrollView->SetSelectScrollListData(SeekItem);
	}
}

void UUP_Quest::OnClick_RewardIcon(int32 InIndex)
{
	UPacketFunctionLibrary::DAILY_QUEST_REWARD_RQ(InIndex);
}

void UUP_Quest::InvalidateData_ImportantMission()
{
	InvalidateFloorScrollView();
	InvalidateMissionScrollView();	

	SelectActiveFloor();
}

void UUP_Quest::InvalidateFloorScrollView()
{
	FloorScrollView->ClearScrollItemData();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	UFloorScrollItemData* FloorScrollItemData = nullptr;
	for ( int32 i = 0; i < RGameInstance->QuestManager->ImportantQuestList.Num(); ++i )
	{
		if(CachedFloorScrollDataList.IsValidIndex(i))
		{
			FloorScrollItemData = CachedFloorScrollDataList[i];
			FloorScrollItemData->Floor = i;

			FloorScrollView->ItemDataList.Emplace(FloorScrollItemData);
		}
	}

	FloorScrollView->InvalidateData();
}

void UUP_Quest::InvalidateMissionScrollView()
{
	if (!MissionScrollView->ItemDataList.Num())
		MissionScrollView->Init_NoClear(MISSIONSCROLLITEMMAX);

	//MissionScrollView->Init();
	MissionScrollView->ClearScrollItemData();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if(RGameInstance->QuestManager->ImportantQuestList.IsValidIndex(nCurrentFloor))
	{
		int32 nbaseIndex = 0;
		for (int32 i = nCurrentFloor; 0 < i; --i)
		{
			nbaseIndex += RGameInstance->QuestManager->ImportantQuestList[i].Num();
		}

		TArray<FFloorQuestData>& rList = RGameInstance->QuestManager->ImportantQuestList[nCurrentFloor];
		int32 index = 0;
		for(FFloorQuestData& data : rList)
		{
			//URScrollItemData* ScrollItemData = MissionScrollView->AddScrollItemData(UMissionScrollItemData::StaticClass());
			if(CachedMissionScrollDataList.IsValidIndex(index))
			{
				UMissionScrollItemData* MissionScrollItemData = CachedMissionScrollDataList[index];//Cast<UMissionScrollItemData>(ScrollItemData);
				MissionScrollItemData->FloorQuestKey_int = nbaseIndex + index + 1;
				MissionScrollItemData->TitleKey = data.TitleKey;
				MissionScrollItemData->DocumentKey = data.DocumentKey;
				MissionScrollItemData->TargetStringKey = data.TargetStringKey;
				MissionScrollItemData->ConditionGoal = data.ConditionGoal;
				MissionScrollItemData->nCurrentFloor = nCurrentFloor + 1;
				UUIFunctionLibrary::GetRewardsIconInfoByPreview(data.RewardKey, MissionScrollItemData->RewardItemIconInfo);

				MissionScrollView->ItemDataList.Emplace(MissionScrollItemData);
			}
			
			++index;
		}
	}

	MissionScrollView->InvalidateData();
}

void UUP_Quest::InvalidateData_Achievement()
{
	if (!BP_ScrollView_Quest->ScrollItemList.Num())
		BP_ScrollView_Quest->Init_NoClear(ACHIVEMENT_SCROLLITEM_MAX);

	auto AchievementRowNames = UTableManager::GetInstancePtr()->GetAchievementRowsNames();

	if (!CachedAchivementScrollDataList.Num())
	{
		for (size_t i = 0; i < AchievementRowNames.Num(); ++i)
		{
			UUC_QuestList_Data* ItemData = Cast<UUC_QuestList_Data>(BP_ScrollView_Quest->CreateScrollItemData(UUC_QuestList_Data::StaticClass()));
			ItemData->WidgetSize = BP_ScrollView_Quest->GetItemInitSize();
			
			CachedAchivementScrollDataList.Emplace(ItemData);
		}
	}

	DetailCategory_Notify_ShowingValue.Empty();
	DetailCategory_Notify_ShowingValue.SetNum(static_cast<int32>(EMissionCategoryType::VE_Max));
	BP_ScrollView_Quest->ClearScrollItemData();

	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->AchievementManager))
	{
		auto& AchievementStatus = RGameInstance->AchievementManager->GetAchievementStatus();
		int32 nDataIndex = 0;
		for (auto& AchievementStatusElem : AchievementStatus)
		{
			SetAchievementScrollItem(AchievementStatusElem, nDataIndex++);
		}

// 		Scroll_Quest->InvalidateData();
	}

	SetDetailCategoryNotify();

	BP_ScrollView_Quest->ItemDataList.Sort(
		[](URScrollItemData& A, URScrollItemData& B)
	{
		UUC_QuestList_Data& TargetA = (UUC_QuestList_Data&)A;
		UUC_QuestList_Data& TargetB = (UUC_QuestList_Data&)B;
		if (TargetA.bIsActive > TargetB.bIsActive)
		{
			return true;
		}
		else if (TargetA.bIsActive == TargetB.bIsActive)
		{
			if (TargetA.AchieveType > TargetB.AchieveType)
			{
				return true;
			}
			else
			{
				return false;
			}
		}


		return false;
	}
	);

	BP_ScrollView_Quest->InvalidateData();
}

void UUP_Quest::SetAchievementScrollItem(const FACHIEVE_STATUS& InAcheive_Status, int32 InDataIndex)
{
	auto RGameInstance				= RGAMEINSTANCE(this);
	auto AchivementTableEncodingKey = RGameInstance->AchievementManager->GetEncodingKey(InAcheive_Status.type, InAcheive_Status.passed_level + 1);

	auto AchievementTableInfo = UTableManager::GetInstancePtr()->GetAchievementRow(AchivementTableEncodingKey);
	if (AchievementTableInfo == nullptr)
		return;

	auto DetailMissionCategoryType_IntValue = static_cast<int32>(DetailMissionCategoryType);
	if (DetailMissionCategoryType != EMissionCategoryType::VE_None && DetailMissionCategoryType != AchievementTableInfo->MissionCategory)
	{
		auto OtherDetailMissionCategory = static_cast<int32>(AchievementTableInfo->MissionCategory);
		if (InAcheive_Status.amt >= AchievementTableInfo->passValue)
		{
			DetailCategory_Notify_ShowingValue[OtherDetailMissionCategory] = true;
		}

		return;
	}

	//find target achievement
	if ((AchievementTableInfo->type == InAcheive_Status.type && AchievementTableInfo->level == InAcheive_Status.passed_level + 1)
		|| AchievementTableInfo->level == 0)
	{
// 		auto ScrollItemData = BP_ScrollView_Quest->AddScrollItemData(UUC_QuestList_Data::StaticClass());
		if(CachedAchivementScrollDataList.IsValidIndex(InDataIndex))
		{
			auto AchievementScrollItemData = CachedAchivementScrollDataList[InDataIndex];//Cast<UUC_QuestList_Data>(ScrollItemData);
			AchievementScrollItemData->srl = InAcheive_Status.srl;
			AchievementScrollItemData->passed_level = InAcheive_Status.passed_level;
			AchievementScrollItemData->AchieveType = InAcheive_Status.type;
			AchievementScrollItemData->Shortcut = AchievementTableInfo->Shortcut;
			AchievementScrollItemData->CurrentSuccessValue = InAcheive_Status.amt;

			AchievementScrollItemData->SuccessValue = AchievementTableInfo->passValue;
			AchievementScrollItemData->SuccessRateProgressBarValue = static_cast<float>(InAcheive_Status.amt) / static_cast<float>(AchievementTableInfo->passValue);
			AchievementScrollItemData->NameString_Key = AchievementTableInfo->NameString_Key;
			AchievementScrollItemData->DescriptionKey = AchievementTableInfo->desc_key;
			AchievementScrollItemData->RewardKey = AchievementTableInfo->rewardKey;
			AchievementScrollItemData->type_pt = AchievementTableInfo->type_pt;

			if (InAcheive_Status.amt >= AchievementTableInfo->passValue)
			{
				AchievementScrollItemData->bIsActive = true;
				DetailCategory_Notify_ShowingValue[DetailMissionCategoryType_IntValue] = true;

				auto OtherDetailMissionCategory = static_cast<int32>(AchievementTableInfo->MissionCategory);
				DetailCategory_Notify_ShowingValue[OtherDetailMissionCategory] = true;
			}
			else
			{
				AchievementScrollItemData->bIsActive = false;
			}

			//#ifndef RULE_TODOMEMORY
			// 		FRewardTableInfo* RewardTableInfo = UTableManager::GetInstancePtr()->GetRewardRow(AchievementScrollItemData->RewardKey);
			// 		if (RewardTableInfo)
			//#endif
			{
				UUIFunctionLibrary::GetRewardsIconInfoByPreview(AchievementTableInfo->rewardKey, AchievementScrollItemData->RewardItemIconInfo, 0);
			}

			BP_ScrollView_Quest->ItemDataList.Emplace(AchievementScrollItemData);
		}
	}
}

void UUP_Quest::GetRewardRp(const TArray<FREWARD>& InQuestRewards)
{
	if (IsValid(AchivementRoot))
		AchivementRoot->SetVisibility(ESlateVisibility::Visible);

	auto RGameInstance = RGAMEINSTANCE(this);
	UUP_RewardPopup* RewardPopup = nullptr;
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RewardPopup = UUIFunctionLibrary::ShowRewardPopup();
	}

	if (RewardPopup->IsValidLowLevel())
	{
		//Reward Icon
		TArray<FITEM_ICON_INFO> Reward_Item_Icon_Info; 
		Reward_Item_Icon_Info.Empty();
		for (auto& QuestRewardsElem : InQuestRewards)
		{
			FITEM_ICON_INFO RewardItemIconInfo;
			UUIFunctionLibrary::GetRewardIconByRewardType(QuestRewardsElem, RewardItemIconInfo);
			Reward_Item_Icon_Info.Emplace(RewardItemIconInfo);

		}
		RewardPopup->SetRewardItemItemInfo(Reward_Item_Icon_Info, &InQuestRewards);
	}
}

void UUP_Quest::OnClick_CloseQuest()
{

}

void UUP_Quest::OnClick_Reward() 
{

}

void UUP_Quest::OnClick_CloseNotify()
{

}


void UUP_Quest::OnClick_DetailMissionCategory(EMissionCategoryType InMissionCategory)
{
	DetailMissionCategoryType = InMissionCategory;
	
	RefreshCheckBoxRadio_DetailMissionCategory();
	InvalidateData_Achievement();
}

void UUP_Quest::OnClick_Tab(EQuestTabType eTabType)
{
	QuestTypeSwitcher->SetActiveWidgetIndex((int32)eTabType);
// 	switch (eTabType)
// 	{
// 	case EQuestTabType::DailyQuest:
// 		QuestTypeSwitcher->SetActiveWidgetIndex(0);
// 		break;
// 	case EQuestTabType::ImportantMissionTab:
// 		QuestTypeSwitcher->SetActiveWidgetIndex(1);
// 		break;
// 	case EQuestTabType::AchieveTab:
// 		QuestTypeSwitcher->SetActiveWidgetIndex(2);
// 		break;
// 	}
}

void UUP_Quest::OnFloorSelect(int32 InFloor)
{
	nCurrentFloor = InFloor;
	InvalidateMissionScrollView();
}

void UUP_Quest::RefreshCheckBoxRadio_DetailMissionCategory()
{
	auto DetailMIssionCategoryTypeEumIndex = static_cast<int32>(DetailMissionCategoryType);

	int32 Index = 0;
	for (auto& SwitcherElem : DetailMissionCategory)
	{
		if (Index == DetailMIssionCategoryTypeEumIndex)
		{
			SwitcherElem->SetActiveWidgetIndex(0);
		}
		else
		{
			SwitcherElem->SetActiveWidgetIndex(1);
		}

		++Index;
	}
}



void UUP_Quest::NativeDestruct()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
// 		RGAMEINSTANCE(this)->EventManager->UpdateDailyQuestStatus_Delegate.RemoveDynamic(this, &UUP_Quest::InvalidateData);
// 		RGAMEINSTANCE(this)->EventManager->OnQuestRewardRp_Delegate.RemoveDynamic(this, &UUP_Quest::GetRewardRp);

/*		RGameInstance->AchievementManager->OnUpdateAchievement.RemoveDynamic(this, &UUP_Quest::InvalidateData);*/
		if(RGameInstance->AchievementManager)
			RGameInstance->AchievementManager->OnAchievementRewardRp.RemoveDynamic(this, &UUP_Quest::GetRewardRp);
	}

	Super::NativeDestruct();
}

void UUP_Quest::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UUP_Quest::PrepareWidget()
{
	CanvasPanel_AllPassQuest->SetVisibility(ESlateVisibility::Collapsed);

	TWeakObjectPtr<UWidgetSwitcher> Widget = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("BP_WidgetSwitcher_All")));
	if (Widget.IsValid())
		DetailMissionCategory.Emplace(Widget.Get());

	Widget = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("BP_WidgetSwitcher_Normal")));
	if (Widget.IsValid())
		DetailMissionCategory.Emplace(Widget.Get());

	Widget = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("BP_WidgetSwitcher_Hero")));
	if (Widget.IsValid())
		DetailMissionCategory.Emplace(Widget.Get());

	Widget = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("BP_WidgetSwitcher_Item")));
	if (Widget.IsValid())
		DetailMissionCategory.Emplace(Widget.Get());

	Widget = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("BP_WidgetSwitcher_Adventure")));
	if (Widget.IsValid())
		DetailMissionCategory.Emplace(Widget.Get());

	Widget = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("BP_WidgetSwitcher_Combat")));
	if (Widget.IsValid())
		DetailMissionCategory.Emplace(Widget.Get());

	Widget = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("BP_WidgetSwitcher_Guild")));
	if (Widget.IsValid())
		DetailMissionCategory.Emplace(Widget.Get());
		

	TWeakObjectPtr<UUC_Notify> NotifyWidget = Cast<UUC_Notify>(GetWidgetFromName(TEXT("BP_Notify_All")));
	if (NotifyWidget.IsValid())
		DetailCategory_Notify.Emplace(NotifyWidget.Get());

	NotifyWidget = Cast<UUC_Notify>(GetWidgetFromName(TEXT("BP_Notify_Achieve")));
	if (NotifyWidget.IsValid())
		DetailCategory_Notify.Emplace(NotifyWidget.Get());

	NotifyWidget = Cast<UUC_Notify>(GetWidgetFromName(TEXT("BP_Notify_Hero")));
	if (NotifyWidget.IsValid())
		DetailCategory_Notify.Emplace(NotifyWidget.Get());

	NotifyWidget = Cast<UUC_Notify>(GetWidgetFromName(TEXT("BP_Notify_Item")));
	if (NotifyWidget.IsValid())
		DetailCategory_Notify.Emplace(NotifyWidget.Get());

	NotifyWidget = Cast<UUC_Notify>(GetWidgetFromName(TEXT("BP_Notify_Adventure")));
	if (NotifyWidget.IsValid())
		DetailCategory_Notify.Emplace(NotifyWidget.Get());

	NotifyWidget = Cast<UUC_Notify>(GetWidgetFromName(TEXT("BP_Notify_Combat")));
	if (NotifyWidget.IsValid())
		DetailCategory_Notify.Emplace(NotifyWidget.Get());

	NotifyWidget = Cast<UUC_Notify>(GetWidgetFromName(TEXT("BP_Notify_Guild")));
	if (NotifyWidget.IsValid())
		DetailCategory_Notify.Emplace(NotifyWidget.Get());

	PlayAni("In");
}

void UFloorScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

}

void UFloorScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFloorScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (item == nullptr)
		return;

	UFloorScrollItemData* FloorData = Cast<UFloorScrollItemData>(item);
	if (FloorData == nullptr)
		return;
	
	SelectStateSwitcher->SetActiveWidgetIndex(FloorData->bSelected ? 0 : 1);
	
	FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_FloorShop_FloorControl")));
	text = FText::Format(text, FText::AsNumber(FloorData->Floor + 1));
	ActiveText->SetText(text);

	InActiveText->SetText(text);
}

void UFloorScrollItem::SetSelected(bool bSelected)
{
//	Super::SetSelected(bSelected);
	SelectStateSwitcher->SetActiveWidgetIndex(bSelected ? 0 : 1);
	UFloorScrollItemData* data = Cast<UFloorScrollItemData>(ItemData);

	if(bSelected && data)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		RGameInstance->QuestManager->OnFloorSelect.Broadcast(data->Floor);
	}
}

void UMissionScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (GotoTargetButton)
	{
		GotoTargetButton->OnClicked.RemoveDynamic(this, &UMissionScrollItem::OnClickGotoTarget);
		GotoTargetButton->OnClicked.AddDynamic(this, &UMissionScrollItem::OnClickGotoTarget);
	}
}

void UMissionScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMissionScrollItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bAniPlaying)
	{
		fCurrentDeltaTime += InDeltaTime;
		if (0.5f <= fCurrentDeltaTime)
		{
			bAniPlaying = false;
			SetSizeChange(FVector2D(993.0f, bReverse ? ItemBigSize : ItemSmallSize), true);
			InvalidateData();
			return;
		}

		float fStart = ItemBigSize;
		float fDestination = ItemSmallSize;
		
		if (bReverse)
		{
			fStart = ItemSmallSize;
			fDestination = ItemBigSize;
		}

		
		float fY = FMath::Lerp<float>(fStart, fDestination, fCurrentDeltaTime / 0.5f);

		SetSizeChange(FVector2D(993.0f, fY), true);
		InvalidateData();
	}
}

void UMissionScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (item == nullptr)
		return;

	UMissionScrollItemData* MissionData = Cast<UMissionScrollItemData>(item);
	if (MissionData == nullptr)
		return;

	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 nCurrentMission = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
	int32 nDataMission = MissionData->FloorQuestKey_int;
	SetStateSwitcher(nCurrentMission, nDataMission);
	bool bProgress = nCurrentMission == nDataMission;

	FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, MissionData->TitleKey);
	TitleText->SetText(text);

	if (bProgress)
		TitleText->SetColorAndOpacity(FSlateColor(ProgressColor_Blue));
	else
		TitleText->SetColorAndOpacity(FSlateColor(OtherColor_Gray));

	text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, MissionData->DocumentKey);
	DocumentText->SetRichText(text);

	text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, MissionData->TargetStringKey);
	TargetRichText->SetRichText(text);

	
	if(nCurrentMission < nDataMission)
	{
		CurrentCountText->SetText(FText::AsNumber(0));
		MaxCountText->SetText(FText::AsNumber(MissionData->ConditionGoal));
	}
	else if (nCurrentMission > nDataMission)
	{
		CurrentCountText->SetText(FText::AsNumber(MissionData->ConditionGoal));
		MaxCountText->SetText(FText::AsNumber(MissionData->ConditionGoal));

	}
	else// if (nCurrentMission == nDataMission)
	{
		CurrentCountText->SetText(FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.progress));
		MaxCountText->SetText(FText::AsNumber(MissionData->ConditionGoal));
	}

	for (int32 i = 0; i < RewardBoxList.Num(); ++i)
	{
		if (RewardBoxList.IsValidIndex(i) && RewardBoxList[i]->IsValidLowLevel())
			RewardBoxList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 i = 0; i < MissionData->RewardItemIconInfo.Num(); ++i)
	{
		FITEM_ICON_INFO& RewardItemIconInfoElem = MissionData->RewardItemIconInfo[i];
		
		if (RewardBoxList.IsValidIndex(i) && RewardBoxList[i]->IsValidLowLevel())
			RewardBoxList[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		if (RewardItemList.IsValidIndex(i))
			RewardItemList[i]->SetItemIconStructure(RewardItemIconInfoElem);
	}
}

void UMissionScrollItem::SetStateSwitcher(int32 CurrentProgress, int32 itemkey)
{
	StopAni(TEXT("ContinueAni"));
	
	// CurrentProgress == 0 : 모든미션 클리어한 상태
	if (CurrentProgress == 0 || CurrentProgress > itemkey)
	{
		const int32 nCompleteIndex = 1;

		MissionStateSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MissionStateSwitcher->SetActiveWidgetIndex(nCompleteIndex);
	}
	else if (CurrentProgress < itemkey)
	{
		MissionStateSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}
	else //if(progress == itemkey)
	{
		const int32 nProgressIndex = 0;

		MissionStateSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MissionStateSwitcher->SetActiveWidgetIndex(nProgressIndex);

		PlayAni(TEXT("ContinueAni"), false, 0);

	}
}

void UMissionScrollItem::SetSelected(bool bSelected)
{
	UMissionScrollItemData* MissionData = Cast<UMissionScrollItemData>(ItemData);
	if (MissionData == nullptr)
		return;
	if(MissionData->bSelected != bSelected)
	{
		PlayAni(TEXT("CloseSlotAni"), !bSelected);
		bReverse = bSelected;
		bAniPlaying = true;
		fCurrentDeltaTime = 0.0f;
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nCurrentMission = RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.questId;
	int32 nDataMission = MissionData->FloorQuestKey_int;
	bool bProgress = (nCurrentMission == nDataMission);
	if (!bProgress)
	{
		if(bSelected)
			TitleText->SetColorAndOpacity(FSlateColor(ActiveColor_Blue));
		else
			TitleText->SetColorAndOpacity(FSlateColor(OtherColor_Gray));
	}

}

void UMissionScrollItem::OnClickGotoTarget()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	auto ContentsUnLockManager = RGameInstance->ContentsUnLockManager;
	UMissionScrollItemData* pData = Cast<UMissionScrollItemData>(this->ItemData);

	if (pData && !pData->bSelected)
	{
		ForceSelect();
	}
	else
	{
		//UPacketFunctionLibrary::TOWER_CHANGE_FLOOR_RQ(pData->nCurrentFloor);
		if (ContentsUnLockManager->IsContentsUnlock(EGamePlayModeTypeEnum::VE_Campaign, pData->nCurrentFloor))
		{
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_Campaign);
			RGameInstance->QuestManager->UseNavigation();
		}
		else
		{
			FText title;
			FText desc;

			ContentsUnLockManager->GetUnlockReasonText(EGamePlayModeTypeEnum::VE_Campaign, 
				title, desc, pData->nCurrentFloor);
			UUIFunctionLibrary::ShowCommonPopupSplash(title, desc);
		}
	}
}
