// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBoard.h"
#include "CommandUI.h"
#include "CommonTextBlock.h"
#include "MiniCommandUI.h"
#include "../../Buttons/ButtonBase.h"
#include "../../../../Levels/TavernLevel.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Input/CommonUIInputTypes.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Inventory/Inventory.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Buttons/HoldButton.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/AmmoUpgrade.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/HealthUpgrade.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/WeaponUpgrade.h"

void UQuestBoard::NativeConstruct()
{
	Super::NativeConstruct();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	//Command Images 
	TTGameInstance.Get()->CommandImagesTable->GetAllRows("", ImagesOfRecipe);
	//Recipes
	TTGameInstance.Get()->RecipeDataTable->GetAllRows("", Recipes);
	//Level Progression
	TTGameInstance.Get()->LevelDataTable.Get()->GetAllRows("", LevelsInfo);
	if(TTGameInstance.Get()->CurrentLevel != -1)
	{
		NumberOfCommandsAvailable = LevelsInfo[TTGameInstance.Get()->CurrentLevel+1]->NumberOfPlayerAvailableCommands;
		NumberOfCommandsAvailable -= TTGameInstance.Get()->AcceptedOrders.Num() - TTGameInstance.Get()->FinishedOrders.Num();
	}
	//Hide Tutorial Info
	NextButtonBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	NextButtonImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	TutorialMainCanvasPanel.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Hide and Bind Delete Command Button
	DeleteButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	AcceptAction.Get()->SetVisibility(ESlateVisibility::Hidden);
	AcceptAction.Get()->OnHoldFinished.AddDynamic(this,&UQuestBoard::AcceptCommands);
	
	SetUpCommands();
	
	//Reset Number of Commands In Quest Board per NPC
	NPCNumberOfCommandsInQuestBoard.Empty();
	NPCNumberOfCommandsInQuestBoard.Add(FGameplayTags::Get().NPC_Goctor, 0);
	NPCNumberOfCommandsInQuestBoard.Add(FGameplayTags::Get().NPC_Gailor, 0);
	NPCNumberOfCommandsInQuestBoard.Add(FGameplayTags::Get().NPC_Gmith, 0);
	MaxNumberOfNPCCommands = 3;
	UpdateMiniCommandsInQuestBoardDifficulty();
	//Show QuestBoard Commands depending on progression level
	//and active and completed commands
	RandomProgressionCommands();

	//Set Staring Button for Gamepad
	//RefreshMiniCommandTargetForGamepad();
	
	//Show number of commands that can be selected
	SelectedTextBlock.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), 0, NumberOfCommandsAvailable)));
	//Save satisfaction summed up by all commands
	GoctorFutureSatisfactionValue = 0;
	GailorFutureSatisfactionValue = 0;
	GmithFutureSatisfactionValue = 0;
	//Show Satisfaction Bars
	VisibilityOfSatisfactionBars();
	//Check progression of NPC satisfaction bars
	ObtainSatisfactionProgress();
	//Update satisfaction bars with accepted and finished commands
	UpdateSatisfactionBars();

	EnableCommands();
	//Play Animation
	PlayAnimation(TableOfActiveCommandsAppear);
	if(TTGameInstance.Get()->CurrentLevel == 2 && TTGameInstance.Get()->FirstTimeInLevel2)
	{
		TutorialMainCanvasPanel.Get()->SetVisibility(ESlateVisibility::Visible);
		QuestBoardTutorial();
	}
	else
	{
		StaringButtonForGamepad = MiniCommand1;
		TutorialMainCanvasPanel.Get()->SetVisibility(ESlateVisibility::Hidden);
		BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UQuestBoard::ReturnToTavern)));
		InventoryHandle = RegisterUIActionBinding(FBindUIActionArgs(InventoryInputActionData, true, FSimpleDelegate::CreateUObject(this, &UQuestBoard::ManageInventory)));
	}
}

void UQuestBoard::NativeOnActivated()
{
	Super::NativeOnActivated();
	if(TTGameInstance.Get()->CurrentLevel != -1)
	{
		NumberOfCommandsAvailable = LevelsInfo[TTGameInstance.Get()->CurrentLevel+1]->NumberOfPlayerAvailableCommands;
		UpdateSelectedRequests();
	}
	//Save satisfaction summed up by all commands
	GoctorFutureSatisfactionValue = 0;
	GailorFutureSatisfactionValue = 0;
	GmithFutureSatisfactionValue = 0;
	//Show Satisfaction Bars
	VisibilityOfSatisfactionBars();
	//Check progression of NPC satisfaction bars
	ObtainSatisfactionProgress();
	//Update satisfaction bars with accepted and finished commands
	UpdateSatisfactionBars();
}

void UQuestBoard::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	for(TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommandUI : CommandsInQuestBoard)
	{
		miniCommandUI.Value.Get()->OnCommandAccepted.RemoveAll(this);
	}
	for (TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		command.Get()->ResetCommand();
	}
}

void UQuestBoard::NativeDestruct()
{
	Super::NativeDestruct();

	BackHandle.Unregister();
	InventoryHandle.Unregister();
	DeleteHandle.Unregister();
	DeleteButton.Get()->OnClicked().RemoveAll(this);
	
	Command1.Get()->OnCommandClicked.RemoveAll(this);
	Command2.Get()->OnCommandClicked.RemoveAll(this);
	Command3.Get()->OnCommandClicked.RemoveAll(this);
	Command4.Get()->OnCommandClicked.RemoveAll(this);
	Command5.Get()->OnCommandClicked.RemoveAll(this);
	
	AcceptAction.Get()->OnHoldFinished.RemoveAll(this);
	CommandsInQuestBoard.Empty();
	ImagesOfRecipe.Empty();
	LevelsInfo.Empty();
	Recipes.Empty();
	SelectedCommands.Empty();
	HoveredCommand = nullptr;
	CommandToBeReturned = nullptr;
}

UWidget* UQuestBoard::NativeGetDesiredFocusTarget() const
{
	Super::NativeGetDesiredFocusTarget();
	return StaringButtonForGamepad;
}

FReply UQuestBoard::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(ContinueHandle.IsValid())
	{
		ContinueTutorial();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UQuestBoard::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if(Animation == OpenInventory)
	{
		InventoryHandle = RegisterUIActionBinding(FBindUIActionArgs(InventoryInputActionData, true, FSimpleDelegate::CreateUObject(this, &UQuestBoard::ManageInventory)));
	}
}

void UQuestBoard::SetUpCommands()
{
	//Initialize the 9 MiniCommands
	MiniCommand1.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand1.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[0]);
	MiniCommand2.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand2.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[1]);
	MiniCommand3.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand3.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[2]);
	MiniCommand4.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand4.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[3]);
	MiniCommand5.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand5.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[4]);
	MiniCommand6.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand6.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[5]);
	MiniCommand7.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand7.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[6]);
	MiniCommand8.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand8.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[7]);
	MiniCommand9.Get()->SetVisibility(ESlateVisibility::Hidden);
	MiniCommand9.Get()->SetPlateID(TTGameInstance.Get()->PositionOfCommandsInQuestBoard[8]);
	
	CommandsInQuestBoard.Add(0,MiniCommand1);
	CommandsInQuestBoard.Add(1,MiniCommand2);
	CommandsInQuestBoard.Add(2,MiniCommand3);
	CommandsInQuestBoard.Add(3,MiniCommand4);
	CommandsInQuestBoard.Add(4,MiniCommand5);
	CommandsInQuestBoard.Add(5,MiniCommand6);
	CommandsInQuestBoard.Add(6,MiniCommand7);
	CommandsInQuestBoard.Add(7,MiniCommand8);
	CommandsInQuestBoard.Add(8,MiniCommand9);

	//Initialize the 6 Selected Commands
	Command1.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command1.Get()->SetQuestBoard(this);
	Command1.Get()->SetPlateID(-1);
	Command1.Get()->SetIsInteractionEnabled(false);
	Command2.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command2.Get()->SetQuestBoard(this);
	Command2.Get()->SetPlateID(-1);
	Command2.Get()->SetIsInteractionEnabled(false);
	Command3.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command3.Get()->SetQuestBoard(this);
	Command3.Get()->SetPlateID(-1);
	Command3.Get()->SetIsInteractionEnabled(false);
	Command4.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command4.Get()->SetQuestBoard(this);
	Command4.Get()->SetPlateID(-1);
	Command4.Get()->SetIsInteractionEnabled(false);
	Command5.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command5.Get()->SetQuestBoard(this);
	Command5.Get()->SetPlateID(-1);
	Command5.Get()->SetIsInteractionEnabled(false);
	Command6.Get()->SetVisibility(ESlateVisibility::Hidden);
	Command6.Get()->SetQuestBoard(this);
	Command6.Get()->SetPlateID(-1);
	Command6.Get()->SetIsInteractionEnabled(false);

	SelectedCommands.Add(Command1);
	SelectedCommands.Add(Command2);
	SelectedCommands.Add(Command3);
	if(NumberOfCommandsAvailable >= 3)
	{
		SelectedCommands.Add(Command4);
	}
	if(NumberOfCommandsAvailable >= 4)
	{
		SelectedCommands.Add(Command5);
	}
	if(NumberOfCommandsAvailable >= 5)
	{
		SelectedCommands.Add(Command6);
	}
}

void UQuestBoard::ReturnToTavern()
{
	TObjectPtr<ATavernLevel> tavernLevel = Cast<ATavernLevel>(GetWorld()->GetLevelScriptActor());
	tavernLevel.Get()->QuestBoardAnimation(false);
	if(InventoryIsOpened)
	{
		PlayAnimationReverse(OpenInventory, 6);
	}
	InventoryIsOpened = false;
	DeactivateWidget();
}

void UQuestBoard::RefreshMiniCommandTargetForGamepad()
{
	for (TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommand : CommandsInQuestBoard)
	{
		if(miniCommand.Value.Get()->IsVisible() && miniCommand.Value.Get()->GetIsAvailable())
		{
			StaringButtonForGamepad = miniCommand.Value;
			RequestRefreshFocus();
			break;
		}
	}
}

void UQuestBoard::ManageInventory()
{
	InventoryIsOpened = !InventoryIsOpened;
	if(InventoryIsOpened)
	{
		PlayAnimation(OpenInventory);
		if(HoveredCommand)
		{
			//TODO Probably remove
			HoveredCommand.Get()->SetVisibility(ESlateVisibility::Hidden);
			HoveredCommand.Get()->SetPlateID(-1);
		}
		DisableCommands();
	}
	else
	{
		PlayAnimationReverse(OpenInventory);
		EnableCommands();
	}
	InventoryHandle.Unregister();
}
//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------Commands Overall Logic-----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

TObjectPtr<UMiniCommandUI> UQuestBoard::SetCommandInRandomPosition(int PlateID, FSlateBrush NPCBrush, FGameplayTag NPCTag)
{
	TObjectPtr<UMiniCommandUI> command;
	int difficultyOfPlate = GetDifficultyOfCommand(PlateID);
	command = GetMiniCommandDependingOnDifficultyLevel(difficultyOfPlate, NPCTag);
	if(command)
	{
		command.Get()->EstablishCommand(PlateID, this);
		command.Get()->SetNPCImage(NPCBrush, NPCTag);
		command.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TTGameInstance.Get()->NPCInformationMap[NPCTag].NPCOrders.Add(PlateID);
		NPCNumberOfCommandsInQuestBoard[NPCTag] = NPCNumberOfCommandsInQuestBoard[NPCTag] + 1;
		for (TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommandInQuestBoard : CommandsInQuestBoard)
		{
			if(miniCommandInQuestBoard.Value.Get() == command)
			{
				TTGameInstance.Get()->PositionOfCommandsInQuestBoard[miniCommandInQuestBoard.Key] = PlateID;
				break;
			}
		}
		return command;
	}
	return nullptr;
}

int UQuestBoard::ShowActiveCompletedCommands()
{
	int numberOfActiveCompletedCommands = 0;
	//Check if there are any completed commands
	for (FSecondaryCommand completedPlate : TTGameInstance.Get()->FinishedOrders)
	{
		for (FCommandImage* commandImage : ImagesOfRecipe)
		{
			if(commandImage->Tag == completedPlate.NPCTag)
			{
				TObjectPtr<UMiniCommandUI> miniCommandInQuestBoard;
				for(TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommand : CommandsInQuestBoard)
				{
					if(miniCommand.Value.Get()->GetPlateID() == completedPlate.PlateID && miniCommand.Value.Get()->GetNPCTAG() == completedPlate.NPCTag)
					{
						miniCommandInQuestBoard = miniCommand.Value;
						break;
					}
				}
				if(miniCommandInQuestBoard)
				{
					miniCommandInQuestBoard.Get()->SetIsAvailable(false);
					miniCommandInQuestBoard.Get()->EstablishCommand(completedPlate.PlateID, this);
					miniCommandInQuestBoard.Get()->SetNPCImage(GetNPCImage(completedPlate.NPCTag), completedPlate.NPCTag);
					miniCommandInQuestBoard.Get()->OnCommandAccepted.RemoveAll(this);
					miniCommandInQuestBoard.Get()->SetColorAndOpacity(DarkCommandColor);
					miniCommandInQuestBoard.Get()->SetMiniCommandCompleted();
					miniCommandInQuestBoard.Get()->PlayAppearAnimation();
					AddCompleteOrPendingSelectedCommand(miniCommandInQuestBoard, true);
					//NPCNumberOfCommandsInQuestBoard[completedPlate.NPCTag] = NPCNumberOfCommandsInQuestBoard[completedPlate.NPCTag] + 1;
					++numberOfActiveCompletedCommands;
				}
				break;
			}
		}
	}
	//Check if there are any active commands
	for (FSecondaryCommand activePlate : TTGameInstance.Get()->AcceptedOrders)
	{
		for (FCommandImage* commandImage : ImagesOfRecipe)
		{
			if(commandImage->Tag == activePlate.NPCTag)
			{
				TObjectPtr<UMiniCommandUI> miniCommandInQuestBoard;
				for(TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommand : CommandsInQuestBoard)
				{
					if(miniCommand.Value.Get()->GetPlateID() == activePlate.PlateID && miniCommand.Value.Get()->GetNPCTAG() == activePlate.NPCTag)
					{
						miniCommandInQuestBoard = miniCommand.Value;
						break;
					}
				}
				if(miniCommandInQuestBoard)
				{
					miniCommandInQuestBoard.Get()->SetIsAvailable(false);
					miniCommandInQuestBoard.Get()->EstablishCommand(activePlate.PlateID, this);
					miniCommandInQuestBoard.Get()->SetNPCImage(GetNPCImage(activePlate.NPCTag), activePlate.NPCTag);
					miniCommandInQuestBoard.Get()->OnCommandAccepted.RemoveAll(this);
					miniCommandInQuestBoard.Get()->SetColorAndOpacity(DarkCommandColor);
					miniCommandInQuestBoard.Get()->PlayAppearAnimation();
					AddCompleteOrPendingSelectedCommand(miniCommandInQuestBoard);
					//NPCNumberOfCommandsInQuestBoard[activePlate.NPCTag] = NPCNumberOfCommandsInQuestBoard[activePlate.NPCTag] + 1;
					++numberOfActiveCompletedCommands;
				}
				break;
			}
		}
	}
	
	return numberOfActiveCompletedCommands;
}

void UQuestBoard::RandomProgressionCommands()
{
	//Set Commands depending on number of commands that can appear in QuestBoard
	for(FLevelRow* level : LevelsInfo)
	{
		if(level->Level == TTGameInstance.Get()->CurrentLevel)
		{
			int commandsInPlace = 0;
			//Check the commands saved in NPCOrders
			for(TTuple<FGameplayTag, FNPCInformationStruct> npcInformation : TTGameInstance.Get()->NPCInformationMap)
			{
				//For each NPCs orders
				for (int npcOrder : npcInformation.Value.NPCOrders)
				{
					//Search for them in the QuestBoard and establish them
					for (TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommandInQuestBoard : CommandsInQuestBoard)
					{
						if(miniCommandInQuestBoard.Value.Get()->GetPlateID() == npcOrder && !miniCommandInQuestBoard.Value->GetNPCTAG().IsValid())
						{
							miniCommandInQuestBoard.Value.Get()->EstablishCommand(npcOrder, this);
							miniCommandInQuestBoard.Value.Get()->SetNPCImage(GetNPCImage(npcInformation.Key), npcInformation.Key);
							miniCommandInQuestBoard.Value.Get()->SetVisibility(ESlateVisibility::Visible);
							++NPCNumberOfCommandsInQuestBoard[npcInformation.Key];
							break;
						}
					}
					++commandsInPlace;
				}
			}

			ShowActiveCompletedCommands();

			//The are 3 commands for each NPC, if there are less, search for a new one and add it to the Quest Board
			//Variable used to search BoardOrders from the beginning
			int numberOfCommandInBoardOrder = -1;
			for(int i = commandsInPlace; i < level->NumberOfSecondaryCommands; i++)
			{
				++numberOfCommandInBoardOrder;
				if(numberOfCommandInBoardOrder == TTGameInstance.Get()->BoardOrders.Num())
				{
					numberOfCommandInBoardOrder = 0;
				}
				int randomOrderID = TTGameInstance.Get()->BoardOrders[numberOfCommandInBoardOrder % TTGameInstance.Get()->BoardOrders.Num()];
				//Go through the three NPC
				for(TTuple<FGameplayTag, FNPCInformationStruct> npcInformation : TTGameInstance.Get()->NPCInformationMap)
				{
					if(npcInformation.Key == FGameplayTags::Get().NPC_Gavern)
					{
						//No need to keep looking
						break;
					}
					if(NPCNumberOfCommandsInQuestBoard[npcInformation.Key] != MaxNumberOfNPCCommands)
					{
						if(!SetCommandInRandomPosition(randomOrderID, GetNPCImage(npcInformation.Key), npcInformation.Key))
						{
							--i;
						}
						break;
					}
				}
			}
			break;
		}
	}
}

int UQuestBoard::GetDifficultyOfCommand(int PlateID)
{
	int commandDifficulty = 1;
	for (FRecipes_Struct* recipe : Recipes)
	{
		if(recipe->ID == PlateID)
		{
			commandDifficulty = recipe->Stars;
			break;			
		}
	}
	return commandDifficulty;
}

void UQuestBoard::NoMoreSelectedPlatesAnimation()
{
	PlayAnimation(SelectedCommandsTextShake);
	for (TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		if(command.Get()->IsInPreview())
		{
			command.Get()->ShakeCommand();
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------Mini Commands-------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void UQuestBoard::AddCompleteOrPendingSelectedCommand(TObjectPtr<UMiniCommandUI> MiniCommand, bool IsCompleted)
{
	for (TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		if(!command.Get()->IsVisible())
		{
			command.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			command.Get()->SetCommand(MiniCommand.Get()->GetPlateID());
			command.Get()->SetNPCImage(GetNPCImage(MiniCommand.Get()->GetNPCTAG()), MiniCommand.Get()->GetNPCTAG());
			command.Get()->SetNewColor(DarkCommandColor);
			command.Get()->SetCurrentColor();
			command.Get()->ExecuteAppearAnimation(true);
			break;
		}
	}
}

bool UQuestBoard::AddToSelectedRequests(UMiniCommandUI* MiniCommandUI)
{
	//First check if bar is full, if not, add it to selected requests
	int starsOfCommand = GetDifficultyOfCommand(MiniCommandUI->GetPlateID());
	if(!GoctorSatisfactionBar.Get()->PlayerCompletedAllTiers() && MiniCommandUI->GetNPCTAG() == FGameplayTags::Get().NPC_Goctor &&
		(GoctorUpgradeProgress + GoctorFutureSatisfactionValue - starsOfCommand) >=
			TTGameInstance.Get()->GoctorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel])
	{
		PlayAnimation(GoctorBarShake);
		return false;
	}
	if(!GailorSatisfactionBar.Get()->PlayerCompletedAllTiers() && MiniCommandUI->GetNPCTAG() == FGameplayTags::Get().NPC_Gailor &&
		(GailorUpgradeProgress + GailorFutureSatisfactionValue - starsOfCommand) >=
			TTGameInstance.Get()->GailorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel])
	{
		PlayAnimation(GailorBarShake);
		return false;
	}
	if(!GmithSatisfactionBar.Get()->PlayerCompletedAllTiers() && MiniCommandUI->GetNPCTAG() == FGameplayTags::Get().NPC_Gmith &&
		(GmithUpgradeProgress + GmithFutureSatisfactionValue - starsOfCommand) >=
			TTGameInstance.Get()->GmithSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel])
	{
		PlayAnimation(GmithBarShake);
		return false;
	}
	//Add Command to Selected Requests
	TObjectPtr<UCommandUI> activeCommand = CreateActiveCommand(MiniCommandUI);
	if(activeCommand)
	{
		//Play Command Accepted Sound
		PlaySound(AcceptedCommandSound);

		//Show Accept Box
		AcceptAction.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		activeCommand.Get()->SetCommand(MiniCommandUI->GetPlateID());
		activeCommand.Get()->SetInPreview(false);
		activeCommand.Get()->SetNPCImage(GetNPCImage(MiniCommandUI->GetNPCTAG()), MiniCommandUI->GetNPCTAG());
		activeCommand.Get()->EndPreviewAnimation();
		activeCommand.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		MiniCommandUI->SetColorAndOpacity(DarkCommandColor);
		MiniCommandUI->OnCommandAccepted.RemoveAll(this);

		// DeleteBox.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DeleteButton.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DeleteButton.Get()->OnClicked().RemoveAll(this);
		DeleteButton.Get()->OnClicked().AddUObject(this, &UQuestBoard::RemoveCommandFromActive);
		DeleteHandle.Unregister();
		DeleteHandle = RegisterUIActionBinding(FBindUIActionArgs(DeleteInputActionData, false, FSimpleDelegate::CreateUObject(this, &UQuestBoard::RemoveCommandFromActive)));
		
		//Update Selected Commands Text Block
		UpdateSelectedRequests();
		//Update future value of npc satisfaction bars
		UpdateSatisfactionBars();
		return true;
	}
	return false;
}

void UQuestBoard::HoverSatisfaction(TObjectPtr<UMiniCommandUI> MiniCommand, bool IsHovering)
{
	//First check if miniCommand was already selected
	for (TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		if(!command.Get()->IsInPreview() && MiniCommand.Get()->GetPlateID() == command.Get()->GetPlateID() && MiniCommand.Get()->GetNPCTAG() == command.Get()->GetNPCTAG())
		{
			if(IsHovering)
			{
				HoveredCommand = nullptr;
				HoverCommand(command);
			}
			else
			{
				HoveredCommand = nullptr;
				UnhoverCommand(command);
			}
			return;
		}
	}
	
	//Else Update Satisfaction bars
	int satisfactionOfCommand = 0;
	for(FRecipes_Struct* recipe : Recipes)
	{
		if(recipe->ID == MiniCommand.Get()->GetPlateID())
		{
			satisfactionOfCommand = recipe->Stars;
			//Check NPC and update it's satisfaction bar
			if(MiniCommand.Get()->GetNPCTAG() == FGameplayTags::Get().NPC_Goctor)
			{
				if(IsHovering)
				{
					GoctorFutureSatisfactionValue += satisfactionOfCommand;
				}
				else
				{
					GoctorFutureSatisfactionValue -= satisfactionOfCommand;
				}
				float goctorMaxSatisfactionBarValue = TTGameInstance.Get()->GoctorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel];
				float futureValue = GoctorUpgradeProgress + GoctorFutureSatisfactionValue;
				futureValue = FMath::Clamp(futureValue,0,goctorMaxSatisfactionBarValue);
				float newFutureValue = futureValue / goctorMaxSatisfactionBarValue;
				GoctorSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(futureValue);
			}
			else if(MiniCommand.Get()->GetNPCTAG() == FGameplayTags::Get().NPC_Gailor)
			{
				if(IsHovering)
				{
					GailorFutureSatisfactionValue += satisfactionOfCommand;
				}
				else
				{
					GailorFutureSatisfactionValue -= satisfactionOfCommand;
				}
				float gailorMaxSatisfactionBarValue = TTGameInstance.Get()->GailorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel];
				float futureValue = GailorUpgradeProgress + GailorFutureSatisfactionValue;
				futureValue = FMath::Clamp(futureValue,0,gailorMaxSatisfactionBarValue);
				float newFutureValue = futureValue / gailorMaxSatisfactionBarValue;
				GailorSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(futureValue);
			}
			else if(MiniCommand.Get()->GetNPCTAG() == FGameplayTags::Get().NPC_Gmith)
			{
				if(IsHovering)
				{
					GmithFutureSatisfactionValue += satisfactionOfCommand;
				}
				else
				{
					GmithFutureSatisfactionValue -= satisfactionOfCommand;
				}
				float gmithMaxSatisfactionBarValue = TTGameInstance.Get()->GmithSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel];
				float futureValue = GmithUpgradeProgress + GmithFutureSatisfactionValue; 
				futureValue = FMath::Clamp(futureValue,0,gmithMaxSatisfactionBarValue);
				float newFutureValue = futureValue / gmithMaxSatisfactionBarValue;
				GmithSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(futureValue);
			}
			break;
		}
	}
	if(IsHovering)
	{
		//Show a preview of command
		for (TObjectPtr<UCommandUI> selectedCommand : SelectedCommands)
		{
			if(selectedCommand.Get()->GetPlateID() == -1)
			{
				HoveredCommand = selectedCommand;
				HoveredCommand.Get()->SetInPreview(true);
				HoveredCommand.Get()->SetVisibility(ESlateVisibility::Visible);
				HoveredCommand.Get()->SetCommand(MiniCommand.Get()->GetPlateID());
				HoveredCommand.Get()->SetNPCImage(GetNPCImage(MiniCommand.Get()->GetNPCTAG()), MiniCommand.Get()->GetNPCTAG());
				HoveredCommand.Get()->ExecuteAppearAnimation(true,true);
				break;
			}
		}
	}
	else
	{
		//Hide Preview
		if(HoveredCommand)
		{
			HoveredCommand.Get()->SetInPreview(false);
			HoveredCommand.Get()->ExecuteAppearAnimation(false);
			HoveredCommand.Get()->SetPlateID(-1);
			HoveredCommand = nullptr;
		}
	}
}

TObjectPtr<UCommandUI> UQuestBoard::CreateActiveCommand(UMiniCommandUI* MiniCommandUI)
{
	for(TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		if(command.Get()->GetPlateID() == MiniCommandUI->GetPlateID() && command.Get()->GetNPCTAG() == MiniCommandUI->GetNPCTAG())
		{
			CommandToBeReturned = command;
			return command;
		}
	}
	
	return nullptr;
}

TObjectPtr<UMiniCommandUI> UQuestBoard::GetMiniCommandDependingOnDifficultyLevel(int DifficultyOfPlate,
                                                                                 FGameplayTag NPCTag)
{
	//Goctor
	if(NPCTag == FGameplayTags::Get().NPC_Goctor)
	{
		if(GoctorDifficultyPossibleCommands[0] == DifficultyOfPlate && CommandsInQuestBoard[0].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[0];
		}
		if(GoctorDifficultyPossibleCommands[1] == DifficultyOfPlate && CommandsInQuestBoard[1].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[1];
		}
		if(GoctorDifficultyPossibleCommands[2] == DifficultyOfPlate && CommandsInQuestBoard[2].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[2];
		}
	}
	//Gailor
	else if(NPCTag == FGameplayTags::Get().NPC_Gailor)
	{
		if(GailorDifficultyPossibleCommands[0] == DifficultyOfPlate && CommandsInQuestBoard[3].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[3];
		}
		if(GailorDifficultyPossibleCommands[1] == DifficultyOfPlate && CommandsInQuestBoard[4].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[4];
		}
		if(GailorDifficultyPossibleCommands[2] == DifficultyOfPlate && CommandsInQuestBoard[5].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[5];
		}
	}
	//Gmith
	else if(NPCTag == FGameplayTags::Get().NPC_Gmith)
	{
		if(GmithDifficultyPossibleCommands[0] == DifficultyOfPlate && CommandsInQuestBoard[6].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[6];
		}
		if(GmithDifficultyPossibleCommands[1] == DifficultyOfPlate && CommandsInQuestBoard[7].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[7];
		}
		if(GmithDifficultyPossibleCommands[2] == DifficultyOfPlate && CommandsInQuestBoard[8].Get()->GetPlateID() == -1)
		{
			return CommandsInQuestBoard[8];
		}
	}
	
	return nullptr;
}

void UQuestBoard::UpdateMiniCommandsInQuestBoardDifficulty()
{
	//Goctor
	switch (TTGameInstance.Get()->UpgradeTier[0])
	{
	case 0:
		GoctorDifficultyPossibleCommands[0] = 1;
		GoctorDifficultyPossibleCommands[1] = 1;
		GoctorDifficultyPossibleCommands[2] = 2;
		break;
	case 1:
		GoctorDifficultyPossibleCommands[0] = 1;
		GoctorDifficultyPossibleCommands[1] = 2;
		GoctorDifficultyPossibleCommands[2] = 3;
		break;
	case 2:
	case 3:
		GoctorDifficultyPossibleCommands[0] = 2;
		GoctorDifficultyPossibleCommands[1] = 3;
		GoctorDifficultyPossibleCommands[2] = 4;
		break;
	}
	//Gailor
	switch (TTGameInstance.Get()->UpgradeTier[1])
	{
	case 0:
		GailorDifficultyPossibleCommands[0] = 1;
		GailorDifficultyPossibleCommands[1] = 1;
		GailorDifficultyPossibleCommands[2] = 2;
		break;
	case 1:
		GailorDifficultyPossibleCommands[0] = 1;
		GailorDifficultyPossibleCommands[1] = 2;
		GailorDifficultyPossibleCommands[2] = 3;
		break;
	case 2:
		GailorDifficultyPossibleCommands[0] = 2;
		GailorDifficultyPossibleCommands[1] = 3;
		GailorDifficultyPossibleCommands[2] = 4;
		break;
	case 3:
	case 4:
		GailorDifficultyPossibleCommands[0] = 3;
		if(TTGameInstance.Get()->CurrentLevel >= 4)
		{
			GailorDifficultyPossibleCommands[1] = 4;
			GailorDifficultyPossibleCommands[2] = 5;
		}
		else
		{
			GailorDifficultyPossibleCommands[1] = 3;
			GailorDifficultyPossibleCommands[2] = 4;
		}
		
		break;
	}
	//Gmith
	switch (TTGameInstance.Get()->UpgradeTier[2])
	{
	case 0:
		GmithDifficultyPossibleCommands[0] = 1;
		GmithDifficultyPossibleCommands[1] = 2;
		GmithDifficultyPossibleCommands[2] = 3;
		break;
	case 1:
		GmithDifficultyPossibleCommands[0] = 2;
		GmithDifficultyPossibleCommands[1] = 3;
		GmithDifficultyPossibleCommands[2] = 4;
		break;
	case 2:
	case 3:
		GmithDifficultyPossibleCommands[0] = 3;
		if(TTGameInstance.Get()->CurrentLevel >= 4)
		{
			GmithDifficultyPossibleCommands[1] = 4;
			GmithDifficultyPossibleCommands[2] = 5;
		}
		else
		{
			GmithDifficultyPossibleCommands[1] = 3;
			GmithDifficultyPossibleCommands[2] = 4;
		}
		break;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------Selected Commands-------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

void UQuestBoard::HoverCommand(UCommandUI* Command)
{
	HoveredCommand = Command;
	Command->WhiteCommand();
	Command->SetRenderScale(FVector2d(1.1f, 1.1f));
	TObjectPtr<UCanvasPanelSlot> hoveredCommandCanvasSlot = Cast<UCanvasPanelSlot>(HoveredCommand.Get()->Slot);
	hoveredCommandCanvasSlot.Get()->SetZOrder(hoveredCommandCanvasSlot.Get()->GetZOrder() + 10);
}

void UQuestBoard::UnhoverCommand(UCommandUI* Command)
{
	if(Command)
	{
		Command->SetRenderScale(FVector2d(1.0f, 1.0f));
		Command->SetCurrentColor();
		TObjectPtr<UCanvasPanelSlot> hoveredCommandCanvasSlot = Cast<UCanvasPanelSlot>(Command->Slot);
		if(hoveredCommandCanvasSlot.Get()->GetZOrder() > 5)
		{
			hoveredCommandCanvasSlot.Get()->SetZOrder(hoveredCommandCanvasSlot.Get()->GetZOrder() - 10);
		}
	}
}

void UQuestBoard::RemoveCommandFromActive()
{
	//Play Command back To Quest Board Sound
	PlaySound(CommandBackToQuestBoardSound);
	//UnhoverCommand(CommandToBeReturned);
	// HoveredCommand = nullptr;
	ReturnToQuestBoard(CommandToBeReturned);
	//Move commands in selected requests
	int numberOfCommandsInSelectedRequests = GetNumberOfCommandsInSelectedRequests();
	int lastCommand = numberOfCommandsInSelectedRequests - 1;
	SelectedCommands[lastCommand].Get()->SetPlateID(-1);
	SelectedCommands[lastCommand].Get()->ExecuteAppearAnimation(false);
	//Check if any command in selected requests is visible. If there all invisible, hide de DeleteBox and AcceptBox
	bool allInvisible = true;
	for(int i = numberOfCommandsInSelectedRequests - 2; i >= 0 ; i--)
	{
		if(SelectedCommands[i].Get()->GetPlateID() != -1 &&
			!TTGameInstance.Get()->FinishedOrders.Contains(FSecondaryCommand(SelectedCommands[i].Get()->GetNPCTAG(), SelectedCommands[i].Get()->GetPlateID())) &&
			!TTGameInstance.Get()->AcceptedOrders.Contains(FSecondaryCommand(SelectedCommands[i].Get()->GetNPCTAG(), SelectedCommands[i].Get()->GetPlateID())))
		{
			CommandToBeReturned = SelectedCommands[i];
			allInvisible = false;
			break;
		}
	}
	if(allInvisible)
	{

		DeleteButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		DeleteButton.Get()->OnClicked().RemoveAll(this);
		DeleteHandle.Unregister();
		AcceptAction.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	//Check if any command is in preview and stop hover in that command and change it to an upper one
	int idOfPreviewedCommand = -1;
	FGameplayTag tagOfPreviewedNPCTag;
	for (TObjectPtr<UCommandUI> previewedCommand : SelectedCommands)
	{
		if(previewedCommand.Get()->IsInPreview())
		{
			idOfPreviewedCommand = previewedCommand.Get()->GetPlateID();
			tagOfPreviewedNPCTag = previewedCommand.Get()->GetNPCTAG();
			break;
		}
	}
	//If there is a command previewing
	if(idOfPreviewedCommand != -1)
	{
		for (TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommand : CommandsInQuestBoard)
		{
			if(miniCommand.Value.Get()->GetPlateID() == idOfPreviewedCommand && miniCommand.Value.Get()->GetNPCTAG() == tagOfPreviewedNPCTag)
			{
				HoverSatisfaction(miniCommand.Value, false);
				HoverSatisfaction(miniCommand.Value, true);
				break;
			}
		}
	}
	//Update Selected Commands Text Block
	UpdateSelectedRequests();
	UpdateSatisfactionBars();
}

void UQuestBoard::AcceptCommands()
{
	for (TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		if(!TTGameInstance.Get()->FinishedOrders.Contains(FSecondaryCommand(command.Get()->GetNPCTAG(), command.Get()->GetPlateID())) &&
			!TTGameInstance.Get()->AcceptedOrders.Contains(FSecondaryCommand(command.Get()->GetNPCTAG(), command.Get()->GetPlateID())) &&
			command.Get()->GetPlateID() != -1 && !command.Get()->IsInPreview())
		{
			TTGameInstance.Get()->AcceptedOrders.Add(FSecondaryCommand(command.Get()->GetNPCTAG(), command.Get()->GetPlateID()));
			//Reset
			command.Get()->SetIsInteractionEnabled(false);
			command.Get()->SetNewColor(DarkCommandColor);
			command.Get()->SetCurrentColor();
			command.Get()->OnCommandClicked.RemoveAll(this);
		}
	}
	AcceptAction.Get()->SetVisibility(ESlateVisibility::Hidden);
	DeleteButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Update Inventory
	Inventory.Get()->ShowSecondaryCommands();
	Inventory.Get()->HideNoSecondaryCommands();
}

void UQuestBoard::ReturnToQuestBoard(TObjectPtr<UCommandUI> Command)
{
	uint16_t numberOfCommand = 0;
	for(TTuple<int, TObjectPtr<UMiniCommandUI>> command : CommandsInQuestBoard)
	{
		if(command.Value.Get()->GetPlateID() == Command.Get()->GetPlateID() && command.Value.Get()->GetNPCTAG() == Command.Get()->GetNPCTAG())
		{
			command.Value.Get()->EstablishCommand(Command.Get()->GetPlateID(), this);
			command.Value.Get()->SetInDissapearAnimation(false);
			command.Value.Get()->SetColorAndOpacity(FLinearColor(1,1,1,1));
			command.Value.Get()->SetUnAvailable(false);
			command.Value.Get()->SetIsAvailable(true);
			command.Value.Get()->OnCommandAccepted.RemoveAll(this);
			break;
		}
		++numberOfCommand;
	}
}

int UQuestBoard::GetNumberOfCommandsInSelectedRequests()
{
	int numberOfCommands = 0;
	for (TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		if(command.Get()->GetPlateID() != -1 && !command.Get()->IsInPreview())
		{
			++numberOfCommands;
			if(numberOfCommands == NumberOfCommandsAvailable)
			{
				break;
			}
		}
	}
	return numberOfCommands;
}

void UQuestBoard::UpdateSelectedRequests()
{
	//Show number of commands that can be selected
	SelectedTextBlock.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),
									GetNumberOfCommandsInSelectedRequests(),
									NumberOfCommandsAvailable)));
}

//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------NPC------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

FSlateBrush UQuestBoard::GetNPCImage(FGameplayTag NPCTag)
{
	for( FCommandImage* npcImage : ImagesOfRecipe)
	{
		if(npcImage->Tag == NPCTag)
		{
			return npcImage->ImageOfTag;
		}
	}
	return FSlateBrush();
}

void UQuestBoard::VisibilityOfSatisfactionBars()
{
	GoctorSatisfactionBar.Get()->SetVisibility(ESlateVisibility::Hidden);
	GailorSatisfactionBar.Get()->SetVisibility(ESlateVisibility::Hidden);
	GmithSatisfactionBar.Get()->SetVisibility(ESlateVisibility::Hidden);
	GoctorSatisfactionBar.Get()->UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel);
	GailorSatisfactionBar.Get()->UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel);
	GmithSatisfactionBar.Get()->UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel);
	GoctorSatisfactionBar.Get()->ShowUpgrades(TTGameInstance, false);
	GailorSatisfactionBar.Get()->ShowUpgrades(TTGameInstance, false);
	GmithSatisfactionBar.Get()->ShowUpgrades(TTGameInstance, false);
	//Visibility of NPCs
	for (FLevelRow* levelRow : LevelsInfo)
	{
		for(FGameplayTag npc: levelRow->NPCS)
		{
			if(npc == FGameplayTags::Get().NPC_Goctor)
			{
				GoctorSatisfactionBar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);	
				if(GoctorSatisfactionBar.Get()->PlayerCompletedAllTiers())
				{
					GoctorSatisfiedImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					GoctorSatisfiedText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			else if(npc == FGameplayTags::Get().NPC_Gailor)
			{
				GailorSatisfactionBar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				if(GailorSatisfactionBar.Get()->PlayerCompletedAllTiers())
				{
					GailorSatisfiedImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					GailorSatisfiedText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			else if(npc == FGameplayTags::Get().NPC_Gmith)
			{
				GmithSatisfactionBar.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				if(GmithSatisfactionBar.Get()->PlayerCompletedAllTiers())
				{
					GmithSatisfiedImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					GmithSatisfiedText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
		if(levelRow->Level == TTGameInstance.Get()->CurrentLevel)
		{
			break;
		}
	}
}

void UQuestBoard::ObtainSatisfactionProgress()
{
	GoctorUpgradeProgress = TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionValue;
	GailorUpgradeProgress = TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionValue;
	GmithUpgradeProgress = TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionValue;
	float maxGoctorSatisfactionValue = TTGameInstance.Get()->GoctorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel];
	float maxGailorSatisfactionValue = TTGameInstance.Get()->GailorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel];
	float maxGmithSatisfactionValue = TTGameInstance.Get()->GmithSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel];
	GoctorSatisfactionBar.Get()->AddPercentNormalSatisfactionbar(GoctorUpgradeProgress);
	GailorSatisfactionBar.Get()->AddPercentNormalSatisfactionbar(GailorUpgradeProgress);
	GmithSatisfactionBar.Get()->AddPercentNormalSatisfactionbar(GmithUpgradeProgress);
	GoctorSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(0);
	GailorSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(0);
	GmithSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(0);
}

void UQuestBoard::UpdateSatisfactionBars()
{
	//Clean
	GoctorSatisfactionBar.Get()->GetFutureSatistactionBar()->SetPercent(0);
	GailorSatisfactionBar.Get()->GetFutureSatistactionBar()->SetPercent(0);
	GmithSatisfactionBar.Get()->GetFutureSatistactionBar()->SetPercent(0);
	GoctorFutureSatisfactionValue = 0;
	GailorFutureSatisfactionValue = 0;
	GmithFutureSatisfactionValue = 0;
	
	//Add selected requests
	for(TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		if(command.Get()->IsVisible())
		{
			//Get Satisfaction of plate
			int satisfactionOfCommand = GetSatisfationOfARecipe(command.Get()->GetPlateID());
			DetectNPCSatisfactionBar(command.Get()->GetNPCTAG(), satisfactionOfCommand);
		}
	}
}

void UQuestBoard::DetectNPCSatisfactionBar(FGameplayTag NPCTag, int SatisfactionOfCommand)
{
	//Check NPC and update it's satisfaction bar
	if(NPCTag == FGameplayTags::Get().NPC_Goctor)
	{
		GoctorFutureSatisfactionValue += SatisfactionOfCommand;
		float goctorMaxSatisfactionBarValue = TTGameInstance.Get()->GoctorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel];
		float futureValue = GoctorUpgradeProgress + GoctorFutureSatisfactionValue;
		futureValue = FMath::Clamp(futureValue,0,goctorMaxSatisfactionBarValue);
		float newFutureValue = (1 / goctorMaxSatisfactionBarValue) * futureValue;
		GoctorSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(futureValue);
	}
	else if(NPCTag == FGameplayTags::Get().NPC_Gailor)
	{
		GailorFutureSatisfactionValue += SatisfactionOfCommand;
		float gailorMaxSatisfactionBarValue = TTGameInstance.Get()->GailorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel];
		float futureValue = GailorUpgradeProgress + GailorFutureSatisfactionValue;
		futureValue = FMath::Clamp(futureValue,0,gailorMaxSatisfactionBarValue);
		float newFutureValue = (1 / gailorMaxSatisfactionBarValue) * futureValue;
		GailorSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(futureValue);
	}
	else if(NPCTag == FGameplayTags::Get().NPC_Gmith)
	{
		GmithFutureSatisfactionValue += SatisfactionOfCommand;
		float gmithMaxSatisfactionBarValue = TTGameInstance.Get()->GmithSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel];
		float futureValue = GmithUpgradeProgress + GmithFutureSatisfactionValue; 
		futureValue = FMath::Clamp(futureValue,0,gmithMaxSatisfactionBarValue);
		float newFutureValue = (1 / gmithMaxSatisfactionBarValue) * futureValue;
		GmithSatisfactionBar.Get()->AddPercentFutureSatisfactionbar(futureValue);
	}
}

int UQuestBoard::GetSatisfationOfARecipe(int PlateID)
{
	for(FRecipes_Struct* recipe : Recipes)
	{
		if(recipe->ID == PlateID)
		{
			return recipe->Stars;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------TUTORIAL-------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void UQuestBoard::QuestBoardTutorial()
{
	QuestBoardTutorialAnimation();
}

void UQuestBoard::EnableContinueButton(bool ShouldBeEnabled)
{
	if(ShouldBeEnabled)
	{
		ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UQuestBoard::ContinueTutorial)));
	}
	else
	{
		ContinueHandle.Unregister();
	}
}

void UQuestBoard::EnableExitButton()
{
	//RefreshMiniCommandTargetForGamepad();
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UQuestBoard::ReturnToTavern)));
	InventoryHandle = RegisterUIActionBinding(FBindUIActionArgs(InventoryInputActionData, true, FSimpleDelegate::CreateUObject(this, &UQuestBoard::ManageInventory)));
}

void UQuestBoard::EnableCommands()
{
	for (TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommand : CommandsInQuestBoard)
	{
		miniCommand.Value.Get()->SetIsInteractionEnabled(true);
	}
}

void UQuestBoard::DisableCommands()
{
	for (TTuple<int, TObjectPtr<UMiniCommandUI>> miniCommand : CommandsInQuestBoard)
	{
		miniCommand.Value.Get()->SetIsInteractionEnabled(false);
	}
	for (TObjectPtr<UCommandUI> command : SelectedCommands)
	{
		command.Get()->SetIsInteractionEnabled(false);
	}
}

void UQuestBoard::GrabCommand()
{
	MiniCommand2.Get()->SetIsEnabled(false);
	MiniCommand3.Get()->SetIsEnabled(false);
	MiniCommand4.Get()->SetIsEnabled(false);
	MiniCommand5.Get()->SetIsEnabled(false);
	MiniCommand6.Get()->SetIsEnabled(false);
	MiniCommand1.Get()->OnClicked().AddUObject(this, &UQuestBoard::TutorialCommandGrabbed);
	StaringButtonForGamepad = MiniCommand1;
	RequestRefreshFocus();
}

void UQuestBoard::TutorialCommandGrabbed()
{
	TutorialMainCanvasPanel.Get()->SetVisibility(ESlateVisibility::Visible);
	MiniCommand1.Get()->OnClicked().RemoveAll(this);
	MiniCommand2.Get()->SetIsEnabled(true);
	MiniCommand3.Get()->SetIsEnabled(true);
	MiniCommand4.Get()->SetIsEnabled(true);
	MiniCommand5.Get()->SetIsEnabled(true);
	MiniCommand6.Get()->SetIsEnabled(true);
	ContinueTutorial();
}

