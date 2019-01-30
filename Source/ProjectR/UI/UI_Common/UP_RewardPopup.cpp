// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_RewardPopup.h"
#include "UtilFunctionIntegrated.h"
#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"

#include "Table/CurrencyPropertyTableInfo.h"
#include "UI/Common/UC_Item_Icons.h"


void UUP_RewardPopup::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgets<UUC_Item_Icons>(this, ItemIconList);
	FindChildWidgetsWithCertainName<UTextBlock>(this, ItemTextBlockList, TEXT("Item_Text_0"));
	FindChildWidgetsWithCertainName<UPanelWidget>(this, ItemPanelList, TEXT("RewardArray_0"));

	PlayAni(TEXT("Popup_Appear"));

	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/UP_Campaign_Campaign_Stage_Appear_Cue.UP_Campaign_Campaign_Stage_Appear_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	RefreshUI();

	if (IsValid(VerticalBox_QuestGuide))
	{
		VerticalBox_QuestGuide->OnClicked.RemoveDynamic(this, &UUP_RewardPopup::Click_VerticalBox);
		VerticalBox_QuestGuide->OnClicked.AddDynamic(this, &UUP_RewardPopup::Click_VerticalBox);
	}

}

void UUP_RewardPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlayHideAni)
	{
		if (!IsPlayAni("Popup_Appear"))
		{
			bPlayHideAni = false;
			RemoveFromViewport();
		}
	}
}

void UUP_RewardPopup::HideAniPlay()
{
	if (bPlayHideAni)
		return;
	bPlayHideAni = true;
	PlayAni("Popup_Appear", true);
	Click_Ok();
}

void UUP_RewardPopup::InitRewardPopup()
{
	OnClickOK.Clear();

// 	Caption = InCaption;
	
	ItemIconInfoList.Empty();
}

void UUP_RewardPopup::SetRewardItem(FITEM& ItemData, bool bFirst)
{
	ItemIconInfoList.Empty();

	FITEM_ICON_INFO info;
	if (UUIFunctionLibrary::GetItemIconInfoWithUD(info, ItemData.itemUD))
	{
		ItemIconInfoList.Emplace(info);
	}
	bFirstReward = bFirst;
	RefreshUI();
}

void UUP_RewardPopup::SetRewardItems(TArray<FITEM>& ItemDataList, bool bFirst)
{
	ItemIconInfoList.Empty(ItemDataList.Num());

	for (auto& ItemData : ItemDataList)
	{
		FITEM_ICON_INFO info;
		if (UUIFunctionLibrary::GetItemIconInfoWithUD(info, ItemData.itemUD))
		{
			ItemIconInfoList.Emplace(info);
		}
	}
	bFirstReward = bFirst;
	RefreshUI();
}

void UUP_RewardPopup::SetGrindRewardItems(TArray<FITEM>& ItemDataList, TArray<int32>& BeforeItemCounts)
{
	ItemIconInfoList.Empty(ItemDataList.Num());

	int32 Index = 0;
	for (auto& ItemData : ItemDataList)
	{
		FITEM_ICON_INFO info;
		if (UUIFunctionLibrary::GetItemIconInfoWithUD(info, ItemData.itemUD))
		{
			if (BeforeItemCounts.IsValidIndex(Index))
			{
				info.count -= BeforeItemCounts[Index];
			}
			ItemIconInfoList.Emplace(info);
			++Index;
		}
	}
	
	RefreshUI();
}

void UUP_RewardPopup::SetRewardItemItemInfo(TArray<FITEM_ICON_INFO>& InItemIconInfoList, const TArray<FREWARD>* InRewards, bool bFirst)
{
	ItemIconInfoList = InItemIconInfoList;

	if(InRewards != nullptr)
	{
		Rewards = *(InRewards);
	}
	bFirstReward = bFirst;
	RefreshUI();
}

void UUP_RewardPopup::SetQuestRewardItem(TArray<FITEM_ICON_INFO>& InItemIconInfoList, const TArray<FREWARD>* InRewards /*= nullptr*/, FText InQuestText)
{
	ItemIconInfoList = InItemIconInfoList;

	if (InRewards != nullptr)
	{
		Rewards = *(InRewards);
	}

	bFirstReward = false;
	bQuestReward = true;
	QuestText = InQuestText;
	RefreshUI();
}

void UUP_RewardPopup::RefreshUI()
{
	UPanelWidget* ItemPanel = nullptr;
	UUC_Item_Icons* ItemIcon = nullptr;
	UTextBlock* ItemTextBlock = nullptr;
	
	if(IsValid(LocalizingTextBlock_FirstRewardLabel))
	{
		LocalizingTextBlock_FirstRewardLabel->SetVisibility(bFirstReward ? (ESlateVisibility::SelfHitTestInvisible) : (ESlateVisibility::Collapsed));
	}

	if(IsValid(QuestRewardsText))
	{
		QuestRewardsText->SetVisibility(bQuestReward ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		QuestRewardsText->SetText(QuestText);
	}

	int ItemIndex = 0;
	for (; ItemIndex < MAX_REWARD_ITEM_COUNT; ++ItemIndex)
	{
		if (!ItemPanelList.IsValidIndex(ItemIndex) ||
			!ItemIconList.IsValidIndex(ItemIndex) ||
			!ItemTextBlockList.IsValidIndex(ItemIndex))
		{
			continue;
		}

		ItemPanel = ItemPanelList[ItemIndex];
		ItemIcon = ItemIconList[ItemIndex];
		ItemTextBlock = ItemTextBlockList[ItemIndex];

		ESlateVisibility PanelVisible = ESlateVisibility::Collapsed;
		if (ItemIconInfoList.IsValidIndex(ItemIndex))
		{
			FITEM_ICON_INFO ItemIconInfo = ItemIconInfoList[ItemIndex];

			ItemIcon->SetItemIconStructure(ItemIconInfo);				

			switch (ItemIconInfo.ItemCategory)
			{
				case EREWARD_TYPE::WALLET:
				{
					FWalletTableInfo * walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(Rewards[ItemIndex].rewardId);

					if (walletInfo == nullptr)
						break;

					ItemTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, walletInfo->CommonStringKey));
				}
				break;
				case EREWARD_TYPE::NONE:
				{
					FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemIconInfo.itemId);
					if (ItemTableInfo)
					{
						ItemTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->ItemName));
						break;
					}
					
					FCharacterTableInfo* pCharacter = UTableManager::GetInstancePtr()->GetCharacterRow(ItemIconInfo.ToolTipID);
					if(pCharacter)
					{
						ItemTextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, pCharacter->DisplayName));
						break;
					}
				}
				break;
			}
			
			
			PanelVisible = ESlateVisibility::Visible;
		}

		ItemPanel->SetVisibility(PanelVisible);
	}

	bFirstReward = false;
	bQuestReward = false;
	QuestText = FText::GetEmpty();
}

void UUP_RewardPopup::Click_Ok()
{
	OnClickOK.Broadcast();
	OnClickFirstRewardOK.Broadcast();
}

void UUP_RewardPopup::Click_VerticalBox()
{
	UUIFunctionLibrary::HideRewardPopup();
}
