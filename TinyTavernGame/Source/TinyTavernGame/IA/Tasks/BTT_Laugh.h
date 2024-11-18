// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_Laugh.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_Laugh : public UBTTask_BlueprintBase
{
public:
	UBTT_Laugh(FObjectInitializer const& ObjectInitializer);

private:
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true", Tooltip= "Minimum distance to laugh from the player."))
	float MinDistanceToFleeFrom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behaviour Params", meta = (AllowPrivateAccess = "true", Tooltip= "Maximum distance to laugh from the player."))
	float MaxDistanceToFleeFrom;
};
