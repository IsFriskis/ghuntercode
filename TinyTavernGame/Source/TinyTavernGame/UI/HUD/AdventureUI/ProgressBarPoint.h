// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBarPoint.generated.h"

class UImage;
class UTexture2D;

enum EProgress
{
	Empty,
	Half,
	Full
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UProgressBarPoint : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void UpdateImage(bool _isPointLoosed);
	virtual void UpdateImage(EProgress Progress);
	virtual void SetFullImage(UTexture2D* Texture);
	virtual bool IsPointLoosed() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> PointImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TObjectPtr<UTexture2D> FullImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TObjectPtr<UTexture2D> HalfImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TObjectPtr<UTexture2D> EmptyImage;

	bool PointLoosed = false;
	
};
