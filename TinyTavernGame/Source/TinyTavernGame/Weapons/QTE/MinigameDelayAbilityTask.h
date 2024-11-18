// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MinigameDelayAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDelayTimerComplete);

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMinigameDelayAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UMinigameDelayAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<UMinigameDelayAbilityTask> CreateMinigameDelayTask(UGameplayAbility* Owner, float Delay);
	void MarkForDeath();

	FOnDelayTimerComplete OnTimerCompleted;
	

protected:
	virtual void TickTask(float DeltaTime) override;
	


private:
	float TimeLeft;
	float WorldTimeDilation;
	float PlayerTimeDilation;
	bool MarkedForDeath;
	
};
