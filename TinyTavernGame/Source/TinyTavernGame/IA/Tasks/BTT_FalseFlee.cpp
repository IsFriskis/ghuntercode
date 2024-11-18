// TinyTavern


#include "BTT_FalseFlee.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UBTT_FalseFlee::UBTT_FalseFlee(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "False Flee Behaviour";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_FalseFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyBase = OwnerComp.GetAIOwner()->GetPawn<AEnemyBase>();

	if (EnemyBase)
	{
		Player = Cast<APlayerCharacter>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("PlayerReference"));
		QueryRequest = FEnvQueryRequest(EQSQuery, EnemyBase.Get());
		QueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &UBTT_FalseFlee::EQSSearchFinished);
	}
	return  EBTNodeResult::InProgress;
}

void UBTT_FalseFlee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	EnemyBase.Get()->AIController->MoveToLocation(FinalFleeLocation, -1.0f, false, true,
		  true, true, 0, true);
	
	if(EnemyBase.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Vulnerable))
	{
		EnemyBase.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(BlockFalseFleeTag);
		EnemyBase.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(BlockTailSweepFalseFleeTag);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	if(FVector::Dist(Player->GetActorLocation(), EnemyBase->GetActorLocation()) < PlayerDetectionRange)
	{
		EnemyBase.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(BlockFalseFleeTag);
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	}
	
	if(FVector::Dist(EnemyBase->GetActorLocation(), FinalFleeLocation) < AcceptableRadiusReachDestination)
	{
		EnemyBase.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(BlockFalseFleeTag);
		EnemyBase.Get()->AIController->MoveToLocation(Player->GetActorLocation(), -1.0f, false, true,
		  true, true, 0, true);
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	}
}

void UBTT_FalseFlee::EQSSearchFinished(TSharedPtr<FEnvQueryResult>Result) 
{
	if(Result->IsSuccessful())
	{
		FinalFleeLocation = Result.Get()->GetItemAsLocation(0);    
	}
}


