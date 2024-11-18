// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkewerTimeZone.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USkewerTimeZone : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetSelectionZone(float ZoneSize, float OffsetAngle);
	void RunSuccessOrFailureAnimation(bool Success, bool SecondError, FWidgetAnimationDynamicEvent Event);
	void RunInitialAnimation();
	void SetImageColor(FLinearColor NewColor);
	void InitImageColor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<UImage> TimeZoneSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
		TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
		FLinearColor TintColor;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SuccessAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FailureAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SecondFailureAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> TimeZoneInitialAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
		TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

private:
	const char* Parameter_Percent = "Percent";
	const char* Parameter_Texture = "Texture";
	const char* Parameter_Tint = "Tint";
	const char* Parameter_Angle = "AngleNormalized";
	float BaseAngle = 0;
};
