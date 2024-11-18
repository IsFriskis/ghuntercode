// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "HoldButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoldFinishedSignature);

class UProgressBar;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UHoldButton : public UButtonBase
{
	GENERATED_BODY()


public:
	FOnHoldFinishedSignature OnHoldFinished;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> HoldInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HoldProgressBar;
	UFUNCTION()
	virtual void BeginHolding();
	UFUNCTION()
	virtual void EndHolding();
	
private:
	float MaxHoldTime = 1.0;
	FAlphaBlend HoldBlend;
	bool PlayerIsHolding = false;
	
	UFUNCTION()
	void HoldFinishedAction();
};
