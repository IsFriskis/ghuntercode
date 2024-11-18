// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TinyTavernGame/Weapons/PlayWeaponAnimationAbility.h"
#include "JamSlamAbility.generated.h"

class UNiagaraSystem;
class UAbilityTask_WaitGameplayEvent;
class AJamSlamArea;
class UTTAnimNotify;
class UJamSlamAimTask;
class UInputAction;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UJamSlamAbility : public UPlayWeaponAnimationAbility
{
	GENERATED_BODY()

public:
	FVector GetJamSpawnLocation() const { return JamSpawnLocation; }

	void SetJamSpawnLocation(FVector _JamSpawnLocation) { this->JamSpawnLocation = _JamSpawnLocation; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float PetrificationSpawnMaxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float PetrificationSpawnMinRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float PetrificationDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float PetrificationDurationAfterWine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float PetrificationAreaRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float PetrificationAreaTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float TimeBeforePetrification;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jam Ingredient Info")
	float HeightOfJamSlamMeteor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	float JamSlamMeteorSpeed;
	UPROPERTY(EditAnywhere, Category = "TT| Jam Ingredient Info")
	float AimSmoothFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	TSubclassOf<UGameplayEffect> PetrificationEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	TSubclassOf<UGameplayEffect> InProcessOfPetrificationEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Jam Ingredient Info")
	TSubclassOf<AJamSlamArea> JamSlamAreaClass;
	UPROPERTY(EditAnywhere, Category = "TT| Jam Ingredient Info")
	TObjectPtr<UAnimMontage> HoldAnim;
	UPROPERTY(EditAnywhere, Category = "TT| Jam Ingredient Info")
	TObjectPtr<UStaticMesh> JamIndicatorMesh;
	UPROPERTY(EditAnywhere, Category = "TT| Jam Ingredient Info")
	TObjectPtr<UNiagaraSystem> SplashNiagaraSystem;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UJamSlamAimTask> JamSlamAimTask;
	TObjectPtr<UTTAnimNotify> JamAnimNotifySpawn;
	TObjectPtr<UTTAnimNotify> JamAnimNotify;
	FVector JamSpawnLocation;
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEvent;
	
	bool InputHasBeenReleased = false;

	UFUNCTION()
	void FinishAbility(FGameplayEventData Payload);
	UFUNCTION()
	void SpawnJam();
	void JamSlamLogic();
	UFUNCTION()
	void ClearWeaponsOnNotify();
	virtual void FinishPlayAnimation() override;

	bool bAlreadyEnding = false;
};
