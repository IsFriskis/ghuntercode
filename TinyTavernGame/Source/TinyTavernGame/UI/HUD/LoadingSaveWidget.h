// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "LoadingSaveWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndLoadingScreen);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API ULoadingSaveWidget : public UTTUserWidget
{
	GENERATED_BODY()

public:
	FOnEndLoadingScreen OnEndLoadingScreen;
protected:
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LoadingAnimation;
	
	virtual void NativeConstruct() override;
	
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
