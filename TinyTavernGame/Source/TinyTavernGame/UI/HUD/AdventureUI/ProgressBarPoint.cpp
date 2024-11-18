// TinyTavern


#include "ProgressBarPoint.h"
#include "Components/Image.h"

void UProgressBarPoint::UpdateImage(bool _isPointLoosed)
{
	
	if(_isPointLoosed)
	{
		PointImage.Get()->SetBrushFromTexture(EmptyImage);
		PointLoosed = true;
	}
	else
	{
		PointImage.Get()->SetBrushFromTexture(FullImage);
		PointLoosed = false;
	}
}

void UProgressBarPoint::UpdateImage(EProgress Progress)
{
	switch (Progress)
	{
	case Empty:
		PointLoosed = true;
		PointImage.Get()->SetBrushFromTexture(EmptyImage);
		break;
	case Half:
		PointLoosed = true;
		PointImage.Get()->SetBrushFromTexture(HalfImage);
		break;
	case Full:
		PointLoosed = true;
		PointImage.Get()->SetBrushFromTexture(FullImage);
		break;
	}
}

void UProgressBarPoint::SetFullImage(UTexture2D* Texture)
{
	FullImage = Texture;
}

bool UProgressBarPoint::IsPointLoosed() const
{
	return PointLoosed;
}
