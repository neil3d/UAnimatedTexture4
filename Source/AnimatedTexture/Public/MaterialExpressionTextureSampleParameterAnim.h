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
#include "UObject/ObjectMacros.h"
#include "Materials/MaterialExpressionTextureSampleParameter.h"
#include "Runtime/Launch/Resources/Version.h"

#include "MaterialExpressionTextureSampleParameterAnim.generated.h"

class UTexture;

UCLASS(collapsecategories, hidecategories = Object)
class ANIMATEDTEXTURE_API UMaterialExpressionTextureSampleParameterAnim : public UMaterialExpressionTextureSampleParameter
{
	GENERATED_UCLASS_BODY()


	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
		virtual void GetCaption(TArray<FString>& OutCaptions) const override;
#endif // WITH_EDITOR
	//~ End UMaterialExpression Interface

	//~ Begin UMaterialExpressionTextureSampleParameter Interface
	virtual bool TextureIsValid(UTexture* InTexture, FString& OutMessage);
	virtual void SetDefaultTexture();
	//~ End UMaterialExpressionTextureSampleParameter Interface
};



