// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "XmlManager.h"



UXmlManager* UXmlManager::InstancePtr = nullptr;

UXmlManager::UXmlManager()
{	
}

UXmlString* UXmlManager::LoadXmlStringFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;

	FStringAssetReference strRef = Path.ToString();

	UObject* obj = strRef.ResolveObject();
	if (!IsValid(obj))
	{
		obj = strRef.TryLoad();
	}

	return Cast<UXmlString>(obj);
}

UXmlManager* UXmlManager::GetInstancePtr()
{
	if (InstancePtr == nullptr)
	{
		InstancePtr = NewObject< UXmlManager >((UObject*)GetTransientPackage(), UXmlManager::StaticClass());
		InstancePtr->InitData();
		InstancePtr->AddToRoot();
	}
	
	return InstancePtr;
}

void UXmlManager::ReleaseSingleton()
{
	if (InstancePtr)
	{
		InstancePtr->RemoveFromRoot();
		InstancePtr = nullptr;
	}
}

void UXmlManager::InitData()
{
}

UXmlString* UXmlManager::GetAIString(FName FileName)
{
	FString str = FPaths::GetBaseFilename(FileName.ToString());
	FString strFullPath = "JSONString'/Game/Data/Xmls/AI/" + FileName.ToString() + "." + str + "'";

	UXmlString* XmlString = LoadXmlStringFromPath(*strFullPath);

	return XmlString;
}

UXmlString* UXmlManager::GetAIStarategyString(FName FileName)
{
	FString str = FPaths::GetBaseFilename(FileName.ToString());
	FString strFullPath = "XmlString'/Game/Data/Xmls/AIStrategy/" + FileName.ToString() + "." + str + "'";

	UXmlString* XmlString = LoadXmlStringFromPath(*strFullPath);

	return XmlString;
}

UXmlString* UXmlManager::GetEtcString(FName FileName)
{
	FString str = FPaths::GetBaseFilename(FileName.ToString());
	FString strFullPath = "JSONString'/Game/Data/Xmls/Etc/" + FileName.ToString() + "." + str + "'";

	UXmlString* XmlString = LoadXmlStringFromPath(*strFullPath);

	return XmlString;
}
