// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "HoldButton.h"
#include "TurnsInvisibleHoldButton.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTurnsInvisibleHoldButton : public UHoldButton
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void BeginHolding() override;
	virtual void EndHolding() override;
};
