// TinyTavern


#include "BTT_Laugh.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"

UBTT_Laugh::UBTT_Laugh(FObjectInitializer const& ObjectInitializer)
{
    NodeName = "Laugh Behaviour";
}

EBTNodeResult::Type UBTT_Laugh::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI controller and blackboard component
    AEnemyAAIController* AIController = Cast<AEnemyAAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }
    
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    // Get the enemy pawn and player character
    APawn* EnemyPawn = AIController->GetPawn();
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!EnemyPawn || !PlayerCharacter)
    {
        return EBTNodeResult::Failed;
    }

    // Calculate the direction towards the player
    FVector PlayerPosition = PlayerCharacter->GetActorLocation();
    FVector EnemyPosition = EnemyPawn->GetActorLocation();
    FVector ChaseDirection = PlayerPosition - EnemyPosition;
    ChaseDirection.Normalize();

    // Set the enemy's rotation to face towards the player
    FRotator NewRotation = FRotationMatrix::MakeFromX(ChaseDirection).Rotator();
    EnemyPawn->SetActorRotation(NewRotation);

    // Calculate the desired distance between the enemy and the player
    float DesiredDistance = FMath::RandRange(MinDistanceToFleeFrom, MaxDistanceToFleeFrom); // Convert from cm to meters

    // Calculate the desired position to maintain the desired distance from the player
    FVector DesiredPosition = PlayerPosition - ChaseDirection * DesiredDistance;

    // Move the enemy towards the desired position
    AIController->MoveToLocation(DesiredPosition);

    return EBTNodeResult::Succeeded;
}
