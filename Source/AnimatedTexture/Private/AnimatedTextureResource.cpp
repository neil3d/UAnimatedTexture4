// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTextureResource.h"
#include "AnimatedTexture2D.h"
#include "AnimatedTextureSource.h"

#include "DeviceProfiles/DeviceProfile.h"	// Engine
#include "DeviceProfiles/DeviceProfileManager.h"	// Engine

FAnimatedTextureResource::FAnimatedTextureResource(UAnimatedTexture2D * InOwner) :Owner(InOwner)
{
}

uint32 FAnimatedTextureResource::GetSizeX() const
{
	if (Owner && Owner->GetAnimSource())
		return Owner->GetAnimSource()->GetGlobalWidth();
	return 2;
}

uint32 FAnimatedTextureResource::GetSizeY() const
{
	if (Owner && Owner->GetAnimSource())
		return Owner->GetAnimSource()->GetGlobalHeight();

	return 2;
}

void FAnimatedTextureResource::InitRHI()
{
	//-- create FSamplerStateRHIRef FTexture::SamplerStateRHI
	CreateSamplerStates(
		GetDefaultMipMapBias()
	);

	//-- create FTextureRHIRef FTexture::TextureRHI
	uint32 Flags = 0;
	uint32 NumMips = 1;

	FRHIResourceCreateInfo CreateInfo;
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
		(ESamplerFilter)UDeviceProfileManager::Get().GetActiveProfile()->GetTextureLODSettings()->GetSamplerFilter(Owner),
		Owner->AddressX == TA_Wrap ? AM_Wrap : (Owner->AddressX == TA_Clamp ? AM_Clamp : AM_Mirror),
		Owner->AddressY == TA_Wrap ? AM_Wrap : (Owner->AddressY == TA_Clamp ? AM_Clamp : AM_Mirror),
		AM_Wrap,
		MipMapBias
	);
	SamplerStateRHI = RHICreateSamplerState(SamplerStateInitializer);

	FSamplerStateInitializerRHI DeferredPassSamplerStateInitializer
	(
		(ESamplerFilter)UDeviceProfileManager::Get().GetActiveProfile()->GetTextureLODSettings()->GetSamplerFilter(Owner),
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