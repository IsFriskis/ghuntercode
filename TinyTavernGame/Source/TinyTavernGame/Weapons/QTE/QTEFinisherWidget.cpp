// TinyTavern


#include "QTEFinisherWidget.h"
#include "Components/Button.h"

UQTEFinisherWidget::UQTEFinisherWidget(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
}

void UQTEFinisherWidget::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UQTEFinisherWidget::HideWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UQTEFinisherWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UQTEFinisherWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);
}

void UQTEFinisherWidget::OnQTEButtonPressed()
{
    // Handle QTE success logic here
    HideWidget();
    // Call a delegate or event to notify the game mode or player controller
}
