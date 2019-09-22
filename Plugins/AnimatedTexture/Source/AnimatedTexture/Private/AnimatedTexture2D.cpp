
#include "AnimatedTexture2D.h"
#include "AnimatedTextureSource.h"
#include "AnimatedTextureResource.h"

float UAnimatedTexture2D::GetSurfaceWidth() const
{
	if (AnimSource)
		return AnimSource->GetImageWidth();
	return 2.0f;
}

float UAnimatedTexture2D::GetSurfaceHeight() const
{
	if (AnimSource)
		return AnimSource->GetImageHeight();
	return 2.0f;
}

FTextureResource* UAnimatedTexture2D::CreateResource()
{
	return new FAnimatedTextureResource(this);
}

void UAnimatedTexture2D::SetAnimSource(UAnimatedTextureSource* AnimSource) {
	AnimSource = AnimSource;

	UpdateResource();
}
