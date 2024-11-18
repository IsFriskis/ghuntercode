// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GrillAbility.generated.h"

/**
 * 
 */

class UAbilityTask_WaitGameplayEvent;
class APlayerCharacter;
class UTT_AttributeSet;
class UGrillAbilityTask;
class UNiagaraComponent;

USTRUCT()
struct FGrillParams
{
	GENERATED_BODY()

	UPROPERTY()
	UGameplayAbility* OwningAbility;

	UPROPERTY()
	AActor* OwningActor;

	UPROPERTY()
	TObjectPtr<UTT_AttributeSet> AttributeSet;

	float Angle;
	float Range;
	float Width;
	float WeaponDamagePerSecond;
	float DrunkDamageMultiplier;
	float CostPerSecond;
	float CooldownBetweenShots;
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY()
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_GameTraceChannel1;
	TObjectPtr<UStaticMesh> PrevisualizationRange;
};

UCLASS()
class TINYTAVERNGAME_API UGrillAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info", meta = (Tooltip = "Total angle for the hitbox"))
	float HitboxAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info", meta = (Tooltip = "How close the player has to be to the target to burn them"))
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info", meta = (Tooltip = "Width of the initial spread, this works best by using the gnome's width"))
	float Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info", meta = (Tooltip = "Drunk effect damage multiplier"))
	float DrunkDamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info", meta = (Tooltip = "Burn effect damage"))
	float BurnDamagePerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info", meta = (Tooltip = "Damage per second that the enemy gets hit (SEPARATE FROM BURN DAMAGE)"))
	float WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info")
	float CostPerShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Grill Info")
	float CooldownBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Effects")
	TSubclassOf<UGameplayEffect> CooldownEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Effects")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Effects")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation")
	UAnimMontage* FiringAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Previsulaization")
	TObjectPtr<UStaticMesh> PrevisualizationRange;
	
	UFUNCTION(BlueprintCallable)
	bool NoAmmunition(APlayerCharacter* _Player);
	UFUNCTION(BlueprintCallable)
	void ShowIndicator();
	
private:
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UNiagaraComponent> VFXComponent;
	TObjectPtr<UGrillAbilityTask> GrillAbilityTask;
	TObjectPtr<UTT_AttributeSet> PlayerAttributes;
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEvent;

	UFUNCTION()
	void FinishAbility(FGameplayEventData Payload);

	UFUNCTION(BlueprintCallable)
	void ApplyCooldownEffect(float Duration);
};
