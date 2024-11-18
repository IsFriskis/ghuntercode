// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ChargeAbility.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "ChargeAimAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimCompleted, FExecuteChargeParams&, ChargeParams);

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UChargeAimAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UChargeAimAbilityTask(const FObjectInitializer& ObjectInitializer);
	virtual void TickTask(float DeltaTime) override;
	static TObjectPtr<UChargeAimAbilityTask> ExecuteAim(const FExecuteChargeParams& Params);

	UPROPERTY(BlueprintAssignable)
	FOnAimCompleted OnCompleted;

	FExecuteChargeParams Params;

};
