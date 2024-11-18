// TinyTavern


#include "MashingMinigameAbilityTask.h"
#include "../../Player/TTPlayerController.h"
#include "../../TinyTavernUtils.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"

UMashingMinigameAbilityTask::UMashingMinigameAbilityTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UMashingMinigameAbilityTask> UMashingMinigameAbilityTask::CreateMinigameTask(FMashingMinigameParams Params)
{
	TObjectPtr<UMashingMinigameAbilityTask> MashingMinigameAbilityTask = NewAbilityTask<UMashingMinigameAbilityTask>(Params.OwningAbility);
	MashingMinigameAbilityTask->CurrentTimeLeft = Params.TimeWindow;
	MashingMinigameAbilityTask->TotalTimeMinigame = Params.TimeWindow;
	MashingMinigameAbilityTask->QTEWidget = Params.WidgetInstance;
	MashingMinigameAbilityTask->CorrectInputProgression = Params.CorrectInputProgression;
	MashingMinigameAbilityTask->ProgressionLostPerSecond = Params.ProgressionLostPerSecond;
	MashingMinigameAbilityTask->OwningAbility = Params.OwningAbility;

	UWorld* World = MashingMinigameAbilityTask->GetWorld();

	MashingMinigameAbilityTask->WorldTimeDilation = World->GetWorldSettings()->GetEffectiveTimeDilation();
	MashingMinigameAbilityTask->PlayerTimeDilation = MashingMinigameAbilityTask->GetOwnerActor()->CustomTimeDilation;

	if (ATTPlayerController* playerController = Cast<ATTPlayerController>(World->GetFirstPlayerController()))
	{
		playerController->OnQTETenderizer.AddDynamic(MashingMinigameAbilityTask, &UMashingMinigameAbilityTask::OnPressedX);
	}

	MashingMinigameAbilityTask->NextCorrectInput = EButtonToPress::X;
	MashingMinigameAbilityTask->OnButtonPressed.AddDynamic(MashingMinigameAbilityTask, &UMashingMinigameAbilityTask::OnInputPressed);

	return MashingMinigameAbilityTask;
}

void UMashingMinigameAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (MarkedEndTask)
	{
		EndTask();
		return;
	}

	if (CurrentTimeLeft <= 0)
	{
		OnTimerFinished.Broadcast();
	}
	else
	{
		CurrentTimeLeft -= DeltaTime / (WorldTimeDilation * PlayerTimeDilation);

		MashingProgress -= ProgressionLostPerSecond * (DeltaTime / (WorldTimeDilation * PlayerTimeDilation));
		MashingProgress = FMath::Clamp(MashingProgress, 0, 100);
	}

	// UI
	QTEWidget->UpdateSmashBarPercent(MashingProgress);
	QTEWidget->UpdateTimerSmashBarPercent(1 - CurrentTimeLeft / TotalTimeMinigame);
}


void UMashingMinigameAbilityTask::OnInputPressed(EButtonToPress Button)
{
	// Add to progress
	MashingProgress += CorrectInputProgression;

	// UI
	QTEWidget->UpdateSmashBarPercent(MashingProgress);
	//QTEWidget->RunSmashButtonAnimation();

	if (MashingProgress >= 100)
	{
		OnMinigameComplete.Broadcast();
	}

}
