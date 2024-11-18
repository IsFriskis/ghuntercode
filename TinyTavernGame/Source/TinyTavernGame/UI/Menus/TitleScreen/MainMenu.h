// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "../ConfirmMenu.h"
#include "../TT_CommonActivatableWidget.h"
#include "MainMenu.generated.h"

class UTT_GameInstance;
class UTitleScreen;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConfirmDialog);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMainMenu : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	void SetTitleScreenParent(TObjectPtr<UTitleScreen> NewTitleScreen);
	UFUNCTION(BlueprintCallable)
	void WelcomeToGarcosa();
	
protected:
	//Buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> NewGameButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ContinueButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> OptionsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> CreditsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ExitButton;
	//-------------------------------------------------------------------------

	//Level
	UPROPERTY(EditAnywhere, Category = "TT| Level")
	FName PlayLevel;

	//Exit Confirm Menu Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText TitleConfirmMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText DescriptionConfirmMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FConfirmButtonOption ConfirmMenuOption;
	//New Game Confirm Menu Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText NewGameTitleConfirmMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FText NewGameDescriptionConfirmMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu")
	FConfirmButtonOption NewGameConfirmMenuOption;

	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:
	TObjectPtr<UTitleScreen> TitleScreen;
	FDelegateHandle StartGameDelegate;
	FDelegateHandle OpenOptionsMenuDelegate;
	FDelegateHandle ExitMenuDelegate;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	UFUNCTION()
	void NewGame();
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void OpenOptionsMenu();
	UFUNCTION()
	void OpenExitMenu();
	UFUNCTION()
	void OpenCredits();
};
