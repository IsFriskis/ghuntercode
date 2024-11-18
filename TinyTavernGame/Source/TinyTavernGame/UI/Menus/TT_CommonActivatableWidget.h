// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "TT_CommonActivatableWidget.generated.h"

class ATTPlayerController;

UENUM(BlueprintType)
enum class FInputConfig: uint8
{
	All,
	Game,
	Menu
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTT_CommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| TT Common Activatable Widget")
	FInputConfig InputConfig = FInputConfig::All;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| TT Common Activatable Widget")
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::NoCapture;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnDeactivated() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

private:
	TObjectPtr<ATTPlayerController> PlayerController;
};
