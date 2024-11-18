// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArticyBaseTypes.h"
#include "GameplayTasksComponent.h"
#include "Engine/LevelScriptActor.h"
#include "../UI/Menus/Tavern/Tavern.h"   //Need it here for the enum
#include "TavernLevel.generated.h"

UENUM()
enum ETavernButtons
{
	RECIPEBOOK,
	QUESTBOARD,
	TAVERNDOOR,
	GAVERN,
	GMITH,
	GOCTOR,
	GAILOR,
	EASTEREGG
};

class APostProcessVolume;
class ASpotLight;
class ULoadingSaveWidget;
struct FArticyId;
class ALevelSequenceActor;
class UTT_GameInstance;
struct FLevelRow;
struct FRecipes_Struct;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API ATavernLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	ATavernLevel();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Post Process")
	TObjectPtr<APostProcessVolume> MasterPostProcess;
	UPROPERTY(BlueprintReadWrite, Category = "TT| Components")
	TObjectPtr<UGameplayTasksComponent> GameplayTasksComponent;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTavern> TavernInstance;
	UPROPERTY(EditAnywhere, Category = "TT| Sounds")
	TObjectPtr<USoundBase> TavernMusic;
	UPROPERTY(EditAnywhere, Category = "TT| Sounds")
	TObjectPtr<USoundBase> TavernAmbience;
	
	UPROPERTY(EditAnywhere, Category = "TT| Loading")
	TObjectPtr<AActor> LoadingActor;
	UPROPERTY(EditAnywhere, Category = "TT| Loading")
	TSubclassOf<ULoadingSaveWidget> LoadingSaveWidgetClass;
	
	//Adventure Level
	UPROPERTY(EditAnywhere, Category = "TT| Adventure")
	FName TutorialLevel;
	UPROPERTY(EditAnywhere, Category = "TT| Adventure")
	FName AdventureLevel;
	UPROPERTY(EditAnywhere, Category = "TT| Final Level")
	FName FinalCinematicLevel;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> QuestBoardZoomInBoard;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> GmithZoom;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> GavernZoom;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> GoctorZoom;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> GailorZoom;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> DoorZoom;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> FinalDoorZoom;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> GookerZoom;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> FadeInAnimation;
	UPROPERTY(EditAnywhere, Category = "TT| Sequences")
	TObjectPtr<ALevelSequenceActor> EasterEggAnimation;
	//TT Sprite Actors
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ATTSpriteActor> DoorSprite;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ATTSpriteActor> QuestBoardSprite;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ATTSpriteActor> GavernSprite;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ATTSpriteActor> GoctorSprite;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ATTSpriteActor> GailorSprite;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ATTSpriteActor> GmithSprite;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
    TObjectPtr<ATTSpriteActor> GookerSprite;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ATTSpriteActor> RecipeBookSprite;
	//Light
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ASpotLight> AdventureDoorLight;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Actors")
	TObjectPtr<ASpotLight> EasterLight;

	//Plays the zoom animation that moves camera to the Quest Board
	UFUNCTION(BlueprintCallable)
	void QuestBoardAnimation(bool ZoomIn);
	//Plays the zoom animation that moves camera to Gmith
	UFUNCTION(BlueprintCallable)
	void NPCGmithAnimation(bool ZoomIn);
	//Plays the zoom animation that moves camera to Goctor
	UFUNCTION(BlueprintCallable)
	void NPCGoctorAnimation(bool ZoomIn);
	//Plays the zoom animation that moves camera to Gailor
	UFUNCTION(BlueprintCallable)
	void NPCGailorAnimation(bool ZoomIn);
	//Plays the zoom animation that moves camera to Gavern
	UFUNCTION(BlueprintCallable)
	void NPCGavernAnimation(bool ZoomIn);
	//Play Gooker animation
	UFUNCTION(BlueprintCallable)
	void NPCGookerAnimation(bool ZoomIn);
	UFUNCTION(BlueprintCallable)
	void NPCGookerTutorialAnimation(bool ZoomIn);
	//Play Easter Egg animation
	UFUNCTION(BlueprintCallable)
	void EasterEggShowAnimation(bool ZoomIn);
	void HoverEasterEgg(bool ShouldHover);
	//Plays the zoom animation that moves camera to Adventure
	UFUNCTION(BlueprintCallable)
	void GoToAdventure();
	UFUNCTION(BlueprintCallable)
	void GoToFinalLevel();
	//When Door Hover, activate Door Light, unhover, deactivate it
	void SetAdventureDoorLight(bool Visibility);
	//Plays the zoom animation that moves camera to Adventure
	UFUNCTION(BlueprintCallable)
	void PlayFadeAnimation();
	//Gooker Dissapears and lets normal interaction with Tavern
	void NormalTavernAfterGooker();
	//Play a certain ArticyID for Gooker
	void SetSecondaryGookerDialog(FArticyId _GookerDialog){GookerDialog = _GookerDialog;}
	FArticyId GetSecondaryGookerDialog(){return GookerDialog;}
	ETavernButtons GetCurrentTavernButton(){return CurrentTavernButton;}
	TObjectPtr<UAudioComponent> GetTavernAudioComponent(){return TavernAudioComponent;}
	TObjectPtr<UAudioComponent> GetTavernAmbienceAudioComponent(){return TavernAmbienceAudioComponent;}
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "TT| Widgets")
	TSubclassOf<UTavern> TavernWidget;
	bool ShowingBoard = false;
	bool InEasterEggAnimation = false;
	FTavernWidgets WidgetToAddName;
	//Game Instance
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	//Recipe and level datatable
	TArray<FRecipes_Struct*> Recipes;
	//Gooker Dialog not related to progression
	FArticyId GookerDialog = NULL;
	ETavernButtons CurrentTavernButton = QUESTBOARD;
	TObjectPtr<ULoadingSaveWidget> LoadingWidget;
	//Used first time in Tavern
	TObjectPtr<UAudioComponent> TavernAudioComponent;
	TObjectPtr<UAudioComponent> TavernAmbienceAudioComponent;
	UFUNCTION()
	void StartTavern();
	//Function executed when Quest Board Zoom is Finished
	UFUNCTION()
	void OnFinishedQuestBoardAnimation();
	//When animation of Door finished, tp to Adventure
	UFUNCTION()
	void TPToAdventure();
	UFUNCTION()
	void TPToLastLevel();
	UFUNCTION()
	void OnLevelLoaded();
	//When animation of Fade In is finished, tp to tutorial or adventure depending on level 
	UFUNCTION()
	void TPToTutorial();
	//Randomized the available orders in the quest board
	//when player enters the tavern
	void RandomizeAvailableBoardOrders();
};
