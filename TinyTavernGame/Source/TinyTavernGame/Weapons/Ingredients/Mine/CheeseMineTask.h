// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CheeseMineTask.generated.h"

class UHud3D;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndCheeseMine);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheeseMineTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UCheeseMineTask(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable)
	FEndCheeseMine OnCheeseMinePlanted;
	
	static TObjectPtr<UCheeseMineTask> ExecuteMineTask(UGameplayAbility* OwningAbility, float _MinePlantTime, TObjectPtr<UHud3D> _PlayerHud3D);
	void CancelMine();
protected:
	virtual void TickTask(float DeltaTime) override;
private:
	float MinePlantTime;
	TObjectPtr<UHud3D> PlayerHud3D;

	float CurrentTime;
	FAlphaBlend MinePlantBlend;
};
