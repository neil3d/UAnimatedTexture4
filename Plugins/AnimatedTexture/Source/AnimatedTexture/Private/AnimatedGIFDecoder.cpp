
#include "AnimatedGIFDecoder.h"

void UAnimatedGIFDecoder::Import_Init(uint32 InGlobalWidth, uint32 InGlobalHeight, uint32 InPaletteSize, uint32 FrameCount)
{
	GlobalWidth = InGlobalWidth;
	GlobalHeight = InGlobalHeight;

	Palette.Init(FColor::Black, InPaletteSize);
	Frames.Init(FGIFFrame(), FrameCount);
}
