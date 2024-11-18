// TinyTavern


#include "TTRadialSlider.h"
#include "Components/RadialSlider.h"

void UTTRadialSlider::SetSelectionZone(float ZoneSize, float Offset)
{
	SelectionZoneSlider.Get()->SetSliderHandleEndAngle(ZoneSize);
	SelectionZoneSlider.Get()->SetAngularOffset(Offset);
}

void UTTRadialSlider::RunSuccessOrFailureAnimation(bool Success, bool SecondError, FWidgetAnimationDynamicEvent Event)
{
	if (Success)
	{
		PlayAnimation(SuccessAnimation);
	}
	else
	{
		if (SecondError)
		{
			//BindToAnimationFinished(SecondFailureAnimation, Event);
			PlayAnimation(SecondFailureAnimation);
		}
		else
		{
			PlayAnimation(FailureAnimation);
		}

	}
}
