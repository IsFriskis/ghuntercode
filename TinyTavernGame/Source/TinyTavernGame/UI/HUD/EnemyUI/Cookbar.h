// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cookbar.generated.h"

class UImage;
class UTexture2D;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCookbar : public UUserWidget
{
	GENERATED_BODY()
	
public:


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CookbarImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TObjectPtr<UTexture2D> RawImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TObjectPtr<UTexture2D> CookedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TObjectPtr<UTexture2D> BurntImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TObjectPtr<UTexture2D> BrokenImage;
};
