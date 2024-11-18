// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BehaviorTree/BTService.h"
#include "CheckAttributeAndFinishNode.generated.h"

struct FTagsToValue;
class UAbilitySystemComponent;
class AEnemyBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheckAttributeAndFinishNode : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	// UPROPERTY(BlueprintReadOnly, EditAnywhere)
	// TArray<FTagsToValue> TagsAndValues;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayAttribute MaxAttribute;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayAttribute Attribute;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	float Percentage;

	
};
