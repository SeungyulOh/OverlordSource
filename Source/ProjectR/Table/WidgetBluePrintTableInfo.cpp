// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "WidgetBluePrintTableInfo.h"
#include "UtilFunctionIntegrated.h"


UClass* FWidgetBluePrintTableInfo::GetWidgetClass()
{	
	return UUtilFunctionLibrary::GetBaseWidgetFromAssetID(BlueprintGeneratedClass);
}