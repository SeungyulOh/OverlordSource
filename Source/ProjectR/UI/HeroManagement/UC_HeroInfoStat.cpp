// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroInfoStat.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Modes/TestGameMode.h"


void UUC_HeroInfoStat::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_HeroInfoStat::NativeDestruct()
{
	Super::NativeDestruct();
}


void UUC_HeroInfoStat::Refresh(UCharacterBattleInfo* CharacterBattleInfo)
{
	FText LabelString = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UDescriptionFunctionLibrary::HeroPropertyToUIStringKey(CharacterPropertyType));
	if(IsValid(Text_StatName))
		Text_StatName->SetText(LabelString);

	// value
	FText ValueText = UDescriptionFunctionLibrary::HeroPropertyValueToText(CharacterPropertyType, CharacterBattleInfo);
	if(IsValid(Text_Value))
		Text_Value->SetText(ValueText);
	// contribution from item
	if (IsValid(Text_DetailValue))
	{
		FText detailText = FText();

		if (UDescriptionFunctionLibrary::IsHeroPropertyContributedByItem(CharacterPropertyType, CharacterBattleInfo))
		{
			FText ItemContribText = UDescriptionFunctionLibrary::HeroItemPropertyToText(CharacterPropertyType, CharacterBattleInfo);
			const FLinearColor TextColor(0.47932f, 0.89627f, 0.138432f, 1.f);
			Text_DetailValue->SetColorAndOpacity(TextColor);
			detailText = FText::FromString(FString::Printf(TEXT("(%s)"), *ItemContribText.ToString()));
			//Text_DetailValue->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *ItemContribText.ToString())));
		}
		else
		{
			const FLinearColor TextColor(0.760784f, 0.819608f, 0.831373f, 1.f);
			Text_DetailValue->SetColorAndOpacity(TextColor);
			detailText = FText::FromString(FString::Printf(TEXT("(%s)"), *FString::FromInt(0)));
			//Text_DetailValue->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *FString::FromInt(0))));
		}

		ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(pGameMode))// 테스트맵일때만
		{
			// 추가능력치가 있을때
			if (UDescriptionFunctionLibrary::IsHeroPorpetryAdditional(CharacterPropertyType, CharacterBattleInfo))
			{
				FText AdditionalText = UDescriptionFunctionLibrary::HeroAdditionalPropertyToText(CharacterPropertyType, CharacterBattleInfo);
				detailText = FText::Format(FText::FromString(TEXT("{0} {1}")), detailText, AdditionalText);
			}
		}
		

		Text_DetailValue->SetText(detailText);
	}
}
