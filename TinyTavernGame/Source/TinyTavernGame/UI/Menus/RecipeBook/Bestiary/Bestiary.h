// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "Bestiary.generated.h"

class UEnemyIndex;
class UButtonBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBestiary : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()
	
public:

	// virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEnemyIndex> GamusinoIndex;
};
