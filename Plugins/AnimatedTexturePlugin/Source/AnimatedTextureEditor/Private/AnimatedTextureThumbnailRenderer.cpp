// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTextureThumbnailRenderer.h"
#include "AnimatedTexture2D.h"

#include "CanvasTypes.h"	// Engine
#include "CanvasItem.h"	// Engine
#include "Engine/Texture2D.h"	// Engine
#include "ThumbnailRendering/ThumbnailManager.h"	// UnrealEd

void UAnimatedTextureThumbnailRenderer::GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const
{
	UAnimatedTexture2D* Texture = Cast<UAnimatedTexture2D>(Object);

	if (Texture != nullptr)
	{
		OutWidth = FMath::TruncToInt(Zoom * (float)Texture->GetSurfaceWidth());
		OutHeight = FMath::TruncToInt(Zoom * (float)Texture->GetSurfaceHeight());
	}
	else
	{
		OutWidth = OutHeight = 0;
	}
}

void UAnimatedTextureThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas, bool bAdditionalViewFamily) 
{
	UAnimatedTexture2D* Texture = Cast<UAnimatedTexture2D>(Object);
	if (Texture != nullptr && Texture->Resource != nullptr)
	{
		if (Texture->SupportsTransparency)
		{
			// If support transparentcy, draw a checkerboard background first.
			const int32 CheckerDensity = 8;
			UTexture2D* Checker = UThumbnailManager::Get().CheckerboardTexture;
			Canvas->DrawTile(
				0.0f, 0.0f, Width, Height,							// Dimensions
				0.0f, 0.0f, CheckerDensity, CheckerDensity,			// UVs
				FLinearColor::White, Checker->Resource);			// Tint & Texture
		}

		// Use A canvas tile item to draw
		FCanvasTileItem CanvasTile(FVector2D(X, Y), Texture->Resource, FVector2D(Width, Height), FLinearColor::White);
		CanvasTile.BlendMode = Texture->SupportsTransparency ? SE_BLEND_Translucent : SE_BLEND_Opaque;
		CanvasTile.Draw(Canvas);

	}// end of if(texture is valid)
}
