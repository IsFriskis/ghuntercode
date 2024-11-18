// TinyTavern


#include "IA_EnemyManager.h"

void UIA_EnemyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UIA_EnemyManager::OnPostWorldInitialization);
}

void UIA_EnemyManager::Deinitialize()
{
	Super::Deinitialize();
}

void UIA_EnemyManager::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues InitValues)
{
	//TODO: Empty method till needs use
}
