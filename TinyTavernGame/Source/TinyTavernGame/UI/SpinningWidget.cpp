// TinyTavern


#include "SpinningWidget.h"

#include "Components/Image.h"

void USpinningWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ShouldSpin)
	{
		SpinningBlend.SetBlendTime(SpinningTime);
		SpinningBlend.SetValueRange(SpinningMinValue,SpinningMaxValue);
	}
}

void USpinningWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if(ShouldSpin)
	{
		SpinningBlend.Update(InDeltaTime);
		SpinningImage.Get()->SetRenderTransformAngle(SpinningBlend.GetBlendedValue());
		if(SpinningBlend.IsComplete())
		{
			SpinningBlend.Reset();
		}
	}
}
