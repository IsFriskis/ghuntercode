// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IA_EnemyManager.generated.h"

class AEnemyGroup;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UIA_EnemyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;

	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues InitValues);

	//Getters, setters, deleters
	TArray<AEnemyGroup*> GetEnemyGroups() const { return EnemyGroups; }
	void AddEnemyGroup(AEnemyGroup* EnemyGroupToAdd) {EnemyGroups.Push(EnemyGroupToAdd);}
	void DeleteEnemyGroup(AEnemyGroup* EnemyGroupToDelete) {EnemyGroups.Remove(EnemyGroupToDelete);}

private:
	TArray<AEnemyGroup*> EnemyGroups;
	
};
