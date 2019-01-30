// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectRTarget : TargetRules
{
	public ProjectRTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Game;

        ExtraModuleNames.Add("ProjectR");
        if(bBuildEditor)
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