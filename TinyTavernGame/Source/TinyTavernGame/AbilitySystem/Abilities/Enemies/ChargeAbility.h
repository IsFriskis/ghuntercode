// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AI/Navigation/NavigationTypes.h"
#include "ChargeAbility.generated.h"

class AEnemyAAIController;

//TODO: Meterle plomo al Gonzalo.
USTRUCT()
struct FExecuteChargeParams
{
	GENERATED_BODY()

	UPROPERTY()
	UGameplayAbility* OwningAbility;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	AEnemyAAIController* EnemyController;

	float AimTime;
	float Distance;
	float RecoveryTime;
	float OriginalSpeed;
	float MaxSpeed;
	float AccelerationPercentage;
	float DecelerationPercentage;
	float HitboxRangeX;
	float HitboxRangeY;
	float StunTime;
	FVector TargetLocation;
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	TSubclassOf<UGameplayEffect> StunEffect;
	TSubclassOf<UGameplayEffect> DamageEffect;
	TSubclassOf<UGameplayEffect> InCombatEffect;
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UChargeAbility : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	void SetMontagePlayRate(FName SectionName, float PlayRate);

	UFUNCTION()
	void OnAimCompleted(FExecuteChargeParams& ChargeParams);

	UFUNCTION()
	void OnChargeCompleted(FExecuteChargeParams& ChargeParams);

	UFUNCTION()
	void OnRecoveryCompleted();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info",  meta = (Tooltip = "Time it takes for the enemy to lock onto the player"))
	float AimTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info", meta = (Tooltip = "Distance the enemy charges"))
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info", meta = (Tooltip = "Time it takes for the enemy to recover after charging"))
	float RecoveryTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info",  meta = (Tooltip = "Of total distance enemy will travel, how much of it will be accelerating - Value between 0 and 1, must be lower than DecelerationPercentage"))
	float AccelerationPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info",  meta = (Tooltip = "Of total distance enemy will travel, how much of it will be deaccelerating - Value between 0 and 1, must be higher than AccelerationPercentage"))
	float DecelerationPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info")
	TSubclassOf<UGameplayEffect> StunEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Charge Info")
	TSubclassOf<UGameplayEffect> InCombatEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Info")
	float HitboxRangeX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Info")
	float HitboxRangeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Info")
	float StunTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Info")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation Info")
	TObjectPtr<UAnimMontage> ChargeMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT|Animation Info")
	FName StartSectionName = "Aim";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT|Animation Info")
	FName LoopSectionName = "Charge";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT|Animation Info")
	FName EndSectionName = "Recovery";

	float OriginalSpeed;
};





