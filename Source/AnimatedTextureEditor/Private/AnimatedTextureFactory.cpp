// Copyright 2019 Neil Fang. All Rights Reserved.

#include "AnimatedTextureFactory.h"
#include "AnimatedTextureEditorModule.h"
#include "AnimatedTexture2D.h"

#include "Runtime/Launch/Resources/Version.h"
#include "Subsystems/ImportSubsystem.h"	// UnrealEd
#include "EditorFramework/AssetImportData.h"	// Engine
#include "Editor.h"	// UnrealEd

UAnimatedTextureFactory::UAnimatedTextureFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UAnimatedTexture2D::StaticClass();

	Formats.Add(TEXT("gif;GIF(Animation Supported)"));

	bCreateNew = false;
	bEditorImport = true;

	// Required to checkbefore UReimportTextureFactory
	ImportPriority = DefaultImportPriority + 1;
}

bool UAnimatedTextureFactory::DoesSupportClass(UClass* Class)
{
	return Class == UAnimatedTexture2D::StaticClass();
}

bool UAnimatedTextureFactory::FactoryCanImport(const FString& Filename)
{
	FString Extension = FPaths::GetExtension(Filename, true);

	return Extension.Compare(TEXT(".gif"), ESearchCase::IgnoreCase) == 0;
}

UObject* UAnimatedTextureFactory::FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context,
	const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	check(Type);
	check(Class == UAnimatedTexture2D::StaticClass());

	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, Class, InParent, Name, Type);

	// if the texture already exists, remember the user settings
	UAnimatedTexture2D* ExistingTexture = FindObject<UAnimatedTexture2D>(InParent, *Name.ToString());
	if (ExistingTexture) {
		// TODO: use existing object settings
	}

	FTextureReferenceReplacer RefReplacer(ExistingTexture);

	// call parent method to create/overwrite anim texture object
	UAnimatedTexture2D* AnimTexture = Cast<UAnimatedTexture2D>(
		CreateOrOverwriteAsset(Class, InParent, Name, Flags)
		);
	if (AnimTexture == nullptr) {
		UE_LOG(LogAnimTextureEditor, Error, TEXT("Create Animated Texture FAILED, Name=%s."), *(Name.ToString()));
		return nullptr;
	}

	// just copy file blob to AnimTexture object
    AnimTexture->ImportFile(Buffer, BufferEnd-Buffer);

	//Replace the reference for the new texture with the existing one so that all current users still have valid references.
	RefReplacer.Replace(AnimTexture);

	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, AnimTexture);

	// Invalidate any materials using the newly imported texture. (occurs if you import over an existing texture)
	AnimTexture->PostEditChange();

	return AnimTexture;
}
