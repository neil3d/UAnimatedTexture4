// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTextureSource.h"

bool UAnimatedTextureSource::TickAnim(float DeltaTime, FAnmatedTextureState & AnimState, float DefaultFrameDelay, bool bLooping)
{
	bool NextFrame = false;
	float FrameDelay = GetFrameDelay(AnimState.CurrentFrame);
	if (FrameDelay == 0.0f)
		FrameDelay = DefaultFrameDelay;
	AnimState.FrameTime += DeltaTime;

	// skip long duration
	float Duration = GetTotalDuration();
	if (AnimState.FrameTime > Duration)
	{
		float N = FMath::TruncToFloat(AnimState.FrameTime / Duration);
		AnimState.FrameTime -= N * Duration;
	}

	// step to next frame
	if (AnimState.FrameTime > FrameDelay) {
		AnimState.CurrentFrame++;
		AnimState.FrameTime -= FrameDelay;
		NextFrame = true;

		// loop
		int NumFrame = GetFrameCount();
		if (AnimState.CurrentFrame >= NumFrame)
			AnimState.CurrentFrame = bLooping ? 0 : NumFrame - 1;
	}

	return NextFrame;
}
