// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "MinigameAbilityTask.h"
#include "TinyTavernGame/Enums/ETimeZoneStates.h"
#include "TimingMinigameAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTimingZonesComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSecondMiss);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCorrectTiming);

USTRUCT()
struct FTimingMinigameParams
{
	GENERATED_BODY()

	UPROPERTY()
	UGameplayAbility* OwningAbility;

	UPROPERTY()
	UQTEOverlay* WidgetInstance;

	TArray<FTimeZone> TimeZones;

	float AmountOfZones;
	float EarliestPossibleTiming;
	float TimeWindowForInputPress;
	float TimeForFullCycle;
	float MinGapBetweenZones;
	float TimingMarginForError;
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTimingMinigameAbilityTask : public UMinigameAbilityTask
{
	GENERATED_BODY()
	
public:
	UTimingMinigameAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<UTimingMinigameAbilityTask> CreateMinigameTask(FTimingMinigameParams Params);

	UPROPERTY(BlueprintAssignable)
	FOnAllTimingZonesComplete OnAllZonesComplete;

	UPROPERTY(BlueprintAssignable)
	FOnSecondMiss OnSecondMiss;

	UPROPERTY(BlueprintAssignable)
	FOnCorrectTiming OnCorrectTiming;

protected:
	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnInputPressed(EButtonToPress Button = EButtonToPress::INVALID);


private:
	void UpdateCurrentTimeZone();
	bool IsPastCurrentTimingZone();
	void DisplayInputFeedback(bool Success, bool SecondError = false);
	bool IsInTimingWindow();
	bool CheckIfMinigameSuccess();
	bool ShouldReenableInput();

	TArray<FTimeZone> TimeZones;
	float AmountOfZones;
	float TimeForFullCycle;
	float EarliestPossibleTimingNormalized = 0;
	float TimeWindowForInputNormalized = 0;
	float MinGapBetweenZonesNormalized = 0;
	float TimingMarginForErrorNormalized = 0;

	int CurrentZoneIndex = 0;
	int CurrentCursorZone = 0;
	bool IsOnSecondCycle = false;

	// This value is from 0 to 1
	float CurrentCycleProgress = 0;
	bool CursorMovingForwards = true;

	float WorldTimeDilation = 0;
	float PlayerTimeDilation = 0;

	const int MAX_AMOUNT_ZONES = 7;
};
