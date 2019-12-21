// Copyright 2019 Neil Fang. All Rights Reserved.

#include "ReimportAnimatedTextureFactory.h"
#include "AnimatedTextureEditorModule.h"
#include "AnimatedTexture2D.h"

#include "HAL/FileManager.h"	// Core
#include "EditorFramework/AssetImportData.h"	// Engine

bool UReimportAnimatedTextureFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UAnimatedTexture2D* pTex = Cast<UAnimatedTexture2D>(Obj);
	if (pTex) {
		pTex->AssetImportData->ExtractFilenames(OutFilenames);
		return true;
	}
	return false;
}

void UReimportAnimatedTextureFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UAnimatedTexture2D* pTex = Cast<UAnimatedTexture2D>(Obj);
	if (pTex && ensure(NewReimportPaths.Num() == 1))
	{
		pTex->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

EReimportResult::Type UReimportAnimatedTextureFactory::Reimport(UObject* Obj)
{
	if (!Obj || !Obj->IsA(UAnimatedTexture2D::StaticClass()))
	{
		return EReimportResult::Failed;
	}

	UAnimatedTexture2D* pTex = Cast<UAnimatedTexture2D>(Obj);

	const FString ResolvedSourceFilePath = pTex->AssetImportData->GetFirstFilename();
	if (!ResolvedSourceFilePath.Len())
	{
		return EReimportResult::Failed;
	}

	// Ensure that the file provided by the path exists
	if (IFileManager::Get().FileSize(*ResolvedSourceFilePath) == INDEX_NONE)
	{
		UE_LOG(LogAnimTextureEditor, Warning, TEXT("cannot reimport: [] file cannot be found."), *ResolvedSourceFilePath);
		return EReimportResult::Failed;
	}

	enum TextureAddress AddressX = pTex->AddressX;
	enum TextureAddress AddressY = pTex->AddressY;
	float PlayRate = pTex->PlayRate;
	bool SupportsTransparency = pTex->SupportsTransparency;
	float DefaultFrameDelay = pTex->DefaultFrameDelay;
	bool bLooping = pTex->bLooping;

	bool OutCanceled = false;
	if (ImportObject(pTex->GetClass(), pTex->GetOuter(), *pTex->GetName(), RF_Public | RF_Standalone, ResolvedSourceFilePath, nullptr, OutCanceled) != nullptr)
	{
		pTex->AssetImportData->Update(ResolvedSourceFilePath);

		pTex->AddressX = AddressX;
		pTex->AddressY = AddressY;
		pTex->PlayRate = PlayRate;
		pTex->SupportsTransparency = SupportsTransparency;
		pTex->DefaultFrameDelay = DefaultFrameDelay;
		pTex->bLooping = bLooping;

		// Try to find the outer package so we can dirty it up
		if (pTex->GetOuter())
		{
			pTex->GetOuter()->MarkPackageDirty();
		}
		else
		{
			pTex->MarkPackageDirty();
		}
	}
	else if (OutCanceled)
	{
		UE_LOG(LogAnimTextureEditor, Warning, TEXT("[%s] reimport canceled"), *(Obj->GetName()));
		return EReimportResult::Cancelled;
	}
	else
	{
		UE_LOG(LogAnimTextureEditor, Warning, TEXT("[%s] reimport failed"), *(Obj->GetName()));
		return EReimportResult::Failed;
	}

	return EReimportResult::Succeeded;
}

int32 UReimportAnimatedTextureFactory::GetPriority() const
{
	return ImportPriority;
}

