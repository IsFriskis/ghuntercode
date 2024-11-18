// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "JamSlamAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "JamSlamAimTask.generated.h"

class ATTPlayerController;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UJamSlamAimTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UJamSlamAimTask(const FObjectInitializer& ObjectInitializer);
	
	static TObjectPtr<UJamSlamAimTask> ExecuteJamSlamAimTask(UGameplayAbility* OwningAbility, TObjectPtr<APlayerCharacter> _Player, float _PetrificationSpawnMaxRadius, float
	                                                         _PetrificationSpawnMinRadius, float _PetrificationAreaRadius, TObjectPtr<UAnimMontage> HoldAnim,
	                                                         TObjectPtr<UStaticMesh> _JamIndicatorMesh, float _AimSmoothFactor);
	
protected:
	virtual void TickTask(float DeltaTime) override;

	void AimWithMouse();
	void AimWithController();
	void NotAiming();
	FVector CalculateNewPosition();

private:
	TObjectPtr<UJamSlamAbility> OwningAbility;
	TObjectPtr<APlayerCharacter> Player;
	TObjectPtr<ATTPlayerController> PlayerController;
	float PetrificationAreaRadius;
	float PetrificationSpawnMaxRadius;
	float PetrificationSpawnMinRadius;
	float LerpFactor = 0.15f;

	FVector PrevPos = FVector::ZeroVector;
	FVector2d PrevInput = FVector2d::ZeroVector;

	FVector NewPos;
	
};
