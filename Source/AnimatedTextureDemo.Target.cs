// Copyright 2019 Neil Fang. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AnimatedTextureDemoTarget : TargetRules
{
	public AnimatedTextureDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "AnimatedTextureDemo" } );
	}
}
