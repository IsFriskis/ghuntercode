// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "GameplayTaskOwnerInterface.h"
#include "FAlphaBlendTask.generated.h"



/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUAlphaBlendTask,float,value);

UCLASS()
class TINYTAVERNGAME_API UFAlphaBlendTask : public UGameplayTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FUAlphaBlendTask	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FUAlphaBlendTask	Values;

	UFAlphaBlendTask(const FObjectInitializer& ObjectInitializer);
	
	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();

	/** 
	 * Start Calculate the endpoint for the charge attack
	 * OnInterrupted and OnCancelled cancel previous animation and dont return nothing
	 * @param TaskOwner Owner of the Task
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param BlendTime Duration of Blend
	 * @param Reversed Reversed Values Start and End
	 * @param BlendOption Type of Blend
	 * @param StartValue Initial Value for Blend
	 * @param EndValue End Value for Blend
 */
	UFUNCTION(BlueprintCallable, Category="TT_Nodes", meta = (DisplayName="FAlpahBlend",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	
	static UFAlphaBlendTask* FAlphaBlendTask(FName TaskInstanceName,float BlendTime,bool Reversed, EAlphaBlendOption BlendOption, float StartValue, float EndValue  );

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;
	
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	float BlendTime = 1;

	UPROPERTY()
	bool Reversed = false;

	UPROPERTY()
	EAlphaBlendOption BlendOption = EAlphaBlendOption::Linear;

	UPROPERTY()
	float StartValue = 1;

	UPROPERTY()
	float EndValue = 0;

	FAlphaBlend AlphaBlend;
	float Timer = 0;
	
	
};
