// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ResultStatistics.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "UI/Control/RScrollView.h"
#include "UI/Result/BattleRecordScrollItemData.h"
#include "UI/Result/UC_ResultStatistics_ScrollItem.h"



void UUC_ResultStatistics::NativeConstruct()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		BattleRecordMgr = RGameInstance->BattleRecordManager;
		PlayModeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
	}

	Super::NativeConstruct();

	AllyRecordScrollItemDatas.Empty();
	EnemyRecordScrollItemDatas.Empty();

	AllyRecordSum.Empty();
	EnemyRecordSum.Empty();

	AvatarInfoBox->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUC_ResultStatistics::InitResultStatistics()
{
	AllyRecordSum.Init(0, RECORD_TYPE_MAX);
	EnemyRecordSum.Init(0, RECORD_TYPE_MAX);

	if (EGamePlayModeTypeEnum::VE_BattleGround == PlayModeType)
	{
		FGroupBattleRecord* MyGroupBattleRecord = BattleRecordMgr->GetGroupBattleRecord(MyGroupID);
		FGroupBattleRecord* EnemyGroupBattleRecord = BattleRecordMgr->GetGroupBattleRecord(EnemyGroupID);
		if (MyGroupBattleRecord && EnemyGroupBattleRecord)
		{
			SumGroupAllRecord(MyGroupBattleRecord, AllyRecordSum);
			SumGroupAllRecord(EnemyGroupBattleRecord, EnemyRecordSum);

			AllyRecordScrollView->Init();
			InitGroupScrollItemData(MyGroupBattleRecord, AllyRecordSum[0], AllyRecordScrollView);

			EnemyRecordScrollView->Init();
			InitGroupScrollItemData(EnemyGroupBattleRecord, EnemyRecordSum[0], EnemyRecordScrollView);

			SelectRadioTabs(EBattleRecordType::VE_KillCount);
		}
	}
	else if (EGamePlayModeTypeEnum::VE_GuildRaid == PlayModeType)
	{
		FGroupBattleRecord* MyGroupBattleRecord = BattleRecordMgr->GetGroupBattleRecord(MyGroupID);
		FGroupBattleRecord* EnemyGroupBattleRecord = BattleRecordMgr->GetGroupBattleRecord(EnemyGroupID);
		if (MyGroupBattleRecord && EnemyGroupBattleRecord)
		{
			SumGroupAllRecord(MyGroupBattleRecord, AllyRecordSum);
			SumGroupAllRecord(EnemyGroupBattleRecord, EnemyRecordSum);

			AllyRecordScrollView->Init();
			InitGroupScrollItemData(MyGroupBattleRecord, AllyRecordSum[0], AllyRecordScrollView);

			EnemyRecordScrollView->Init();
			for (FUserBattleRecord* NPCRec : EnemyGroupBattleRecord->UserBattleRecordList)
			{
				if (NPCRec)
				{
					InitUserScrollItemData(NPCRec->HeroBattleRecordList, EnemyRecordSum[0], EnemyRecordScrollView);
				}
			}

			SelectRadioTabs(EBattleRecordType::VE_KillCount);
		}
	}
	else if (EGamePlayModeTypeEnum::VE_Arena == PlayModeType || 
			 EGamePlayModeTypeEnum::VE_Colosseum == PlayModeType ||
			 EGamePlayModeTypeEnum::VE_MirrorDungeon == PlayModeType)
	{
		FUserBattleRecord* MyUserBattleRecord = BattleRecordMgr->GetUserBattleRecord(MyUserKID);
		FUserBattleRecord* EnemyUserBattleRecord = BattleRecordMgr->GetUserBattleRecord(EnemyUserKID);
		if (MyUserBattleRecord && EnemyUserBattleRecord)
		{
			MyNickName = MyUserBattleRecord->UserNickName;
			EnemyNickName = EnemyUserBattleRecord->UserNickName;

			SumAllUserRecord(MyUserBattleRecord, AllyRecordSum);
			SumAllUserRecord(EnemyUserBattleRecord, EnemyRecordSum);

			AllyRecordScrollView->Init();
			InitUserScrollItemData(MyUserBattleRecord->HeroBattleRecordList, AllyRecordSum[0], AllyRecordScrollView);

			EnemyRecordScrollView->Init();
			InitUserScrollItemData(EnemyUserBattleRecord->HeroBattleRecordList, EnemyRecordSum[0], EnemyRecordScrollView);

			SelectRadioTabs(EBattleRecordType::VE_KillCount);
		}
	}
	else
	{
		FUserBattleRecord* MyUserBattleRecord = BattleRecordMgr->GetUserBattleRecord(MyUserKID);
		FGroupBattleRecord* EnemyGroupBattleRecord = BattleRecordMgr->GetGroupBattleRecord(EnemyGroupID);
		if (MyUserBattleRecord && EnemyGroupBattleRecord)
		{
			SumAllUserRecord(MyUserBattleRecord, AllyRecordSum);

			AllyRecordScrollView->Init();
			InitUserScrollItemData(MyUserBattleRecord->HeroBattleRecordList, AllyRecordSum[0], AllyRecordScrollView);

			SumGroupAllRecord(EnemyGroupBattleRecord, EnemyRecordSum);

			EnemyRecordScrollView->Init();
			for (FUserBattleRecord* NPCRec : EnemyGroupBattleRecord->UserBattleRecordList)
			{
				if (NPCRec)
				{
					InitUserScrollItemData(NPCRec->HeroBattleRecordList, EnemyRecordSum[0], EnemyRecordScrollView);
				}
			}

			SelectRadioTabs(EBattleRecordType::VE_KillCount);
		}
	}
}

void UUC_ResultStatistics::OnClickCloseButton()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UUC_ResultStatistics::SelectRadioTabs(EBattleRecordType RecordType)
{
	if (nullptr == BattleRecordMgr)
		return;

	if (CurrentViewRecordType == RecordType)
		return;

	CurrentViewRecordType = RecordType;

	switch (PlayModeType)
	{
	case EGamePlayModeTypeEnum::VE_Arena:
	case EGamePlayModeTypeEnum::VE_Colosseum:
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
		DisplayUserBattleRecord();
		break;

	case EGamePlayModeTypeEnum::VE_BattleGround:
	case EGamePlayModeTypeEnum::VE_GuildRaid:
	case EGamePlayModeTypeEnum::VE_RaidKillBoss:
		DisplayGroupBattleRecord();
		break;

	//case EGamePlayModeTypeEnum::VE_BattleField:
	//	break;

	default:
		DisplayCampaignBattleRecord();
		break;
	}
}

void UUC_ResultStatistics::DisplayUserBattleRecord()
{
	FText NickText;
	if(false == MyNickName.IsEmpty())
	{
		NickText = FText::FromString(MyNickName);
		MyNickNameText->SetVisibility(ESlateVisibility::HitTestInvisible);
		MyNickNameText->SetText(NickText);
	}
	else
	{
		MyNickNameText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(false == EnemyNickName.IsEmpty())
	{
		NickText = FText::FromString(EnemyNickName);
		EnemyNickNameText->SetVisibility(ESlateVisibility::HitTestInvisible);
		EnemyNickNameText->SetText(NickText);
	}
	else
	{
		EnemyNickNameText->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 RecordTypeInt = (int32)CurrentViewRecordType;

	int32 MyValue = AllyRecordSum[RecordTypeInt];
	FText ValueText = FText::AsNumber(MyValue);
	MyTotalRecordText->SetText(ValueText);

	int32 EnemyValue = EnemyRecordSum[RecordTypeInt];
	ValueText = FText::AsNumber(EnemyValue);
	EnemyTotalRecordText->SetText(ValueText);

	int32 TotalValue = MyValue + EnemyValue;
	if (0 < TotalValue)
	{
		if (0 < MyValue)
		{
			float MyPercent = (float)MyValue / (float)TotalValue;
			MyTotalRecordProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			MyTotalRecordProgress->SetPercent(MyPercent);
		}
		else
		{
			MyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (0 < EnemyValue)
		{
			float EnemyPercent = (float)EnemyValue / (float)TotalValue;
			EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			EnemyTotalRecordProgress->SetPercent(EnemyPercent);
		}
		else
		{
			EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		MyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
	}

	SortItemDatas(AllyRecordScrollView, CurrentViewRecordType, AllyRecordSum);
	AllyRecordScrollView->InvalidateData();

	SortItemDatas(EnemyRecordScrollView, CurrentViewRecordType, EnemyRecordSum);
	EnemyRecordScrollView->InvalidateData();
}

void UUC_ResultStatistics::DisplayGroupBattleRecord()
{
	MyNickNameText->SetVisibility(ESlateVisibility::Collapsed);
	EnemyNickNameText->SetVisibility(ESlateVisibility::Collapsed);

	int32 RecordTypeInt = (int32)CurrentViewRecordType;

	int32 AllyValue = AllyRecordSum[RecordTypeInt];
	FText ValueText = FText::AsNumber(AllyValue);
	MyTotalRecordText->SetText(ValueText);

	int32 EnemyValue = EnemyRecordSum[RecordTypeInt];
	ValueText = FText::AsNumber(EnemyValue);
	EnemyTotalRecordText->SetText(ValueText);

	int32 TotalValue = AllyValue + EnemyValue;
	if (0 < TotalValue)
	{
		if (0 < AllyValue)
		{
			float MyPercent = (float)AllyValue / (float)TotalValue;
			MyTotalRecordProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			MyTotalRecordProgress->SetPercent(MyPercent);
		}
		else
		{
			MyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (0 < EnemyValue)
		{
			float EnemyPercent = (float)EnemyValue / (float)TotalValue;
			EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			EnemyTotalRecordProgress->SetPercent(EnemyPercent);
		}
		else
		{
			EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		MyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
	}

	SortItemDatas(AllyRecordScrollView, CurrentViewRecordType, AllyRecordSum);
	AllyRecordScrollView->InvalidateData();

	SortItemDatas(EnemyRecordScrollView, CurrentViewRecordType, EnemyRecordSum);
	EnemyRecordScrollView->InvalidateData();
}

void UUC_ResultStatistics::DisplayCampaignBattleRecord()
{
	MyNickNameText->SetVisibility(ESlateVisibility::Collapsed);
	EnemyNickNameText->SetVisibility(ESlateVisibility::Collapsed);

	int32 RecordTypeInt = (int32)CurrentViewRecordType;

	int32 MyValue = AllyRecordSum[RecordTypeInt];
	FText ValueText = FText::AsNumber(MyValue);
	MyTotalRecordText->SetText(ValueText);

	int32 EnemyValue = EnemyRecordSum[RecordTypeInt];
	ValueText = FText::AsNumber(EnemyValue);
	EnemyTotalRecordText->SetText(ValueText);

	int32 TotalValue = MyValue + EnemyValue;
	if (0 < TotalValue)
	{
		if (0 < MyValue)
		{
			float MyPercent = (float)MyValue / (float)TotalValue;
			MyTotalRecordProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			MyTotalRecordProgress->SetPercent(MyPercent);
		}
		else
		{
			MyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (0 < EnemyValue)
		{
			float EnemyPercent = (float)EnemyValue / (float)TotalValue;
			EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
			EnemyTotalRecordProgress->SetPercent(EnemyPercent);
		}
		else
		{
			EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		MyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
		EnemyTotalRecordProgress->SetVisibility(ESlateVisibility::Collapsed);
	}

	SortItemDatas(AllyRecordScrollView, CurrentViewRecordType, AllyRecordSum);
	AllyRecordScrollView->InvalidateData();

	SortItemDatas(EnemyRecordScrollView, CurrentViewRecordType, EnemyRecordSum);
	EnemyRecordScrollView->InvalidateData();
}

void UUC_ResultStatistics::SumAllUserRecord(FUserBattleRecord* UserRecord, TArray<int32>& SumList)
{
	if (nullptr == UserRecord)
		return;

	SumList[0] += UserRecord->GetSumOfKillCount();
	SumList[1] += UserRecord->GetSumOfAllAttack();
	SumList[2] += UserRecord->GetSumOfDamage();
	SumList[3] += UserRecord->GetSumOfHeal();
	SumList[4] += UserRecord->GetSumOfSkillCount();
}

void UUC_ResultStatistics::SumGroupAllRecord(FGroupBattleRecord* GroupRecord, TArray<int32>& SumList)
{
	SumList.Init(0, RECORD_TYPE_MAX);

	if (nullptr == GroupRecord)
		return;

	int32 UserCount = GroupRecord->UserBattleRecordList.Num();
	for (int32 n = 0; n < UserCount; ++n)
	{
		SumAllUserRecord(GroupRecord->UserBattleRecordList[n], SumList);
	}
}

void UUC_ResultStatistics::InitUserScrollItemData(TArray<FBattleRecord*>& RecordList, int32 InitSum, URScrollView* ScrollView)
{
	if (nullptr == ScrollView)
		return;

	int32 RecordCount = RecordList.Num();
	UBattleRecordScrollItemData* Data = nullptr;
	for (int n = 0; n < RecordCount; ++n)
	{
		Data = Cast<UBattleRecordScrollItemData>(ScrollView->AddScrollItemData(UBattleRecordScrollItemData::StaticClass()));
		if (Data)
		{
			Data->MyRecord = false;
			Data->AllyRecord = (ScrollView == AllyRecordScrollView);
			Data->GroupPlayMode = true;
			Data->RecordType = EBattleRecordType::VE_KillCount;
			Data->SumByRecordType = InitSum;
			Data->RecordRank = n + 1;
			Data->BattleRecord = RecordList[n];
		}
	}
}

void UUC_ResultStatistics::InitGroupScrollItemData(FGroupBattleRecord* GroupRecord, int32 InitSum, URScrollView* ScrollView)
{
	if (nullptr == ScrollView)
		return;

	if (nullptr == GroupRecord)
		return;

	int32 UserCount = GroupRecord->UserBattleRecordList.Num();
	UBattleRecordScrollItemData* Data = nullptr;
	FUserBattleRecord* UserRecord = nullptr;
	for (int32 n = 0; n < UserCount; ++n)
	{
		UserRecord = GroupRecord->UserBattleRecordList[n];
		if (UserRecord)
		{
			Data = Cast<UBattleRecordScrollItemData>(ScrollView->AddScrollItemData(UBattleRecordScrollItemData::StaticClass()));
			if (Data)
			{
				Data->MyRecord = (UserRecord->UserKID == MyUserKID);
				Data->AllyRecord = (ScrollView == AllyRecordScrollView);
				Data->GroupPlayMode = true;
				Data->RecordType = EBattleRecordType::VE_KillCount;
				Data->SumByRecordType = InitSum;
				Data->RecordRank = n + 1;
				Data->BattleRecord = UserRecord->GetTotalRecord();
			}
		}
	}
}

void UUC_ResultStatistics::SortItemDatas(URScrollView* ScrollView, EBattleRecordType RecordType, TArray<int32>& RecordSum)
{
	TArray<URScrollItemData*>& DataList = ScrollView->GetScrollItemAll();

	int32 RecordTypeInt = (int32)RecordType;
	int32 Count = DataList.Num();
	UBattleRecordScrollItemData* CurrentData = nullptr;
	UBattleRecordScrollItemData* CompareData = nullptr;

	bool SwapMyRecord = false;
	FBattleRecord* SwapBattleRecord = nullptr;
	int32 CurrentValue = 0;
	int32 CompareValue = 0;

	for (int32 n = 0; n < Count; ++n)
	{
		CurrentData = Cast<UBattleRecordScrollItemData>(DataList[n]);
		if (nullptr == CurrentData)
			continue;

		for (int32 s = n + 1; s < Count; ++s)
		{
			CompareData = Cast<UBattleRecordScrollItemData>(DataList[s]);
			if (nullptr == CompareData)
				continue;

			switch (RecordType)
			{
			case EBattleRecordType::VE_KillCount:
				CurrentValue = CurrentData->BattleRecord->nKillCount;
				CompareValue = CompareData->BattleRecord->nKillCount;
				break;

			case EBattleRecordType::VE_AmountOfAttack:
				CurrentValue = CurrentData->BattleRecord->GetAmountOfAttack();
				CompareValue = CompareData->BattleRecord->GetAmountOfAttack();
				break;

			case EBattleRecordType::VE_AmountOfDamage:
				CurrentValue = CurrentData->BattleRecord->nAmountOfDamage;
				CompareValue = CompareData->BattleRecord->nAmountOfDamage;
				break;

			case EBattleRecordType::VE_AmountOfHeal:
				CurrentValue = CurrentData->BattleRecord->nAmountOfHeal;
				CompareValue = CompareData->BattleRecord->nAmountOfHeal;
				break;

			case EBattleRecordType::VE_SkillCount:
				CurrentValue = CurrentData->BattleRecord->nSkillCount;
				CompareValue = CompareData->BattleRecord->nSkillCount;
				break;
			}

			if (CompareValue > CurrentValue)
			{
				SwapMyRecord = CompareData->MyRecord;
				SwapBattleRecord = CompareData->BattleRecord;

				CompareData->MyRecord = CurrentData->MyRecord;
				CompareData->BattleRecord = CurrentData->BattleRecord;

				CurrentData->MyRecord = SwapMyRecord;
				CurrentData->BattleRecord = SwapBattleRecord;

				CurrentValue = CompareValue;
			}
		}

		CurrentData->RecordType = RecordType;
		CurrentData->SumByRecordType = RecordSum[RecordTypeInt];
		if (0 < CurrentValue)
		{
			CurrentData->RecordRank = n + 1;
		}
		else
		{
			CurrentData->RecordRank = 999;
		}
	}
}
