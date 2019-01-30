// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Currency_Icon.h"

#include "UtilFunctionIntegrated.h"



void UUC_Currency_Icon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Currency_Icon::Refresh(FName IconKey, int32 Amount)
{
	//Image setting
	UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(IconKey), Image_Icon);
	
	Text_StackNum->SetText(FText::AsNumber(Amount));
}
