// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SecondGame : ModuleRules
{
	public SecondGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
			
			// Enhanced Input
			"EnhancedInput",
		});
	}
}
