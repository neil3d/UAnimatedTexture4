// Copyright 2019 Neil Fang. All Rights Reserved.

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
	if (!bPlaying)
		return;

	if (AnimSource && Resource) {
		if (AnimSource->TickAnim(DeltaTime*PlayRate, AnimState, DefaultFrameDelay, bLooping))
			AnimSource->DecodeFrameToRHI(Resource, AnimState, SupportsTransparency);
	}
}


#if WITH_EDITOR
void UAnimatedTexture2D::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	bool RequiresNotifyMaterials = false;
	bool ResetAnimState = false;

	UProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	if (PropertyThatChanged)
	{
		const FName PropertyName = PropertyThatChanged->GetFName();

		static const FName SupportsTransparencyName = GET_MEMBER_NAME_CHECKED(UAnimatedTexture2D, SupportsTransparency);

		if (PropertyName == SupportsTransparencyName)
		{
			RequiresNotifyMaterials = true;
			ResetAnimState = true;
		}
	}// end of if(prop is valid)

	if (ResetAnimState)
	{
		AnimState = FAnmatedTextureState();
		AnimSource->DecodeFrameToRHI(Resource, AnimState, SupportsTransparency);
	}

	if (RequiresNotifyMaterials)
		NotifyMaterials();
}
#endif // WITH_EDITOR

float UAnimatedTexture2D::GetAnimationLength() const
{
	if (AnimSource)
		return AnimSource->GetTotalDuration();

	return 0.0f;
}

void UAnimatedTexture2D::SetAnimSource(UAnimatedTextureSource* InAnimSource) {
	AnimSource = InAnimSource;
	AnimState = FAnmatedTextureState();

	if (Resource)
		AnimSource->DecodeFrameToRHI(Resource, AnimState, SupportsTransparency);
	UpdateResource();
}

void UAnimatedTexture2D::UpdateFirstFrame()
{
	if (AnimSource && Resource) 
	{
		AnimState = FAnmatedTextureState();
		AnimSource->DecodeFrameToRHI(Resource, AnimState, SupportsTransparency);
	}
}

void UAnimatedTexture2D::Play()
{
	bPlaying = true;
}

void UAnimatedTexture2D::PlayFromStart()
{
	AnimState = FAnmatedTextureState();
	bPlaying = true;
}

void UAnimatedTexture2D::Stop()
{
	bPlaying = false;
}
