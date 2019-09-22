
#include "AnimatedGIFDecoder.h"

/*
 * using: https://github.com/hidefromkgb/gif_load
*/

void UAnimatedGIFDecoder::Import_Init(uint32 InGlobalWidth, uint32 InGlobalHeight, uint32 InPaletteSize, uint32 FrameCount)
{
	GlobalWidth = InGlobalWidth;
	GlobalHeight = InGlobalHeight;

	Palette.Init(FColor::Black, InPaletteSize);
	Frames.Init(FFrame(), FrameCount);
}
