// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DetectPlayerFromWall.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UDetectPlayerFromWall : public UBTService
{
	GENERATED_BODY()
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT|DetectPlayerFromWallParameters")
	float MinDistanceFromWall;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT|DetectPlayerFromWallParameters")
	float MaxDistanceFromWall;
};
