// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectREditorTarget : TargetRules
{
	public ProjectREditorTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Editor;
        ExtraModuleNames.Add("ProjectR");
        ExtraModuleNames.Add("ProjectREditor");
		
    }

    // public override void SetupGlobalEnvironment(
	// 	TargetInfo Target,
	// 	ref LinkEnvironmentConfiguration OutLinkEnvironmentConfiguration,
	// 	ref CPPEnvironmentConfiguration OutCPPEnvironmentConfiguration
	// 	)
	// {
	// 	// Enable the use of FAES
	// 	OutCPPEnvironmentConfiguration.Definitions.Add("AES_KEY=1");
	// }
}
