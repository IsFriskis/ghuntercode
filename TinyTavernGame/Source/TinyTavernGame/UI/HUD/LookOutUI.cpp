// TinyTavern


#include "LookOutUI.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Input/CommonUIInputTypes.h"
#include "TinyTavernGame/UI/Menus/Buttons/ButtonBase.h"

void ULookOutUI::NativeConstruct()
{
	Super::NativeConstruct();
	MovableLookOutPanelSlot = Cast<UCanvasPanelSlot>(MovableLookOutPanel.Get()->Slot);
	MoveLeftButton.Get()->OnPressed().AddUObject(this, &ULookOutUI::HoldLeft);
	MoveLeftButton.Get()->OnReleased().AddUObject(this, &ULookOutUI::ReleaseLeft);
	MoveRightButton.Get()->OnPressed().AddUObject(this, &ULookOutUI::HoldRight);
	MoveRightButton.Get()->OnReleased().AddUObject(this, &ULookOutUI::ReleaseRight);
	MoveUpButton.Get()->OnPressed().AddUObject(this, &ULookOutUI::HoldUp);
	MoveUpButton.Get()->OnReleased().AddUObject(this, &ULookOutUI::ReleaseUp);
	MoveDownButton.Get()->OnPressed().AddUObject(this, &ULookOutUI::HoldDown);
	MoveDownButton.Get()->OnReleased().AddUObject(this, &ULookOutUI::ReleaseDown);
	ActivateWidget();
}

void ULookOutUI::NativeDestruct()
{
	Super::NativeDestruct();
	MoveLeftButton.Get()->OnPressed().RemoveAll(this);
	MoveLeftButton.Get()->OnReleased().RemoveAll(this);
	MoveRightButton.Get()->OnPressed().RemoveAll(this);
	MoveRightButton.Get()->OnReleased().RemoveAll(this);
	MoveUpButton.Get()->OnPressed().RemoveAll(this);
	MoveUpButton.Get()->OnReleased().RemoveAll(this);
	MoveDownButton.Get()->OnPressed().RemoveAll(this);
	MoveDownButton.Get()->OnReleased().RemoveAll(this);
}

void ULookOutUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(IsHoldingLeft)
	{
		MoveLeft();
	}
	if(IsHoldingRight)
	{
		MoveRight();
	}
	if(IsHoldingUp)
	{
		MoveUp();
	}
	if(IsHoldingDown)
	{
		MoveDown();
	}
	if(!IsHoldingDown && !IsHoldingLeft && !IsHoldingUp && !IsHoldingRight)
	{
		MovePanel.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		LookOutPanelPosition = FVector2d(-960.0f,-540.0f);
	}
	LookOutPanelPosition = FMath::Lerp(MovableLookOutPanelSlot.Get()->GetPosition(), LookOutPanelPosition, InDeltaTime);
	MovableLookOutPanelSlot.Get()->SetPosition(LookOutPanelPosition);
}

void ULookOutUI::MoveLeft()
{
	MovePanel.Get()->SetVisibility(ESlateVisibility::Hidden);
	LookOutPanelPosition.X -= MoveAmount;
	LookOutPanelPosition.X = FMath::Clamp(LookOutPanelPosition.X, MinPosition.X, MaxPosition.X);
}

void ULookOutUI::HoldLeft()
{
	IsHoldingLeft = true;
}

void ULookOutUI::ReleaseLeft()
{
	IsHoldingLeft = false;
}

void ULookOutUI::HoldRight()
{
	IsHoldingRight = true;
}

void ULookOutUI::ReleaseRight()
{
	IsHoldingRight = false;
}

void ULookOutUI::HoldUp()
{
	IsHoldingUp = true;
}

void ULookOutUI::ReleaseUp()
{
	IsHoldingUp = false;
}

void ULookOutUI::HoldDown()
{
	IsHoldingDown = true;
}

void ULookOutUI::ReleaseDown()
{
	IsHoldingDown = false;
}

void ULookOutUI::MoveRight()
{
	MovePanel.Get()->SetVisibility(ESlateVisibility::Hidden);
	LookOutPanelPosition.X += MoveAmount;
	LookOutPanelPosition.X = FMath::Clamp(LookOutPanelPosition.X, MinPosition.X, MaxPosition.X);
}

void ULookOutUI::MoveUp()
{
	MovePanel.Get()->SetVisibility(ESlateVisibility::Hidden);
	LookOutPanelPosition.Y -= MoveAmount;
	LookOutPanelPosition.Y = FMath::Clamp(LookOutPanelPosition.Y, MinPosition.Y, MaxPosition.Y);
}

void ULookOutUI::MoveDown()
{
	MovePanel.Get()->SetVisibility(ESlateVisibility::Hidden);
	LookOutPanelPosition.Y += MoveAmount;
	LookOutPanelPosition.Y = FMath::Clamp(LookOutPanelPosition.Y, MinPosition.Y, MaxPosition.Y);
}
