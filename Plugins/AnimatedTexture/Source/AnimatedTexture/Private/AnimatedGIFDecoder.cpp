
#include "AnimatedGIFDecoder.h"
#include "RenderingThread.h"	// RenderCore

void UAnimatedGIFDecoder::Import_Init(uint32 InGlobalWidth, uint32 InGlobalHeight, uint32 InPaletteSize, uint32 FrameCount)
{
	GlobalWidth = InGlobalWidth;
	GlobalHeight = InGlobalHeight;

	Palette.Init(FColor::Black, InPaletteSize);
	Frames.Init(FGIFFrame(), FrameCount);
}

void UAnimatedGIFDecoder::DecodeFrameToRHI(FTextureResource * RHIResource, int FrameIndex)
{
	//-- Render Command Data
	struct FRenderCommandData
	{
		FTextureResource * RHIResource;
		FGIFFrame* GIFFrame;
		TArray<FColor>* Palette;
	};

	typedef TSharedPtr<FRenderCommandData, ESPMode::ThreadSafe> FCommandDataPtr;
	FCommandDataPtr CommandData = MakeShared<FRenderCommandData, ESPMode::ThreadSafe>();

	CommandData->GIFFrame = &Frames[FrameIndex];
	CommandData->RHIResource = RHIResource;
	CommandData->Palette = &Palette;

	//-- Equeue command
	ENQUEUE_RENDER_COMMAND(DecodeGIFFrameToTexture)(
		[CommandData](FRHICommandListImmediate& RHICmdList)
	{
		if (!CommandData->RHIResource || !CommandData->RHIResource->TextureRHI)
			return;

		FTexture2DRHIRef Texture2DRHI = CommandData->RHIResource->TextureRHI->GetTexture2D();
		if (!Texture2DRHI)
			return;

		uint32 DestPitch = 0;
		FColor* DestinationBuffer = (FColor*)RHILockTexture2D(Texture2DRHI, 0, RLM_WriteOnly, DestPitch, false);
		if (!DestinationBuffer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to lock texture for write"));
			return;
		}

		//-- write texture
		TArray<FColor>& Palette = *(CommandData->Palette);
		FGIFFrame& GIFFrame = *(CommandData->GIFFrame);

		int32 TexW = Texture2DRHI->GetSizeX();
		int32 TexH = Texture2DRHI->GetSizeY();

		for (uint32 y = 0; y < GIFFrame.Height; y++) {
			for (uint32 x = 0; x < GIFFrame.Width; x++) {
				uint32 PixeIndex = y * GIFFrame.Width + x;

				DestinationBuffer[(y + GIFFrame.OffsetY)*TexW + x + GIFFrame.OffsetX] = Palette[GIFFrame.PixelIndices[PixeIndex]];
			}
		}


		//--
		RHIUnlockTexture2D(Texture2DRHI, 0, false);
	}
	);
}
