// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "XmlString.h"
#include "XmlNode.h"


//static ConstructorHelpers::FObjectFinder<UJSONString> Test_BP(TEXT("JSONString'/Game/Data/test.test'"));
//UJSONString* testJsonString = Test_BP.Object;


void UXmlString::LoadXml()
{
	if (!XmlString.IsEmpty())
	{	
		XmlFile.LoadFile(XmlString, EConstructMethod::ConstructFromBuffer);
		UE_LOG(LogGamePlay, Log, TEXT("^^^^^^^^^ Load xml file - %s, - %d ^^^^^^^^^ "), *ImportPath, this);
	}
}

FXmlNode* UXmlString::GetRootNode()
{
	if (XmlFile.IsValid())
		return XmlFile.GetRootNode();

	LoadXml();

	return XmlFile.GetRootNode();
}

void UXmlString::PostInitProperties()
{
#if WITH_EDITORONLY_DATA
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
#endif
	Super::PostInitProperties();
}

void UXmlString::BeginDestroy()
{	
	UnloadXml();
	UE_LOG(LogGamePlay, Log, TEXT("^^^^^^^^^ Destroy xml file - %s, - %d ^^^^^^^^^ "), *ImportPath, this);
	Super::BeginDestroy();
}

void UXmlString::UnloadXml()
{
	if (XmlFile.IsValid())
		XmlFile.Clear();
}