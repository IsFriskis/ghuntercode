// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "SettingsListEntry.h"
#include "TTRotator.h"
#include "SettingsListEntryDiscrete.generated.h"

class UTTRotator;
class UButtonBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USettingsListEntryDiscrete : public USettingsListEntry
{
	GENERATED_BODY()

public:
	virtual void SetSetting(FText ContentText, TObjectPtr<UTT_UserSettings> _TTUserSettings, float _CurrentSettingValue, int _SettingMaxValue, TArray<FText> _Options) override;
	TObjectPtr<UWidget> GetRotator();
	TObjectPtr<UWidget> GetButtonDecrease();
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ButtonDecrease;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTTRotator> SettingsRotator;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ButtonIncrease;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void OnDecreasedSetting();
	void OnIncreasedSetting();

	UFUNCTION()
	void HandleOnRotated(int32 Value);
};
