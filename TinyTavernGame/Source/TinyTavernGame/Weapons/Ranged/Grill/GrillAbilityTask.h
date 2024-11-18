// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GrillAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GrillAbility.h"
#include "GrillAbilityTask.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfGrillAmmo);

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UGrillAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UGrillAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<UGrillAbilityTask> ExecuteTask(FGrillParams Params);

	UPROPERTY(BlueprintAssignable)
	FOnOutOfGrillAmmo OnOutOfAmmo;

	bool MarkedEndTask = false;

protected:
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
private:
	void ApplyEffect(TSubclassOf<UGameplayEffect> Effect, float Magnitude, AActor* Actor, UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle EffectContextHandle);
	void ShowHitbox();
	bool IsInsideBoxSweep(TArray<FHitResult>& Hits);
	bool IsInsideAngle(const FHitResult& Hit);
	bool IsNotBlocked(const FHitResult& Hit);
	void NewEnemyInDamage(TArray<FHitResult> Hits);

	FVector SocketLocation;
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	TArray<TObjectPtr<AActor>> EnemiesInDamage;
	TArray<TObjectPtr<AActor>> PreviousEnemiesInDamage;
	FGrillParams Params;


	
};
