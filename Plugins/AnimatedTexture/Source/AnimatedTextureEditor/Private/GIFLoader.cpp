
#include "gif_load/gif_load.h"
#include "AnimatedGIFDecoder.h"

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
		UAnimatedGIFDecoder::FFrame& ImportFrame = OutGIF->GetFrame(whdr->ifrm);

		//---- copy properties
		ImportFrame.FrameWidth = whdr->frxd;
		ImportFrame.FrameHeight = whdr->fryd;
		ImportFrame.FrameOffsetX = whdr->frxo;
		ImportFrame.FrameOffsetY = whdr->fryo;

		ImportFrame.Time = whdr->time;
		ImportFrame.iFrame = whdr->ifrm;

		//---- copy pixel data
		int NumPixel = ImportFrame.FrameWidth*ImportFrame.FrameHeight;
		ImportFrame.PixelIndices.Init(0, NumPixel);
		memcpy(ImportFrame.PixelIndices.GetData(), whdr->bptr, NumPixel);
	}
}

void LoadGIFBinary(UAnimatedGIFDecoder* OutGIF, const uint8 * Buffer, uint32 BufferSize)
{
	GIF_Load((void*)Buffer, BufferSize, GIFFrameLoader, 0, (void*)OutGIF, 0L);
}