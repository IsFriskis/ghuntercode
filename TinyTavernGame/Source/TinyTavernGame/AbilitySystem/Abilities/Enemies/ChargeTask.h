// TinyTavern

#pragma once


#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "ChargeTask.generated.h"


UCLASS()
class TINYTAVERNGAME_API UChargeTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	
	UChargeTask(const FObjectInitializer& ObjectInitializer);
	virtual void TickTask(float DeltaTime) override;
	//static TObjectPtr<UChargeTask> ExecuteCharge(const FExecuteChargeParams& Params);
	void ApplyStunEffect(float _StunTime, AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect);
	void ApplyDamageEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect);
	void ApplyInCombatEffect(AActor* Actor, TSubclassOf<UGameplayEffect> _GameplayEffect);
	void SetChargeAim(FVector Direction);
	void Charge();



private:

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	UPROPERTY()
	TObjectPtr<AEnemyAAIController> EnemyController;

	//Vars to pass from ability
	float AimTime;
	float RecoveryTime;
	float SpeedBoost;
	float AccelerationPercentage;
	float DecelerationPercentage;
	TSubclassOf<UGameplayEffect> StunEffect;
	TSubclassOf<UGameplayEffect> DamageEffect;
	TSubclassOf<UGameplayEffect> InCombatEffect;
	
	//Vars for hitbox
	
	float HitboxRangeX;
	float HitboxRangeY;
	float StunTime;
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	//Help vars
	FVector StartPos;
	FVector EndPos;

	UPROPERTY()
	UTT_AttributeSet* EnemyAttributeSet;
};


