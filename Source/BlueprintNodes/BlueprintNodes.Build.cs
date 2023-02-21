// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using UnrealBuildTool;

public class BlueprintNodes : ModuleRules
{
    public BlueprintNodes(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "UnrealScape"
            });
    }
}