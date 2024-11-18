// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TT_AssetManager.generated.h"

/**
* This class is added to the DefaultEngine.Ini config file in the [/Script/Engine.Engine] section
	with the format AssetManagerClassName=/Script/TinyTavernGame.TT_AssetManager.
	
* Its function is to establish the objects to load before starting the game loading
 */
UCLASS()
class TINYTAVERNGAME_API UTT_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UTT_AssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
	
};
