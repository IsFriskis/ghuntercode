// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tavern.generated.h"

class UFirstTimeInTavernUI;
class UEasterEggUI;
class AUIPlayer;
struct FLevelRow;
class ATTSpriteActor;
class UCommonActivatableWidget;
class UTT_GameInstance;
class UNPCDialog;
class URecipeBook;
class UQuestBoard;
class UTavernMenu;
class UCommonActivatableWidgetStack;
//---Enum of Tavern Widgets---
UENUM(BlueprintType)
enum class FTavernWidgets: uint8
{
	TavernMenu,
	QuestBoard,
	Adventure,
	RecipeBook,
	GmithMenu,
	GoctorMenu,
	GailorMenu,
	GavernMenu,
	GookerMenu,
	EasterEggMenu,
	FirstTimeInTavern
};
//-------------------------
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTavern : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Adds a Widget to the Stack
	TObjectPtr<UCommonActivatableWidget> AddWidgetToTavernStack(FTavernWidgets WidgetName);
	//Hides the tavern Menu buttons
	void HideTavernMenu();
	//Manages stack of tavern widgets during animations
	void ManageStackOfTavernWidgets(bool Hide, FTavernWidgets WidgetName = FTavernWidgets::TavernMenu);
	void SetTavernSpriteActor(TObjectPtr<ATTSpriteActor> _DoorSprite,TObjectPtr<ATTSpriteActor> _QuestBoardSprite, TObjectPtr<ATTSpriteActor> _GavernSprite, TObjectPtr<ATTSpriteActor> _GoctorSprite, TObjectPtr<ATTSpriteActor> _GailorSprite, TObjectPtr<ATTSpriteActor> _GmithSprite, TObjectPtr<ATTSpriteActor> _GookerSprite, TObjectPtr<ATTSpriteActor> _RecipeBook);
	void CloseRecipeBook();
	void SetTavernMusicAudioComponent(TObjectPtr<UAudioComponent> _TavernMusicComponent);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> TavernStack;
	//Common Activatable Widgets
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UTavernMenu> TavernMenuWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UQuestBoard> QuestBoardWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<URecipeBook> PauseMenuWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UNPCDialog> GmithWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UNPCDialog> GoctorWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UNPCDialog> GailorWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UNPCDialog> GavernWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UNPCDialog> GookerWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UEasterEggUI> EasterEggWidget;
	UPROPERTY(EditAnywhere, Category = "TT| Tavern Widgets")
	TSubclassOf<UFirstTimeInTavernUI> FirstTimeInTavernWidget;
	
	//TTSprite Actors
	TObjectPtr<ATTSpriteActor> DoorSprite;
	TObjectPtr<ATTSpriteActor> QuestBoardSprite;
	TObjectPtr<ATTSpriteActor> GavernSprite;
	TObjectPtr<ATTSpriteActor> GoctorSprite;
	TObjectPtr<ATTSpriteActor> GailorSprite;
	TObjectPtr<ATTSpriteActor> GmithSprite;
	TObjectPtr<ATTSpriteActor> GookerSprite;
	TObjectPtr<ATTSpriteActor> RecipeBook;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	TObjectPtr<UTavernMenu> TavernMenu;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	TObjectPtr<AUIPlayer> UIPlayer;
	//Level Database
	TArray<FLevelRow*> LevelRows;
	//Variable used so that spawning the pause button doesn't ruin it
	bool InPauseTransition = false;
	//Used first time in Tavern to control its volume
	TObjectPtr<UAudioComponent> TavernMusicComponent;
	TObjectPtr<UAudioComponent> TavernAmbienceComponent;
	
	void VisibilityOfNPCs();
};
