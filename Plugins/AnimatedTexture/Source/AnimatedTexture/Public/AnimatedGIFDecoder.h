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

USTRUCT()
struct FGIFFrame
{
	GENERATED_BODY()

	//-- delay, frame number
	UPROPERTY()
		uint32 Time;

	UPROPERTY()
		uint32 Index;

	//-- current frame dimensions and offset
	UPROPERTY()
		uint32 Width;

	UPROPERTY()
		uint32 Height;

	UPROPERTY()
		uint32 OffsetX;

	UPROPERTY()
		uint32 OffsetY;

	//-- frame pixel indices or metadata      
	UPROPERTY()
		TArray<uint8>	PixelIndices;

	FGIFFrame():Time(0),Index(0),Width(0),Height(0),OffsetX(0),OffsetY(0)
	{}
};

/**
 * Animated GIF Data Storage & Runtime Decoder
 */
UCLASS()
class ANIMATEDTEXTURE_API UAnimatedGIFDecoder : public UAnimatedTextureSource
{
	GENERATED_BODY()

public:
	void Import_Init(uint32 InGlobalWidth, uint32 InGlobalHeight, uint32 InPaletteSize, uint32 FrameCount);

	int32 GetFrameCount() const {
		return Frames.Num();
	}
	FGIFFrame& GetFrame(int32 Index) {
		return Frames[Index];
	}

	TArray<FColor>& GetPalette() {
		return Palette;
	}

	virtual uint32 GetGlobalWidth() const override { return GlobalWidth; }
	virtual uint32 GetGlobalHeight() const override { return GlobalHeight; }
	virtual void DecodeFrameToRHI(FTextureResource* RHIResource, int Frame) override;

protected:
	UPROPERTY()
	uint32 GlobalWidth;

	UPROPERTY()
	uint32 GlobalHeight;

	UPROPERTY()
	TArray<FColor> Palette;

	UPROPERTY()
	TArray<FGIFFrame> Frames;
};
