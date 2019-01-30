// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "JSONString.h"


//static ConstructorHelpers::FObjectFinder<UJSONString> Test_BP(TEXT("JSONString'/Game/Data/test.test'"));
//UJSONString* testJsonString = Test_BP.Object;


void UJSONString::PostInitProperties()
{
#if WITH_EDITORONLY_DATA
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
#endif
	Super::PostInitProperties();
}
