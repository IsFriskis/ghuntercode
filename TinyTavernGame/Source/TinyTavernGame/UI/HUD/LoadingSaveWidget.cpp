// TinyTavern


#include "LoadingSaveWidget.h"

void ULoadingSaveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(LoadingAnimation, 0 ,-1.0f);
}

void ULoadingSaveWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	
}
