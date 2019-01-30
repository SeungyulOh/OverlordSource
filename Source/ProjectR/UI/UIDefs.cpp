// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UIDefs.h"

#include "UtilFunctionIntegrated.h"

FText FOptionalUIStringKey::GetText()
{
	if (!UIStringKey.IsNone())
	{
		FText Text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UIStringKey);
		if (!Text.IsEmpty())
		{
			return Text;
		}
		else
		{
			UE_LOG(LogUI, Warning, TEXT("FOptionalUIStringKey::Render: failed to look up UIStringKey %s"), *UIStringKey.ToString());
		}
	}
	
	return FallbackText;
}
