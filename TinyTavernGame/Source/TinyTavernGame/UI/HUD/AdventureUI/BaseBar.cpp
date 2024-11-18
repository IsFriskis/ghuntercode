// TinyTavern


#include "BaseBar.h"
#include "Components/ProgressBar.h"

void UBaseBar::SetPercent()
{
	//ProgressBar.Get()->SetPercent(CurrentAttributeValue/MaxAttributeValue);
}

void UBaseBar::OnAttributeChange(float newAttributeValue)
{
	CurrentAttributeValue = newAttributeValue;
	SetPercent();
}

void UBaseBar::OnMaxAttributeChange(float newMaxAttributeValue)
{
	MaxAttributeValue = newMaxAttributeValue;
	SetPercent();
}
