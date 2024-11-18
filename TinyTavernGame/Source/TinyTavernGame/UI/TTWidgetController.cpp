// TinyTavern


#include "TTWidgetController.h"

void UTTWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
	WeaponComponent = WCParams.WeaponComponent;
}

void UTTWidgetController::BroadCastInitialValues()
{
	
}

void UTTWidgetController::BindCallbacksToDependencies()
{
	
}
