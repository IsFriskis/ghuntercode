// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FinisherAbilityTask.generated.h"

UENUM()
enum class ECutType : uint8
{
	Cleaver UMETA(DisplayName = "Cleaver"),
	Tenderizer UMETA(DisplayName = "Tenderizer"),
	Skewer UMETA(DisplayName = "Skewer")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputPressed, FGameplayTag, CutType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeEnded);

class UQTEOverlay;

/**
 *
 */
UCLASS()
class TINYTAVERNGAME_API UFinisherAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFinisherAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<UFinisherAbilityTask> CreateFinisherTask(UGameplayAbility* OwningAbility, float TimeWindow, int WeaponLevel, UQTEOverlay* WidgetInstance);

	UFUNCTION()
	void OnCleaverPressed();

	UFUNCTION()
	void OnTenderizerPressed();

	UFUNCTION()
	void OnSkewerPressed();

	UPROPERTY(BlueprintAssignable)
	FOnInputPressed	OnPress;

	UPROPERTY(BlueprintAssignable)
	FOnTimeEnded OnTimeEnded;



protected:
	virtual void TickTask(float DeltaTime) override;

	TObjectPtr<UQTEOverlay> QTEWidget;

private: 
	void UnbindInputs();

	float QTEWindowDuration = 0;
	float TimeLeft = 0;
	int WeaponLevel = 0;
	bool MarkedEndTask;
};
