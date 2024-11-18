// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "TTRotator.generated.h"


class UImage;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTTRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
	void SetImages(int MaxAmount, int _CurrentValue);
	void UpdateRotator(int FutureValue);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SettingBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Rotator")
	FSlateBrush UnhoverImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Rotator")
	FSlateBrush HoverImage;

private:
	int CurrentValue = 0;
	void SetAllImagesToUnHover();
};
