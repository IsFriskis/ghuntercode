// TinyTavern


#include "AbilityTask_OnCollision_Socket.h"

#include "GameFramework/Character.h"

UAbilityTask_OnCollision_Socket::UAbilityTask_OnCollision_Socket(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

void UAbilityTask_OnCollision_Socket::OnGameplayAbilityCancelled()
{
	
}

UAbilityTask_OnCollision_Socket* UAbilityTask_OnCollision_Socket::OnCollisionSocketAbilityTask(
	UGameplayAbility* OwningAbility, FName TaskInstanceName,float ImpactRadius,bool DrawDebug, TArray<AActor*> IgnoreActors,
	float Timer, bool NeedTick,USkeletalMeshComponent* MeshComponent, FName StartSocketName, FName EndSocketName)
{
	UAbilityTask_OnCollision_Socket* OnCollisionSocketObj = NewAbilityTask<UAbilityTask_OnCollision_Socket>(OwningAbility, TaskInstanceName);
	OnCollisionSocketObj->ImpactRadius = ImpactRadius;
	OnCollisionSocketObj->DrawDebug = DrawDebug;
	OnCollisionSocketObj->IgnoreActors = IgnoreActors;
	OnCollisionSocketObj->Timer = Timer;
	OnCollisionSocketObj->NeedTick = NeedTick;
	OnCollisionSocketObj->MeshComponent = MeshComponent;
	OnCollisionSocketObj->StartSocketName = StartSocketName;
	OnCollisionSocketObj->EndSocketName = EndSocketName;
	
	return OnCollisionSocketObj;
}

void UAbilityTask_OnCollision_Socket::Activate()
{
	Super::Activate();
}

void UAbilityTask_OnCollision_Socket::TickTask(float DeltaTime)
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

void UAbilityTask_OnCollision_Socket::OnSweepBegin()
{
	TArray<FHitResult> OutHits;
	
	FCollisionShape ColSphere = FCollisionShape::MakeSphere(ImpactRadius);

	FVector SweepStart = MeshComponent->GetSocketLocation(StartSocketName);;
	FVector SweepEnd = MeshComponent->GetSocketLocation(EndSocketName);
	
	FCollisionQueryParams QueryParams;
	IgnoreActors.Add(Ability.Get()->GetActorInfo().OwnerActor.Get());
	QueryParams.AddIgnoredActors(IgnoreActors);

	if (DrawDebug)
	{
		FVector CapsuleCenter =FVector((SweepStart.X+SweepEnd.X)/2,(SweepStart.Y+SweepEnd.Y)/2,(SweepStart.Z+SweepEnd.Z)/2); 
		float Halfcapsule = ((SweepStart-SweepEnd)/2).Size();
		auto CapsuleRotation = MeshComponent->GetSocketLocation(StartSocketName).Rotation().Quaternion();
		DrawDebugCapsule(GetWorld(),CapsuleCenter,Halfcapsule,ImpactRadius,CapsuleRotation,FColor::Red,false,0.2,0,1);
	}
	
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, ColSphere, QueryParams);
	if(isHit)
	{
		for (auto& Hit : OutHits)
		{
				OnHit.Broadcast(Hit.GetActor());
		}
	}
}
