// Copyright 2019 Neil Fang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "AnimatedTextureThumbnailRenderer.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATEDTEXTUREEDITOR_API UAnimatedTextureThumbnailRenderer : public UThumbnailRenderer
{
	GENERATED_BODY()
	
	// Begin UThumbnailRenderer Object
	virtual void GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const override;
#if ENGINE_MAJOR_VERSION <= 4 && ENGINE_MINOR_VERSION > 24
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily) override;
#else
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas) override;
#endif
	// End UThumbnailRenderer Object
};
