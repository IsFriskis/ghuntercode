// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ButtonBase.h"
#include "SkipButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkipSignature);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USkipButton : public UButtonBase
{
	GENERATED_BODY()

public:
	FOnSkipSignature OnSkip;
	void InCollapsedState();
	void InvisibleSkipThatTurnsVisibleButDoesNotSkip();
	UFUNCTION(BlueprintCallable)
	void EnableSkipButton();
	
protected:
	//For Skipping
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle SkipInputActionData;
	FUIActionBindingHandle SkipHandle;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	bool IsCollapsed = false;
	UFUNCTION()
	void SkipCinematic();
};
