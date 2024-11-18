// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "BTT_ModifyAttribute.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_ModifyAttribute : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

	UBTT_ModifyAttribute(FObjectInitializer const &ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category ="TT|Parametros")
	FGameplayAttribute Attribute;
	UPROPERTY(EditAnywhere, Category ="TT|Parametros")
	float Value;
	UPROPERTY(EditAnywhere, Category ="TT|Parametros")
	FGameplayTag AddTag;
	UPROPERTY(EditAnywhere, Category ="TT|Parametros")
	FGameplayTag RemoveTag;
};
