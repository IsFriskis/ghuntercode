// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "BoilAbility.h"
#include "BoilAbilityTask.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfBoilAmmo);

class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBoilAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UBoilAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<UBoilAbilityTask> ExecuteTask(FBoilParams Params);
	void PrepareForDestroy();

	UPROPERTY(BlueprintAssignable)
	FOnOutOfBoilAmmo OnOutOfAmmo;

	bool MarkedEndTask = false;

protected:
	virtual void TickTask(float DeltaTime) override;

	FBoilParams Params;

private:
	void ApplyEffect(TSubclassOf<UGameplayEffect> Effect, float Magnitude, AActor* Actor, UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle EffectContextHandle);
	void ShowHitbox();
	bool IsInsideBoxSweep(TArray<FHitResult>& Hits);
	void FilterByAngle(TArray<FHitResult>& Hits);
	bool IsNotBlocked(const FHitResult& Hit);
	void HandleEdgeCases();

	void UpdateAttributes();
	void UpdateVFXParameters();
	
	
	float CurrentConcentration = 0;
	float CurrentAngle = 0;
	float CurrentDamage = 0;
	float CurrentRange = 0;
	float CurrentWidth = 0;
	float WineDamageMultiplier = 1.0f;

	TArray<AActor*> ConfirmedHits;
	TArray<AActor*> EdgeCases;
	
	TObjectPtr<UNiagaraComponent> VFXComponent;

	TArray<float> StartConeAngles = { 80, 80, 85, 80 };
	TArray<float> EndConeAngles = { 10, 10, 12, 10 };

	TArray<float> StartLifetime = { .21f, .21f, .21f, .21f };
	TArray<float> EndLifetime = { .25f, .25f, .25f, .25f };

	TArray<int> StartSpawnRate = { 2500, 60, 2500, 1200 };
	TArray<int> EndSpawnRate = { 1600, 600, 0, 200 };

	UGameplayEffect* OnHitVFX;
	TObjectPtr<APlayerCharacter> Player;
};
