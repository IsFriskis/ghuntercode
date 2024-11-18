#include "BTT_Flee.h"

#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Math/UnrealMathUtility.h" // Include for FMath::RandRange
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyGroup.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/GamusinoGroup.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UBTT_Flee::UBTT_Flee(FObjectInitializer const& ObjectInitializer)
{
    NodeName = "Flee Behaviour";
    bNotifyTick = true;
}
EBTNodeResult::Type UBTT_Flee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //Gamusino specific beahviour
    EnemyBase = OwnerComp.GetAIOwner()->GetPawn<AEnemyBase>();
    stopMoving = false;
    if (EnemyBase)
    {
        AGamusinoGroup* GamusinoGroup = Cast<AGamusinoGroup>(EnemyBase->GetGroup());
        if(GamusinoGroup)
        {
            GamusinoGroup->NotifyIsAttacked();
        }

        //Get Player
        Player = Cast<APlayerCharacter>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("playerReference"));
        //Initialize EQS Query
        QueryRequest = FEnvQueryRequest(EQSQuery, EnemyBase.Get());
    }
    RunTimerLimit = FMath::RandRange(RunTimerMin,RunTimerMax);
    return  EBTNodeResult::InProgress;
}

void UBTT_Flee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if(EnemyBase.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Vulnerable))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    
    if(QueryTimer == 0.0f)
    {
        QueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &UBTT_Flee::EQSSearchFinished);
    }
    QueryTimer += DeltaSeconds;
    if(FVector::Dist(Player->GetActorLocation(), EnemyBase->GetActorLocation()) < PlayerDetectionRange)
    {
        RunTimer += DeltaSeconds;
    }
    else
    {
        RunTimer = 0;
    }
    
    if(QueryTimer > 0.5f)
    {
        QueryTimer = 0.0f;
    }
    if(RunTimer > RunTimerLimit)
    {
        stopMoving = true;
        RunTimer = -50.0f;
        EnemyBase->AIController->StopMovement();
        EnemyBase->GetAbilitySystemComponent()->AddLooseGameplayTag(TimerFleeTag);

        FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
    }

    if(!stopMoving)
    {
        EnemyBase.Get()->AIController->MoveToLocation(FinalFleeLocation, -1.0f, false, true,
            true, true, 0, true);
    }
    
    //If the enemy far away from player, end flee
    FVector EnemyPosition = EnemyBase.Get()->GetActorLocation();
    FVector PlayerPosition = Player.Get()->GetActorLocation();
    if(FVector::Dist(EnemyPosition, PlayerPosition) > MaxDistanceToPlayer)
    {
        EnemyBase.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(NextStateTag);
        FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
    }
    
}

void UBTT_Flee::EQSSearchFinished(TSharedPtr<FEnvQueryResult> Result)
{
    if(Result->IsSuccessful())
    {
        FinalFleeLocation = Result.Get()->GetItemAsLocation(0);    
    }
}


