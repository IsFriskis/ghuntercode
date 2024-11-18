// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "SLoading.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT | Loading Screen")
	//FSlateBrush BackgroundBrush;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//TSharedPtr<SLoading> LoadingSlate;
	
};
