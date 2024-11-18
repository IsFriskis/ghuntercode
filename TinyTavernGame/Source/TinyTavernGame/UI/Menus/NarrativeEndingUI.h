// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "NarrativeEndingUI.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UNarrativeEndingUI : public UTTUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	TObjectPtr<UMaterialInstance> DissolveMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	float DissolveTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Note Button Settings")
	FAlphaBlend EndingBlend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NarrativeVideoImage;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_FinishedDissolve();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	TObjectPtr<UMaterialInstanceDynamic> DynamicMat;
	bool ShouldImageDissolve = false;
};
