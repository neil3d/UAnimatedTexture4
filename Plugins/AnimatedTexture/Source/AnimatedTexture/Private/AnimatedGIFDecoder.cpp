
#include "AnimatedGIFDecoder.h"
#include "AnimatedTextureModule.h"
#include "RenderingThread.h"	// RenderCore

void UAnimatedGIFDecoder::Import_Init(uint32 InGlobalWidth, uint32 InGlobalHeight, uint8 InBackground, uint32 InFrameCount)
{
	GlobalWidth = InGlobalWidth;
	GlobalHeight = InGlobalHeight;
	Background = InBackground;

	Frames.Init(FGIFFrame(), InFrameCount);
}

float UAnimatedGIFDecoder::GetFrameDelay(int FrameIndex) const
{
	const FGIFFrame& Frame = Frames[FrameIndex];
	return Frame.Time;
}

void UAnimatedGIFDecoder::DecodeFrameToRHI(FTextureResource * RHIResource, int FrameIndex)
{
	//-- Render Command Data
	struct FRenderCommandData
	{
		FTextureResource * RHIResource;
		FGIFFrame* GIFFrame;
	};

	typedef TSharedPtr<FRenderCommandData, ESPMode::ThreadSafe> FCommandDataPtr;
	FCommandDataPtr CommandData = MakeShared<FRenderCommandData, ESPMode::ThreadSafe>();

	CommandData->GIFFrame = &Frames[FrameIndex];
	CommandData->RHIResource = RHIResource;

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
			UE_LOG(LogAnimTexture, Warning, TEXT("Unable to lock texture for write"));
			return;
		}

		//-- write texture
		FGIFFrame& GIFFrame = *(CommandData->GIFFrame);
		TArray<FColor>& Pal = GIFFrame.Palette;

		uint32 TexW = Texture2DRHI->GetSizeX();
		uint32 TexH = Texture2DRHI->GetSizeY();

		uint32 Dest = TexW * GIFFrame.OffsetY + GIFFrame.OffsetX;
		uint32 Src = 0;
		uint32 Iter = GIFFrame.Interlacing ? 0 : 4;
		uint32 Fin = GIFFrame.Interlacing ? 4 : 5;

		FMemory::Memzero(DestinationBuffer, TexW*TexH * 4);

		for (; Iter < Fin; Iter++) {
			uint32 YOffset = 16U >> ((Iter > 1) ? Iter : 1);
			for (uint32 Y = (8 >> Iter) & 7; Y < GIFFrame.Height; Y += YOffset) {
				for (uint32 X = 0; X < GIFFrame.Width; X++) {
					uint32 TexIndex = GIFFrame.Width*Y + X + Dest;
					uint8 PixelIndex = GIFFrame.PixelIndices[Src];
					if (PixelIndex == GIFFrame.TransparentIndex)
						DestinationBuffer[TexIndex].A = 0;
					else
						DestinationBuffer[TexIndex] = Pal[PixelIndex];
					Src++;
				}// end of for(x)
			}// end of for(y)
		}// end of for(iter)

		for (uint32 y = 0; y < GIFFrame.Height; y++) {
			for (uint32 x = 0; x < GIFFrame.Width; x++) {
				uint32 PixeIndex = y * GIFFrame.Width + x;

				DestinationBuffer[(y + GIFFrame.OffsetY)*TexW + x + GIFFrame.OffsetX] = Pal[GIFFrame.PixelIndices[PixeIndex]];
			}
		}


		//--
		RHIUnlockTexture2D(Texture2DRHI, 0, false);
	}
	);
}
