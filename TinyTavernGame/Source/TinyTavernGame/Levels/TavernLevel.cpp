// Fill out your copyright notice in the Description page of Project Settings.


#include "TavernLevel.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Algo/RandomShuffle.h"
#include "Components/SpotLightComponent.h"
#include "Engine/SpotLight.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Actor/TTSpriteActor.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/Settings/TT_SaveGameSystem.h"
#include "TinyTavernGame/Settings/TT_UserSettings.h"
#include "TinyTavernGame/UI/HUD/LoadingSaveWidget.h"

ATavernLevel::ATavernLevel()
{
	GameplayTasksComponent = CreateDefaultSubobject<UGameplayTasksComponent>(TEXT("GameplayTasksComponent"));
}

void ATavernLevel::BeginPlay()
{
	Super::BeginPlay();
	
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	TTGameInstance.Get()->DetectTimePassed();
	// (new FAsyncTask<FCreateDefaultGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get()))->StartBackgroundTask();
	(new FAsyncTask<FCreateDefaultGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get()))->StartSynchronousTask();
	FAsyncTask<FSaveGameAsyncTask>* saveGameTask = new FAsyncTask<FSaveGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get());
	// saveGameTask->StartBackgroundTask();
	saveGameTask->StartSynchronousTask();
	StartTavern();
}

void ATavernLevel::StartTavern()
{
	TTGameInstance.Get()->MasterPostProcess = MasterPostProcess;
	TTGameInstance.Get()->ApplySettings();
	//Seteamos el Id de posicion de Ghunter del mapa en la taberna
	TTGameInstance.Get()->GhunterMapPosition = 0;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = EMouseCursor::Default;
	CurrentTavernButton = QUESTBOARD;
	//First Time in Tavern
	if(!TTGameInstance.Get()->FirstTimeInTavern)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), TavernMusic);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TavernAmbience, FVector(-53, -24, -10));
	}
	else
	{
		TavernAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), TavernMusic);
		TavernAmbienceAudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), TavernAmbience, FVector(-53, -24, -10));
	}
	TavernInstance = CreateWidget<UTavern>(GetWorld()->GetFirstPlayerController(), TavernWidget);
	TavernInstance.Get()->AddToViewport();
	TavernInstance.Get()->SetTavernSpriteActor(DoorSprite,QuestBoardSprite,GavernSprite,GoctorSprite,GailorSprite,GmithSprite,GookerSprite, RecipeBookSprite);
	QuestBoardZoomInBoard.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::OnFinishedQuestBoardAnimation);
	GmithZoom.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::OnFinishedQuestBoardAnimation);
	GoctorZoom.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::OnFinishedQuestBoardAnimation);
	GavernZoom.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::OnFinishedQuestBoardAnimation);
	GailorZoom.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::OnFinishedQuestBoardAnimation);
	EasterEggAnimation.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::OnFinishedQuestBoardAnimation);
	DoorZoom.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::TPToAdventure);
	FinalDoorZoom.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::TPToLastLevel);
	FadeInAnimation.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::TPToTutorial);
	
	//Randomize position of quest board commands
	RandomizeAvailableBoardOrders();
	//Manage Sprite Actors
	if(TTGameInstance.Get()->CurrentLevel < 2)
	{
		QuestBoardSprite.Get()->InActiveSprite();
	}
	AdventureDoorLight.Get()->SpotLightComponent.Get()->SetVisibility(false);
}

void ATavernLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	QuestBoardZoomInBoard.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	GmithZoom.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	GoctorZoom.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	GailorZoom.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	GavernZoom.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	DoorZoom.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	FinalDoorZoom.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	FadeInAnimation.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	EasterEggAnimation.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
	Recipes.Empty();
}

void ATavernLevel::OnFinishedQuestBoardAnimation()
{
	QuestBoardSprite.Get()->LightUp(false);
	GoctorSprite.Get()->LightUp(false);
	GailorSprite.Get()->LightUp(false);
	GmithSprite.Get()->LightUp(false);
	GavernSprite.Get()->LightUp(false);
	RecipeBookSprite.Get()->LightUp(false);
	if(!InEasterEggAnimation)
	{
		EasterLight.Get()->SpotLightComponent.Get()->SetHiddenInGame(true);
	}
	TavernInstance.Get()->ManageStackOfTavernWidgets(ShowingBoard, WidgetToAddName);
}

void ATavernLevel::RandomizeAvailableBoardOrders()
{
	TTGameInstance.Get()->BoardOrders.Empty();
	TTGameInstance.Get()->RecipeDataTable->GetAllRows("", Recipes);
	for(FRecipes_Struct* recipe: Recipes)
	{
		if(recipe->Progression <= TTGameInstance.Get()->CurrentLevel)
		{
			//If not accepted or finished, or a main order in that level
			// if(!TTGameInstance.Get()->AcceptedOrders.Contains(recipe->ID) &&
			// 	!TTGameInstance.Get()->FinishedOrders.Contains(recipe->ID) &&
			// 	recipe->Principal_Mission != TTGameInstance.Get()->CurrentLevel)
			// {
			if(recipe->Principal_Mission != TTGameInstance.Get()->CurrentLevel)
			{
				TTGameInstance.Get()->BoardOrders.Add(recipe->ID);
			}
		}
	}
	Algo::RandomShuffle(TTGameInstance.Get()->BoardOrders);
}

void ATavernLevel::NormalTavernAfterGooker()
{
	TavernInstance.Get()->ManageStackOfTavernWidgets(false, FTavernWidgets::TavernMenu);
}

void ATavernLevel::QuestBoardAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		QuestBoardSprite.Get()->LightUp(true);
		QuestBoardZoomInBoard.Get()->GetSequencePlayer()->Play();
		CurrentTavernButton = QUESTBOARD;
		WidgetToAddName = FTavernWidgets::QuestBoard;
		TavernInstance.Get()->HideTavernMenu();
	}
	else
	{
		QuestBoardZoomInBoard.Get()->GetSequencePlayer()->PlayReverse();
		WidgetToAddName = FTavernWidgets::TavernMenu;
	}
	ShowingBoard = ZoomIn;
}

void ATavernLevel::NPCGmithAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		GmithSprite.Get()->LightUp(true);
		GmithZoom.Get()->GetSequencePlayer()->Play();
		CurrentTavernButton = GMITH;
		WidgetToAddName = FTavernWidgets::GmithMenu;
		TavernInstance.Get()->HideTavernMenu();
	}
	else
	{
		GmithZoom.Get()->GetSequencePlayer()->PlayReverse();
		WidgetToAddName = FTavernWidgets::TavernMenu;
	}
	ShowingBoard = ZoomIn;
}

void ATavernLevel::NPCGoctorAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		GoctorSprite.Get()->LightUp(true);
		GoctorZoom.Get()->GetSequencePlayer()->Play();
		CurrentTavernButton = GOCTOR;
		WidgetToAddName = FTavernWidgets::GoctorMenu;
		TavernInstance.Get()->HideTavernMenu();
	}
	else
	{
		GoctorZoom.Get()->GetSequencePlayer()->PlayReverse();
		WidgetToAddName = FTavernWidgets::TavernMenu;
	}
	ShowingBoard = ZoomIn;
}

void ATavernLevel::NPCGailorAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		GailorSprite.Get()->LightUp(true);
		GailorZoom.Get()->GetSequencePlayer()->Play();
		CurrentTavernButton = GAILOR;
		WidgetToAddName = FTavernWidgets::GailorMenu;
		TavernInstance.Get()->HideTavernMenu();
	}
	else
	{
		GailorZoom.Get()->GetSequencePlayer()->PlayReverse();
		WidgetToAddName = FTavernWidgets::TavernMenu;
	}
	ShowingBoard = ZoomIn;
}

void ATavernLevel::NPCGavernAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		GavernSprite.Get()->LightUp(true);
		GavernZoom.Get()->GetSequencePlayer()->Play();
		CurrentTavernButton = GAVERN;
		WidgetToAddName = FTavernWidgets::GavernMenu;
		TavernInstance.Get()->HideTavernMenu();
	}
	else
	{
		GavernZoom.Get()->GetSequencePlayer()->PlayReverse();
		WidgetToAddName = FTavernWidgets::TavernMenu;
	}
	ShowingBoard = ZoomIn;
}

void ATavernLevel::NPCGookerAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		GookerZoom.Get()->GetSequencePlayer()->Play();
	}
	else
	{
		GookerZoom.Get()->GetSequencePlayer()->PlayReverse();
	}
}

void ATavernLevel::NPCGookerTutorialAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		GookerZoom.Get()->GetSequencePlayer()->OnFinished.AddDynamic(this, &ATavernLevel::OnFinishedQuestBoardAnimation);
		GookerZoom.Get()->GetSequencePlayer()->Play();
		WidgetToAddName = FTavernWidgets::GookerMenu;
		TavernInstance.Get()->HideTavernMenu();
	}
	else
	{
		GookerZoom.Get()->GetSequencePlayer()->OnFinished.RemoveAll(this);
		GookerZoom.Get()->GetSequencePlayer()->PlayReverse();
		WidgetToAddName = FTavernWidgets::TavernMenu;
	}
	ShowingBoard = ZoomIn;
}

void ATavernLevel::EasterEggShowAnimation(bool ZoomIn)
{
	if (ZoomIn)
	{
		EasterEggAnimation.Get()->GetSequencePlayer()->SetPlayRate(1.0f);
		EasterEggAnimation.Get()->GetSequencePlayer()->Play();
		CurrentTavernButton = EASTEREGG;
		WidgetToAddName = FTavernWidgets::EasterEggMenu;
		TavernInstance.Get()->HideTavernMenu();
	}
	else
	{
		EasterEggAnimation.Get()->GetSequencePlayer()->SetPlayRate(3.0f);
		EasterEggAnimation.Get()->GetSequencePlayer()->PlayReverse();
		WidgetToAddName = FTavernWidgets::TavernMenu;
	}
	InEasterEggAnimation = ZoomIn;
	ShowingBoard = ZoomIn;
}

void ATavernLevel::HoverEasterEgg(bool ShouldHover)
{
	if(ShouldHover)
	{
		EasterLight.Get()->SpotLightComponent.Get()->SetHiddenInGame(false);
	}
	else
	{
		if(!EasterEggAnimation.Get()->GetSequencePlayer()->IsPlaying())
		{
			EasterLight.Get()->SpotLightComponent.Get()->SetHiddenInGame(true);
		}
	}
}

void ATavernLevel::GoToAdventure()
{
	TTGameInstance.Get()->CompletedMainOrder = false;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = EMouseCursor::None;
	DoorSprite.Get()->ChangeToHoveredSprite(true);
	AdventureDoorLight.Get()->SpotLightComponent.Get()->SetVisibility(true);
	DoorZoom.Get()->GetSequencePlayer()->Play();
	TavernInstance.Get()->HideTavernMenu();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void ATavernLevel::GoToFinalLevel()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = EMouseCursor::None;
	DoorSprite.Get()->ChangeToHoveredSprite(true);
	AdventureDoorLight.Get()->SpotLightComponent.Get()->SetVisibility(true);
	FinalDoorZoom.Get()->GetSequencePlayer()->Play();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void ATavernLevel::SetAdventureDoorLight(bool Visibility)
{
	AdventureDoorLight.Get()->SpotLightComponent.Get()->SetVisibility(Visibility);
}

void ATavernLevel::PlayFadeAnimation()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = EMouseCursor::None;
	FadeInAnimation.Get()->GetSequencePlayer()->Play();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void ATavernLevel::TPToAdventure()
{
	if(TTGameInstance.Get()->IsGavernDay && TTGameInstance.Get()->GavernUpgrade != NOUPGRADE)
	{
		TTGameInstance.Get()->IsGavernDay = false;
		TTGameInstance.Get()->GavernUpgrade = NOUPGRADE;
	}
	else if(TTGameInstance.Get()->GavernUpgrade != NOUPGRADE)
	{
		TTGameInstance.Get()->IsGavernDay = true;
	}
	
	(new FAsyncTask<FSaveGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get()))->StartBackgroundTask();
	
	UGameplayStatics::OpenLevel(GetWorld(),AdventureLevel);
}

void ATavernLevel::TPToLastLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FinalCinematicLevel);
}

void ATavernLevel::OnLevelLoaded()
{
	ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(), TutorialLevel);
	if (LevelStreaming)
	{
		LevelStreaming->OnLevelLoaded.RemoveDynamic(this, &ATavernLevel::OnLevelLoaded);
	}
	(new FAsyncTask<FSaveGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get()))->StartBackgroundTask();
	UGameplayStatics::OpenLevel(GetWorld(), TutorialLevel);
}

void ATavernLevel::TPToTutorial()
{
	(new FAsyncTask<FSaveGameAsyncTask>(TTGameInstance.Get()->GetSubsystem<UTT_SaveGameSystem>(), TTGameInstance.Get()))->StartBackgroundTask();
	UGameplayStatics::OpenLevel(GetWorld(), TutorialLevel);
}

