// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Aim.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskAimDelegate, FVector, Location);


UCLASS()
class TINYTAVERNGAME_API UAbilityTask_Aim : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskAimDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskAimDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskAimDelegate	OnCancelled;

	

	UAbilityTask_Aim(const FObjectInitializer& ObjectInitializer);
    /** Callback function for when the owning Gameplay Ability is cancelled */

	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnGameplayAbilityCancelled();


	/** 
	 * Start Calculate the endpoint for the charge attack
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param Target Target to be used to calculate the load
	 * @param TargetDistance Fix target to calculate de Distance
	 * @param Distance Use Distance max to calculate the point of the charge
	 * @param Tolerance Modify Final Distance
	 * @param AimTime Time to wait until finish aim, use for orientate the actor to target;
	 * @param OrientToTarget Rotate Actor to Target
	 * @param RotationSpeed Rotation Speed

 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="AimAndWait",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_Aim* AimAbilityTask(UGameplayAbility* OwningAbility,
		FName TaskInstanceName,AActor* Target,float AimTime,float HeightOffset,float RotationSpeed,bool OrientToTarget,bool TargetDistance, float Distance, float Tolerance);

	virtual void Activate() override;
	void Aim();
	
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<AActor> Target;

	UPROPERTY()
	float Distance;

	UPROPERTY()
	bool TargetDistance;

	UPROPERTY()
	float HeightOffset=0;

	UPROPERTY()
	float Tolerance;

	UPROPERTY()
	bool bStopWhenAbilityEnds;	

	UPROPERTY()
	float AimTime;

	UPROPERTY()
	float RotationSpeed = 2;

	UPROPERTY()
	bool OrientToTarget;

private:
	TObjectPtr<AActor> Charger ;

};
