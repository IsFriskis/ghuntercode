// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_ErraticDodge.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_ErraticDodge : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:


	UBTT_ErraticDodge(FObjectInitializer const& ObjectInitializer);


	UAbilitySystemComponent* ASC;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void FinishBTTask(const FAbilityEndedData& AbilityEndedData);

	
};
