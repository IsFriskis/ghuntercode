// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ButtonBase.h"
#include "MainMenuButton.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMainMenuButton : public UButtonBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnUnhovered() override;
};
