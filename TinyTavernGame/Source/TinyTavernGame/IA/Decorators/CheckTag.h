// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CheckTag.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheckTag : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> TagsToCheck;
	
};
