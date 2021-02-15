// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTexture2D.h"
#include "AnimatedTextureResource.h"
#include "AnimatedGIF.h"

float UAnimatedTexture2D::GetSurfaceWidth() const
{
	if (GIFDecoder) return GIFDecoder->getWidth();

	return 1.0f;
}

float UAnimatedTexture2D::GetSurfaceHeight() const
{
	if (GIFDecoder) return GIFDecoder->getHeight();
	return 1.0f;
}

FTextureResource* UAnimatedTexture2D::CreateResource()
{
	if (FileBlob.Num() > 0)
	{
		GIFDecoder = MakeShared<FAnimatedGIF, ESPMode::ThreadSafe>();
		GIFDecoder->loadFromMemory(FileBlob.GetData(), FileBlob.Num());
		AnimationLength = GIFDecoder->getDuration(DefaultFrameDelay * 1000) / 1000.0f;
	}

	FTextureResource* NewResource = new FAnimatedTextureResource(this);
	return NewResource;
}

void UAnimatedTexture2D::Tick(float DeltaTime)
{
	if (!bPlaying)
		return;
	if (!GIFDecoder)
		return;

	FrameTime += DeltaTime;
	if (FrameTime < FrameDelay)
		return;

	FrameDelay = RenderFrameToTexture();
	FrameTime = 0;

}


#if WITH_EDITOR
void UAnimatedTexture2D::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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
		FrameDelay = RenderFrameToTexture();
		FrameTime = 0;
	}

	if (RequiresNotifyMaterials)
		NotifyMaterials();
}
#endif // WITH_EDITOR

float UAnimatedTexture2D::RenderFrameToTexture()
{
	// decode a new frame to memory buffer
	int nFrameDelay = GIFDecoder->playFrame(DefaultFrameDelay * 1000);

	// copy frame to RHI texture


	return nFrameDelay / 1000.0f;
}

float UAnimatedTexture2D::GetAnimationLength() const
{
	return AnimationLength;
}

void UAnimatedTexture2D::UpdateFirstFrame()
{
	if (GIFDecoder && Resource)
	{
		FrameDelay = RenderFrameToTexture();
		FrameTime = 0;
	}
}

void UAnimatedTexture2D::ImportFile(const uint8* Buffer, uint32 BufferSize) {
	this->FileBlob = TArray<uint8>(Buffer, BufferSize);
}

void UAnimatedTexture2D::Play()
{
	bPlaying = true;
}

void UAnimatedTexture2D::PlayFromStart()
{
	FrameTime = 0;
	FrameDelay = 0;
	bPlaying = true;
	if (GIFDecoder) GIFDecoder->reset();
}

void UAnimatedTexture2D::Stop()
{
	bPlaying = false;
}
