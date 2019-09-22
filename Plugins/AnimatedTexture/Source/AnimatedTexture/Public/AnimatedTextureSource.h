/**
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

	virtual bool TickAnim(float DeltaTime, int LastFrame, float FrameTime) { return false; }
	virtual void DecodeFrameToRHI(FTextureResource* RHIResource, int Frame) {}

};
