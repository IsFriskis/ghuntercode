// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TT_GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "TT_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTT_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UTT_SaveGame();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trial")
	bool BoolStateToSave;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> BoardOrders;
	
	//Accepted orders
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<FSecondaryCommand> AcceptedOrders;

	//Finished orders 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<FSecondaryCommand> FinishedOrders;

	//Discovered plates 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<FString> DiscoveredPlates;

	//Discovered Enemies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TT| Recipes")
	TArray<FGameplayTag> EnemiesDiscoveredTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<FSecondaryCommand> SecondaryOrdersExtra;
	
	//Accepted orders
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TMap<int, bool> MainOrders;

	//Premiums in Main Order
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> PremiumMainOrders;

	//Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> ExtraPlatesInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> ExtraPlatesDuringGavernDay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TMap<int,bool> StatuesPlatesInInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> RelicsInventory = {0,0,0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> UpgradeTier = {0,0,0};

	//Gavern Upgrade Obtained
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TEnumAsByte<EGavernUpgrades> GavernUpgrade = NOUPGRADE;

	//Check if the in the day of adventure, you have obtained the Gavern Upgrade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	bool IsGavernDay = false;

	//Order of Commands in Quest Board and with the PlateID that goes there
	UPROPERTY(EditAnywhere, Category = "TT| Questboard")
	TArray<unsigned int> PositionOfCommandsInQuestBoard;

	//Check Acitvate Backdoors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoors")
	TArray<int> OpenBackDoors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoors")
	TArray<int> UsedLevers;
	
	//Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Attributes")
	TArray<FTTAttributesStruct> AttributesData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Level")
	int CurrentLevel = -2;

	//Statues
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TMap<int, bool> StatuesOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TArray<int> TPsOpened;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TArray<int> OpenedTutorials;

	//Chest Opened Goctor(0-2), Gailor(3-6), Gmith(7-9)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TMap<int, bool> ChestOpened;
	
	//Unlocked weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Weapons", meta = (Categories = "Weapon"))
	TMap<FGameplayTag, bool> UnlockedWeapons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Weapons", meta = (Categories = "Weapon"))
	TMap<FGameplayTag, TEnumAsByte<FWeaponsLevel>> WeaponsLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cinematics")
	bool ShoulPlayInitialCinematic = true;

	//Tutorial
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Tutorial")
	bool InTutorial = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool FirstTimeInLevel1 = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool FirstTimeInLevel2 = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool FirstTimeInLevel3 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool FirstTimeInLevel4 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool FirstTimeInLevel5 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool CompletedMainOrder = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool FirstTimeInTavern = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| ProgressManagment")
	bool FirstTimeInGame = true;

	//NPC
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TMap<int, bool> GuncleNoteFound;

	//Save all dialogs read so that they appear in a different color
	//telling the player that they already read it
	UPROPERTY()
	TArray<FName> DialogsRead;
	
	UPROPERTY(VisibleAnywhere, Category = "TT| Narrative")
	TMap<FGameplayTag, FNPCInformationStruct> NPCInformationMap;

	//Ending
	UPROPERTY()
	bool PoisonRoyalPlate = false;
	UPROPERTY()
	bool CompletedTheGame = false;
	//Death
	UPROPERTY()
	TEnumAsByte<EPlayerDeathCasuistic> PlayerDied = NODEATH;
	UPROPERTY()
	float TimedPlayed = 0.0f;
	UPROPERTY()
	FAchievementData AchievementData;
	
protected:

private:

	FString SlotToSaveName = "SlotA";
	int32 SlotToSaveUserIdIndex = 1;
};
