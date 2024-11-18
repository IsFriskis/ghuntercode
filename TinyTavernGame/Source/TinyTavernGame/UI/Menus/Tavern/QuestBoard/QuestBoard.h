// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "QuestBoard.generated.h"

class UHorizontalBox;
class UInventory;
class UCanvasPanel;
class UWeaponUpgrade;
class UAmmoUpgrade;
class UHealthUpgrade;
class UMiniCommandUI;
class UGridPanel;
struct FRecipes_Struct;
struct FCommandImage;
struct FLevelRow;
class UHoldButton;
class UScaleBox;
class UScrollBox;
class UCommonTextBlock;
class UButtonBase;
class UCommandUI;
class UImage;
class UTT_GameInstance;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UQuestBoard : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	//Get Number of Commands In Selected Requests to detect if
	//more commands can be selected
	int GetNumberOfCommandsInSelectedRequests();
	void UpdateSelectedRequests();
	//It updates the satisfaction bars of the npc in terms of future
	//value once completed that command. It show the future value of
	//selected requests as well as secondary orders using DetectNPCSatisfactionBar();
	void UpdateSatisfactionBars();
	void HoverSatisfaction(TObjectPtr<UMiniCommandUI> MiniCommand, bool IsHovering);
	UFUNCTION(BlueprintImplementableEvent)
	void QuestBoardTutorialAnimation();
	int GetNumberOfCommandsAvailable(){return NumberOfCommandsAvailable;}
	//Refreshes the where the gamepad points to, for better navigation
	void RefreshMiniCommandTargetForGamepad();
	//Changes Numbers of Selected Commands Text
	UFUNCTION()
	bool AddToSelectedRequests(UMiniCommandUI* MiniCommandUI);
	void NoMoreSelectedPlatesAnimation();
	
protected:
	//TableOfActive/Completed Commands Animation
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> TableOfActiveCommandsAppear;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> OpenInventory;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> GoctorBarShake;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> GailorBarShake;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> GmithBarShake;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SelectedCommandsTextShake;
	//Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventory> Inventory;
	//Mini Commands Places
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand6;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand7;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand8;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMiniCommandUI> MiniCommand9;
	//Selected Commands Places
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> Command1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> Command2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> Command3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> Command4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> Command5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommandUI> Command6;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScaleBox> DeleteBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> DeleteBackgroundImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScaleBox> AcceptBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> AcceptBackgroundImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> SelectedTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHoldButton> AcceptAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButtonBase> DeleteButton;

	//Satisfaction Bars
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHealthUpgrade> GoctorSatisfactionBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> GoctorSatisfiedImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> GoctorSatisfiedText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UAmmoUpgrade> GailorSatisfactionBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> GailorSatisfiedImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> GailorSatisfiedText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWeaponUpgrade> GmithSatisfactionBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> GmithSatisfiedImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> GmithSatisfiedText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Quest Board Info")
	FLinearColor DarkCommandColor = FLinearColor(0.1f,0.1f,0.1f,0.9f);
	//Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Quest Board Info| Sounds")
	TObjectPtr<USoundBase> AcceptedCommandSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Quest Board Info| Sounds")
	TObjectPtr<USoundBase> CommandBackToQuestBoardSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Quest Board Info| Sounds")
	TObjectPtr<USoundBase> ErrorSound;
	//Used to back with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;
	//Used to return command to QuestBoard with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle DeleteInputActionData;
	FUIActionBindingHandle DeleteHandle;
	//Tutorial
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> TutorialMainCanvasPanel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> NextButtonImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> NextButtonBox;
	//Used to continue in tutorial
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ContinueInputActionData;
	FUIActionBindingHandle ContinueHandle;
	//Used to open inventory
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle InventoryInputActionData;
	FUIActionBindingHandle InventoryHandle;

	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	UFUNCTION(BlueprintImplementableEvent)
	void ContinueTutorial();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

private:
	TArray<FLevelRow*> LevelsInfo;
	TArray<FCommandImage*> ImagesOfRecipe;
	TArray<FRecipes_Struct*> Recipes;
	bool InventoryIsOpened = false;
	//Game Instance
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	
	TObjectPtr<UButtonBase> StaringButtonForGamepad;
	TObjectPtr<UCommandUI> HoveredCommand;
	//Commands position that are occupied in Quest Board
	TMap<int,TObjectPtr<UMiniCommandUI>> CommandsInQuestBoard;
	//Selected Commands
	TArray<TObjectPtr<UCommandUI>> SelectedCommands;
	//TArray<bool> CommandsOccupied;
	TMap<FGameplayTag, int> NPCNumberOfCommandsInQuestBoard;
	int NumberOfCommandsAvailable = 3;
	int MaxNumberOfNPCCommands = 2;
	//Progress of Satisfaction Bars
	float GoctorUpgradeProgress = 0;
	float GailorUpgradeProgress = 0;
	float GmithUpgradeProgress = 0;
	//Save satisfaction summed up by all commands
	float GoctorFutureSatisfactionValue = 0;
	float GailorFutureSatisfactionValue = 0;
	float GmithFutureSatisfactionValue = 0;
	//Difficulty of mini command grid
	TArray<int> GoctorDifficultyPossibleCommands = {1,1,2};
	TArray<int> GailorDifficultyPossibleCommands = {1,1,2};
	TArray<int> GmithDifficultyPossibleCommands = {1,2,3};
	TObjectPtr<UCommandUI> CommandToBeReturned;
	//SetUps Mini and Selected Commands
	void SetUpCommands();
	//Deactivates widget, and heads back to the Tavern Menu
	UFUNCTION(BlueprintCallable)
	void ReturnToTavern();

	//Hover Functions for Showing Command Bigger
	UFUNCTION(BlueprintCallable)
	void HoverCommand(UCommandUI* Command);
	UFUNCTION(BlueprintCallable)
	void UnhoverCommand(UCommandUI* Command);

	//Remove Command from Selected Request and bring back to Quest Board
	UFUNCTION()
	void RemoveCommandFromActive();
	//Accept Commands from Selected Request and into ActiveCommands(Book) 
	UFUNCTION()
	void AcceptCommands();

	//Add a Command Randomly to Quest Board
	//TObjectPtr<UCommandUI> SetCommandInRandomPosition(int PlateID, FSlateBrush NPCBrush, FGameplayTag NPCTag);
	TObjectPtr<UMiniCommandUI> SetCommandInRandomPosition(int PlateID, FSlateBrush NPCBrush, FGameplayTag NPCTag);
	void ReturnToQuestBoard(TObjectPtr<UCommandUI> Command);

	//Create Invisible Commands in Uniform Grid to pass information from quest board
	//to the active scroll box
	TObjectPtr<UCommandUI> CreateActiveCommand(UMiniCommandUI* MiniCommandUI);
	//Adds a Command to the Selected Request in the form of completed or pending
	void AddCompleteOrPendingSelectedCommand(TObjectPtr<UMiniCommandUI> MiniCommand, bool IsCompleted = false);
	//Get Image of NPC
	FSlateBrush GetNPCImage(FGameplayTag NPCTag);
	
	//Check for Completed or Active Commands, so that the show
	//torn up in Quest Board
	int ShowActiveCompletedCommands();
	//Out Of All Board Order available, create them and check
	//if NPC has 3 or not and assign them all 3
	void RandomProgressionCommands();
	int GetDifficultyOfCommand(int PlateID);
	TObjectPtr<UMiniCommandUI> GetMiniCommandDependingOnDifficultyLevel(int DifficultyOfPlate, FGameplayTag NPCTag);
	//Search Command With ID
	bool CommandWithIDExists(int ID, FGameplayTag NPCTag);
	//Next Command for gamepad navigation
	//void DetectNextValidButton(UCommandUI* CommandUI);
	//void DetectNextValidButton(UMiniCommandUI* MiniCommandUI);
	//Show Satisfaction Bar depending on Level of Progression
	void VisibilityOfSatisfactionBars();
	//It gets the current progression of satisfaction from all NPC
	void ObtainSatisfactionProgress();
	void DetectNPCSatisfactionBar(FGameplayTag NPCTag, int SatisfactionOfCommand);
	int GetSatisfationOfARecipe(int PlateID);
	//Tutorial of quest board
	void QuestBoardTutorial();
	UFUNCTION(BlueprintCallable)
	void EnableContinueButton(bool ShouldBeEnabled);
	UFUNCTION(BlueprintCallable)
	void EnableExitButton();
	UFUNCTION()
	void ManageInventory();
	void EnableCommands();
	void DisableCommands();
	//Used in tutorial so that player is forced to get Command
	UFUNCTION(BlueprintCallable)
	void GrabCommand();
	UFUNCTION()
	void TutorialCommandGrabbed();
	//Updates the difficulty level of the commands depending on npc satisfaction bar
	void UpdateMiniCommandsInQuestBoardDifficulty();
};
