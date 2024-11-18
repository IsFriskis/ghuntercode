// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TinyTavernGame/Weapons/PlayWeaponAnimationAbility.h"
#include "WineAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UWineAimTask;
class UTTAnimNotify;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UWineAbility : public UPlayWeaponAnimationAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	UPROPERTY(EditAnywhere, Category = "TT| Wine Ability Info")
	TSubclassOf<UGameplayEffect> DrunkEffect;
	UPROPERTY(EditAnywhere, Category = "TT| Wine Ability Info")
	float WineRadius;
	UPROPERTY(EditAnywhere, Category = "TT| Wine Ability Info")
	TObjectPtr<UAnimMontage> HoldAnim;
	UPROPERTY(EditAnywhere, Category = "TT| Wine Ability Info")
	TObjectPtr<UStaticMesh> WineIndicatorMesh;
	
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UTTAnimNotify> WineAnimNotify;
	TObjectPtr<UWineAimTask> WineAimTask;
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEvent;
	bool InputHasBeenReleased = false;

	UFUNCTION()
	void FinishAbility(FGameplayEventData Payload);
	//Throws the wine on the floor detecting enemies and other ingredients
	//And applies effects
	UFUNCTION()
	void ExecuteSplash();
	void RunWineLogic();

private:
	bool bAlreadyEnding = false;
};
