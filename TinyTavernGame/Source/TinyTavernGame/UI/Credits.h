// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Menus/TT_CommonActivatableWidget.h"
#include "Credits.generated.h"


class USkipButton;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreditsEnded);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCredits : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnCreditsEnded OnCreditsEnded;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USkipButton> SkipButton;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CreditsAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Sounds")
	TObjectPtr<USoundBase> CreditMusic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Sounds")
	bool ShouldHeadToMainMenu = false;
	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	TObjectPtr<UAudioComponent> MusicComponent;
	UFUNCTION()
	void BackLogic();
};
