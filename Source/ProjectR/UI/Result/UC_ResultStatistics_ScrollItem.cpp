// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ResultStatistics_ScrollItem.h"
#include "UI/Result/BattleRecordScrollItemData.h"
#include "UtilFunctionIntegrated.h"



void UUC_ResultStatistics_ScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (false == IsValid(item))
		return;

	BattleRecordItemData = Cast<UBattleRecordScrollItemData>(item);
	if (false == IsValid(BattleRecordItemData))
		return;

	if (BattleRecordItemData->BattleRecord)
	{
		if (BattleRecordItemData->AllyRecord)
		{
			SetViewAllyRecord();
		}
		else
		{
			SetViewEnemyRecord();
		}
	}
}

void UUC_ResultStatistics_ScrollItem::SetViewAllyRecord()
{
	if (AllySwitcher)
	{
		AllySwitcher->SetActiveWidgetIndex(0);
	}

	if (MyHeroRecord && BattleRecordItemData->MyRecord)
	{
		MyHeroRecord->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		MyHeroRecord->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetRecordRankText(AllyRecordRankBox, AllyRecordRank);
	SetNickNamePortrait(AllyNickName, AllyPortraitImg);
	SetRecordValueText(AllyRecordText);

	if (EBattleRecordType::VE_AmountOfAttack == BattleRecordItemData->RecordType)
	{
		SetAttackRecordValueProgress(AllyRecord1Progress, AllyRecord2Progress);
		SetAttackRecordValueText(AllyRecord1Text, AllyRecord2Text);
	}
	else
	{
		SetRecordValueProgress(AllyRecord1Progress, AllyRecord2Progress);

		AllyRecord1Text->SetVisibility(ESlateVisibility::Collapsed);
		AllyRecord2Text->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_ResultStatistics_ScrollItem::SetViewEnemyRecord()
{
	if (AllySwitcher)
	{
		AllySwitcher->SetActiveWidgetIndex(1);
	}

	MyHeroRecord->SetVisibility(ESlateVisibility::Collapsed);

	SetRecordRankText(EnemyRecordRankBox, EnemyRecordRank);
	SetNickNamePortrait(EnemyNickName, EnemyPortraitImg);
	SetRecordValueText(EnemyRecordText);

	if (EBattleRecordType::VE_AmountOfAttack == BattleRecordItemData->RecordType)
	{
		SetAttackRecordValueProgress(EnemyRecord1Progress, EnemyRecord2Progress);
		SetAttackRecordValueText(EnemyRecord1Text, EnemyRecord2Text);
	}
	else
	{
		SetRecordValueProgress(EnemyRecord1Progress, EnemyRecord2Progress);

		EnemyRecord1Text->SetVisibility(ESlateVisibility::Collapsed);
		EnemyRecord2Text->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_ResultStatistics_ScrollItem::SetRecordRankText(UHorizontalBox* RankBox, UTextBlock* RankText)
{
	FText ValueText;
	if (RankBox && RankText)
	{
		if (5 >= BattleRecordItemData->RecordRank)
		{
			RankBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			ValueText = FText::AsNumber(BattleRecordItemData->RecordRank);
			RankText->SetText(ValueText);
		}
		else
		{
			RankBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUC_ResultStatistics_ScrollItem::SetNickNamePortrait(UTextBlock* NickNameText, UImage* PortraitImg)
{
	if (NickNameText)
	{
		FBattleRecord* Record = BattleRecordItemData->BattleRecord;
		FText ValueText;
		UObject* Portrait = nullptr;
		if (false == BattleRecordItemData->GroupPlayMode)
		{
			if (Record->CharacterID.IsValid())
			{
				if (0 < Record->UserKID)
				{
					ValueText = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Hero, Record->CharacterID);
					Portrait = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, Record->CharacterID);
				}
				else
				{
					ValueText = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Npc, Record->CharacterID);
					Portrait = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Npc, Record->CharacterID);
				}
				NickNameText->SetText(ValueText);
				UUIFunctionLibrary::SetImageBrush(Portrait, PortraitImg);
			}
		}
		else
		{
			// Need to change info from Avatar
			if (Record->CharacterID.IsValid())
			{
				if (0 <= Record->UserKID)
				{
					ValueText = FText::FromString(Record->NickName);
					Portrait = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, Record->CharacterID);
				}
				else
				{
					ValueText = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Npc, Record->CharacterID);
					Portrait = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Npc, Record->CharacterID);
				}
				NickNameText->SetText(ValueText);
				UUIFunctionLibrary::SetImageBrush(Portrait, PortraitImg);
			}
		}
	}
}

void UUC_ResultStatistics_ScrollItem::SetRecordValueText(UTextBlock* RecordText)
{
	FText ValueText;
	if (RecordText)
	{
		FBattleRecord* Record = BattleRecordItemData->BattleRecord;
		switch (BattleRecordItemData->RecordType)
		{
		case EBattleRecordType::VE_KillCount:		ValueText = FText::AsNumber(Record->nKillCount);	break;
		case EBattleRecordType::VE_AmountOfAttack:	ValueText = FText::AsNumber(Record->GetAmountOfAttack());	break;
		case EBattleRecordType::VE_AmountOfDamage:	ValueText = FText::AsNumber(Record->nAmountOfDamage);	break;
		case EBattleRecordType::VE_AmountOfHeal:	ValueText = FText::AsNumber(Record->nAmountOfHeal);	break;
		case EBattleRecordType::VE_SkillCount:		ValueText = FText::AsNumber(Record->nSkillCount);	break;
		}
		RecordText->SetText(ValueText);
	}
}

void UUC_ResultStatistics_ScrollItem::SetAttackRecordValueText(UTextBlock* RecordText1, UTextBlock* RecordText2)
{
	if (EnemyRecord1Text && EnemyRecord2Text)
	{
		FText ValueText;
		FBattleRecord* Record = BattleRecordItemData->BattleRecord;
		float Sum = (float)Record->GetAmountOfAttack();
		if(0.0f < Sum)
		{
			int32 NormalAtt = Record->nAmountOfNormalAttack;
			int32 SkillAtt = Record->nAmountOfSkillAttack;
			int32 NormalPer = int32(((float)NormalAtt / Sum) * 100.0f);
			int32 SkillPer = 100 - NormalPer;

			RecordText1->SetVisibility(ESlateVisibility::HitTestInvisible);
			RecordText2->SetVisibility(ESlateVisibility::HitTestInvisible);

			ValueText = FText::Format(FText::FromString("{0}({1}%)"), FText::AsNumber(NormalAtt), FText::AsNumber(NormalPer));
			RecordText1->SetText(ValueText);

			ValueText = FText::Format(FText::FromString("{0}({1}%)"), FText::AsNumber(SkillAtt), FText::AsNumber(SkillPer));
			RecordText2->SetText(ValueText);
		}
		else
		{
			RecordText1->SetVisibility(ESlateVisibility::Collapsed);
			RecordText2->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUC_ResultStatistics_ScrollItem::SetRecordValueProgress(UProgressBar* RecordProgress1, UProgressBar* RecordProgress2)
{
	if (RecordProgress1 && RecordProgress2)
	{
		if (0 < BattleRecordItemData->SumByRecordType)
		{
			FBattleRecord* Record = BattleRecordItemData->BattleRecord;

			float RecValue = 0.0f;
			switch (BattleRecordItemData->RecordType)
			{
			case EBattleRecordType::VE_KillCount:		RecValue = (float)Record->nKillCount;	break;
			case EBattleRecordType::VE_AmountOfDamage:	RecValue = (float)Record->nAmountOfDamage;	break;
			case EBattleRecordType::VE_AmountOfHeal:	RecValue = (float)Record->nAmountOfHeal;	break;
			case EBattleRecordType::VE_SkillCount:		RecValue = (float)Record->nSkillCount;	break;
			}

			if(0.0f < RecValue)
			{
				float Per = RecValue / (float)BattleRecordItemData->SumByRecordType;
				RecordProgress1->SetVisibility(ESlateVisibility::HitTestInvisible);
				RecordProgress1->SetPercent(Per);

				RecordProgress2->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				RecordProgress1->SetVisibility(ESlateVisibility::Collapsed);
				RecordProgress2->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			RecordProgress1->SetVisibility(ESlateVisibility::Collapsed);
			RecordProgress2->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUC_ResultStatistics_ScrollItem::SetAttackRecordValueProgress(UProgressBar* RecordProgress1, UProgressBar* RecordProgress2)
{
	if (RecordProgress1 && RecordProgress2)
	{
		float Total = (float)BattleRecordItemData->SumByRecordType;
		if (0.0f < Total)
		{
			FBattleRecord* Record = BattleRecordItemData->BattleRecord;
			float SumOfAttack = (float)Record->GetAmountOfAttack();
			if (0.0f < SumOfAttack)
			{
				float RecVlueNormal = (float)Record->nAmountOfNormalAttack;
				float RecVlueSkill = (float)Record->nAmountOfSkillAttack;

				float PerSum = SumOfAttack / Total;
				float PerNormal = RecVlueNormal / Total;
				float PerSkill = RecVlueSkill / Total;

				RecordProgress1->SetVisibility(ESlateVisibility::HitTestInvisible);
				RecordProgress1->SetPercent(PerSum);

				if(0.0f < RecVlueSkill)
				{
					RecordProgress2->SetVisibility(ESlateVisibility::HitTestInvisible);
					RecordProgress2->SetPercent(PerSkill);
				}
				else
				{
					RecordProgress2->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			else
			{
				RecordProgress1->SetVisibility(ESlateVisibility::Collapsed);
				RecordProgress2->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			RecordProgress1->SetVisibility(ESlateVisibility::Collapsed);
			RecordProgress2->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}