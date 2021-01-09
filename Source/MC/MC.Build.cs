// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MC : ModuleRules
{
	public MC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ProceduralMeshComponent" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "FastNoiseGenerator", "FastNoise" });
		PrivateIncludePathModuleNames.AddRange(new string[] { "FastNoiseGenerator" });
	}
}
