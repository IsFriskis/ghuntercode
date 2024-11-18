// TinyTavern


#include "NPCDialog.h"

#include "ArticyFlowPlayer.h"
#include "UpgradeUI.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Input/CommonUIInputTypes.h"
#include "Interfaces/ArticyFlowObject.h"
#include "Interfaces/ArticyNode.h"
#include "Interfaces/ArticyObjectWithText.h"
#include "TinyTavernGame/Actor/TTSpriteActor.h"
#include "TinyTavernGame/ArticyGenerated/Tutorial_Dialogue_1st_DraftArticyTypes.h"
#include "TinyTavernGame/ArticyGenerated/Tutorial_Dialogue_1st_DraftDatabase.h"
#include "TinyTavernGame/ArticyGenerated/Tutorial_Dialogue_1st_DraftGlobalVariables.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Levels/TavernLevel.h"
#include "TinyTavernGame/Player/UIPlayer/UIPlayer.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Buttons/DialogButton.h"
#include "TinyTavernGame/UI/Menus/Buttons/NPCQuestionButton.h"
#include "CommonRichTextBlock.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/UI/Menus/Tavern/QuestBoard/CommandUI.h"
#include "CommonHierarchicalScrollBox.h"
#include "Input/UIActionBinding.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/UI/Menus/Buttons/NPCOptionButton.h"

void UNPCDialog::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetButtons();
	HideButtons();
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	UIPlayer = Cast<AUIPlayer>(GetOwningPlayerPawn());
	PlayerArticyFlowPlayer = UIPlayer.Get()->PlayerArticyFlowPlayer;
	UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->OnPlayerPaused.AddDynamic(this, &UNPCDialog::ContinueDialog);
	UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->OnBranchesUpdated.AddDynamic(this, &UNPCDialog::UpdateBranches);
	ArticyGlobalVariables = static_cast<UTutorial_Dialogue_1st_DraftGlobalVariables*>(UArticyGlobalVariables::GetDefault(GetWorld()));
	//Get reference to level
	TavernLevel = Cast<ATavernLevel>(GetWorld()->GetLevelScriptActor());
	//Orders
	TTGameInstance.Get()->RecipeDataTable.Get()->GetAllRows("", Orders);
	//Get Expressions
	TTGameInstance.Get()->GmithExpressionDataTable->GetAllRows("", GmithExpressions);
	TTGameInstance.Get()->GoctorExpressionDataTable->GetAllRows("", GoctorExpressions);
	TTGameInstance.Get()->GailorExpressionDataTable->GetAllRows("", GailorExpressions);		
	TTGameInstance.Get()->GavernExpressionDataTable->GetAllRows("", GavernExpressions);		
	TTGameInstance.Get()->GookerExpressionDataTable->GetAllRows("", GookerExpressions);
	//Hide Commands
	Command1.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command1.Get()->SetIsInteractionEnabled(false);
	Command2.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command2.Get()->SetIsInteractionEnabled(false);
	Command3.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command3.Get()->SetIsInteractionEnabled(false);
	Command4.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command4.Get()->SetIsInteractionEnabled(false);
	Command5.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command5.Get()->SetIsInteractionEnabled(false);
	InUpgradeDialog = false;
	EmptyAllText();
	//Gooker or not
	if(NPCName == FGameplayTags::Get().NPC_Gooker)
	{
		TTGameInstance.Get()->LevelDataTable->GetAllRows("", LevelRows);
		GookerDialog();
	}
	else
	{
		//In Gavern Tutorial
		if(NPCName == FGameplayTags::Get().NPC_Gavern && TTGameInstance.Get()->CurrentLevel == 0)
		{
			UpdateSatisfactionBar();
			CheckInitialNPCInformation();
			UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(GavernTutorialDialog);
		}
		else
		{
			UpdateSatisfactionBar();
			CheckInitialNPCInformation();
			//Execute animation depending on ProgressToFill
			NPCSatisfactionBarBlend.SetBlendTime(BlendTime);
			NPCSatisfactionBarBlend.SetValueRange(CurrentProgressValue,UpgradeUI.Get()->GetPercentageToFill(ProgressToFill));
			NPCSatisfactionBarBlend.Reset();
			CurrentProgressValue = UpgradeUI.Get()->GetPercentageToFill(ProgressToFill);
			//Show upgrades
			if(CurrentProgressValue == 1)
			{
				UpgradeUI.Get()->ShowUpgrades(TTGameInstance, true);
				PlayerCompletedAllTiers = UpgradeUI.Get()->PlayerCompletedAllTiers();
			}
			else
			{
				UpgradeUI.Get()->ShowUpgrades(TTGameInstance, false);
				PlayerCompletedAllTiers = UpgradeUI.Get()->PlayerCompletedAllTiers();
			}
			if(FirstTime)
			{
				if(NPCName == FGameplayTags::Get().NPC_Gmith)
				{
					if(TTGameInstance.Get()->CompletedMainOrder)
					{
						UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(GreetingDialogs[TTGameInstance.Get()->CurrentLevel-3]);
					}
					else
					{
						UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(FailedMainCommandDialogs[0]);
					}
				}
				else if(NPCName == FGameplayTags::Get().NPC_Gavern)
				{
					if(TTGameInstance.Get()->CompletedMainOrder)
					{
						UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(GreetingDialogs[TTGameInstance.Get()->CurrentLevel]);
					}
					else
					{
						UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(FailedMainCommandDialogs[0]);
					}
				}
				else
				{
					if(TTGameInstance.Get()->CompletedMainOrder)
					{
						int greetingsDialogsIndex = TTGameInstance.Get()->CurrentLevel-2;
						UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(GreetingDialogs[greetingsDialogsIndex]);
					}
					else
					{
						UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(FailedMainCommandDialogs[0]);
					}
				}
				FirstTime = false;
			}
			else
			{
				//Create Main NPC Menu
				CreateMainNPCMenu();	
			}
		}
	}
	UpgradeUI.Get()->GetFutureSatistactionBar().Get()->SetPercent(0);
}

void UNPCDialog::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UNPCDialog::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	EnableFocusTimer = true;
	FocusTimer = 0.0f;
	Command1.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command1.Get()->ResetCommand();
	Command2.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command2.Get()->ResetCommand();
	Command3.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command3.Get()->ResetCommand();
	Command4.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command4.Get()->ResetCommand();
	Command5.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command5.Get()->ResetCommand();
}

void UNPCDialog::NativeDestruct()
{
	Super::NativeDestruct();

	EmptyAllText();
	EmptyNPCButtons();
	GamepadCurrentButton.Get()->OnFocusLost().RemoveAll(this);
	PlayerArticyFlowPlayer.Get()->OnPlayerPaused.RemoveAll(this);
	PlayerArticyFlowPlayer.Get()->OnBranchesUpdated.RemoveAll(this);
	ProgressToFill = 0;
	CurrentProgressValue = 0;
	UpgradeUI.Get()->GetSatistactionBar().Get()->SetPercent(0);
	UpgradeUI.Get()->GetFutureSatistactionBar().Get()->SetPercent(0);
	NPCSatisfactionBarBlend.Reset();
	FinishedSatisfactionBarAnimation = true;
	TryUpgrade = false;
	NumberOfCommandsToGive = 0;
	DialogsIDAvailable.Empty();
	BackHandle.Unregister();
	GmithExpressions.Empty();
	GoctorExpressions.Empty();
	GailorExpressions.Empty();		
	GavernExpressions.Empty();		
	GookerExpressions.Empty();
	Orders.Empty();
	LevelRows.Empty();
}

UWidget* UNPCDialog::NativeGetDesiredFocusTarget() const
{
	Super::NativeGetDesiredFocusTarget();
	
	if(!GamepadCurrentButton->IsVisible())
	{
		if(DialogButton1.Get()->IsVisible())
		{
			return DialogButton1;
		}
		if(QuestionsButton1.Get()->IsVisible())
		{
			return QuestionsButton1;
		}
		if(ExitButton.Get()->IsVisible())
		{
			return ExitButton;
		}
		if(BackButton.Get()->IsVisible())
		{
			return BackButton;
		}
		if(QuestionButton.Get()->IsVisible())
		{
			return QuestionButton;
		}
		if(UpgradeButton.Get()->IsVisible())
		{
			return UpgradeButton;
		}
	}
	return GamepadCurrentButton;
}

void UNPCDialog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!EnableFocusTimer)
	{
		FocusTimer += InDeltaTime;
		if(FocusTimer >= 0.5f)
		{
			GamepadCurrentButton.Get()->SetFocus();
			FocusTimer = 0;
			EnableFocusTimer = true;
		}
	}
	
	//Upgrade Satisfaction bar
	if(!FinishedSatisfactionBarAnimation)
	{
		NPCSatisfactionBarBlend.Update(InDeltaTime);
		UpgradeUI.Get()->GetSatistactionBar().Get()->SetPercent(NPCSatisfactionBarBlend.GetBlendedValue());
		//When bar doesn't move, ended it
		if(NPCSatisfactionBarBlend.IsComplete())
		{
			UpdateUpgradeButton();
			FinishedSatisfactionBarAnimation = true;
			UpdateNPCSatisfactionValues(false);
			CurrentProgressValue = UpgradeUI.Get()->GetPercentageToFill(ProgressToFill);
			if(ProgressToFill == MaxSatisfactionBarValue)
			{
				if(NPCName == FGameplayTags::Get().NPC_Gavern)
				{
					ProgressToFill = 0;
					CurrentProgressValue = 0;
					UpgradeUI.Get()->GetSatistactionBar().Get()->SetPercent(0);
					UpdateNPCSatisfactionValues(false);
					//Show Wheel
					BP_ShowWheel();
					//Achivement of Spinning the Wheel for the first time
					TTGameInstance.Get()->SpinThatWheel();
				}
				else
				{
					if(TryUpgrade && UpgradeUI.Get()->ExecuteUpgrade(TTGameInstance))
					{
						PlayerCompletedAllTiers = true;
						ProgressToFill = 0;
						CurrentProgressValue = 0;
						UpgradeUI.Get()->GetSatistactionBar().Get()->SetPercent(0);
						UpdateNPCSatisfactionValues(true);
						//Depending on NPC execute one upgrade dialog or another
						ShowUpgradeText();
					}
					else
					{
						NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Visible);
						DialogImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						GamepadCurrentButton = UpgradeButton;
						RequestRefreshFocus();
						PlaySound(ErrorSound);	
					}
				}
			}
			else
			{
				NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Visible);
				DialogImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				if(UpgradeButton->IsVisible())
				{
					GamepadCurrentButton = UpgradeButton;
					RequestRefreshFocus();
				}
			}
			NPCSatisfactionBarBlend.SetValueRange(ProgressToFill,ProgressToFill);
		}
	}
}

FReply UNPCDialog::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(ContinueHandle.IsValid())
	{
		if(BackHandle.IsValid())
		{
			//If last part of tutorial
			ReturnToTavern();
		}
		else
		{
			//Else continue tutorial
			ContinueTutorial();	
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UNPCDialog::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if(Animation == HandInCommands)
	{
		//When Flying Commands Appear Finish Animation
		//Hide Commands
		Command1.Get()->SetVisibility(ESlateVisibility::Hidden);
		Command1.Get()->SetPlateID(-1);
		Command2.Get()->SetVisibility(ESlateVisibility::Hidden);
		Command2.Get()->SetPlateID(-1);
		Command3.Get()->SetVisibility(ESlateVisibility::Hidden);
		Command3.Get()->SetPlateID(-1);
		Command4.Get()->SetVisibility(ESlateVisibility::Hidden);
		Command4.Get()->SetPlateID(-1);
		Command5.Get()->SetVisibility(ESlateVisibility::Hidden);
		Command5.Get()->SetPlateID(-1);
		
		if(GivingExtraPlateInTutorial)
		{
			BP_EndShowingPlateToGavernInTutorial();
			NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Visible);
			DialogImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			NPCSatisfactionBarBlend.SetBlendTime(BlendTime);
			NPCSatisfactionBarBlend.Reset();
			
			//Simulate that you have Gavern an extra Plate with 1 Star
			ProgressToFill = 1;
			NPCSatisfactionBarBlend.SetValueRange(CurrentProgressValue,UpgradeUI.Get()->GetPercentageToFill(ProgressToFill));
			UpdateNPCSatisfactionValues(false);
			FinishedSatisfactionBarAnimation = false;
			BP_IncreasingSatisfactionBar(ProgressToFill, MaxSatisfactionBarValue);
			//Back Handle
			BackHandle.Unregister();
			ContinueHandle.Unregister();
			BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNPCDialog::ReturnToTavern)));
			ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNPCDialog::ReturnToTavern)));
		}
		else
		{
			if(!PlayerCompletedAllTiers)
			{
				NPCSatisfactionBarBlend.SetBlendTime(BlendTime);
				NPCSatisfactionBarBlend.SetValueRange(CurrentProgressValue,UpgradeUI.Get()->GetPercentageToFill(ProgressToFill));
				NPCSatisfactionBarBlend.Reset();
				BP_IncreasingSatisfactionBar(ProgressToFill, MaxSatisfactionBarValue);
				FinishedSatisfactionBarAnimation = false;
			}
		}
	}
}

void UNPCDialog::GookerDialog(bool AnimationNeeded)
{
	//Death dialog
	if(TTGameInstance.Get()->PlayerDied != NODEATH)
	{
		//Show Gooker Animation
		TavernLevel.Get()->NPCGookerAnimation(true); 
		switch (TTGameInstance.Get()->PlayerDied)
		{
		case WITHNOMAINORDER:
			UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(DeathWithNoMainOrder);
			break;
		case WITHALLMAINPLATES:
			UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(DeathWithAllMainPlates[TTGameInstance.Get()->CurrentLevel-2]);
			break;
		}
	}
	//Main story dialog
	else if(TavernLevel.Get()->GetSecondaryGookerDialog().IsNull())
	{
		if(AnimationNeeded)
		{
			//Show Gooker Animation
			TavernLevel.Get()->NPCGookerAnimation(true);
		}
		//Progression dialogs
		for (FLevelRow* levelRow : LevelRows)
		{
			if(TTGameInstance.Get()->CurrentLevel == levelRow->Level)
			{
				UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(levelRow->GookerDialogForMainOrder);
				break;
			}
		}
	}
	else
	{
		//Secondary Gooker Dialogs, like in tutorial
		UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(TavernLevel.Get()->GetSecondaryGookerDialog());
	}
}

void UNPCDialog::UpdateUpgradeButton()
{
	UpgradeButton.Get()->SetNormalColor();
	if(ProgressToFill == MaxSatisfactionBarValue && !PlayerCompletedAllTiers)
	{
		if(!UpgradeUI.Get()->DoesPlayerHaveRelic())
		{
			UpgradeButton.Get()->ChangeNormalColor();
			UpgradeButton.Get()->SetTextOfButton(UpgradeRelicNeededText);	
		}
		else
		{
			UpgradeButton.Get()->SetTextOfButton(UpgradeText);	
		}
	}
	else
	{
		UpgradeButton.Get()->SetTextOfButton(GivePlateText);
	}
}

void UNPCDialog::ContinueDialog(TScriptInterface<IArticyFlowObject> ArticyObject)
{
	TObjectPtr<UArticyNode> articyNode = Cast<UArticyNode>(ArticyObject.GetInterface());
	IArticyObjectWithText* articyObjectText;
	IArticyObjectWithSpeaker* articyObjectSpeaker;
	switch (articyNode->GetType()) {
		case EArticyPausableType::FlowFragment:
			break;
		case EArticyPausableType::Dialogue:
			PlayerArticyFlowPlayer.Get()->Play();
			break;
		case EArticyPausableType::DialogueFragment:
			articyObjectText = Cast<IArticyObjectWithText>(ArticyObject.GetObject());
			articyObjectSpeaker = Cast<IArticyObjectWithSpeaker>(ArticyObject.GetObject());
			ArticySpeakerDisplayName = Cast<IArticyObjectWithDisplayName>(UTutorial_Dialogue_1st_DraftDatabase::Get(GetWorld())->GetObject(articyObjectSpeaker->GetSpeakerId()));
			if(ArticySpeakerDisplayName)
			{
				AddNewText(articyObjectText->GetText(), ArticySpeakerDisplayName->GetDisplayName());
			}
			else
			{
				AddNewText(articyObjectText->GetText(), FText::FromString(""));
			}
			break;
		case EArticyPausableType::Hub:
			break;
		case EArticyPausableType::Jump:
			break;
		case EArticyPausableType::Condition:
			break;
		case EArticyPausableType::Instruction:
			break;
		case EArticyPausableType::Pin:
			break;
	}
}

void UNPCDialog::UpdateBranches(const TArray<FArticyBranch>& AvailableBranches)
{
	BackHandle.Unregister();
	//Global Variables
	ChangeExpression();
	if(UpgradeButton)
	{
		UpgradeButton->OnClicked().RemoveAll(this);
	}
	if(QuestionButton)
	{
		QuestionButton->OnClicked().RemoveAll(this);
	}
	if(ExitButton)
	{
		ExitButton->OnClicked().RemoveAll(this);
	}
	if(BackButton)
	{
		BackButton->OnClicked().RemoveAll(this);
	}
	
	HideButtons();
	for(int i = AvailableBranches.Num()-1; i >= 0; i--)
	{
		FArticyBranch branch = AvailableBranches[i];
		TObjectPtr<UTutorial_Dialogue_1st_DraftDialogueFragment> articyTutorialDialogueFragment = Cast<UTutorial_Dialogue_1st_DraftDialogueFragment>(branch.GetTarget().GetInterface());
		if(articyTutorialDialogueFragment)
		{
			//If Ghunter speaking and has something to say as a button, pass to the next dialog with an overlay
			if(ArticySpeakerDisplayName->GetDisplayName().EqualTo(FText::FromString("GHUNTER")))
			{
				FText branchText = articyTutorialDialogueFragment.Get()->MenuText;
				if(branchText.IsEmpty())
				{
					GhunterBranchToAutoPlay = branch;
					GetWorld()->GetTimerManager().SetTimer(
					TimerHandle, // handle to cancel timer at a later time
					this, // the owning object
					&UNPCDialog::GhunterFinishedTalking, // function to call on elapsed
					GhunterSpeakingTime, // float delay until elapsed
					false);
					
					return;
				}
				GhunterTalking = true;
			}
			//If not Ghunter speaking
			TObjectPtr<UDialogButton> newDialogButton = GetFirstDialogButtonInvisible();
			newDialogButton.Get()->SetVisibility(ESlateVisibility::Visible);
			newDialogButton.Get()->AssignText(branch, PlayerArticyFlowPlayer, articyTutorialDialogueFragment);
			//This is done so that new children appear from bottom to up
			newDialogButton.Get()->SetRenderTransformAngle(180);
			//Update Gamepad button
			GamepadCurrentButton = newDialogButton;
			RequestRefreshFocus();
			GamepadCurrentButton.Get()->SetFocus();
			FocusTimer = 0.0f;
			EnableFocusTimer = false;
		}
	}
	if(DialogButton1.Get()->IsVisible() && !DialogButton2.Get()->IsVisible() && !BackButton.Get()->IsVisible())
	{
		DialogButton1.Get()->NativeOnHovered();
		DialogButton1.Get()->SetFocus();
	}

	//If Wrapbox has no children, it means the end of dialog, so go back to main npc menu or questions menu
	if(AreAllButtonsInvisible())
	{
		if(NPCName != FGameplayTags::Get().NPC_Gooker)
		{
			if(NPCName == FGameplayTags::Get().NPC_Gavern && TTGameInstance.Get()->CurrentLevel == 0)
			{
				GavernTutorial();
			}
			else if(InAQuestion)
			{
				//Add Retreat button
				BackButton.Get()->SetVisibility(ESlateVisibility::Visible);
				BackButton.Get()->SetTextOfButton(RetreatToQuestionsText);
				BackButton.Get()->SetRenderTransformAngle(180);
				BackButton.Get()->OnClicked().AddUObject(this, &UNPCDialog::HeadBack);
				//Back Handle
				BackHandle.Unregister();
				BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNPCDialog::HeadBack)));
				//Update Gamepad button
				GamepadCurrentButton = BackButton;
				RequestRefreshFocus();
			}
			else
			{
				if(InUpgradeDialog)
				{
					NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Hidden);
					DialogImage.Get()->SetVisibility(ESlateVisibility::Hidden);
					BP_ShowUpgrade(TTGameInstance.Get()->GavernUpgrade);
					InUpgradeDialog = false;
				}
				CreateMainNPCMenu();	
			}
		}
		else
		{
			//If Player Died
			if(TTGameInstance.Get()->PlayerDied != NODEATH)
			{
				if(TTGameInstance.Get()->PlayerDied == WITHNOMAINORDER)
				{
					TTGameInstance.Get()->PlayerDied = NODEATH;
					TavernLevel.Get()->NPCGookerAnimation(false);
					//Continue with normal tavern interaction
					DeactivateWidget();
					TavernLevel.Get()->NormalTavernAfterGooker();
				}
				else
				{
					TTGameInstance.Get()->PlayerDied = NODEATH;
					GookerDialog(false);	
				}
			}
			//If Gooker talked to you, you earned a new level for completing the main order
			//Add new NPC, Weapon and add new Main Order
			//Also: you start in Tavern, so no progression update, and when you finish progression in alpha(level 4)
			//no pogression, and twice the player is in tutorial
			else if(TTGameInstance.Get()->CurrentLevel == 6)
			{
				TavernLevel.Get()->GoToFinalLevel();
				DeactivateWidget();
			}
			else
			{
				UpdateProgression();
				if(TTGameInstance.Get()->CurrentLevel == -1)
				{
					TavernLevel.Get()->PlayFadeAnimation();
					//Continue with normal tavern interaction
					DeactivateWidget();
				}
				else
				{
					if(TavernLevel.Get()->GetSecondaryGookerDialog().IsNull())
					{
						TavernLevel.Get()->NPCGookerAnimation(false);
					}
					else
					{
						TavernLevel.Get()->NPCGookerTutorialAnimation(false);
					}
					//Continue with normal tavern interaction
					DeactivateWidget();
					TavernLevel.Get()->NormalTavernAfterGooker();
				}
			}
		}
	}
}

void UNPCDialog::ShowUpgrades()
{
	//Remove Notification Icon on this NPC
	RemoveNotificationFromNPC();
	//Play Commands Animation and After finish, execute logic
	if(NPCName != FGameplayTags::Get().NPC_Gavern)
	{
		if(!GiveSecondaryPlates())
		{
			if(ProgressToFill == MaxSatisfactionBarValue && UpgradeUI.Get()->DoesPlayerHaveRelic())
			{
				FinishedSatisfactionBarAnimation = false;
			}
		}
		TryUpgrade = true;
	}
	else
	{
		//If NPC is Gavern, give extra plates
		for (int extraPlateID : TTGameInstance.Get()->ExtraPlatesInventory)
		{
			bool onGavernDay = false;
			//If obtained during Gavern Day
			if(TTGameInstance.Get()->ExtraPlatesDuringGavernDay.Contains(extraPlateID))
			{
				onGavernDay = true;
			}
			ProgressToFill += GetSatisfaction(extraPlateID, onGavernDay);
			ProgressToFill = FMath::Clamp(ProgressToFill, 0, MaxSatisfactionBarValue);
			++NumberOfCommandsToGive;
			//Set Commands
			if(!Command1.Get()->IsVisible())
			{
				Command1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command1.Get()->SetCommand(extraPlateID);
				Command1.Get()->SetExtraStar(onGavernDay);
				Command1.Get()->SetNPCImage(NPCImage, NPCName);
				Command1.Get()->ExecuteAppearAnimation(true);
			}
			else if(!Command2.Get()->IsVisible())
			{
				Command2.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command2.Get()->SetCommand(extraPlateID);
				Command2.Get()->SetExtraStar(onGavernDay);
				Command2.Get()->SetNPCImage(NPCImage, NPCName);
				Command2.Get()->ExecuteAppearAnimation(true);
			}
			else if(!Command3.Get()->IsVisible())
			{
				Command3.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command3.Get()->SetCommand(extraPlateID);
				Command3.Get()->SetExtraStar(onGavernDay);
				Command3.Get()->SetNPCImage(NPCImage, NPCName);
				Command3.Get()->ExecuteAppearAnimation(true);
			}
			else if(!Command4.Get()->IsVisible())
			{
				Command4.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command4.Get()->SetCommand(extraPlateID);
				Command4.Get()->SetExtraStar(onGavernDay);
				Command4.Get()->SetNPCImage(NPCImage, NPCName);
				Command4.Get()->ExecuteAppearAnimation(true);
			}
			else if(!Command5.Get()->IsVisible())
			{
				Command5.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command5.Get()->SetCommand(extraPlateID);
				Command5.Get()->SetExtraStar(onGavernDay);
				Command5.Get()->SetNPCImage(NPCImage, NPCName);
				Command5.Get()->ExecuteAppearAnimation(true);
			}
		}
		//If no extra plates to give
		if(ProgressToFill != 0 && CurrentProgressValue != UpgradeUI.Get()->GetPercentageToFill(ProgressToFill))
		{
			BackHandle.Unregister();
			PlayAnimation(HandInCommands);
			NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Hidden);
			DialogImage.Get()->SetVisibility(ESlateVisibility::Hidden);
			TTGameInstance.Get()->ExtraPlatesInventory.Empty();
			TTGameInstance.Get()->ExtraPlatesDuringGavernDay.Empty();
			//If Gavern bar filled, apply temporary upgrade
			if(ProgressToFill == MaxSatisfactionBarValue)
			{
				//Gavern Upgrades
				// if(TTGameInstance.Get()->CurrentLevel == 1)
				// {
				// 	TArray<EGavernUpgrades> posibleUpgrades = {SHIELD};
				// 	int32 randomID = FMath::RandRange(0, posibleUpgrades.Num()-1);
				// 	TTGameInstance.Get()->GavernUpgrade = posibleUpgrades[randomID];
				// }
				// else
				// {
				// 	TArray<EGavernUpgrades> posibleUpgrades = {SHIELD, EXTRAREPUTATION, PUFFBALLRESISTANCE};
				// 	int32 randomID = FMath::RandRange(0, posibleUpgrades.Num()-1);
				// 	TTGameInstance.Get()->GavernUpgrade = posibleUpgrades[randomID];
				// }
				TavernLevel.Get()->GavernSprite.Get()->ChangeToUniqueSprite();
			}
		}
		else
		{
			PlaySound(ErrorSound);
		}
	}
}

void UNPCDialog::ShowQuestions()
{
	BackHandle.Unregister();
	if(BackButton)
	{
		BackButton->OnClicked().RemoveAll(this);
	}
	HideButtons();
	//Add Retreat button
	BackButton.Get()->SetVisibility(ESlateVisibility::Visible);
	BackButton.Get()->SetTextOfButton(BackToMainMenuText);
	BackButton.Get()->SetRenderTransformAngle(180);
	BackButton.Get()->OnClicked().AddUObject(this, &UNPCDialog::HeadBack);
	//Add Question Buttons
	for(int i = DialogsIDAvailable.Num()-1; i >= 0; i--)
	{
		FArticyId dialogID = DialogsIDAvailable[i];
		TObjectPtr<UNPCQuestionButton> newQuestionButton = GetFirstNPCQuestionInvisibleButton();
		newQuestionButton.Get()->SetVisibility(ESlateVisibility::Visible);
		newQuestionButton.Get()->SetFlowPlayer(PlayerArticyFlowPlayer);
		newQuestionButton.Get()->SetDialogID(dialogID);
		newQuestionButton.Get()->SetNPCDialogParent(this);
		newQuestionButton.Get()->SetTextOfButton(Cast<IArticyObjectWithText>(dialogID.GetObject(ArticyGlobalVariables))->GetText());
		newQuestionButton.Get()->SetRenderTransformAngle(180);
		//Update Gamepad button
		GamepadCurrentButton = newQuestionButton;
		RequestRefreshFocus();
	}
}

void UNPCDialog::ReturnToTavern()
{
	if(NPCName == FGameplayTags::Get().NPC_Gavern)
	{
		TavernLevel.Get()->GavernSprite.Get()->ShowOutline(false);
		TavernLevel.Get()->NPCGavernAnimation(false);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gmith)
	{
		TavernLevel.Get()->GmithSprite.Get()->ShowOutline(false);
		TavernLevel.Get()->NPCGmithAnimation(false);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Goctor)
	{
		TavernLevel.Get()->GoctorSprite.Get()->ShowOutline(false);
		TavernLevel.Get()->NPCGoctorAnimation(false);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gailor)
	{
		TavernLevel.Get()->GailorSprite.Get()->ShowOutline(false);
		TavernLevel.Get()->NPCGailorAnimation(false);
	}
	DeactivateWidget();
}

void UNPCDialog::HeadBack()
{
	if(InAQuestion)
	{
		ShowQuestions();
		InAQuestion = false;
	}
	else
	{
		HideButtons();
		CreateMainNPCMenu();
	}
}

void UNPCDialog::CreateMainNPCMenu()
{
	//Change the transform angle of button so that the new children appear from bottom to up
	ExitButton.Get()->SetVisibility(ESlateVisibility::Visible);
	ExitButton.Get()->SetTextOfButton(ExitText);
	ExitButton.Get()->SetRenderTransformAngle(180);
	ExitButton.Get()->OnClicked().AddUObject(this, &UNPCDialog::ReturnToTavern);
	
	QuestionButton.Get()->SetVisibility(ESlateVisibility::Visible);
	QuestionButton.Get()->SetTextOfButton(QuestionsText);
	QuestionButton.Get()->SetRenderTransformAngle(180);
	QuestionButton.Get()->OnClicked().AddUObject(this, &UNPCDialog::ShowQuestions);
	
	UpgradeButton.Get()->SetVisibility(ESlateVisibility::Visible);
	UpdateUpgradeButton();
	UpgradeButton.Get()->SetRenderTransformAngle(180);	
	UpgradeButton.Get()->OnClicked().AddUObject(this, &UNPCDialog::ShowUpgrades);

	//Back Handle
	BackHandle.Unregister();
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNPCDialog::ReturnToTavern)));
	//Update Gamepad button
	GamepadCurrentButton = UpgradeButton;
	RequestRefreshFocus();
}

void UNPCDialog::CheckInitialNPCInformation()
{
	for(TTuple<FGameplayTag, FNPCInformationStruct> npcInformation : TTGameInstance.Get()->NPCInformationMap)
	{
		if(npcInformation.Key == NPCName)
		{
			//Get Dialogs
			for (FArticyId dialogID : npcInformation.Value.NPCDialogs)
			{
				DialogsIDAvailable.Add(dialogID);
			}
			//Set Initial Information
			MaxSatisfactionBarValue = SatisfactionBarMaxValues[npcInformation.Value.CurrentNPCSatisfactionLevel];
			ProgressToFill = npcInformation.Value.CurrentNPCSatisfactionValue;
			if(ProgressToFill == 0)
			{
				FinishedSatisfactionBarAnimation = true;
				UpgradeUI.Get()->GetSatistactionBar().Get()->SetPercent(0);
			}
			else
			{
				UpgradeUI.Get()->GetSatistactionBar().Get()->SetPercent(UpgradeUI.Get()->GetPercentageToFill(ProgressToFill));
			}
			break;
		}
	}
}

bool UNPCDialog::GiveSecondaryPlates()
{
	bool aSecondaryPlateToOfferFound = false;
	//Copy used because in each iteration, a value is removed from the original array
	TArray<int> copyOfNPCOrders = TTGameInstance.Get()->NPCInformationMap[NPCName].NPCOrders;
	for(int npcOrder : copyOfNPCOrders)
	{
		if(TTGameInstance.Get()->FinishedOrders.Contains(FSecondaryCommand(NPCName, npcOrder)))
		{
			FSecondaryCommand command = FSecondaryCommand(NPCName, npcOrder);
			//Remove PositionOfCommandsInQuestBoard
			RemoveCommandFromQuestBoard(command);
			bool onGavernDay = false;
			TTGameInstance.Get()->FinishedOrders.Remove(command);
			//Check if Secondary Command was obtained during Gavern Day
			if(TTGameInstance.Get()->SecondaryOrdersExtra.Contains(command))
			{
				TTGameInstance.Get()->SecondaryOrdersExtra.Remove(command);
				onGavernDay = true;
			}
			ProgressToFill += GetSatisfaction(command.PlateID, onGavernDay);
			ProgressToFill = FMath::Clamp(ProgressToFill, 0, MaxSatisfactionBarValue);
			TTGameInstance.Get()->NPCInformationMap[NPCName].CurrentNPCSatisfactionValue = ProgressToFill;
			++NumberOfCommandsToGive;
			//Set Commands
			if(!Command1.Get()->IsVisible())
			{
				Command1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command1.Get()->SetCommand(command.PlateID);
				Command1.Get()->SetNPCImage(NPCImage, NPCName);
				Command1.Get()->ExecuteAppearAnimation(true);
			}
			else if(!Command2.Get()->IsVisible())
			{
				Command2.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command2.Get()->SetCommand(command.PlateID);
				Command2.Get()->SetNPCImage(NPCImage, NPCName);
				Command2.Get()->ExecuteAppearAnimation(true);
			}
			else if(!Command3.Get()->IsVisible())
			{
				Command3.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Command3.Get()->SetCommand(command.PlateID);
				Command3.Get()->SetNPCImage(NPCImage, NPCName);
				Command3.Get()->ExecuteAppearAnimation(true);
			}
			NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Hidden);
			DialogImage.Get()->SetVisibility(ESlateVisibility::Hidden);
			BackHandle.Unregister();
			PlayAnimation(HandInCommands);
			//Achievement of handing in a secondary command
			TTGameInstance.Get()->HandInASecondaryOrder();
			aSecondaryPlateToOfferFound = true;
			TTGameInstance.Get()->NPCInformationMap[NPCName].NPCOrders.Remove(command.PlateID);
		}
	}
	return aSecondaryPlateToOfferFound;
}

void UNPCDialog::RemoveCommandFromQuestBoard(FSecondaryCommand FinishedCommand)
{
	if(FinishedCommand.NPCTag == FGameplayTags::Get().NPC_Goctor)
	{
		if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[0])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[0] = -1;
		}
		else if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[1])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[1] = -1;
		}
		else if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[2])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[2] = -1;
		}
	}
	else if(FinishedCommand.NPCTag == FGameplayTags::Get().NPC_Gailor)
	{
		if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[3])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[3] = -1;
		}
		else if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[4])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[4] = -1;
		}
		else if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[5])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[5] = -1;
		}
	}
	else if(FinishedCommand.NPCTag == FGameplayTags::Get().NPC_Gmith)
	{
		if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[6])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[6] = -1;
		}
		else if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[7])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[7] = -1;
		}
		else if(FinishedCommand.PlateID == TTGameInstance.Get()->PositionOfCommandsInQuestBoard[8])
		{
			TTGameInstance.Get()->PositionOfCommandsInQuestBoard[8] = -1;
		}
	}
}

int UNPCDialog::GetSatisfaction(int PlateID, bool WasPickedInGavernDay, bool WasPickedPremium)
{
	for(FRecipes_Struct* order : Orders)
	{
		if(order->ID == PlateID)
		{
			int extraForGavernDay = 0;
			int extraForPremium = 0;
			if(WasPickedInGavernDay)
			{
				extraForGavernDay = TTGameInstance.Get()->GavernExtraReputationValue;
			}
			if(WasPickedPremium)
			{
				extraForPremium = TTGameInstance.Get()->PremiumExtraValue;
			}
			return order->Stars + extraForGavernDay + extraForPremium;
		}
	}
	return 0;
}

void UNPCDialog::UpdateProgression()
{
	for (FLevelRow* levelRow : LevelRows)
	{
		if(levelRow->Level == TTGameInstance.Get()->CurrentLevel)
		{
			for(TTuple<FGameplayTag, TEnumAsByte<FWeaponsLevel>> weapon : levelRow->WeaponsUnlockedInLevel)
			{
				TTGameInstance.Get()->UnlockedWeapons[weapon.Key] = true;
				if(weapon.Key != FGameplayTags::Get().Weapon_CanObtainBoil && weapon.Key != FGameplayTags::Get().Weapon_CanObtainFry)
				{
					TTGameInstance.Get()->WeaponsLevel[weapon.Key] = weapon.Value;
				}
				//Achievement of tier 2 and 3 of melee weapon
				TTGameInstance.Get()->MeleeLevelUp(weapon.Value);
			}
			break;
		}
	}
	if(TTGameInstance.Get()->CurrentLevel != 0)
	{
		TTGameInstance.Get()->MainOrders.Empty();
		TTGameInstance.Get()->PremiumMainOrders.Empty();
		for(FRecipes_Struct* recipe : Orders)
		{
			if(recipe->Principal_Mission == TTGameInstance.Get()->CurrentLevel)
			{
				TTGameInstance.Get()->MainOrders.Add(recipe->ID, false);
				if(recipe->Vip)
				{
					TTGameInstance.Get()->PremiumMainOrders.Add(recipe->ID);
				}
			}
		}
	}
}

void UNPCDialog::UpdateNPCSatisfactionValues(bool LevelIncrease)
{
	for(TTuple<FGameplayTag, FNPCInformationStruct> npcInformation : TTGameInstance.Get()->NPCInformationMap)
	{
		if(npcInformation.Key == NPCName)
		{
			if(LevelIncrease)
			{
				npcInformation.Value.CurrentNPCSatisfactionLevel = npcInformation.Value.CurrentNPCSatisfactionLevel + 1;
				if(SatisfactionBarMaxValues.Num() == npcInformation.Value.CurrentNPCSatisfactionLevel)
				{
					npcInformation.Value.CurrentNPCSatisfactionLevel = npcInformation.Value.CurrentNPCSatisfactionLevel - 1;
				}
				MaxSatisfactionBarValue = SatisfactionBarMaxValues[npcInformation.Value.CurrentNPCSatisfactionLevel];
			}
			npcInformation.Value.CurrentNPCSatisfactionValue = ProgressToFill;
			TTGameInstance.Get()->NPCInformationMap[npcInformation.Key] = npcInformation.Value;
			break;
		}
	}
	UpdateSatisfactionBar();
}

void UNPCDialog::SetButtons()
{
	DialogButton1.Get()->SetNPCDialog(this);
	NPCButtons.Add(DialogButton1);
	DialogButton2.Get()->SetNPCDialog(this);
	NPCButtons.Add(DialogButton2);
	DialogButton3.Get()->SetNPCDialog(this);
	NPCButtons.Add(DialogButton3);
	DialogButton4.Get()->SetNPCDialog(this);
	NPCButtons.Add(DialogButton4);
	DialogButton5.Get()->SetNPCDialog(this);
	NPCButtons.Add(DialogButton5);
	DialogButton6.Get()->SetNPCDialog(this);
	NPCButtons.Add(DialogButton6);
	QuestionsButton1.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton1);
	QuestionsButton2.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton2);
	QuestionsButton3.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton3);
	QuestionsButton4.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton4);
	QuestionsButton5.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton5);
	QuestionsButton6.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton6);
	QuestionsButton7.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton7);
	QuestionsButton8.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton8);
	QuestionsButton9.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionsButton9);
	UpgradeButton.Get()->SetNPCDialog(this);
	NPCButtons.Add(UpgradeButton);
	QuestionButton.Get()->SetNPCDialog(this);
	NPCButtons.Add(QuestionButton);
	ExitButton.Get()->SetNPCDialog(this);
	NPCButtons.Add(ExitButton);
	BackButton.Get()->SetNPCDialog(this);
	NPCButtons.Add(BackButton);
}

void UNPCDialog::HideButtons()
{
	for(TObjectPtr<UButtonBase> button : NPCButtons)
	{
		button.Get()->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UNPCDialog::EmptyNPCButtons()
{
	DialogButton1->OnClicked().RemoveAll(this);
	DialogButton2->OnClicked().RemoveAll(this);
	DialogButton3->OnClicked().RemoveAll(this);
	DialogButton4->OnClicked().RemoveAll(this);
	DialogButton5->OnClicked().RemoveAll(this);
	QuestionsButton1->OnClicked().RemoveAll(this);
	QuestionsButton2->OnClicked().RemoveAll(this);
	QuestionsButton3->OnClicked().RemoveAll(this);
	QuestionsButton4->OnClicked().RemoveAll(this);
	QuestionsButton5->OnClicked().RemoveAll(this);
	QuestionsButton6->OnClicked().RemoveAll(this);
	QuestionsButton7->OnClicked().RemoveAll(this);
	QuestionsButton8->OnClicked().RemoveAll(this);
	QuestionsButton9->OnClicked().RemoveAll(this);
	UpgradeButton->OnClicked().RemoveAll(this);
	QuestionButton->OnClicked().RemoveAll(this);
	ExitButton->OnClicked().RemoveAll(this);
	BackButton->OnClicked().RemoveAll(this);
	NPCButtons.Empty();
}

bool UNPCDialog::AreAllButtonsInvisible()
{
	for(TObjectPtr<UButtonBase> button : NPCButtons)
	{
		if(button.Get()->IsVisible())
		{
			return false;
		}
	}
	return true;
}

TObjectPtr<UButtonBase> UNPCDialog::GetFirstInvisibleButton()
{
	for(TObjectPtr<UButtonBase> button : NPCButtons)
	{
		if(!button.Get()->IsVisible())
		{
			return button;
		}
	}
	return nullptr;
}

TObjectPtr<UNPCQuestionButton> UNPCDialog::GetFirstNPCQuestionInvisibleButton()
{
	for(TObjectPtr<UButtonBase> button : NPCButtons)
	{
		TObjectPtr<UNPCQuestionButton> npcQuestionButton = Cast<UNPCQuestionButton>(button.Get());
		if(npcQuestionButton && !npcQuestionButton.Get()->IsVisible())
		{
			return npcQuestionButton;
		}
	}
	return nullptr;
}

TObjectPtr<UDialogButton> UNPCDialog::GetFirstDialogButtonInvisible()
{
	for(TObjectPtr<UButtonBase> button : NPCButtons)
	{
		TObjectPtr<UDialogButton> dialogButton = Cast<UDialogButton>(button.Get());
		if(dialogButton && !dialogButton.Get()->IsVisible())
		{
			return dialogButton;
		}
	}
	return nullptr;
}

void UNPCDialog::ShowGavernUpgrade(EGavernUpgrades GavernUpgrade)
{
	switch (GavernUpgrade)
	{
	case SHIELD:
		UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(ShieldUpgradeDialog);
		break;
	case EXTRAREPUTATION:
		UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(ExtraReputationUpgradeDialog);
		break;
	case PUFFBALLRESISTANCE:
		UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(PuffballUpgradeDialog);
		break;
	default: ;
	}
}

void UNPCDialog::UpdateSatisfactionBar()
{
	//Show Bar
	if(NPCName == FGameplayTags::Get().NPC_Goctor)
	{
		SatisfactionBarMaxValues = TTGameInstance.Get()->GoctorSatisfactionBarMaxValues;
		UpgradeUI.Get()->UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gailor)
	{
		SatisfactionBarMaxValues = TTGameInstance.Get()->GailorSatisfactionBarMaxValues;
		UpgradeUI.Get()->UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gmith)
	{
		SatisfactionBarMaxValues = TTGameInstance.Get()->GmithSatisfactionBarMaxValues;
		UpgradeUI.Get()->UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gavern)
	{
		SatisfactionBarMaxValues = TTGameInstance.Get()->GavernSatisfactionBarMaxValues;
		UpgradeUI.Get()->UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gavern].CurrentNPCSatisfactionLevel);
	}
}

void UNPCDialog::ChangeExpression()
{
	if(ArticySpeakerDisplayName != nullptr && !ArticySpeakerDisplayName->GetDisplayName().EqualTo(FText::FromString("GHUNTER")))
	{
		//GMITH
		if(NPCName == FGameplayTags::Get().NPC_Gmith)
		{
			bool foundExpressionNotNormal = false;
			for (FNPCExpressions* gmithExpression : GmithExpressions)
			{
				bool succeded = false;
				FArticyGvName gmithName;
				gmithName.FullName = gmithExpression->VariableName;
				bool hasExpression = ArticyGlobalVariables.Get()->GetBoolVariable(gmithName, succeded);
				if(hasExpression)
				{
					TavernLevel.Get()->GmithSprite.Get()->ChangeToPassedExpression(gmithExpression->HeadExpression);
					PlaySound(gmithExpression->ExpressionSound);
					foundExpressionNotNormal = true;
					break;
				}
			}
			if(!foundExpressionNotNormal)
			{
				TavernLevel.Get()->GmithSprite.Get()->ChangeToNormalSprite();
				PlaySound(NormalNPCSounds[FMath::RandRange(0,NormalNPCSounds.Num()-1)]);
			}
		}
		//GOCTOR
		else if(NPCName == FGameplayTags::Get().NPC_Goctor)
		{
			bool foundExpressionNotNormal = false;
			for (FNPCExpressions* goctorExpression : GoctorExpressions)
			{
				bool succeded = false;
				FArticyGvName goctorName;
				goctorName.FullName = goctorExpression->VariableName;
				bool hasExpression = ArticyGlobalVariables.Get()->GetBoolVariable(goctorName, succeded);
				if(hasExpression)
				{
					TavernLevel.Get()->GoctorSprite.Get()->ChangeToPassedExpression(goctorExpression->HeadExpression);
					PlaySound(goctorExpression->ExpressionSound);
					foundExpressionNotNormal = true;
					break;
				}
			}
			if(!foundExpressionNotNormal)
			{
				TavernLevel.Get()->GoctorSprite.Get()->ChangeToNormalSprite();
				PlaySound(NormalNPCSounds[FMath::RandRange(0,NormalNPCSounds.Num()-1)]);
			}
		}
		//GAILOR
		else if(NPCName == FGameplayTags::Get().NPC_Gailor)
		{
			bool foundExpressionNotNormal = false;
			for (FNPCExpressions* gailorExpression : GailorExpressions)
			{
				bool succeded = false;
				FArticyGvName gailorName;
				gailorName.FullName = gailorExpression->VariableName;
				bool hasExpression = ArticyGlobalVariables.Get()->GetBoolVariable(gailorName, succeded);
				if(hasExpression)
				{
					TavernLevel.Get()->GailorSprite.Get()->ChangeToPassedExpression(gailorExpression->HeadExpression);
					PlaySound(gailorExpression->ExpressionSound);
					foundExpressionNotNormal = true;
					break;
				}
			}
			if(!foundExpressionNotNormal)
			{
				TavernLevel.Get()->GailorSprite.Get()->ChangeToNormalSprite();
				PlaySound(NormalNPCSounds[FMath::RandRange(0,NormalNPCSounds.Num()-1)]);
			}
		}
		//GAVERN
		else if(NPCName == FGameplayTags::Get().NPC_Gavern)
		{
			bool foundExpressionNotNormal = false;
			for (FNPCExpressions* gavernExpression : GavernExpressions)
			{
				bool succeded = false;
				FArticyGvName gavernName;
				gavernName.FullName = gavernExpression->VariableName;
				bool hasExpression = ArticyGlobalVariables.Get()->GetBoolVariable(gavernName, succeded);
				if(hasExpression)
				{
					TavernLevel.Get()->GavernSprite.Get()->ChangeToPassedExpression(gavernExpression->HeadExpression);
					PlaySound(gavernExpression->ExpressionSound);
					foundExpressionNotNormal = true;
					break;
				}
			}
			if(!foundExpressionNotNormal)
			{
				TavernLevel.Get()->GavernSprite.Get()->ChangeToNormalSprite();
				PlaySound(NormalNPCSounds[FMath::RandRange(0,NormalNPCSounds.Num()-1)]);
			}
		}
		//GOOKER
		else if(NPCName == FGameplayTags::Get().NPC_Gooker)
		{
			bool foundExpressionNotNormal = false;
			for (FNPCExpressions* gookerExpression : GookerExpressions)
			{
				bool succeded = false;
				FArticyGvName gookerName;
				gookerName.FullName = gookerExpression->VariableName;
				bool hasExpression = ArticyGlobalVariables.Get()->GetBoolVariable(gookerName, succeded);
				if(hasExpression)
				{
					if(gookerName.FullName.IsEqual(FName("GookerExpressions.GookerLesson")))
					{
						TavernLevel.Get()->GookerSprite.Get()->ChangeToUniqueSprite(true);
					}
					else
					{
						TavernLevel.Get()->GookerSprite.Get()->ChangeToPassedExpression(gookerExpression->HeadExpression);	
					}
					PlaySound(gookerExpression->ExpressionSound);
					foundExpressionNotNormal = true;
					break;
				}
			}
			if(!foundExpressionNotNormal)
			{
				TavernLevel.Get()->GookerSprite.Get()->ChangeToNormalSprite();
				PlaySound(NormalNPCSounds[FMath::RandRange(0,NormalNPCSounds.Num()-1)]);
			}
		}
	}
}

void UNPCDialog::AddNewText(FText NewText, FText SpeakerName)
{
	int numberOfVisibleText = 0;
	//Get first empty text block
	for (int i = 0; i < PreviousTextVerticalBox.Get()->GetChildrenCount()-1; i++)
	{
		TObjectPtr<UCommonRichTextBlock> richTextBlock = Cast<UCommonRichTextBlock>(PreviousTextVerticalBox.Get()->GetChildAt(i));
		//Check if not Spacer
		if(richTextBlock)
		{
			if(richTextBlock.Get()->GetText().IsEmpty())
			{
				break;
			}
		}
		++numberOfVisibleText;
	}
	//Pass the text from one text block to the next depending on how many of the have some text
	int i = numberOfVisibleText;
	while(i > 0)
	{
		TObjectPtr<UCommonRichTextBlock> currentText = Cast<UCommonRichTextBlock>(PreviousTextVerticalBox.Get()->GetChildAt(i));
		if(currentText)
		{
			TObjectPtr<UCommonRichTextBlock> nextText = Cast<UCommonRichTextBlock>(PreviousTextVerticalBox.Get()->GetChildAt(i-2));
			currentText.Get()->SetText(nextText.Get()->GetText());
			//Set Style darker
			currentText.Get()->SetTextStyleSet(PreviousNPCTextDataTable);
			i -= 2;
		}
	}
	//Set the new Text
	TObjectPtr<UCommonRichTextBlock> firstText = Cast<UCommonRichTextBlock>(PreviousTextVerticalBox.Get()->GetChildAt(0));
	FString newText;
	if(!SpeakerName.IsEmpty())
	{
		newText.Append("<");
		newText.Append(SpeakerName.ToString());
		newText.Append(">");
		newText.Append(SpeakerName.ToString());
		newText.Append("- </>");
	}
	newText.Append(NewText.ToString());
	firstText.Get()->SetText(FText::FromString(newText));
	//Set Style White or depending on NPC
	firstText.Get()->SetTextStyleSet(NPCTextDataTable);
}

void UNPCDialog::EmptyAllText()
{
	for (int i = 0; i < PreviousTextVerticalBox.Get()->GetChildrenCount()-1; i++)
	{
		TObjectPtr<UCommonRichTextBlock> richTextBlock = Cast<UCommonRichTextBlock>(PreviousTextVerticalBox.Get()->GetChildAt(i));
		if(richTextBlock)
		{
			richTextBlock.Get()->SetText(FText::FromString(""));
		}
	}
}

void UNPCDialog::GavernTutorial()
{
	GavernTutorialAnimation();
}

void UNPCDialog::EnableContinueButton(bool ShouldBeEnabled)
{
	if(ShouldBeEnabled)
	{
		ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNPCDialog::ContinueTutorial)));
	}
	else
	{
		ContinueHandle.Unregister();
	}
}

void UNPCDialog::OfferPlateInTutorial()
{
	ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UNPCDialog::GiveExtraPlateToGavernInTutorial)));
	//For Tutorial Reasons, give extra plate to player and make him ofter it to Gavern
	UpgradeButton.Get()->SetVisibility(ESlateVisibility::Visible);
	UpgradeButton.Get()->SetNormalColor();
	UpgradeButton.Get()->SetTextOfButton(GivePlateText);
	UpgradeButton.Get()->SetRenderTransformAngle(180);	
	UpgradeButton.Get()->OnClicked().AddUObject(this, &UNPCDialog::GiveExtraPlateToGavernInTutorial);
	GamepadCurrentButton = UpgradeButton;
	RequestRefreshFocus();
}

void UNPCDialog::GiveExtraPlateToGavernInTutorial()
{
	ContinueHandle.Unregister();
	//Used to hide mask
	BP_GiveExtraPlateToGavernInTutorial();
	//After giving Gavern the extra plate, show main NPC Menu
	UpgradeButton.Get()->OnClicked().RemoveAll(this);
	HideButtons();
	GivingExtraPlateInTutorial = true;
	Command1.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Command1.Get()->SetCommand(111000);
	Command1.Get()->SetNPCImage(NPCImage, NPCName);
	Command1.Get()->ExecuteAppearAnimation(true);
	BackHandle.Unregister();
	PlayAnimation(HandInCommands);
	NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	DialogImage.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UNPCDialog::GhunterFinishedTalking()
{
	PlayerArticyFlowPlayer.Get()->PlayBranch(GhunterBranchToAutoPlay);
	GhunterTalking = false;
}

void UNPCDialog::RemoveNotificationFromNPC()
{
	if(NPCName == FGameplayTags::Get().NPC_Gavern)
	{
		TavernLevel.Get()->GavernSprite.Get()->ShowNotification(false);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Goctor)
	{
		TavernLevel.Get()->GoctorSprite.Get()->ShowNotification(false);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gailor)
	{
		TavernLevel.Get()->GailorSprite.Get()->ShowNotification(false);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gmith)
	{
		TavernLevel.Get()->GmithSprite.Get()->ShowNotification(false);
	}
}


void UNPCDialog::PlayCertainDialog(FArticyId ArticyID)
{
	UIPlayer.Get()->PlayerArticyFlowPlayer.Get()->SetStartNodeById(ArticyID);
}

void UNPCDialog::EndShowingUpgrade()
{
	NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Visible);
	DialogImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if(GamepadCurrentButton.Get()->IsVisible())
	{
		GamepadCurrentButton.Get()->SetFocus();
	}
}

void UNPCDialog::ShowUpgradeText()
{
	NPCScrollBox.Get()->SetVisibility(ESlateVisibility::Visible);
	DialogImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	InUpgradeDialog = true;
	//Show Upgrade Text
	if(NPCName == FGameplayTags::Get().NPC_Goctor)
	{
		PlayerArticyFlowPlayer.Get()->SetStartNodeById(UpgradeDialogs[TTGameInstance.Get()->UpgradeTier[0] - 1]);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gailor)
	{
		PlayerArticyFlowPlayer.Get()->SetStartNodeById(UpgradeDialogs[TTGameInstance.Get()->UpgradeTier[1] - 1]);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gmith)
	{
		PlayerArticyFlowPlayer.Get()->SetStartNodeById(UpgradeDialogs[TTGameInstance.Get()->UpgradeTier[2] - 1]);
	}
	else if(NPCName == FGameplayTags::Get().NPC_Gavern)
	{
		ShowGavernUpgrade(TTGameInstance.Get()->GavernUpgrade);
	}
}

void UNPCDialog::ChangeGamepadCurrentCutton(TObjectPtr<UButtonBase> NewButton)
{
	GamepadCurrentButton = NewButton;
	GamepadCurrentButton.Get()->SetFocus();
}

void UNPCDialog::CheckAmountOfCommands()
{
	if(NumberOfCommandsToGive <= 3)
	{
		StopAnimation(HandInCommands);
	}
}

void UNPCDialog::HeadToLastLevel()
{
	TavernLevel.Get()->GoToFinalLevel();
}