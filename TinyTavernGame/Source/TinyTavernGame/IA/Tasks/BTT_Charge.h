// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_Charge.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_Charge : public UBTTask_BlueprintBase
{
public:
	UBTT_Charge(FObjectInitializer const &ObjectInitializer);

private:
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void EndTask(const FAbilityEndedData& Data);
	
private:
	
	UPROPERTY()
	TObjectPtr<AEnemyBase> Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true", Tooltip= "Time enemy will charge before attacking"))
	float ChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true", Tooltip= "Percentage of movement speed gained in charge"))
	int SpeedBoostPercentage;
	
};
