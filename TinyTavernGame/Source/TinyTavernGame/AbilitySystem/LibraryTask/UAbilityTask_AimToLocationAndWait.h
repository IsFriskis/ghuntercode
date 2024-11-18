// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UAbilityTask_AimToLocationAndWait.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskAimToLocationDelegate, FVector, Location);


UCLASS()
class TINYTAVERNGAME_API UAbilityTask_AimToLocationAndWait : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskAimToLocationDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskAimToLocationDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskAimToLocationDelegate	OnCancelled;

	UAbilityTask_AimToLocationAndWait(const FObjectInitializer& ObjectInitializer);
	/** Callback function for when the owning Gameplay Ability is cancelled */

	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnGameplayAbilityCancelled();


	/** 
	 * Start Calculate the endpoint for the charge attack
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param Location loaction to be used to calculate the load
	 * @param TargetDistance Fix target to calculate de Distance
	 * @param Distance Use Distance max to calculate the point of the charge
	 * @param Tolerance Modify Final Distance
	 * @param AimTime Time to wait until finish aim, use for orientate the actor to target;
	 * @param OrientToTarget Rotate Actor to location 

 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="AimToLocationAndWait",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_AimToLocationAndWait* AbilityTask_AimToLocationAndWait(UGameplayAbility* OwningAbility,
		FName TaskInstanceName,float AimTime, float Distance);

	virtual void Activate() override;
	void AimToLocation();
	
	FDelegateHandle InterruptedHandle;



	UPROPERTY()
	float Distance;



	UPROPERTY()
	bool bStopWhenAbilityEnds;	

	UPROPERTY()
	float AimTime;


private:
	TObjectPtr<AActor> Charger ;
};
