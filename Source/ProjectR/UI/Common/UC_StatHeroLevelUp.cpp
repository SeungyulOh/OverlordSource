// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_StatHeroLevelUp.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/RInventory.h"


void UUC_StatHeroLevelUp::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_StatHeroLevelUp::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_StatHeroLevelUp::SetStat(const FString& CurrentValue, const FString& NextValue, const FString& StatName, const FString& AddValue, bool bMaxLevel)
{
	if (!bMaxLevel)
	{
		Text_AddValue->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TextBlock_NextValule->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		if (IsValid(TextBlock_NextValule))
			TextBlock_NextValule->SetText(FText::FromString(NextValue));

		if (IsValid(Text_AddValue))
			Text_AddValue->SetText(FText::Format(FText::FromString("+{0}"), FText::FromString(AddValue)));
	}
	else
	{

		TextBlock_NextValule->SetVisibility(ESlateVisibility::Collapsed);
		Text_AddValue->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(TextBlock_CurrentValule))
		TextBlock_CurrentValule->SetText(FText::FromString(CurrentValue));
	if (IsValid(TextBlock_StatName))
	{
		FText txt = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(*StatName));
		TextBlock_StatName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(*StatName)));
	}
	
	
}