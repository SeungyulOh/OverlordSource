// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#include "ProjectREditor.h"
//#include "Internationalization.h"
#include "AssetToolsModule.h"
#include "Factory/AssetTypeActions_JSONString.h"
#include "Factory/AssetTypeActions_XmlString.h"

#include "UpdateService/Auth/RAuth.h"

DEFINE_LOG_CATEGORY(LogProjectREditor);

class FProjectREditor : public IModuleInterface
{
private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		TSharedRef<IAssetTypeActions> GJSONAction = MakeShareable(new FAssetTypeActions_JSONString());
		TSharedRef<IAssetTypeActions> GXmlAction = MakeShareable(new FAssetTypeActions_XmlString());
		RegisterAssetTypeAction(AssetTools, GJSONAction);
		RegisterAssetTypeAction(AssetTools, GXmlAction);
	}

	virtual void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
			{
				AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
			}
		}
		CreatedAssetTypeActions.Empty();
	}

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}

};

IMPLEMENT_GAME_MODULE(FProjectREditor, ProjectREditor)

