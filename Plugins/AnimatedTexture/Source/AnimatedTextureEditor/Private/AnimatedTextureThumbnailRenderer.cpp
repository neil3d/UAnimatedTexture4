
#include "AnimatedTextureThumbnailRenderer.h"
#include "AnimatedTexture2D.h"

#include "CanvasItem.h"	// Engine

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

void UAnimatedTextureThumbnailRenderer::Draw(UObject * Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget *, FCanvas * Canvas)
{
	UAnimatedTexture2D* Texture = Cast<UAnimatedTexture2D>(Object);
	if (Texture != nullptr && Texture->Resource != nullptr)
	{
		// Use A canvas tile item to draw
		FCanvasTileItem CanvasTile(FVector2D(X, Y), Texture->Resource, FVector2D(Width, Height), FLinearColor::White);
		CanvasTile.BlendMode = Texture->SupportsTransparency ? SE_BLEND_Translucent : SE_BLEND_Opaque;
		//CanvasTile.BatchedElementParameters = BatchedElementParameters;
		CanvasTile.Draw(Canvas);

	}// end of if(texture is valid)
}
