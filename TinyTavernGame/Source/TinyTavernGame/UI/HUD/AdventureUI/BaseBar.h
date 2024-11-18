// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "BaseBar.generated.h"

class USizeBox;
class UProgressBar;
class UEnemyCookBar;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBaseBar : public UTTUserWidget
{
	GENERATED_BODY()

public:
	TObjectPtr<UEnemyCookBar> GetEnemyCookbar() { return EnemyCookBar; }

protected:
	
	virtual void SetPercent();

	UFUNCTION(BlueprintCallable)
	void OnAttributeChange(float newAttributeValue);
	
	UFUNCTION(BlueprintCallable)
	void OnMaxAttributeChange(float newMaxAttributeValue);
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEnemyCookBar> EnemyCookBar;
	

private:
	float CurrentAttributeValue;
	float MaxAttributeValue;
};
