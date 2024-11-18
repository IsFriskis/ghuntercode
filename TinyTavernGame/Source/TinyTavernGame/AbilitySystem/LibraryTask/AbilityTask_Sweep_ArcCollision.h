// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Sweep_ArcCollision.generated.h"

class AEnemyBase;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskSweepArcCollisionsDelegate, AActor*, ActorHit);

UCLASS()
class TINYTAVERNGAME_API UAbilityTask_Sweep_ArcCollision : public UAbilityTask
{
	GENERATED_BODY()

	
public:
	
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskSweepArcCollisionsDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskSweepArcCollisionsDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskSweepArcCollisionsDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskSweepArcCollisionsDelegate	OnHit;

	UAbilityTask_Sweep_ArcCollision(const FObjectInitializer& ObjectInitializer);
	/** Callback function for when the owning Gameplay Ability is cancelled */

	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnGameplayAbilityCancelled();


	/** 
	 * Start Calculate the endpoint for the charge attack
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param SocketName Socket from which the wave is launched
	 * @param LeftAngle Angle in degrees to the left of the point of origin.
	 * @param RightAngle Angle in degrees to the Right of the point of origin.
	 * @param ArchWide width between the outer and inner arch
	 * @param Distance Distance travel until disappear
	 * @param Speed Wave movement speed
	 * @param DrawDebug Preview visualization
	 
	 

 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="SweepArcCollisions",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_Sweep_ArcCollision* SweepArcCollisions(UGameplayAbility* OwningAbility,
		FName TaskInstanceName,FName SocketName,float ArchWide,float LeftAngle, float RightAngle,float Distance,float Speed,float DelayFirstTime,bool DrawDebug);

	virtual void Activate() override;
	
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	bool bStopWhenAbilityEnds;	

	// Arc Params
	FVector StartLocation = FVector::Zero() ;
	FName SocketName;
	float ArchWide = 25.f;
	float LeftAngle = -15.f;
	float RightAngle = 15.f;
	float Distance = 300.f;
	float Speed = 25.f;
	float DelayFirstTime = 0.f;
	bool DrawDebug = false;

	//Internal Params

	float External_Radius = 0.f;
	float Internal_Radius = 0.f;
	TObjectPtr<AEnemyBase> Enemy;
	float timer = 0;
	bool FirstTime = false;

};
