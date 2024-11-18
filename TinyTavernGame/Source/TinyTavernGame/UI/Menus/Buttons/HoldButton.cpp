// TinyTavern


#include "HoldButton.h"

#include "Components/ProgressBar.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"

void UHoldButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	HoldInput.Get()->OnPressed().AddUObject(this, &UHoldButton::BeginHolding);
	HoldInput.Get()->OnReleased().AddUObject(this, &UHoldButton::EndHolding);
	OnPressed().AddUObject(this, &UHoldButton::BeginHolding);
	OnReleased().AddUObject(this, &UHoldButton::EndHolding);

	HoldProgressBar.Get()->SetPercent(0);
	PlayerIsHolding = false;
	HoldBlend.SetBlendTime(MaxHoldTime);
	HoldBlend.SetValueRange(0,1);
}

void UHoldButton::NativeDestruct()
{
	Super::NativeDestruct();
	
	HoldInput.Get()->OnPressed().RemoveAll(this);
	HoldInput.Get()->OnReleased().RemoveAll(this);
	OnPressed().RemoveAll(this);
	OnReleased().RemoveAll(this);
}

void UHoldButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(PlayerIsHolding)
	{
		HoldBlend.Update(InDeltaTime);
		HoldProgressBar.Get()->SetPercent(HoldBlend.GetBlendedValue());
		if(HoldBlend.IsComplete())
		{
			HoldFinishedAction();
		}
	}
}

void UHoldButton::HoldFinishedAction()
{
	OnHoldFinished.Broadcast();
	HoldProgressBar.Get()->SetPercent(0);
	PlayerIsHolding = false;
}

void UHoldButton::BeginHolding()
{
	HoldBlend.Reset();
	PlayerIsHolding = true;
}

void UHoldButton::EndHolding()
{
	HoldProgressBar.Get()->SetPercent(0);
	HoldBlend.Reset();
	PlayerIsHolding = false;
}


