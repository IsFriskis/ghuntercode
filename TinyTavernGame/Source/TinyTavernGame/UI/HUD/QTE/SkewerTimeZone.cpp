// TinyTavern


#include "SkewerTimeZone.h"
#include "Components/Image.h"

void USkewerTimeZone::NativeConstruct()
{
	Super::NativeConstruct();

	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		TimeZoneSlider.Get()->SetBrushFromMaterial(DynamicMaterial);
		//BaseAngle = 10 / 335;
	}
}

void USkewerTimeZone::SetSelectionZone(float ZoneSize, float OffsetAngle)
{
	if (DynamicMaterial)
	{
		DynamicMaterial.Get()->SetScalarParameterValue(Parameter_Percent, ZoneSize);
		TimeZoneSlider.Get()->SetRenderTransformPivot(FVector2D(0.5, 0.5));
		DynamicMaterial.Get()->SetScalarParameterValue(Parameter_Angle, OffsetAngle / 335);
	}
}

void USkewerTimeZone::SetImageColor(FLinearColor NewColor)
{
	TintColor = NewColor;
	DynamicMaterial.Get()->SetVectorParameterValue(Parameter_Tint, TintColor);
}

void USkewerTimeZone::InitImageColor()
{
	DynamicMaterial.Get()->SetVectorParameterValue(Parameter_Tint, TintColor);
}

void USkewerTimeZone::RunSuccessOrFailureAnimation(bool Success, bool SecondError, FWidgetAnimationDynamicEvent Event)
{
	if (Success)
	{
		PlayAnimation(SuccessAnimation);
	}
	else
	{
		if (SecondError)
		{
			PlayAnimation(SecondFailureAnimation);
		}
		else
		{
			PlayAnimation(FailureAnimation);
		}

	}
}

void USkewerTimeZone::RunInitialAnimation()
{
	PlayAnimation(TimeZoneInitialAnimation);
}
