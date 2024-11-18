// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleScreen.generated.h"

class UInitialCinematic;
class UCredits;
class UTT_GameInstance;
class UInitialization;
class UImage;
class UCommonActivatableWidgetStack;
class UOptions;
class UTT_CommonActivatableWidget;
class UConfirmMenu;
class UMainMenu;

UENUM(BlueprintType)
enum class FTitleScreenWidgets : uint8
{
	MainMenu,
	Options,
	Controls,
	ConfirmMenu,
	InitialCinematic,
	Intro,
	Credits
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTitleScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Adds a Widget to the Stack
	TObjectPtr<class UCommonActivatableWidget> AddWidgetToTitleScreenStack(FTitleScreenWidgets WidgetName);
	void StartMusic();
	void StartBackgroundAnimation(float TimeStarts = 1.0f);
	void AddMainMenu();
	
protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BlackBackgroundDissapears;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> TitleScreenStack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BlackBackground;
	//Common Activatable Widgets
	UPROPERTY(EditAnywhere, Category = "TT| TitleScreen")
	TObjectPtr<USoundBase> TitleScreenMusic;
	UPROPERTY(EditAnywhere, Category = "TT| TitleScreen Widgets")
	TSubclassOf<UMainMenu> MainMenuWidget;
	UPROPERTY(EditAnywhere, Category = "TT| TitleScreen Widgets")
	TSubclassOf<UInitialCinematic> InitialCinematicWidget;
	UPROPERTY(EditAnywhere, Category = "TT| TitleScreen Widgets")
	TSubclassOf<UInitialization> IntroWidget;
	UPROPERTY(EditAnywhere, Category = "TT| TitleScreen Widgets")
	TSubclassOf<UOptions> OptionsWidget;
	UPROPERTY(EditAnywhere, Category = "TT| TitleScreen Widgets")
	TSubclassOf<UConfirmMenu> ConfirmMenuWidget;
	UPROPERTY(EditAnywhere, Category = "TT| TitleScreen Widgets")
	TSubclassOf<UCredits> CreditsWidget;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<UMainMenu> MainMenu;
	TObjectPtr<UAudioComponent> MusicComponent;
	TObjectPtr<UCredits> Credits;
	UFUNCTION()
	void CreditsEnded();
};
