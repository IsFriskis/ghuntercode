// TinyTavern


#include "ChargeAimAbilityTask.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UChargeAimAbilityTask::UChargeAimAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

void UChargeAimAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	//Init vars that are required to be accesible multiple times
	FVector GnomePos = Params.TargetActor->GetActorLocation();
	FVector EnemyPos = GetOwnerActor()->GetActorLocation();
	FVector Direction = GnomePos - EnemyPos;
	Direction.Normalize();

	//Charge time calculation (Just rotates the character towards player)
	GetOwnerActor()->SetActorRotation(Direction.Rotation());
	Params.AimTime -= DeltaTime;

	if (Params.AimTime <= 0.0f)
	{
		Params.TargetLocation = EnemyPos + Direction * Params.Distance;
		OnCompleted.Broadcast(Params);
		EndTask();
	}
}

TObjectPtr<UChargeAimAbilityTask> UChargeAimAbilityTask::ExecuteAim(const FExecuteChargeParams& Params)
{
	TObjectPtr<UChargeAimAbilityTask> AimTask = NewAbilityTask<UChargeAimAbilityTask>(Params.OwningAbility);
	AimTask->Params = Params;

	return AimTask;
}
