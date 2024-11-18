// TinyTavern


#include "UAbilityTask_AimToLocationAndWait.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Engine/SceneCapture.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "TinyTavernGame/AbilitySystem/Abilities/Enemies/ChargeTask.h"


UAbilityTask_AimToLocationAndWait::UAbilityTask_AimToLocationAndWait(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}


UAbilityTask_AimToLocationAndWait* UAbilityTask_AimToLocationAndWait::AbilityTask_AimToLocationAndWait(UGameplayAbility* OwningAbility,
                                                                    FName TaskInstanceName, float AimTime,float Distance)
{
	UAbilityTask_AimToLocationAndWait* AimToLocation = NewAbilityTask<UAbilityTask_AimToLocationAndWait>(OwningAbility, TaskInstanceName);
	
	AimToLocation->AimTime = AimTime;
	AimToLocation->Distance = Distance;


	return AimToLocation;
}
void UAbilityTask_AimToLocationAndWait::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	// if (OrientToTarget)
	// {
	// 	if (AimTime > 0)
	// 	{
	// 		// AimTime -= DeltaTime;
	// 		// FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Charger->GetActorLocation(),Location);
	// 		// FRotator NewRot = FMath::RInterpTo(Charger->GetActorRotation(), PlayerRot, DeltaTime, 2);
	// 		// Charger->SetActorRotation(NewRot);	
	// 	}
	// 	else
	// 	{
	// 		AimToLocation();
	// 	}
	// }
	// else
	// {
	// 	AimToLocation();
	// }
	AimToLocation();
}

void UAbilityTask_AimToLocationAndWait::OnGameplayAbilityCancelled()
{
	EndTask();
}

void UAbilityTask_AimToLocationAndWait::Activate()
{
	Super::Activate();
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	Charger = ASC->GetOwnerActor();
	
}

void UAbilityTask_AimToLocationAndWait::AimToLocation()
{
	
	FVector EnemyLocation = Charger->GetActorLocation();
	FVector ForwardDirection = Charger->GetActorForwardVector();
	FVector TargetDirection;
	int32 Result =  FMath::RandRange(1,3);
	//Right
	if(Result == 1)
	{
		
		float angle = PI/4;
		float cosAngle = FMath::Cos(angle);
		float sinAngle = FMath::Sin(angle);
		TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
			ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
			ForwardDirection.Z); 
		
		
	}
	//Left
	else if(Result == 2)
	{
	
		float angle = -PI/4;
		float cosAngle = FMath::Cos(angle);
		float sinAngle = FMath::Sin(angle);
		TargetDirection = FVector(ForwardDirection.X * cosAngle - ForwardDirection.Y * sinAngle,
			ForwardDirection.X * sinAngle + ForwardDirection.Y * cosAngle,
			ForwardDirection.Z);
		
		
	}
	//Forward
	else
	{

		TargetDirection = ForwardDirection;
	}
	
	FVector ProjectedLocation = EnemyLocation + (TargetDirection * Distance) ;

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this->GetWorld());
	FNavLocation NavLocation;
	FVector Extent = FVector(0.f, 0.0f, 0.0f);
	bool isInNavMesh = NavSystem->ProjectPointToNavigation(ProjectedLocation,NavLocation,Extent);
	if(isInNavMesh)
	{
		OnCompleted.Broadcast(ProjectedLocation);
		EndTask();
	}
	if(!isInNavMesh)
	{
		
		OnInterrupted.Broadcast(ProjectedLocation);
		EndTask();
	}
}