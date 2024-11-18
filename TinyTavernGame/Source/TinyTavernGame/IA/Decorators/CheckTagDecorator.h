// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CheckTagDecorator.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheckTagDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCheckTagDecorator();
protected:
	virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true"),meta=(EditCondition="SingleTagToCheck==false"))
	TArray<FGameplayTag> TagsToCheck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true"),meta=(EditCondition="SingleTagToCheck==true"))
	FGameplayTag TagToCheck;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true"))
	bool SingleTagToCheck;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true"),meta=(EditCondition="SingleTagToCheck==false"))
	bool OrCheckTag;
};
