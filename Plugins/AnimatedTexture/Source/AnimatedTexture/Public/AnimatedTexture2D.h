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
#include "Engine/Texture.h"
#include "AnimatedTexture2D.generated.h"

class UAnimatedTextureSource;

/**
 * 
 */
UCLASS(BlueprintType, Category = AnimatedTexture)
class ANIMATEDTEXTURE_API UAnimatedTexture2D : public UTexture
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimatedTexture, meta = (DisplayName = "X-axis Tiling Method"), AssetRegistrySearchable, AdvancedDisplay)
		TEnumAsByte<enum TextureAddress> AddressX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimatedTexture, meta = (DisplayName = "Y-axis Tiling Method"), AssetRegistrySearchable, AdvancedDisplay)
		TEnumAsByte<enum TextureAddress> AddressY;
	
public:
	void SetAnimSource(UAnimatedTextureSource* AnimSource);
	UAnimatedTextureSource* GetAnimSource() const { return AnimSource; }

	//~ Begin UTexture Interface.
	virtual float GetSurfaceWidth() const override;
	virtual float GetSurfaceHeight() const override;
	virtual FTextureResource* CreateResource() override;
	virtual EMaterialValueType GetMaterialType() const override { return MCT_Texture2D; }
	//~ End UTexture Interface.

protected:
	UPROPERTY()
	UAnimatedTextureSource* AnimSource;
};
