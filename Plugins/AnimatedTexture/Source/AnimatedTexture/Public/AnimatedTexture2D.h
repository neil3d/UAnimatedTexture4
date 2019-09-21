// Fill out your copyright notice in the Description page of Project Settings.

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
