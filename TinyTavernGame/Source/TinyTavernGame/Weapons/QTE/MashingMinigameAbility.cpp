// TinyTavern Includes
#include "MashingMinigameAbility.h"
#include "MashingMinigameAbilityTask.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "MinigameDelayAbilityTask.h"

// External Includes
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


void UMashingMinigameAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	QTEWidget.Get()->SetQTEWidget(ECuts::Smash);
}

void UMashingMinigameAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

void UMashingMinigameAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMashingMinigameAbility::OnButtonPressed(EButtonToPress Button)
{
	// Guard clause
	if (!IsInputAllowed())
	{
		return;
	}

	QTEWidget->StopSmashButtonBlinkAnimation();

	if (AnimationMontage)
	{
		FName AnimationSectionName = AnimationMontage->GetSectionName(1);
		TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "Hit", AnimationMontage, AnimationSpeedMultiplier, AnimationSectionName, false);
		MontageTask->ReadyForActivation();
		QTEWidget->RunSmashButtonAnimation();
	}
}

void UMashingMinigameAbility::OnTimeRanOut()
{
	PlayMissSound();
	OnFailureDelegate.Broadcast();
	MinigameTask->MarkedEndTask = true;
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UMashingMinigameAbility::OnMinigameSuccess()
{
	OnSuccessDelegate.Broadcast();
	MinigameTask->MarkedEndTask = true;
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UMashingMinigameAbility::ActivateMinigame()
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
				Notify->OnNotified.AddDynamic(this, &UMashingMinigameAbility::OnHit);
			}
		}
	}

	FMashingMinigameParams Params;
	Params.OwningAbility = this;
	Params.TimeWindow = *MinigameTimeWindow.Find(Enemy->GetEnemyTypeExact());
	Params.CorrectInputProgression = *CorrectInputGain.Find(Enemy->GetEnemyTypeExact());
	Params.ProgressionLostPerSecond = *ProgressionLostPerSecond.Find(Enemy->GetEnemyTypeExact());
	Params.ButtonToMash = ButtonToMash;
	Params.WidgetInstance = QTEWidget;

	MinigameTask = UMashingMinigameAbilityTask::CreateMinigameTask(Params);
	MinigameTask->OnButtonPressed.AddDynamic(this, &UMashingMinigameAbility::OnButtonPressed);
	Cast<UMashingMinigameAbilityTask>(MinigameTask)->OnTimerFinished.AddDynamic(this, &UMashingMinigameAbility::OnTimeRanOut);
	Cast<UMashingMinigameAbilityTask>(MinigameTask)->OnMinigameComplete.AddDynamic(this, &UMashingMinigameAbility::OnMinigameSuccess);
	MinigameTask->ReadyForActivation();
}
