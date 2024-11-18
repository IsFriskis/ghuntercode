// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CheeseAimTask.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheeseAimTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UCheeseAimTask(const FObjectInitializer& ObjectInitializer);
	
	static TObjectPtr<UCheeseAimTask> ExecuteCheeseAimTask(UGameplayAbility* OwningAbility, TObjectPtr<APlayerCharacter> _Player, float _CheeseMineInnerRadius, float _CheeseMineOuterRadius, TObjectPtr<UAnimMontage> HoldAnim, float _MinePlacementDistanceFromPlayer, TObjectPtr<UStaticMesh> _CheeseIndicatorMesh);
	
protected:
	virtual void TickTask(float DeltaTime) override;
	
private:
	TObjectPtr<APlayerCharacter> Player;
	float CheeseMineInnerRadius;
	float CheeseMineOuterRadius;
	float MinePlacementDistanceFromPlayer;
};
