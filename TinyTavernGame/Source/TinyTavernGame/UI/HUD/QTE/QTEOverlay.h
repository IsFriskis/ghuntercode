// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "../Source/TinyTavernGame/Enums/ECuts.h"
#include "../Source/TinyTavernGame/Enums/ETimeZoneStates.h"
#include "../Source/TinyTavernGame/Enums/EButtonToPress.h"
#include "Runtime/AdvancedWidgets/Public/Components/RadialSlider.h"
#include "UMG.h"
#include "Animation/WidgetAnimation.h"
#include "QTEOverlay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinigameAnimationFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinigameInitialAnimationFinished);

class UOverlay;
class UWidgetSwitcher;
class UCommonActionWidget;
class UImage;
class UQTERadialSlider;
class USkewerTimeZone;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UQTEOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetQTEWidget(ECuts Cut);
	void UpdateElapsedTime(float ElapsedTime);
	void RunMinigameResultAnimation(bool Success);
	void RunMinigameResultAnimation();

	void RunSelectionButtonsAnimation();
	void RunMinigameStartAnimation();

	UFUNCTION()
	void OnMinigameInitialAnimationFinished();

	void UpdateQTESelectionBarTimer(float Value);

	UFUNCTION()
	void OnMinigameResultAnimationFinished();
	
	//Slice
	void DisplayNewSliceButton(EButtonToPress ButtonToDisplay);
	void HideSliceButton();
	void UpdateSliceBarTimer(float Value);
	void RunSliceFeedbackAnimation(int AmountOfErrors, bool Success);
	void RunSliceButtonAnimation();


	//Smash
	void UpdateSmashBarPercent(float Percent);
	void UpdateTimerSmashBarPercent(float Value);
	void RunSmashButtonAnimation();
	void DisplaySmashButton();
	void RunSmashButtonBlinkAnimation();
	void StopSmashButtonBlinkAnimation();
	
	//Skewer
	//Set the time window player has to press the button, Max Offset = 310.00, uses degrees(float)
	void SetTimeZones(TArray<FTimeZone> TimeZones, float ZoneSize);
	//Updates the position of the cursor in the circle, value can go from 0 to 1;
	void SetCursorPosition(float Value);
	//Run the animations when player succeeds or fails 
	void RunSkewerFeedbackAnimation(int CurrentTimeZoneIndex, bool Success, bool SecondError);
	void DisplaySkewerButton();
	void RunSkewerButtonAnimation();
	void RunSkewerStartAnimation();
	void RunInitialTimeZoneAnimations();

	float GetHandleEndAngle();

	FWidgetAnimationDynamicEvent OnInitialAnimationFinishedHandler;
	FOnMinigameInitialAnimationFinished OnInitialAnimationFinished;

	FWidgetAnimationDynamicEvent OnSkewerAnimationFinishHandler;
	FWidgetAnimationDynamicEvent OnMinigameAnimationFinishHandler;
	FOnMinigameAnimationFinished OnMinigameAnimationFinished;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT")
	FGameplayTag WeaponTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT")
	FDataTableRowHandle DataTableRowHandle;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MinigameStartAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SmashButtonBlinkAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SkewerStartAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SelectionButtonsAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UQTERadialSlider> MinigameResultBar;

	//QTESelectionPanel
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Slice;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Smash;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Skewer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SliceFlag;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SmashFlag;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SkewerFlag;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UQTERadialSlider> SelectionTimerBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> SliceButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> SmashButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> SkewerButton;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SliceButtonsAnimation;
	
	//SlicePanel
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> Button1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UQTERadialSlider> SliceTimerBar;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SliceButtonPressAnimation;
	
	//SmashPanel
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> SmashPressButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UQTERadialSlider> SmashBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UQTERadialSlider> SmashTimerBar;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SmashButtonPressAnimation;
	
	//SkewerPanel
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActionWidget> SkewerPressButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UQTERadialSlider> BackgroundSkewerBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> SkewerTimeZonesOverlay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URadialSlider> CursorSlider;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SkewerButtonPressAnimation;
	
private:
	ECuts SelectedCut;
	int DashLevel;
};
