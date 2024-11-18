// TinyTavern


#include "TTHUD.h"
#include "TTUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "HUD/OverlayWidgetController.h"

void ATTHUD::InItOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, UWeaponComponent* WC)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized, please fill out BP_TTHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized, please fill out BP_TTHUD"));

	//Create the Widget and save it in member variable.
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UTTUserWidget>(Widget);

	//Get the Widget Controller
	const FWidgetControllerParams WidgetControllerParams(PC,ASC,AS,WC);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	//Assign the OverlayWidgetController that we just get to the OverlayWidget member Variable.
	OverlayWidget->SetWidgetController(WidgetController);

	//Broadcast initial values of the attributes and Bind the OnAttributeChangeDelegates of the ASC to the WidgetController
	WidgetController->BroadCastInitialValues();
	WidgetController->BindCallbacksToDependencies();
	
	//Add Widget to the ViewPort
	Widget->AddToViewport();
}

TObjectPtr<UOverlayWidgetController> ATTHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	//If OverlayWidgetController is null, create one and return it.
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		return OverlayWidgetController;
	}
	//If OverlayWidgetController isn't null, return it.
	return OverlayWidgetController;
}
