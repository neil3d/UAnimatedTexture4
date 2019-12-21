/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub Repo: https://github.com/neil3d/UnrealAnimatedTexturePlugin
 * GitHub Page: http://neil3d.github.io
 *
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TextureResource.h"	// Engine
#include "AnimatedTextureSource.generated.h"

struct FAnmatedTextureState {
	int CurrentFrame;
	float FrameTime;

	FAnmatedTextureState() :CurrentFrame(0), FrameTime(0) {}
};

/**
 * Abstract Animated Texture Source, including runtime decoder/decompresser
 */
UCLASS()
class ANIMATEDTEXTURE_API UAnimatedTextureSource : public UObject
{
	GENERATED_BODY()

public:
	virtual uint32 GetGlobalWidth() const { return 0; }
	virtual uint32 GetGlobalHeight() const { return 0; }
	virtual float GetFrameDelay(int FrameIndex) const { return 0.1f; }
	virtual int GetFrameCount() const { return 1; }
	virtual float GetTotalDuration() const { return 0.0f; }


	virtual bool TickAnim(float DeltaTime, FAnmatedTextureState& AnimState, float DefaultFrameDelay, bool bLooping);
	virtual void DecodeFrameToRHI(FTextureResource* RHIResource, FAnmatedTextureState& AnimState, bool SupportsTransparency) {}

};
