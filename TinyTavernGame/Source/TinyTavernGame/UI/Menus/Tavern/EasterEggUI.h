// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "EasterEggUI.generated.h"

class ATavernLevel;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UEasterEggUI : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

protected:
	//Used to back with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;

	void NativeConstruct() override;
	void NativeDestruct() override;
	
private:
	TObjectPtr<ATavernLevel> TavernLevel;
	UFUNCTION()
	void BackLogic();
};
