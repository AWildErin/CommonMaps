// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonMaps : ModuleRules
{
	public CommonMaps(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { });
		PrivateIncludePaths.AddRange(new string[] { });
		DynamicallyLoadedModuleNames.AddRange( new string[] { } );

		PublicDependencyModuleNames.AddRange( new string[] { "Core", } );

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings"
			}
		);
	}
}
