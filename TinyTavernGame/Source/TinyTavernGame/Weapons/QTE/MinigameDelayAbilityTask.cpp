// TinyTavern


#include "MinigameDelayAbilityTask.h"

UMinigameDelayAbilityTask::UMinigameDelayAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UMinigameDelayAbilityTask> UMinigameDelayAbilityTask::CreateMinigameDelayTask(UGameplayAbility* Owner, float Delay)
{
	TObjectPtr<UMinigameDelayAbilityTask> MinigameDelayAbilityTask = NewAbilityTask<UMinigameDelayAbilityTask>(Owner);
	MinigameDelayAbilityTask->TimeLeft = Delay;
	MinigameDelayAbilityTask->WorldTimeDilation = MinigameDelayAbilityTask->GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();
	MinigameDelayAbilityTask->PlayerTimeDilation = MinigameDelayAbilityTask->GetOwnerActor()->CustomTimeDilation;

	return MinigameDelayAbilityTask;
}

void UMinigameDelayAbilityTask::MarkForDeath()
{
	MarkedForDeath = true;
}

void UMinigameDelayAbilityTask::TickTask(float DeltaTime)
{
	if (MarkedForDeath)
	{
		EndTask();
		return;
	}

	if (TimeLeft <= 0)
	{
		OnTimerCompleted.Broadcast();	
	}

	float FinalTime = DeltaTime / (WorldTimeDilation * PlayerTimeDilation);
	TimeLeft -= FinalTime;
}
