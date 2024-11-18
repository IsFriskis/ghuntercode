// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "TinyTavernGame/UI/Menus/ConfirmMenu.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "Options.generated.h"

class UHoldButton;
class UCommonActionWidget;
class UHorizontalBox;
class UControlListEntry;
class UCommonVisibilitySwitcher;
class UConfirmMenu;
class UImage;
class UTT_GameInstance;
class USettingsListEntryContinous;
class UButtonBase;
class USettingsListEntry;
class USettingsListEntryDiscrete;
class UVerticalBox;
class UTT_UserSettings;

USTRUCT(BlueprintType)
struct FControlInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ControlText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ControlImage;
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UOptions : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ApplyInputActionData;
	FUIActionBindingHandle ApplyHandle;
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ResetInputActionData;
	FUIActionBindingHandle ResetHandle;
	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly, Category = "TT")
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;

	void InTitleScreen();
	void SetIsInTavern(){IsInTavern = true;}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> RecipeBookReference;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> SettingsVideoBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> SettingsDisplayBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> SettingsAudioBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> SettingsGameplayBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonVisibilitySwitcher> SettingsSwitcher;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> VideoSettingsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> DisplaySettingsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> AudioSettingsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> GameplaySettingsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ControlsSettingsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ExitButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHoldButton> ApplyButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ResetButton;

	//Controls Settings
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> SettingsControlsBox1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> SettingsControlsBox2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TSubclassOf<USettingsListEntryDiscrete> SettingsListEntryDiscreteClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TSubclassOf<USettingsListEntryContinous> SettingsListEntryContinousClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TSubclassOf<UControlListEntry> ControlListEntryClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TArray<FControlInput> ControlsLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TArray<FControlInput> ControlsRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options Sound")
	TObjectPtr<USoundBase> ApplySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options Sound")
    TObjectPtr<USoundBase> ResetSound;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UConfirmMenu> OptionsConfirmMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText TitleConfirmMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText DescriptionConfirmMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FConfirmButtonOption ConfirmMenuOption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText TitleConfirmMenuInTavern;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText DescriptionConfirmMenuInTavern;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FConfirmButtonOption ConfirmMenuOptionInTavern;
	
	virtual void NativeConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeDestruct() override;
	void SetMainOptionsButtons();
	void EliminateSettings();

private:
	TObjectPtr<UTT_UserSettings> UserSettings;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TArray<TObjectPtr<USettingsListEntry>> SettingsList;
	TArray<TObjectPtr<UControlListEntry>> ControlsList;
	int HideFPS = false;
	bool IsInTitleScreen = false;
	bool IsInTavern = false;
	float PreviousMasterVolume = 0.5f;
	float PreviousMusicVolume = 0.5f;
	float PreviousSFXVolume = 0.5f;
	//Fixes naviagation for gamepad
	void EstablishNavigation();
	UFUNCTION()
	void ApplySettings();
	UFUNCTION()
	void ResetSettings();
	UFUNCTION()
	void ExitGame();
	UFUNCTION()
	void ShowVideoSettings();
	UFUNCTION()
	void ShowDisplaySettings();
	UFUNCTION()
	void ShowAudioSettings();
	UFUNCTION()
	void ShowGameplaySettings();
	UFUNCTION()
	void ShowControlsSettings();
	//--------------------------------------------------VIDEO----------------------------------------------------------------------
	void SetVideoOptions();
	UFUNCTION()
	void ChangeAntiAliasing(float Value);
	UFUNCTION()
	void ChangeFoliageQuality(float Value);
	UFUNCTION()
	void ChangePostProcessing(float Value);
	UFUNCTION()
	void ChangeShadingQuality(float Value);
	UFUNCTION()
	void ChangeShadowQuality(float Value);
	UFUNCTION()
	void ChangeTextureQuality(float Value);
	UFUNCTION()
	void ChangeVisualEffectQuality(float Value);
	UFUNCTION()
	void ChangeGlobalIlluminationQuality(float Value);
	UFUNCTION()
	void ChangeReflectionQuality(float Value);
	//--------------------------------------------------DISPLAY--------------------------------------------------------------------
	void SetDisplayOptions();
	UFUNCTION()
	void ChangeWindowMode(float Value);
	UFUNCTION()
	void ChangeVSync(float Value);
	UFUNCTION()
	void ChangeResolution(float Value);
	UFUNCTION()
	void ChangeFrameRateLimit(float Value);
	//--------------------------------------------------SOUND----------------------------------------------------------------------
	void SetAudioOptions();
	UFUNCTION()
	void ChangeMasterVolume(float Value);
	UFUNCTION()
	void ChangeMusicVolume(float Value);
	UFUNCTION()
	void ChangeSFXVolume(float Value);
	//--------------------------------------------------GAMEPLAY----------------------------------------------------------------------
	void SetGameplayOptions();
	UFUNCTION()
	void ChangeInterpolationSpeed(float Value);
	UFUNCTION()
	void ChangeGamepadSensitivityX(float Value);
	UFUNCTION()
	void ChangeGamepadSensitivityY(float Value);
	UFUNCTION()
	void ChangeVibration(float Value);
	UFUNCTION()
	void ChangeColorBlindMode(float Value);
	UFUNCTION()
	void ChangeColorBlindStrength(float Value);
	UFUNCTION()
	void ChangeBrightness(float Value);
	UFUNCTION()
	void ShowFPS(float Value);
	UFUNCTION()
	void ChangeMotionBlur(float Value);
	//--------------------------------------------------CONTROLS----------------------------------------------------------------------
	void SetControlsOptions();
};
