// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTUserWidget.generated.h"

struct FGameplayAttribute;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTTUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	//This function will set the Widget controller member variable to the param "InWidgetController",
	//it will also call the WidgetControllerSet() Function (event)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	
protected:
	//This function (event) will let us do different behaviour whenever the Widget gets its WidgetController set.
	UFUNCTION(BlueprintNativeEvent)
	void WidgetControllerSet();
	virtual void WidgetControllerSet_Implementation();
};
