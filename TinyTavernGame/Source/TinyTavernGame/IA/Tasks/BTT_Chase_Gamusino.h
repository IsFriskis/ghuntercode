// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayEffectTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "BTT_Chase_Gamusino.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_Chase_Gamusino : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_Chase_Gamusino(FObjectInitializer const &ObjectInitializer);

	UFUNCTION()
	void MovementAICompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Parametros")
	float MaxDistanceChase;

	UPROPERTY(EditAnywhere, Category = "Parametros")
	float MinDistanceChase;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> GE_SpeedBoost;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	
private:
	FActiveGameplayEffectHandle ActiveHandle;
	TObjectPtr<UAbilitySystemComponent> ASC;
	bool isFleeing = false;
	FVector FleeDirection = FVector::Zero();


	TObjectPtr<AEnemyBase> EnemyBase;
	TObjectPtr<AEnemyAAIController> EnemyController;
};
