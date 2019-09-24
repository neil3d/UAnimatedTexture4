
#include "AnimatedTexture2D.h"
#include "AnimatedTextureSource.h"
#include "AnimatedTextureResource.h"

float UAnimatedTexture2D::GetSurfaceWidth() const
{
	if (AnimSource)
		return AnimSource->GetGlobalWidth();
	return 2.0f;
}

float UAnimatedTexture2D::GetSurfaceHeight() const
{
	if (AnimSource)
		return AnimSource->GetGlobalHeight();
	return 2.0f;
}

FTextureResource* UAnimatedTexture2D::CreateResource()
{
	FTextureResource* NewResource = new FAnimatedTextureResource(this);
	return NewResource;
}

void UAnimatedTexture2D::Tick(float DeltaTime)
{
	if (AnimSource && Resource) {
		if (AnimSource->TickAnim(DeltaTime, AnimState, DefaultFrameDelay))
			AnimSource->DecodeFrameToRHI(Resource, AnimState);
	}
}

void UAnimatedTexture2D::SetAnimSource(UAnimatedTextureSource* InAnimSource) {
	AnimSource = InAnimSource;
	AnimState = FAnmatedTextureState();

	if (Resource)
		AnimSource->DecodeFrameToRHI(Resource, AnimState);
	UpdateResource();
}
