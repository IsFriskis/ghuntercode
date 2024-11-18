// TinyTavern


#include "SkipButton.h"

#include "Input/CommonUIInputTypes.h"

void USkipButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkipButton::NativeDestruct()
{
	Super::NativeDestruct();
	SkipHandle.Unregister();
}

void USkipButton::InCollapsedState()
{
	IsCollapsed = true;
}

void USkipButton::InvisibleSkipThatTurnsVisibleButDoesNotSkip()
{
	SkipHandle.Unregister();
}

void USkipButton::EnableSkipButton()
{
	//Skip Handler
	SkipHandle = RegisterUIActionBinding(FBindUIActionArgs(SkipInputActionData, false, FSimpleDelegate::CreateUObject(this, &USkipButton::SkipCinematic)));
}

void USkipButton::SkipCinematic()
{
	SkipHandle.Unregister();
	OnSkip.Broadcast();
}
