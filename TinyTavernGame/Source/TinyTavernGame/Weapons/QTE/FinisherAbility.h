// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "Animation/WidgetAnimation.h"
#include "FinisherAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinigameSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinigameFailure);

class UQTEOverlay;
class UInputMappingContext;
class UFinisherAbilityTask;
class UMinigameAbilityTask;
class AEnemyBase;
class APlayerCharacter;
class ATTPlayerController;
class UAbilityTask_PlayMontageAndWait;

enum class EEnemyType : uint8;

UENUM()
enum class EMinigameType : uint8
{
	None UMETA(DisplayName = "None"),
	Sequence UMETA(DisplayName = "Sequence Minigame"),
	Mashing UMETA(DisplayName = "Mashing Minigame"),
	Timing UMETA(DisplayName = "Timing Minigame")
};

USTRUCT(BlueprintType)
struct FMinigameProgression
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EEnemyType, float> EnemyProgressionMap;
};

USTRUCT()
struct FMinigameData : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FOnMinigameSuccess OnSuccessDelegate;

	UPROPERTY()
		FOnMinigameFailure OnFailureDelegate;

	UPROPERTY()
		AEnemyBase* Enemy;

	UPROPERTY()
		TObjectPtr<UQTEOverlay> QTEWidget;


public:
	// This is required for all child structs of FGameplayAbilityTargetData
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FMinigameData::StaticStruct();
	}
};


/**
 *
 */
UCLASS()
class TINYTAVERNGAME_API UFinisherAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnQTEButtonPressed(FGameplayTag CutType);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
		TSubclassOf<UQTEOverlay> QTEWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
		float TimeForBasicQTE = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
		float TimeDilationForActorsGlobal = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
		float TimeDilationForActorsInQTE = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
		float NewDishDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
		TSubclassOf<UGameplayEffect> InvulnerabilityEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation")
		TMap<FGameplayTag, UAnimMontage*> FinisherAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFX")
		TSubclassOf<AActor> BloodVFXFinisher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|VFX")
		TSubclassOf<AActor> BloodVFX;

private:
	UFUNCTION()
		void OnMinigameSucceeded();

	UFUNCTION()
		void OnMinigameFailed();

	UFUNCTION()
		void EndQTE();

	UFUNCTION()
		void OnSelectionTimeRanOut();

	UFUNCTION()
		void OnFinishingHit();

	UFUNCTION()
		void OnHit();

	UFUNCTION()
		void PlayQTEFailAnimation();

	UFUNCTION()
		void OnFacePlantFinished();

	UFUNCTION()
		void CallRecipeAbility();

	UFUNCTION()
		void PlayFinisherAnimation();

	UFUNCTION()
		void DisablePlayerInvulnerability();

	void StartQTE();
	void ApplyNormalTimeDilation();
	void ResetCustomTimeDilation();
	void PrepareMinigame();
	void CleanAnimNotifies();
	void HandleEnemyTagsForQTE();
	void ShowQTEWidget(ATTPlayerController* PlayerController);
	void HideQTEWidget();
	void ApplyTimeDilation(float TimeDilation);
	void ApplyPlayerInvulnerability();
	void ActivateEnemySkeletalMeshCollision(bool ShouldEnable);
	void DisablePlayerAndEnemyOverlay();

	UFUNCTION()
	void SaveEnemyRecipeData();



	const float OriginalGlobalTimeDilation = 1.0f;

	// Actors in QTE
	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<ATTPlayerController> PlayerController;

	TObjectPtr<UQTEOverlay>QTEWidgetInstance;
	TObjectPtr<UMinigameAbilityTask> MinigameTask;

	TObjectPtr<UAbilityTask_PlayMontageAndWait> FinisherMontageTask;

	bool MinigameSuccess = false;
	FGameplayTagContainer EnemyTagsInRecipe;
	FGameplayTag CutTypeSelected;
	unsigned int MeleeWeaponLevel = 0;
};
