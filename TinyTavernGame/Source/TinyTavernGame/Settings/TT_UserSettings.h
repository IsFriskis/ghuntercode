// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TT_UserSettings.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTT_UserSettings : public UGameUserSettings
{
private:
	GENERATED_BODY()

public:
	UTT_UserSettings(const FObjectInitializer& ObjectInitializer);
	virtual void LoadSettings(bool bForceReload = false) override;
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	EColorVisionDeficiency GetColorBlindMode(){return ColorBlindMode;}
	void SetColorBlindMode(EColorVisionDeficiency Value){ColorBlindMode = Value;}
	
	int GetColorBlindStrength(){return ColorBlindStrength;}
	void SetColorBlindStrength(int Value){ColorBlindStrength = Value;}
	
	int ShouldShowFPS(){return FPSShowned;}
	void SetShowFPS(int Value){FPSShowned = Value;}
	
	float GetBrightnessValue(){return BrightnessValue;}
	void SetBrightness(float Value){BrightnessValue = Value;}
	
	float GetMasterVolume(){return MasterVolume;}
	void SetMasterVolume(float Value){MasterVolume = Value;}
	
	float GetMusicVolume(){return MusicVolume;}
	void SetMusicVolume(float Value){MusicVolume = Value;}
	
	float GetSFXVolume(){return SFXVolume;}
	void SetSFXVolume(float Value){SFXVolume = Value;}

	int GetMotionBlur() const{ return MotionBlur;}
	void SetMotionBlur(int _MotionBlur){ MotionBlur = _MotionBlur;}
	
	int GetVibration() const{ return Vibration;}
	void SetVibration(int _Vibration){ Vibration = _Vibration;}

	float GetInterpolationSpeed() const{ return InterpolationSpeed;}
	void SetInterpolationSpeed(float _InterpolationSpeed){ InterpolationSpeed = _InterpolationSpeed;}
	
	float GetGamepadSensitivityX() const{ return GamepadSensitivityX;}
	void SetGamepadSensitivityX(float _GamepadSensitivityX){ GamepadSensitivityX = _GamepadSensitivityX;}
	float GetGamepadSensitivityY() const{ return GamepadSensitivityY;}
	void SetGamepadSensitivityY(float _GamepadSensitivityY){ GamepadSensitivityY = _GamepadSensitivityY;}

	void SaveBackUp();
	void Reset();
	
	virtual void SetToDefaults() override;
	
protected:
	UPROPERTY(Config)
	int FirstTimeDetectingSettings = false;
	UPROPERTY(Config)
	EColorVisionDeficiency ColorBlindMode = EColorVisionDeficiency::NormalVision;
	UPROPERTY(Config)
	int ColorBlindStrength = 0;
	UPROPERTY(Config)
	int FPSShowned = 0;
	UPROPERTY(Config)
	int HideFPS = false;
	UPROPERTY(Config)
	float BrightnessValue = 11.0f;
	UPROPERTY(Config)
	float MasterVolume = 0.5f;
	UPROPERTY(Config)
	float MusicVolume = 1.0f;
	UPROPERTY(Config)
	float SFXVolume = 1.0f;
	UPROPERTY(Config)
	int MotionBlur = 0;
	UPROPERTY(Config)
	int Vibration = 1;
	UPROPERTY(Config)
	float InterpolationSpeed = 0.0f;
	UPROPERTY(Config)
	float GamepadSensitivityX = 1.0f;
	UPROPERTY(Config)
	float GamepadSensitivityY = 1.0f;

	//----------Backup for Reset----------------
	//Engine
	UPROPERTY(Config)
	float BackUp_ResolutionQuality;
	UPROPERTY(Config)
	int32 BackUp_ViewDistanceQuality;
	UPROPERTY(Config)
	int32 BackUp_AntiAliasingQuality;
	UPROPERTY(Config)
	int32 BackUp_ShadowQuality;
	UPROPERTY(Config)
	int32 BackUp_GlobalIlluminationQuality;
	UPROPERTY(Config)
	int32 BackUp_ReflectionQuality;
	UPROPERTY(Config)
	int32 BackUp_PostProcessQuality;
	UPROPERTY(Config)
	int32 BackUp_TextureQuality;
	UPROPERTY(Config)
	int32 BackUp_EffectsQuality;
	UPROPERTY(Config)
	int32 BackUp_FoliageQuality;
	UPROPERTY(Config)
	int32 BackUp_ShadingQuality;
	UPROPERTY(Config)
	FIntPoint BackUp_Resolution;
	UPROPERTY(Config)
	int32 BackUp_FullScreenMode;
	UPROPERTY(Config)
	float BackUp_FrameRateLimit;
	UPROPERTY(Config)
	bool BackUp_UseVSync;
	//Custom
	UPROPERTY(Config)
	EColorVisionDeficiency BackUp_ColorBlindMode = EColorVisionDeficiency::NormalVision;
	UPROPERTY(Config)
	int BackUp_ColorBlindStrength = 0;
	UPROPERTY(Config)
	int BackUp_FPSShowned = 0;
	UPROPERTY(Config)
	int BackUp_HideFPS = false;
	UPROPERTY(Config)
	float BackUp_BrightnessValue = 11.0f;
	UPROPERTY(Config)
	float BackUp_MasterVolume = 0.5f;
	UPROPERTY(Config)
	float BackUp_MusicVolume = 0.5f;
	UPROPERTY(Config)
	float BackUp_SFXVolume = 0.5f;
	UPROPERTY(Config)
	int BackUp_MotionBlur = 0;
	UPROPERTY(Config)
	int BackUp_Vibration = 1;
	UPROPERTY(Config)
	float BackUp_InterpolationSpeed = 0.0f;
	UPROPERTY(Config)
	float BackUp_GamepadSensitivityX = 1.0f;
	UPROPERTY(Config)
	float BackUp_GamepadSensitivityY = 1.0f;
};
