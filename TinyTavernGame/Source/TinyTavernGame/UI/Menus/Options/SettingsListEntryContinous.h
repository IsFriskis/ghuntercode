// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "SettingsListEntry.h"
#include "SettingsListEntryContinous.generated.h"

class USlider;
class UProgressBar;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USettingsListEntryContinous : public USettingsListEntry
{
	GENERATED_BODY()

public:
	virtual void SetSetting(FText ContentText, TObjectPtr<UTT_UserSettings> _TTUserSettings, float _CurrentSettingValue, int _SettingMaxValue, TArray<FText> _Options) override;
	TObjectPtr<UWidget> GetSlider();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> SettingProgressBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> SettingSlider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Settings")
	FSlateBrush HoveredSliderImage;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
private:
	FProgressBarStyle SettingsNormalProgressBarFillStyle;
	FProgressBarStyle SettingsHoverProgressBarFillStyle;
	
	UFUNCTION()
	void SliderChanged(float Value);
};
