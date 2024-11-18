// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TaskGamePlayAbilityEnded.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskGamePlayAbilityEnded);
/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class TINYTAVERNGAME_API UTaskGamePlayAbilityEnded : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FTaskGamePlayAbilityEnded OnEnded;


	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UTaskGamePlayAbilityEnded* ListenForGamePlayAbilityEnded(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> abilityClass);


	UFUNCTION()
	void EndTask();
	
protected:

	UAbilitySystemComponent* ASC;
	FGameplayTagContainer TagStillApplied;
	TMap<FGameplayTag, FDelegateHandle> HandlesMap;

	UFUNCTION()
	virtual void OnCallback (const FGameplayTag CallbackTag, int32 NewCount);


	
};
