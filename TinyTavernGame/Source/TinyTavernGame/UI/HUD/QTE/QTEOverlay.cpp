// TinyTavern


#include "QTEOverlay.h"

#include "CommonActionWidget.h"
#include "TTRadialSlider.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "UMG.h"
#include "Animation/WidgetAnimation.h"
#include "TinyTavernGame/Weapons/QTE/FinisherAbility.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "QTERadialSlider.h"
#include "SkewerTimeZone.h"


void UQTEOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	WidgetSwitcher.Get()->SetActiveWidgetIndex(0);

	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	for (TTuple<FGameplayTag, TEnumAsByte<FWeaponsLevel>> Weapon : GameInstance->WeaponsLevel)
	{
		FGameplayTag Tag = Weapon.Key;

		if (Tag.MatchesTag(WeaponTag))
		{
			DashLevel = Weapon.Value.GetIntValue();
		}
	}

	if (DashLevel == 0)
	{
		SmashButton->SetVisibility(ESlateVisibility::Hidden);
		Smash->SetVisibility(ESlateVisibility::Hidden);
		SmashFlag->SetVisibility(ESlateVisibility::Hidden);
		SkewerButton->SetVisibility(ESlateVisibility::Hidden);
		Skewer->SetVisibility(ESlateVisibility::Hidden);
		SkewerFlag->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (DashLevel == 1)
	{
		SkewerButton->SetVisibility(ESlateVisibility::Hidden);
		Skewer->SetVisibility(ESlateVisibility::Hidden);
		SkewerFlag->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQTEOverlay::SetQTEWidget(ECuts Cut)
{
	switch (Cut)
	{
	case ECuts::Slice:
		WidgetSwitcher.Get()->SetActiveWidgetIndex(1);
		PlayAnimation(SliceButtonsAnimation, 0, 0);
		break;
	case ECuts::Smash:
		WidgetSwitcher.Get()->SetActiveWidgetIndex(2);
		break;
	case ECuts::Skewer:
		WidgetSwitcher.Get()->SetActiveWidgetIndex(3);
		break;
	default:
		WidgetSwitcher.Get()->SetActiveWidgetIndex(0);
		break;
	}
	SelectedCut = Cut;
}

void UQTEOverlay::UpdateElapsedTime(float ElapsedTime)
{
	switch (SelectedCut)
	{
	case ECuts::Slice:
		break;
	case ECuts::Smash:
		//BackgroundSmashBar.Get()->SetValue(ElapsedTime);
		break;
	case ECuts::Skewer:
		break;
	default:
		break;
	}
}

void UQTEOverlay::RunMinigameResultAnimation(bool Success)
{
	WidgetSwitcher.Get()->SetActiveWidgetIndex(4);
	OnMinigameAnimationFinishHandler.BindDynamic(this, &UQTEOverlay::OnMinigameResultAnimationFinished);
	MinigameResultBar.Get()->PlayFinalResultAnimation(Success, OnMinigameAnimationFinishHandler);
}

void UQTEOverlay::RunMinigameResultAnimation()
{
	WidgetSwitcher.Get()->SetActiveWidgetIndex(4);
	OnMinigameAnimationFinishHandler.BindDynamic(this, &UQTEOverlay::OnMinigameResultAnimationFinished);
	MinigameResultBar.Get()->PlayFinalResultAnimation(false, OnMinigameAnimationFinishHandler);
}

void UQTEOverlay::RunSelectionButtonsAnimation()
{
	PlayAnimation(SelectionButtonsAnimation, 0, 0);
}

void UQTEOverlay::RunMinigameStartAnimation()
{
	OnInitialAnimationFinishedHandler.BindDynamic(this, &UQTEOverlay::OnMinigameInitialAnimationFinished);
	BindToAnimationFinished(MinigameStartAnimation, OnInitialAnimationFinishedHandler);
	PlayAnimation(MinigameStartAnimation);
}

void UQTEOverlay::OnMinigameInitialAnimationFinished()
{
	UnbindAllFromAnimationFinished(MinigameStartAnimation);
	OnInitialAnimationFinishedHandler.Clear();
	OnInitialAnimationFinished.Broadcast();
}

void UQTEOverlay::UpdateQTESelectionBarTimer(float Value)
{
	SelectionTimerBar.Get()->SetRadialSliderPercent(Value);
}

void UQTEOverlay::OnMinigameResultAnimationFinished()
{
	UnbindAllFromAnimationFinished(MinigameResultBar->SuccessAnimation);
	UnbindAllFromAnimationFinished(MinigameResultBar->FailureAnimation);
	OnMinigameAnimationFinishHandler.Clear();
	OnMinigameAnimationFinished.Broadcast();
}


void UQTEOverlay::DisplayNewSliceButton(EButtonToPress ButtonToDisplay)
{
	switch (ButtonToDisplay)
	{
	case EButtonToPress::X:
		DataTableRowHandle.RowName = FName(TEXT("X"));
		break;
	case EButtonToPress::Y:
		DataTableRowHandle.RowName = FName(TEXT("Y"));
		break;
	default:
		DataTableRowHandle.RowName = FName(TEXT("B"));
		break;
	}
	
	Button1->SetInputAction(DataTableRowHandle);
	Button1->SetVisibility(ESlateVisibility::Visible);
}

void UQTEOverlay::HideSliceButton()
{
	Button1->SetVisibility(ESlateVisibility::Hidden);
}

void UQTEOverlay::UpdateSliceBarTimer(float Value)
{
	SliceTimerBar.Get()->SetRadialSliderPercent(Value);
}

void UQTEOverlay::RunSliceFeedbackAnimation(int AmountOfErrors, bool Success)
{
	SliceTimerBar->PlaySliceFeedbackAnimation(AmountOfErrors, Success);
}

void UQTEOverlay::RunSliceButtonAnimation()
{
	PlayAnimation(SliceButtonPressAnimation);
}

void UQTEOverlay::UpdateSmashBarPercent(float Percent)
{
	SmashBar.Get()->SetRadialSliderPercent(Percent / 100);
}

void UQTEOverlay::UpdateTimerSmashBarPercent(float Value)
{
	SmashTimerBar.Get()->SetRadialSliderPercent(Value);
}

void UQTEOverlay::RunSmashButtonAnimation()
{
	PlayAnimation(SmashButtonPressAnimation);
}

void UQTEOverlay::DisplaySmashButton()
{
	DataTableRowHandle.RowName = FName(TEXT("X"));
	SmashPressButton->SetInputAction(DataTableRowHandle);
	SmashPressButton->SetVisibility(ESlateVisibility::Visible);
}

void UQTEOverlay::RunSmashButtonBlinkAnimation()
{
	PlayAnimation(SmashButtonBlinkAnimation, 0, 0);
}

void UQTEOverlay::StopSmashButtonBlinkAnimation()
{
	StopAnimation(SmashButtonBlinkAnimation);
}

//Use to set the zone that indicates the time window player has to click - Values are converted in this function, parameters should be between 0 and 1
void UQTEOverlay::SetTimeZones(TArray<FTimeZone> TimeZones, float ZoneSize)
{
	//Set the Selection Zone for specific SelectionTimeZone
	int Index = 0;
	for (FTimeZone TimeZone : TimeZones)
	{
		USkewerTimeZone* TimeZoneWidget = Cast<USkewerTimeZone>(SkewerTimeZonesOverlay.Get()
			->GetChildAt(Index));
		Index++;
		TimeZoneWidget->SetSelectionZone(ZoneSize, TimeZone.Position * GetHandleEndAngle());
		TimeZoneWidget->SetVisibility(ESlateVisibility::Visible);
	}
	DisplaySkewerButton();

}

//Use to set the position where the cursor is in the circle - Value is from 0 to 1
void UQTEOverlay::SetCursorPosition(float Value)
{
	CursorSlider.Get()->SetValue(Value);
}

void UQTEOverlay::RunSkewerFeedbackAnimation(int CurrentTimeZoneIndex, bool Success, bool SecondError)
{
	USkewerTimeZone* TimeZone = Cast<USkewerTimeZone>(SkewerTimeZonesOverlay.Get()
		->GetChildAt(CurrentTimeZoneIndex));

	//OnSkewerAnimationFinishHandler.BindDynamic(this, &UQTEOverlay::RunMinigameResultAnimation);
	TimeZone->RunSuccessOrFailureAnimation(Success, SecondError, OnSkewerAnimationFinishHandler);
}

void UQTEOverlay::DisplaySkewerButton()
{
	DataTableRowHandle.RowName = FName(TEXT("B"));
	SkewerPressButton->SetInputAction(DataTableRowHandle);
	SkewerPressButton->SetVisibility(ESlateVisibility::Visible);
}

void UQTEOverlay::RunSkewerButtonAnimation()
{
	PlayAnimation(SkewerButtonPressAnimation);
}

void UQTEOverlay::RunSkewerStartAnimation()
{
	PlayAnimation(SkewerStartAnimation);
}

void UQTEOverlay::RunInitialTimeZoneAnimations()
{
	TArray<UWidget*> Widgets = SkewerTimeZonesOverlay.Get()->GetAllChildren();

	for (UWidget* Widget : Widgets)
	{
		if (USkewerTimeZone* TimeZone = Cast<USkewerTimeZone>(Widget))
		{
			TimeZone->RunInitialAnimation();
		}
	}
}

// Returns in degrees
float UQTEOverlay::GetHandleEndAngle()
{
	return CursorSlider.Get()->SliderHandleEndAngle;
}
