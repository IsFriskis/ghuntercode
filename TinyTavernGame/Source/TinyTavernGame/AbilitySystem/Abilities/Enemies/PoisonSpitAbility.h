// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "PlayAnimationAbility.h"
#include "PoisonSpitAbility.generated.h"

class APoisonSpitProjectile;
class APlayerCharacter;
class AEnemyBase;
class UTTAnimNotify;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UPoisonSpitAbility : public UPlayAnimationAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	UPROPERTY(EditAnywhere, Category = "Poison Spit Info")
	TSubclassOf<APoisonSpitProjectile> PoisonSpitProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Poison Spit Info")
	float ProjectileMaxDistance = 800.0f;
	UPROPERTY(EditAnywhere, Category = "Poison Spit Info")
	float ProjectileSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Poison Spit Info")
	float ProjectileRadius = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Poison Spit Info")
	float ProjectileExplosionRadius = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Poison Spit Info")
	TSubclassOf<UGameplayEffect> DamageEffect;
	//TODO Change Spawn Location of Projectile with a Socket in Enemy
	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UTTAnimNotify> PoisonAnimNotify;
	
	//When Animation says, spit projectile
	UFUNCTION()
	void ExecuteSpit();
};