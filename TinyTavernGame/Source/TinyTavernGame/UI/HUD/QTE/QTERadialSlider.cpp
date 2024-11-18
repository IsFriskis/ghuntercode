// TinyTavern


#include "QTERadialSlider.h"
#include "Components/Image.h"

void UQTERadialSlider::NativeConstruct()
{
	Super::NativeConstruct();
	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		RadialSliderImage.Get()->SetBrushFromMaterial(DynamicMaterial);
		InitRadialSlider();
		InitImageColor();
	}
}

void UQTERadialSlider::SetRadialSliderPercent(float Percent)
{
	if (DynamicMaterial)
	{
		DynamicMaterial.Get()->SetScalarParameterValue(Parameter_Percent, Percent);
	}
}

void UQTERadialSlider::SetImageColor(FLinearColor NewColor)
{
	TintColor = NewColor;
	DynamicMaterial.Get()->SetVectorParameterValue(Parameter_Tint, TintColor);
}

void UQTERadialSlider::InitImageColor()
{
	DynamicMaterial.Get()->SetVectorParameterValue(Parameter_Tint, TintColor);
}

void UQTERadialSlider::PlayFinalResultAnimation(bool Success, FWidgetAnimationDynamicEvent Callback)
{
	if (Success)
	{
		BindToAnimationFinished(SuccessAnimation, Callback);
		PlayAnimation(SuccessAnimation);
	}
	else
	{
		BindToAnimationFinished(FailureAnimation, Callback);
		PlayAnimation(FailureAnimation);
	}
}

void UQTERadialSlider::PlaySliceFeedbackAnimation(int LivesLeft, bool Success)
{
	// Add Animation based on UI
	if (Success)
	{
		PlayAnimation(SliceSuccessAnimation);
	}
	else
	{
		Lives = LivesLeft;
		OnSliceFeedbackAnimationFinished.BindDynamic(this, &UQTERadialSlider::OnSliceAnimationFinished);
		BindToAnimationFinished(SliceFailureAnimation, OnSliceFeedbackAnimationFinished);
		PlayAnimation(SliceFailureAnimation);
	}
}

void UQTERadialSlider::OnSliceAnimationFinished()
{
	UnbindFromAnimationFinished(SliceFailureAnimation, OnSliceFeedbackAnimationFinished);

	int LifeColor;
	if (Lives > 2)
	{
		LifeColor = 2;
	}
	else
	{
		LifeColor = Lives - 1;
	}

	SetImageColor(SliceErrorsColors[LifeColor]);
}

void UQTERadialSlider::InitRadialSlider()
{
	if (DynamicMaterial)
	{
		if (StartsFull)
		{
			SetRadialSliderPercent(1.0f);
		}
		else
		{
			SetRadialSliderPercent(0.0f);
		}
		RadialSliderImage.Get()->SetRenderTransformPivot(FVector2D(0.5, 0.5));
		DynamicMaterial.Get()->SetScalarParameterValue(Parameter_Angle, OffsetAngle / 360);
	}
}
