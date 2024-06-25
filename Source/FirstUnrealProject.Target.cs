// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FirstUnrealProjectTarget : TargetRules
{
	public FirstUnrealProjectTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        bOverrideBuildEnvironment = true;

        DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("FirstUnrealProject");
	}
}
