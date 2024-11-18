// Fill out your copyright notice in the Description page of Project Settings.


#include "TavernMenu.h"
#include "../Buttons/ButtonBase.h"
#include "Tavern.h"
#include "UpgradeAnimationWidget.h"
#include "../../../Levels/TavernLevel.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Input/CommonUIInputTypes.h"
#include "QuestBoard/CommandUI.h"
#include "TinyTavernGame/TT_GameplayTags.h"

#include "TinyTavernGame/Actor/TTSpriteActor.h"
#include "TinyTavernGame/Player/UIPlayer/UIPlayer.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UTavernMenu::NativeConstruct()
{
	Super::NativeConstruct();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	//Get Tavern level
	TavernLevel = Cast<ATavernLevel>(GetWorld()->GetLevelScriptActor());
	//Link button functions
	OpenQuestDelegate = QuestBoardButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::OpenQuestBoard);
	QuestBoardButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverQuestBoard);
	QuestBoardButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverQuestBoard);
	HeadToAdventureDelegate = AdventureButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::HeadToAdventure);
	AdventureButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverDoor);
	AdventureButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverDoor);
	RecipeBookButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::OpenRecipeBookWidget);
	RecipeBookButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverRecipeBook);
	RecipeBookButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverRecipeBook);
	EasterEggButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	
	TTGameInstance.Get()->LevelDataTable->GetAllRows("", LevelRows);
	if(TTGameInstance.Get()->CurrentLevel > -1)
	{
		EstablishNavigation();
		if(TTGameInstance.Get()->FirstTimeInLevel3 || TTGameInstance.Get()->FirstTimeInLevel4 || TTGameInstance.Get()->FirstTimeInLevel5)
		{
			RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Hidden);
			QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Hidden);
			AdventureButton.Get()->SetVisibility(ESlateVisibility::Hidden);
			NPCGoctorButton.Get()->SetVisibility(ESlateVisibility::Hidden);
			NPCGoctorButton.Get()->SetIsEnabled(false);
			NPCGailorButton.Get()->SetIsEnabled(false);
			NPCGmithButton.Get()->SetIsEnabled(false);
			NPCGavernButton.Get()->SetIsEnabled(false);
			//Show Upgrades, then after animation, show main commands
			ShowUpgrades();
			TTGameInstance.Get()->FirstTimeInLevel3 = false;
			TTGameInstance.Get()->FirstTimeInLevel4 = false;
			TTGameInstance.Get()->FirstTimeInLevel5 = false;
		}
		else
		{
			TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::Hidden);
			RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Visible);
			AdventureButton.Get()->SetVisibility(ESlateVisibility::Visible);
			if(TTGameInstance.Get()->CurrentLevel == 1)
			{
				QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Hidden);
				GamepadButton = AdventureButton;
			}
			else
			{
				QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Visible);
				//GamepadButton equals the last tavern button clicked
				switch (TavernLevel.Get()->GetCurrentTavernButton()) {
				case RECIPEBOOK:
						GamepadButton = RecipeBookButton;
					break;
				case QUESTBOARD:
						GamepadButton = QuestBoardButton;
					break;
				case TAVERNDOOR:
						GamepadButton = AdventureButton;
					break;
				case GAVERN:
						GamepadButton = NPCGavernButton;
					break;
				case GMITH:
						GamepadButton = NPCGmithButton;
					break;
				case GOCTOR:
						GamepadButton = NPCGoctorButton;
					break;
				case GAILOR:
						GamepadButton = NPCGailorButton;
					break;
				}
			}
		}
	}
}

void UTavernMenu::NativeDestruct()
{
	Super::NativeDestruct();

	QuestBoardButton.Get()->OnClicked().Remove(OpenQuestDelegate);
	QuestBoardButton.Get()->OnHovered().RemoveAll(this);
	QuestBoardButton.Get()->OnUnhovered().RemoveAll(this);
	AdventureButton.Get()->OnClicked().Remove(HeadToAdventureDelegate);
	AdventureButton.Get()->OnHovered().RemoveAll(this);
	AdventureButton.Get()->OnUnhovered().RemoveAll(this);
	NPCGoctorButton.Get()->OnClicked().RemoveAll(this);
	NPCGoctorButton.Get()->OnHovered().RemoveAll(this);
	NPCGoctorButton.Get()->OnUnhovered().RemoveAll(this);
	NPCGmithButton.Get()->OnClicked().RemoveAll(this);
	NPCGmithButton.Get()->OnHovered().RemoveAll(this);
	NPCGmithButton.Get()->OnUnhovered().RemoveAll(this);
	NPCGailorButton.Get()->OnClicked().RemoveAll(this);
	NPCGailorButton.Get()->OnHovered().RemoveAll(this);
	NPCGailorButton.Get()->OnUnhovered().RemoveAll(this);
	NPCGavernButton.Get()->OnClicked().RemoveAll(this);
	NPCGavernButton.Get()->OnHovered().RemoveAll(this);
	NPCGavernButton.Get()->OnUnhovered().RemoveAll(this);
	RecipeBookButton.Get()->OnClicked().RemoveAll(this);
	RecipeBookButton.Get()->OnHovered().RemoveAll(this);
	RecipeBookButton.Get()->OnUnhovered().RemoveAll(this);
	EasterEggButton.Get()->OnClicked().RemoveAll(this);
	EasterEggButton.Get()->OnHovered().RemoveAll(this);
	EasterEggButton.Get()->OnUnhovered().RemoveAll(this);
	LevelRows.Empty();
	ContinueHandle.Unregister();
}

UWidget* UTavernMenu::NativeGetDesiredFocusTarget() const
{
	if(GamepadButton)
	{
		Super::NativeGetDesiredFocusTarget();
		return GamepadButton;
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UTavernMenu::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	//Continue with click if tutorial
	if(TutorialNumber != 1 && TutorialNumber != 11 && TutorialNumber != 14 && TutorialNumber != 98 && TutorialNumber != 99 && TutorialNumber != 100)
	{
		NextButtonBox.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NextButtonImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UTavernMenu::AdvanceInTutorial)));
	}
	else if(Animation == GookerTalksAnimation)
	{
		GookerShowsUp();
	}
	else if(TutorialNumber == 99)
	{
		//Main Commands
		NextButtonBox.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NextButtonImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UTavernMenu::ContinueWithTavernAfterMainCommands)));
	}
	else if(TutorialNumber == 100)
	{
		//Hide Commands
		ReturnVisibilityOfButtonsAfterMainCommands();
	}
	else if(TutorialNumber == 1 && !TalkedToGavernInTuto)
	{
		ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UTavernMenu::HeadToGavernTutorial)));
	}
	else if(TutorialNumber != 98 && TutorialNumber != 11)
	{
		NextButtonBox.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NextButtonImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UTavernMenu::TavernShowGooker)));
	}
}

FReply UTavernMenu::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(ContinueHandle.IsValid())
	{
		if(TutorialNumber != 1 && TutorialNumber != 98 && TutorialNumber != 99 && TutorialNumber != 100)
		{
			AdvanceInTutorial();
		}
		else if(TutorialNumber == 98)
		{
			EndShowUpgrades();
		}
		else if(TutorialNumber == 99)
		{
			//Main Commands
			ContinueWithTavernAfterMainCommands();
		}
		else if(TutorialNumber == 100)
		{
			//Hide Commands
			ReturnVisibilityOfButtonsAfterMainCommands();
		}
		else if(TutorialNumber == 1)
		{
			HeadToGavernTutorial();
		}
		else
		{
			TavernShowGooker();
		}
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UTavernMenu::OpenQuestBoard()
{
	QuestBoardSprite.Get()->ShowOutline(true);
	TavernLevel.Get()->QuestBoardAnimation(true);
}

void UTavernMenu::HeadToAdventure()
{
	if(TTGameInstance.Get()->CurrentLevel > 1)
	{
		if(!TTGameInstance.Get()->FinishedOrders.IsEmpty())
		{
			TObjectPtr<UConfirmMenu> confirmMenu = Cast<UConfirmMenu>(TavernMenuStack.Get()->AddWidget(TavernConfirmMenuWidget));
			confirmMenu->AddConfirmMenu(TitleConfirmMenuOrderNotGiven, DescriptionConfirmMenuOrderNotGiven, ConfirmMenuOptionOrderNotGiven);
			return;
		}
		if(TTGameInstance.Get()->AcceptedOrders.IsEmpty())
		{
			TObjectPtr<UConfirmMenu> confirmMenu = Cast<UConfirmMenu>(TavernMenuStack.Get()->AddWidget(TavernConfirmMenuWidget));
			confirmMenu->AddConfirmMenu(TitleConfirmMenuNoAcceptedOrders, DescriptionConfirmMenuNoAcceptedOrders, ConfirmMenuOptionNoAcceptedOrders);
			return;
		}
	}
	//Call Door Animation with door opened
	AdventureButton.Get()->OnUnhovered().RemoveAll(this);
	DoorSprite.Get()->ChangeToHoveredSprite(true);
	TavernLevel.Get()->SetAdventureDoorLight(true);
	TavernLevel.Get()->GoToAdventure();
}

void UTavernMenu::OpenGmithWidget()
{
	TavernLevel.Get()->NPCGmithAnimation(true);
	GmithSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OpenGoctorWidget()
{
	TavernLevel.Get()->NPCGoctorAnimation(true);
	GoctorSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OpenGailorWidget()
{
	TavernLevel.Get()->NPCGailorAnimation(true);
	TavernLevel.Get()->GailorSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OpenGavernWidget()
{
	TavernLevel.Get()->GavernSprite.Get()->ShowOutline(true);
	TavernLevel.Get()->NPCGavernAnimation(true);
}

void UTavernMenu::OpenRecipeBookWidget()
{
	TObjectPtr<AUIPlayer> UIPlayer = Cast<AUIPlayer>(GetOwningPlayerPawn());
	UIPlayer.Get()->OpenSettingsMenu();
}

void UTavernMenu::ShowEasterEggWidget()
{
	TavernLevel.Get()->EasterEggShowAnimation(true);
	//Obtain The Team Achievement
	TTGameInstance.Get()->TheTeam();
}

void UTavernMenu::SetTavernSpriteActors(TObjectPtr<ATTSpriteActor> _DoorSprite,
                                        TObjectPtr<ATTSpriteActor> _QuestBoardSprite, TObjectPtr<ATTSpriteActor> _GavernSprite,
                                        TObjectPtr<ATTSpriteActor> _GoctorSprite, TObjectPtr<ATTSpriteActor> _GailorSprite,
                                        TObjectPtr<ATTSpriteActor> _GmithSprite, TObjectPtr<ATTSpriteActor> _GookerSprite,
                                        TObjectPtr<ATTSpriteActor> _RecipeBookSprite)
{
	DoorSprite = _DoorSprite;
	QuestBoardSprite = _QuestBoardSprite;
	GavernSprite = _GavernSprite;
	GoctorSprite = _GoctorSprite;
	GailorSprite = _GailorSprite;
	GmithSprite = _GmithSprite;
	GookerSprite = _GookerSprite;
	RecipeBookSprite = _RecipeBookSprite;
	//Visibility of NPCs
	for (FLevelRow* levelRow : LevelRows)
	{
		for(FGameplayTag npc: levelRow->NPCS)
		{
			if(npc == FGameplayTags::Get().NPC_Gavern)
			{
				NPCGavernButton.Get()->SetVisibility(ESlateVisibility::Visible);
				NPCGavernButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::OpenGavernWidget);
				NPCGavernButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverGavern);
				NPCGavernButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverGavern);
			}
			else if(npc == FGameplayTags::Get().NPC_Goctor)
			{
				NPCGoctorButton.Get()->SetVisibility(ESlateVisibility::Visible);
				NPCGoctorButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::OpenGoctorWidget);
				NPCGoctorButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverGoctor);
				NPCGoctorButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverGoctor);
			}
			else if(npc == FGameplayTags::Get().NPC_Gailor)
			{
				NPCGailorButton.Get()->SetVisibility(ESlateVisibility::Visible);
				NPCGailorButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::OpenGailorWidget);
				NPCGailorButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverGailor);
				NPCGailorButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverGailor);
			}
			else if(npc == FGameplayTags::Get().NPC_Gmith)
			{
				NPCGmithButton.Get()->SetVisibility(ESlateVisibility::Visible);
				NPCGmithButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::OpenGmithWidget);
				NPCGmithButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverGmith);
				NPCGmithButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverGmith);
			}
		}
		if(levelRow->Level == TTGameInstance.Get()->CurrentLevel)
		{
			break;
		}
	}
	if(TTGameInstance.Get()->CurrentLevel >= 2)
	{
		CheckForCompletedOrders();
	}
}

void UTavernMenu::Level0()
{
	TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if(TalkedToGavernInTuto)
	{
		TutorialNumber = 0;
	}
	AdvanceInTutorial();
	
	RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	NPCGavernButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	AdventureButton.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UTavernMenu::Level1()
{
	TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TutorialNumber = 4;
	AdvanceInTutorial();
	RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	NPCGavernButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	AdventureButton.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UTavernMenu::Level2()
{
	if(!ShouldShowSecondPartOfLevel2)
	{
		ShouldShowSecondPartOfLevel2 = true;
		//Show Upgrades, then after animation, show main commands
		TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		AdventureButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		NPCGoctorButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		NPCGoctorButton.Get()->SetIsEnabled(false);
		NPCGailorButton.Get()->SetIsEnabled(false);
		NPCGmithButton.Get()->SetIsEnabled(false);
		NPCGavernButton.Get()->SetIsEnabled(false);
		ShowUpgrades();
	}
	else if(TutorialNumber == 99)
	{
		TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TutorialNumber = 11;
		AdvanceInTutorial();
		RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		NPCGavernButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		AdventureButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		NPCGoctorButton.Get()->SetVisibility(ESlateVisibility::Hidden);
		NPCGailorButton.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::Hidden);
		NPCGoctorButton.Get()->SetVisibility(ESlateVisibility::Visible);
		NPCGailorButton.Get()->SetVisibility(ESlateVisibility::Visible);
		NPCGavernButton.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NPCGoctorButton.Get()->SetIsEnabled(true);
		NPCGailorButton.Get()->SetIsEnabled(true);
		NPCGavernButton.Get()->SetIsEnabled(true);
		TTGameInstance.Get()->FirstTimeInLevel2 = false;
	}
}

void UTavernMenu::OnHoverDoor()
{
	DoorSprite.Get()->ChangeToHoveredSprite(true);
	TavernLevel.Get()->SetAdventureDoorLight(true);
}

void UTavernMenu::OnUnhoverDoor()
{
	DoorSprite.Get()->ChangeToHoveredSprite(false);
	TavernLevel.Get()->SetAdventureDoorLight(false);
}

void UTavernMenu::OnHoverQuestBoard()
{
	QuestBoardSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OnUnhoverQuestBoard()
{
	QuestBoardSprite.Get()->ShowOutline(false);
}

void UTavernMenu::OnHoverGavern()
{
	GavernSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OnUnhoverGavern()
{
	GavernSprite.Get()->ShowOutline(false);
}

void UTavernMenu::OnHoverGoctor()
{
	GoctorSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OnUnhoverGoctor()
{
	GoctorSprite.Get()->ShowOutline(false);
}

void UTavernMenu::OnHoverGailor()
{
	GailorSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OnUnhoverGailor()
{
	GailorSprite.Get()->ShowOutline(false);
}

void UTavernMenu::OnHoverGmith()
{
	GmithSprite.Get()->ShowOutline(true);
}

void UTavernMenu::OnUnhoverGmith()
{
	GmithSprite.Get()->ShowOutline(false);
}

void UTavernMenu::OnHoverRecipeBook()
{
	RecipeBookSprite.Get()->ChangeToHoveredSprite(true);
}

void UTavernMenu::OnUnhoverRecipeBook()
{
	RecipeBookSprite.Get()->ChangeToHoveredSprite(false);
}

void UTavernMenu::OnHoverEasterEgg()
{
	TavernLevel.Get()->HoverEasterEgg(true);
}

void UTavernMenu::OnUnhoverEasterEgg()
{
	TavernLevel.Get()->HoverEasterEgg(false);
}

void UTavernMenu::AdvanceInTutorial()
{
	ContinueHandle.Unregister();
	NextButtonBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	NextButtonImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	++TutorialNumber;
	switch (TutorialNumber)
	{
	case 1:
		if(!TalkedToGavernInTuto)
		{
			//Gavern Tutorial
			PlayAnimation(Tutorial1Animation);
		}
		else
		{
			GookerShowsUp();
			TalkedToGavernInTuto = false;
		}
		break;
	//Gavern Talks
	//Gooker Talks
	case 2:
		//Command Info Tutorial
		PlayAnimation(Tutorial2Animation);
		break;
	case 3:
		//Command Tutorial Tutorial
		PlayAnimation(Tutorial2BAnimation);
		break;
	case 4:
		//Player to Tutorial with Cheese Mine
		PlayAnimationReverse(Tutorial2Animation, 2.0f);
		TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::Hidden);
		TavernLevel.Get()->PlayFadeAnimation();
		break;
	case 5:
		//Main Commands
		LoadMainOrders();
		PlayAnimation(MainCommandsAnimation);
		break;
	case 6:
		//Main Commands 2
		PlayAnimation(MainCommandsExplanation1Animation);
		break;
	case 7:
		//Main Commands 3
		PlayAnimation(MainCommandsExplanation2Animation);
		break;
	case 8:
		//Main Commands 
		PlayAnimation(MainCommandsExplanation3Animation);
		break;
	case 9:
		//Recipe Book Tutorial
		PlayAnimation(Tutorial4Animation);
		break;
	case 10:
		//Door Tutorial
		PlayAnimation(Tutorial5Animation);
		break;
	case 11:
		//Player can head to Level
		TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayAnimation(Tutorial5BAnimation);
		//Call Door Animation with door opened
		AdventureButton.Get()->SetVisibility(ESlateVisibility::Visible);
		NPCGavernButton.Get()->SetVisibility(ESlateVisibility::Visible);
		RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Visible);
		GamepadButton = AdventureButton;
		RequestRefreshFocus();
		break;
	case 12:
		//NPC Tutorial
		PlayAnimation(Tutorial6Animation);
		break;
	case 13:
		//Quest Board Tutorial
		PlayAnimation(Tutorial7Animation);
		break;
	case 14:
		TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Visible);
		GamepadButton = QuestBoardButton;
		RequestRefreshFocus();
		ShouldShowSecondPartOfLevel2 = true;
		break;
	}
}

void UTavernMenu::TavernShowGooker()
{
	PlayAnimation(GookerTalksAnimation);
	ContinueHandle.Unregister();
}

void UTavernMenu::GookerShowsUp()
{
	TavernLevel.Get()->SetSecondaryGookerDialog(GookerShowingCommands);
	TavernLevel.Get()->NPCGookerTutorialAnimation(true);
}

void UTavernMenu::ShowUpgrades()
{
	NextButtonBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	NextButtonImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	UnlockedWeapons.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UnlockedWeapons.Get()->OnUpgradeAnimationFinished.AddDynamic(this, &UTavernMenu::EndShowingUpgrade);
	if(TTGameInstance.Get()->CurrentLevel == 2)
	{
		//SMASH
		UnlockedWeapons.Get()->BP_ExecuteSmashAnimation();
	}
	else if(TTGameInstance.Get()->CurrentLevel == 3)
	{
		//BOIl
		UnlockedWeapons.Get()->BP_ExecuteBoilAnimation();
	}
	else if(TTGameInstance.Get()->CurrentLevel == 4)
	{
		//SKEWER
		UnlockedWeapons.Get()->BP_ExecuteSkewerAnimation();
	}
	else if(TTGameInstance.Get()->CurrentLevel == 5)
	{
		//FRY
		UnlockedWeapons.Get()->BP_ExecuteFryAnimation();
	}
	PlayAnimation(ShowUpgradesAnimation);
	TutorialNumber = 98;
}

void UTavernMenu::EndShowUpgrades()
{
	NextButtonBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	NextButtonImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	LoadMainOrders();
	ShowMainOrders();
	ContinueHandle.Unregister();
}

void UTavernMenu::EndShowingUpgrade()
{
	NextButtonBox.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NextButtonImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    ContinueHandle.Unregister();
	ContinueHandle = RegisterUIActionBinding(FBindUIActionArgs(ContinueInputActionData, false, FSimpleDelegate::CreateUObject(this, &UTavernMenu::EndShowUpgrades)));
}

void UTavernMenu::LoadMainOrders()
{
	//Set Invisible First
	MainCommand1.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommand2.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommand3.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommand4.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommand5.Get()->SetVisibility(ESlateVisibility::Collapsed);
	MainCommand6.Get()->SetVisibility(ESlateVisibility::Collapsed);
	//Check the amount of main orders
	for (TTuple<int, bool> mainCommand : TTGameInstance.Get()->MainOrders)
	{
		if(MainCommand1.Get()->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MainCommand1.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			MainCommand1.Get()->SetCommand(mainCommand.Key);
			MainCommand1.Get()->MakeMainCommand();
			continue;
		}
		if(MainCommand2.Get()->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MainCommand2.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			MainCommand2.Get()->SetCommand(mainCommand.Key);
			MainCommand2.Get()->MakeMainCommand();
			continue;
		}
		if(MainCommand3.Get()->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MainCommand3.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			MainCommand3.Get()->SetCommand(mainCommand.Key);
			MainCommand3.Get()->MakeMainCommand();
			continue;
		}
		if(MainCommand4.Get()->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MainCommand4.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			MainCommand4.Get()->SetCommand(mainCommand.Key);
			MainCommand4.Get()->MakeMainCommand();
			continue;
		}
		if(MainCommand5.Get()->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MainCommand5.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			MainCommand5.Get()->SetCommand(mainCommand.Key);
			MainCommand5.Get()->MakeMainCommand();
			continue;
		}
		if(MainCommand6.Get()->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MainCommand6.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
			MainCommand6.Get()->SetCommand(mainCommand.Key);
			MainCommand6.Get()->MakeMainCommand();
		}
	}
}

void UTavernMenu::ShowMainOrders()
{
	PlayAnimation(MainCommandsAnimation);
	TutorialNumber = 99;
}

void UTavernMenu::ContinueWithTavernAfterMainCommands()
{
	NextButtonBox.Get()->SetVisibility(ESlateVisibility::Hidden);
	NextButtonImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	if(ShouldShowSecondPartOfLevel2)
	{
		//To hide Main COmmand, have to do it in the following animation
		Level2();
	}
	else
	{
		PlayAnimationReverse(MainCommandsAnimation);
		TutorialNumber = 100;
	}
}

void UTavernMenu::HeadToGavernTutorial()
{
	ContinueHandle.Unregister();
	OpenGavernWidget();
	TalkedToGavernInTuto = true;
}

void UTavernMenu::ReturnVisibilityOfButtonsAfterMainCommands()
{
	ContinueHandle.Unregister();
	MainCommand1.Get()->SetVisibility(ESlateVisibility::Hidden);
	MainCommand2.Get()->SetVisibility(ESlateVisibility::Hidden);
	MainCommand3.Get()->SetVisibility(ESlateVisibility::Hidden);
	MainCommand4.Get()->SetVisibility(ESlateVisibility::Hidden);
	MainCommand5.Get()->SetVisibility(ESlateVisibility::Hidden);
	TutorialMainCanvas.Get()->SetVisibility(ESlateVisibility::Hidden);
	RecipeBookButton.Get()->SetVisibility(ESlateVisibility::Visible);
	QuestBoardButton.Get()->SetVisibility(ESlateVisibility::Visible);
	AdventureButton.Get()->SetVisibility(ESlateVisibility::Visible);
	NPCGoctorButton.Get()->SetIsEnabled(true);
	NPCGailorButton.Get()->SetIsEnabled(true);
	NPCGmithButton.Get()->SetIsEnabled(true);
	NPCGavernButton.Get()->SetIsEnabled(true);
	GamepadButton = QuestBoardButton;
	RequestRefreshFocus();
}

void UTavernMenu::CheckForCompletedOrders()
{
	//If playe completed a secondary quests
	if(!TTGameInstance.Get()->FinishedOrders.IsEmpty())
	{
		for(FSecondaryCommand secondaryFinishedCommand : TTGameInstance.Get()->FinishedOrders)
		{
			if(secondaryFinishedCommand.NPCTag == FGameplayTags::Get().NPC_Goctor)
			{
				GoctorSprite.Get()->ShowNotification(true);
			}
			else if(secondaryFinishedCommand.NPCTag == FGameplayTags::Get().NPC_Gailor)
			{
				GailorSprite.Get()->ShowNotification(true);
			}
			else if(secondaryFinishedCommand.NPCTag == FGameplayTags::Get().NPC_Gmith)
			{
				GmithSprite.Get()->ShowNotification(true);
			}
		}
	}
	//If player completed a bar and comes with a relic
	else
	{
		if(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionValue ==
			TTGameInstance.Get()->GoctorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel]
			&& TTGameInstance.Get()->RelicsInventory[0] > TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel)
		{
			GoctorSprite.Get()->ShowNotification(true);
		}
		else if(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionValue ==
			TTGameInstance.Get()->GailorSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel]
			&& TTGameInstance.Get()->RelicsInventory[1] > TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel)
		{
			GailorSprite.Get()->ShowNotification(true);
		}
		else if(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionValue ==
			TTGameInstance.Get()->GmithSatisfactionBarMaxValues[TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel]
			&& TTGameInstance.Get()->RelicsInventory[2] > TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel)
		{
			GmithSprite.Get()->ShowNotification(true);
		}
	}
	//If player has extra plates
	if(!TTGameInstance.Get()->ExtraPlatesInventory.IsEmpty())
	{
		GavernSprite.Get()->ShowNotification(true);
	}
}

void UTavernMenu::EstablishNavigation()
{
	if(TTGameInstance.Get()->CurrentLevel == 1)
	{
		//Recipe Book, Gavern and Door are visible
		AdventureButton.Get()->SetNavigationRuleExplicit(EUINavigation::Down, RecipeBookButton);
		RecipeBookButton.Get()->SetNavigationRuleExplicit(EUINavigation::Up, AdventureButton);
		RecipeBookButton.Get()->SetNavigationRuleExplicit(EUINavigation::Down, AdventureButton);
		AdventureButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right, NPCGavernButton);
		RecipeBookButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right, NPCGavernButton);
		NPCGavernButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right, AdventureButton);
	}
	else if(TTGameInstance.Get()->CurrentLevel == 2)
	{
		//Recipe Book, Gavern, Door, Goctor, Gailor, and Quest Board are visible
		NPCGailorButton.Get()->SetNavigationRuleExplicit(EUINavigation::Left, NPCGavernButton);
		NPCGavernButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right, NPCGailorButton);
		QuestBoardButton.Get()->SetNavigationRuleExplicit(EUINavigation::Down, NPCGailorButton);
	}
	//EASTER EGG, If Player completed all secondary misions
	if(TTGameInstance.Get()->UpgradeTier[0] == 3 && TTGameInstance.Get()->UpgradeTier[1] == 4 &&
		TTGameInstance.Get()->UpgradeTier[2] == 3)
	{
		EasterEggButton.Get()->SetVisibility(ESlateVisibility::Visible);
		EasterEggButton.Get()->OnClicked().AddUObject(this, &UTavernMenu::ShowEasterEggWidget);
		EasterEggButton.Get()->OnHovered().AddUObject(this, &UTavernMenu::OnHoverEasterEgg);
		EasterEggButton.Get()->OnUnhovered().AddUObject(this, &UTavernMenu::OnUnhoverEasterEgg);
		NPCGmithButton->SetNavigationRuleExplicit(EUINavigation::Left, EasterEggButton);
		NPCGavernButton.Get()->SetNavigationRuleExplicit(EUINavigation::Right, EasterEggButton);
	}
}
