// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectREditor : ModuleRules
{
    public ProjectREditor(ReadOnlyTargetRules Target) : base(Target)
    {
		PublicIncludePaths.AddRange(
			new string[] {
                "ProjectR",
                "ProjectREditor"
				// ... add public include paths required here ...
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				"ProjectREditor/Factory",
                //"Developer/AssetTools/Private"
					// ... add other private include paths required here ...
			}
		);

        PublicDependencyModuleNames.AddRange(new string[] 
		{ 
            "ProjectR"
		});

        PrivateDependencyModuleNames.AddRange(new string[] 
		{
            "Core",
            "CoreUObject",
            "Slate",
            "Engine",
            "UnrealEd",
            "EditorStyle",
            "SlateCore",
            "TextureAtlas",
            //"InputCore",            
            //"AssetTools",
            //"KismetWidgets",
            //"GraphEditor",
            //"Kismet",
			//"PropertyEditor",
            //"RenderCore",
            //"LevelEditor",
			//"Paper2D",
            //"ContentBrowser",
            //"WorkspaceMenuStructure",            
            //"JsonUtilities",
            //"ClassViewer",
            "AssetRegistry",
            //"ActorPickerMode",
            //"XmlParser",
            "HTTP",
            "Json",
            "Sockets"
        });

        PrivateIncludePathModuleNames.AddRange(
         new string[] {
             "AssetTools"
			}
		);
    }
}
