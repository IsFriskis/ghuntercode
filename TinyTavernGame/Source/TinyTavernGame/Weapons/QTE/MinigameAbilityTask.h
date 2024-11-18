// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "../Source/TinyTavernGame/Enums/EButtonToPress.h"
#include "MinigameAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMinigameButtonPressed, EButtonToPress, Button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSuccessfulHit);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinigameSuccess);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinigameFailure);

enum class EEnemyType : uint8;
class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UQTEOverlay;

/**
 *
 */
UCLASS()
class TINYTAVERNGAME_API UMinigameAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnMinigameButtonPressed OnButtonPressed;

	UPROPERTY(BlueprintAssignable)
	FOnSuccessfulHit OnSuccessfulHit;

	UFUNCTION()
	void OnPressedX();

	UFUNCTION()
	void OnPressedY();

	UFUNCTION()
	void OnPressedB();

	bool MarkedEndTask = false;

protected:
	//virtual void OnButtonPressed(EButtonToPress Button);
	void EndMinigame(bool MinigameSucceded);
	void PlayMissSound();
	EButtonToPress GetRandomButton();

	//Logic
	EButtonToPress NextCorrectInput;

	UPROPERTY()
	UGameplayAbility* OwningAbility;

	// UI
	UPROPERTY()
	TObjectPtr<UQTEOverlay> QTEWidget;

	UPROPERTY()
	UAnimMontage* AnimationMontage;

	float AnimationSpeed = 1.0f;
	int AnimationIndex = 0;
	
private:

};
