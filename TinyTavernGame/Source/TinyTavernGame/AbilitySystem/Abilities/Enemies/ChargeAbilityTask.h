// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ChargeAbility.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "../../../AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "../../../Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "ChargeAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitWall);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeComplete, FExecuteChargeParams&, ChargeParams);

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UChargeAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UChargeAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<UChargeAbilityTask> ExecuteCharge(const FExecuteChargeParams& Params);

	void ApplyStunEffect(float _StunTime, AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect);
	void ApplyDamageEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect);
	void ApplyInCombatEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect);

	UPROPERTY(BlueprintAssignable)
	FOnHitWall OnHitWall;

	UPROPERTY(BlueprintAssignable)
	FOnChargeComplete OnCompleted;

protected:
	virtual void TickTask(float DeltaTime) override;
	void DrawHitbox();

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
	FExecuteChargeParams Params;

	//Help vars
	FVector StartPos;
	FVector EndPos;

	UPROPERTY()
	UTT_AttributeSet* EnemyAttributeSet = nullptr;

	float ChargeTimer = 0;
};
