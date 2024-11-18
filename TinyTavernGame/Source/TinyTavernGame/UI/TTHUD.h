// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TTHUD.generated.h"

class UWeaponComponent;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UTTUserWidget;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API ATTHUD : public AHUD
{
	GENERATED_BODY()
public:
	//In this function we create the Widget, assign its widget controller, and add it to the viewport.
	void InItOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, UWeaponComponent* WC);
	//This function will create the OverlayWidgetController if it hasn't been created yet, otherwise will return the OverlayWidgetController 
	TObjectPtr<UOverlayWidgetController> GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTTUserWidget> OverlayWidget;
	
private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTTUserWidget> OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
