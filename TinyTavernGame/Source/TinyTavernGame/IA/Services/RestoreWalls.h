// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RestoreWalls.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API URestoreWalls : public UBTService
{
	GENERATED_BODY()
public:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	float OccultWallTimer = 0.0f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT|WallParameters")
	float WallLifeSpan;
};
