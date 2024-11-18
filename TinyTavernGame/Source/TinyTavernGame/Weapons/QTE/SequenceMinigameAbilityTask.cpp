// TinyTavern


#include "SequenceMinigameAbilityTask.h"
#include "../../Player/TTPlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "../../TinyTavernUtils.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"

USequenceMinigameAbilityTask::USequenceMinigameAbilityTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<USequenceMinigameAbilityTask> USequenceMinigameAbilityTask::CreateMinigameTask(FSequenceMinigameParams Params)
{
	TObjectPtr<USequenceMinigameAbilityTask> SequenceMinigameAbilityTask = NewAbilityTask<USequenceMinigameAbilityTask>(Params.OwningAbility);
	
	SequenceMinigameAbilityTask->OwningAbility = Params.OwningAbility;
	SequenceMinigameAbilityTask->DelayTimeBetweenInputs = Params.DelayTimeBetweenInputs;
	SequenceMinigameAbilityTask->TimeWindowPerInput = Params.TimeWindowPerInput;
	SequenceMinigameAbilityTask->QTEWidget = Params.WidgetInstance;

	UWorld* World = SequenceMinigameAbilityTask->GetWorld();

	if (ATTPlayerController* playerController = Cast<ATTPlayerController>(World->GetFirstPlayerController()))
	{
		playerController->OnQTECleaver.AddDynamic(SequenceMinigameAbilityTask, &USequenceMinigameAbilityTask::OnPressedY);
		playerController->OnQTETenderizer.AddDynamic(SequenceMinigameAbilityTask, &USequenceMinigameAbilityTask::OnPressedX);
		playerController->OnQTESkewer.AddDynamic(SequenceMinigameAbilityTask, &USequenceMinigameAbilityTask::OnPressedB);
	}

	SequenceMinigameAbilityTask->OnButtonPressed.AddDynamic(SequenceMinigameAbilityTask, &USequenceMinigameAbilityTask::OnInputPressed);

	SequenceMinigameAbilityTask->ResetInputTimer();
	SequenceMinigameAbilityTask->PlayerTimeDilation = SequenceMinigameAbilityTask->GetOwnerActor()->CustomTimeDilation;;
	SequenceMinigameAbilityTask->WorldTimeDilation = SequenceMinigameAbilityTask->GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();

	return SequenceMinigameAbilityTask;
}

void USequenceMinigameAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	if (MarkedEndTask)
	{
		EndTask();
		return;
	}

	if (!ButtonPressed && RemainingTimeForInput <= 0)
	{
		ButtonPressed = true;
		OnTimerFinished.Broadcast();
		SetDelayForNextInput();
	}
	else
	{
		if (!ButtonPressed)
		{
			RemainingTimeForInput -= DeltaTime / (WorldTimeDilation * PlayerTimeDilation);
		}
		
	}

	QTEWidget->UpdateSliceBarTimer(RemainingTimeForInput / TimeWindowPerInput);
}

void USequenceMinigameAbilityTask::OnInputPressed(EButtonToPress Button)
{
	ButtonPressed = true;
	SetDelayForNextInput();
}

void USequenceMinigameAbilityTask::ResetInputTimer()
{
	// Reset variables
	ButtonPressed = false;
	RemainingTimeForInput = TimeWindowPerInput;
}

void USequenceMinigameAbilityTask::SetDelayForNextInput()
{
	// Call function after delay
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USequenceMinigameAbilityTask::ResetInputTimer, DelayTimeBetweenInputs * WorldTimeDilation, false);
}


