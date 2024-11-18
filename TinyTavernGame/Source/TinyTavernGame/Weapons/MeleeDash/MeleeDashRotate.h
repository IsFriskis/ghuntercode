// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "MeleeDashRotate.generated.h"

class UMeleeDashAbility;

DECLARE_MULTICAST_DELEGATE(FOnTaskEnded)
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMeleeDashRotate : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	UMeleeDashRotate();
	
	TObjectPtr<UMeleeDashRotate> ExecuteDashRotateTask(UGameplayAbility* OwningAbility);
	
	virtual void TickTask(float DeltaTime) override;

	void FinishTask();

	FOnTaskEnded OnTaskEnded;

protected:
	
	UPROPERTY()
	TObjectPtr<UMeleeDashAbility> MeleeDashAbility;
	
private:
	FAlphaBlend Blend;

	float TimeToRotate;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;

	FVector TargetVector;
};
