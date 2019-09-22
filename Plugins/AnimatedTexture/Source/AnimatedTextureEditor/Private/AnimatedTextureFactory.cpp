
#include "AnimatedTextureFactory.h"
#include "AnimatedTextureEditorModule.h"
#include "AnimatedTexture2D.h"
#include "AnimatedTextureSource.h"

#include "EditorFramework/AssetImportData.h"	// Engine
#include "Subsystems/ImportSubsystem.h"	// UnrealEd
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

bool UAnimatedTextureFactory::FactoryCanImport(const FString & Filename)
{
	FString Extension = FPaths::GetExtension(Filename, true);

	return Extension.Compare(TEXT(".gif"), ESearchCase::IgnoreCase);
}

UObject * UAnimatedTextureFactory::FactoryCreateBinary(UClass * Class, UObject * InParent, FName Name, EObjectFlags Flags, UObject * Context,
	const TCHAR * Type, const uint8 *& Buffer, const uint8 * BufferEnd, FFeedbackContext * Warn)
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


	// load gif file
	UAnimatedTextureSource* GIFSource = ImportGIF(Buffer, BufferEnd - Buffer);
	if (GIFSource) {
		AnimTexture->SetAnimSource(GIFSource);
	}
	else {
		UE_LOG(LogAnimTextureEditor, Error, TEXT("Import GIF FAILED, Name=%s."), *(Name.ToString()));
		return nullptr;
	}

	//Replace the reference for the new texture with the existing one so that all current users still have valid references.
	RefReplacer.Replace(AnimTexture);

	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, AnimTexture);

	// Invalidate any materials using the newly imported texture. (occurs if you import over an existing texture)
	AnimTexture->PostEditChange();

	return AnimTexture;
}

bool UAnimatedTextureFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames) {
	UAnimatedTexture2D* pTex = Cast<UAnimatedTexture2D>(Obj);
	if (pTex) {
		pTex->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}
	return false;
}

void UAnimatedTextureFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) {
	UAnimatedTexture2D* pTex = Cast<UAnimatedTexture2D>(Obj);
	if (pTex && ensure(NewReimportPaths.Num() == 1))
	{
		pTex->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

EReimportResult::Type UAnimatedTextureFactory::Reimport(UObject* Obj) {
	return EReimportResult::Succeeded;
}

int32 UAnimatedTextureFactory::GetPriority() const {
	return ImportPriority;
}

UAnimatedTextureSource * UAnimatedTextureFactory::ImportGIF(const uint8 * Buffer, uint32 BufferSize)
{
	return nullptr;
}
