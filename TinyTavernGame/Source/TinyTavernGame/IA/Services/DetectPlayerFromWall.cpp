// TinyTavern


#include "DetectPlayerFromWall.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Character/Enemy/Cucafera/CucaferaGroup.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void UDetectPlayerFromWall::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
	if(IsValid(Player) && IsValid(enemy))
	{	
		ACucaferaGroup* CucaferaGroup = Cast<ACucaferaGroup>(enemy->GetGroup());
		if(CucaferaGroup && !CucaferaGroup->GuardianWallsArray.IsEmpty())
		{
			for (AGuardianWall* Wall : CucaferaGroup->GuardianWallsArray)
			{
				float distanceBetweenPlayerAndWall = FVector::Dist(Player->GetActorLocation(), Wall->GetActorLocation());
				if(distanceBetweenPlayerAndWall >= MinDistanceFromWall && distanceBetweenPlayerAndWall <= MaxDistanceFromWall)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("PlayerIsNearWall",true);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("NearWall",Wall);
					enemy->AIController->BehaviorTreeComponent->RestartTree(EBTRestartMode::ForceReevaluateRootNode);
					return;
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("PlayerIsNearWall",false);
				}
			}
		}
	}
}
