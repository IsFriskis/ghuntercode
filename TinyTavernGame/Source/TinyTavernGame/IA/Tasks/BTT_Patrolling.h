// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/IA/MIscelaneous/Waypoint.h"
#include "BTT_Patrolling.generated.h"

/**
 * 
 */

UCLASS()
class TINYTAVERNGAME_API UBTT_Patrolling : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	UBTT_Patrolling();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	TObjectPtr<AEnemyBase> EnemyBase;
	TObjectPtr<AEnemyAAIController> EnemyAaiController;
	TObjectPtr<AWaypoint>CurrentWaypoint;

	TArray<AWaypoint*> WaypointsVisited;
	TArray<AWaypoint*> MapKeys;
};
