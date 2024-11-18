// TinyTavern


#include "FinisherAbilityTask.h"
#include "../../Player/TTPlayerController.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"

UFinisherAbilityTask::UFinisherAbilityTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UFinisherAbilityTask> UFinisherAbilityTask::CreateFinisherTask(UGameplayAbility* OwningAbility, float TimeWindow, int WeaponLevel, UQTEOverlay* WidgetInstance)
{
	TObjectPtr<UFinisherAbilityTask> FinisherAbilityTask = NewAbilityTask<UFinisherAbilityTask>(OwningAbility);
	FinisherAbilityTask->QTEWindowDuration = TimeWindow;
	FinisherAbilityTask->TimeLeft = TimeWindow;
	FinisherAbilityTask->WeaponLevel = WeaponLevel;
	FinisherAbilityTask->QTEWidget = WidgetInstance;

	UWorld* World = FinisherAbilityTask->GetWorld();

	if (World)
	{
		//Show QTE Widget
		ATTPlayerController* playerController = Cast<ATTPlayerController>(World->GetFirstPlayerController());

		playerController->OnQTECleaver.AddDynamic(FinisherAbilityTask, &UFinisherAbilityTask::OnCleaverPressed);
		playerController->OnQTETenderizer.AddDynamic(FinisherAbilityTask, &UFinisherAbilityTask::OnTenderizerPressed);
		playerController->OnQTESkewer.AddDynamic(FinisherAbilityTask, &UFinisherAbilityTask::OnSkewerPressed);
	}


	return FinisherAbilityTask;
}

void UFinisherAbilityTask::OnCleaverPressed()
{
	if (WeaponLevel >= 0)
	{
		UnbindInputs();
		OnPress.Broadcast(FGameplayTags::Get().States_Cuts_Sliced);
		MarkedEndTask = true;
	}

}

void UFinisherAbilityTask::OnTenderizerPressed()
{
	if (WeaponLevel >= 1)
	{
		UnbindInputs();
		OnPress.Broadcast(FGameplayTags::Get().States_Cuts_Smashed);
		MarkedEndTask = true;
	}
}

void UFinisherAbilityTask::OnSkewerPressed()
{
	if (WeaponLevel >= 2)
	{
		UnbindInputs();
		OnPress.Broadcast(FGameplayTags::Get().States_Cuts_Skewered);
		MarkedEndTask = true;
	}
}


void UFinisherAbilityTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	if (MarkedEndTask)
	{
		EndTask();
		return;
	}

	if (TimeLeft <= 0)
	{
		UnbindInputs();
		OnTimeEnded.Broadcast();
		MarkedEndTask = true;
		return;
	}

	float currentTimeDilation = GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();

	TimeLeft -= DeltaTime / currentTimeDilation;
	QTEWidget->UpdateQTESelectionBarTimer(TimeLeft / QTEWindowDuration);
}

void UFinisherAbilityTask::UnbindInputs()
{
	if (ATTPlayerController* playerController = Cast<ATTPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		playerController->OnQTECleaver.RemoveAll(this);
		playerController->OnQTETenderizer.RemoveAll(this);
		playerController->OnQTESkewer.RemoveAll(this);
	}
}
