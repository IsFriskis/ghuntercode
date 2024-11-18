// TinyTavern


#include "SettingsListEntryDiscrete.h"


#include "TTRotator.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"

void USettingsListEntryDiscrete::NativeConstruct()
{
	Super::NativeConstruct();

	//Bindings
	ButtonDecrease.Get()->OnClicked().AddUObject(this, &USettingsListEntryDiscrete::OnDecreasedSetting);
	ButtonIncrease.Get()->OnClicked().AddUObject(this, &USettingsListEntryDiscrete::OnIncreasedSetting);
	//Setting Up Rotator
	SettingsRotator.Get()->PopulateTextLabels(Options);
	SettingsRotator.Get()->SetSelectedItem(CurrentSettingValue);
	SettingsRotator.Get()->OnRotated.AddDynamic(this, &USettingsListEntryDiscrete::HandleOnRotated);
}

void USettingsListEntryDiscrete::NativeDestruct()
{
	Super::NativeDestruct();
	ButtonDecrease.Get()->OnClicked().RemoveAll(this);
	ButtonIncrease.Get()->OnClicked().RemoveAll(this);
	SettingsRotator.Get()->OnRotated.RemoveAll(this);
}

void USettingsListEntryDiscrete::SetSetting(FText ContentText, TObjectPtr<UTT_UserSettings> _TTUserSettings,
	float _CurrentSettingValue, int _SettingMaxValue, TArray<FText> _Options)
{
	Super::SetSetting(ContentText, _TTUserSettings, _CurrentSettingValue, _SettingMaxValue, _Options);
	SettingsRotator.Get()->SetImages(SettingMaxValue+1, CurrentSettingValue);
}

TObjectPtr<UWidget> USettingsListEntryDiscrete::GetRotator()
{
	return SettingsRotator.Get();
}

TObjectPtr<UWidget> USettingsListEntryDiscrete::GetButtonDecrease()
{
	return ButtonDecrease;
}

void USettingsListEntryDiscrete::OnDecreasedSetting()
{
	--CurrentSettingValue;
	if(CurrentSettingValue < 0)
	{
		CurrentSettingValue = SettingMaxValue;
	}
	SettingsRotator.Get()->ShiftTextLeft();
	SettingsRotator.Get()->UpdateRotator(CurrentSettingValue);
	CurrentSettingValue = FMath::Clamp(CurrentSettingValue, 0, SettingMaxValue);
	OnChangeSetting.Broadcast(CurrentSettingValue);
}

void USettingsListEntryDiscrete::OnIncreasedSetting()
{
	++CurrentSettingValue;
	if(CurrentSettingValue > SettingMaxValue)
	{
		CurrentSettingValue = 0;
	}
	SettingsRotator.Get()->ShiftTextRight();
	SettingsRotator.Get()->UpdateRotator(CurrentSettingValue);
	CurrentSettingValue = FMath::Clamp(CurrentSettingValue, 0, SettingMaxValue);
	OnChangeSetting.Broadcast(CurrentSettingValue);
}

void USettingsListEntryDiscrete::HandleOnRotated(int32 Value)
{
	CurrentSettingValue = FMath::Clamp(Value, 0, SettingMaxValue);
	SettingsRotator.Get()->UpdateRotator(CurrentSettingValue);
}
