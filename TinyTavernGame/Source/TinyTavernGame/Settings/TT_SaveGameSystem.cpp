// TinyTavern


#include "TT_SaveGameSystem.h"

#include "TT_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TinyTavernUtils.h"



//UTT_SaveGame* UTT_SaveGameSystem::SaveGameInstance = nullptr;

UTT_SaveGameSystem::UTT_SaveGameSystem(const FObjectInitializer& ObjectInitializer)
{
	//Init Slot Name & UserIdIndex
	SlotToSaveName = SAVEGAME_SLOT_NAME ;
	SlotToSaveUserIdIndex = SAVEGAME_USERINDEX;

	CreateSaveGameInstance();
}


bool UTT_SaveGameSystem::CreateSaveGameInstance()
{
	SaveGameInstance = Cast<UTT_SaveGame>(UGameplayStatics::CreateSaveGameObject(UTT_SaveGame::StaticClass()));
	return true;
}

bool UTT_SaveGameSystem::CreateDefaultSaveGameInstance(UGameInstance* _GameInstance)
{
	if(!UGameplayStatics::DoesSaveGameExist(SAVEGAMEDEFAULT_SLOT_NAME, SAVEGAME_USERINDEX))
	{
		SaveGameInstance = Cast<UTT_SaveGame>(UGameplayStatics::CreateSaveGameObject(UTT_SaveGame::StaticClass()));
		if(ParseAllGameInstanceToSaveGame(_GameInstance))
		{
		
			if(UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVEGAMEDEFAULT_SLOT_NAME, SAVEGAME_USERINDEX))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
			
	}
	return true;
}


bool UTT_SaveGameSystem::LoadSaveGameData()
{
	if(UGameplayStatics::DoesSaveGameExist(SAVEGAME_SLOT_NAME, SAVEGAME_USERINDEX))
	{
		SaveGameInstance = Cast<UTT_SaveGame>(UGameplayStatics::LoadGameFromSlot(SAVEGAME_SLOT_NAME, SAVEGAME_USERINDEX));
		
		if (SaveGameInstance)
		{
			ApplyLoadToGameInstance(SaveGameInstance);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//UGameplayStatics::CreateSaveGameObject(UTT_SaveGame::StaticClass());
		//UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVEGAME_SLOT_NAME,SAVEGAME_USERINDEX);
		
		return false;	
	} 
}

bool UTT_SaveGameSystem::ApplyLoadToGameInstance(UTT_SaveGame* _SaveGameInstance)
{
	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	
	if(GameInstance && _SaveGameInstance)
	{
		// ORDERS
        GameInstance->BoardOrders = _SaveGameInstance->BoardOrders;
        GameInstance->AcceptedOrders = _SaveGameInstance->AcceptedOrders;
        GameInstance->FinishedOrders = _SaveGameInstance->FinishedOrders;
        GameInstance->DiscoveredPlates = _SaveGameInstance->DiscoveredPlates;
		GameInstance->EnemiesDiscoveredTags = _SaveGameInstance->EnemiesDiscoveredTags;
        GameInstance->SecondaryOrdersExtra = _SaveGameInstance->SecondaryOrdersExtra;
        GameInstance->MainOrders = _SaveGameInstance->MainOrders;
        GameInstance->PremiumMainOrders = _SaveGameInstance->PremiumMainOrders;

        // INVENTORY
        GameInstance->ExtraPlatesInventory = _SaveGameInstance->ExtraPlatesInventory;
        GameInstance->ExtraPlatesDuringGavernDay = _SaveGameInstance->ExtraPlatesDuringGavernDay;
        GameInstance->StatuesPlatesInInventory = _SaveGameInstance->StatuesPlatesInInventory;

        GameInstance->UsedLevers = _SaveGameInstance->UsedLevers;
        GameInstance->OpenBackDoors = _SaveGameInstance->OpenBackDoors;

        GameInstance->RelicsInventory = _SaveGameInstance->RelicsInventory;
        GameInstance->UpgradeTier = _SaveGameInstance->UpgradeTier;

        GameInstance->GavernUpgrade = _SaveGameInstance->GavernUpgrade;
        GameInstance->IsGavernDay = _SaveGameInstance->IsGavernDay;
		
		if(_SaveGameInstance->PositionOfCommandsInQuestBoard.Num() == 0)
		{
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
			_SaveGameInstance->PositionOfCommandsInQuestBoard.Add(-1);
		}
        GameInstance->PositionOfCommandsInQuestBoard = _SaveGameInstance->PositionOfCommandsInQuestBoard;
		//FGameplayModifierInfo maxhealthAttribute;
		//FGameplayAttribute maxhealth;
		//maxhealth.AttributeName = "TT_AttributeSet.MaxHealth";
		//maxhealthAttribute.Attribute = maxhealth;
		//maxhealthAttribute.ModifierOp = EGameplayModOp::Override;
	//	maxhealthAttribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(6.0f);
		//GameInstance->AttributesData.Add(FTTAttributesStruct(EAttributeEnum::MAXHEALTH, FGameplayModifierInfo());
        GameInstance->AttributesData = _SaveGameInstance->AttributesData;
        GameInstance->CurrentLevel = _SaveGameInstance->CurrentLevel;

        // LEVEL PROGRESSION
        GameInstance->StatuesOpened = _SaveGameInstance->StatuesOpened;
        GameInstance->TPsOpened = _SaveGameInstance->TPsOpened;
        GameInstance->OpenedTutorials = _SaveGameInstance->OpenedTutorials;
        GameInstance->ChestOpened = _SaveGameInstance->ChestOpened;

        // WEAPON PROGRESSION
        GameInstance->UnlockedWeapons = _SaveGameInstance->UnlockedWeapons;
        GameInstance->WeaponsLevel = _SaveGameInstance->WeaponsLevel;

        GameInstance->ShoulPlayInitialCinematic = _SaveGameInstance->ShoulPlayInitialCinematic;

        // TUTORIAL
        GameInstance->FirstTimeInLevel1 = _SaveGameInstance->FirstTimeInLevel1;
        GameInstance->FirstTimeInLevel2 = _SaveGameInstance->FirstTimeInLevel2;
        GameInstance->FirstTimeInLevel3 = _SaveGameInstance->FirstTimeInLevel3;
        GameInstance->FirstTimeInLevel4 = _SaveGameInstance->FirstTimeInLevel4;
        GameInstance->FirstTimeInLevel5 = _SaveGameInstance->FirstTimeInLevel5;
		GameInstance->FirstTimeInGame = _SaveGameInstance->FirstTimeInGame;
		GameInstance->FirstTimeInTavern = _SaveGameInstance->FirstTimeInTavern;
        GameInstance->CompletedMainOrder = _SaveGameInstance->CompletedMainOrder;
        GameInstance->GuncleNoteFound = _SaveGameInstance->GuncleNoteFound;
        GameInstance->NPCInformationMap = _SaveGameInstance->NPCInformationMap;
		GameInstance->DialogsRead = _SaveGameInstance->DialogsRead;

		//Ending
		GameInstance->PoisonRoyalPlate = _SaveGameInstance->PoisonRoyalPlate;
		GameInstance->AchievementData = _SaveGameInstance->AchievementData;
		GameInstance->CompletedTheGame = _SaveGameInstance->CompletedTheGame;
		//Death
		GameInstance->PlayerDied = _SaveGameInstance->PlayerDied;
		GameInstance->TimedPlayed = _SaveGameInstance->TimedPlayed;
		return true;
	}
	else
	{
		return false;
	}
}

bool UTT_SaveGameSystem::ParseAllGameInstanceToSaveGame(UGameInstance* _GameInstance)
{
	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(_GameInstance);
	
	if(GameInstance && SaveGameInstance)
	{
		//ORDERS
		SaveGameInstance->BoardOrders = GameInstance->BoardOrders;
		SaveGameInstance->AcceptedOrders = GameInstance->AcceptedOrders;
		SaveGameInstance->FinishedOrders = GameInstance->FinishedOrders;
		SaveGameInstance->DiscoveredPlates = GameInstance->DiscoveredPlates;
		SaveGameInstance->EnemiesDiscoveredTags = GameInstance->EnemiesDiscoveredTags;
		SaveGameInstance->SecondaryOrdersExtra = GameInstance->SecondaryOrdersExtra;
		SaveGameInstance->MainOrders = GameInstance->MainOrders;
		SaveGameInstance->PremiumMainOrders = GameInstance->PremiumMainOrders;

		//INVENTORY
		SaveGameInstance->ExtraPlatesInventory = GameInstance->ExtraPlatesInventory;
		SaveGameInstance->ExtraPlatesDuringGavernDay = GameInstance->ExtraPlatesDuringGavernDay;
		SaveGameInstance->StatuesPlatesInInventory = GameInstance->StatuesPlatesInInventory;
		
		SaveGameInstance->UsedLevers = GameInstance->UsedLevers;
		SaveGameInstance->OpenBackDoors = GameInstance->OpenBackDoors;
		
		SaveGameInstance->RelicsInventory = GameInstance->RelicsInventory;
		SaveGameInstance->UpgradeTier = GameInstance->UpgradeTier;
		
		SaveGameInstance->GavernUpgrade = GameInstance->GavernUpgrade;
		SaveGameInstance->IsGavernDay = GameInstance->IsGavernDay;
		SaveGameInstance->PositionOfCommandsInQuestBoard = GameInstance->PositionOfCommandsInQuestBoard;
		SaveGameInstance->AttributesData = GameInstance->AttributesData;
		SaveGameInstance->CurrentLevel = GameInstance->CurrentLevel;

		//LEVEL PROGRESSION
		SaveGameInstance->StatuesOpened = GameInstance->StatuesOpened;
		SaveGameInstance->TPsOpened = GameInstance->TPsOpened;
		SaveGameInstance->OpenedTutorials = GameInstance->OpenedTutorials;
		SaveGameInstance->ChestOpened = GameInstance->ChestOpened;

		//WEAPON PROGRESSION
		SaveGameInstance->UnlockedWeapons = GameInstance->UnlockedWeapons;
		SaveGameInstance->WeaponsLevel = GameInstance->WeaponsLevel;

		SaveGameInstance->ShoulPlayInitialCinematic = GameInstance->ShoulPlayInitialCinematic;
		
		SaveGameInstance->FirstTimeInLevel1 = GameInstance->FirstTimeInLevel1;
		SaveGameInstance->FirstTimeInLevel2 = GameInstance->FirstTimeInLevel2;
		SaveGameInstance->FirstTimeInLevel3 = GameInstance->FirstTimeInLevel3;
		SaveGameInstance->FirstTimeInLevel4 = GameInstance->FirstTimeInLevel4;
		SaveGameInstance->FirstTimeInLevel5 = GameInstance->FirstTimeInLevel5;
		SaveGameInstance->FirstTimeInGame = GameInstance->FirstTimeInGame;
		SaveGameInstance->FirstTimeInTavern = GameInstance->FirstTimeInTavern;
		SaveGameInstance->CompletedMainOrder = GameInstance->CompletedMainOrder;
		SaveGameInstance->GuncleNoteFound = GameInstance->GuncleNoteFound;
		SaveGameInstance->NPCInformationMap = GameInstance->NPCInformationMap;
		SaveGameInstance->DialogsRead = GameInstance->DialogsRead;


		//Ending
		SaveGameInstance->PoisonRoyalPlate = GameInstance->PoisonRoyalPlate; 
		SaveGameInstance->AchievementData = GameInstance->AchievementData;
		SaveGameInstance->CompletedTheGame = GameInstance->CompletedTheGame;
		
		//Death
		SaveGameInstance->PlayerDied = GameInstance->PlayerDied;
		SaveGameInstance->TimedPlayed = GameInstance->TimedPlayed;
		
		return true;
		
	}
	else
	{
		return false;
	}
}


bool UTT_SaveGameSystem::SaveGameData(UGameInstance* _GameInstance)
{

	if(SaveGameInstance)
	{
		//I HAVE SAVEGAMEINSTANCE
		if(ParseAllGameInstanceToSaveGame(_GameInstance))
		{
			// start an example job
			//
			
			//PARSE GOES GOOD
			if(UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVEGAME_SLOT_NAME,SAVEGAME_USERINDEX))
			{
				//SAVE GOES GOOD
				return true;
			}
			else
			{
				//SAVE GOES BAD
				print("SaveGameData: Failed to save game");
				return false;
			}
		}
		else
		{
			print("SaveGameData: Failed to parse game instance to save game");
			return false;
		}
	}
	else
	{
		if(LoadSaveGameData())
		{
			//TRY TO LOAD
			if(UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVEGAME_SLOT_NAME,SAVEGAME_USERINDEX))
			{
				//SAVE GOES GOOD
				return true;
			}
			else
			{
				//SAVE GOES BAD
				print("SaveGameData: Failed to save game instance to save game");
				return false;
			}
		}
		else
		{
			//NO LOAD FOUND
			if(CreateSaveGameInstance())
			{
				//CREATE NEW INSTANCE
				if(UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVEGAME_SLOT_NAME,SAVEGAME_USERINDEX))
				{
					//SAVE GOES GOOD
					return true;
				}
				else
				{
					//SAVE GOES BAD
					print("SaveGameData: Failed to save game instance to save game");
					return false;
				}
			}
			else
			{
				//CREATE NEW INSTANCE GOES BAD
				print("SaveGameData: Failed to create save game instance");
				return false;
			}
		}
	}
}

bool UTT_SaveGameSystem::DeleteSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SAVEGAME_SLOT_NAME, SAVEGAME_USERINDEX))
	{
		if(UGameplayStatics::DeleteGameInSlot(SAVEGAME_SLOT_NAME, SAVEGAME_USERINDEX))
		{
			if(UGameplayStatics::DoesSaveGameExist(SAVEGAMEDEFAULT_SLOT_NAME, SAVEGAME_USERINDEX))
			{
				SaveGameInstance = Cast<UTT_SaveGame>(UGameplayStatics::LoadGameFromSlot(SAVEGAMEDEFAULT_SLOT_NAME, SAVEGAME_USERINDEX));
				if(SaveGameInstance)
				{
					ApplyLoadToGameInstance(SaveGameInstance);
					return true;
				}
			}
			return false;
		}
		return false;
	}
	return false;
}

