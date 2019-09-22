/**
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub Repo: https://github.com/neil3d/UnrealAnimatedTexturePlugin
 * GitHub Page: http://neil3d.github.io
 *
*/

#pragma once

#include "CoreMinimal.h"
#include "AnimatedTextureSource.h"
#include "AnimatedGIFDecoder.generated.h"

/**
 * Animated GIF Data Storage & Runtime Decoder
 */
UCLASS()
class ANIMATEDTEXTURE_API UAnimatedGIFDecoder : public UAnimatedTextureSource
{
	GENERATED_BODY()

public:
	struct FFrame {
		//-- delay, frame number
		uint32 Time,
			iFrame;
		
		//-- current frame dimensions and offset
		uint32 FrameWidth,
			FrameHeight;
		uint32 FrameOffsetX,
			FrameOffsetY;

		//-- frame pixel indices or metadata      
		TArray<uint8>	PixelIndices;
	};

	void Import_Init(uint32 InGlobalWidth, uint32 InGlobalHeight, uint32 InPaletteSize, uint32 FrameCount);

	int32 GetFrameCount() const {
		return Frames.Num();
	}
	FFrame& GetFrame(int32 Index) {
		return Frames[Index];
	}

	TArray<FColor>& GetPalette() {
		return Palette;
	}

protected:
	uint32 GlobalWidth;
	uint32 GlobalHeight;

	TArray<FColor> Palette;
	TArray<FFrame> Frames;
};
