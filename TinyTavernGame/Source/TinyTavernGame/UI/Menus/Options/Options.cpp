// TinyTavern


#include "Options.h"

#include "CommonActionWidget.h"
#include "CommonTextBlock.h"
#include "CommonVisibilitySwitcher.h"
#include "ControlListEntry.h"
#include "SettingsListEntryContinous.h"
#include "SettingsListEntryDiscrete.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/Settings/TT_UserSettings.h"
#include "TinyTavernGame/UI/Menus/ConfirmMenu.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"
#include "TinyTavernGame/UI/Menus/Buttons/HoldButton.h"

void UOptions::NativeConstruct()
{
	Super::NativeConstruct();
	
	UserSettings = Cast<UTT_UserSettings>(GEngine->GameUserSettings);
	UserSettings->LoadSettings();
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	//Add Settings
	SetVideoOptions();
	SetDisplayOptions();
	SetAudioOptions();
	SetGameplayOptions();
	SetControlsOptions();
	//Navigation
	EstablishNavigation();
	SetMainOptionsButtons();
	//Action Bar
	ApplyButton.Get()->OnHoldFinished.AddDynamic(this, &UOptions::ApplySettings);
	ResetButton.Get()->OnClicked().AddUObject(this, &UOptions::ResetSettings);

	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, false, FSimpleDelegate::CreateUObject(this, &UOptions::ApplySettings)));
	ResetHandle = RegisterUIActionBinding(FBindUIActionArgs(ResetInputActionData, false, FSimpleDelegate::CreateUObject(this, &UOptions::ResetSettings)));
	HideFPS = UserSettings.Get()->ShouldShowFPS();
	RecipeBookReference.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Save previous sounds
	PreviousMasterVolume = UserSettings.Get()->GetMasterVolume();
	PreviousMusicVolume = UserSettings.Get()->GetMusicVolume();
	PreviousSFXVolume = UserSettings.Get()->GetSFXVolume();
}

UWidget* UOptions::NativeGetDesiredFocusTarget() const
{
	return VideoSettingsButton;
}

void UOptions::NativeDestruct()
{
	Super::NativeDestruct();

	//Apply sound, either previous or changed
	//Sound
	UserSettings.Get()->SetMasterVolume(PreviousMasterVolume);
	UserSettings.Get()->SetMusicVolume(PreviousMusicVolume);
	UserSettings.Get()->SetSFXVolume(PreviousSFXVolume);
	TTGameInstance.Get()->ChangeMasterVolume();
	TTGameInstance.Get()->ChangeMusic();
	TTGameInstance.Get()->ChangeSFX();
	TTGameInstance.Get()->PushSound();
	EliminateSettings();
	ApplyHandle.Unregister();
	ResetHandle.Unregister();
	BackHandle.Unregister();
	ApplyButton.Get()->OnHoldFinished.RemoveAll(this);
	ResetButton.Get()->OnClicked().RemoveAll(this);
}

void UOptions::SetMainOptionsButtons()
{
	//Bind Changing buttons
	ExitButton.Get()->OnClicked().AddUObject(this, &UOptions::ExitGame);
	VideoSettingsButton.Get()->OnClicked().AddUObject(this, &UOptions::ShowVideoSettings);
	DisplaySettingsButton.Get()->OnClicked().AddUObject(this, &UOptions::ShowDisplaySettings);
	AudioSettingsButton.Get()->OnClicked().AddUObject(this, &UOptions::ShowAudioSettings);
	GameplaySettingsButton.Get()->OnClicked().AddUObject(this, &UOptions::ShowGameplaySettings);
	ControlsSettingsButton.Get()->OnClicked().AddUObject(this, &UOptions::ShowControlsSettings);
}

void UOptions::EliminateSettings()
{
	for(TObjectPtr<USettingsListEntry> setting : SettingsList)
	{
		setting.Get()->OnChangeSetting.RemoveAll(this);
	}
	ExitButton.Get()->OnClicked().RemoveAll(this);
	ControlsSettingsButton.Get()->OnClicked().RemoveAll(this);
	GameplaySettingsButton.Get()->OnClicked().RemoveAll(this);
	AudioSettingsButton.Get()->OnClicked().RemoveAll(this);
	VideoSettingsButton.Get()->OnClicked().RemoveAll(this);
	DisplaySettingsButton.Get()->OnClicked().RemoveAll(this);
	SettingsVideoBox.Get()->ClearChildren();
	SettingsDisplayBox.Get()->ClearChildren();
	SettingsAudioBox.Get()->ClearChildren();
	SettingsGameplayBox.Get()->ClearChildren();
	SettingsControlsBox1.Get()->ClearChildren();
	SettingsControlsBox2.Get()->ClearChildren();
	SettingsList.Empty();
	ControlsList.Empty();
}

void UOptions::EstablishNavigation()
{
	TObjectPtr<USettingsListEntryDiscrete> firstDiscreteSettingChild;
	TObjectPtr<USettingsListEntryContinous> firstContinuosSettingChild;
	switch (SettingsSwitcher.Get()->GetActiveWidgetIndex())
	{
	case 0:
			//Video
			firstDiscreteSettingChild = Cast<USettingsListEntryDiscrete>(SettingsVideoBox.Get()->GetChildAt(0));
			VideoSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			DisplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			AudioSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			GameplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			ControlsSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			ExitButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
		break;
	case 1:
			//Display
			firstDiscreteSettingChild = Cast<USettingsListEntryDiscrete>(SettingsDisplayBox.Get()->GetChildAt(0));
			VideoSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			DisplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			AudioSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			GameplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			ControlsSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
			ExitButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstDiscreteSettingChild.Get()->GetButtonDecrease());
		break;
	case 2:
			//Audio
			firstContinuosSettingChild = Cast<USettingsListEntryContinous>(SettingsAudioBox.Get()->GetChildAt(0));
			VideoSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			DisplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			AudioSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			GameplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			ControlsSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			ExitButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
		break;
	case 3:
			//Gameplay
			firstContinuosSettingChild = Cast<USettingsListEntryContinous>(SettingsGameplayBox.Get()->GetChildAt(0));
			VideoSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			DisplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			AudioSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			GameplaySettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			ControlsSettingsButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
			ExitButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right,firstContinuosSettingChild.Get()->GetSlider());
		break;
	}
}

void UOptions::ApplySettings()
{
	UserSettings.Get()->SaveSettings();
	TTGameInstance.Get()->ApplySettings();
	PlaySound(ApplySound);
	EliminateSettings();
	SetMainOptionsButtons();
	SetVideoOptions();
	SetDisplayOptions();
	SetAudioOptions();
	SetGameplayOptions();
	SetControlsOptions();
	EstablishNavigation();
	//Change previous audio settings
	PreviousMasterVolume = UserSettings.Get()->GetMasterVolume();
	PreviousMusicVolume = UserSettings.Get()->GetMusicVolume();
	PreviousSFXVolume = UserSettings.Get()->GetSFXVolume();
}

void UOptions::ResetSettings()
{
	// UserSettings.Get()->ResetToCurrentSettings();
	UserSettings.Get()->Reset();
	UserSettings.Get()->SaveSettings();
	TTGameInstance.Get()->ApplySettings();
	PlaySound(ResetSound);
	EliminateSettings();
	SetMainOptionsButtons();
	SetVideoOptions();
	SetDisplayOptions();
	SetAudioOptions();
	SetGameplayOptions();
	SetControlsOptions();
	EstablishNavigation();
}

void UOptions::InTitleScreen()
{
	IsInTitleScreen = true;
	RecipeBookReference.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ExitButton.Get()->SetTextOfButton(FText::FromString("Return"));
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UOptions::ExitGame)));
}

void UOptions::ExitGame()
{
	if(!IsInTitleScreen)
	{
		if(!IsInTavern)
		{
			//Recipe book Options on Adventure
			TObjectPtr<UConfirmMenu> confirmMenu = CreateWidget<UConfirmMenu>(GetWorld(), OptionsConfirmMenuWidget);
			confirmMenu.Get()->AddConfirmMenu(TitleConfirmMenu, DescriptionConfirmMenu, ConfirmMenuOption);
			confirmMenu.Get()->ActivateWidget();
			confirmMenu.Get()->AddToViewport(5);
		}
		else
		{
			//Recipe book Options on Tavern
			TObjectPtr<UConfirmMenu> confirmMenu = CreateWidget<UConfirmMenu>(GetWorld(), OptionsConfirmMenuWidget);
			confirmMenu.Get()->AddConfirmMenu(TitleConfirmMenuInTavern, DescriptionConfirmMenuInTavern, ConfirmMenuOptionInTavern);
			confirmMenu.Get()->ActivateWidget();
			confirmMenu.Get()->AddToViewport(5);
		}
	}
	else
	{
		BackHandle.Unregister();
		DeactivateWidget();
	}
}

void UOptions::ShowVideoSettings()
{
	SettingsSwitcher.Get()->SetActiveWidgetIndex(0);
	EstablishNavigation();
}

void UOptions::ShowDisplaySettings()
{
	SettingsSwitcher.Get()->SetActiveWidgetIndex(1);
	EstablishNavigation();
}

void UOptions::ShowAudioSettings()
{
	SettingsSwitcher.Get()->SetActiveWidgetIndex(2);
	EstablishNavigation();
}

void UOptions::ShowGameplaySettings()
{
	SettingsSwitcher.Get()->SetActiveWidgetIndex(3);
	EstablishNavigation();
}

void UOptions::ShowControlsSettings()
{
	SettingsSwitcher.Get()->SetActiveWidgetIndex(4);
	EstablishNavigation();
}

//-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------VIDEO----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void UOptions::SetVideoOptions()
{
	//--Texture--
	TObjectPtr<USettingsListEntryDiscrete> textureSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	textureSetting.Get()->SetSetting(FText::FromString("Textures"), UserSettings,  UserSettings.Get()->GetTextureQuality(),3,
									{
												FText::FromString("LOW"),
												FText::FromString("MEDIUM"),
												FText::FromString("HIGH"),
												FText::FromString("EPIC"),
									});
	textureSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeTextureQuality);
	SettingsVideoBox.Get()->AddChildToVerticalBox(textureSetting);
	SettingsList.Add(textureSetting);
	//--Shadow--
	TObjectPtr<USettingsListEntryDiscrete> shadowSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	shadowSetting.Get()->SetSetting(FText::FromString("Shadows"), UserSettings,  UserSettings.Get()->GetShadowQuality(),3,
									{
												FText::FromString("LOW"),
												FText::FromString("MEDIUM"),
												FText::FromString("HIGH"),
												FText::FromString("EPIC"),
									});
	shadowSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeShadowQuality);
	SettingsVideoBox.Get()->AddChildToVerticalBox(shadowSetting);
	SettingsList.Add(shadowSetting);
	//--Foliage--
	TObjectPtr<USettingsListEntryDiscrete> foliageSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	foliageSetting.Get()->SetSetting(FText::FromString("Foliage"), UserSettings,  UserSettings.Get()->GetFoliageQuality(),3,
									{
												FText::FromString("LOW"),
												FText::FromString("MEDIUM"),
												FText::FromString("HIGH"),
												FText::FromString("EPIC"),
									});
	foliageSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeFoliageQuality);
	SettingsVideoBox.Get()->AddChildToVerticalBox(foliageSetting);
	SettingsList.Add(foliageSetting);
	//--Shading--
	TObjectPtr<USettingsListEntryDiscrete> shadingSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	shadingSetting.Get()->SetSetting(FText::FromString("Shading"), UserSettings,  UserSettings.Get()->GetShadingQuality(),3,
									{
												FText::FromString("LOW"),
												FText::FromString("MEDIUM"),
												FText::FromString("HIGH"),
												FText::FromString("EPIC"),
									});
	shadingSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeShadingQuality);
	SettingsVideoBox.Get()->AddChildToVerticalBox(shadingSetting);
	SettingsList.Add(shadingSetting);
	//--Post Processing--
	TObjectPtr<USettingsListEntryDiscrete> postProcessingSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	postProcessingSetting.Get()->SetSetting(FText::FromString("Post Processing"), UserSettings,  UserSettings.Get()->GetPostProcessingQuality(),3,
									{
												FText::FromString("LOW"),
												FText::FromString("MEDIUM"),
												FText::FromString("HIGH"),
												FText::FromString("EPIC"),
									});
	postProcessingSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangePostProcessing);
	SettingsVideoBox.Get()->AddChildToVerticalBox(postProcessingSetting);
	SettingsList.Add(postProcessingSetting);
	//--Anti Aliasing--
	TObjectPtr<USettingsListEntryDiscrete> antiAliasSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	antiAliasSetting.Get()->SetSetting(FText::FromString("Anti Aliasing"), UserSettings,  UserSettings.Get()->GetAntiAliasingQuality(),4,
									{
												FText::FromString("OFF"),
												FText::FromString("FXAA"),
												FText::FromString("TAA"),
												FText::FromString("MSAA"),
												FText::FromString("TSR")
									});
	antiAliasSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeAntiAliasing);
	SettingsVideoBox.Get()->AddChildToVerticalBox(antiAliasSetting);
	SettingsList.Add(antiAliasSetting);
	//--Visual Effects--
	TObjectPtr<USettingsListEntryDiscrete> visualEffectsSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	visualEffectsSetting.Get()->SetSetting(FText::FromString("Visual Effects"), UserSettings,  UserSettings.Get()->GetVisualEffectQuality(),3,
									{
												FText::FromString("LOW"),
												FText::FromString("MEDIUM"),
												FText::FromString("HIGH"),
												FText::FromString("EPIC"),
									});
	visualEffectsSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeVisualEffectQuality);
	SettingsVideoBox.Get()->AddChildToVerticalBox(visualEffectsSetting);
	SettingsList.Add(visualEffectsSetting);
	//--GlobalIlumination--
	TObjectPtr<USettingsListEntryDiscrete> globalIluminationSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	globalIluminationSetting.Get()->SetSetting(FText::FromString("Global Illumination"), UserSettings,  UserSettings.Get()->GetGlobalIlluminationQuality(),3,
									{
												FText::FromString("OFF"),
												FText::FromString("AMBIENT OCCLUSION"),
												FText::FromString("LUMEN HIGH"),
												FText::FromString("LUMEN EPIC"),
									});
	globalIluminationSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeGlobalIlluminationQuality);
	SettingsVideoBox.Get()->AddChildToVerticalBox(globalIluminationSetting);
	SettingsList.Add(globalIluminationSetting);
	//--Reflections--
	TObjectPtr<USettingsListEntryDiscrete> reflectionSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	reflectionSetting.Get()->SetSetting(FText::FromString("Reflections"), UserSettings,  UserSettings.Get()->GetReflectionQuality(),3,
									{
												FText::FromString("OFF"),
												FText::FromString("SCREEN SPACE"),
												FText::FromString("LUMEN HIGH"),
												FText::FromString("LUMEN EPIC"),
									});
	reflectionSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeReflectionQuality);
	SettingsVideoBox.Get()->AddChildToVerticalBox(reflectionSetting);
	SettingsList.Add(reflectionSetting);
}

void UOptions::ChangeAntiAliasing(float Value)
{
	UserSettings.Get()->SetAntiAliasingQuality(Value);
}

void UOptions::ChangeFoliageQuality(float Value)
{
	UserSettings.Get()->SetFoliageQuality(Value);
}

void UOptions::ChangePostProcessing(float Value)
{
	UserSettings.Get()->SetPostProcessingQuality(Value);
}

void UOptions::ChangeShadingQuality(float Value)
{
	UserSettings.Get()->SetShadingQuality(Value);
}

void UOptions::ChangeShadowQuality(float Value)
{
	UserSettings.Get()->SetShadowQuality(Value);
}

void UOptions::ChangeTextureQuality(float Value)
{
	UserSettings.Get()->SetTextureQuality(Value);
}

void UOptions::ChangeVisualEffectQuality(float Value)
{
	UserSettings.Get()->SetVisualEffectQuality(Value);
}

void UOptions::ChangeGlobalIlluminationQuality(float Value)
{
	UserSettings.Get()->SetGlobalIlluminationQuality(Value);
}

void UOptions::ChangeReflectionQuality(float Value)
{
	UserSettings.Get()->SetReflectionQuality(Value);
}

//-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------DISPLAY----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

void UOptions::SetDisplayOptions()
{
	//--Window Mode--
	TObjectPtr<USettingsListEntryDiscrete> windowModeSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	windowModeSetting.Get()->SetSetting(FText::FromString("Display Mode"), UserSettings,  UKismetMathLibrary::Conv_ByteToInt(UserSettings.Get()->GetFullscreenMode()),2,
									{
												FText::FromString("FullScreen"),
												FText::FromString("Windowed Fullscreen"),
												FText::FromString("Windowed")
									});
	windowModeSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeWindowMode);
	SettingsDisplayBox.Get()->AddChildToVerticalBox(windowModeSetting);
	SettingsList.Add(windowModeSetting);
	//--Resolution--
	TObjectPtr<USettingsListEntryDiscrete> resolutionSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	int currentResolutionIndex = 0;
	switch(UserSettings.Get()->GetScreenResolution().X)
	{
	case 1280: currentResolutionIndex = 0;
		break;
	case 1600: currentResolutionIndex = 1;
		break;
	case 1920: currentResolutionIndex = 2;
		break;
	case 2560: currentResolutionIndex = 3;
		break;
	case 3840: currentResolutionIndex = 4;
		break;
	default:
		break;
	}
	resolutionSetting.Get()->SetSetting(FText::FromString("Resolution"), UserSettings,  currentResolutionIndex,4,
									{
												FText::FromString("1280x720"),
												FText::FromString("1600x900"),
												FText::FromString("1920x1080"),
												FText::FromString("2560x1440"),
												FText::FromString("3840x2160")
									});
	resolutionSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeResolution);
	SettingsDisplayBox.Get()->AddChildToVerticalBox(resolutionSetting);
	SettingsList.Add(resolutionSetting);
	//--FrameRateLimit--
	TObjectPtr<USettingsListEntryDiscrete> frameRateLimitSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	int currentFrameRateLimitIndex = 0;
	if(UserSettings.Get()->GetFrameRateLimit() == 30)
	{
		currentFrameRateLimitIndex = 0;
	}
	else if(UserSettings.Get()->GetFrameRateLimit() == 60)
	{
		currentFrameRateLimitIndex = 1;
	}
	else if(UserSettings.Get()->GetFrameRateLimit() == 120)
	{
		currentFrameRateLimitIndex = 2;
	}
	else if(UserSettings.Get()->GetFrameRateLimit() == 240)
	{	currentFrameRateLimitIndex = 3;
	}
	else
	{
		currentFrameRateLimitIndex = 4; //Unlimited
	}
	frameRateLimitSetting.Get()->SetSetting(FText::FromString("Frame Rate Limit"), UserSettings, currentFrameRateLimitIndex,4,
									{
												FText::FromString("30"),
												FText::FromString("60"),
												FText::FromString("120"),
												FText::FromString("240"),
												FText::FromString("UNLIMITED")
									});
	frameRateLimitSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeFrameRateLimit);
	SettingsDisplayBox.Get()->AddChildToVerticalBox(frameRateLimitSetting);
	SettingsList.Add(frameRateLimitSetting);
	//--VSYNC--
	TObjectPtr<USettingsListEntryDiscrete> vSyncSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	vSyncSetting.Get()->SetSetting(FText::FromString("VSync"), UserSettings,  UserSettings.Get()->IsVSyncEnabled(),1,
									{
												FText::FromString("OFF"),
												FText::FromString("ON"),
									});
	vSyncSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeVSync);
	SettingsDisplayBox.Get()->AddChildToVerticalBox(vSyncSetting);
	SettingsList.Add(vSyncSetting);
}

void UOptions::ChangeWindowMode(float Value)
{
	UserSettings.Get()->SetFullscreenMode(EWindowMode::ConvertIntToWindowMode(Value));
}

void UOptions::ChangeVSync(float Value)
{
	bool newValue = Value ? 1 : 0;
	UserSettings.Get()->SetVSyncEnabled(newValue);
}

void UOptions::ChangeResolution(float Value)
{
	FIntPoint resolution;
	resolution.X = 1920; resolution.Y = 1080;
	switch (FMath::FloorToInt(Value)){
	case 0:
		resolution.X = 1280; resolution.Y = 720;
		break;
	case 1:
		resolution.X = 1600; resolution.Y = 900;
		break;
	case 2:
		resolution.X = 1920; resolution.Y = 1080;
		break;
	case 3:
		resolution.X = 2560; resolution.Y = 1440;
		break;
	case 4:
		resolution.X = 3840; resolution.Y = 2160;
		break;
	default:
		break;
	}
	UserSettings->SetScreenResolution(resolution);
}

void UOptions::ChangeFrameRateLimit(float Value)
{
	float frameRateLimit = 60;
	switch (FMath::FloorToInt(Value))
	{
	case 0:	frameRateLimit = 30;
		break;
	case 1:	frameRateLimit = 60;
		break;
	case 2:	frameRateLimit = 120;
		break;
	case 3:	frameRateLimit = 240;
		break;
	case 4:	frameRateLimit = 0; //Unlimited
		break;
	}
	UserSettings.Get()->SetFrameRateLimit(frameRateLimit);
}
//-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------AUDIO----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void UOptions::SetAudioOptions()
{
	//--Master Volume--
	TObjectPtr<USettingsListEntryContinous> masterVolumeSetting = Cast<USettingsListEntryContinous>(CreateWidget(GetWorld(), SettingsListEntryContinousClass));
	masterVolumeSetting.Get()->SetSetting(FText::FromString("Master Volume"), UserSettings, UserSettings.Get()->GetMasterVolume(),1,
									{
									});
	masterVolumeSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeMasterVolume);
	SettingsAudioBox.Get()->AddChildToVerticalBox(masterVolumeSetting);
	SettingsList.Add(masterVolumeSetting);
	//--Music Volume--
	TObjectPtr<USettingsListEntryContinous> musicVolumeSetting = Cast<USettingsListEntryContinous>(CreateWidget(GetWorld(), SettingsListEntryContinousClass));
	musicVolumeSetting.Get()->SetSetting(FText::FromString("Music Volume"), UserSettings, UserSettings.Get()->GetMusicVolume(),1,
									{
									});
	musicVolumeSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeMusicVolume);
	SettingsAudioBox.Get()->AddChildToVerticalBox(musicVolumeSetting);
	SettingsList.Add(musicVolumeSetting);
	//--SFX Volume--
	TObjectPtr<USettingsListEntryContinous> sfxVolumeSetting = Cast<USettingsListEntryContinous>(CreateWidget(GetWorld(), SettingsListEntryContinousClass));
	sfxVolumeSetting.Get()->SetSetting(FText::FromString("SFX Volume"), UserSettings, UserSettings.Get()->GetSFXVolume(),1,
									{
									});
	sfxVolumeSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeSFXVolume);
	SettingsAudioBox.Get()->AddChildToVerticalBox(sfxVolumeSetting);
	SettingsList.Add(sfxVolumeSetting);
}

void UOptions::ChangeMasterVolume(float Value)
{
	UserSettings.Get()->SetMasterVolume(Value);
	TTGameInstance.Get()->ChangeMasterVolume();
	TTGameInstance.Get()->PushSound();
}

void UOptions::ChangeMusicVolume(float Value)
{
	UserSettings.Get()->SetMusicVolume(Value);
	TTGameInstance.Get()->ChangeMusic();
	TTGameInstance.Get()->PushSound();
}

void UOptions::ChangeSFXVolume(float Value)
{
	UserSettings.Get()->SetSFXVolume(Value);
	TTGameInstance.Get()->ChangeSFX();
	TTGameInstance.Get()->PushSound();
}

//-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------GAMEPLAY-------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
void UOptions::SetGameplayOptions()
{
	//--Brightness--
	TObjectPtr<USettingsListEntryContinous> brightnessSetting = Cast<USettingsListEntryContinous>(CreateWidget(GetWorld(), SettingsListEntryContinousClass));
	brightnessSetting.Get()->SetSetting(FText::FromString("Brightness"), UserSettings, UserSettings.Get()->GetBrightnessValue()-10,2,
									{
									});
	brightnessSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeBrightness);
	SettingsGameplayBox.Get()->AddChildToVerticalBox(brightnessSetting);
	SettingsList.Add(brightnessSetting);
	//--Interpolation Speed--
	TObjectPtr<USettingsListEntryContinous> interpolationSpeedSetting = Cast<USettingsListEntryContinous>(CreateWidget(GetWorld(), SettingsListEntryContinousClass));
	interpolationSpeedSetting.Get()->SetSetting(FText::FromString("Interpolation Speed"), UserSettings, UserSettings.Get()->GetInterpolationSpeed(),100,
									{
									});
	interpolationSpeedSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeInterpolationSpeed);
	SettingsGameplayBox.Get()->AddChildToVerticalBox(interpolationSpeedSetting);
	SettingsList.Add(interpolationSpeedSetting);
	//--Vibration--
	TObjectPtr<USettingsListEntryDiscrete> vibrationSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	vibrationSetting.Get()->SetSetting(FText::FromString("Vibration"), UserSettings,  static_cast<int>(UserSettings.Get()->GetVibration()),1,
									{
												FText::FromString("OFF"),
												FText::FromString("ON"),
									});
	vibrationSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeVibration);
	SettingsGameplayBox.Get()->AddChildToVerticalBox(vibrationSetting);
	SettingsList.Add(vibrationSetting);
	//--Color Blind Mode--
	TObjectPtr<USettingsListEntryDiscrete> colorBlindModeSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	colorBlindModeSetting.Get()->SetSetting(FText::FromString("Color Blind Mode"), UserSettings,  static_cast<int>(UserSettings.Get()->GetColorBlindMode()),3,
									{
												FText::FromString("Normal Vision"),
												FText::FromString("Deuteranope"),
												FText::FromString("Protanope"),
												FText::FromString("Tritanope")
									});
	colorBlindModeSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeColorBlindMode);
	SettingsGameplayBox.Get()->AddChildToVerticalBox(colorBlindModeSetting);
	SettingsList.Add(colorBlindModeSetting);
	//--Color Blind Strength--
	TObjectPtr<USettingsListEntryDiscrete> colorBlindStrengthSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	colorBlindStrengthSetting.Get()->SetSetting(FText::FromString("Color Blind Strength"), UserSettings,  UserSettings.Get()->GetColorBlindStrength(),4,
									{
												FText::FromString("1"),
												FText::FromString("2"),
												FText::FromString("3"),
												FText::FromString("4"),
												FText::FromString("5"),
									});
	colorBlindStrengthSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeColorBlindStrength);
	SettingsGameplayBox.Get()->AddChildToVerticalBox(colorBlindStrengthSetting);
	SettingsList.Add(colorBlindStrengthSetting);
	//--Motion Blur--
	TObjectPtr<USettingsListEntryDiscrete> motionBlurSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	int currentMotionBlurIndex = 0;
	if(UserSettings.Get()->GetMotionBlur() == 0.0F)
	{
		currentMotionBlurIndex = 0;
	}
	else if(UserSettings.Get()->GetMotionBlur() == 0.3F)
	{
		currentMotionBlurIndex = 1;
	}
	else if(UserSettings.Get()->GetMotionBlur() == 0.6F)
	{
		currentMotionBlurIndex = 2;
	}
	else 
	{	currentMotionBlurIndex = 3;
	}
	motionBlurSetting.Get()->SetSetting(FText::FromString("Motion Blur"), UserSettings,  currentMotionBlurIndex,3,
									{
												FText::FromString("OFF"),
												FText::FromString("LOW"),
												FText::FromString("MEDIUM"),
												FText::FromString("HIGH"),
									});
	motionBlurSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ChangeMotionBlur);
	SettingsGameplayBox.Get()->AddChildToVerticalBox(motionBlurSetting);
	SettingsList.Add(motionBlurSetting);
	//--Show FPS--
	TObjectPtr<USettingsListEntryDiscrete> showFPSSetting = Cast<USettingsListEntryDiscrete>(CreateWidget(GetWorld(), SettingsListEntryDiscreteClass));
	showFPSSetting.Get()->SetSetting(FText::FromString("Show FPS"), UserSettings,  UserSettings.Get()->ShouldShowFPS(),1,
									{
												FText::FromString("OFF"),
												FText::FromString("ON")
									});
	showFPSSetting.Get()->OnChangeSetting.AddDynamic(this, &UOptions::ShowFPS);
	SettingsGameplayBox.Get()->AddChildToVerticalBox(showFPSSetting);
	SettingsList.Add(showFPSSetting);
}

void UOptions::ChangeInterpolationSpeed(float Value)
{
	UserSettings.Get()->SetInterpolationSpeed(Value);
}

void UOptions::ChangeGamepadSensitivityX(float Value)
{
	if(Value == 0.0f)
	{
		Value = 0.01f;
	}
	UserSettings.Get()->SetGamepadSensitivityX(Value);
}

void UOptions::ChangeGamepadSensitivityY(float Value)
{
	if(Value == 0.0f)
	{
		Value = 0.01f;
	}
	UserSettings.Get()->SetGamepadSensitivityY(Value);
}

void UOptions::ChangeVibration(float Value)
{
	UserSettings.Get()->SetVibration(Value);
}

void UOptions::ChangeColorBlindMode(float Value)
{
	EColorVisionDeficiency colorMode = EColorVisionDeficiency::NormalVision;
	switch (FMath::FloorToInt(Value))
	{
	case 0:
		colorMode = EColorVisionDeficiency::NormalVision;
		break;
	case 1:
		colorMode = EColorVisionDeficiency::Deuteranope;
		break;
	case 2:
		colorMode = EColorVisionDeficiency::Protanope;
		break;
	case 3:
		colorMode = EColorVisionDeficiency::Tritanope;
		break;
	}
	UserSettings.Get()->SetColorBlindMode(colorMode);
}

void UOptions::ChangeColorBlindStrength(float Value)
{
	UserSettings.Get()->SetColorBlindStrength(Value * 2);
}

void UOptions::ChangeBrightness(float Value)
{
	//10 because of the configuration of the Post Process in game
	UserSettings.Get()->SetBrightness(Value + 10);
}

void UOptions::ShowFPS(float Value)
{
	UserSettings.Get()->SetShowFPS(FMath::FloorToInt(Value));
}

void UOptions::ChangeMotionBlur(float Value)
{
	float motionBlurValue = 0;

	if(Value == 0.0f)
	{
		motionBlurValue = 0;
	}
	else if(Value == 1.0f)
	{
		motionBlurValue = 0.3f;
	}
	else if(Value == 2.0f)
	{
		motionBlurValue = 0.6f;
	}
	else if(Value == 3.0f)
	{
		motionBlurValue = 1.0f;
	}
	UserSettings.Get()->SetMotionBlur(motionBlurValue);
}

//-----------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------CONTROLS-------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

void UOptions::SetControlsOptions()
{
	for (FControlInput control : ControlsLeft)
	{
		TObjectPtr<UControlListEntry> newPCControl = Cast<UControlListEntry>(CreateWidget(GetWorld(), ControlListEntryClass));
		newPCControl.Get()->SetControl(control.ControlText, control.ControlImage);
		SettingsControlsBox1.Get()->AddChildToVerticalBox(newPCControl);
		ControlsList.Add(newPCControl);
	}
	for (FControlInput control : ControlsRight)
	{
		TObjectPtr<UControlListEntry> newPCControl = Cast<UControlListEntry>(CreateWidget(GetWorld(), ControlListEntryClass));
		newPCControl.Get()->SetControl(control.ControlText, control.ControlImage);
		SettingsControlsBox2.Get()->AddChildToVerticalBox(newPCControl);
		ControlsList.Add(newPCControl);
	}
}
