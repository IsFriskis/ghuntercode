// TinyTavern


#include "TTRotator.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UTTRotator::SetImages(int MaxAmount, int _CurrentValue)
{
	CurrentValue = _CurrentValue;
	for (int i = 0; i < SettingBox.Get()->GetChildrenCount(); i++)
	{
		TObjectPtr<UImage> rotatorImage = Cast<UImage>(SettingBox.Get()->GetChildAt(i));
		rotatorImage.Get()->SetBrush(UnhoverImage);
		if(i >= MaxAmount)
		{
			rotatorImage.Get()->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	Cast<UImage>(SettingBox.Get()->GetChildAt(CurrentValue))->SetBrush(HoverImage);
}

void UTTRotator::UpdateRotator(int FutureValue)
{
	SetAllImagesToUnHover();
	// if(FutureValue != CurrentValue)
	// {
		TObjectPtr<UImage> rotatorImage = Cast<UImage>(SettingBox.Get()->GetChildAt(FutureValue));
		rotatorImage.Get()->SetBrush(HoverImage);
	// }
}

void UTTRotator::SetAllImagesToUnHover()
{
	for (TObjectPtr<UWidget> rotatorImageWidget : SettingBox.Get()->GetAllChildren())
	{
		TObjectPtr<UImage> rotatorImage = Cast<UImage>(rotatorImageWidget);
		rotatorImage.Get()->SetBrush(UnhoverImage);
	}
}
