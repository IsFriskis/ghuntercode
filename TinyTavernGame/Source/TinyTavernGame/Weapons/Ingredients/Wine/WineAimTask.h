// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WineAimTask.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UWineAimTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UWineAimTask(const FObjectInitializer& ObjectInitializer);
	
	static TObjectPtr<UWineAimTask> ExecuteWineAimTask(UGameplayAbility* OwningAbility, TObjectPtr<APlayerCharacter> _Player, float _WineRadius, TObjectPtr<UAnimMontage> HoldAnim, TObjectPtr<UStaticMesh> _WineIndicatorMesh);
	
protected:
	virtual void TickTask(float DeltaTime) override;
	
private:
	TObjectPtr<APlayerCharacter> Player;
	float WineRadius;
};
