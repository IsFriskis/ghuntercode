// TinyTavern


#include "AbilityTask_OnCollision.h"

#include "Components/ShapeComponent.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UAbilityTask_OnCollision::UAbilityTask_OnCollision(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

void UAbilityTask_OnCollision::OnGameplayAbilityCancelled()
{
	
}

UAbilityTask_OnCollision* UAbilityTask_OnCollision::OnCollisionAbilityTask(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, FVector ImpactLocation, FVector ImpactOffset, float ImpactRadius, TArray<AActor*>IgnoreActors, float Timer, bool NeedTick)
{
	UAbilityTask_OnCollision* OnCollisionObj = NewAbilityTask<UAbilityTask_OnCollision>(OwningAbility, TaskInstanceName);
	OnCollisionObj->ImpactLocation = ImpactLocation;
	OnCollisionObj->ImpactOffset = ImpactOffset;
	OnCollisionObj->ImpactRadius = ImpactRadius;
	OnCollisionObj->IgnoreActors = IgnoreActors;
	OnCollisionObj->Timer = Timer;
	OnCollisionObj->NeedTick = NeedTick;
		
	return OnCollisionObj;
}

//Logica que hace la task cuando se activa 
void UAbilityTask_OnCollision::Activate()
{
	Super::Activate();
}

void UAbilityTask_OnCollision::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	OnSweepBegin();
	if(!NeedTick)
	{
		OnCompleted.Broadcast(NullActorBroadcast);
		EndTask();
	}
	else
	{
		TotalTime += DeltaTime;
		if(Timer <= TotalTime)
		{
			OnCompleted.Broadcast(NullActorBroadcast);
			EndTask();
		}
	}
}

void UAbilityTask_OnCollision::OnSweepBegin()
{
	TArray<FHitResult> OutHits;

	FCollisionShape ColSphere = FCollisionShape::MakeSphere(ImpactRadius);
	FVector SweepStart = ImpactLocation + ImpactOffset;
	FVector SweepEnd = SweepStart;
	
	FCollisionQueryParams QueryParams;
	IgnoreActors.Add(Ability.Get()->GetActorInfo().OwnerActor.Get());
	QueryParams.AddIgnoredActors(IgnoreActors);
	
	

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, ColSphere, QueryParams);
	
	for (auto& Hit : OutHits)
	{
			OnHit.Broadcast(Hit.GetActor());
	}
}





