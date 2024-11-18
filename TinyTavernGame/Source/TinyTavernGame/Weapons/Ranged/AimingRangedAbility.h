// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AimingRangedAbility.generated.h"

USTRUCT(BlueprintType)
struct FIndicatorsInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PositionOfGrill = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleOfGrill = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PositionOfBoil = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleOfBoil = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PositionOfFry = 347.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleOfFry = 6.95f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PositionOfFryExplosion = 700.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleOfFryExplosion = 3.0f;
	
};

class UAimingRangeAbilityTask;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UAimingRangedAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "TT| Indicators")
	TObjectPtr<UStaticMesh> GrillIndicatorMesh;
	UPROPERTY(EditAnywhere, Category = "TT| Indicators")
	TObjectPtr<UStaticMesh> BoilIndicatorMesh;
	UPROPERTY(EditAnywhere, Category = "TT| Indicators")
	TObjectPtr<UStaticMesh> FryIndicatorMesh;
	UPROPERTY(EditAnywhere, Category = "TT| Indicators")
	TObjectPtr<UStaticMesh> FryExplosionIndicatorMesh;
	UPROPERTY(EditAnywhere, Category = "TT| Indicators")
	FIndicatorsInfo IndicatorsInfo;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<UAimingRangeAbilityTask> AimRangeTask;
};
