// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ExeDungeon : ModuleRules
{
	public ExeDungeon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject",
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"UMG",
            "Niagara",
            "AIModule", 
			"NavigationSystem", 
			"GameplayTasks" });
    
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateIncludePaths.AddRange(
			new string[]
			{
				"ExeDungeon",
			}
		);
	}
}
