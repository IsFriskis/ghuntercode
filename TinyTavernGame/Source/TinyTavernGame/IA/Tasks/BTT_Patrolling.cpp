// TinyTavern


#include "BTT_Patrolling.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyGroup.h"

UBTT_Patrolling::UBTT_Patrolling()
{
	NodeName = TEXT("Patrolling Behaviour");
}

EBTNodeResult::Type UBTT_Patrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	EnemyAaiController = EnemyBase->AIController = Cast<AEnemyAAIController>(OwnerComp.GetAIOwner());
	if(EnemyBase)
	{
		for (auto Element : EnemyBase->GetGroup()->PatrolWaypointsMap)
		{
			EnemyBase->GetGroup()->PatrolWaypointsMap.GetKeys(MapKeys);
			if(WaypointsVisited.Num() == MapKeys.Num())
			{
				EnemyBase->GetGroup()->PatrolWaypointsMap.Emplace(CurrentWaypoint,nullptr);
				WaypointsVisited.Reset();
			}
			AWaypoint* Waypoint = Element.Key;
			AEnemyBase* EnemyPointer = EnemyBase->GetGroup()->PatrolWaypointsMap.FindRef(Waypoint);
			if(EnemyPointer == nullptr && !WaypointsVisited.Contains(Waypoint))
			{
				EnemyBase->GetGroup()->PatrolWaypointsMap.Add(Waypoint,EnemyBase);
				CurrentWaypoint = Waypoint;
				WaypointsVisited.Add(CurrentWaypoint);
				break;
			}
			if(WaypointsVisited.Contains(Waypoint))
			{
				EnemyBase->GetGroup()->PatrolWaypointsMap.Emplace(CurrentWaypoint,nullptr);
				AWaypoint* const* WaypointPP = EnemyBase->GetGroup()->PatrolWaypointsMap.FindKey(nullptr);
				if(WaypointPP != nullptr && !WaypointsVisited.Contains(Waypoint))
				{
					AWaypoint* WaypointPA = *WaypointPP;
					CurrentWaypoint = WaypointPA;
					EnemyBase->GetGroup()->PatrolWaypointsMap.Emplace(WaypointPA,EnemyBase);
					break;
				}
				FinishExecute(true);
			}
		}
		if(EnemyAaiController)
		{
			EnemyBase->AIController->GetBlackboardComponent()->SetValueAsVector(FName("NextWaypoint"),CurrentWaypoint->GetActorLocation());
			FinishExecute(true);
		}
	}
	return EBTNodeResult::InProgress;
}


