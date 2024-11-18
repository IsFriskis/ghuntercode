// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ArticyBaseTypes.h"
#include "ArticyFlowPlayer.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "ULevel_DataTable.h"
#include "Engine/GameInstance.h"
#include "TT_GameInstance.generated.h"

//---------------------------------------------------------------------------
//-------------------------ACHIEVEMENTS--------------------------------------
//---------------------------------------------------------------------------
#define ACH_ENDING_1 FName("ACH_ENDING_1")
#define ACH_ENDING_2 FName("ACH_ENDING_2")
#define ACH_FIRST_DISH FName("ACH_FIRST_DISH")
#define ACH_BURN_10_ENEMIES FName("ACH_BURN_10_ENEMIES")
#define ACH_ALL_UPGRADES FName("ACH_ALL_UPGRADES")
#define ACH_CHEESE_20_ENEMIES FName("ACH_CHEESE_20_ENEMIES")
#define ACH_ALL_DISHES FName("ACH_ALL_DISHES")
#define ACH_ALL_LOOKOUTS FName("ACH_ALL_LOOKOUTS")
#define ACH_WELCOME_TO_GARCOSA FName("ACH_WELCOME_TO_GARCOSA")
#define ACH_SPEEDRUN_GHUNTER FName("ACH_SPEEDRUN_GHUNTER")
#define ACH_FIRST_GAMUSINO FName("ACH_FIRST_GAMUSINO")
#define ACH_FIRST_CUCAFERA FName("ACH_FIRST_CUCAFERA")
#define ACH_FIRST_BASILISK FName("ACH_FIRST_BASILISK")
#define ACH_THE_TEAM FName("ACH_THE_TEAM")
#define ACH_DISCOVER_ALL_NOTES FName("ACH_DISCOVER_ALL_NOTES")
#define ACH_FIRST_CUELEBRE FName("ACH_FIRST_CUELEBRE")
#define ACH_USE_HEALING_CIRCLE FName("ACH_USE_HEALING_CIRCLE")
#define ACH_ALL_BACKDOORS FName("ACH_ALL_BACKDOORS")
#define ACH_OPEN_A_STATUE FName("ACH_OPEN_A_STATUE")
#define ACH_NO_HIT FName("ACH_NO_HIT")
#define ACH_SPIN_THAT_WHEEL FName("ACH_SPIN_THAT_WHEEL")
#define ACH_BOIL_TOTEM FName("ACH_BOIL_TOTEM")
#define ACH_FRY_TOTEM FName("ACH_FRY_TOTEM")
#define ACH_SECONDARY_ORDER_COMPLETE FName("ACH_SECONDARY_ORDER_COMPLETE")
#define ACH_JAM_20_ENEMIES FName("ACH_JAM_20_ENEMIES")
#define ACH_WINE_20_ENEMIES FName("ACH_WINE_20_ENEMIES")
#define ACH_GATTLEAXE FName("ACH_GATTLEAXE")
#define ACH_SKEWER FName("ACH_SKEWER")

#define TOTAL_DISHES_INGAME 27
#define TOTAL_LOOKOUTS 5
#define TOTAL_STATUES 6
//---------------------------------------------------------------------------

class UTTUserWidget;
class APostProcessVolume;
class UTT_UserSettings;
class ATPWithChoices;
class ATeleport;
class USteamStatsAndAchievements;
class ULoadingScreenWidget;

USTRUCT()
struct FNPCInformationStruct
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FArticyId> NPCDialogs;
	//Level of NPC Satisfaction Bar
	UPROPERTY()
	int CurrentNPCSatisfactionLevel = 0;
	//Current value of Satisfaction bar
	UPROPERTY()
	float CurrentNPCSatisfactionValue = 0;
	//Command IDs
	UPROPERTY()
	TArray<int> NPCOrders;
};

UENUM()
enum EAttributeEnum
{
	//DON'T CHANGE ORDER PLEASE
	MAXHEALTH,	HEALTH,
	MAXGRILLEDAMMO, GRILLEDAMMO,
	MAXBOILAMMO, BOILAMMO,
	MAXFRYAMMO, FRYAMMO,
	MAXCHEESEAMMO, CHEESEAMMO,
	MAXJAMAMMO, JAMAMMO,
	MAXWINEAMMO, WINEAMMO
};
USTRUCT(BlueprintType)
struct FTTAttributesStruct 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Attributes")
	TEnumAsByte<EAttributeEnum> NameOfAttribute = MAXHEALTH;
	//Saves current attribute information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Attributes")
	FGameplayModifierInfo Attribute;
};

UENUM()
enum EGavernUpgrades
{
	NOUPGRADE = 0,
	SHIELD = 1,
	EXTRAREPUTATION = 2,
	PUFFBALLRESISTANCE = 3
};

UENUM()
enum EPlayerDeathCasuistic
{
	NODEATH = 0,
	WITHNOMAINORDER = 1,
	WITHALLMAINPLATES = 2
};

USTRUCT(BlueprintType)
struct FSecondaryCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag NPCTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlateID = 0;

	bool operator==(const FSecondaryCommand& OtherCommand) const
	{
		return (OtherCommand.NPCTag == NPCTag && OtherCommand.PlateID == PlateID);
	}
};
USTRUCT()
struct FAchievementData
{
	GENERATED_BODY()

	bool FlagRoyalCook;
	bool FlagForGuncle;
	bool FlagLetsGetCooking;
	bool FlagGastrognomicalStrength;
	bool FlagAllDishes;
	bool FlagAllLookouts;
	bool FlagSpeedrunCompleted;
	bool FlagOneGamusino;
	bool FlagOneCucafera;
	bool FlagOneBasilisk;
	bool FlagTheTeam;
	bool FlagAllNotesDiscovered;
	bool FlagOpenedAllStatues;
	bool FlagSolvedAStatue;
	bool FlagOneCuelebre;
	bool FlagUseAHealingCircle;
	bool FlagPlayerDied;
	bool FlagSpinThatWheel;
	bool FlagHandInSecondaryOrder;

	int EnemiesBurnt;
	int EnemiesHitWithCheese;
	int EnemiesHitWithJam;
	int EnemiesHitWithWine;
	int PlatesDiscovered;
	TArray<bool> LookoutsInteracted;
	int NumberOfStatuesOpened;

	FAchievementData()
	{
		FlagRoyalCook = false;
		FlagForGuncle = false;
		FlagLetsGetCooking = false;
		FlagGastrognomicalStrength = false;
		FlagAllDishes = false;
		FlagAllLookouts = false;
		FlagSpeedrunCompleted = false;
		FlagOneGamusino = false;
		FlagOneCucafera = false;
		FlagOneBasilisk = false;
		FlagTheTeam = false;
		FlagAllNotesDiscovered = false;
		FlagOneCuelebre = false;
		FlagUseAHealingCircle = false;
		FlagOpenedAllStatues = false;
		FlagSolvedAStatue = false;
		FlagPlayerDied = false;
		FlagSpinThatWheel = false;
		FlagHandInSecondaryOrder = false;
		
		EnemiesBurnt = 0;
		EnemiesHitWithCheese = 0;
		EnemiesHitWithJam = 0;
		EnemiesHitWithWine = 0;
		PlatesDiscovered = 0;
		LookoutsInteracted = {false, false, false, false, false};
		NumberOfStatuesOpened = 0;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCompletedRecipe, int, ID, FGameplayTag, NPCTag, bool, PremiumCompleted = false);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingCompleted);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTT_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TT| Loading Screen")
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	int MaxSatisfactionOfACommand = 220;
	//Not Acepted Orders
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
	
	//Secondary order Obtained on Gavern Day with Extra Reputation 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<FSecondaryCommand> SecondaryOrdersExtra;

	//Accepted orders
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TMap<int, bool> MainOrders;

	//Premiums in Main Order
	TArray<int> PremiumMainOrders;
	
	//Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> ExtraPlatesInventory;
	//ExtraPlates obtained during Gavern Day
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> ExtraPlatesDuringGavernDay;

	//Plates Of Statues in inventory so that in the inventory, Gavern picture and stars don't show up
	//the bool represents if completed or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TMap<int,bool> StatuesPlatesInInventory;

	//ID Levers Used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoors")
	TArray<int> UsedLevers;
	//ID OpenBackDoors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| BackDoors")
	TArray<int> OpenBackDoors;
	//Infinite Inventory for Tutorial
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> TutorialPlatesInventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	int MaxNumberOfExtraPlates = 6;

	//RelicsInventory[0] == Goctor, RelicsInventory[1] == Gailor, RelicsInventory[2] == Gmith
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> RelicsInventory = {0,0,0};

	//UpgradeTier[0] == Health, UpgradeTier[1] == Ammo, UpgradeTier[2] == Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TArray<int> UpgradeTier = {0,0,0};
		
	//Check if the in the day of adventure, you have obtained the Gavern Upgrade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes", meta=(UIMin=0, UIMax=5, ClampMin=0, ClampMax=5))
	float GavernExtraReputationValue = 0.2f;
	
	//Premium Extra
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes", meta=(UIMin=0, UIMax=5, ClampMin=0, ClampMax=5))
	float PremiumExtraValue = 1;
	
	//Order of Commands in Quest Board and with the PlateID that goes there
	UPROPERTY(EditAnywhere, Category = "TT| Recipes")
	TArray<unsigned int> PositionOfCommandsInQuestBoard;
	
	//Recipes DT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TObjectPtr<UDataTable> RecipeDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TObjectPtr<UDataTable> StatuesPlatesDataTable;

	FOnCompletedRecipe OnCompletedRecipe;
	FOnLoadingCompleted OnLoadingCompleted;
	
	//Commands Images
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TObjectPtr<UDataTable> CommandImagesTable;
	
	//Completed All Main Orders
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Recipes")
	TSubclassOf<UTTUserWidget> CompletedMainOrderWidget;

	//UPGRADE
	
	//Gavern Upgrade Obtained
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| UPGRADES")
	TEnumAsByte<EGavernUpgrades> GavernUpgrade = NOUPGRADE;

	//Check if the in the day of adventure, you have obtained the Gavern Upgrade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| UPGRADES")
	bool IsGavernDay = false;

	//Check if the in the day of adventure, you have obtained the Gavern Upgrade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| UPGRADES")
	int NumberOfHeartShield = 4;

	//Max Satisfaction Bar Value depending on level of progression with NPC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| UPGRADES")
	TArray<float> GoctorSatisfactionBarMaxValues = {100, 120, 150};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| UPGRADES")
	TArray<float> GailorSatisfactionBarMaxValues = {100, 120, 150};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| UPGRADES")
	TArray<float> GmithSatisfactionBarMaxValues = {100, 120, 150};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| UPGRADES")
	TArray<float> GavernSatisfactionBarMaxValues = {100, 120, 150};
	
	//Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Attributes")
	TArray<FTTAttributesStruct> AttributesData;
	
	//Level DT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TObjectPtr<UDataTable> LevelDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Level")
	int CurrentLevel = -1;

	//Statues
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TMap<int, bool> StatuesOpened;

	//Teleports
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TArray<int> TPsOpened;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TArray<TObjectPtr<ATPWithChoices>> TPsInWorld;
	
	//Chest Opened Goctor(0-2), Gailor(3-6), Gmith(7-9)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TMap<int, bool> ChestOpened;

	//Tutotials in Level
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Level")
	TArray<int> OpenedTutorials;
	
	//Weapon DT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Weapons")
	TObjectPtr<UDataTable> WeaponDataTable;
	
	//Unlocked weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Weapons", meta = (Categories = "Weapon"))
	TMap<FGameplayTag, bool> UnlockedWeapons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Weapons", meta = (Categories = "Weapon"))
	TMap<FGameplayTag, TEnumAsByte<FWeaponsLevel>> WeaponsLevel;
	
	//Cinematics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cinematics")
	bool ShoulPlayInitialCinematic = true;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Cinematics")
	bool ShowLogosAtStart = true;

	//NPC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TObjectPtr<UDataTable> GookerExpressionDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TObjectPtr<UDataTable> GavernExpressionDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TObjectPtr<UDataTable> GoctorExpressionDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TObjectPtr<UDataTable> GailorExpressionDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TObjectPtr<UDataTable> GmithExpressionDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TArray<FArticyId> GmithStartingDialogs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TArray<FArticyId> GoctorStartingDialogs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TArray<FArticyId> GailorStartingDialogs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TArray<FArticyId> GavernStartingDialogs;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TMap<int, bool> GuncleNoteFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	TObjectPtr<UDataTable> GuncleNotesDataTable;
	
	UPROPERTY(EditAnywhere, Category = "TT| Narrative")
	TMap<FGameplayTag, FNPCInformationStruct> NPCInformationMap;

	UPROPERTY(EditAnywhere, Category = "TT| LevelControl")
	bool FromMenu = false;
	
	//Save all dialogs read so that they appear in a different color
	//telling the player that they already read it
	TArray<FName> DialogsRead;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APostProcessVolume> MasterPostProcess;
	
	//Options
	TObjectPtr<UTT_UserSettings> UserSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TObjectPtr<USoundMix> SoundMix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TObjectPtr<USoundClass> MasterVolumeSoundClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TObjectPtr<USoundClass> MusicClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TObjectPtr<USoundClass> SFXClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	TObjectPtr<USoundClass> QTESoundClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Options")
	bool TriggerFantasyMusic = false;
	
	float InterpolationSpeed = 0.0f;
	float GamepadSensitivityX = 1.0f;
	float GamepadSensitivityY = 1.0f;
	int HideFPS = false;
	int GhunterMapPosition = 0;
	//Ending
	UPROPERTY()
	bool PoisonRoyalPlate = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Narrative")
	bool PlayerDecidedToPoisonTheKing = false;
	bool CompletedTheGame = false;
	//---
	//Death
	EPlayerDeathCasuistic PlayerDied = NODEATH;
	
	float TimedPlayed = 0.0f;
	FDateTime StartingTimeOfGameSession;
	
	virtual void OnStart() override;
	
	UFUNCTION()
	bool CheckCompleteDishes(int PlateID, bool IsPremium);
	
	UFUNCTION(BlueprintCallable)
	void DeathConsequences(float Percent);
	//Saves Ammunition when heading to Tavern
	UFUNCTION(BlueprintCallable)
	void SaveAmmunition();
	//Settings
	UFUNCTION(BlueprintCallable)
	void ApplySettings();
	void ChangeMasterVolume();
	void ChangeMusic();
	void ChangeSFX();
	void PushSound();

	//Tps
	bool IsTpUnlocked(int ID);
	void DeleteTPFromArray(int ID);
	void SetTPUnlocked(int ID);

	UFUNCTION(BlueprintCallable)
	void ApplySaveGameInstance();
	UFUNCTION(BlueprintCallable)
	void CompleteTutorialMainOrder();
	void InitializeSpeedRunTime();
	void DetectTimePassed();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_WriteAchievementForSteam(FName AchievementName);
	
	UFUNCTION()
	void BeginLoadingScreen(const FString& String);

	UFUNCTION()
	void EndLoadingScreen(UWorld* World);

	FAchievementData AchievementData;

	UFUNCTION(BlueprintCallable)
	void ActivateEnding();

	bool HasAllUpgrades();
	
	virtual void Init() override;

	void CheckIfAllMainOrdersHaveBeenCompleted();

	//---------------------------------------------------------------------------
	//-------------------------ACHIEVEMENTS--------------------------------------
	//---------------------------------------------------------------------------
	void RoyalCookCompleted();
	void ForGuncleCompleted();
	void OvercookedEnemy();
	void UnlockedUpgrade();
	void CheeseApplied();
	void PlateDiscovered();
	void LookoutInteracted(int LookoutID);
	void WelcomeToGarcosa();
	UFUNCTION(BlueprintCallable)
	void SpeedrunGhunter();
	void CheckCreature(FGameplayTag EnemyTag);
	void FirstGamusino();
	void FirstCucafera();
	void FirstBasilisk();
	void TheTeam();
	void ObtainAllNotes();
	UFUNCTION(BlueprintCallable)
	void FirstCuelebre();
	UFUNCTION(BlueprintCallable)
	void UsedAHealingCircle();
	void OpenAllStatues();
	void OpenAStatue();
	UFUNCTION(BlueprintCallable)
	void NoHit();
	void SpinThatWheel();
	void TotemUnlocked(FGameplayTag WeaponUnlocked);
	void HandInASecondaryOrder();
	void JamApplied();
	void WineApplied();
	void MeleeLevelUp(FWeaponsLevel MeleeWeaponLevel);
	//---------------------------------------------------------------------------
	
private:
	bool CheckIfPremium(int PlateID);
	TSharedPtr<SWidget> LoadingSWidgetPtr;
};
