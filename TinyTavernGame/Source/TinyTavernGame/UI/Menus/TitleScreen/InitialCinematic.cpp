// TinyTavern


#include "InitialCinematic.h"

#include "TitleScreen.h"
#include "Animation/WidgetAnimation.h"
#include "Components/AudioComponent.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Buttons/SkipButton.h"

void UInitialCinematic::NativeConstruct()
{
	SkipButton.Get()->SetVisibility(ESlateVisibility::Collapsed);
	//Skip Handler
	TTGameInstance= Cast<UTT_GameInstance>(GetGameInstance());
	if(!TTGameInstance.Get()->FirstTimeInGame)
	{
		SkipButton.Get()->InCollapsedState();
		SkipButton.Get()->OnSkip.AddDynamic(this, &UInitialCinematic::HeadToMainMenu);
		SkipButtonInvisible.Get()->InvisibleSkipThatTurnsVisibleButDoesNotSkip();
	}
	NarrationAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), InitialCinematicSound);
	Super::NativeConstruct();
}

void UInitialCinematic::NativeDestruct()
{
	SkipButton.Get()->OnSkip.RemoveAll(this);
	if(NarrationAudioComponent)
	{
		NarrationAudioComponent.Get()->Stop();
	}
}

void UInitialCinematic::ShowSkipButton()
{
	SkipHandle.Unregister();
	SkipButton.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SkipButtonInvisible.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UInitialCinematic::SetTitleScreen(TObjectPtr<UTitleScreen> _TitleScreen)
{
	TitleScreen = _TitleScreen;
}

bool UInitialCinematic::IsFirstTime()
{
	return TTGameInstance.Get()->FirstTimeInGame;
}

void UInitialCinematic::HeadToMainMenu()
{
	SkipButton.Get()->OnSkip.RemoveAll(this);
	if(!Skipped)
	{
		if(NarrationAudioComponent)
		{
			NarrationAudioComponent.Get()->Stop();
		}
		Skipped = true;
		StopAnimation(Intro);
		TitleScreen.Get()->StartMusic();
		TitleScreen.Get()->StartBackgroundAnimation(1.0f);
		DeactivateWidget();
		TitleScreen.Get()->AddMainMenu();
	}
}
