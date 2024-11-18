// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ArticyBaseTypes.h"
#include "ArticyFlowPlayer.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"
#include "NPCDialog.generated.h"

class UNPCOptionButton;
class IArticyObjectWithDisplayName;
struct FRecipes_Struct;
class UCommandUI;
class UPaperSprite;
class UTutorial_Dialogue_1st_DraftGlobalVariables;
class UImage;
class UVerticalBox;
class UUpgradeUI;
class ATavernLevel;
class UNPCQuestionButton;
class UTT_GameInstance;
class UButtonBase;
class AUIPlayer;
class UDialogButton;
class UArticyFlowPlayer;
class IArticyFlowObject;
class UCommonTextBlock;
class UCommonHierarchicalScrollBox;

USTRUCT(BlueprintType)
struct FNarrativeDialog : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SourceString;
};
USTRUCT(BlueprintType)
struct FNPCExpressions : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName VariableName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSprite> HeadExpression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> ExpressionSound;
};
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UNPCDialog : public UTT_CommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HandInCommands;
	
	void SetInAQuestion(bool _InAQuestion){InAQuestion = _InAQuestion;}
	//Tuto
	void PlayCertainDialog(FArticyId ArticyID);
	UFUNCTION(BlueprintImplementableEvent)
	void GavernTutorialAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void ContinueTutorial();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveExtraPlateToGavernInTutorial();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EndShowingPlateToGavernInTutorial();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ShowUpgrade(EGavernUpgrades GavernUpgrade = NOUPGRADE);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ShowWheel();
	//Shows Upgrade Dialog
	UFUNCTION(BlueprintCallable)
	void EndShowingUpgrade();
	//NPC tells the upgrade dialog and afterward, show upgrade animation
	void ShowUpgradeText();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EndGameDecisionAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_IncreasingSatisfactionBar(int FutureValue, int MaxValue);
	void ChangeGamepadCurrentCutton(TObjectPtr<UButtonBase> NewButton);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	//Widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonHierarchicalScrollBox> NPCScrollBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> DialogImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> PreviousTextVerticalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUpgradeUI> UpgradeUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDialogButton> DialogButton1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDialogButton> DialogButton2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDialogButton> DialogButton3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDialogButton> DialogButton4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDialogButton> DialogButton5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UDialogButton> DialogButton6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCQuestionButton> QuestionsButton9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCOptionButton> UpgradeButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCOptionButton> QuestionButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCOptionButton> ExitButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNPCOptionButton> BackButton;
	//Commands to show
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Main Info", meta = (Categories = "NPC."))
	FGameplayTag NPCName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Main Info")
	FSlateBrush NPCImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FText UpgradeText = FText::FromString("Upgrade My Gear");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FText UpgradeRelicNeededText = FText::FromString("Upgrade(Item Required)");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FSlateColor UpgradeRelicNeededTextColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FText GivePlateText = FText::FromString("Give Plates");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FText QuestionsText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FText ExitText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FText RetreatToQuestionsText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	FText BackToMainMenuText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	TObjectPtr<UDataTable> NPCTextDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	TObjectPtr<UDataTable> PreviousNPCTextDataTable;
	
	//Articy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	TArray<FArticyId> GreetingDialogs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	TArray<FArticyId> FailedMainCommandDialogs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	TArray<FArticyId> UpgradeDialogs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Dialog Info")
	float GhunterSpeakingTime = 0.5f;

	//Commands Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Command Info")
	float BlendTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Sounds")
	TObjectPtr<USoundBase> ErrorSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Sounds")
	TArray<TObjectPtr<USoundBase>> NormalNPCSounds;

	//Gavern Upgrade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	FArticyId GavernTutorialDialog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	TSubclassOf<UCommonTextStyle> NormalTextStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	FArticyId ShieldUpgradeDialog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	TSubclassOf<UCommonTextStyle> ShieldTextStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	FArticyId ExtraReputationUpgradeDialog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	TSubclassOf<UCommonTextStyle> ExtraReputationTextStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	FArticyId PuffballUpgradeDialog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gavern")
	TSubclassOf<UCommonTextStyle> PuffballResistanceTextStyle;

	//Gooker Death Dialog
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gooker")
	FArticyId DeathWithNoMainOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| NPC Gooker")
	TArray<FArticyId> DeathWithAllMainPlates;
	
	//Used to back with Gamepad
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;
	FUIActionBindingHandle BackHandle;
	//Used to continue in Gavern tutorial
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ContinueInputActionData;
	FUIActionBindingHandle ContinueHandle;
	
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	//During animation of "HandInCommands", check the amount of commands to give
	//if 3 or below, stop the animation before time
	UFUNCTION(BlueprintCallable)
	void CheckAmountOfCommands();
	UFUNCTION(BlueprintCallable)
	void HeadToLastLevel();

private:
	TObjectPtr<ATavernLevel> TavernLevel;
	TObjectPtr<AUIPlayer> UIPlayer;
	TObjectPtr<UArticyFlowPlayer> PlayerArticyFlowPlayer;
	TObjectPtr<UTutorial_Dialogue_1st_DraftGlobalVariables> ArticyGlobalVariables;
	TArray<FArticyId> DialogsIDAvailable;
	TArray<TObjectPtr<UButtonBase>> NPCButtons;
	TArray<FLevelRow*> LevelRows;
	TArray<FRecipes_Struct*> Orders;
	TArray<FNPCExpressions*> GmithExpressions;
	TArray<FNPCExpressions*> GoctorExpressions;
	TArray<FNPCExpressions*> GailorExpressions;
	TArray<FNPCExpressions*> GavernExpressions;
	TArray<FNPCExpressions*> GookerExpressions;
	bool InUpgradeDialog = false;
	bool PlayerCompletedAllTiers = false;
	//Detect when Ghunter is speaking
	bool GhunterTalking = false;
	FArticyBranch GhunterBranchToAutoPlay;
	//Button where gamepad is at. It refreshes after each interaction with dialog system
	TObjectPtr<UButtonBase> GamepadCurrentButton;
	//Variable used to know if player is asking a question to the NPC
	//so that when dialog finishes, it can return to the questions menu
	//and not the main npc option menu
	bool InAQuestion = false;
	//Max Value of Satisfaction bar depending on the current satisfaction level of an NPC
	float MaxSatisfactionBarValue = 6;
	TArray<float> SatisfactionBarMaxValues = {100, 120, 150};
	//Depending on the number of commands completed, the bar will fill more or less
	float ProgressToFill = 0;
	FAlphaBlend NPCSatisfactionBarBlend;
	bool FinishedSatisfactionBarAnimation = true;
	float CurrentProgressValue;
	FTimerHandle TimerHandle;
	//Variable so that NPC tells a dialog if first time taking to him when entering tavern
	bool FirstTime = true;
	bool GivingExtraPlateInTutorial = false;
	IArticyObjectWithDisplayName* ArticySpeakerDisplayName;
	bool IsGhunterSpeaking = false;
	//Keeps a track of how many commands are going to be offer to the NPC
	uint32_t NumberOfCommandsToGive = 0;
	//When player hit the Upgrade Button
	bool TryUpgrade = false;
	//Variables that once enter an NPC refresh focus
	//Widget loses focus do to spamming
	bool EnableFocusTimer = true;
	float FocusTimer = 0;
	//Narrative Functions
	UFUNCTION()
	void ContinueDialog(TScriptInterface<IArticyFlowObject> ArticyObject);
	UFUNCTION()
	void UpdateBranches(const TArray<FArticyBranch>& AvailableBranches);
	//Function that show upgrade panel unless it's Gavern, which will take all
	//of the extra dishes
	UFUNCTION()
	void ShowUpgrades();
	UFUNCTION()
	void ShowQuestions();
	UFUNCTION(BlueprintCallable)
	void ReturnToTavern();
	UFUNCTION()
	void HeadBack();
	void CreateMainNPCMenu();
	//Command Functions
	//Functions that checks completed commands that belong to this NPC
	void CheckInitialNPCInformation();
	bool GiveSecondaryPlates();
	void RemoveCommandFromQuestBoard(FSecondaryCommand FinishedCommand);
	//Obtain satisfaction depending on command id
	int GetSatisfaction(int PlateID, bool WasPickedInGavernDay = false, bool WasPickedPremium = false);
	//Update Main Order, Checks new NPCs and new Weapons
	void UpdateProgression();
	//Updates the npc Satisfaction bar value, and level of progression
	void UpdateNPCSatisfactionValues(bool LevelIncrease);
	//Saves buttons in an array
	void SetButtons();
	//Set All buttons invisible
	void HideButtons();
	//Emptys arrays and removes bindings
	void EmptyNPCButtons();
	bool AreAllButtonsInvisible();
	TObjectPtr<UDialogButton> GetFirstDialogButtonInvisible();
	TObjectPtr<UButtonBase> GetFirstInvisibleButton();
	TObjectPtr<UNPCQuestionButton> GetFirstNPCQuestionInvisibleButton();
	//Updates text informing of the  Gavern Upgrade obtained
	void ShowGavernUpgrade(EGavernUpgrades GavernUpgrade);
	void UpdateSatisfactionBar();
	//Expressions
	void ChangeExpression();
	void AddNewText(FText NewText, FText SpeakerName);
	void EmptyAllText();
	//Tutorial Gavern
	void GavernTutorial();
	UFUNCTION(BlueprintCallable)
	void EnableContinueButton(bool ShouldBeEnabled);
	UFUNCTION(BlueprintCallable)
	void OfferPlateInTutorial();
	UFUNCTION()
	void GiveExtraPlateToGavernInTutorial();
	UFUNCTION()
	void GhunterFinishedTalking();
	//When plates offer, remove notification from the top of NPCs
	void RemoveNotificationFromNPC();
	//Logic for what Gooker should say:
	//death dialog, main story dialog or tutorial dialog
	void GookerDialog(bool AnimationNeeded = true);
	//Sets the text of the upgrade button
	void UpdateUpgradeButton();
};
