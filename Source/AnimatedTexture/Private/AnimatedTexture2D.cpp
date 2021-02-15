// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTexture2D.h"
#include "AnimatedTextureSource.h"
#include "AnimatedTextureResource.h"

float UAnimatedTexture2D::GetSurfaceWidth() const
{
	return 1.0f;
}

float UAnimatedTexture2D::GetSurfaceHeight() const
{
	return 1.0f;
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
		//AnimSource->DecodeFrameToRHI(Resource, AnimState, SupportsTransparency);
	}

	if (RequiresNotifyMaterials)
		NotifyMaterials();
}
#endif // WITH_EDITOR

float UAnimatedTexture2D::GetAnimationLength() const
{

	return 0.0f;
}

void UAnimatedTexture2D::UpdateFirstFrame()
{
	/*if (AnimSource && Resource) 
	{
		AnimState = FAnmatedTextureState();
		AnimSource->DecodeFrameToRHI(Resource, AnimState, SupportsTransparency);
	}*/
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
