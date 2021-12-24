// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTexture2D.h"
#include "AnimatedTextureResource.h"
#include "AnimatedTextureModule.h"
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
		auto NewGIFDecoder = MakeShared<FAnimatedGIF, ESPMode::ThreadSafe>();
		if (NewGIFDecoder->loadFromMemory(FileBlob.GetData(), FileBlob.Num()))
		{
			AnimationLength = NewGIFDecoder->getDuration(DefaultFrameDelay * 1000) / 1000.0f;
			SupportsTransparency = NewGIFDecoder->supportsTransparency();
			GIFDecoder = NewGIFDecoder;
		}
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

	FrameTime += DeltaTime * PlayRate;
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

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
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
	int nFrameDelay = GIFDecoder->playFrame(DefaultFrameDelay * 1000, bLooping);

	// copy frame to RHI texture
	struct FRenderCommandData
	{
		FTextureResource* RHIResource;
		const uint8* FrameBuffer;
	};

	typedef TSharedPtr<FRenderCommandData, ESPMode::ThreadSafe> FCommandDataPtr;
	FCommandDataPtr CommandData = MakeShared<FRenderCommandData, ESPMode::ThreadSafe>();
	CommandData->RHIResource = Resource;
	CommandData->FrameBuffer = (const uint8*)(GIFDecoder->getFrameBuffer());

	//-- equeue render command
	ENQUEUE_RENDER_COMMAND(GIF_RenderFrameToTexture)(
		[CommandData](FRHICommandListImmediate& RHICmdList)
		{
			if (!CommandData->RHIResource || !CommandData->RHIResource->TextureRHI)
				return;

			FTexture2DRHIRef Texture2DRHI = CommandData->RHIResource->TextureRHI->GetTexture2D();
			if (!Texture2DRHI)
				return;

			uint32 TexWidth = Texture2DRHI->GetSizeX();
			uint32 TexHeight = Texture2DRHI->GetSizeY();
			uint32 SrcPitch = TexWidth * sizeof(FColor);
			
			FUpdateTextureRegion2D Region;
			Region.SrcX = Region.SrcY = Region.DestX = Region.DestY = 0;
			Region.Width = TexWidth;
			Region.Height = TexHeight;

			RHIUpdateTexture2D(Texture2DRHI, 0, Region, SrcPitch, CommandData->FrameBuffer);
		});

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
