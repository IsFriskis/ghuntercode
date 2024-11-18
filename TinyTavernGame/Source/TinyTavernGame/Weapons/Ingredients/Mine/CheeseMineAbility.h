// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CheeseMineAbility.generated.h"

class UTTAnimNotify;
class UAbilityTask_WaitGameplayEvent;
class UCheeseAimTask;
class AMine;
class UCheeseMineTask;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheeseMineAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cheese Info")
	float MinePlantTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cheese Info")
	float CheeseMineInnerRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cheese Info")
	float CheeseMineOuterRadius = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cheese Info")
	float MinePlacementDistanceFromPlayer = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cheese Info")
	UAnimMontage* PlantMineMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cheese Info")
	TSubclassOf<AMine> CheeseMine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cheese Info")
	TObjectPtr<UStaticMesh> CheeseIndicatorMesh;
	UPROPERTY(EditAnywhere, Category = "TT| Cheese Info")
	TObjectPtr<UAnimMontage> HoldAnim;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	TObjectPtr<APlayerCharacter> Player;
	//TObjectPtr<UCheeseMineTask> CheeseMineTask;
	TObjectPtr<UCheeseAimTask> CheeseAimTask;
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEvent;
	TObjectPtr<UTTAnimNotify> CheeseAnimNotify;
	bool InputHasBeenReleased = false;
	bool OnLogic = false;

	UFUNCTION()
	void InputReleasedAbility(FGameplayEventData Payload);
	UFUNCTION()
	void FinishAbility();
	//Plant Mine Logic
	void PlantCheeseMine();
};
