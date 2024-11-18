// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TT_CommonActivatableWidget.h"
#include "ConfirmMenu.generated.h"

class UTT_GameInstance;
class UTitleScreen;
class UButtonBase;
class UCommonTextBlock;

UENUM(BlueprintType)
enum class FConfirmButtonOption : uint8
{
	AccessAdventureLevel,
	AccessTitleScreen,
	ExitGame,
	NewGame
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UConfirmMenu : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	//Modifies the content displayed by the confirm dialog menu
	void AddConfirmMenu(FText Title, FText Description, FConfirmButtonOption _ConfirmButtonOption);
	void SetTitleScreen(TObjectPtr<UTitleScreen> _TitleScreen){ TitleScreen = _TitleScreen;}
	
protected:
	//Text Box
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TitleTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> DescriptionTextBlock;
	//Buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> ConfirmButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> CancelButton;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
private:
	FDelegateHandle CancelDelegate;
	FDelegateHandle ExitGameDelegate;
	FConfirmButtonOption ConfirmButtonOption;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<UTitleScreen> TitleScreen;
	
	//Assigns the "Confirm" Button it's utility(example: Exit the Game)
	void FigureOutConfirmButtonPurpose(FConfirmButtonOption ConfirmButtonOption);
	//Cancel the action deactivating the widget
	UFUNCTION()
	void Cancel();
	UFUNCTION()
	void ExitGame();
	UFUNCTION()
	void AccessAdventure();
	UFUNCTION()
	void AccessTitleScreen();
	UFUNCTION()
	void NewGame();
};
