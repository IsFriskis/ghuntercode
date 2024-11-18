// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "Initialization.generated.h"

class UTitleScreen;
class UTavern;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UInitialization : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetTitleScreen(TObjectPtr<UTitleScreen> _TitleScreen){ TitleScreen = _TitleScreen;}
	
protected:
	UFUNCTION(BlueprintCallable)
	void StartInitialCinematic();

private:
	TObjectPtr<UTitleScreen> TitleScreen;
};
