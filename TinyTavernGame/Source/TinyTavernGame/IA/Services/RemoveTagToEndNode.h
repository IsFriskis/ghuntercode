// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RemoveTagToEndNode.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API URemoveTagToEndNode : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	TObjectPtr<AEnemyBase> Enemy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag Tag;
};
