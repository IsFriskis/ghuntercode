// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ArcCollisions.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskArcCollisionsDelegate, AActor*, ActorHit);


UCLASS()
class TINYTAVERNGAME_API UAbilityTask_ArcCollisions : public UAbilityTask
{
	GENERATED_BODY()


public:
	
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskArcCollisionsDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskArcCollisionsDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskArcCollisionsDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskArcCollisionsDelegate	OnHit;

	UAbilityTask_ArcCollisions(const FObjectInitializer& ObjectInitializer);
	/** Callback function for when the owning Gameplay Ability is cancelled */

	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnGameplayAbilityCancelled();


	/** 
	 * Start Calculate the endpoint for the charge attack
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param StartLocation
	 * @param StartRotation 
	 * @param Radius Distance to calculate end points
	 * @param AngleDegrees Arc_Radius
	 * @param Segments Number of lines to calculate collisions;
	 

 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="ArcCollisions",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_ArcCollisions* ArcCollisions(UGameplayAbility* OwningAbility,
		FName TaskInstanceName,FVector StartLocation,FRotator StartRotation,float Radius, float RadiusStep,float AngleDegrees,int32 Segments);

	virtual void Activate() override;
	
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	bool bStopWhenAbilityEnds;	

	// Arc Params
	FVector StartLocation ;
	FRotator StartRotation;
	float Radius;
	float CurrentRadius;
	float AngleDegrees;
	int32 Segments;

	UPROPERTY()
	float RadiusStep;
};

