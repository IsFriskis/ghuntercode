// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Components/ShapeComponent.h"
#include "AbilityTask_Charge.generated.h"

/**
 * 
 */

struct FAIRequestID;
class UBoxComponent;
class UCapsuleComponent;

namespace EPathFollowingResult
{
	enum Type : int;
}

class AAIController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskChargeDelegate, AActor*, ActorsHit);

UCLASS()
class TINYTAVERNGAME_API UAbilityTask_Charge : public UAbilityTask
{
	GENERATED_BODY()

public:

	UAbilityTask_Charge(const FObjectInitializer& ObjectInitializer);
	/** 
 * Start Calculate the endpoint for the charge attack
 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
 *
 * @param TaskInstanceName Set to override the name of this task, for later querying
 * @param Position Location of the charge
 * @param MoveActors Displace Actors on his way during charge
 * @param IgnoreActors Ignore Actors to displace during charge
 * @param DestroyActors Destroy Actors on his way during charge
 * @param ActorsToDestroy Ignore Actors to destroy during charge
 * @param ActorsToDestroy Ignore Actors to destroy during charge


*/
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="ChargeAndWait",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_Charge* ChargeAbilityTask(UGameplayAbility* OwningAbility,
		FName TaskInstanceName,FVector TargetPoint,bool ApplyHitEffects, bool MoveActors,float Radius,bool ShowDebug, float Force, TArray<TSubclassOf<AActor>> IgnoreActors, bool DestroyActors, TArray<TSubclassOf<AActor>> ActorsToDestroy);

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskChargeDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskChargeDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskChargeDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskChargeDelegate	OnHitActor;

	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	FVector TargetPoint;

	UPROPERTY()
	bool ApplyHitEffects;
	
	UPROPERTY()
	bool MoveActors;

	UPROPERTY()
	float Radius;
	
	UPROPERTY()
	TArray<TSubclassOf<AActor>> IgnoreActors;

	UPROPERTY()
	float Force = 1000;

	UPROPERTY()
	bool DestroyActors;

	UPROPERTY()
	bool ShowDebug;
	
	UPROPERTY()
	TArray<TSubclassOf<AActor>> ActorsToDestroy;

	UPROPERTY()
	TObjectPtr<AActor> Collision;
	////////////////NO SE UTILIZAN/////////////////////
	UPROPERTY()
	bool bStopWhenAbilityEnds;

	TObjectPtr<AAIController> AIController;

	float AccelerationPercentage;
	float DecelerationPercentage;

	TEnumAsByte<ECollisionChannel> CollisionChannel;

	
	//Help vars
	FVector StartPos;
	FVector EndPos;
	////////////////////////////////////////////////////
private:
	TObjectPtr<AActor> Charger;
	TArray<AActor*> ActorsInMovement;
	TArray<AActor*> IgnoreSelf ;
public:
	virtual void TickTask(float DeltaTime) override;
	
	virtual void Activate() override;
	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();
	
	//void ApplyStunEffect(float StunTime, AActor* Actor, TSubclassOf<UGameplayEffect> GameplayEffect);
	void ApplyDamageEffect(AActor* Actor, TSubclassOf<UGameplayEffect> GameplayEffect);
	
	void Charge();

	UFUNCTION()
	void ChargeCompleted( FAIRequestID  RequestID, EPathFollowingResult::Type  Result);

	UFUNCTION()
	void OnSweepBegin();
};


