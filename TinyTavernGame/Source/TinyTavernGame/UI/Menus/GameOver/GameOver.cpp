// TinyTavern


#include "GameOver.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/CommonActivatableWidgetContainer.h"


void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameOver::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGameOver::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	HeadToTavern();
}


void UGameOver::HeadToTavern()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TavernGym");
}
