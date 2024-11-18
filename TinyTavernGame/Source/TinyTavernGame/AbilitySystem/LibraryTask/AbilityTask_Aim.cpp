// TinyTavern


#include "AbilityTask_Aim.h"

#include "AbilitySystemComponent.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TinyTavernGame/AbilitySystem/Abilities/Enemies/ChargeTask.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UAbilityTask_Aim::UAbilityTask_Aim(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}


UAbilityTask_Aim* UAbilityTask_Aim::AimAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	AActor* Target, float AimTime,float HeightOffset,float RotationSpeed,bool OrientToTarget,bool TargetDistance,float Distance, float Tolerance)
{
	UAbilityTask_Aim* AimObj = NewAbilityTask<UAbilityTask_Aim>(OwningAbility, TaskInstanceName);
	AimObj->Target = Target;
	AimObj->AimTime = AimTime;
	AimObj->RotationSpeed = RotationSpeed;
	AimObj->OrientToTarget = OrientToTarget;
	AimObj->TargetDistance = TargetDistance;
	AimObj->Distance = Distance;
	AimObj->HeightOffset = HeightOffset;
	AimObj->Tolerance = Tolerance;

	return AimObj;
}

void UAbilityTask_Aim::Activate()
{
	Super::Activate();
	
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	Charger = ASC->GetOwnerActor();
	
}


void UAbilityTask_Aim::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (OrientToTarget)
	{
		if (AimTime > 0)
		{
			AimTime -= DeltaTime;
			FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Charger->GetActorLocation(),Target->GetActorLocation());
			FRotator NewRot = FMath::RInterpTo(Charger->GetActorRotation(), PlayerRot, DeltaTime, RotationSpeed);
		
			NewRot.Pitch = HeightOffset;
			Charger->SetActorRotation(NewRot);	
		}
		else
		{
			Aim();
		}
	}
	else
	{
		Aim();
	}
}

void UAbilityTask_Aim::Aim()
{
	FVector InitialChargerPosition = Charger->GetActorLocation();
	FVector InitialTargetPosition = Target->GetActorLocation();

	if(TargetDistance)
	{
		Distance = FVector::Dist(InitialChargerPosition,InitialTargetPosition);
	}

	FVector Direction = InitialTargetPosition - InitialChargerPosition;
	Direction = Direction.GetSafeNormal();
	
	FVector EndPoint = InitialChargerPosition + Direction * (Distance + Tolerance); 

	OnCompleted.Broadcast(EndPoint);
	EndTask();
}


void UAbilityTask_Aim::OnGameplayAbilityCancelled()
{
}
