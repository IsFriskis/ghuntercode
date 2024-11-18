// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QTERadialSlider.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UQTERadialSlider : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void InitRadialSlider();
	void SetRadialSliderPercent(float Percent);
	void SetImageColor(FLinearColor NewColor);
	void InitImageColor();
	void PlayFinalResultAnimation(bool Success, FWidgetAnimationDynamicEvent Callback);
	void PlaySliceFeedbackAnimation(int LivesLeft, bool Success);

	UFUNCTION()
	void OnSliceAnimationFinished();

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FailureAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SuccessAnimation;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<UImage> RadialSliderImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
		TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
		FLinearColor TintColor;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SliceFailureAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SliceSuccessAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings", meta = (ToolTip = "Color displayed at X Mistakes"))
		TArray<FLinearColor> SliceErrorsColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
		float OffsetAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
		bool StartsFull = true;

		FWidgetAnimationDynamicEvent OnSliceFeedbackAnimationFinished;

private:
	const char* Parameter_Percent = "Percent";
	const char* Parameter_Texture = "Texture";
	const char* Parameter_Tint = "Tint";
	const char* Parameter_Angle = "AngleNormalized";
	int Lives = 0;
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
};
