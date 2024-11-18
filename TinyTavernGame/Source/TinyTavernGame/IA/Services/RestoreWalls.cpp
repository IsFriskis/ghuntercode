// TinyTavern


#include "RestoreWalls.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tasks/Task.h"
#include "TinyTavernGame/Actor/Hazards/GuardianWall.h"
#include "TinyTavernGame/Character/Enemy/Cucafera/CucaferaGroup.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void URestoreWalls::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	ACucaferaGroup* CucaferaGroup = Cast<ACucaferaGroup>(enemy->GetGroup());
	if(enemy && enemy->AIController->GetBlackboardComponent()->GetValueAsBool("HasUsedWall") != false)
	{
		OccultWallTimer += DeltaSeconds;
		if(OccultWallTimer >= WallLifeSpan)
		{
			OccultWallTimer = 0;
			for (AActor* Wall : CucaferaGroup->GuardianWallsArray)
			{
				AGuardianWall* GuardianWall = Cast<AGuardianWall>(Wall);
				if(GuardianWall)
				{
					GuardianWall->HideWallsAgain(7);
				}
			}
		}
	}
}
