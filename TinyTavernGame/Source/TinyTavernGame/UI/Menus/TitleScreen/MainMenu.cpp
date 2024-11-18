// TinyTavern


#include "MainMenu.h"
#include "../Buttons/ButtonBase.h"
#include "Kismet/GameplayStatics.h"
#include "TitleScreen.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/Settings/TT_SaveGameSystem.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	TTGameInstance.Get()->FromMenu = true;
	if(!UGameplayStatics::DoesSaveGameExist(SAVEGAME_SLOT_NAME, SAVEGAME_USERINDEX))
	{
		ContinueButton.Get()->SetTextOfButton(FText::FromString("NEW GAME"));
		NewGameButton.Get()->SetVisibility(ESlateVisibility::Collapsed);
		ContinueButton.Get()->SetNavigationRuleExplicit(EUINavigation::Up, ExitButton);
		ExitButton.Get()->SetNavigationRuleExplicit(EUINavigation::Down, ContinueButton);
	}
	else
	{
		NewGameButton.Get()->OnClicked().AddUObject(this, &UMainMenu::NewGame);
		ContinueButton.Get()->SetTextOfButton(FText::FromString("CONTINUE"));
	}
	//Link button functions
	StartGameDelegate = ContinueButton.Get()->OnClicked().AddUObject(this, &UMainMenu::StartGame);
	OpenOptionsMenuDelegate = OptionsButton.Get()->OnClicked().AddUObject(this, &UMainMenu::OpenOptionsMenu);
	CreditsButton.Get()->OnClicked().AddUObject(this, &UMainMenu::OpenCredits);
	ExitMenuDelegate = ExitButton.Get()->OnClicked().AddUObject(this, &UMainMenu::OpenExitMenu);

	//First Time in the game
	if(TTGameInstance.Get()->FirstTimeInGame)
	{
		TTGameInstance.Get()->FirstTimeInGame = false;
		
		(new FAsyncTask<FCreateDefaultGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get()))->StartSynchronousTask();
		FAsyncTask<FSaveGameAsyncTask>* saveGameTask = new FAsyncTask<FSaveGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get());
		saveGameTask->StartSynchronousTask();
	}
}

void UMainMenu::NativeOnActivated()
{
	Super::NativeOnActivated();
	if(TTGameInstance.Get()->ShoulPlayInitialCinematic)
	{
		ContinueButton.Get()->SetTextOfButton(FText::FromString("NEW GAME"));
	}
	else
	{
		ContinueButton.Get()->SetTextOfButton(FText::FromString("CONTINUE"));
	}
}

void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();
	NewGameButton.Get()->OnClicked().RemoveAll(this);
	ContinueButton.Get()->OnClicked().Remove(StartGameDelegate);
	OptionsButton.Get()->OnClicked().Remove(OpenOptionsMenuDelegate);
	CreditsButton.Get()->OnClicked().RemoveAll(this);
	ExitButton.Get()->OnClicked().Remove(ExitMenuDelegate);
}

UWidget* UMainMenu::NativeGetDesiredFocusTarget() const
{
	return ContinueButton;
}

void UMainMenu::SetTitleScreenParent(TObjectPtr<UTitleScreen> NewTitleScreen)
{
	TitleScreen = NewTitleScreen;
}

void UMainMenu::WelcomeToGarcosa()
{
	//First Achievement
	TTGameInstance.Get()->WelcomeToGarcosa();
}

void UMainMenu::NewGame()
{
	if (TitleScreen != nullptr)
	{
		TObjectPtr<UConfirmMenu> confirmMenu = Cast<UConfirmMenu>(TitleScreen.Get()->AddWidgetToTitleScreenStack(FTitleScreenWidgets::ConfirmMenu));
		confirmMenu.Get()->SetTitleScreen(TitleScreen);
		confirmMenu.Get()->AddConfirmMenu(NewGameTitleConfirmMenu, NewGameDescriptionConfirmMenu, NewGameConfirmMenuOption);
	}
}

void UMainMenu::StartGame()
{
	TTGameInstance.Get()->InitializeSpeedRunTime();
	TTGameInstance.Get()->ShoulPlayInitialCinematic = false;
	//If player quits with the recipe book, spawn him in tutorial
	if(!TTGameInstance.Get()->FromMenu && (TTGameInstance.Get()->CurrentLevel == -1 || TTGameInstance.Get()->CurrentLevel == 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), "P_Tutorial");
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), PlayLevel);	
	}
}

void UMainMenu::OpenOptionsMenu()
{
	TitleScreen.Get()->AddWidgetToTitleScreenStack(FTitleScreenWidgets::Options);
}

void UMainMenu::OpenExitMenu()
{
	if (TitleScreen != nullptr)
	{
		TObjectPtr<UConfirmMenu> confirmMenu = Cast<UConfirmMenu>(TitleScreen.Get()->AddWidgetToTitleScreenStack(FTitleScreenWidgets::ConfirmMenu));
		confirmMenu->AddConfirmMenu(TitleConfirmMenu, DescriptionConfirmMenu, ConfirmMenuOption);
	}
}

void UMainMenu::OpenCredits()
{
	TitleScreen.Get()->AddWidgetToTitleScreenStack(FTitleScreenWidgets::Credits);
}
