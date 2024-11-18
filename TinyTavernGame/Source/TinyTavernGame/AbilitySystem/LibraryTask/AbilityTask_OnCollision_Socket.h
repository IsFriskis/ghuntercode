// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_OnCollision_Socket.generated.h"

class UGameplayEffect;
class APlayerCharacter;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskOnCollisionSocketDelegate, AActor*, Actor);
UCLASS()
class TINYTAVERNGAME_API UAbilityTask_OnCollision_Socket : public UAbilityTask
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionSocketDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionSocketDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionSocketDelegate	OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskOnCollisionSocketDelegate	OnHit;
	
	UAbilityTask_OnCollision_Socket(const FObjectInitializer& ObjectInitializer);
	
	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();

	/** 
	 * OnCollisionSocketAbilityTask
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="OnCollisionSocket",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UAbilityTask_OnCollision_Socket* OnCollisionSocketAbilityTask(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, float ImpactRadius,bool DrawDebug, TArray<AActor*>IgnoreActors, float Timer, bool NeedTick, USkeletalMeshComponent* MeshComponent, FName StartSocketName, FName EndSocketName);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;
	
	FDelegateHandle InterruptedHandle;
	
	UPROPERTY()
	float ImpactRadius;

	UPROPERTY()
	FName StartSocketName;

	UPROPERTY()
	FName EndSocketName;

	UPROPERTY()
	bool DrawDebug;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	
	UPROPERTY()
	bool NeedTick;

	UPROPERTY()
	float Timer;

	float TotalTime;
	
	UPROPERTY()
	TObjectPtr<AActor> NullActorBroadcast;
	
	UPROPERTY()
	TArray<AActor*>IgnoreActors;
	
	UFUNCTION()
	void OnSweepBegin();
};
