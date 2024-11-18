// TinyTavern


#include "Credits.h"

#include "Components/AudioComponent.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Menus/Buttons/SkipButton.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UCredits::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	BackLogic();
}

void UCredits::NativeOnActivated()
{
	Super::NativeOnActivated();
	PlayAnimation(CreditsAnimation);
	MusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), CreditMusic);
	SkipButton.Get()->OnSkip.AddDynamic(this, &UCredits::BackLogic);
}

void UCredits::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	BackHandle.Unregister();
	SkipButton.Get()->OnSkip.RemoveAll(this);
}

void UCredits::BackLogic()
{
	if(MusicComponent)
	{
		MusicComponent.Get()->Stop();
	}
	OnCreditsEnded.Broadcast();
	DeactivateWidget();
	if(ShouldHeadToMainMenu)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "TitleScreen");
	}
}
