// Fill out your copyright notice in the Description page of Project Settings.
#include "ProjectREditor.h"
#include "JSONReimportFactory.h"

#include "JSONString.h"

#define LOCTEXT_NAMESPACE "JSONReimportFactory"

UJSONReimportFactory::UJSONReimportFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Formats.Add(TEXT("json;JavaScript Object Notation"));
}

bool UJSONReimportFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UJSONString* DataTable = Cast<UJSONString>(Obj);
	if (DataTable && DataTable->AssetImportData)
	{
		DataTable->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}

	return false;
}

void UJSONReimportFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UJSONString* DataTable = Cast<UJSONString>(Obj);
	if (DataTable && ensure(NewReimportPaths.Num() == 1))
	{
		DataTable->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

EReimportResult::Type UJSONReimportFactory::Reimport(UObject* Obj)
{
	UJSONString* DataTable = Cast<UJSONString>(Obj);
	if (!DataTable)
	{
		return EReimportResult::Failed;
	}

	// Make sure file is valid and exists
	const FString Filename = DataTable->AssetImportData->GetFirstFilename();
	if (!Filename.Len() || IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
	{
		return EReimportResult::Failed;
	}

	if (Filename.IsEmpty() == false)
	{
		FString FilePath = IFileManager::Get().ConvertToRelativePath(*Filename);

		FString Data;
		if (FFileHelper::LoadFileToString(Data, *FilePath))
		{
			const TCHAR* Ptr = *Data;
			CurrentFilename = FilePath; //not thread safe but seems to be how it is done..
			auto Result = FactoryCreateText(Obj->GetClass(), Obj->GetOuter(), Obj->GetFName(), Obj->GetFlags(), NULL, *FPaths::GetExtension(FilePath), Ptr, Ptr + Data.Len(), NULL);
			return EReimportResult::Succeeded;
		}
	}

	return EReimportResult::Failed;;
}

int32 UJSONReimportFactory::GetPriority() const
{
	return ImportPriority;
}

#undef LOCTEXT_NAMESPACE
