
#include "AnimatedGIFDecoder.h"
#include "AnimatedTextureModule.h"
#include "RenderingThread.h"	// RenderCore

enum EGIF_Mode
{
	GIF_NONE = 0,
	GIF_CURR = 1,
	GIF_BKGD = 2,
	GIF_PREV = 3
};

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

void UAnimatedGIFDecoder::DecodeFrameToRHI(FTextureResource * RHIResource, FAnmatedTextureState& AnimState)
{
	if (FrameBuffer[0].Num() != GlobalHeight * GlobalWidth) {
		Last = 0;
		for (int i = 0; i < 2; i++)
			FrameBuffer[i].Init(FColor(0, 0, 0, 0), GlobalHeight*GlobalWidth);
	}

	//-- Render Command Data
	struct FRenderCommandData
	{
		FTextureResource * RHIResource;
		FGIFFrame* GIFFrame;
		UAnimatedGIFDecoder* Decoder;
	};

	typedef TSharedPtr<FRenderCommandData, ESPMode::ThreadSafe> FCommandDataPtr;
	FCommandDataPtr CommandData = MakeShared<FRenderCommandData, ESPMode::ThreadSafe>();

	CommandData->GIFFrame = &Frames[AnimState.CurrentFrame];
	CommandData->RHIResource = RHIResource;
	CommandData->Decoder = this;

	//-- Equeue command
	ENQUEUE_RENDER_COMMAND(DecodeGIFFrameToTexture)(
		[CommandData](FRHICommandListImmediate& RHICmdList)
	{
		if (!CommandData->RHIResource || !CommandData->RHIResource->TextureRHI)
			return;

		FTexture2DRHIRef Texture2DRHI = CommandData->RHIResource->TextureRHI->GetTexture2D();
		if (!Texture2DRHI)
			return;

		FGIFFrame& GIFFrame = *(CommandData->GIFFrame);
		FColor* PICT = CommandData->Decoder->FrameBuffer[0].GetData();
		FColor* PREV = CommandData->Decoder->FrameBuffer[1].GetData();;
		uint32& Last = CommandData->Decoder->Last;

		TArray<FColor>& Pal = GIFFrame.Palette;

		uint32 XDim = Texture2DRHI->GetSizeX();
		uint32 YDim = Texture2DRHI->GetSizeY();

		//-- decode to frame buffer
		uint32 DDest = XDim * GIFFrame.OffsetY + GIFFrame.OffsetX;
		uint32 Src = 0;
		uint32 Iter = GIFFrame.Interlacing ? 0 : 4;
		uint32 Fin = !Iter ? 4 : 5;

		for (; Iter < Fin; Iter++) {
			uint32 YOffset = 16U >> ((Iter > 1) ? Iter : 1);

			for (uint32 Y = (8 >> Iter) & 7; Y < GIFFrame.Height; Y += YOffset) 
			{
				for (uint32 X = 0; X < GIFFrame.Width; X++) 
				{
					uint32 TexIndex = XDim * Y + X + DDest;
					uint8 PixelIndex = GIFFrame.PixelIndices[Src];
					PICT[TexIndex] = Pal[PixelIndex];

					if (PixelIndex == GIFFrame.TransparentIndex) 
						PICT[TexIndex].A = 0;
					Src++;
				}// end of for(x)
			}// end of for(y)
		}// end of for(iter)

		//-- write texture
		uint32 DestPitch = 0;
		FColor* SrcBuffer = PICT;
		FColor* DestBuffer = (FColor*)RHILockTexture2D(Texture2DRHI, 0, RLM_WriteOnly, DestPitch, false);
		if (!DestBuffer)
		{
			UE_LOG(LogAnimTexture, Warning, TEXT("Unable to lock texture for write"));
			return;
		}

		if (DestPitch == XDim * sizeof(FColor)) {
			FMemory::Memcpy(DestBuffer, SrcBuffer, XDim*YDim * sizeof(FColor));
		}
		else {
			// copy row by row
			uint32 SrcPitch = XDim * sizeof(FColor);
			for (uint32 y = 0; y < YDim; y++) {
				FMemory::Memcpy(DestBuffer, SrcBuffer, XDim * sizeof(FColor));
				DestBuffer += DestPitch;
				SrcBuffer += SrcPitch;
			}
		}

		RHIUnlockTexture2D(Texture2DRHI, 0, false);

		//-- frame blending
		FColor* PICT2 = nullptr;
		FColor* PREV2 = nullptr;
		uint32 FrameXD = 0;
		uint32 FrameYD = 0;
		EGIF_Mode Mode;
		if ((GIFFrame.Mode == GIF_PREV) && !Last)
		{
			FrameXD = XDim;
			FrameYD = YDim;
			Mode = GIF_BKGD;
			DDest = 0;
		}
		else
		{
			Last = (GIFFrame.Mode == GIF_PREV) ? Last : GIFFrame.Index + 1;
			PICT2 = ((GIFFrame.Mode == GIF_PREV) ? PICT : PREV);
			PREV2 = ((GIFFrame.Mode == GIF_PREV) ? PREV : PICT);
			for (uint32 x = XDim * YDim; x > 0; --x)
				PICT2[x - 1] = PREV2[x - 1];
		}

		// cutting a hole for the next frame
		if (GIFFrame.Mode == GIF_BKGD) 
		{
			FColor CLR;
			if (GIFFrame.TransparentIndex >= 0) {
				CLR = GIFFrame.Palette[GIFFrame.TransparentIndex];
				CLR.A = 0;
			}
			else
				CLR = GIFFrame.Palette[CommandData->Decoder->Background];

			for (uint32 y = 0; y < FrameYD; y++)
				for (uint32 x = 0; x < FrameXD; x++)
					PICT[XDim* y + x + DDest] = CLR;
		}
	}
	);
}
