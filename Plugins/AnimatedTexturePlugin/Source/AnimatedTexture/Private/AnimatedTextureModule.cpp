// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTextureModule.h"

DEFINE_LOG_CATEGORY(LogAnimTexture);
#define LOCTEXT_NAMESPACE "FAnimatedTextureModule"

void FAnimatedTextureModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAnimatedTextureModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAnimatedTextureModule, AnimatedTexture)