// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "OnEnd_AddTag.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UOnEnd_AddTag : public UBTDecorator
{
	GENERATED_BODY()
public:
	UOnEnd_AddTag();
protected:
	virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true"))
	FGameplayTag Tag_To_Add;
};
