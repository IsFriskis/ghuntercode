// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "ErraticDodgeAbility.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UErraticDodgeAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:

	ACharacter* Character;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditAnywhere, Category = "TT|Ability Config");
	float Distance;

	UPROPERTY(EditAnywhere, Category = "TT|Montages")
	TObjectPtr<UAnimMontage> ErraticDodgeMontage;

	UFUNCTION()
	void OnCompletedAnim();

	UFUNCTION()
	void OnEndTask();
	
};
