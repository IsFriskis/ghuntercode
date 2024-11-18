// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TurnToAshesTask.generated.h"

class UBurnEnemy;
class UTurnToAshesAbility;

DECLARE_MULTICAST_DELEGATE(FOnFullMaterialBurnChange);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTurnToAshesTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UTurnToAshesTask(const FObjectInitializer& ObjectInitializer);
	
	TObjectPtr<UTurnToAshesTask> ExecuteTurnToAshesTask(UGameplayAbility* OwningAbility);

	virtual void TickTask(float DeltaTime) override;

	FOnFullMaterialBurnChange OnFullMaterialBurnChange;

	UPROPERTY()
	UBurnEnemy* TurnToAshesAbility;

	float BlendValue = 0.0f; // Example blend value, should be updated based on your logic
	bool bIsInitialized = false;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
};
