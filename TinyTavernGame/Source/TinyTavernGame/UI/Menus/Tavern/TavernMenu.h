// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArticyBaseTypes.h"
#include "TinyTavernGame/UI/Menus/ConfirmMenu.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "TavernMenu.generated.h"

class UUpgradeAnimationWidget;
class UImage;
class UHorizontalBox;
class UCanvasPanel;
class UCommandUI;
struct FLevelRow;
class ATTSpriteActor;
class UTT_GameInstance;
class ATavernLevel;
class UCommonActivatableWidgetStack;
class UConfirmMenu;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTavernMenu : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetTavernSpriteActors(TObjectPtr<ATTSpriteActor> _DoorSprite,TObjectPtr<ATTSpriteActor> _QuestBoardSprite, TObjectPtr<ATTSpriteActor> _GavernSprite, TObjectPtr<ATTSpriteActor> _GoctorSprite, TObjectPtr<ATTSpriteActor> _GailorSprite, TObjectPtr<ATTSpriteActor> _GmithSprite, TObjectPtr<ATTSpriteActor> _GookerSprite, TObjectPtr<ATTSpriteActor> _RecipeBookSprite);
	void Level0();
	void Level1();
	void Level2();
	
protected:
	//Animations
	//Level 0
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial1Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial2Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial2BAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> GookerTalksAnimation;
	//Level 1
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial4Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial5Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial5BAnimation;
	//Level 2
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial6Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Tutorial7Animation;
	//More Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MainCommandsAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MainCommandsExplanation1Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MainCommandsExplanation2Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MainCommandsExplanation3Animation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowUpgradesAnimation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUpgradeAnimationWidget> UnlockedWeapons;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> TutorialMainCanvas;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NextButtonImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> NextButtonBox;
	
	//Stack
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> TavernMenuStack;
	//Buttons
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> AdventureButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> QuestBoardButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> RecipeBookButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> NPCGmithButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> NPCGavernButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> NPCGailorButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> NPCGoctorButton;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> EasterEggButton;

	//Main Commands
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand1;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand2;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand3;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand4;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand5;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> MainCommand6;

	//Upgrades
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrades")
	FSlateBrush MeleeWeaponLevel2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Upgrades")
	FSlateBrush MeleeWeaponLevel3;
	
	//Confirm Menu When Leaving Door
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UConfirmMenu> TavernConfirmMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu| Not Accepted Order")
	FText TitleConfirmMenuNoAcceptedOrders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu| Not Accepted Order")
	FText DescriptionConfirmMenuNoAcceptedOrders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu| Not Accepted Order")
	FConfirmButtonOption ConfirmMenuOptionNoAcceptedOrders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu| Order Not Given")
	FText TitleConfirmMenuOrderNotGiven;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu| Order Not Given")
	FText DescriptionConfirmMenuOrderNotGiven;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Confirm Menu| Order Not Given")
	FConfirmButtonOption ConfirmMenuOptionOrderNotGiven;
	

	//Gooker extra dialogs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Gooker Dialogs Tutorial")
	FArticyId GookerShowingCommands;

	//With click continue on tutorial
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ContinueInputActionData;
	FUIActionBindingHandle ContinueHandle;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	//Tavern Level
	TObjectPtr<ATavernLevel> TavernLevel;
	//Variable used to detect the current hovered button
	TObjectPtr<UButtonBase> HoveredButton;
	//TTSprite Actors
	TObjectPtr<ATTSpriteActor> DoorSprite;
	TObjectPtr<ATTSpriteActor> QuestBoardSprite;
	TObjectPtr<ATTSpriteActor> GavernSprite;
	TObjectPtr<ATTSpriteActor> GoctorSprite;
	TObjectPtr<ATTSpriteActor> GailorSprite;
	TObjectPtr<ATTSpriteActor> GmithSprite;
	TObjectPtr<ATTSpriteActor> GookerSprite;
	TObjectPtr<ATTSpriteActor> RecipeBookSprite;
	//Desired Focus Target
	TObjectPtr<UButtonBase> GamepadButton;
	//Delegates
	FDelegateHandle OpenQuestDelegate;
	FDelegateHandle HeadToAdventureDelegate;
	//Level Database
	TArray<FLevelRow*> LevelRows;
	//Keep track of tutorial animation
	uint16_t TutorialNumber = 0;
	bool TalkedToGavernInTuto = false;
	bool ShouldShowSecondPartOfLevel2 = false;
	//Shows the Quest Board Widget
	UFUNCTION()
	void OpenQuestBoard();
	//Teleports player to the level
	UFUNCTION()
	void HeadToAdventure();
	//Shows NPC Gmith Widget
	UFUNCTION()
	void OpenGmithWidget();
	//Shows NPC Goctor Widget
	UFUNCTION()
	void OpenGoctorWidget();
	//Shows NPC Gailor Widget
	UFUNCTION()
	void OpenGailorWidget();
	//Shows NPC Gavern Widget
	UFUNCTION()
	void OpenGavernWidget();
	//Shows Recipe Book Widget
	UFUNCTION()
	void OpenRecipeBookWidget();
	//Shows Easter Egg
	UFUNCTION()
	void ShowEasterEggWidget();
	//Changes visibility of Tavern Sprite Actors
	UFUNCTION()
	void OnHoverDoor();
	UFUNCTION()
	void OnUnhoverDoor();
	UFUNCTION()
	void OnHoverQuestBoard();
	UFUNCTION()
	void OnUnhoverQuestBoard();
	UFUNCTION()
	void OnHoverGavern();
	UFUNCTION()
	void OnUnhoverGavern();
	UFUNCTION()
	void OnHoverGoctor();
	UFUNCTION()
	void OnUnhoverGoctor();
	UFUNCTION()
	void OnHoverGailor();
	UFUNCTION()
	void OnUnhoverGailor();
	UFUNCTION()
	void OnHoverGmith();
	UFUNCTION()
	void OnUnhoverGmith();
	UFUNCTION()
	void OnHoverRecipeBook();
	UFUNCTION()
	void OnUnhoverRecipeBook();
	UFUNCTION()
	void OnHoverEasterEgg();
	UFUNCTION()
	void OnUnhoverEasterEgg();
	UFUNCTION()
	void AdvanceInTutorial();
	UFUNCTION()
	void TavernShowGooker();
	void GookerShowsUp();
	//Show Upgrades
	void ShowUpgrades();
	UFUNCTION()
	void EndShowUpgrades();
	UFUNCTION()
	void EndShowingUpgrade();
	//Sets Main Orders
	void LoadMainOrders();
	void ShowMainOrders();
	UFUNCTION()
	void ContinueWithTavernAfterMainCommands();
	UFUNCTION()
	void HeadToGavernTutorial();
	void ReturnVisibilityOfButtonsAfterMainCommands();
	//Checks if player has completed any secondary dishes
	//if so, add notification to NPC if one or more of th
	//orders is from them
	void CheckForCompletedOrders();
	void EstablishNavigation();
};
