// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TT_CommonActivatableWidget.h"
#include "PoisonKingUI.generated.h"

class UButtonBase;
class AGetPoisonForKing;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UPoisonKingUI : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetPoisonForKingSphere(TObjectPtr<AGetPoisonForKing> _GetPoisonForKing);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EndEffects();
	
protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> AppearAnimation;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ObtainPoisonButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> DenyButton;
	
	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
private:
	TObjectPtr<AGetPoisonForKing> GetPoisonForKing;
	UFUNCTION()
	void GetPoison();
	UFUNCTION()
	void BackLogic();
};
