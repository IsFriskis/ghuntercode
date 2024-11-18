// TinyTavern Includes
#include "TimingMinigameAbility.h"
#include "TimingMinigameAbilityTask.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "MinigameDelayAbilityTask.h"

// External Includes
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"



void UTimingMinigameAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	EarliestPossibleTimingNormalized = EarliestPossibleTiming / *TimeForFullCycle.Find(Enemy->GetEnemyTypeExact());
	TimingMarginForErrorNormalized = TimingMarginForError / *TimeForFullCycle.Find(Enemy->GetEnemyTypeExact());
	TimeWindowForInputNormalized = *TimeWindowForInputPress.Find(Enemy->GetEnemyTypeExact()) / *TimeForFullCycle.Find(Enemy->GetEnemyTypeExact());
	MinGapBetweenZonesNormalized = MinGapBetweenZones / *TimeForFullCycle.Find(Enemy->GetEnemyTypeExact());

	InitializeMinigameTimingZones();
	QTEWidget.Get()->SetQTEWidget(ECuts::Skewer);
}

void UTimingMinigameAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

void UTimingMinigameAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UTimingMinigameAbility::OnSecondMiss()
{
	OnFailureDelegate.Broadcast();
}

void UTimingMinigameAbility::OnAllZonesComplete()
{
	OnSuccessDelegate.Broadcast();
}

void UTimingMinigameAbility::OnCorrectTiming()
{
	if (AnimationMontage)
	{
		FName AnimationSectionName = AnimationMontage->GetSectionName(0);
		TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "Hit", AnimationMontage, AnimationSpeedMultiplier, AnimationSectionName, false);
		MontageTask->ReadyForActivation();
	}
}

void UTimingMinigameAbility::InitializeMinigameTimingZones()
{
	float InitialPositionLimit = QTEWidget->GetHandleEndAngle() / 360; // Normalize value 0-1
	float CurrentPosition = EarliestPossibleTimingNormalized;
	int ZoneCount = *AmountOfZones.Find(Enemy->GetEnemyTypeExact());

	for (int i = 0; i < ZoneCount; i++)
	{
		float TotalSpace = (InitialPositionLimit - CurrentPosition);
		TotalSpace -= (MinGapBetweenZonesNormalized * FMath::Clamp(ZoneCount - (i + 1), 0, MAX_AMOUNT_ZONES));
		float TotalSpacePerZone = TotalSpace / static_cast<float>(ZoneCount - i) - TimeWindowForInputNormalized;

		float RandomOffset = FMath::RandRange(0.0f, TotalSpacePerZone);
		float ZonePosition = CurrentPosition + RandomOffset;

		FTimeZone TimeZone;
		TimeZone.Position = ZonePosition;
		TimeZone.State = ETimeZoneStates::NotPressed;

		TimeZones.Add(TimeZone);


		float RemainingPartOfZone = (TotalSpacePerZone - RandomOffset) + TimeWindowForInputNormalized + MinGapBetweenZonesNormalized;
		CurrentPosition = ZonePosition + RemainingPartOfZone;
	}

	QTEWidget->SetTimeZones(TimeZones, TimeWindowForInputNormalized);
}

void UTimingMinigameAbility::ActivateMinigame()
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
				Notify->OnNotified.AddDynamic(this, &UTimingMinigameAbility::OnHit);
			}
		}
	}

	FTimingMinigameParams Params;
	Params.OwningAbility = this;
	Params.AmountOfZones = *AmountOfZones.Find(Enemy->GetEnemyTypeExact());
	Params.EarliestPossibleTiming = EarliestPossibleTiming;
	Params.MinGapBetweenZones = MinGapBetweenZones;
	Params.TimeForFullCycle = *TimeForFullCycle.Find(Enemy->GetEnemyTypeExact());
	Params.TimeWindowForInputPress = *TimeWindowForInputPress.Find(Enemy->GetEnemyTypeExact());
	Params.TimingMarginForError = TimingMarginForError;
	Params.WidgetInstance = QTEWidget;
	Params.TimeZones = TimeZones;
	
	MinigameTask = UTimingMinigameAbilityTask::CreateMinigameTask(Params);
	Cast<UTimingMinigameAbilityTask>(MinigameTask)->OnAllZonesComplete.AddDynamic(this, &UTimingMinigameAbility::OnAllZonesComplete);
	Cast<UTimingMinigameAbilityTask>(MinigameTask)->OnSecondMiss.AddDynamic(this, &UTimingMinigameAbility::OnSecondMiss);
	Cast<UTimingMinigameAbilityTask>(MinigameTask)->OnCorrectTiming.AddDynamic(this, &UTimingMinigameAbility::OnCorrectTiming);
	MinigameTask->ReadyForActivation();
}










