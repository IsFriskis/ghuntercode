// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"

#include "InitialCinematic.generated.h"


class UTT_GameInstance;
class USkipButton;
class UTitleScreen;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UInitialCinematic : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetTitleScreen(TObjectPtr<UTitleScreen> _TitleScreen);
	UFUNCTION(BlueprintCallable)
	bool IsFirstTime();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle SkipInputActionData;
	FUIActionBindingHandle SkipHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USkipButton> SkipButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USkipButton> SkipButtonInvisible;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Intro;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Sound")
	TObjectPtr<USoundBase> InitialCinematicSound;
	
	UFUNCTION(BlueprintCallable)
	void HeadToMainMenu();
	UFUNCTION(BlueprintCallable)
	void ShowSkipButton();
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	TObjectPtr<UTitleScreen> TitleScreen;
	bool Skipped = false;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<UAudioComponent> NarrationAudioComponent;
};
