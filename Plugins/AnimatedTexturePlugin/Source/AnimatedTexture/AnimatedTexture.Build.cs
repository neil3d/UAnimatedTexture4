// Copyright 2019 Neil Fang. All Rights Reserved.

using UnrealBuildTool;

public class AnimatedTexture : ModuleRules
{
    public AnimatedTexture(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "RHI",
                "RenderCore",
            }
            );
    }
}
