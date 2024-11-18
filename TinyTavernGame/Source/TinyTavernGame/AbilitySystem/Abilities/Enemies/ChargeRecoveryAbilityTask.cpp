// TinyTavern


#include "ChargeRecoveryAbilityTask.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UChargeRecoveryAbilityTask::UChargeRecoveryAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

void UChargeRecoveryAbilityTask::TickTask(float DeltaTime)
{
	RecoveryTime -= DeltaTime;

	if (RecoveryTime <= 0.0f)
	{
		OnCompleted.Broadcast();
		EndTask();
	}
}

TObjectPtr<UChargeRecoveryAbilityTask> UChargeRecoveryAbilityTask::ExecuteRecovery(const FExecuteChargeParams& Params)
{
	TObjectPtr<UChargeRecoveryAbilityTask> RecoveryTask = NewAbilityTask<UChargeRecoveryAbilityTask>(Params.OwningAbility);

	RecoveryTask->RecoveryTime = Params.RecoveryTime;

	return RecoveryTask;
}
