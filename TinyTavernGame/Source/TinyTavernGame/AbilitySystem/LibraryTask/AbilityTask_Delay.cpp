// TinyTavern


#include "AbilityTask_Delay.h"

UAbilityTask_Delay* UAbilityTask_Delay::DelayTask(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	float Time)
{
	UAbilityTask_Delay* DelayObj = NewAbilityTask<UAbilityTask_Delay>(OwningAbility, TaskInstanceName);

	DelayObj->Time = Time;

	return DelayObj;
}

UAbilityTask_Delay::UAbilityTask_Delay(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = false;
}



void UAbilityTask_Delay::Activate()
{
	Super::Activate();

		GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, // handle to cancel timer at a later time
		this, // the owning object
		&UAbilityTask_Delay::EndTime, // function to call on elapsed
		Time, // float delay until elapsed
		true,-1);
	}

void UAbilityTask_Delay::EndTime()
{
	OnCompleted.Broadcast();
}


