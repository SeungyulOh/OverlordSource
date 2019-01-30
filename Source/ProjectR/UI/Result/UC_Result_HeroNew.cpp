// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Result_HeroNew.h"
#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"

#include "Table/CharacterTableInfo.h"
#include "Network/PacketDataStructures.h"


void UUC_Result_HeroNew::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Result_HeroNew::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bProgressShowStart)
	{
		CurrentTime += InDeltaTime;

		int32 HeroExpMax = 1;

		FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(CurrentLevel)));
		if (tableInfo)
		{
			//HeroExpMax = tableInfo->HeroMaxExp;
		}

		if (CurrentTime >= ShowTime)
		{
			CurrentTime = ShowTime;
			bProgressShowStart = false;
		}

		float CurValue = 0.0f;
		if (bProgressShowStart)
		{
			CurValue = StartValue + DiffValue * CurrentTime / ShowTime - MinusValue;

			if (CurValue >= (float)HeroExpMax)
			{
				MinusValue += (float)HeroExpMax;
				CurValue -= (float)HeroExpMax;

				++CurrentLevel;
				tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(CurrentLevel)));
				if (tableInfo)
				{
// 					HeroExpMax = tableInfo->HeroMaxExp;
					CurrentMax = HeroExpMax;
				}

				if (IsValid(Image_Up))
				{
					if (Image_Up->GetVisibility() == ESlateVisibility::Collapsed)
					{
						Image_Up->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					}
				}
			}
		}
		else
		{
			CurValue = EndValue;

			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if ( IsValid(RGameInstance->RInventory))
			{
				FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(ResultHeroIconInfo.heroUD);
				if (outHero != nullptr)
				{
					CurrentLevel = outHero->level;

					tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(CurrentLevel)));
					if (tableInfo)
					{
// 						HeroExpMax = tableInfo->HeroMaxExp;
						CurrentMax = HeroExpMax;
					}
				}
			}
		}

		if (IsValid(TextBlock_Level))
		{
			TextBlock_Level->SetText(FText::AsNumber(CurrentLevel));
		}

		float RewardExpPercent = (float)CurValue / (float)CurrentMax;
		if (IsValid(BP_ProgressBar_Success) && IsValid(TextBlock_ExpProgress))
		{
			BP_ProgressBar_Success->SetPercent(RewardExpPercent);
			TextBlock_ExpProgress->SetText(FText::AsPercent(RewardExpPercent));
		}
	}
}

void UUC_Result_HeroNew::UpdateInfo(FHERO_ICON_INFO& InHeroIcon, FHeroSlotInfo& InSlotInfo)
{
	ResultHeroIconInfo = InHeroIcon;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->RInventory))
	{
		FHERO* outHero = RGameInstance->RInventory->GetHeroWithUD(ResultHeroIconInfo.heroUD);
		if (outHero != nullptr)
		{
			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ResultHeroIconInfo.heroId);
			if (CharacterTableInfo)
			{
				if (IsValid(Image_HeroCard))
				{
					FString text = TEXT("Campaign_BG_Hero_") + CharacterTableInfo->DisplayName.ToString();
					Image_HeroCard->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(FName(*text)));
				}
				/*
				if (ResultHeroIconInfo.acquiredExp == 0)
				{
					if (IsValid(LevelText))
					{
						LevelText->SetText(FText::AsNumber(outHero.level));
					}

					float RewardExpPercent = (float)outHero.exp / (float)ResultHeroIconInfo.MaxExp;
					if (IsValid(ExpProgressBar) && IsValid(ExpProgressText))
					{
						ExpProgressBar->SetPercent(RewardExpPercent);
						ExpProgressText->SetText(FText::AsPercent(RewardExpPercent));
					}					
				}
				else
				{
					int32 currentLevel = outHero.level;
					int32 acquireExp = ResultHeroIconInfo.acquiredExp;
					int32 heroExp = outHero.exp;
					while (heroExp < acquireExp)
					{
						acquireExp -= heroExp;
						--currentLevel;
						FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(currentLevel)));
						if (tableInfo)
						{
							heroExp = tableInfo->HeroMaxExp;
						}
					}

					heroExp -= acquireExp;

					CurrentLevel = currentLevel;
					DiffValue = (float)ResultHeroIconInfo.acquiredExp;
					StartValue = heroExp;
					EndValue = outHero.exp;

					FLevelGrowthTableInfo* tableInfo1 = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(currentLevel)));
					if (tableInfo1)
					{
						CurrentMax = tableInfo1->HeroMaxExp;
					}

					if (IsValid(LevelText))
					{
						LevelText->SetText(FText::AsNumber(CurrentLevel));
					}

					float RewardExpPercent = (float)heroExp / (float)CurrentMax;
					if (IsValid(ExpProgressBar) && IsValid(ExpProgressText))
					{
						ExpProgressBar->SetPercent(RewardExpPercent);
						ExpProgressText->SetText(FText::AsPercent(RewardExpPercent));
					}
				}
				*/
				if (IsValid(HorizontalBox_Exp))
				{
					if (ResultHeroIconInfo.acquiredExp == 0)
					{
						HorizontalBox_Exp->SetVisibility(ESlateVisibility::Collapsed);
					}
					else
					{
						HorizontalBox_Exp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

						if (IsValid(TextBlock_Exp))
						{
							TextBlock_Exp->SetText(FText::AsNumber(ResultHeroIconInfo.acquiredExp));
						}
					}
				}

				if (IsValid(Image_Up))
				{
					Image_Up->SetVisibility(ESlateVisibility::Collapsed);

					//if (InSlotInfo.HeroLevel < ResultHeroIconInfo.level)
					//{
					//	UpImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					//}
				}
			}
		}
	}
}

void UUC_Result_HeroNew::StartProgress(float InTime)
{
	if (!ResultHeroIconInfo.heroId.IsNone() && ResultHeroIconInfo.acquiredExp > 0)
	{
		bProgressShowStart = true;
		ShowTime = InTime;
		CurrentTime = 0.0f;
		MinusValue = 0.0f;
	}
}