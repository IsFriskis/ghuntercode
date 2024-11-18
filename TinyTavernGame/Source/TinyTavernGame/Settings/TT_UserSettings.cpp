// TinyTavern


#include "TT_UserSettings.h"

UTT_UserSettings::UTT_UserSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ColorBlindMode = EColorVisionDeficiency::NormalVision;
	ColorBlindStrength = 0;
	FPSShowned = 0;
	HideFPS = false;
	BrightnessValue = 11.0f;
	MasterVolume = 0.5f;
	MusicVolume = 1.0f;
	SFXVolume = 1.0f;
	MotionBlur = 0;
	Vibration = 1;
	InterpolationSpeed = 0.0f;
	GamepadSensitivityX = 1.0f;
	GamepadSensitivityY = 1.0f;
	FirstTimeDetectingSettings = false;
	FrameRateLimit = 60;
}

void UTT_UserSettings::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);
	if(!FirstTimeDetectingSettings)
	{
		RunHardwareBenchmark();
		ApplyHardwareBenchmarkResults();
		FirstTimeDetectingSettings = true;
		SaveBackUp();
		SaveSettings();
	}
}

void UTT_UserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
}

void UTT_UserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	FrameRateLimit = 60;
	FullscreenMode = 0;
}

void UTT_UserSettings::Reset()
{
	//Engine
	ScalabilityQuality.ResolutionQuality = BackUp_ResolutionQuality;
	ScalabilityQuality.ViewDistanceQuality = BackUp_ViewDistanceQuality;
	ScalabilityQuality.AntiAliasingQuality = BackUp_AntiAliasingQuality;
	ScalabilityQuality.ShadowQuality = BackUp_ShadowQuality;
	ScalabilityQuality.GlobalIlluminationQuality = BackUp_GlobalIlluminationQuality;
	ScalabilityQuality.ReflectionQuality = BackUp_ReflectionQuality;
	ScalabilityQuality.PostProcessQuality = BackUp_PostProcessQuality;
	ScalabilityQuality.TextureQuality = BackUp_TextureQuality;
	ScalabilityQuality.EffectsQuality = BackUp_EffectsQuality;
	ScalabilityQuality.FoliageQuality = BackUp_FoliageQuality;
	ScalabilityQuality.ShadingQuality = BackUp_ShadingQuality;
	ResolutionSizeX = BackUp_Resolution.X;
	ResolutionSizeY = BackUp_Resolution.Y;
	FullscreenMode = BackUp_FullScreenMode;
	FrameRateLimit = BackUp_FrameRateLimit;
	bUseVSync = BackUp_UseVSync;
	//Custom
	ColorBlindMode = BackUp_ColorBlindMode;
	ColorBlindStrength = BackUp_ColorBlindStrength;
	FPSShowned = BackUp_FPSShowned;
	HideFPS = BackUp_HideFPS;
	BrightnessValue = BackUp_BrightnessValue;
	MasterVolume = BackUp_MasterVolume;
	MusicVolume = BackUp_MusicVolume;
	SFXVolume = BackUp_SFXVolume;
	MotionBlur = BackUp_MotionBlur;
	Vibration = BackUp_Vibration;
	InterpolationSpeed = BackUp_InterpolationSpeed;
	GamepadSensitivityX = BackUp_GamepadSensitivityX;
	GamepadSensitivityY = BackUp_GamepadSensitivityY;
}

void UTT_UserSettings::SaveBackUp()
{
	//Engine
	BackUp_ResolutionQuality = ScalabilityQuality.ResolutionQuality;
	BackUp_ViewDistanceQuality = ScalabilityQuality.ViewDistanceQuality;
	BackUp_AntiAliasingQuality = ScalabilityQuality.AntiAliasingQuality;
	BackUp_ShadowQuality = ScalabilityQuality.ShadowQuality;
	BackUp_GlobalIlluminationQuality = ScalabilityQuality.GlobalIlluminationQuality;
	BackUp_ReflectionQuality = ScalabilityQuality.ReflectionQuality;
	BackUp_PostProcessQuality = ScalabilityQuality.PostProcessQuality;
	BackUp_TextureQuality = ScalabilityQuality.TextureQuality;
	BackUp_EffectsQuality = ScalabilityQuality.EffectsQuality;
	BackUp_FoliageQuality = ScalabilityQuality.FoliageQuality;
	BackUp_ShadingQuality = ScalabilityQuality.ShadingQuality;
	BackUp_Resolution.X = ResolutionSizeX;
	BackUp_Resolution.Y = ResolutionSizeY;
	BackUp_FullScreenMode = FullscreenMode;
	BackUp_FrameRateLimit = FrameRateLimit;
	BackUp_UseVSync = bUseVSync;
	//Custom
	BackUp_ColorBlindMode = ColorBlindMode;
	BackUp_ColorBlindStrength = ColorBlindStrength;
	BackUp_FPSShowned = FPSShowned;
	BackUp_HideFPS = HideFPS;
	BackUp_BrightnessValue = BrightnessValue;
	BackUp_MasterVolume = MasterVolume;
	BackUp_MusicVolume = MusicVolume;
	BackUp_SFXVolume = SFXVolume;
	BackUp_MotionBlur = MotionBlur;
	BackUp_Vibration = Vibration;
	BackUp_InterpolationSpeed = InterpolationSpeed;
	BackUp_GamepadSensitivityX = GamepadSensitivityX;
	BackUp_GamepadSensitivityY = GamepadSensitivityY;
}
