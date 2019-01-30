// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ProjectREditor.h"
#include "EditorConsoleUtils.h"

#include "ModuleManager.h"
#include "AssetRegistryModule.h"
#include "IAssetRegistry.h"
#include "IConsoleManager.h"


/*
Find Blueprint classes derived from given base class using the Asset Registry as described in the article
http://kantandev.com/articles/finding-all-classes-blueprints-with-a-given-base
*/
void FindBlueprintClasses(const TArray<FString>& InParams, UWorld* InWorld)
{
	if (InParams.Num() < 1)
		return;

	const FString& BaseClassName = InParams[0];

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName(TEXT("AssetRegistry")));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FString> ContentPaths;
	ContentPaths.Add(TEXT("/Game"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	TSet<FName> DerivedNames;
	TArray<FName> BaseNames;
	BaseNames.Add(FName(*BaseClassName));
	TSet<FName> Excluded;
	AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);

	// Print the derived classes
	for (auto Iter = DerivedNames.CreateConstIterator(); Iter; ++Iter) {
		const FName& DerivedName = *Iter;
		UE_LOG(LogProjectREditor, Log, TEXT("%s"), *DerivedName.ToString());
	}
}

UEditorConsoleUtils::UEditorConsoleUtils()
{
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("FindBlueprintClasses"),
		TEXT("UEditorConsoleUtils::FindBlueprintClasses\n"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&FindBlueprintClasses),
		ECVF_Default);
}