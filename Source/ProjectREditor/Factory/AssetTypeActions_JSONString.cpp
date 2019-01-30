// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ProjectREditor.h"
#include "AssetTypeActions_JSONString.h"

//#include "Editor/DataTableEditor/Public/DataTableEditorModule.h"
//#include "Editor/DataTableEditor/Public/IDataTableEditor.h"
//#include "Engine/DataTable.h"
//#include "DesktopPlatformModule.h"
//#include "IMainFrameModule.h"



#define LOCTEXT_NAMESPACE "AssetTypeActions"

void FAssetTypeActions_JSONString::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	auto JsonString = GetTypedWeakObjectPtrs<UJSONString>(InObjects);

	TArray<FString> ImportPaths;
	for (auto TableIter = JsonString.CreateConstIterator(); TableIter; ++TableIter)
	{
		const UJSONString* CurTable = Cast<UJSONString>((*TableIter).Get());
		if (CurTable)
		{
			CurTable->AssetImportData->ExtractFilenames(ImportPaths);
		}
	}

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ReimportJson", "Reimport"),
		LOCTEXT("ReimportJsonTooltip", "Reimport the selected Json(s)."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.AssetActions.ReimportAsset"),
		FUIAction(
		FExecuteAction::CreateSP(this, &FAssetTypeActions_JSONString::ExecuteReimport, JsonString),
		FCanExecuteAction::CreateSP(this, &FAssetTypeActions_JSONString::CanExecuteReimport, JsonString)
		)
		);

	TArray<FString> PotentialFileExtensions;
	PotentialFileExtensions.Add(TEXT(".json"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("DataTable_OpenSourceData", "Open Source Data"),
		LOCTEXT("DataTable_OpenSourceDataTooltip", "Opens the data table's source data file in an external editor. It will search using the following extensions: .json"),
		FSlateIcon(),
		FUIAction(
		FExecuteAction::CreateSP(this, &FAssetTypeActions_JSONString::ExecuteFindSourceFileInExplorer, ImportPaths, PotentialFileExtensions),
		FCanExecuteAction::CreateSP(this, &FAssetTypeActions_JSONString::CanExecuteFindSourceFileInExplorer, ImportPaths, PotentialFileExtensions)
		)
		);
}

bool FAssetTypeActions_JSONString::CanExecuteReimport(const TArray<TWeakObjectPtr<UJSONString>> Objects) const
{
	for (auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto Object = (*ObjIt).Get();
		if (Object)
		{
			if (!Object->ImportPath.IsEmpty())
				return true;
		}
	}

	return false;
}

void FAssetTypeActions_JSONString::ExecuteReimport(const TArray<TWeakObjectPtr<UJSONString>> Objects) const
{
	for (auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto Object = (*ObjIt).Get();
		if (Object)
		{
			if (!Object->ImportPath.IsEmpty())
				FReimportManager::Instance()->Reimport(Object, /*bAskForNewFileIfMissing=*/false);
		}
	}
}

void FAssetTypeActions_JSONString::GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{
	for (auto& Asset : TypeAssets)
	{
		const auto DataTable = CastChecked<UJSONString>(Asset);
		if (DataTable->AssetImportData)
		{
			DataTable->AssetImportData->ExtractFilenames(OutSourceFilePaths);
		}
	}
}

void FAssetTypeActions_JSONString::ExecuteFindSourceFileInExplorer(TArray<FString> Filenames, TArray<FString> OverrideExtensions)
{
	for (TArray<FString>::TConstIterator FilenameIter(Filenames); FilenameIter; ++FilenameIter)
	{
		const FString CSVFilename = FPaths::ConvertRelativePathToFull(*FilenameIter);
		const FString RootPath = FPaths::GetPath(CSVFilename);
		const FString BaseFilename = FPaths::GetBaseFilename(CSVFilename, true);

		for (TArray<FString>::TConstIterator ExtensionItr(OverrideExtensions); ExtensionItr; ++ExtensionItr)
		{
			const FString FilenameWithExtension(FString::Printf(TEXT("%s/%s%s"), *RootPath, *BaseFilename, **ExtensionItr));
			if (VerifyFileExists(FilenameWithExtension))
			{
				FPlatformProcess::LaunchFileInDefaultExternalApplication(*FilenameWithExtension, NULL, ELaunchVerb::Edit);
				break;
			}
		}
	}
}

bool FAssetTypeActions_JSONString::CanExecuteFindSourceFileInExplorer(TArray<FString> Filenames, TArray<FString> OverrideExtensions) const
{
	// Verify that extensions were provided
	if (OverrideExtensions.Num() == 0)
	{
		return false;
	}

	// Verify that the file exists with any of the given extensions
	for (TArray<FString>::TConstIterator FilenameIter(Filenames); FilenameIter; ++FilenameIter)
	{
		const FString CSVFilename = FPaths::ConvertRelativePathToFull(*FilenameIter);
		const FString RootPath = FPaths::GetPath(CSVFilename);
		const FString BaseFilename = FPaths::GetBaseFilename(CSVFilename, true);

		for (TArray<FString>::TConstIterator ExtensionItr(OverrideExtensions); ExtensionItr; ++ExtensionItr)
		{
			const FString FilenameWithExtension(FString::Printf(TEXT("%s/%s%s"), *RootPath, *BaseFilename, **ExtensionItr));
			if (VerifyFileExists(FilenameWithExtension))
			{
				return true;
			}
		}
	}

	return false;
}

bool FAssetTypeActions_JSONString::VerifyFileExists(const FString& InFileName) const
{
	return (!InFileName.IsEmpty() && IFileManager::Get().FileSize(*InFileName) != INDEX_NONE);
}

#undef LOCTEXT_NAMESPACE