// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_IdleWander.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyGroup.h"

class AEnemyAAIController;

UBTT_IdleWander::UBTT_IdleWander(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Idle Wander Behaviour";
	bNotifyTick = true;
	
}

EBTNodeResult::Type UBTT_IdleWander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Enemy = OwnerComp.GetAIOwner()->GetPawn<AEnemyBase>();
	Enemy->AIController.Get()->ReceiveMoveCompleted.AddUniqueDynamic(this,&UBTT_IdleWander::OnFinishedMove);
	return EBTNodeResult::InProgress;
}

void UBTT_IdleWander::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AlertBreak)
	{
		if(Enemy.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Alert) || Enemy.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_IA_Investigating))
		{
			Enemy->GetCharacterMovement()->StopMovementImmediately();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	
	if(IsWaiting)
	{
		TimeInWait += DeltaSeconds;
		if(TimeInWait >= WaitDuration)
		{
			GenerateNewTargetLocation();
			IsWaiting = false;
			TimeInWait = 0.0f;
			
		}
	}
	else
	{
		Enemy.Get()->AIController.Get()->MoveToLocation(TargetLocation.Location);
	}
}

void UBTT_IdleWander::OnFinishedMove(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if(Result == EPathFollowingResult::Success)
	{
		IsWaiting = true;
		FinishExecute(true);
	}
}

void UBTT_IdleWander::GenerateNewTargetLocation()
{
	FVector Origin;
	// Obtain npc location to use as an origin
	if(Enemy->AIController->GetBlackboardComponent()->GetValueAsBool("WanderFromGroup"))
	{
		Origin = Enemy->GetGroup()->GetActorLocation();
	}
	else
	{
		Origin = Enemy.Get()->GetActorLocation();
	}
	// Get the navigation system and generate a random location
	if(const UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		NavSys->GetRandomPointInNavigableRadius(Origin, WanderRadius, TargetLocation);
	}
}
