// TinyTavern


#include "TimingMinigameAbilityTask.h"
#include "TimingMinigameAbility.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"


UTimingMinigameAbilityTask::UTimingMinigameAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UTimingMinigameAbilityTask> UTimingMinigameAbilityTask::CreateMinigameTask(FTimingMinigameParams Params)
{
	TObjectPtr<UTimingMinigameAbilityTask> TimingMinigameAbilityTask = NewAbilityTask<UTimingMinigameAbilityTask>(Params.OwningAbility);
	TimingMinigameAbilityTask->QTEWidget = Params.WidgetInstance;
	TimingMinigameAbilityTask->OwningAbility = Params.OwningAbility;
	TimingMinigameAbilityTask->TimeForFullCycle = Params.TimeForFullCycle;
	TimingMinigameAbilityTask->EarliestPossibleTimingNormalized = Params.EarliestPossibleTiming / Params.TimeForFullCycle;
	TimingMinigameAbilityTask->TimingMarginForErrorNormalized = Params.TimingMarginForError / Params.TimeForFullCycle;
	TimingMinigameAbilityTask->TimeWindowForInputNormalized = Params.TimeWindowForInputPress / Params.TimeForFullCycle;
	TimingMinigameAbilityTask->MinGapBetweenZonesNormalized = Params.MinGapBetweenZones / Params.TimeForFullCycle;
	TimingMinigameAbilityTask->AmountOfZones = Params.AmountOfZones;
	TimingMinigameAbilityTask->TimeZones = Params.TimeZones;

	UWorld* World = TimingMinigameAbilityTask->GetWorld();

	if (ATTPlayerController* playerController = Cast<ATTPlayerController>(World->GetFirstPlayerController()))
	{
		playerController->OnQTESkewer.AddDynamic(TimingMinigameAbilityTask, &UTimingMinigameAbilityTask::OnPressedB);
	}

	TimingMinigameAbilityTask->OnButtonPressed.AddDynamic(TimingMinigameAbilityTask, &UTimingMinigameAbilityTask::OnInputPressed);
	TimingMinigameAbilityTask->WorldTimeDilation = TimingMinigameAbilityTask->GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();
	TimingMinigameAbilityTask->PlayerTimeDilation = TimingMinigameAbilityTask->GetOwnerActor()->CustomTimeDilation;

	TimingMinigameAbilityTask->NextCorrectInput = EButtonToPress::B;

	return TimingMinigameAbilityTask;
}

void UTimingMinigameAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (MarkedEndTask)
	{
		EndTask();
	}

	float FinalTime = DeltaTime / (WorldTimeDilation * PlayerTimeDilation);

	if (CursorMovingForwards)
	{
		CurrentCycleProgress += FinalTime / TimeForFullCycle;

		// Reverse direction once we are over 1
		if (CurrentCycleProgress >= 1)
		{
			CurrentCycleProgress = FMath::Clamp(CurrentCycleProgress, 0, 1);
			CursorMovingForwards = false;
			NextCorrectInput = EButtonToPress::B;
		}
	}
	else
	{
		CurrentCycleProgress -= FinalTime / TimeForFullCycle;

		if (CurrentCycleProgress <= 0)
		{
			OnSecondMiss.Broadcast();
			EndTask();
		}
	}

	UpdateCurrentTimeZone();

	// Update Overlay
	QTEWidget.Get()->SetCursorPosition(CurrentCycleProgress);
}

void UTimingMinigameAbilityTask::OnInputPressed(EButtonToPress Button)
{
	if (NextCorrectInput == EButtonToPress::INVALID)
	{
		return;
	}

	QTEWidget->RunSkewerButtonAnimation();

	if (IsInTimingWindow())
	{
		// Good Timing
		DisplayInputFeedback(true);
		TimeZones[CurrentZoneIndex].State = ETimeZoneStates::Success;
		if (CheckIfMinigameSuccess())
		{	
			OnAllZonesComplete.Broadcast();
			EndTask();
		}
		else
		{
			OnCorrectTiming.Broadcast();
		}
	}
	else
	{
		// Bad Timing
		if (TimeZones[CurrentZoneIndex].State == ETimeZoneStates::FailedOnce)
		{
			if (!CursorMovingForwards)
			{
				DisplayInputFeedback(false, true);
				OnSecondMiss.Broadcast();
				PlayMissSound();
				EndTask();
			}
		}
		else
		{
			// Second Chance
			TimeZones[CurrentZoneIndex].State = ETimeZoneStates::FailedOnce;
			DisplayInputFeedback(false, false);
			PlayMissSound();
		}

	}

	if (!IsOnSecondCycle)
	{
		NextCorrectInput = EButtonToPress::INVALID;
		if (CurrentZoneIndex < TimeZones.Num() - 1)
		{
			// On any zone except last one
			CurrentZoneIndex++;
		}
		else
		{
			// On last zone
			IsOnSecondCycle = true;

			if (IsInTimingWindow())
			{
				// Find a zone that is pending
				while (TimeZones[CurrentZoneIndex].State == ETimeZoneStates::Success && CurrentZoneIndex > 0)
				{
					CurrentZoneIndex--;
				}
				CurrentZoneIndex = FMath::Clamp(CurrentZoneIndex, 0, TimeZones.Num() - 1);
			}
		}

	}
	else
	{
		// Find a zone that is pending
		while (TimeZones[CurrentZoneIndex].State == ETimeZoneStates::Success && CurrentZoneIndex > 0)
		{
			CurrentZoneIndex--;
		}
		CurrentZoneIndex = FMath::Clamp(CurrentZoneIndex, 0, TimeZones.Num() - 1);
	}
}


void UTimingMinigameAbilityTask::UpdateCurrentTimeZone()
{
	if (CursorMovingForwards && !IsOnSecondCycle)
	{
		// Advance Zone and mark as failed
		if (IsPastCurrentTimingZone())
		{
			if (TimeZones[CurrentZoneIndex].State == ETimeZoneStates::NotPressed)
			{
				TimeZones[CurrentZoneIndex].State = ETimeZoneStates::FailedOnce;
				DisplayInputFeedback(false, false);
				PlayMissSound();

				// At the last zone
				if (CurrentZoneIndex == TimeZones.Num() - 1)
				{
					IsOnSecondCycle = true;
					NextCorrectInput = EButtonToPress::INVALID;
					return;
				}
				else
				{
					CurrentZoneIndex++;
				}
			}
		}

		if (ShouldReenableInput())
		{
			CurrentCursorZone++;
			NextCorrectInput = EButtonToPress::B;
		}
	}
	else if (!CursorMovingForwards && IsOnSecondCycle)
	{
		// Last Zone, we have basically lost
		if (IsPastCurrentTimingZone())
		{
			if (TimeZones[CurrentZoneIndex].State != ETimeZoneStates::Success)
			{
				DisplayInputFeedback(false, true);
				OnSecondMiss.Broadcast();
				PlayMissSound();
				EndTask();
			}
		}
	}
}

bool UTimingMinigameAbilityTask::IsInTimingWindow()
{
	float CurrentZoneEnd = TimeZones[CurrentZoneIndex].Position + TimeWindowForInputNormalized + TimingMarginForErrorNormalized;

	float CurrentZoneStart = TimeZones[CurrentZoneIndex].Position - TimingMarginForErrorNormalized;

	return CurrentCycleProgress > CurrentZoneStart && CurrentCycleProgress < CurrentZoneEnd;
}

bool UTimingMinigameAbilityTask::CheckIfMinigameSuccess()
{
	for (FTimeZone TimeZone : TimeZones)
	{
		if (TimeZone.State != ETimeZoneStates::Success)
		{
			return false;
		}
	}
	return true;
}

bool UTimingMinigameAbilityTask::ShouldReenableInput()
{
	float Limit = TimeZones[CurrentCursorZone].Position + TimeWindowForInputNormalized + TimingMarginForErrorNormalized + MinGapBetweenZonesNormalized;
	return CurrentCycleProgress >= Limit;
}

void UTimingMinigameAbilityTask::DisplayInputFeedback(bool Success, bool SecondError)
{
	QTEWidget.Get()->RunSkewerFeedbackAnimation(CurrentZoneIndex, Success, SecondError);
}

bool UTimingMinigameAbilityTask::IsPastCurrentTimingZone()
{
	if (CursorMovingForwards)
	{
		float Limit = TimeZones[CurrentZoneIndex].Position + TimeWindowForInputNormalized + TimingMarginForErrorNormalized;
		return CurrentCycleProgress > Limit;
	}
	else
	{
		float Limit = TimeZones[CurrentZoneIndex].Position - TimingMarginForErrorNormalized;
		return CurrentCycleProgress < Limit;
	}
}
