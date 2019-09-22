
#include "AnimatedGIFDecoder.h"
#include "gif_load/gif_load.h" // from: https://github.com/hidefromkgb/gif_load

extern "C"
{
	void GIFFrameLoader(void *data, struct GIF_WHDR *whdr) {
		UAnimatedGIFDecoder* OutGIF = (UAnimatedGIFDecoder*)data;

		//-- init on first frame
		if (OutGIF->GetFrameCount() == 0) {
			int PaletteSize = whdr->clrs;
			OutGIF->Import_Init(whdr->xdim, whdr->ydim, whdr->clrs, whdr->nfrm);

			TArray<FColor>& Palette = OutGIF->GetPalette();
			check(Palette.Num() == PaletteSize);

			for (int i = 0; i < PaletteSize; i++) {
				FColor& uc = Palette[i];
				uc.R = whdr->cpal[i].R;
				uc.G = whdr->cpal[i].G;
				uc.B = whdr->cpal[i].B;
			}
		}

		//-- import frame
		int FrameIndex = whdr->ifrm;
		
		check(OutGIF->GetFrameCount() == whdr->nfrm);
		check(FrameIndex >= 0 && FrameIndex < OutGIF->GetFrameCount());

		FGIFFrame& Frame = OutGIF->GetFrame(FrameIndex);

		//---- copy properties
		Frame.Width = whdr->frxd;
		Frame.Height = whdr->fryd;
		Frame.OffsetX = whdr->frxo;
		Frame.OffsetY = whdr->fryo;

		Frame.Time = whdr->time;
		Frame.Index = whdr->ifrm;

		//---- copy pixel data
		int NumPixel = Frame.Width*Frame.Height;
		Frame.PixelIndices.Init(0, NumPixel);
		memcpy(Frame.PixelIndices.GetData(), whdr->bptr, NumPixel);
	}
}

void LoadGIFBinary(UAnimatedGIFDecoder* OutGIF, const uint8 * Buffer, uint32 BufferSize)
{
	GIF_Load((void*)Buffer, BufferSize, GIFFrameLoader, 0, (void*)OutGIF, 0L);
}