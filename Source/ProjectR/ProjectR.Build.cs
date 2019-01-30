// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System;
using System.IO;
using Tools.DotNETCommon;


public class ProjectR : ModuleRules
{

    private string ThirdPartyPath
    {
        get
        {
            return System.IO.Path.GetFullPath(
                System.IO.Path.Combine(ModuleDirectory, "../../ThirdParty")
            );
        }
    }

    public ProjectR(ReadOnlyTargetRules Target) : base(Target)
    {
        //UEBuildConfiguration.bUseLoggingInShipping = true;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "AIModule",
                "UMG",
                "Slate",
                "SlateCore",
                "HTTP",
                "Json",
                "XmlParser",
                "JsonUtilities",
                
                "zlib",
                "MoviePlayer",
                "LevelSequence",
                "Media",
                "MediaAssets",
                "MovieScene",
				"MovieSceneTracks",
				"BuildPatchServices",
                "Paper2D",
				"PakFile",
                "Sockets",
                "Networking",
                "RHI",
				"ApplicationCore",
				"PublisherAPI",
                "PacketHandler",
                "OnlineSubsystem",
                "OnlineSubsystemUtils",
                "WebBrowser"
            }
        );
        PrivateDependencyModuleNames.AddRange(new string[] {
            

            }
        );
        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "Launch"
            }
        );
		LoadFlatBuffers(Target);
        ApplyBuildVersion();
        PublicDefinitions.Add("USE_CAFESDK_PLUGIN=0");
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            //string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            //AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "ProjectR_APL.xml")));

            DynamicallyLoadedModuleNames.Add("OnlineSubsystemGoogle");
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemGooglePlay");
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemFacebook");
            //            RuntimeDependencies.Add(Path.Combine(PluginPath, "ProjectR_APL.xml"));
        }
        else if(Target.Platform == UnrealTargetPlatform.IOS)
        {
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemIOS");
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemFacebook");
        }
    }

    private void ApplyBuildVersion()
    {
        // Read from ProjectR/Build/Build.version if exists

        string BuildVersionPath = Path.Combine(ModuleDirectory, "..", "..", "Build", "Build.version");

        string BuildVersion;
        if (File.Exists(BuildVersionPath))
        {
            PublicDefinitions.Add("PROJECTR_NOPATCH_BUILD=0");

			FileReference BuildVersionFile = new FileReference(BuildVersionPath);
			string[] Lines = FileReference.ReadAllLines(BuildVersionFile);

			if (Lines.Length >= 1)
			{
				BuildVersion = Lines[0];
			}
			else
			{
				BuildVersion = "FailedToReadBuildVersion";
			}
		}
        else
        {
            PublicDefinitions.Add("PROJECTR_NOPATCH_BUILD=1");
            BuildVersion = "c-live-1";
            //BuildVersion = "c-dev-1";
            //if (Target.Platform == UnrealTargetPlatform.Android)
            //{
            //BuildVersion = "c-live-1";
            //}
            //else
            //{
            //BuildVersion = "c-dev-1";
            //}
        }

        PublicDefinitions.Add("PROJECTR_BUILD_VERSION=" + BuildVersion);
		System.Console.WriteLine("PROJECTR_BUILD_VERSION=" + BuildVersion);
	}

	public void LoadFlatBuffers(ReadOnlyTargetRules Target) {
		var FlatBuffersInstallPath = Path.Combine(ThirdPartyPath, "FlatBuffers");
		PublicIncludePaths.Add(Path.Combine(FlatBuffersInstallPath, "include"));
	}
}