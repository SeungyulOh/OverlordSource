// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UC_Colosseum_Class.h"


void UUC_Colosseum_Class::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Colosseum_Class::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Colosseum_Class::SetColosseumClassIndex(int32 SelectIndex)
{
	FColosseumGrade* ColosseumGrade = UTableManager::GetInstancePtr()->GetColosseumGrade(FName(*(FString::FromInt(SelectIndex))));

	if (ColosseumGrade == nullptr)
		return;

	int32 ActiveIndex = (SelectIndex - 1) / 4;

	if (IsValid(Switcher_Tier))
		Switcher_Tier->SetActiveWidgetIndex(ActiveIndex);

	if (ColosseumGrade->Grade.EqualTo(FText::FromString("NONE")))
	{
		if (IsValid(Text_Tier))
			Text_Tier->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (IsValid(Text_Tier))
			Text_Tier->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(Text_Tier))
	{
		if(Text_Tier->IsVisible())
			Text_Tier->SetText(ColosseumGrade->Grade);
	}

}