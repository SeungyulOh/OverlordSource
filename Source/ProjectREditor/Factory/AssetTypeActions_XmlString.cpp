// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ProjectREditor.h"
#include "AssetTypeActions_XmlString.h"



#define LOCTEXT_NAMESPACE "AssetTypeActions"

void FAssetTypeActions_XmlString::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	auto XmlString = GetTypedWeakObjectPtrs<UXmlString>(InObjects);

	TArray<FString> ImportPaths;
	for (auto TableIter = XmlString.CreateConstIterator(); TableIter; ++TableIter)
	{
		const UXmlString* CurTable = Cast<UXmlString>((*TableIter).Get());
		if (CurTable)
		{
			CurTable->AssetImportData->ExtractFilenames(ImportPaths);
		}
	}

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ReimportXml", "Reimport"),
		LOCTEXT("ReimportXmlTooltip", "Reimport the selected Xml(s)."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.AssetActions.ReimportAsset"),
		FUIAction(
		FExecuteAction::CreateSP(this, &FAssetTypeActions_XmlString::ExecuteReimport, XmlString),
		FCanExecuteAction::CreateSP(this, &FAssetTypeActions_XmlString::CanExecuteReimport, XmlString)
		)
		);

	TArray<FString> PotentialFileExtensions;
	PotentialFileExtensions.Add(TEXT(".xml"));
	MenuBuilder.AddMenuEntry(
		LOCTEXT("DataTable_OpenSourceData", "Open Source Data"),
		LOCTEXT("DataTable_OpenSourceDataTooltip", "Opens the data table's source data file in an external editor. It will search using the following extensions: .xml"),
		FSlateIcon(),
		FUIAction(
		FExecuteAction::CreateSP(this, &FAssetTypeActions_XmlString::ExecuteFindSourceFileInExplorer, ImportPaths, PotentialFileExtensions),
		FCanExecuteAction::CreateSP(this, &FAssetTypeActions_XmlString::CanExecuteFindSourceFileInExplorer, ImportPaths, PotentialFileExtensions)
		)
		);
}

bool FAssetTypeActions_XmlString::CanExecuteReimport(const TArray<TWeakObjectPtr<UXmlString>> Objects) const
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

void FAssetTypeActions_XmlString::ExecuteReimport(const TArray<TWeakObjectPtr<UXmlString>> Objects) const
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

void FAssetTypeActions_XmlString::GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const
{
	for (auto& Asset : TypeAssets)
	{
		const auto DataTable = CastChecked<UXmlString>(Asset);
		if (DataTable->AssetImportData)
		{
			DataTable->AssetImportData->ExtractFilenames(OutSourceFilePaths);
		}
	}
}

void FAssetTypeActions_XmlString::ExecuteFindSourceFileInExplorer(TArray<FString> Filenames, TArray<FString> OverrideExtensions)
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

bool FAssetTypeActions_XmlString::CanExecuteFindSourceFileInExplorer(TArray<FString> Filenames, TArray<FString> OverrideExtensions) const
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

bool FAssetTypeActions_XmlString::VerifyFileExists(const FString& InFileName) const
{
	return (!InFileName.IsEmpty() && IFileManager::Get().FileSize(*InFileName) != INDEX_NONE);
}



#undef LOCTEXT_NAMESPACE