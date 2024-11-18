// TinyTavern


#include "AmmoCounter.h"
#include "Components/TextBlock.h"


void UAmmoCounter::UpdateAmmo(float CurrentAmmo, float CurrentMaxAmmo)
{
	AmmoCounter.Get()->SetText(FText::FromString(FString::Printf(TEXT("%i"), FMath::RoundToInt(CurrentAmmo/10))));
	MaxAmmoCounter.Get()->SetText(FText::FromString(FString::Printf(TEXT("%i"), FMath::RoundToInt(CurrentMaxAmmo/10))));
}
