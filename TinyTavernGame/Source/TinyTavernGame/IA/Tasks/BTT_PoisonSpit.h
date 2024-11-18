// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_PoisonSpit.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_PoisonSpit : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UBTT_PoisonSpit(FObjectInitializer const& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void FinishBTTask(const FAbilityEndedData& AbilityEndedData);
};
