// TinyTavern


#include "TT_CommonActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "Windows/WindowsPlatformInput.h"

void UTT_CommonActivatableWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FKey& virtual_key = const_cast<FKey&>(EKeys::Virtual_Accept);
	virtual_key = FPlatformInput::GetGamepadAcceptKey();
	PlayerController = Cast<ATTPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if(PlayerController)
	{
		PlayerController.Get()->CurrentMouseCursor = EMouseCursor::Default;
	}
}

void UTT_CommonActivatableWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if(PlayerController)
	{
		PlayerController.Get()->HideCursorAndSetCorrectInputMode();
	}
}

void UTT_CommonActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	// if(PlayerController)
	// {
	// 	PlayerController.Get()->HideCursorAndSetCorrectInputMode();
	// }
}

TOptional<FUIInputConfig> UTT_CommonActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case FInputConfig::All:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode, false);
	case FInputConfig::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode, false);
	case FInputConfig::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture, false);
	default:
		return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::CapturePermanently, false);
	}
}
