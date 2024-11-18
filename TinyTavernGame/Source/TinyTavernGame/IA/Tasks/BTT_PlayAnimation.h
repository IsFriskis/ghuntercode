// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_PlayAnimation.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_PlayAnimationAbility : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UBTT_PlayAnimationAbility(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	void FinishBTTask(const FAbilityEndedData& AbilityEndedData);
private:
	UPROPERTY(EditAnywhere, Category = "Animation Task Info")
	bool ShouldAlwaysFacePlayer = true;
	UPROPERTY(EditAnywhere, Category = "Animation Task Info")
	FGameplayTag AbilityTag;
};
