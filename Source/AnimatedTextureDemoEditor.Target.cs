// Copyright 2019 Neil Fang. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AnimatedTextureDemoEditorTarget : TargetRules
{
	public AnimatedTextureDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "AnimatedTextureDemo" } );
	}
}
