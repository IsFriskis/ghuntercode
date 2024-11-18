// TinyTavern Includes
#include "SequenceMinigameAbility.h"
#include "SequenceMinigameAbilityTask.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "MinigameDelayAbilityTask.h"

// External Includes
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


void USequenceMinigameAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	QTEWidget.Get()->SetQTEWidget(ECuts::Slice);
}

void USequenceMinigameAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	for (FAnimNotifyEvent AnimEvent : AnimationMontage->Notifies)
	{
		UTTAnimNotify* Notify = Cast<UTTAnimNotify>(AnimEvent.Notify.Get());
		if(Notify)
		{
			Notify->OnNotified.RemoveAll(this);
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USequenceMinigameAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void USequenceMinigameAbility::OnButtonPressed(EButtonToPress Button)
{
	// Guard clause
	if (!IsInputAllowed())
	{
		return;
	}
	
	QTEWidget->RunSliceButtonAnimation();

	int LivesLeft = *InputsFailedToLose.Find(Enemy->GetEnemyTypeExact()) - CurrentFailedInputs;

	if (IsInputCorrect(Button))
	{
		CurrentSuccessfulInputs++;
		QTEWidget->RunSliceFeedbackAnimation(LivesLeft, true);
		if (CurrentSuccessfulInputs >= *InputsRequiredToWin.Find(Enemy->GetEnemyTypeExact()))
		{
			OnSuccessDelegate.Broadcast();
			MinigameTask->MarkedEndTask = true;

			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
		else
		{
			// Play Hit Animation
			FName AnimationSectionName = AnimationMontage->GetSectionName(GetAnimationIndex(Button));
			TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
			MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "Hit", AnimationMontage, AnimationSpeedMultiplier, AnimationSectionName, false);
			MontageTask->ReadyForActivation();
		}

	}
	else
	{
		CurrentFailedInputs++;
		QTEWidget->RunSliceFeedbackAnimation(LivesLeft, false);
		PlayMissSound();
		if (LivesLeft <= 0)
		{
			OnFailureDelegate.Broadcast();
			MinigameTask->MarkedEndTask = true;
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
	}
	QTEWidget->HideSliceButton();
	SetDelayForNextInput();
}

void USequenceMinigameAbility::OnTimerRanOut()
{
	CurrentFailedInputs++;
	if (CurrentFailedInputs >= *InputsFailedToLose.Find(Enemy->GetEnemyTypeExact()))
	{
		OnFailureDelegate.Broadcast();
		MinigameTask->MarkedEndTask = true;
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

	// TODO: Probably add feedback to each button press - UI Animation?
	QTEWidget->RunSliceFeedbackAnimation(CurrentFailedInputs, false);
	QTEWidget->HideSliceButton();
	PlayMissSound();
	SetDelayForNextInput();
}

int USequenceMinigameAbility::GetAnimationIndex(EButtonToPress Button)
{
	if (!AnimationMontage)
	{
		return -1; // Forces crash if animation montage not set - Array out of bounds
	}

	switch (Button)
	{
		case EButtonToPress::X:
		{
			return 0;
		}
		case EButtonToPress::Y:
		{
			return 1;
		}
		case EButtonToPress::B:
		{
			return 2;
		}
	}	
	
	return 0;
}

void USequenceMinigameAbility::GenerateNextInput()
{
	// Generate Button
	NextCorrectInput = GetRandomButton();

	//UI
	QTEWidget->DisplayNewSliceButton(NextCorrectInput);
}

void USequenceMinigameAbility::SetDelayForNextInput()
{
	// Account for time dilation
	float WorldTimeDilation = GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();

	// Disable Input
	NextCorrectInput = EButtonToPress::INVALID;

	// Call function after delay
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USequenceMinigameAbility::GenerateNextInput, DelayBetweenGeneratedInputs * WorldTimeDilation, false);
}

void USequenceMinigameAbility::ActivateMinigame()
{
	DelayTask->OnTimerCompleted.RemoveAll(this);
	DelayTask->MarkForDeath();

	int AmountOfNotifies = AnimationMontage->Notifies.Num();

	if (AmountOfNotifies > 0)
	{
		for (int i = 0; i < AmountOfNotifies; i++)
		{
			UTTAnimNotify* Notify = Cast<UTTAnimNotify>(AnimationMontage->Notifies[i].Notify.Get());
			if (Notify)
			{
				Notify->OnNotified.AddDynamic(this, &USequenceMinigameAbility::OnHit);
			}
		}
	}

	FSequenceMinigameParams Params;
	Params.OwningAbility = this;
	Params.DelayTimeBetweenInputs = DelayBetweenGeneratedInputs;
	Params.TimeWindowPerInput = *TimeWindowPerButton.Find(Enemy->GetEnemyTypeExact());
	Params.WidgetInstance = QTEWidget;

	MinigameTask = USequenceMinigameAbilityTask::CreateMinigameTask(Params);
	MinigameTask->OnButtonPressed.AddDynamic(this, &USequenceMinigameAbility::OnButtonPressed);
	Cast<USequenceMinigameAbilityTask>(MinigameTask)->OnTimerFinished.AddDynamic(this, &USequenceMinigameAbility::OnTimerRanOut);
	MinigameTask->ReadyForActivation();
	GenerateNextInput();
}
