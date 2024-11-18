// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FinisherAbility.h"
#include "TinyTavernGame/Enums/EButtonToPress.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "MinigameAbility.generated.h"


class AEnemyBase;
class APlayerCharacter;
class UMinigameDelayAbilityTask;
enum class EEnemyType : uint8;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMinigameAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnPressedX();

	UFUNCTION()
	void OnPressedY();

	UFUNCTION()
	void OnPressedB();

	UPROPERTY()
	FOnMinigameSuccess OnSuccessDelegate;

	UPROPERTY()
	FOnMinigameFailure OnFailureDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFX")
	TSubclassOf<AActor> BloodVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation")
	float AnimationSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation")
	UAnimMontage* AnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
		float InitialMinigameDelay = 0.0f;


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	void PlayMissSound();

	UFUNCTION()
		void OnMinigameSucceeded();

	UFUNCTION()
		void OnMinigameFailed();

	UFUNCTION()
		void OnHit();

	UFUNCTION()
		virtual void ActivateMinigame() {};

	UFUNCTION()
		void OnInitialMinigameAnimationFinished();

	virtual void OnButtonPressed(EButtonToPress Button) {};
	bool IsInputAllowed();
	bool IsInputCorrect(EButtonToPress Button);
	EButtonToPress GetRandomButton();

	EButtonToPress NextCorrectInput;

	TObjectPtr<UQTEOverlay> QTEWidget;
	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<UMinigameAbilityTask> MinigameTask;
	TObjectPtr<UMinigameDelayAbilityTask> DelayTask;

};
