// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TTUserWidget.h"
#include "SpinningWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USpinningWidget : public UTTUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SpinningImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Spinning Settings")
	bool ShouldSpin = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Spinning Settings")
	float SpinningTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Spinning Settings")
	FAlphaBlend SpinningBlend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Spinning Settings")
	float SpinningMinValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Spinning Settings")
	float SpinningMaxValue = 360;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
