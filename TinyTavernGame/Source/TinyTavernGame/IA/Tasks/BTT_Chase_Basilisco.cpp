// TinyTavern


#include "BTT_Chase_Basilisco.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UBTT_Chase_Basilisco::UBTT_Chase_Basilisco(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Chase_Basilisco::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	return EBTNodeResult::InProgress;
}

void UBTT_Chase_Basilisco::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<AEnemyAAIController> EnemyAI = Enemy->AIController;
	APlayerCharacter* Player =  Cast<APlayerCharacter>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("playerReference"));

	FVector const PlayerPosition = Player->GetActorLocation();
	FVector const Direction = PlayerPosition - Enemy->GetActorLocation();
	float PlayerDistance = Direction.Length();
	Enemy->AIController->MoveToLocation(PlayerPosition);

	if (PlayerDistance < MaxDistanceChase)
	{
		EnemyAI->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	
}

void UBTT_Chase_Basilisco::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
