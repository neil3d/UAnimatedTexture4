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

#include "AnimatedTextureFactory.h"
#include "EditorReimportHandler.h"	// UnrealEd
#include "ReimportAnimatedTextureFactory.generated.h"

class UAnimatedTexture2D;

/**
 * Reimport Animated Texture Source
 */
UCLASS()
class ANIMATEDTEXTUREEDITOR_API UReimportAnimatedTextureFactory : public UAnimatedTextureFactory, public FReimportHandler
{
	GENERATED_BODY()

public:
	//~ Begin FReimportHandler Interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	virtual int32 GetPriority() const override;
	//~ End FReimportHandler Interface

};
