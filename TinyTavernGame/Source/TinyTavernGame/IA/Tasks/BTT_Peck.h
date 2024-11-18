// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_Peck.generated.h"

struct FAbilityEndedData;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_Peck : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	UBTT_Peck(FObjectInitializer const& ObjectInitializer);
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UFUNCTION()
	void FinishBTTask(const FAbilityEndedData& AbilityEndedData);
};
