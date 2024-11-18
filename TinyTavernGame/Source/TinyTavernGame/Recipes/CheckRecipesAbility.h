// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CheckRecipesAbility.generated.h"

class APlayerCharacter;
class UNewDishWidget;

USTRUCT()
struct FRecipeTagsPayload : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGameplayTagContainer EnemyTagsInRecipe;
	
	// This is required for all child structs of FGameplayAbilityTargetData
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FRecipeTagsPayload::StaticStruct();
	}
};


UCLASS()
class TINYTAVERNGAME_API UCheckRecipesAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool ReplicateEndAbility, bool WasCancelled) override;

	UPROPERTY(EditAnywhere, Category = "TT|Widgets")
	TSubclassOf<UNewDishWidget> NewDishWidgetClass;

private:
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	
	
};
