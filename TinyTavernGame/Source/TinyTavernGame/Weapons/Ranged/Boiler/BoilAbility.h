// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BoilAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UTT_AttributeSet;
class UBoilAbilityTask;
class APlayerCharacter;
class UNiagaraComponent;

USTRUCT()
struct FBoilParams
{
	GENERATED_BODY()

	UPROPERTY()
	UGameplayAbility* OwningAbility;

	UPROPERTY()
	AActor* OwningActor;

	UPROPERTY()
	TObjectPtr<UTT_AttributeSet> AttributeSet;

	float BaseAngle;
	float ConcentrationRate;
	float BaseWidth;
	float MinWidth;
	float BaseRange;
	float MaxRange;
	float BaseDamage;
	float MaxDamage;
	float CostPerSecond;
	float WineDamageMultiplier;
	
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY()
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_GameTraceChannel1;

	UPROPERTY()
	UGameplayEffect* OnHitVFX;
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBoilAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	UFUNCTION()
	void OnOutOfAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float HitboxAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float TimeToMaxConcentration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float TimeToMaxConcentrationUpgraded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float BaseWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float MinWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float BaseRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float BaseDamagePerHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float MaxDamagePerHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Tooltip = "Drunk effect damage multiplier"))
	float WineDamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	float CostPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	TSubclassOf<UGameplayEffect> OnHitVFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFX")
	float PeriodToGC;

private:
	TObjectPtr<UBoilAbilityTask> BoilAbilityTask;
	TObjectPtr<UTT_AttributeSet> PlayerAttributes;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UNiagaraComponent> VFXComponent;
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEvent;

	UFUNCTION()
	void FinishAbility(FGameplayEventData Payload);
	void EndVFXSpawnRate();
	bool AbilityMarkedForDeath = false;
};
