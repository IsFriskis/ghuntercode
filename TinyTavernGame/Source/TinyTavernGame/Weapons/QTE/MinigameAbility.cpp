// TinyTavern


#include "MinigameAbility.h"

#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "MinigameDelayAbilityTask.h"


void UMinigameAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData != nullptr)
	{
		const FGameplayAbilityTargetData* TargetData = TriggerEventData->TargetData.Get(0);

		if (const FMinigameData* MinigameData = static_cast<const FMinigameData*>(TargetData))
		{
			Enemy = MinigameData->Enemy;
			QTEWidget = MinigameData->QTEWidget;
			OnSuccessDelegate = MinigameData->OnSuccessDelegate;
			OnFailureDelegate = MinigameData->OnFailureDelegate;

			QTEWidget->OnInitialAnimationFinished.AddDynamic(this, &UMinigameAbility::OnInitialMinigameAnimationFinished);
			QTEWidget->RunMinigameStartAnimation();
		}
	}
}

void UMinigameAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMinigameAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMinigameAbility::PlayMissSound()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	Player->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_QTEFailedInput);
}

void UMinigameAbility::OnMinigameSucceeded()
{
	OnSuccessDelegate.Broadcast();
}

void UMinigameAbility::OnMinigameFailed()
{
	OnFailureDelegate.Broadcast();
}

void UMinigameAbility::OnInitialMinigameAnimationFinished()
{
	QTEWidget->OnInitialAnimationFinished.RemoveAll(this);
	QTEWidget->RunSmashButtonBlinkAnimation();
	QTEWidget->RunSkewerStartAnimation();
	QTEWidget->RunInitialTimeZoneAnimations();
	DelayTask = UMinigameDelayAbilityTask::CreateMinigameDelayTask(this, InitialMinigameDelay);
	DelayTask->OnTimerCompleted.AddDynamic(this, &UMinigameAbility::ActivateMinigame);
	DelayTask->ReadyForActivation();
}

bool UMinigameAbility::IsInputAllowed()
{
	return NextCorrectInput != EButtonToPress::INVALID;
}

bool UMinigameAbility::IsInputCorrect(EButtonToPress Button)
{
	return Button == NextCorrectInput;
}

void UMinigameAbility::OnHit()
{
	// Spawn blood
	TObjectPtr<UAbilitySystemComponent> TargetASC = Enemy->GetAbilitySystemComponent();
	AActor* BloodActor = GetWorld()->SpawnActor<AActor>(BloodVFX, Enemy->GetActorLocation(), Enemy->GetActorRotation());

	// Speed up blood because game is slowed down
	BloodActor->CustomTimeDilation = 1.0 / GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();

	// Random Sound
	int RandomValue = FMath::RandRange(0, 1);
	if (RandomValue == 0)
	{
		TargetASC->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_HitSound1);
	}
	else
	{
		TargetASC->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_HitSound2);
	}
}

void UMinigameAbility::OnPressedX()
{
	OnButtonPressed(EButtonToPress::X);
}

void UMinigameAbility::OnPressedY()
{
	OnButtonPressed(EButtonToPress::Y);
}

void UMinigameAbility::OnPressedB()
{
	OnButtonPressed(EButtonToPress::B);
}

EButtonToPress UMinigameAbility::GetRandomButton()
{
	const uint32 enumCount = static_cast<uint32>(EButtonToPress::Y); // Y is the last button in the enum

	uint32 randomIndex = FMath::RandRange(0, enumCount);

	return static_cast<EButtonToPress>(randomIndex);
}


