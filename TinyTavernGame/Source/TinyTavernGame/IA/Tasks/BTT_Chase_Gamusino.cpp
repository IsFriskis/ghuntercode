// TinyTavern


#include "BTT_Chase_Gamusino.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/EnemyGamusino.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


class UNavigationSystemV1;

UBTT_Chase_Gamusino::UBTT_Chase_Gamusino(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = true;
}

void UBTT_Chase_Gamusino::MovementAICompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if(Result == EPathFollowingResult::Success)
	{
		FleeDirection = FVector::Zero();
		EnemyController->ReceiveMoveCompleted.RemoveAll(this);
		FinishExecute(true);
	}
}

EBTNodeResult::Type UBTT_Chase_Gamusino::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	EnemyController = EnemyBase->AIController;
	EnemyController->ReceiveMoveCompleted.AddDynamic(this ,&UBTT_Chase_Gamusino::MovementAICompleted);
	if(EnemyBase)
	{
		ASC =  EnemyBase->GetAbilitySystemComponent();
		const FGameplayEffectContextHandle Handle = ASC->MakeEffectContext();
		const FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GE_SpeedBoost, 1, Handle);
		ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Chase_Gamusino::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	const APlayerCharacter* Player =  Cast<APlayerCharacter>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject("playerReference"));
	const FVector EnemyLocation = EnemyBase->GetActorLocation();
	const FVector PlayerPosition = Player->GetActorLocation();
	const FVector Direction = PlayerPosition - EnemyLocation;
	const float PlayerDistance = Direction.Length();
	
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this->GetWorld());
	FNavLocation NavLocation;
	const FVector Extent = FVector(0.f, 0.0f, 0.0f);
	const bool isInNavMesh = NavSystem->ProjectPointToNavigation(PlayerPosition,NavLocation,Extent);

	const float TolerableDistance = FMath::RandRange(0.f,(MaxDistanceChase - MinDistanceChase)/2);
	//const float NewTolerableDistance = FMath::RandRange(MinDistanceChase,MaxDistanceChase);
	
	TArray<FHitResult> OutResults;
	GetWorld()->SweepMultiByChannel(OutResults,EnemyLocation,EnemyLocation + FVector(0.1f,0.1f,0.1f),FQuat::Identity,
		ECC_Visibility,FCollisionShape::MakeSphere(55));

	for(FHitResult hit : OutResults)
	{
		AEnemyGamusino* OtherGamusino = Cast<AEnemyGamusino>(hit.GetActor());
		if(OtherGamusino && OtherGamusino != EnemyBase)
		{
			FVector OtherGamusinoLocation = OtherGamusino->GetActorLocation();
			FVector DirectionToOtherGamusino =  EnemyLocation - OtherGamusinoLocation;
			DirectionToOtherGamusino.Normalize();
			FVector Impulse = DirectionToOtherGamusino * -20000;
			OtherGamusino->GetCharacterMovement()->AddImpulse(Impulse);
		}
	}
	
	if(!isFleeing && isInNavMesh)
	{
		EnemyBase->AIController->MoveToLocation(PlayerPosition);
	}

	// if(!isInNavMesh)
	// {
	// 	Enemy->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_Chase);
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 	
	// }

	if (PlayerDistance < MinDistanceChase)
	{
		DrawDebugSphere(GetWorld(),FleeDirection,50,12,FColor::Red,false,0.1f,0,1);
		bool isChaseInNavMesh = NavSystem->ProjectPointToNavigation(FleeDirection,NavLocation,Extent);
		
		if(FleeDirection == FVector::Zero() && isChaseInNavMesh)
		{
			FVector EnemyBackwardVector = EnemyBase->GetActorForwardVector() * -1;
			float RandomAngleOffset = FMath::RandRange(-75.f,75.f);
			FVector RotatedDirection = EnemyBackwardVector.RotateAngleAxis(RandomAngleOffset,FVector::UpVector).GetSafeNormal();
			FleeDirection = PlayerPosition + RotatedDirection * (MinDistanceChase + TolerableDistance);
			isFleeing = true;
		}
		if(!isChaseInNavMesh)
		{
			FleeDirection = FVector::Zero();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		EnemyBase->AIController->MoveToLocation(FleeDirection);
	}
	
	if (PlayerDistance < MaxDistanceChase && !isFleeing)
	{
		//EnemyBase->AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	if(PlayerDistance > MinDistanceChase && isFleeing)
	{
		isFleeing =false;
		//EnemyBase->AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_Chase_Gamusino::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	ASC->RemoveActiveGameplayEffect(ActiveHandle);
}
