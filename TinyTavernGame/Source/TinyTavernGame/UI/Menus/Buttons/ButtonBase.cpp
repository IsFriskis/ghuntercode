// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonBase.h"
#include "CommonTextBlock.h"
#include "Windows/WindowsPlatformInput.h"

void UButtonBase::NativePreConstruct()
{
	Super::NativeConstruct();
	
	SetTextOfButton(TextOfButton);
	ButtonIndex = 0;
}

void UButtonBase::SetTextOfButton(FText NewText)
{
	if(!NewText.IsEmpty())
	{
		ButtonText.Get()->SetText(NewText);
	}
	else
	{
		ButtonText.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	if (HasAnimationOnHover)
	{
		PlayAnimation(HoverAnimation, 0, HoverAnimationLoops);
	}
	FKey& virtual_key = const_cast<FKey&>(EKeys::Virtual_Accept);
	virtual_key = FPlatformInput::GetGamepadAcceptKey();
}

void UButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	StopAnimation(HoverAnimation);
}
