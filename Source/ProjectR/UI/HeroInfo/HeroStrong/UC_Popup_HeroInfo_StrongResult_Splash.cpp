// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UC_Popup_HeroInfo_StrongResult_Splash.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Global/TableManager.h"
#include "UIFunctionLibrary.h"
#include "UP_GenericScrollview.h"

void UUC_Popup_HeroInfo_StrongResult_Splash::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Popup_HeroInfo_StrongResult_Splash::NativeDestruct()
{
	Super::NativeDestruct();
}


void UUC_Popup_HeroInfo_StrongResult_Splash::SetEnchantResult(TArray<bool>& InResult, TArray<FName>& InHeroIDs, TArray<int32>& InPercent, int32 InGetPoint)
{
	UTableManager* TM = UTableManager::GetInstancePtr();

	checkf(TM != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	if(variables.Text_Result)
	{
		FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_HeroEnchant_ResultMessage")));
		variables.Text_Result->SetText(FText::Format(text, FText::AsNumber(InGetPoint)));
	}

	for (int32 i = 0; i < variables.WidgetSwitcher_Icon.Num(); i++)
	{
		if (IsValid(variables.WidgetSwitcher_Icon[i]) == false || variables.WidgetSwitcher_Icon.IsValidIndex(i) == false)
			continue;

		if (InResult.IsValidIndex(i))
		{
			variables.WidgetSwitcher_Icon[i]->SetVisibility(ESlateVisibility::Visible);
			if (variables.SuccessIcon.IsValidIndex(i) && InHeroIDs.IsValidIndex(i))
				variables.SuccessIcon[i]->SetHeroIconInfoWithID(InHeroIDs[i], EHeroOwnershipType::VE_None);

			if (variables.FailIcon.IsValidIndex(i) && InHeroIDs.IsValidIndex(i))
				variables.FailIcon[i]->SetHeroIconInfoWithID(InHeroIDs[i], EHeroOwnershipType::VE_None);

			if (InResult[i])
				variables.WidgetSwitcher_Icon[i]->SetActiveWidgetIndex(0);
			else
				variables.WidgetSwitcher_Icon[i]->SetActiveWidgetIndex(1);

			if (variables.Text_EnchantPer.IsValidIndex(i) && IsValid(variables.Text_EnchantPer[i]))
			{
				variables.Text_EnchantPer[i]->SetVisibility(ESlateVisibility::Visible);
				variables.Text_EnchantPer[i]->SetText(FText::Format(FText::FromString(TEXT("{0}.0%")), InPercent[i]));
			}

			FCharacterCostEnchant* CCE = TM->GetCharacterCostEnchant(TEXT("1"));
			FCharacterTableInfo* CTI = TM->GetCharacterRow(InHeroIDs[i]);

			int32 UpgradePer = 0;

			switch (CTI->Grade)
			{
			case 1:
				UpgradePer = CCE->SpiritGrade1 / 10;
				break;
			case 2:
				UpgradePer = CCE->SpiritGrade2 / 10;
				break;
			case 3:
				UpgradePer = CCE->SpiritGrade3 / 10;
				break;
			case 4:
				UpgradePer = CCE->SpiritGrade4 / 10;
				break;
			case 5:
				UpgradePer = CCE->SpiritGrade5 / 10;
				break;
			case 6:
				UpgradePer = CCE->SpiritGrade6 / 10;
				break;
			default:
				break;
			}
			if (variables.Text_EnchantPer.IsValidIndex(i))
			{
				variables.Text_EnchantPer[i]->SetVisibility(ESlateVisibility::Visible);
				variables.Text_EnchantPer[i]->SetText(FText::FromString(FString::FromInt(UpgradePer) + TEXT("%")));
			}
		}
		else
		{
			variables.WidgetSwitcher_Icon[i]->SetVisibility(ESlateVisibility::Collapsed);
			variables.Text_EnchantPer[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UUC_Popup_HeroInfo_StrongResult_Splash::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	SetVisibility(ESlateVisibility::Collapsed);

	UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_HERO_ENCHANT);
	UUIFunctionLibrary::GetGenericScrollview()->Appear(true);

	return FReply::Unhandled();
}
