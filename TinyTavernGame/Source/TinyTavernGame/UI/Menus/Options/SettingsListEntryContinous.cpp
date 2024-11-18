// TinyTavern


#include "SettingsListEntryContinous.h"

#include "Components/ProgressBar.h"
#include "Components/Slider.h"

void USettingsListEntryContinous::NativeConstruct()
{
	Super::NativeConstruct();

	SettingSlider.Get()->OnValueChanged.AddDynamic(this, &USettingsListEntryContinous::SliderChanged);
	SettingsNormalProgressBarFillStyle = SettingProgressBar.Get()->GetWidgetStyle();
	SettingsHoverProgressBarFillStyle = SettingProgressBar.Get()->GetWidgetStyle();
	SettingsHoverProgressBarFillStyle.BackgroundImage = HoveredSliderImage;
}

void USettingsListEntryContinous::NativeDestruct()
{
	Super::NativeDestruct();
	SettingSlider.Get()->OnValueChanged.RemoveAll(this);
}

void USettingsListEntryContinous::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	SettingProgressBar.Get()->SetWidgetStyle(SettingsHoverProgressBarFillStyle);
}

void USettingsListEntryContinous::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SettingProgressBar.Get()->SetWidgetStyle(SettingsNormalProgressBarFillStyle);
}

void USettingsListEntryContinous::SetSetting(FText ContentText, TObjectPtr<UTT_UserSettings> _TTUserSettings,
                                             float _CurrentSettingValue, int _SettingMaxValue, TArray<FText> _Options)
{
	Super::SetSetting(ContentText, _TTUserSettings, _CurrentSettingValue, _SettingMaxValue, _Options);
	SettingSlider.Get()->SetMaxValue(SettingMaxValue);
	SettingSlider.Get()->SetValue(CurrentSettingValue);
	SettingProgressBar.Get()->SetPercent(CurrentSettingValue / SettingSlider.Get()->GetMaxValue());
}

TObjectPtr<UWidget> USettingsListEntryContinous::GetSlider()
{
	return SettingSlider.Get();
}

void USettingsListEntryContinous::SliderChanged(float Value)
{
	SettingProgressBar.Get()->SetPercent(Value / SettingSlider.Get()->GetMaxValue());
	OnChangeSetting.Broadcast(Value);
}
