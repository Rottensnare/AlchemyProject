// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlchemyProject : ModuleRules
{
	public AlchemyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "SlateCore", "Slate", "AIModule", "GameplayTasks", "NavigationSystem", "GameplayTags", "Json", "JsonUtilities" });
		PrivateDependencyModuleNames.AddRange(new string[]{"GameplayAbilities"});
	}
}
