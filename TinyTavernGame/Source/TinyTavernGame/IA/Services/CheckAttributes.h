// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BehaviorTree/BTService.h"
#include "CheckAttributes.generated.h"

class AEnemyBase;

/*USTRUCT(BlueprintType)
struct FTagsToValue
{
	GENERATED_BODY()
	//Tag to add
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag Tag;
	//Value (%) needed to add the Tag
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Value = 0;
};*/
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheckAttributes : public UBTService
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/*UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FTagsToValue> TagsAndValues;*/

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttributeKey;
	
};
