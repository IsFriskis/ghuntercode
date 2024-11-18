// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "FryWeaponAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FryHoldTask.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UFryHoldTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFryHoldTask(const FObjectInitializer& ObjectInitializer);

	virtual void TickTask(float DeltaTime) override;
	void TimerLogic(float DeltaTime);
	void PrevisualizationLogicWithTiers(int32 Tier);
	static TObjectPtr<UFryHoldTask> ExecuteFryHoldTask(UGameplayAbility* OwningAbility);
	float GetHoldTime() { return FryProjectileBlend.GetBlendedValue(); }
	float GetHoldSpeedTime() { return FryProjectileSpeedBlend.GetBlendedValue(); }
	float GetHoldTimeAlive() { return FryProjectileTimeAliveBlend.GetBlendedValue(); }

protected:
	UNiagaraComponent* NiagaraComponent;
	float Timer = 0.f;
	bool FirstExec = true;

private:
	UPROPERTY()
	TObjectPtr<UFryWeaponAbility> FryWeaponAbility;
	FAlphaBlend FryProjectileBlend;
	FAlphaBlend FryProjectileSpeedBlend;
	FAlphaBlend FryProjectileTimeAliveBlend;
	int MaxTierOfFry = 2;
};
