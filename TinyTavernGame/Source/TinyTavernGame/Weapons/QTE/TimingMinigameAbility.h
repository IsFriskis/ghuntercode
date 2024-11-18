// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "MinigameAbility.h"
#include "TinyTavernGame/Enums/ETimeZoneStates.h"
#include "TimingMinigameAbility.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTimingMinigameAbility : public UMinigameAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
	TMap<FGameplayTag, int> AmountOfZones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> TimeWindowForInputPress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> TimeForFullCycle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
		float EarliestPossibleTiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
		float TimingMarginForError;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
		float MinGapBetweenZones;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	UFUNCTION()
	void OnSecondMiss();

	UFUNCTION()
	void OnAllZonesComplete();

	UFUNCTION()
	void OnCorrectTiming();

	void InitializeMinigameTimingZones();

	void ActivateMinigame();

private:
	TArray<FTimeZone> TimeZones;
	float EarliestPossibleTimingNormalized = 0;
	float TimeWindowForInputNormalized = 0;
	float MinGapBetweenZonesNormalized = 0;
	float TimingMarginForErrorNormalized = 0;
	const int MAX_AMOUNT_ZONES = 7;
};
