
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
	FTextureResource* Resource =  new FAnimatedTextureResource(this);
	if (AnimSource)
		AnimSource->DecodeFrameToRHI(Resource, 0);

	return Resource;
}

void UAnimatedTexture2D::Tick(float DeltaTime)
{
	if (AnimSource && Resource) {
		if (AnimSource->TickAnim(DeltaTime, State))
			AnimSource->DecodeFrameToRHI(Resource, State.CurrentFrame);
	}
}

void UAnimatedTexture2D::SetAnimSource(UAnimatedTextureSource* InAnimSource) {
	AnimSource = InAnimSource;

	UpdateResource();
}
