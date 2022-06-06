// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_Minesweeper : ModuleRules
{
	public UE_Minesweeper(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "WebSockets" });
	}
}
