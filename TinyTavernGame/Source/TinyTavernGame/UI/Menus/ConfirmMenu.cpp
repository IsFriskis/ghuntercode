// TinyTavern


#include "ConfirmMenu.h"
#include "CommonTextBlock.h"
#include "Buttons/ButtonBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TinyTavernGame/Levels/TavernLevel.h"
#include "TinyTavernGame/Settings/TT_SaveGameSystem.h"
#include "TitleScreen/TitleScreen.h"

class UTT_SaveGameSystem;
class FLoadGameAsyncTask;

void UConfirmMenu::NativeConstruct()
{
	Super::NativeConstruct();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	CancelDelegate = CancelButton.Get()->OnClicked().AddUObject(this, &UConfirmMenu::Cancel);
}

void UConfirmMenu::NativeDestruct()
{
	Super::NativeDestruct();
	
	ConfirmButton.Get()->OnClicked().RemoveAll(this);
	CancelButton.Get()->OnClicked().Remove(CancelDelegate);
}

UWidget* UConfirmMenu::NativeGetDesiredFocusTarget() const
{
	Super::NativeGetDesiredFocusTarget();
	return ConfirmButton;
}

void UConfirmMenu::AddConfirmMenu(FText Title, FText Description, FConfirmButtonOption _ConfirmButtonOption)
{
	TitleTextBlock->SetText(Title);
	DescriptionTextBlock->SetText(Description);
	ConfirmButtonOption = _ConfirmButtonOption;
	FigureOutConfirmButtonPurpose(ConfirmButtonOption);
}


void UConfirmMenu::FigureOutConfirmButtonPurpose(FConfirmButtonOption _ConfirmButtonOption)
{
	switch (_ConfirmButtonOption)
	{
	case FConfirmButtonOption::ExitGame:
		ConfirmButton.Get()->OnClicked().AddUObject(this, &UConfirmMenu::ExitGame);
		break;
	case FConfirmButtonOption::AccessAdventureLevel:
		ConfirmButton.Get()->OnClicked().AddUObject(this, &UConfirmMenu::AccessAdventure);
		break;
	case FConfirmButtonOption::AccessTitleScreen:
		ConfirmButton.Get()->OnClicked().AddUObject(this, &UConfirmMenu::AccessTitleScreen);
		break;
	case FConfirmButtonOption::NewGame:
		ConfirmButton.Get()->OnClicked().AddUObject(this, &UConfirmMenu::NewGame);
		break;
	default:
		break;
	}
}

void UConfirmMenu::Cancel()
{
	if(ConfirmButtonOption == FConfirmButtonOption::AccessTitleScreen)
	{
		RemoveFromParent();
	}
	else
	{
		DeactivateWidget();	
	}
}

void UConfirmMenu::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UConfirmMenu::AccessAdventure()
{
	//Get reference to level
	TObjectPtr<ATavernLevel> tavernLevel = Cast<ATavernLevel>(GetWorld()->GetLevelScriptActor());
	tavernLevel.Get()->GoToAdventure();
}

void UConfirmMenu::AccessTitleScreen()
{
	(new FAsyncTask<FLoadGameAsyncTask>(GetGameInstance()->GetSubsystem<UTT_SaveGameSystem>())	)->StartBackgroundTask();
	UGameplayStatics::OpenLevel(GetWorld(), "TitleScreen");
}

void UConfirmMenu::NewGame()
{
	TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>()->DeleteSaveGame();
	TTGameInstance.Get()->ShoulPlayInitialCinematic = false;
	TTGameInstance.Get()->InitializeSpeedRunTime();
	UGameplayStatics::OpenLevel(GetWorld(), "TavernGym");
}
