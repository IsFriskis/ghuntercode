// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ControlListEntry.generated.h"

class UCommonActionWidget;
class UImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UControlListEntry : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetControl(FText ControlText, FDataTableRowHandle ControlImage);
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> ContentTextBlock;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> InputImage;
};
