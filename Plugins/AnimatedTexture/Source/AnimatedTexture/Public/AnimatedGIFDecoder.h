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

public:
	UPROPERTY()
		float Time;	// next frame delay in sec

	UPROPERTY()
		uint32 Index;	// 0-based index of the current frame

	UPROPERTY()
		uint32 Width;	// current frame width

	UPROPERTY()
		uint32 Height;	// current frame height

	UPROPERTY()
		uint32 OffsetX;	// current frame horizontal offset

	UPROPERTY()
		uint32 OffsetY;	// current frame vertical offset

	UPROPERTY()
		bool Interlacing;	// see: https://en.wikipedia.org/wiki/GIF#Interlacing

	UPROPERTY()
		uint8 Mode;	// next frame (sic next, not current) blending mode

	UPROPERTY()
		int16 TransparentIndex;	// 0-based transparent color index (or −1 when transparency is disabled)

	UPROPERTY()
		TArray<uint8> PixelIndices;	// pixel indices for the current frame

	UPROPERTY()
		TArray<FColor> Palette;	// the current palette

	FGIFFrame() :Time(0), Index(0), Width(0), Height(0), OffsetX(0), OffsetY(0),
		Interlacing(false),Mode(0),TransparentIndex(-1)
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
	void Import_Init(uint32 InGlobalWidth, uint32 InGlobalHeight, uint8 InBackground, uint32 InFrameCount);

	FGIFFrame& GetFrame(int32 Index) {
		return Frames[Index];
	}

	virtual uint32 GetGlobalWidth() const override { return GlobalWidth; }
	virtual uint32 GetGlobalHeight() const override { return GlobalHeight; }
	virtual float GetFrameDelay(int FrameIndex) const override;
	virtual int GetFrameCount() const override { return Frames.Num(); }

	virtual void DecodeFrameToRHI(FTextureResource* RHIResource, FAnmatedTextureState& AnimState) override;

public:
	UPROPERTY()
		uint32 GlobalWidth;

	UPROPERTY()
		uint32 GlobalHeight;

	UPROPERTY()
		uint8 Background;	// 0-based background color index for the current palette

	UPROPERTY()
		TArray<FGIFFrame> Frames;

public:
	TArray<FColor>	FrameBuffer[2];
	uint32 Last;
};
