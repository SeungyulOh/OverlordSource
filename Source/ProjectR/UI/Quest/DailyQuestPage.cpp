
#include "ProjectR.h"
#include "DailyQuestPage.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "UI/Common/UC_Item_Icons.h"
#include "UI/Quest/UC_DailyContentsList.h"

#include "Network/PacketFunctionLibrary.h"

void UDailyQuestPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	UPacketFunctionLibrary::DAILY_QUEST_STATUS_RQ();

	URGameInstance* RGameInstnace = RGAMEINSTANCE(this);
	RGameInstnace->QuestManager->OnReceiveQuestInfo.Clear();
	RGameInstnace->QuestManager->OnReceiveQuestInfo.AddDynamic(this, &UDailyQuestPage::Refresh);

	Init();
}

void UDailyQuestPage::Init()
{
	TArray<UTextBlock*>&		rScoreTextList			= variables.DestinationStep_ScoreTextList;
	TArray<UUC_Item_Icons*>&	rItemList				= variables.DestinationStep_ItemList;
	TArray<UOverlay*>&			rCompleteImageList		= variables.DestinationStep_CompleteImageList;
	TArray<UImage*>&			rReceiveableImageList	= variables.DestinationStep_ReceiveableImageList;

	// initialize
	{
		const int32 nMax = (rCompleteImageList.Num() < rReceiveableImageList.Num()) ? rReceiveableImageList.Num() : rCompleteImageList.Num();
		for (int32 i = 0; i < nMax; ++i)
		{
			if(rCompleteImageList.IsValidIndex(i) && rCompleteImageList[i]->IsValidLowLevel())
				rCompleteImageList[i]->SetVisibility(ESlateVisibility::Collapsed);

			if(rReceiveableImageList.IsValidIndex(i) && rReceiveableImageList[i]->IsValidLowLevel())
			{
				rReceiveableImageList[i]->SetVisibility(ESlateVisibility::Collapsed);
				//rReceiveableImageList[i]->OnMouseButtonDownEvent.BindUFunction(this, "OnClickReward");
			}
		}
	}
	UTableManager* TM = UTableManager::GetInstancePtr();
	// right top side ui
	TArray<FName> RowNames = TM->GetDailyQuestRewardsRowsNames();
	for (int32 i = 0; i < RowNames.Num(); ++i)
	{
		FName& Key = RowNames[i];

		FDailyQuestRewardsTable* pTable = TM->GetDailyQuestRewardsTableRow(Key);
		if (pTable)
		{
			rScoreTextList[i]->SetText(FText::AsNumber(pTable->MinScore));

			FITEM_ICON_INFO iconinfo;
			UUIFunctionLibrary::GetItemIconInfoByWallet(pTable->WalletId, pTable->Amt, iconinfo);
			rItemList[i]->SetItemIconStructure(iconinfo, true);
		}
	}

	TArray<UUC_DailyContentsList*>& rContentsList = variables.ContentsInfoButtonList;
	RowNames.Empty();
	RowNames = TM->GetDailyQuestContentsRowsNames();
	for (int32 i = 0; i < RowNames.Num(); ++i)
	{
		FName& Key = RowNames[i];
		FDailyQuestContents* pTable = TM->GetDailyQuestContentsTableRow(Key);
		if (pTable)
		{
			if (rContentsList.IsValidIndex(i) && rContentsList[i]->IsValidLowLevel())
			{
				rContentsList[i]->SetContents(pTable->ContentsType, pTable->Score);
			}
		}
	}

}

void UDailyQuestPage::Refresh()
{
	UQuestManager* QuestManager = RGAMEINSTANCE(this)->QuestManager;

	if (QuestManager)
	{
		variables.CurrentScoreText->SetText(FText::AsNumber(QuestManager->CurrentScore));

		int32 nTotalClearCount = 0;
		for(int32 i = 0; i < QuestManager->ClearCountList.Num(); ++i)
			nTotalClearCount += QuestManager->ClearCountList[i];

		for (int32 i = 0; i < QuestManager->ClearCountList.Num(); ++i)
		{
			if(variables.ClearCountText.IsValidIndex(i) && variables.ClearCountText[i]->IsValidLowLevel())
				variables.ClearCountText[i]->SetText(FText::AsNumber(QuestManager->ClearCountList[i]));

			if (variables.ScoreRatioProgressBar.IsValidIndex(i) && variables.ScoreRatioProgressBar[i]->IsValidLowLevel())
			{
				if (0 < nTotalClearCount)
					variables.ScoreRatioProgressBar[i]->SetPercent((float)QuestManager->ClearCountList[i] / (float)nTotalClearCount);
				else
					variables.ScoreRatioProgressBar[i]->SetPercent(0);

			}
		}

		for (int32 i = 0; i < QuestManager->RewardableList.Num(); ++i)
		{
			variables.DestinationStep_CompleteImageList[i]->SetVisibility(ESlateVisibility::Collapsed);
			variables.DestinationStep_ReceiveableImageList[i]->SetVisibility(ESlateVisibility::Collapsed);

			switch (QuestManager->RewardableList[i])
			{
			case (int32)EREWARD_STATE_TYPE::NotRewardable:
				break;
			case (int32)EREWARD_STATE_TYPE::Rewardable:
				variables.DestinationStep_ReceiveableImageList[i]->SetVisibility(ESlateVisibility::Visible);
				break;
			case (int32)EREWARD_STATE_TYPE::AlreadyRewarded:
				variables.DestinationStep_CompleteImageList[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				break;
			}
		}
	}
}

void UDailyQuestPage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());

	if (CurrentTime.GetTicks() - nTickCheckTime >= FTimespan::FromSeconds(1.0f).GetTicks())
	{
		nTickCheckTime = CurrentTime.GetTicks();

		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if (RGameInstance->QuestManager == nullptr)
			return;
		int32& nRemainTime = RGameInstance->QuestManager->TimeLeft;
		int Hour = 0;
		int Min = 0;
		int Sec = 0;

		--nRemainTime;

		if (nRemainTime > 0)
		{
			Hour = nRemainTime / 3600;
			Min = (nRemainTime % 3600) / 60;
			Sec = nRemainTime % 60;

			variables.RemainHourText->SetText(UUIFunctionLibrary::Get_0n_Number(Hour));
			variables.RemainMinuteText->SetText(UUIFunctionLibrary::Get_0n_Number(Min));
			variables.RemainSecondText->SetText(UUIFunctionLibrary::Get_0n_Number(Sec));
		}

		if (nRemainTime == 0)
		{
			UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				FText::FromString(TEXT("Daily Quest Reset")),
				ECommonPopupType::VE_OK);

			UPacketFunctionLibrary::DAILY_QUEST_STATUS_RQ();
		}
	}

}

void UDailyQuestPage::OnClickReward(int32 InIndex)
{
	UPacketFunctionLibrary::DAILY_QUEST_REWARD_RQ(InIndex);
}
