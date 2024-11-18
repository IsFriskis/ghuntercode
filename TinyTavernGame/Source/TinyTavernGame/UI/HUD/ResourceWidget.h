// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "../TTUserWidget.h"
#include "ResourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UResourceWidget : public UTTUserWidget
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UCommonActionWidget> InputActionWidget;

};
