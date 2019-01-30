// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroIcon_With_Exp.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Common/HeroScrollItemData.h"
#include "UtilFunctionIntegrated.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "GameInfo/RewardInfo.h"
#include "SharedConstants/GlobalConstants.h"


void UUC_HeroIcon_With_Exp::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ProgressBar_Addition_Exp) == false)
		return;
	if (IsValid(ProgressBar_Base_Exp) == false)
		return;
	if (IsValid(Image_LevelUp_Result) == false)
		return;
	if (IsValid(Text_AddExp) == false)
		return;
}

void UUC_HeroIcon_With_Exp::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	if (item == nullptr)
	{
		return;
	}

	auto HeroScrollItemData = Cast<UHeroScrollItemData>(item);
	if (HeroScrollItemData == nullptr)
	{
		return;
	}

	HeroIconPtr->SetHeroIconInfoCopy(HeroScrollItemData->HeroIconInfo);
}

void UUC_HeroIcon_With_Exp::ShowAddExp(int32 InAddExp)
{
	auto RGameInstance = RGAMEINSTANCE(this);

	
	{
		auto RewardInfo = RGameInstance->RewardInfo;
		

		if (IsValid(RewardInfo))
		{
			auto ClearGrade = RewardInfo->GetClearGrade();

			switch (ClearGrade)
			{
			case EPLAY_CLEAR_GRADE::CLEAR_FAILED:
				Text_AddExp->SetVisibility(ESlateVisibility::Hidden);
				break;

			default:
				Text_AddExp->SetVisibility(ESlateVisibility::Visible);
				Text_AddExp->SetText(FText::AsNumber(InAddExp));

				if (HeroIconPtr)
				{
					FHERO_ICON_INFO HeroIconInfo;
					HeroIconPtr->GetHeroIconInfo(HeroIconInfo);

					auto HeroInfosBeforGetReward = RGameInstance->RewardInfo->GetPlayDeckHeroInfoBeforeGetReward();
					auto TargetHero = HeroInfosBeforGetReward.FindByPredicate(
						[&](const FHERO HeroInfo) 
						{
						if (HeroInfo.heroUD == HeroIconInfo.heroUD)
							return true;
						return false;
					});

					if (TargetHero == nullptr)
					{
						Image_LevelUp_Result->SetVisibility(ESlateVisibility::Hidden);
						return;
					}
					
					if (HeroIconInfo.heroUD.Equals("") == false && HeroIconInfo.heroId.IsEqual("") == false)
					{
// 						int32 MaxExp = HeroIconPtr->HeroInfo.MaxExp;
// 						int32 CurrentExp = HeroIconPtr->HeroInfo.exp;
// 						int32 AcquiredExp = HeroIconPtr->HeroInfo.acquiredExp;
						if(TargetHero->level < HeroIconInfo.level)
							Image_LevelUp_Result->SetVisibility(ESlateVisibility::Visible);
						else
							Image_LevelUp_Result->SetVisibility(ESlateVisibility::Hidden);

// 						if (MaxExp <= CurrentExp + AcquiredExp)
// 						{
// 							Image_LevelUp_Result->SetVisibility(ESlateVisibility::Visible);
// 						}
// 						else
// 						{
// 							Image_LevelUp_Result->SetVisibility(ESlateVisibility::Hidden);
// 						}
					}
				}
			}
		}
	}
}