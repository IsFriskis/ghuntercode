// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "AimingRangedAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AimingRangeAbilityTask.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UAimingRangeAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAimingRangeAbilityTask(const FObjectInitializer& ObjectInitializer);
	
	static TObjectPtr<UAimingRangeAbilityTask> ExecuteAimRangeTask(UGameplayAbility* OwningAbility, TObjectPtr<APlayerCharacter> _Player,
		TObjectPtr<UStaticMesh> _GrillIndicatorMesh, TObjectPtr<UStaticMesh> _BoilIndicatorMesh,
		TObjectPtr<UStaticMesh> _FryIndicatorMesh, TObjectPtr<UStaticMesh> _FryExplosionIndicatorMesh,
		FIndicatorsInfo _IndicatorsInfo);
	void TickTask(float DeltaTime) override;
	
private:
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UStaticMesh> GrillIndicatorMesh;
	TObjectPtr<UStaticMesh> BoilIndicatorMesh;
	TObjectPtr<UStaticMesh> FryIndicatorMesh;
	TObjectPtr<UStaticMesh> FryExplosionIndicatorMesh;
	FIndicatorsInfo IndicatorsInfo;
};
