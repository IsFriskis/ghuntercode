// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_RollDice.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_RollDice : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_RollDice(FObjectInitializer const &ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category ="Parametros")
	float TruePercentage;
	UPROPERTY(EditAnywhere,Category ="Parametros")
	FGameplayTag TrueTag;
	UPROPERTY(EditAnywhere,Category ="Parametros")
	FGameplayTag FalseTag;
};
