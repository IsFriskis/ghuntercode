// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/Object.h"
#include "TTWidgetController.generated.h"

class UWeaponComponent;
class UAttributeSet;
class UAbilitySystemComponent;

//This Struct is Use to save the params needed to set the member variables of a WidgetController.
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC,UAbilitySystemComponent* ASC, UAttributeSet* AS, UWeaponComponent* WC)
		: PlayerController(PC),AbilitySystemComponent(ASC) ,AttributeSet(AS), WeaponComponent(WC){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWeaponComponent> WeaponComponent = nullptr;
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTTWidgetController : public UObject
{
	GENERATED_BODY()

public:
	
	//This function will Set the WidgetController member variables using the struct WidgetControllerParams
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	//This function will set the initial values to display in the widget that is controlling.
	virtual void BroadCastInitialValues();
	// This function will bind the OnAttributeChangeDelegates of the ASC to the WidgetController
	virtual void BindCallbacksToDependencies();
protected:

	//Variables needed to Broadcast values to the widgets
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UWeaponComponent> WeaponComponent;
};
