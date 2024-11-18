// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ResourceManager.generated.h"

class AResource;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UResourceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// BoilResource functions
	TArray<AResource*> GetBoilResources() const { return BoilResources; }
	void AddBoilResource(AResource* ResourceToAdd) { BoilResources.Add(ResourceToAdd); }
	void RemoveBoilResource(AResource* ResourceToRemove) { BoilResources.Remove(ResourceToRemove); }
	void SetBoilResources(const TArray<AResource*>& NewBoilResources) { BoilResources = NewBoilResources; }

	// FryResource functions
	TArray<AResource*> GetFryResources() const { return FryResources; }
	void AddFryResource(AResource* ResourceToAdd) { FryResources.Add(ResourceToAdd); }
	void RemoveFryResource(AResource* ResourceToRemove) { FryResources.Remove(ResourceToRemove); }
	void SetFryResources(const TArray<AResource*>& NewFryResources) { FryResources = NewFryResources; }

	
	void NotifyCanObtainFryResource();
	void NotifyCanObtainBoilResource();

	void Init();
protected:

private:
	TArray<AResource*> BoilResources;
	TArray<AResource*> FryResources;

};

