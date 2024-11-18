// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QTEFinisherWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UQTEFinisherWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UQTEFinisherWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "QTE")
    void ShowWidget();

    UFUNCTION(BlueprintCallable, Category = "QTE")
    void HideWidget();

    UPROPERTY(BlueprintAssignable)
    FOnButtonPressed OnButtonPressed;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

private:

    UFUNCTION()
    void OnQTEButtonPressed();

};
