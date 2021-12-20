// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTextureResource.h"
#include "AnimatedTexture2D.h"

#include "DeviceProfiles/DeviceProfile.h"	// Engine
#include "DeviceProfiles/DeviceProfileManager.h"	// Engine

FAnimatedTextureResource::FAnimatedTextureResource(UAnimatedTexture2D* InOwner) :Owner(InOwner)
{
}

uint32 FAnimatedTextureResource::GetSizeX() const
{
	if (Owner) return Owner->GetSurfaceWidth();
	return 2;
}

uint32 FAnimatedTextureResource::GetSizeY() const
{
	if (Owner)
		return Owner->GetSurfaceHeight();

	return 2;
}

static ESamplerAddressMode _ConvertAddressMode(enum TextureAddress addr)
{
	ESamplerAddressMode ret = AM_Wrap;
	switch (addr)
	{
	case TA_Wrap:
		ret = AM_Wrap;
		break;
	case TA_Clamp:
		ret = AM_Clamp;
		break;
	case TA_Mirror:
		ret = AM_Mirror;
		break;
	}
	return ret;
}

void FAnimatedTextureResource::InitRHI()
{
	// Create the sampler state RHI resource.
	ESamplerAddressMode AddressU = _ConvertAddressMode(Owner->AddressX);
	ESamplerAddressMode AddressV = _ConvertAddressMode(Owner->AddressY);
	ESamplerAddressMode AddressW = AM_Wrap;

	FSamplerStateInitializerRHI SamplerStateInitializer
	(
		(ESamplerFilter)UDeviceProfileManager::Get().GetActiveProfile()->GetTextureLODSettings()->GetSamplerFilter(Owner),
		AddressU,
		AddressV,
		AddressW
	);
	SamplerStateRHI = GetOrCreateSamplerState(SamplerStateInitializer);

	ETextureCreateFlags  Flags = TexCreate_None;
	if (!Owner->SRGB)
		bIgnoreGammaConversions = true;

	if (Owner->SRGB)
		Flags |= TexCreate_SRGB;
	if (Owner->bNoTiling)
		Flags |= TexCreate_NoTiling;

	uint32 NumMips = 1;
	FString Name = Owner->GetName();
	FRHIResourceCreateInfo CreateInfo(*Name);
	TextureRHI = RHICreateTexture2D(GetSizeX(), GetSizeY(), PF_B8G8R8A8, NumMips, 1, Flags, CreateInfo);
	TextureRHI->SetName(Owner->GetFName());
	RHIUpdateTextureReference(Owner->TextureReference.TextureReferenceRHI, TextureRHI);

	if (Owner)
		Owner->UpdateFirstFrame();
}

void FAnimatedTextureResource::ReleaseRHI()
{
	FTextureResource::ReleaseRHI();
}

int32 FAnimatedTextureResource::GetDefaultMipMapBias() const
{
	return 0;
}

void FAnimatedTextureResource::CreateSamplerStates(float MipMapBias)
{
	FSamplerStateInitializerRHI SamplerStateInitializer
	(
		SF_Bilinear,
		Owner->AddressX == TA_Wrap ? AM_Wrap : (Owner->AddressX == TA_Clamp ? AM_Clamp : AM_Mirror),
		Owner->AddressY == TA_Wrap ? AM_Wrap : (Owner->AddressY == TA_Clamp ? AM_Clamp : AM_Mirror),
		AM_Wrap,
		MipMapBias
	);
	SamplerStateRHI = RHICreateSamplerState(SamplerStateInitializer);

	FSamplerStateInitializerRHI DeferredPassSamplerStateInitializer
	(
		SF_Bilinear,
		Owner->AddressX == TA_Wrap ? AM_Wrap : (Owner->AddressX == TA_Clamp ? AM_Clamp : AM_Mirror),
		Owner->AddressY == TA_Wrap ? AM_Wrap : (Owner->AddressY == TA_Clamp ? AM_Clamp : AM_Mirror),
		AM_Wrap,
		MipMapBias,
		1,
		0,
		2
	);

	DeferredPassSamplerStateRHI = RHICreateSamplerState(DeferredPassSamplerStateInitializer);
}