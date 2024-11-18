// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameFramework/Character.h"
#include "AbilityTask_OnCollision.generated.h"

class UGameplayEffect;
class APlayerCharacter;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskOnCollisionDelegate, AActor*, Actor);

UCLASS()
class TINYTAVERNGAME_API UAbilityTask_OnCollision : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionDelegate	OnHit;
	
	UAbilityTask_OnCollision(const FObjectInitializer& ObjectInitializer);
	
	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();

	/** 
	 * Start Calculate the endpoint for the charge attack
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param ImpactLocation 
	 * @param ImpactOffset 
	 * @param ImpactRadius 
	 * @param ImpactRadius 
 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="OnCollision",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_OnCollision* OnCollisionAbilityTask(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, FVector ImpactLocation, FVector ImpactOffset, float ImpactRadius, TArray<AActor*>IgnoreActors, float Timer, bool NeedTick);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;
	
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<UShapeComponent> Collider;
	
	UPROPERTY()
	FVector ImpactLocation;

	UPROPERTY()
	FVector ImpactOffset;

	UPROPERTY()
	float ImpactRadius;
	
	UPROPERTY()
	bool bStopWhenAbilityEnds;

	UPROPERTY()
	bool NeedTick;

	UPROPERTY()
	float Timer;

	float TotalTime;
	
	UPROPERTY()
	TObjectPtr<AActor> NullActorBroadcast;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;

	UPROPERTY()
	TArray<AActor*>IgnoreActors;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> GameplayEffectToApply;
	
	UFUNCTION()
	void OnSweepBegin();
};
