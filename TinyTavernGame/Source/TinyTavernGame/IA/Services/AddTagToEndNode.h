// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AddTagToEndNode.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UAddTagToEndNode : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	TObjectPtr<AEnemyBase> Enemy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool RemoveTag;
	

};
