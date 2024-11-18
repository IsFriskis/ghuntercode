// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_Chase_Basilisco.generated.h"

class AEnemyBase;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_Chase_Basilisco : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_Chase_Basilisco(FObjectInitializer const &ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Parametros")
	float MaxDistanceChase;

	TObjectPtr<AEnemyBase> Enemy;

};
