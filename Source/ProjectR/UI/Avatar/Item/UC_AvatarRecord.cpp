// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_AvatarRecord.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"

void UUC_AvatarRecord::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_AvatarRecord::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_AvatarRecord::SetValue(int32 InTotalCount, int32 InWinCount)
{
	TotalCount = InTotalCount;
	WinCount = InWinCount;

	if (TextKey.IsNone())
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (Text_Title)
	{
		auto Text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TextKey);
		Text_Title->SetText(Text);
	}

	if (Text_Total)
	{
		auto Text = FText::AsNumber(TotalCount);
		Text_Total->SetText(Text);
	}
	if (Text_Win)
	{
		auto Text = FText::AsNumber(WinCount);
		Text_Win->SetText(Text);
	}
}
