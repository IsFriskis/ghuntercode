// Fill out your copyright notice in the Description page of Project Settings.


#include "Tavern.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "EasterEggUI.h"
#include "TavernMenu.h"
#include "Kismet/GameplayStatics.h"
#include "NPCS/NPCDialog.h"
#include "QuestBoard/QuestBoard.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Actor/TTSpriteActor.h"
#include "TinyTavernGame/Levels/TavernLevel.h"
#include "TinyTavernGame/Player/UIPlayer/UIPlayer.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/RecipeBook/RecipeBook.h"
#include "TinyTavernGame/UI/Menus/TitleScreen/FirstTimeInTavernUI.h"
#include "Windows/WindowsPlatformInput.h"

void UTavern::NativeConstruct()
{
	Super::NativeConstruct();
	
	//Button A acts like Click
	FKey& virtual_key = const_cast<FKey&>(EKeys::Virtual_Accept);
	virtual_key = FPlatformInput::GetGamepadAcceptKey();
	
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	UIPlayer = Cast<AUIPlayer>(GetOwningPlayerPawn());
	//Check if main order has been completed if not in Max Level == 6
	bool mainOrderCompleted = true;
	if(TTGameInstance.Get()->CurrentLevel + 1 != 7)
	{
		if(TTGameInstance.Get()->CurrentLevel == -1)
		{
			TTGameInstance.Get()->TutorialPlatesInventory.Empty();
			TTGameInstance.Get()->MainOrders.Empty();
			TTGameInstance.Get()->MainOrders.Add(111100, false);
		}
		else
		{
			for (TTuple<int, bool> mainOrder : TTGameInstance.Get()->MainOrders)
			{
				if(!mainOrder.Value)
				{
					mainOrderCompleted = false;
				}
			}
			TTGameInstance.Get()->CompletedMainOrder = mainOrderCompleted;
		}
		if(mainOrderCompleted)
		{
			//Level Up Progression
			if(!TTGameInstance.Get()->FromMenu)
			{
				TTGameInstance.Get()->CurrentLevel = TTGameInstance.Get()->CurrentLevel + 1;
			}
			switch (TTGameInstance.Get()->CurrentLevel)
			{
			case 3:
				TTGameInstance.Get()->FirstTimeInLevel3 = true;
				break;
			case 4:
				TTGameInstance.Get()->FirstTimeInLevel4 = true;
				break;
			case 5:
				TTGameInstance.Get()->FirstTimeInLevel5 = true;
				break;
			}
			if(TTGameInstance.Get()->CurrentLevel == 6)
			{
				AddWidgetToTavernStack(FTavernWidgets::GookerMenu);
			}
			else
			{
				//First Time In Tavern
				if(TTGameInstance.Get()->FirstTimeInTavern)
				{
					TObjectPtr<ATavernLevel> tavernLevel = Cast<ATavernLevel>(GetWorld()->GetLevelScriptActor());
					TavernMusicComponent = tavernLevel.Get()->GetTavernAudioComponent();
					TavernAmbienceComponent = tavernLevel.Get()->GetTavernAmbienceAudioComponent();
					TTGameInstance.Get()->FirstTimeInTavern = false;
					AddWidgetToTavernStack(FTavernWidgets::FirstTimeInTavern);
				}
				else
				{
					AddWidgetToTavernStack(FTavernWidgets::GookerMenu);	
				}
			}
		}
		else
		{
			//If player died with out completing the main order
			if(TTGameInstance.Get()->PlayerDied != NODEATH)
			{
				AddWidgetToTavernStack(FTavernWidgets::GookerMenu);
			}
			else
			{
				//else open tavern menu
				TavernMenu = Cast<UTavernMenu>(AddWidgetToTavernStack(FTavernWidgets::TavernMenu));	
			}
		}
	}
	else
	{
		//If player died in level 7
		if(TTGameInstance.Get()->PlayerDied != NODEATH)
		{
			AddWidgetToTavernStack(FTavernWidgets::GookerMenu);
		}
		else
		{
			TavernMenu = Cast<UTavernMenu>(AddWidgetToTavernStack(FTavernWidgets::TavernMenu));	
		}
	}
	TTGameInstance.Get()->FromMenu = false;
	//Get Level Progression Info
	TTGameInstance.Get()->LevelDataTable->GetAllRows("", LevelRows);
}

void UTavern::NativeDestruct()
{
	Super::NativeDestruct();
	
	LevelRows.Empty();
}

TObjectPtr<UCommonActivatableWidget> UTavern::AddWidgetToTavernStack(FTavernWidgets WidgetName)
{
	TObjectPtr<UTavernMenu> tavernMenu;
	TObjectPtr<URecipeBook> recipeBook;
	TObjectPtr<UFirstTimeInTavernUI> firstTimeInTavern;
	switch (WidgetName)
	{
	case FTavernWidgets::TavernMenu:
		tavernMenu = Cast<UTavernMenu>(TavernStack.Get()->AddWidget(TavernMenuWidget));
		UIPlayer.Get()->AddTavern(tavernMenu, this);
		return tavernMenu;
	case FTavernWidgets::QuestBoard:
		return TavernStack.Get()->AddWidget(QuestBoardWidget);
	case FTavernWidgets::RecipeBook:
		RecipeBook.Get()->ChangeToHoveredSprite(true);
		recipeBook = Cast<URecipeBook>(TavernStack.Get()->AddWidget(PauseMenuWidget));
		recipeBook.Get()->RecipeBookInTavern();
		return recipeBook;
	case FTavernWidgets::GmithMenu:
		return TavernStack.Get()->AddWidget(GmithWidget);
	case FTavernWidgets::GoctorMenu:
		return TavernStack.Get()->AddWidget(GoctorWidget);
	case FTavernWidgets::GailorMenu:
		return TavernStack.Get()->AddWidget(GailorWidget);
	case FTavernWidgets::GavernMenu:
		return TavernStack.Get()->AddWidget(GavernWidget);
	case FTavernWidgets::GookerMenu:
		return TavernStack.Get()->AddWidget(GookerWidget);
	case FTavernWidgets::EasterEggMenu:
		return TavernStack.Get()->AddWidget(EasterEggWidget);
	case FTavernWidgets::FirstTimeInTavern:
		firstTimeInTavern = Cast<UFirstTimeInTavernUI>(TavernStack.Get()->AddWidget(FirstTimeInTavernWidget));
		firstTimeInTavern.Get()->AddTavern(this, TavernMusicComponent, TavernAmbienceComponent);
		return firstTimeInTavern;
	default:
		break;
	}
	return TavernStack.Get()->AddWidget(TavernMenuWidget);
}

void UTavern::HideTavernMenu()
{
	TavernMenu.Get()->DeactivateWidget();
}

void UTavern::ManageStackOfTavernWidgets(bool Hide, FTavernWidgets WidgetName)
{
	if (Hide)
	{
		AddWidgetToTavernStack(WidgetName);
	}
	else
	{
		TavernMenu = Cast<UTavernMenu>(AddWidgetToTavernStack(WidgetName));
		TavernMenu.Get()->SetTavernSpriteActors(DoorSprite,QuestBoardSprite,GavernSprite,GoctorSprite,GailorSprite,GmithSprite, GookerSprite, RecipeBook);
		if(TTGameInstance.Get()->CurrentLevel == 0)
		{
			//Tavern Tutorial part 2
			TavernMenu.Get()->Level0();
		}
		if(TTGameInstance.Get()->CurrentLevel == 1 && TTGameInstance.Get()->FirstTimeInLevel1)
		{
			//Tavern Tutorial part 3
			TavernMenu.Get()->Level1();
			TTGameInstance.Get()->FirstTimeInLevel1 = false;
		}
		if(TTGameInstance.Get()->CurrentLevel == 2 && TTGameInstance.Get()->FirstTimeInLevel2)
		{
			//Tavern Tutorial part 4
			TavernMenu.Get()->Level2();
		}
	}
}

void UTavern::SetTavernSpriteActor(TObjectPtr<ATTSpriteActor> _DoorSprite, TObjectPtr<ATTSpriteActor> _QuestBoardSprite,
	TObjectPtr<ATTSpriteActor> _GavernSprite, TObjectPtr<ATTSpriteActor> _GoctorSprite,
	TObjectPtr<ATTSpriteActor> _GailorSprite, TObjectPtr<ATTSpriteActor> _GmithSprite,
	TObjectPtr<ATTSpriteActor> _GookerSprite, TObjectPtr<ATTSpriteActor> _RecipeBook)
{
	DoorSprite = _DoorSprite;
	QuestBoardSprite = _QuestBoardSprite;
	GavernSprite = _GavernSprite;
	GoctorSprite = _GoctorSprite;
	GailorSprite = _GailorSprite;
	GmithSprite = _GmithSprite;
	GookerSprite = _GookerSprite;
	RecipeBook = _RecipeBook;

	if(TavernMenu)
	{
		TavernMenu.Get()->SetTavernSpriteActors(DoorSprite,QuestBoardSprite,GavernSprite,GoctorSprite,GailorSprite,GmithSprite,GookerSprite, RecipeBook);
	}
	//Show NPCs depending on level of progression
	if(TTGameInstance.Get()->CompletedTheGame || TTGameInstance.Get()->CurrentLevel != 6)
	{
		VisibilityOfNPCs();
	}
}

void UTavern::CloseRecipeBook()
{
	RecipeBook.Get()->ChangeToHoveredSprite(false);
}

void UTavern::SetTavernMusicAudioComponent(TObjectPtr<UAudioComponent> _TavernMusicComponent)
{
	TavernMusicComponent = _TavernMusicComponent;
}

void UTavern::VisibilityOfNPCs()
{
	GavernSprite.Get()->SetActorHiddenInGame(true);
	GoctorSprite.Get()->SetActorHiddenInGame(true);
	GailorSprite.Get()->SetActorHiddenInGame(true);
	GmithSprite.Get()->SetActorHiddenInGame(true);
	//Visibility of NPCs
	for (FLevelRow* levelRow : LevelRows)
	{
		for(FGameplayTag npc: levelRow->NPCS)
		{
			if(npc == FGameplayTags::Get().NPC_Gavern)
			{
				GavernSprite.Get()->SetActorHiddenInGame(false);
			}
			else if(npc == FGameplayTags::Get().NPC_Goctor)
			{
				GoctorSprite.Get()->SetActorHiddenInGame(false);
			}
			else if(npc == FGameplayTags::Get().NPC_Gailor)
			{
				GailorSprite.Get()->SetActorHiddenInGame(false);
			}
			else if(npc == FGameplayTags::Get().NPC_Gmith)
			{
				GmithSprite.Get()->SetActorHiddenInGame(false);
			}
		}
		if(levelRow->Level == TTGameInstance.Get()->CurrentLevel)
		{
			break;
		}
	}
}
