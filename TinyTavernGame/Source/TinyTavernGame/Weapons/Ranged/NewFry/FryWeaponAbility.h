// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FryWeaponAbility.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class APlayerCharacter;
class UFryHoldTask;
class UAbilityTask_WaitGameplayEvent;

USTRUCT(BlueprintType)
struct FFryWeaponTiersParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|MUST")
	float TimeHoldToBeActivated = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|MUST")
	float TimeHoldToBeActivatedWhenUpgraded = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Params")
	float SizeScaleMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Params")
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Params")
	float ExplosionRadiusMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Params")
	float SpeedMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Params")
	float TimeAliveMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Params")
	float CostMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFXParams")
	float RotationSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFXParams")
	float DistorsionBallLevelMultiplier = 1.0f;
	
	bool operator==(const FFryWeaponTiersParams& Other) const
	{
		return TimeHoldToBeActivated == Other.TimeHoldToBeActivated &&
				TimeHoldToBeActivatedWhenUpgraded == Other.TimeHoldToBeActivatedWhenUpgraded &&
			   SizeScaleMultiplier == Other.SizeScaleMultiplier &&
			   DamageMultiplier == Other.DamageMultiplier &&
			   ExplosionRadiusMultiplier == Other.ExplosionRadiusMultiplier &&
			   SpeedMultiplier == Other.SpeedMultiplier &&
			   TimeAliveMultiplier == Other.TimeAliveMultiplier &&
			   CostMultiplier == Other.CostMultiplier &&
			   	RotationSpeedMultiplier == Other.RotationSpeedMultiplier &&
			   		DistorsionBallLevelMultiplier == Other.DistorsionBallLevelMultiplier;
	}
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UFryWeaponAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	void SetHoldTime(float NewTime) { TimeHoldingInput = NewTime; }
	float GetHoldingTime() { return TimeHoldingInput; }
	
	int32 FetchAbilityTier();

	UPROPERTY()
	APlayerCharacter* PlayerRef = nullptr;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	UFUNCTION()
	void OnInputReleased(FGameplayEventData Payload);
	void ApplyCooldownEffect(float Duration);
	bool GetIsUpgraded() { return IsUpgraded; }

	void SpawnLogic(float HoldValue, float SpeedValue, float TimeAlive);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	float BaseSizeScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	float BaseExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	float BaseTimeAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	float BaseCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|BaseParams")
	float WineDamageMultiplier;
	
	UPROPERTY(EditAnywhere, Category = "TT|Tiers")
	TArray<FFryWeaponTiersParams> FryWeaponTiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Projectile")
	TSubclassOf<class AFryNewProjectile> FryProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Previsulaization")
	TObjectPtr<UStaticMesh> PrevisualizationRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Previsulaization")
	TObjectPtr<UStaticMesh> PrevisualizationExplossion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Effect")
	TSubclassOf<class UGameplayEffect> FryGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFX")
	TObjectPtr<class UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|SFX")
	TObjectPtr<class USoundWave> ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|SFX")
	TObjectPtr<class USoundBase> ChargeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|SFX")
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Effect")
	TSubclassOf<class UGameplayEffect> CooldownEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Anim")
	TObjectPtr<class UAnimMontage> FiringAnimation;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|AmmoCost")
	float AmmoCost; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFXParams")
	float DistorsionBallLevel = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFXParams")
	FVector InitialSize = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFXParams")
	float BaseRotationSpeed = 1.0f;
	

	UStaticMeshComponent* WeaponExplosionIndicatorStaticMesh = nullptr;
	
	AFryNewProjectile* NewFryProjectile = nullptr;
private:

	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEvent = nullptr;

	bool IsUpgraded = false;
	
	float TimeHoldingInput = 0.0f;

	UPROPERTY()
	TObjectPtr<UFryHoldTask> FryHoldTask = nullptr;
};
