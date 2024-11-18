// Fill out your copyright notice in the Description page of Project Settings.


#include "TT_AssetManager.h"
#include "../TT_GameplayTags.h"

UTT_AssetManager& UTT_AssetManager::Get()
{
	TObjectPtr<UTT_AssetManager> TT_AssetManager = Cast<UTT_AssetManager>(GEngine->AssetManager);
	return *TT_AssetManager;
}

void UTT_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FGameplayTags::InitializeNativeGameplayTags();
}
