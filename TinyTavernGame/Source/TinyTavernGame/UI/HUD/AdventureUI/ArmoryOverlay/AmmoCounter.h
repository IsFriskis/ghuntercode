// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "AmmoCounter.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UAmmoCounter : public UTTUserWidget
{
	GENERATED_BODY()
public:
	void UpdateAmmo(float CurrentAmmo, float CurrentMaxAmmo);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoCounter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxAmmoCounter;
};
