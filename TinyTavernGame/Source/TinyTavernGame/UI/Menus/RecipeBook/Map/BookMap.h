// TinyTavern

#pragma once
#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "BookMap.generated.h"

class UImage;

USTRUCT(BlueprintType)
struct FMapPositionIcons
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UImage> ImageMapPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedSignature);

class APlayerCharacter;
class UBackgroundBlur;
class UImage;
class UTPButton;
class UCanvasPanel;
class UTT_GameInstance;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBookMap : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void ShowButtons(int CurrentTPID);
	void SetShouldManageClose();

	UPROPERTY()
	FOnButtonClickedSignature OnButtonClicked;
	
protected:
	//For Backing with Gamepad
	UPROPERTY(EditDefaultsOnly, Category = "TT| Recipe Book Info")
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> RecipeBookImage;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBackgroundBlur> Blur;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BoilTotemImage;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BoilTotemOutline;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> FryTotemImage;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> FryTotemOutline;
	
	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnTpFadeOutAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnTpFadeInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BoilTotemBlink;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FryTotemBlink;

	//TP Buttons
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP1_Taberna;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP2_Cascada;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP3_Cruce;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP4_Campamento;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP5_Mercado;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP6_Pozo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP7_Zona3;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP8_CentroZona3;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTPButton> TP9_Laberinto;
	
	//PositionIcons

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> GhunterPositionMap;
	
	UPROPERTY()
	TArray<FMapPositionIcons> MapPositionIcons;

	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UFUNCTION()
	void TeleportPlayer();
	UFUNCTION()
	void OnTPButtonClicked(UTPButton* Button);
	UFUNCTION()
	void OnFadeOutAnimationFinished();
	
private:
	UFUNCTION()
	void BackLogic();
	
	FWidgetAnimationDynamicEvent FadeInAnimationEvent;
	FWidgetAnimationDynamicEvent FadeOutAnimationEvent;
	TObjectPtr<UTPButton> ClickedButton;
	UPROPERTY()
	TArray<TObjectPtr<UTPButton>> TPButtons;
	UPROPERTY()
	TObjectPtr<UTT_GameInstance> GameInstance;
	//Changes the navigation of tp buttons to navigate only
	//in the visible ones
	void SetNavigationOfTPButtons();
	void TP1Navigation();
	void TP2Navigation();
	void TP3Navigation();
	void TP4Navigation();
	void TP5Navigation();
	void TP6Navigation();
	void TP7Navigation();
	void TP8Navigation();
	void TP9Navigation();
};
