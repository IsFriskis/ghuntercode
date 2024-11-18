// TinyTavern


#include "TitleScreen.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "InitialCinematic.h"
#include "Initialization.h"
#include "MainMenu.h"
#include "Components/AudioComponent.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Credits.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "TinyTavernGame/UI/Menus/Options/Options.h"
#include "Windows/WindowsPlatformInput.h"

void UTitleScreen::NativeConstruct()
{
	Super::NativeConstruct();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	BlackBackground.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//Button A acts like Click
	FKey& virtual_key = const_cast<FKey&>(EKeys::Virtual_Accept);
	virtual_key = FPlatformInput::GetGamepadAcceptKey();
	
	// if(TTGameInstance.Get()->ShowLogosAtStart)
	// {
	// 	PlayAnimation(BlackBackgroundDissapears);
	// }
	// else
	if(!TTGameInstance.Get()->ShowLogosAtStart)
	{
		AddMainMenu();
		// UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		StartMusic();
		StartBackgroundAnimation(1.0f);
	}
	else
	{
		AddWidgetToTitleScreenStack(FTitleScreenWidgets::Intro);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
	}
}

void UTitleScreen::NativeDestruct()
{
	Super::NativeDestruct();
	if(Credits)
	{
		Credits.Get()->OnCreditsEnded.RemoveAll(this);
	}
}

void UTitleScreen::CreditsEnded()
{
	StartMusic();
}

TObjectPtr<UCommonActivatableWidget> UTitleScreen::AddWidgetToTitleScreenStack(FTitleScreenWidgets WidgetName)
{
	TObjectPtr<UOptions> optionsMenu;
	TObjectPtr<UInitialization> initialization;
	TObjectPtr<UInitialCinematic> initialCinematic;
	switch (WidgetName)
	{
	case FTitleScreenWidgets::MainMenu:
		return TitleScreenStack.Get()->AddWidget(MainMenuWidget);
	case FTitleScreenWidgets::Options:
		optionsMenu = Cast<UOptions>(TitleScreenStack.Get()->AddWidget(OptionsWidget));
		optionsMenu.Get()->InTitleScreen();
		return optionsMenu;
	case FTitleScreenWidgets::ConfirmMenu:
		return TitleScreenStack.Get()->AddWidget(ConfirmMenuWidget);
	case FTitleScreenWidgets::InitialCinematic:
		// if(MusicComponent)
		// {
		// 	MusicComponent.Get()->Stop();
		// }
		initialCinematic = Cast<UInitialCinematic>(TitleScreenStack.Get()->AddWidget(InitialCinematicWidget));
		initialCinematic.Get()->SetTitleScreen(this);
		return initialCinematic;
	case FTitleScreenWidgets::Intro:
		initialization = Cast<UInitialization>(TitleScreenStack.Get()->AddWidget(IntroWidget));
		initialization.Get()->SetTitleScreen(this);
		return initialization;
	case FTitleScreenWidgets::Credits:
		Credits = Cast<UCredits>(TitleScreenStack.Get()->AddWidget(CreditsWidget));
		Credits.Get()->OnCreditsEnded.RemoveAll(this);
		Credits.Get()->OnCreditsEnded.AddDynamic(this, &UTitleScreen::CreditsEnded);
		if(MusicComponent)
		{
			MusicComponent.Get()->Stop();
		}
		return Credits;
	default:
		break;
	}
	return TitleScreenStack.Get()->AddWidget(MainMenuWidget);
}

void UTitleScreen::StartMusic()
{
	MusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), TitleScreenMusic);
	TTGameInstance.Get()->ShowLogosAtStart = false;
}

void UTitleScreen::StartBackgroundAnimation(float TimeStarts)
{
	PlayAnimation(BlackBackgroundDissapears, TimeStarts);
}

void UTitleScreen::AddMainMenu()
{
	if(MainMenu)
	{
		MainMenu = nullptr;
	}
	MainMenu = Cast<UMainMenu>(AddWidgetToTitleScreenStack(FTitleScreenWidgets::MainMenu));
	MainMenu.Get()->SetTitleScreenParent(this);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}
