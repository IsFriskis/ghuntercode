// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ChargeAbility.h"
#include "ChargeRecoveryAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecoveryCompleted);

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UChargeRecoveryAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UChargeRecoveryAbilityTask(const FObjectInitializer& ObjectInitializer);
	virtual void TickTask(float DeltaTime) override;
	static TObjectPtr<UChargeRecoveryAbilityTask> ExecuteRecovery(const FExecuteChargeParams& Params);

	UPROPERTY(BlueprintAssignable)
	FOnRecoveryCompleted OnCompleted;

	float RecoveryTime;
};
