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
#include "Engine/Texture2DDynamic.h"
#include "AnimatedTexture2D.generated.h"

class UAnimatedTextureSource;

/**
 * 
 */
UCLASS()
class ANIMATEDTEXTURE_API UAnimatedTexture2D : public UTexture2DDynamic
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UAnimatedTextureSource* AnimSource;
};
