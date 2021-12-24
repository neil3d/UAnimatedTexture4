// Copyright 2019 Neil Fang. All Rights Reserved.

using UnrealBuildTool;

public class AnimatedTextureEditor : ModuleRules
{
    public AnimatedTextureEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AnimatedTexture",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "RHI",
                "RenderCore",
            }
            );
    }
}
