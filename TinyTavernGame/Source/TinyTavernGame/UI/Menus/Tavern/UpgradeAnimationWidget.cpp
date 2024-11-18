// TinyTavern


#include "UpgradeAnimationWidget.h"

void UUpgradeAnimationWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	OnUpgradeAnimationFinished.Broadcast();
}
