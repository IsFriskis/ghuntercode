// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_QuadCollisionCheck.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuadCollisionSignature, AActor*, Hits);

UCLASS()
class TINYTAVERNGAME_API UAbilityTask_QuadCollisionCheck : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FQuadCollisionSignature OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FQuadCollisionSignature OnHit;

	//FDelegateHandle InterruptedHandle;

	/**
	 * Start Calculate the endpoint for the charge attack
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param ColliderStartLocation Where the hitbox begins for this collision check
	 * @param Extents Range of the Ability
	 * @param AngleLimit The total angle of the cone. For example, 90 degrees means 45 degrees on each side.
	 * @param ColliderStartWidth Width of the base so the hitbox starts from the sides of the player
	 * @param IgnoreActors Type of actors to ignore
	 * @param ShowHitbox Enable hitboxes
 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "QuadCollisionCheck",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))

	static UAbilityTask_QuadCollisionCheck* QuadCollisionAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector ColliderStartLocation, float Extents, float AngleLimit, float ColliderStartWidth, TArray<TSubclassOf<AActor>> IgnoreActors, bool ShowHitbox);

	UAbilityTask_QuadCollisionCheck(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	bool ShowHitbox;

	UPROPERTY()
	float Extents;

	UPROPERTY()
	float AngleLimit;

	UPROPERTY()
	TArray<TSubclassOf<AActor>>IgnoreActors;

	UPROPERTY()
	FVector ColliderStartLocation;

	UPROPERTY()
	float ColliderStartWidth;

protected:
	virtual void Activate() override;
	

private:
	bool IsInsideBoxSweep();
	void FilterByAngle();
	void HandleEdgeCases();

	TArray<FHitResult> BoxHits;
	TArray<AActor*> ConfirmedHits;
	TArray<AActor*> PotentialEdgeHits;
	AActor* OwningActor;


};
