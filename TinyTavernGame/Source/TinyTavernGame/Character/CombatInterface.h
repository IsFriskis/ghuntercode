// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TINYTAVERNGAME_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Die() =0;


	//HitReact Anim
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	 UAnimMontage* GetHitReactMontage();

	//Stun Anim
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetStunMontage();

	//Die Anim
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetDieMontage();
};
