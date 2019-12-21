/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub Repo: https://github.com/neil3d/UnrealAnimatedTexturePlugin
 * GitHub Page: http://neil3d.github.io
 *
*/

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#include "AnimatedTextureSource.h"
#include "AnimatedGIFDecoder.generated.h"

struct FGIFFrame
{
	float Time;	// next frame delay in sec
	uint32 Index;	// 0-based index of the current frame
	uint32 Width;	// current frame width
	uint32 Height;	// current frame height
	uint32 OffsetX;	// current frame horizontal offset
	uint32 OffsetY;	// current frame vertical offset
	bool Interlacing;	// see: https://en.wikipedia.org/wiki/GIF#Interlacing
	uint8 Mode;	// next frame (sic next, not current) blending mode
	int16 TransparentIndex;	// 0-based transparent color index (or −1 when transparency is disabled)
	TArray<uint8> PixelIndices;	// pixel indices for the current frame
	TArray<FColor> Palette;	// the current palette

	FGIFFrame() :Time(0), Index(0), Width(0), Height(0), OffsetX(0), OffsetY(0),
		Interlacing(false), Mode(0), TransparentIndex(-1)
	{}

	// Serializer.
	friend FArchive& operator<<(FArchive& Ar, FGIFFrame& Frm)
	{
		return Ar << Frm.Time << Frm.Index
			<< Frm.Width << Frm.Height
			<< Frm.OffsetX << Frm.OffsetY
			<< Frm.Interlacing << Frm.Mode << Frm.TransparentIndex
			<< Frm.PixelIndices
			<< Frm.Palette;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

#if ENGINE_MAJOR_VERSION <= 4 && ENGINE_MINOR_VERSION > 23
	friend void operator<<(FStructuredArchive::FSlot Slot, FGIFFrame& Frm)
	{
		FStructuredArchive::FRecord Record = Slot.EnterRecord();
		Record << SA_VALUE(TEXT("Time"), Frm.Time) << SA_VALUE(TEXT("Index"), Frm.Index)
			<< SA_VALUE(TEXT("Width"), Frm.Width) << SA_VALUE(TEXT("Height"), Frm.Height)
			<< SA_VALUE(TEXT("OffsetX"), Frm.OffsetX) << SA_VALUE(TEXT("OffsetY"), Frm.OffsetY)
			<< SA_VALUE(TEXT("Interlacing"), Frm.Interlacing) << SA_VALUE(TEXT("Mode"), Frm.Mode)
			<< SA_VALUE(TEXT("TransparentIndex"), Frm.TransparentIndex)
			<< SA_VALUE(TEXT("PixelIndices"), Frm.PixelIndices)
			<< SA_VALUE(TEXT("Palette"), Frm.Palette)
			;
		}
#else
	friend void operator<<(FStructuredArchive::FSlot Slot, FGIFFrame& Frm)
	{
		FStructuredArchive::FRecord Record = Slot.EnterRecord();
		Record << NAMED_ITEM("Time", Frm.Time) << NAMED_ITEM("Index", Frm.Index)
			<< NAMED_ITEM("Width", Frm.Width) << NAMED_ITEM("Height", Frm.Height)
			<< NAMED_ITEM("OffsetX", Frm.OffsetX) << NAMED_ITEM("OffsetY", Frm.OffsetY)
			<< NAMED_ITEM("Interlacing", Frm.Interlacing) << NAMED_ITEM("Mode", Frm.Mode) << NAMED_ITEM("TransparentIndex", Frm.TransparentIndex)
			<< NAMED_ITEM("PixelIndices", Frm.PixelIndices)
			<< NAMED_ITEM("Palette", Frm.Palette)
			;
	}
#endif

	bool Serialize(FStructuredArchive::FSlot Slot)
	{
		Slot << *this;
		return true;
	}
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
	void Import_Finished();

	FGIFFrame& GetFrame(int32 Index) {
		return Frames[Index];
	}

	virtual uint32 GetGlobalWidth() const override { return GlobalWidth; }
	virtual uint32 GetGlobalHeight() const override { return GlobalHeight; }
	virtual float GetFrameDelay(int FrameIndex) const override;
	virtual int GetFrameCount() const override { return Frames.Num(); }
	virtual float GetTotalDuration() const { return Duration; }

	virtual void DecodeFrameToRHI(FTextureResource* RHIResource, FAnmatedTextureState& AnimState, bool SupportsTransparency) override;
	virtual void Serialize(FArchive& Ar) override;

public:
	UPROPERTY()
		uint32 GlobalWidth = 0;

	UPROPERTY()
		uint32 GlobalHeight = 0;

	UPROPERTY()
		uint8 Background = 0;	// 0-based background color index for the current palette

	UPROPERTY()
		float Duration = 0.0f;

	TArray<FGIFFrame> Frames;

protected:
	TArray<FColor>	FrameBuffer[2];
	uint32 LastFrame;
};
