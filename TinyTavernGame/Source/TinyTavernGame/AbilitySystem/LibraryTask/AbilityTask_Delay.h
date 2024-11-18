// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Delay.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityTaskDelayDelegate);

UCLASS()
class TINYTAVERNGAME_API UAbilityTask_Delay : public UAbilityTask
{
	GENERATED_BODY()

	
public:
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskDelayDelegate	OnCompleted;

	UAbilityTask_Delay(const FObjectInitializer& ObjectInitializer);
	/** Callback function for when the owning Gameplay Ability is cancelled */

	/** 
	 * Delay function
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param Time Amount time to delay.

 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="DelayAndWait",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_Delay* DelayTask(UGameplayAbility* OwningAbility,
		FName TaskInstanceName,float Time);

	virtual void Activate() override;

	void EndTime();

	float Time;
	FTimerHandle TimerHandle;
};
