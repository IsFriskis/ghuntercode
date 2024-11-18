// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTRadialSlider.generated.h"

class URadialSlider;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTTRadialSlider : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetSelectionZone(float ZoneSize, float Offset);
	void RunSuccessOrFailureAnimation(bool Success, bool SecondError, FWidgetAnimationDynamicEvent Event);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URadialSlider> SelectionZoneSlider;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SuccessAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FailureAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SecondFailureAnimation;
};
