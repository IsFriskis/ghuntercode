// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "PlayAnimationAbility.h"
#include "PeckAbility.generated.h"

class APlayerCharacter;
class AEnemyBase;
class UTTAnimNotify;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UPeckAbility : public UPlayAnimationAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageRadius;
	
private:
	TObjectPtr<UTTAnimNotify> PeckAnimNotify;

	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<APlayerCharacter> Player;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> GE_Damage;
	
	UFUNCTION()
	void ExecutePeck();
};
