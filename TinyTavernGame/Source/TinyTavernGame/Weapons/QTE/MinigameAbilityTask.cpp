// TinyTavern


#include "MinigameAbilityTask.h"
#include "FinisherAbility.h"
#include "../../Player/TTPlayerController.h"
#include "../../TinyTavernUtils.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/UI/HUD/QTE/QTEOverlay.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "AbilitySystemComponent.h"

void UMinigameAbilityTask::OnPressedX()
{
	OnButtonPressed.Broadcast(EButtonToPress::X);
}

void UMinigameAbilityTask::OnPressedY()
{
	OnButtonPressed.Broadcast(EButtonToPress::Y);
}

void UMinigameAbilityTask::OnPressedB()
{
	OnButtonPressed.Broadcast(EButtonToPress::B);
}


void UMinigameAbilityTask::EndMinigame(bool MinigameSucceded)
{
	if (ATTPlayerController* playerController = Cast<ATTPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		playerController->OnQTECleaver.RemoveAll(this);
		playerController->OnQTETenderizer.RemoveAll(this);
		playerController->OnQTESkewer.RemoveAll(this);
		playerController->OnQTEButtonA.RemoveAll(this);
	}
}

void UMinigameAbilityTask::PlayMissSound()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwnerActor());
	Player->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_QTE_QTEFailedInput);
}
	
EButtonToPress UMinigameAbilityTask::GetRandomButton()
{
	const uint32 enumCount = static_cast<uint32>(EButtonToPress::Y); // Y is the last button in the enum

	uint32 randomIndex = FMath::RandRange(0, enumCount); 

	return static_cast<EButtonToPress>(randomIndex);
}