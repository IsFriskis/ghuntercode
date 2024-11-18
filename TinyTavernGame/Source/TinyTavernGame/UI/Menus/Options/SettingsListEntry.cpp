// TinyTavern


#include "SettingsListEntry.h"

#include "CommonTextBlock.h"

void USettingsListEntry::SetSetting(FText ContentText, TObjectPtr<UTT_UserSettings> _TTUserSettings,
	float _CurrentSettingValue, int _SettingMaxValue, TArray<FText> _Options)
{
	ContentTextBlock.Get()->SetText(ContentText);
	TTUserSettings = _TTUserSettings;
	CurrentSettingValue = _CurrentSettingValue;
	SettingMaxValue = _SettingMaxValue;
	Options = _Options;
}
