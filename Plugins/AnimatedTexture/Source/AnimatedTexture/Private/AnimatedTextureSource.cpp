// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedTextureSource.h"

bool UAnimatedTextureSource::TickAnim(float DeltaTime, FAnmatedTextureState & State)
{
	bool NextFrame = false;
	float FrameDelay = GetFrameDelay(State.CurrentFrame);
	State.FrameTime += DeltaTime;
	
	if (State.FrameTime > FrameDelay) {
		State.CurrentFrame++;
		State.FrameTime -= FrameDelay;
		NextFrame = true;

		// loop
		int NumFrame = GetFrameCount();
		if (State.CurrentFrame >= NumFrame)
			State.CurrentFrame = 0;
	}

	return NextFrame;
}
