// Copyright(C) 2018 KOKOMO Games. All rights reserved.

#include "ProjectR.h"
#include "UC_PartyEffect.h"
#include "Global/TableManager.h"
#include "Table/CharacterTableInfo.h"
#include "Global/RGameInstance.h"
#include "UtilFunctionLibrary.h"




void UUC_PartyEffect::Update(TArray<FString> heroUDs)
{
	static auto FindColor = [](ECounterType type) -> FSlateColor {
		switch (type)
		{
		case ECounterType::SOLAR: return FSlateColor(FLinearColor(FColor(195, 42, 42)));
		case ECounterType::LIGHT: return FSlateColor(FLinearColor(FColor(195, 108, 6)));
		case ECounterType::DARK: return FSlateColor(FLinearColor(FColor(132, 132, 183)));
		case ECounterType::MOON: return FSlateColor(FLinearColor(FColor(81, 132, 183)));

		default:
			return FSlateColor(FLinearColor::White);
		}
	};

	for (size_t i = 0; i < heroUDs.Num(); ++i)
	{
		FString heroUD = heroUDs[i];
		if (heroUD.IsEmpty())
		{
			TextEffectArray[i]->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}
		else
			TextEffectArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FHERO* hero = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(heroUD);
		FCharacterTableInfo* heroinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
		if (hero)
		{
			if (heroinfo && IsValid(TextEffectArray[i]))
			{
				TextEffectArray[i]->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, heroinfo->AttributeSkillString));
				TextEffectArray[i]->SetColorAndOpacity(FindColor(heroinfo->CounterType));
			}
		}


		/*auto StringHelper = [](TArray<UTextBlock*>& TargetTextArray, const TMap<ECounterType, FName>& InMap) {

			TArray<ECounterType> KeyArray;
			InMap.GenerateKeyArray(KeyArray);
			for (size_t i = 0; i <TargetTextArray.Num(); ++i)
			{
				if (TargetTextArray.IsValidIndex(i))
				{
					if (i < InMap.Num())
					{
						FString OutString = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, InMap[KeyArray[i]]).ToString();
						TargetTextArray[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						TargetTextArray[i]->SetText(FText::FromString(OutString));
						TargetTextArray[i]->SetColorAndOpacity(FindColor(KeyArray[i]));
					}
					else
						TargetTextArray[i]->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		};*/

		/*StringHelper(variables->Text_PartyEffectFirstArray, FirstCounters);
		StringHelper(variables->Text_PartyEffectSecondArray, SecondCounters);*/
	}
}
