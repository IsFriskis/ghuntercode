// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_WaveAttack.generated.h"

struct FAbilityEndedData;
class AEnemyBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_WaveAttack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_WaveAttack(FObjectInitializer const &ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void EndTask(const FAbilityEndedData& Data);
	
private:
	
	UPROPERTY()
	TObjectPtr<AEnemyBase> Enemy;

	UFUNCTION()
	void ActivateAbility();

	UPROPERTY(EditAnywhere)
	float JumpTimer;
	
	FTimerHandle TimerHandle;
	bool CanWave = true;
};
