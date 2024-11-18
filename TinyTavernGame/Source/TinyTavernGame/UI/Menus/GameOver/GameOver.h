// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "../TT_CommonActivatableWidget.h"
#include "../ConfirmMenu.h"	
#include "GameOver.generated.h"

class UConfirmMenu;
class UCommonActivatableWidgetStack;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UGameOver : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnimation;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);
	
private:
	UFUNCTION()
	void HeadToTavern();
};
