// TinyTavern


#include "Inventory.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CommonAnimatedSwitcher.h"
#include "CommonTextBlock.h"
#include "Components/GridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/ScaleBox.h"
#include "Components/WidgetSwitcherSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/AdventureUI/ProgressBarPoint.h"
#include "TinyTavernGame/UI/Menus/Buttons/InventorySlot.h"
#include "TinyTavernGame/UI/Menus/Tavern/QuestBoard/CommandUI.h"

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	TTGameInstance = Cast<UTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//Bind Buttons
	LeftSecondaryStatueButton.Get()->OnClicked().AddUObject(this, &UInventory::MoveSecondaryStatueToTheLeft);
	RightSecondaryStatueButton.Get()->OnClicked().AddUObject(this, &UInventory::MoveSecondaryStatueToTheRight);
	LeftExtraPlateButton.Get()->OnClicked().AddUObject(this, &UInventory::MoveExtraPlatesToTheLeft);
	RightExtraPlateButton.Get()->OnClicked().AddUObject(this, &UInventory::MoveExtraPlatesToTheRight);
	
	//Clear
	LeftSecondaryStatueSwitcher.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	RightSecondaryStatueSwitcher.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	LeftSecondaryStatueButton.Get()->SetVisibility(ESlateVisibility::Visible);
	RightSecondaryStatueButton.Get()->SetVisibility(ESlateVisibility::Visible);
	
	LeftExtraPlatesSwitcher.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	RightExtraPlatesSwitcher.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	LeftExtraPlateButton.Get()->SetVisibility(ESlateVisibility::Visible);
	RightExtraPlateButton.Get()->SetVisibility(ESlateVisibility::Visible);
	
	MainCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommands.Add(MainCommand1);
	MainCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommands.Add(MainCommand2);
	MainCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommands.Add(MainCommand3);
	MainCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommands.Add(MainCommand4);
	MainCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommands.Add(MainCommand5);
	MainCommand6.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommands.Add(MainCommand6);

	//Statue and Secondary Commands
	InitializeSecondaryPlates();

	//Extra Plates
	NumberOfVisibleExtraPlates = 0;
	InitializeExtraPlates();
	//-------------------------------------------------------------------------
	NoCommandsTextBlock.Get()->SetVisibility(ESlateVisibility::Hidden);
	EmptySecondaryCommand.Get()->SetVisibility(ESlateVisibility::Hidden);
	NoExtraPlatesTextBlock.Get()->SetVisibility(ESlateVisibility::Hidden);
	EmptyExtraPlateCommand.Get()->SetVisibility(ESlateVisibility::Hidden);
	//Obtain Images of recipes
	TTGameInstance.Get()->CommandImagesTable->GetAllRows("", ImagesOfRecipe);
	TTGameInstance.Get()->RecipeDataTable->GetAllRows("", Recipes);
	TTGameInstance.Get()->StatuesPlatesDataTable->GetAllRows("", StatuePlates);
	
	//If Inventory in Recipe Book
	//Show Active Main, Statue and Secondary Orders
	ShowMainCommands();
	CreateStatuePlatesInventory();
	SecondaryCommands.Add(SecondaryCommand1);
	SecondaryCommands.Add(SecondaryCommand2);
	SecondaryCommands.Add(SecondaryCommand3);
	SecondaryCommands.Add(SecondaryCommand4);
	SecondaryCommands.Add(SecondaryCommand5);
	//Left Commands
	LeftSecondaryCommands.Add(LeftSecondaryCommand1);
	LeftSecondaryCommands.Add(LeftSecondaryCommand2);
	LeftSecondaryCommands.Add(LeftSecondaryCommand3);
	LeftSecondaryCommands.Add(LeftSecondaryCommand4);
	LeftSecondaryCommands.Add(LeftSecondaryCommand5);
	//Right Commands
	RightSecondaryCommands.Add(RightSecondaryCommand1);
	RightSecondaryCommands.Add(RightSecondaryCommand2);
	RightSecondaryCommands.Add(RightSecondaryCommand3);
	RightSecondaryCommands.Add(RightSecondaryCommand4);
	RightSecondaryCommands.Add(RightSecondaryCommand5);
	
	ShowSecondaryCommands();
	//Create inventory of extra plates
	CreateExtraPlatesInventory();
	//Check visibility of commands, if not visible, hide arrows
	if(!SecondaryCommand1.Get()->IsVisible())
	{
		if(SecondaryStatueSwitcher.Get()->GetChildrenCount() < 6)
		{
			NoCommandsTextBlock.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			EmptySecondaryCommand.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			LeftSecondaryStatueButton.Get()->SetVisibility(ESlateVisibility::Hidden);
			RightSecondaryStatueButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	SearchFirstVisibleSecondaryStatueCommand();
	if(!ExtraPlateCommand1.Get()->IsVisible())
	{
		NoExtraPlatesTextBlock.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		EmptyExtraPlateCommand.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		LeftExtraPlateButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		RightExtraPlateButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	//Establish widget index of the left and right extra plates switcher
	SearchVisibleExtraPlates();
	//If is in QuestBoard
	if(IsInQuestBoard)
	{
		RecipeBookImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		RecipeBookImage.Get()->SetVisibility(ESlateVisibility::Hidden);
		//Able to hover on buttons
		SetInventoryEnabled();
	}
}

void UInventory::NativeDestruct()
{
	Super::NativeDestruct();
	CleanSecondaryStatueSwitcher();
	CurrentStatuePlates.Empty();
	Recipes.Empty();
	ImagesOfRecipe.Empty();
	MainCommands.Empty();
	SecondaryCommands.Empty();
	LeftSecondaryCommands.Empty();
	RightSecondaryCommands.Empty();
	ExtraPlates.Empty();
	LeftExtraPlates.Empty();
	RightExtraPlates.Empty();
	StatuePlates.Empty();
	LeftSecondaryStatueButton.Get()->OnClicked().RemoveAll(this);
	RightSecondaryStatueButton.Get()->OnClicked().RemoveAll(this);
	LeftExtraPlateButton.Get()->OnClicked().RemoveAll(this);
	LeftExtraPlateButton.Get()->OnClicked().RemoveAll(this);
}

UWidget* UInventory::NativeGetDesiredFocusTarget() const
{
	Super::NativeGetDesiredFocusTarget();

	if(SecondaryCommand1.Get()->IsVisible())
	{
		return LeftSecondaryStatueButton;
	}
	if(SecondaryStatueSwitcher.Get()->GetChildrenCount() >= 6)
	{
		return LeftSecondaryStatueButton;
	}
	if(ExtraPlateCommand1.Get()->IsVisible())
	{
		return LeftExtraPlateButton;
	}
	return nullptr;
}

FSlateBrush UInventory::GetRecipeImageFromTag(FGameplayTag ImageTag)
{
	for( FCommandImage* recipeImage : ImagesOfRecipe)
	{
		if(recipeImage->Tag == ImageTag)
		{
			return recipeImage->ImageOfTag;
		}
	}
	return FSlateBrush();
}

FStatuePlates* UInventory::GetStatuePlateInfo(int StatuePlateID)
{
	for (FStatuePlates* statuePlate: StatuePlates)
	{
		if(statuePlate->PlateID == StatuePlateID)
		{
			return statuePlate;
		}
	}
	return nullptr;
}

void UInventory::CreateStatuePlatesInventory()
{
	//First Show the statue plates
	for (TTuple<int, bool> plateID : TTGameInstance.Get()->StatuesPlatesInInventory)
	{
		TObjectPtr<UInventorySlot> leftNewStatueSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotWidget);
		leftNewStatueSlot.Get()->SetIsInteractionEnabled(false);
		leftNewStatueSlot.Get()->AddToViewport(0);
		TObjectPtr<UInventorySlot> middleNewStatueSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotWidget);
		middleNewStatueSlot.Get()->SetIsInteractionEnabled(false);
		middleNewStatueSlot.Get()->AddToViewport(0);
		CurrentStatuePlates.Add(middleNewStatueSlot);
		TObjectPtr<UInventorySlot> rightNewStatueSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotWidget);
		rightNewStatueSlot.Get()->SetIsInteractionEnabled(false);
		rightNewStatueSlot.Get()->AddToViewport(0);
		SecondaryStatueSwitcher.Get()->AddChild(middleNewStatueSlot);
		LeftSecondaryStatueSwitcher.Get()->AddChild(leftNewStatueSlot);
		RightSecondaryStatueSwitcher.Get()->AddChild(rightNewStatueSlot);
		//Check if it is completed
		if(!plateID.Value)
		{
			//Statue extra plate not completed
			middleNewStatueSlot.Get()->SetVisibility(ESlateVisibility::Visible);
			leftNewStatueSlot.Get()->SetVisibility(ESlateVisibility::Visible);
			rightNewStatueSlot.Get()->SetVisibility(ESlateVisibility::Visible);
			FStatuePlates* statuePlate = GetStatuePlateInfo(plateID.Key);
			if(statuePlate)
			{
				middleNewStatueSlot.Get()->MakeStatuePlate(statuePlate->PlateTitle, statuePlate->PlateImportantText, statuePlate->StatuePlateStyle,statuePlate->StatuePlateTextStyle);
				leftNewStatueSlot.Get()->MakeStatuePlate(statuePlate->PlateTitle, statuePlate->PlateImportantText, statuePlate->StatuePlateStyle,statuePlate->StatuePlateTextStyle);
				rightNewStatueSlot.Get()->MakeStatuePlate(statuePlate->PlateTitle, statuePlate->PlateImportantText, statuePlate->StatuePlateStyle,statuePlate->StatuePlateTextStyle);
			}
			goto continueSearch1;
		}
		else
		{
			//Go through all recipes
			for (FRecipes_Struct* recipe : Recipes)
			{
				if(recipe->ID == plateID.Key)
				{
					//Middle
					middleNewStatueSlot.Get()->SetVisibility(ESlateVisibility::Visible);
					middleNewStatueSlot.Get()->SetInventorySlot(plateID.Key,recipe->RecipeSprite, recipe->Ingredient1_Tag.IsValid(),
															recipe->Ingredient2_Tag.IsValid(), recipe->Ingredient3_Tag.IsValid(),
															recipe->EnemyName_Tag, recipe->Cook_Tag,
															recipe->Cut_Tag, recipe->Stars, recipe->Name, TTGameInstance);
					//Left
					leftNewStatueSlot.Get()->SetVisibility(ESlateVisibility::Visible);
					leftNewStatueSlot.Get()->SetInventorySlot(plateID.Key,recipe->RecipeSprite, recipe->Ingredient1_Tag.IsValid(),
															recipe->Ingredient2_Tag.IsValid(), recipe->Ingredient3_Tag.IsValid(),
															recipe->EnemyName_Tag, recipe->Cook_Tag,
															recipe->Cut_Tag, recipe->Stars, recipe->Name, TTGameInstance);
					//Right
					rightNewStatueSlot.Get()->SetVisibility(ESlateVisibility::Visible);
					rightNewStatueSlot.Get()->SetInventorySlot(plateID.Key,recipe->RecipeSprite, recipe->Ingredient1_Tag.IsValid(),
															recipe->Ingredient2_Tag.IsValid(), recipe->Ingredient3_Tag.IsValid(),
															recipe->EnemyName_Tag, recipe->Cook_Tag,
															recipe->Cut_Tag, recipe->Stars, recipe->Name, TTGameInstance);
					if(TTGameInstance.Get()->StatuesPlatesInInventory.Contains(plateID.Key))
					{
						FStatuePlates* statuePlate = GetStatuePlateInfo(plateID.Key);
						middleNewStatueSlot.Get()->StatuePlatePickedUp(statuePlate->StatuePlateStyle, statuePlate->StatuePlateTextStyle, statuePlate->PlateTitle);
						leftNewStatueSlot.Get()->StatuePlatePickedUp(statuePlate->StatuePlateStyle, statuePlate->StatuePlateTextStyle, statuePlate->PlateTitle);
						rightNewStatueSlot.Get()->StatuePlatePickedUp(statuePlate->StatuePlateStyle, statuePlate->StatuePlateTextStyle, statuePlate->PlateTitle);
					}
					goto continueSearch1;
				}
			}
		}
		continueSearch1:
			continue;
	}
}

void UInventory::CreateExtraPlatesInventory()
{
	//Extra Plates
	for (int plateID : TTGameInstance.Get()->ExtraPlatesInventory)
	{
		//Check the first inventory slot available
		for(int i = 0; i < ExtraPlates.Num(); i++)
		{
			if(ExtraPlates[i].Get()->GetVisibility() == ESlateVisibility::Collapsed)
			{
				//Go through all recipes and check if obtained during Gavern Day
				for (FRecipes_Struct* recipe : Recipes)
				{
					if(recipe->ID == plateID)
					{
						//Middle
						ExtraPlates[i].Get()->SetVisibility(ESlateVisibility::Visible);
						ExtraPlates[i].Get()->SetInventorySlot(plateID,recipe->RecipeSprite, recipe->Ingredient1_Tag.IsValid(),
																	recipe->Ingredient2_Tag.IsValid(), recipe->Ingredient3_Tag.IsValid(),
																	recipe->EnemyName_Tag, recipe->Cook_Tag,
																	recipe->Cut_Tag, recipe->Stars, recipe->Name, TTGameInstance, TTGameInstance.Get()->ExtraPlatesDuringGavernDay.Contains(plateID));
						//Left
						LeftExtraPlates[i].Get()->SetVisibility(ESlateVisibility::Visible);
						LeftExtraPlates[i].Get()->SetInventorySlot(plateID,recipe->RecipeSprite, recipe->Ingredient1_Tag.IsValid(),
																	recipe->Ingredient2_Tag.IsValid(), recipe->Ingredient3_Tag.IsValid(),
																	recipe->EnemyName_Tag, recipe->Cook_Tag,
																	recipe->Cut_Tag, recipe->Stars, recipe->Name, TTGameInstance, TTGameInstance.Get()->ExtraPlatesDuringGavernDay.Contains(plateID));
						//Right
						RightExtraPlates[i].Get()->SetVisibility(ESlateVisibility::Visible);
						RightExtraPlates[i].Get()->SetInventorySlot(plateID,recipe->RecipeSprite, recipe->Ingredient1_Tag.IsValid(),
																	recipe->Ingredient2_Tag.IsValid(), recipe->Ingredient3_Tag.IsValid(),
																	recipe->EnemyName_Tag, recipe->Cook_Tag,
																	recipe->Cut_Tag, recipe->Stars, recipe->Name, TTGameInstance, TTGameInstance.Get()->ExtraPlatesDuringGavernDay.Contains(plateID));
						goto continueSearch2;
					}
				}
			}
		}
		continueSearch2:
			continue;
	}
}

void UInventory::ShowMainCommands()
{
	int numberOfMainCommand = 0;
	for(TTuple<int,bool> mainCommand : TTGameInstance.Get()->MainOrders)
	{
		if(MainCommands[numberOfMainCommand].Get()->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MainCommands[numberOfMainCommand].Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			MainCommands[numberOfMainCommand].Get()->SetCommand(mainCommand.Key);
			MainCommands[numberOfMainCommand].Get()->MakeMainCommand();
			if(mainCommand.Value)
			{
				MainCommands[numberOfMainCommand].Get()->SetIsCompleted(true);
			}
			++numberOfMainCommand;
		}
	}
}

void UInventory::ShowSecondaryCommands()
{
	//Secondary commands finished
	int numberOfFinishedSecondaryCommand = 0;
	for(FSecondaryCommand secondaryFinishedCommand : TTGameInstance.Get()->FinishedOrders)
	{
		//Middle
		SecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SecondaryCommands[numberOfFinishedSecondaryCommand].Get()->ExecuteAppearAnimation(true);
		SecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetCommand(secondaryFinishedCommand.PlateID);
		SecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetNPCImage(GetRecipeImageFromTag(secondaryFinishedCommand.NPCTag), secondaryFinishedCommand.NPCTag);
					SecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetIsCompleted(true);
		SecondaryCommands[numberOfFinishedSecondaryCommand].Get()->LightCommand();
		//Left
		LeftSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		LeftSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->ExecuteAppearAnimation(true);
		LeftSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetCommand(secondaryFinishedCommand.PlateID);
		LeftSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetNPCImage(GetRecipeImageFromTag(secondaryFinishedCommand.NPCTag), secondaryFinishedCommand.NPCTag);
		LeftSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetIsCompleted(true);
		LeftSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->LightCommand();
		//Right
		RightSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RightSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->ExecuteAppearAnimation(true);
		RightSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetCommand(secondaryFinishedCommand.PlateID);
		RightSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetNPCImage(GetRecipeImageFromTag(secondaryFinishedCommand.NPCTag), secondaryFinishedCommand.NPCTag);
		RightSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->SetIsCompleted(true);
		RightSecondaryCommands[numberOfFinishedSecondaryCommand].Get()->LightCommand();
		
		++numberOfFinishedSecondaryCommand;
	}
	//Secondary commands active
	int numberOfSecondaryCommand = numberOfFinishedSecondaryCommand;
	for(FSecondaryCommand secondaryCommand : TTGameInstance.Get()->AcceptedOrders)
	{
		//Middle
		SecondaryCommands[numberOfSecondaryCommand].Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SecondaryCommands[numberOfSecondaryCommand].Get()->ExecuteAppearAnimation(true);
		SecondaryCommands[numberOfSecondaryCommand].Get()->SetCommand(secondaryCommand.PlateID);
		SecondaryCommands[numberOfSecondaryCommand].Get()->SetNPCImage(GetRecipeImageFromTag(secondaryCommand.NPCTag), secondaryCommand.NPCTag);
		SecondaryCommands[numberOfSecondaryCommand].Get()->LightCommand();
		//Left
		LeftSecondaryCommands[numberOfSecondaryCommand].Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		LeftSecondaryCommands[numberOfSecondaryCommand].Get()->ExecuteAppearAnimation(true);
		LeftSecondaryCommands[numberOfSecondaryCommand].Get()->SetCommand(secondaryCommand.PlateID);
		LeftSecondaryCommands[numberOfSecondaryCommand].Get()->SetNPCImage(GetRecipeImageFromTag(secondaryCommand.NPCTag), secondaryCommand.NPCTag);
		LeftSecondaryCommands[numberOfSecondaryCommand].Get()->LightCommand();
		//Right
		RightSecondaryCommands[numberOfSecondaryCommand].Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RightSecondaryCommands[numberOfSecondaryCommand].Get()->ExecuteAppearAnimation(true);
		RightSecondaryCommands[numberOfSecondaryCommand].Get()->SetCommand(secondaryCommand.PlateID);
		RightSecondaryCommands[numberOfSecondaryCommand].Get()->SetNPCImage(GetRecipeImageFromTag(secondaryCommand.NPCTag), secondaryCommand.NPCTag);
		RightSecondaryCommands[numberOfSecondaryCommand].Get()->LightCommand();
		
		++numberOfSecondaryCommand;
	}
}

void UInventory::HideNoSecondaryCommands()
{
	EmptySecondaryCommand.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UInventory::SetInventoryEnabled()
{
	for(TObjectPtr<UInventorySlot> inventorySlot : ExtraPlates)
	{
		inventorySlot.Get()->EnableInventorySlot();
	}
}

TArray<TObjectPtr<UInventorySlot>> UInventory::GetInventorySlots()
{
	return ExtraPlates;
}

void UInventory::MoveSecondaryStatueToTheLeft()
{
	int newLeftWidgetIndex = LeftSecondaryStatueSwitcher.Get()->GetActiveWidgetIndex() - 1;
	int newMiddleWidgetIndex = SecondaryStatueSwitcher.Get()->GetActiveWidgetIndex() - 1;
	int newRightWidgetIndex = RightSecondaryStatueSwitcher.Get()->GetActiveWidgetIndex() - 1;
	MoveSwitcherSecondaryStatueWidgetIndex(newLeftWidgetIndex, LeftSecondaryStatueSwitcher, false);
	MoveSwitcherSecondaryStatueWidgetIndex(newMiddleWidgetIndex, SecondaryStatueSwitcher, false);
	MoveSwitcherSecondaryStatueWidgetIndex(newRightWidgetIndex, RightSecondaryStatueSwitcher, false);
}

void UInventory::MoveSecondaryStatueToTheRight()
{
	int newLeftWidgetIndex = LeftSecondaryStatueSwitcher.Get()->GetActiveWidgetIndex() + 1;
	int newMiddleWidgetIndex = SecondaryStatueSwitcher.Get()->GetActiveWidgetIndex() + 1;
	int newRightWidgetIndex = RightSecondaryStatueSwitcher.Get()->GetActiveWidgetIndex() + 1;
	MoveSwitcherSecondaryStatueWidgetIndex(newLeftWidgetIndex, LeftSecondaryStatueSwitcher, true);
	MoveSwitcherSecondaryStatueWidgetIndex(newMiddleWidgetIndex, SecondaryStatueSwitcher, true);
	MoveSwitcherSecondaryStatueWidgetIndex(newRightWidgetIndex, RightSecondaryStatueSwitcher, true);
}

void UInventory::MoveExtraPlatesToTheLeft()
{
	int newLeftWidgetIndex = LeftExtraPlatesSwitcher.Get()->GetActiveWidgetIndex() - 1;
	int newMiddleWidgetIndex = ExtraPlatesSwitcher.Get()->GetActiveWidgetIndex() - 1;
	int newRightWidgetIndex = RightExtraPlatesSwitcher.Get()->GetActiveWidgetIndex() - 1;
	MoveSwitcherWidgetIndex(newLeftWidgetIndex, LeftExtraPlatesSwitcher, NumberOfVisibleExtraPlates);
	MoveSwitcherWidgetIndex(newMiddleWidgetIndex, ExtraPlatesSwitcher, NumberOfVisibleExtraPlates);
	MoveSwitcherWidgetIndex(newRightWidgetIndex, RightExtraPlatesSwitcher, NumberOfVisibleExtraPlates);
}

void UInventory::MoveExtraPlatesToTheRight()
{
	int newLeftWidgetIndex = LeftExtraPlatesSwitcher.Get()->GetActiveWidgetIndex() + 1;
	int newMiddleWidgetIndex = ExtraPlatesSwitcher.Get()->GetActiveWidgetIndex() + 1;
	int newRightWidgetIndex = RightExtraPlatesSwitcher.Get()->GetActiveWidgetIndex() + 1;
	MoveSwitcherWidgetIndex(newLeftWidgetIndex, LeftExtraPlatesSwitcher, NumberOfVisibleExtraPlates);
	MoveSwitcherWidgetIndex(newMiddleWidgetIndex, ExtraPlatesSwitcher, NumberOfVisibleExtraPlates);
	MoveSwitcherWidgetIndex(newRightWidgetIndex, RightExtraPlatesSwitcher, NumberOfVisibleExtraPlates);
}

void UInventory::MoveSwitcherWidgetIndex(int Index, TObjectPtr<UCommonAnimatedSwitcher> Switcher,
	int LastVisibleWidgetIndex)
{
	PlaySound(NextCommandSound);
	if(Index == -1)
	{
		Switcher.Get()->SetActiveWidgetIndex(LastVisibleWidgetIndex - 1);
	}
	else if(Index == LastVisibleWidgetIndex)
	{
		Switcher.Get()->SetActiveWidgetIndex(0);
	}
	else
	{
		Switcher.Get()->SetActiveWidgetIndex(Index);
	}
}

void UInventory::MoveSwitcherSecondaryStatueWidgetIndex(int Index, TObjectPtr<UCommonAnimatedSwitcher> Switcher,
	bool MovingRight)
{
	PlaySound(NextCommandSound);
	if(Index == -1)
	{
		for (int i = Switcher.Get()->GetChildrenCount() - 1; i > 0; i--)
		{
			if(Switcher.Get()->GetChildAt(i)->IsVisible())
			{
				Switcher.Get()->SetActiveWidgetIndex(i);
				break;
			}
		}
	}
	else if(MovingRight)
	{
		//Moving Right
		bool foundWidget = false;
		for (int i = Index; i < Switcher.Get()->GetChildrenCount(); i++)
		{
			if(Switcher.Get()->GetChildAt(i)->IsVisible())
			{
				foundWidget = true;
				Switcher.Get()->SetActiveWidgetIndex(i);
				break;
			}
		}
		if(!foundWidget)
		{
			if(Switcher.Get()->GetChildAt(0)->IsVisible())
			{
				//If secondary command visible
				Switcher.Get()->SetActiveWidgetIndex(0);
			}
			else
			{
				//Else statue
				Switcher.Get()->SetActiveWidgetIndex(5);
			}
		}
	}
	else
	{
		//Moving left
		bool foundWidget = false;
		for (int i = Index; i > -1; i--)
		{
			if(Switcher.Get()->GetChildAt(i)->IsVisible())
			{
				foundWidget = true;
				Switcher.Get()->SetActiveWidgetIndex(i);
				break;
			}
		}
		if(!foundWidget)
		{
			for (int i = Switcher.Get()->GetChildrenCount() - 1; i > -1; i--)
			{
				if(Switcher.Get()->GetChildAt(i)->IsVisible())
				{
					Switcher.Get()->SetActiveWidgetIndex(i);
					break;
				}
			}
		}
	}
}

void UInventory::CleanSecondaryStatueSwitcher()
{
	if(SecondaryStatueSwitcher->GetChildrenCount() >= 5)
	{
		for (int i = SecondaryStatueSwitcher->GetChildrenCount() - 1; i > 4; i--)
		{
			SecondaryStatueSwitcher.Get()->GetChildAt(i)->RemoveFromParent();
			LeftSecondaryStatueSwitcher.Get()->GetChildAt(i)->RemoveFromParent();
			RightSecondaryStatueSwitcher.Get()->GetChildAt(i)->RemoveFromParent();
		}
	}
}

void UInventory::SearchFirstVisibleSecondaryStatueCommand()
{
	int element = 0;
	//First check secondary commands
	for (int i = 0; i < 5; i++)
	{
		if(!SecondaryStatueSwitcher->GetChildAt(i)->IsVisible())
		{
			break;
		}
		++element;
	}
	//Then statues
	if(SecondaryStatueSwitcher->GetChildrenCount() >= 6)
	{
		for (int i = 5; i < SecondaryStatueSwitcher->GetChildrenCount(); i++)
		{
			if(!SecondaryStatueSwitcher->GetChildAt(i)->IsVisible())
			{
				break;
			}
			++element;
		}
	}
	int firstElementVisible = 0;
	//If no secondary commands, but have a statue plate
	if(!SecondaryStatueSwitcher->GetChildAt(0)->IsVisible() && SecondaryStatueSwitcher->GetChildrenCount() >= 6)
	{
		firstElementVisible = 5;
	}
	SecondaryStatueSwitcher.Get()->SetActiveWidgetIndex(firstElementVisible);
	if(element == 1)
	{
		LeftSecondaryStatueSwitcher.Get()->SetVisibility(ESlateVisibility::Hidden);
		RightSecondaryStatueSwitcher.Get()->SetVisibility(ESlateVisibility::Hidden);
		LeftSecondaryStatueButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		RightSecondaryStatueButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	else if(element == 2)
	{
		LeftSecondaryStatueSwitcher.Get()->SetVisibility(ESlateVisibility::Hidden);
		int rightElement = firstElementVisible + 1;
		MoveSwitcherSecondaryStatueWidgetIndex(rightElement, RightSecondaryStatueSwitcher, true);
	}
	else if(element > 1)
	{
		int leftElement = firstElementVisible - 1;
		int rightElement = firstElementVisible + 1;
		MoveSwitcherSecondaryStatueWidgetIndex(leftElement, LeftSecondaryStatueSwitcher, false);
		MoveSwitcherSecondaryStatueWidgetIndex(rightElement, RightSecondaryStatueSwitcher, true);
	}
}

void UInventory::SearchVisibleExtraPlates()
{
	//Get All Visible extra plates
	for (TObjectPtr<UWidget> widget : ExtraPlatesSwitcher.Get()->GetAllChildren())
	{
		if(!widget.Get()->IsVisible())
		{
			break;
		}
		++NumberOfVisibleExtraPlates;
	}
	//Get number of visible extra plates
	ExtraPlatesSwitcher.Get()->SetActiveWidgetIndex(0);
	//If only one extra plate, hide the others switchers
	if(NumberOfVisibleExtraPlates == 1)
	{
		LeftExtraPlatesSwitcher.Get()->SetVisibility(ESlateVisibility::Hidden);
		RightExtraPlatesSwitcher.Get()->SetVisibility(ESlateVisibility::Hidden);
		LeftExtraPlateButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		RightExtraPlateButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	else if(NumberOfVisibleExtraPlates == 2)
	{
		RightExtraPlatesSwitcher.Get()->SetActiveWidgetIndex(1);
		LeftExtraPlatesSwitcher.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		RightExtraPlatesSwitcher.Get()->SetActiveWidgetIndex(1);
		LeftExtraPlatesSwitcher.Get()->SetActiveWidgetIndex(NumberOfVisibleExtraPlates - 1);
	}
}

void UInventory::InitializeExtraPlates()
{
	ExtraPlateCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	ExtraPlateCommand1.Get()->SetIsInteractionEnabled(false);
	ExtraPlates.Add(ExtraPlateCommand1);
	ExtraPlateCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	ExtraPlateCommand2.Get()->SetIsInteractionEnabled(false);
	ExtraPlates.Add(ExtraPlateCommand2);
	ExtraPlateCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	ExtraPlateCommand3.Get()->SetIsInteractionEnabled(false);
	ExtraPlates.Add(ExtraPlateCommand3);
	ExtraPlateCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	ExtraPlateCommand4.Get()->SetIsInteractionEnabled(false);
	ExtraPlates.Add(ExtraPlateCommand4);
	ExtraPlateCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	ExtraPlateCommand5.Get()->SetIsInteractionEnabled(false);
	ExtraPlates.Add(ExtraPlateCommand5);
	
	LeftExtraPlateCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftExtraPlateCommand1.Get()->SetIsInteractionEnabled(false);
	LeftExtraPlates.Add(LeftExtraPlateCommand1);
	LeftExtraPlateCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftExtraPlateCommand1.Get()->SetIsInteractionEnabled(false);
	LeftExtraPlates.Add(LeftExtraPlateCommand2);
	LeftExtraPlateCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftExtraPlateCommand3.Get()->SetIsInteractionEnabled(false);
	LeftExtraPlates.Add(LeftExtraPlateCommand3);
	LeftExtraPlateCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftExtraPlateCommand4.Get()->SetIsInteractionEnabled(false);
	LeftExtraPlates.Add(LeftExtraPlateCommand4);
	LeftExtraPlateCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftExtraPlateCommand5.Get()->SetIsInteractionEnabled(false);
	LeftExtraPlates.Add(LeftExtraPlateCommand5);

	RightExtraPlateCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightExtraPlateCommand1.Get()->SetIsInteractionEnabled(false);
	RightExtraPlates.Add(RightExtraPlateCommand1);
	RightExtraPlateCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightExtraPlateCommand2.Get()->SetIsInteractionEnabled(false);
	RightExtraPlates.Add(RightExtraPlateCommand2);
	RightExtraPlateCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightExtraPlateCommand3.Get()->SetIsInteractionEnabled(false);
	RightExtraPlates.Add(RightExtraPlateCommand3);
	RightExtraPlateCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightExtraPlateCommand4.Get()->SetIsInteractionEnabled(false);
	RightExtraPlates.Add(RightExtraPlateCommand4);
	RightExtraPlateCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightExtraPlateCommand5.Get()->SetIsInteractionEnabled(false);
	RightExtraPlates.Add(RightExtraPlateCommand5);
}

void UInventory::InitializeSecondaryPlates()
{
	//Secondary Commands
	SecondaryCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	SecondaryCommand1.Get()->SetIsInteractionEnabled(false);
	SecondaryCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	SecondaryCommand2.Get()->SetIsInteractionEnabled(false);
	SecondaryCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	SecondaryCommand3.Get()->SetIsInteractionEnabled(false);
	SecondaryCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	SecondaryCommand4.Get()->SetIsInteractionEnabled(false);
	SecondaryCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	SecondaryCommand5.Get()->SetIsInteractionEnabled(false);

	//Left Secondary Commands
	LeftSecondaryCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftSecondaryCommand1.Get()->SetIsInteractionEnabled(false);
	LeftSecondaryCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftSecondaryCommand2.Get()->SetIsInteractionEnabled(false);
	LeftSecondaryCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftSecondaryCommand3.Get()->SetIsInteractionEnabled(false);
	LeftSecondaryCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftSecondaryCommand4.Get()->SetIsInteractionEnabled(false);
	LeftSecondaryCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	LeftSecondaryCommand5.Get()->SetIsInteractionEnabled(false);

	//Middle Secondary Commands
	RightSecondaryCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightSecondaryCommand1.Get()->SetIsInteractionEnabled(false);
	RightSecondaryCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightSecondaryCommand2.Get()->SetIsInteractionEnabled(false);
	RightSecondaryCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightSecondaryCommand3.Get()->SetIsInteractionEnabled(false);
	RightSecondaryCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightSecondaryCommand4.Get()->SetIsInteractionEnabled(false);
	RightSecondaryCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	RightSecondaryCommand5.Get()->SetIsInteractionEnabled(false);
}
