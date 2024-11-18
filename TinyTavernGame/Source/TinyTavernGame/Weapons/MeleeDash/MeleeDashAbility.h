// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "MeleeDashRotate.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Weapons/MeleeDash/MeleeDashTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MeleeDashAbility.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMeleeDashAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Params")
	float HitboxWidth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Params")
	float HitboxHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Params")
	float HitboxLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Hitbox Params")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Smooth Rotation")
	float TimeToTurn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation Vars")
	TObjectPtr<UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation Vars")
	TObjectPtr<UAnimMontage> DashBackwardsMontage;
	
	UPROPERTY(BlueprintReadWrite, Category = "TT|Control Vars")
	TObjectPtr<APlayerCharacter> Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|AutoAim")
	float DistanceToCheck = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|AutoAim")
	float AngleDeviation = 1;


	FVector MakeAutoAimSweep(FVector _Direction);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	UFUNCTION()
	void OnAnimationEnd();

	UFUNCTION()
	void OnRotationEnd();

	UFUNCTION()
	void EndHitbox();
	
	UFUNCTION()
	void RemoveTags();

	UFUNCTION()
	void RemoveDelegates();

	UPROPERTY()
	TObjectPtr<UMeleeDashRotate> MeleeDashRotate;
	UPROPERTY()
	TObjectPtr<UMeleeDashTask> MeleeDashTask;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MeleeDashTaskAnim;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MeleeDashBackwardsTaskAnim;

	float EventMagnitude = 0;
private:

};
