// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SettingsListEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeSetting, float, Value);
class UCommonTextBlock;
class UTT_UserSettings;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USettingsListEntry : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	FChangeSetting OnChangeSetting;
	virtual void SetSetting(FText ContentText, TObjectPtr<UTT_UserSettings> _TTUserSettings, float _CurrentSettingValue, int _SettingMaxValue, TArray<FText> _Options);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> ContentTextBlock;

	TObjectPtr<UTT_UserSettings> TTUserSettings;
	float CurrentSettingValue = 0;
	int SettingMaxValue = 0;
	TArray<FText> Options;
};
