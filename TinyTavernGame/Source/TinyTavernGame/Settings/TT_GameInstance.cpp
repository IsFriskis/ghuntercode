// TinyTavern


#include "TT_GameInstance.h"

#include "MoviePlayer.h"
#include "TT_SaveGameSystem.h"
#include "TT_UserSettings.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Actor/Structures/TPWithChoices.h"
#include "TinyTavernGame/Actor/Resources/ResourceManager.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/UI/HUD/LoadingScreenWidget.h"
#include "TinyTavernGame/UI/Menus/Buttons/InventorySlot.h"
#include "OnlineSubsystemSteam.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineStatsInterface.h"
#include "TinyTavernGame/UI/TTUserWidget.h"

bool UTT_GameInstance::CheckCompleteDishes(int PlateID, bool IsPremium)
{
	if(CurrentLevel > 0)
	{
		//Check Main Orders
		for (TTuple<int, bool> plateID : MainOrders)
		{
			if(!plateID.Value)
			{
				if(PremiumMainOrders.Contains(plateID.Key))
				{
					//If Is Premium and premium main order
					if(IsPremium && CheckIfPremium(PlateID))
					{
						MainOrders[plateID.Key] = true;
						OnCompletedRecipe.Broadcast(PlateID, FGameplayTags::Get().NPC_Goctor,true);
						return true;
					}
				}
				else if(plateID.Key == PlateID && !plateID.Value)
				{
					//Not Premium
					MainOrders[plateID.Key] = true;
					OnCompletedRecipe.Broadcast(PlateID, FGameplayTags::Get().NPC_Goctor, false);
					return true;
				}
			}
		}
		//Check if extra plate is a statue plate
		if(StatuesPlatesInInventory.Contains(PlateID) && !StatuesPlatesInInventory[PlateID])
		{
			StatuesPlatesInInventory[PlateID] = true;
			return true;
		}
		//Check Secondary Orders
		for (FSecondaryCommand plate : AcceptedOrders)
		{
			if(plate.PlateID == PlateID)
			{
				FinishedOrders.Add(plate);
				AcceptedOrders.Remove(plate);
				//Check if During Gavern Day with Extra Reputation
				if(IsGavernDay && GavernUpgrade == EXTRAREPUTATION)
				{
					SecondaryOrdersExtra.Add(plate);
				}
				OnCompletedRecipe.Broadcast(PlateID, plate.NPCTag, false);
				return true;
			}
		}
		//Add to extra plates
		if(ExtraPlatesInventory.Num() != MaxNumberOfExtraPlates)
		{
			if(IsGavernDay && GavernUpgrade == EGavernUpgrades::EXTRAREPUTATION)
			{
				ExtraPlatesDuringGavernDay.Add(PlateID);
			}
			ExtraPlatesInventory.Add(PlateID);
			//Inventory full of Extra Plates, notify player
			Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->InventoryFull();
		}
		else
		{
			//Inventory full of Extra Plates, notify player
			Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->InventoryFull(true);
		}
	}
	else
	{
		TutorialPlatesInventory.Add(PlateID);
	}
	
	return false;
}

void UTT_GameInstance::OnStart()
{
	Super::OnStart();
	
	//Load Settings
	UserSettings = Cast<UTT_UserSettings>(GEngine->GameUserSettings);
	UserSettings.Get()->LoadSettings();
	ApplySettings();
	HideFPS = UserSettings.Get()->ShouldShowFPS();
	
	//Initialize NPC Information
	TArray<FArticyId> GuncleDialogs;
	TArray<FArticyId> GookerDialogs;
	//**Do not touch order please, first gmith, then goctor, then gailor and then the rest**
	NPCInformationMap.Add(FGameplayTags::Get().NPC_Goctor, FNPCInformationStruct(GoctorStartingDialogs));
	NPCInformationMap.Add(FGameplayTags::Get().NPC_Gailor, FNPCInformationStruct(GailorStartingDialogs));
	NPCInformationMap.Add(FGameplayTags::Get().NPC_Gmith, FNPCInformationStruct(GmithStartingDialogs));
	NPCInformationMap.Add(FGameplayTags::Get().NPC_Gavern, FNPCInformationStruct(GavernStartingDialogs));
	NPCInformationMap.Add(FGameplayTags::Get().NPC_Gooker, FNPCInformationStruct(GookerDialogs));
	NPCInformationMap.Add(FGameplayTags::Get().NPC_Guncle, FNPCInformationStruct(GuncleDialogs));

	//Quest Board Info
	//TODO probably delete
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);
	PositionOfCommandsInQuestBoard.Add(-1);

	//Fill guncle notes map
	for (int i = 0 ; i < GuncleNotesDataTable.Get()->GetRowNames().Num(); i++)
	{
		GuncleNoteFound.Add(i,false);
	}
	//Add Statue Opened
	StatuesOpened.Add(0,false);
	StatuesOpened.Add(1,false);
	StatuesOpened.Add(2,false);
	StatuesOpened.Add(3,false);
	StatuesOpened.Add(4,false);
	StatuesOpened.Add(5,false);
	StatuesOpened.Add(6,false);
	StatuesOpened.Add(7,false);
	StatuesOpened.Add(8,false);
	StatuesOpened.Add(9,false);
	StatuesOpened.Add(10,false);
	StatuesOpened.Add(11,false);
	StatuesOpened.Add(12,false);
	StatuesOpened.Add(13,false);
	StatuesOpened.Add(14,false);
	StatuesOpened.Add(15,false);
	
	//Add Chest Opened
	ChestOpened.Add(0,false);
	ChestOpened.Add(1,false);
	ChestOpened.Add(2,false);
	ChestOpened.Add(3,false);
	ChestOpened.Add(4,false);
	ChestOpened.Add(5,false);
	ChestOpened.Add(6,false);
	ChestOpened.Add(7,false);
	ChestOpened.Add(8,false);
	ChestOpened.Add(9,false);
	

	(new FAsyncTask<FLoadGameAsyncTask>(this->GetSubsystem<UTT_SaveGameSystem>()))->StartBackgroundTask();
	
}

void UTT_GameInstance::DeathConsequences(float Percent)
{
	//Ammunition Loss
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(player)
	{
		TObjectPtr<UTT_AttributeSet> ttAttribute = Cast<UTT_AttributeSet>(player.Get()->GetAttributeSet());
		if(ttAttribute)
		{
			for(int i = 0; i < AttributesData.Num(); i++)
			{
				switch (AttributesData[i].NameOfAttribute)
				{
				case GRILLEDAMMO:
					AttributesData[i].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->GrilledAmmo.GetCurrentValue() * Percent);
					break;
				case BOILAMMO:
					AttributesData[i].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->BoiledAmmo.GetCurrentValue() * Percent);
					break;
				case FRYAMMO:
					AttributesData[i].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->FriedAmmo.GetCurrentValue() * Percent);
					break;
				case CHEESEAMMO:
					AttributesData[i].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->CheeseAmmo.GetCurrentValue() * Percent);
					break;
				case JAMAMMO:
					AttributesData[i].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->JamAmmo.GetCurrentValue() * Percent);
					break;
				case WINEAMMO:
					AttributesData[i].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->WineAmmo.GetCurrentValue() * Percent);
					break;
				default: ;
				}
			}
		}
	}
	//Eliminate Secondary Commands and Extra Plates
	for (FSecondaryCommand completedPlate : FinishedOrders)
	{
		AcceptedOrders.Add(completedPlate);
	}
	FinishedOrders.Empty();
	SecondaryOrdersExtra.Empty();
	ExtraPlatesInventory.Empty();
	ExtraPlatesDuringGavernDay.Empty();
	//Player only dies from level 1 and beyond
	if(CurrentLevel > 0)
	{
		uint32_t numberOfMainPlatesCompleted = 0;
		for(TTuple<int, bool> mainPlate : MainOrders)
		{
			if(mainPlate.Value)
			{
				++numberOfMainPlatesCompleted;
			}
		}
		if (numberOfMainPlatesCompleted == MainOrders.Num())
		{
			PlayerDied = WITHALLMAINPLATES;
		}
		else
		{
			PlayerDied = WITHNOMAINORDER;
		}
		//Achievement of NoHit lost
		AchievementData.FlagPlayerDied = true;
	}
}

void UTT_GameInstance::SaveAmmunition()
{
	TObjectPtr<APlayerCharacter> player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(player)
	{
		TObjectPtr<UTT_AttributeSet> ttAttribute = Cast<UTT_AttributeSet>(player.Get()->GetAttributeSet());
		if(ttAttribute)
		{
			//GRILLEDAMMO
			AttributesData[3].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->GrilledAmmo.GetCurrentValue());
			//BOILAMMO
			AttributesData[5].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->BoiledAmmo.GetCurrentValue());
			//FRYAMMO
			AttributesData[7].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->FriedAmmo.GetCurrentValue());
			//CHEESEAMMO
			AttributesData[9].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->CheeseAmmo.GetCurrentValue());
			//JAMAMMO
			AttributesData[11].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->JamAmmo.GetCurrentValue());
			//WINEAMMO
			AttributesData[13].Attribute.ModifierMagnitude = FGameplayEffectModifierMagnitude(ttAttribute.Get()->WineAmmo.GetCurrentValue());
		}
	}
}

void UTT_GameInstance::ApplySettings()
{
	if(UserSettings)
	{
		UserSettings->ApplySettings(true);
		//Apply World Settings
		FSlateApplicationBase::Get().GetRenderer()->SetColorVisionDeficiencyType(UserSettings.Get()->GetColorBlindMode(), UserSettings.Get()->GetColorBlindStrength(), true, false);
		if(UserSettings.Get()->ShouldShowFPS() != HideFPS)
		{
			HideFPS = !HideFPS;
			UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "stat FPS");
		}
		//Post Process Settings
		if(MasterPostProcess)
		{
			//Post Process Loaded Correctly
			if(!MasterPostProcess.Get()->GetActorNameOrLabel().Equals("TavernPostProcess"))
			{
				MasterPostProcess.Get()->Settings.AutoExposureBias = UserSettings.Get()->GetBrightnessValue();
				MasterPostProcess.Get()->Settings.MotionBlurAmount = UserSettings.Get()->GetMotionBlur();
			}
		}
		else
		{
#if WITH_EDITOR
			print("NO POST PROCESS FOUND, ADD ONE NOW");
#endif
		}
		//Sound
		ChangeMasterVolume();
		ChangeMusic();
		ChangeSFX();
		PushSound();
		//Sensitivity
		InterpolationSpeed = UserSettings.Get()->GetInterpolationSpeed();
		GamepadSensitivityX = UserSettings.Get()->GetGamepadSensitivityX();
		GamepadSensitivityY = UserSettings.Get()->GetGamepadSensitivityY();
		if(GetFirstLocalPlayerController())
		{
			GetFirstLocalPlayerController()->bForceFeedbackEnabled = UserSettings.Get()->GetVibration();
		}
	}
}

void UTT_GameInstance::ChangeMasterVolume()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MasterVolumeSoundClass, UserSettings.Get()->GetMasterVolume());
}

void UTT_GameInstance::ChangeMusic()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, MusicClass, UserSettings.Get()->GetMusicVolume());
}

void UTT_GameInstance::ChangeSFX()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SFXClass, UserSettings.Get()->GetSFXVolume());
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, QTESoundClass, UserSettings.Get()->GetSFXVolume());
}

void UTT_GameInstance::PushSound()
{
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
}


bool UTT_GameInstance::IsTpUnlocked(int ID)
{
	for (int Tp: TPsOpened)
	{
		if(Tp == ID)
		{
			return true;
		}
	}
	
	return false;
}

void UTT_GameInstance::DeleteTPFromArray(int _ID)
{
	if(!TPsInWorld.IsEmpty())
	{
		TPsInWorld.Empty();
	}
}

void UTT_GameInstance::SetTPUnlocked(int ID)
{
	TPsOpened.Add(ID);
}

void UTT_GameInstance::ApplySaveGameInstance()
{
}

void UTT_GameInstance::CompleteTutorialMainOrder()
{
	MainOrders[111100] = true;
	OnCompletedRecipe.Broadcast(111100, FGameplayTags::Get().NPC_Goctor,false);
}

void UTT_GameInstance::InitializeSpeedRunTime()
{
	StartingTimeOfGameSession = FDateTime::Now();
}

void UTT_GameInstance::DetectTimePassed()
{
	FTimespan difference = FDateTime::Now() - StartingTimeOfGameSession;
	StartingTimeOfGameSession = FDateTime::Now();
	TimedPlayed += difference.GetDays() * 86400 + difference.GetHours() * 3600 + difference.GetMinutes() * 60 + difference.GetSeconds();
}

bool UTT_GameInstance::CheckIfPremium(int PlateID)
{
	for(int i = 0; i < PremiumMainOrders.Num(); i++)
	{
		//Premium Information
		int enemyPremiumID = PremiumMainOrders[i] / 100000;
		int cutPremiumID = (PremiumMainOrders[i] - 100000) / 10000;
		int cookPremiumID = (PremiumMainOrders[i] - 110000) / 1000;
		//Plate Information
		int enemyPlateID = PlateID / 100000;
		int cutPlateID = (PlateID - 100000) / 10000;
		int cookPlateID = (PlateID - 110000) / 1000;
		//Compare
		if(enemyPremiumID == enemyPlateID &&
			cutPremiumID == cutPlateID &&
			cookPremiumID == cookPlateID)
		{
			return true;
		}
	}

	return false;
}		

void UTT_GameInstance::BeginLoadingScreen(const FString& String)
{
	if (!IsRunningDedicatedServer())
	{
		if(LoadingWidgetClass)
		{
			//print("LOADING SCREEN")
			UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass, TEXT("LoadingScreen"));

			LoadingSWidgetPtr = LoadingWidget->TakeWidget();
			FLoadingScreenAttributes LoadingScreen;
			LoadingScreen.WidgetLoadingScreen = LoadingSWidgetPtr;
			LoadingScreen.bAllowInEarlyStartup = true;
			LoadingScreen.PlaybackType = EMoviePlaybackType::MT_Normal;
			LoadingScreen.bAllowEngineTick = true;
			LoadingScreen.bWaitForManualStop = false;
			LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
			LoadingScreen.MinimumLoadingScreenDisplayTime = 3.0f;

			if(GetMoviePlayer())
			{
				//print("Setup Loading Screen")
				GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
			}
				
		}
	}
}

void UTT_GameInstance::EndLoadingScreen(UWorld* World)
{
	//print("END LOADING SCREEN")
	GetSubsystem<UResourceManager>(this)->Init();
	OnLoadingCompleted.Broadcast();
}

void UTT_GameInstance::ActivateEnding()
{
	DetectTimePassed();
	if (PlayerDecidedToPoisonTheKing)
	{
		ForGuncleCompleted();
	}
	else
	{
		RoyalCookCompleted();
	}
}

bool UTT_GameInstance::HasAllUpgrades()
{
	if (UpgradeTier[0] == 3 && UpgradeTier[1] == 4 && UpgradeTier[2] == 3)
	{
		return true;
	}
	return false;
}


void UTT_GameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UTT_GameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UTT_GameInstance::EndLoadingScreen);
}

void UTT_GameInstance::CheckIfAllMainOrdersHaveBeenCompleted()
{
	if(CompletedMainOrder)
	{
		return;
	}
	for (TTuple<int, bool> plate : MainOrders)
	{
		if(!plate.Value)
		{
			return;
		}
	}
	//If reached this point, all commands have been completed
	CreateWidget(GetWorld(), CompletedMainOrderWidget.Get())->AddToViewport();
	CompletedMainOrder = true;
}

void UTT_GameInstance::RoyalCookCompleted()
{
	if (!AchievementData.FlagRoyalCook)
	{
		AchievementData.FlagRoyalCook = true;
		BP_WriteAchievementForSteam(ACH_ENDING_1);
	}
}

void UTT_GameInstance::ForGuncleCompleted()
{
	if (!AchievementData.FlagForGuncle)
	{
		AchievementData.FlagForGuncle = true;
		BP_WriteAchievementForSteam(ACH_ENDING_2);
	}
}

void UTT_GameInstance::OvercookedEnemy()
{
	if (AchievementData.EnemiesBurnt < 10)
	{
		AchievementData.EnemiesBurnt++;
		if (AchievementData.EnemiesBurnt >= 10)
		{
			BP_WriteAchievementForSteam(ACH_BURN_10_ENEMIES);
		}
	}
}

void UTT_GameInstance::UnlockedUpgrade()
{
	if (!AchievementData.FlagGastrognomicalStrength)
	{
		if (HasAllUpgrades())
		{
			AchievementData.FlagGastrognomicalStrength = true;
			BP_WriteAchievementForSteam(ACH_ALL_UPGRADES);
		}
	}
}

void UTT_GameInstance::CheeseApplied()
{
	if (AchievementData.EnemiesHitWithCheese < 20)
	{
		AchievementData.EnemiesHitWithCheese++;
		if (AchievementData.EnemiesHitWithCheese >= 20)
		{
			BP_WriteAchievementForSteam(ACH_CHEESE_20_ENEMIES);
		}
	}
}

void UTT_GameInstance::PlateDiscovered()
{
	AchievementData.PlatesDiscovered++;

	if (AchievementData.PlatesDiscovered == 1 && !AchievementData.FlagLetsGetCooking)
	{
		AchievementData.FlagLetsGetCooking = true;
		BP_WriteAchievementForSteam(ACH_FIRST_DISH);
	}
	
	if(!AchievementData.FlagAllDishes && DiscoveredPlates.Num() == TOTAL_DISHES_INGAME)
	{
		AchievementData.FlagAllDishes = true;
		BP_WriteAchievementForSteam(ACH_ALL_DISHES);
	}
}

void UTT_GameInstance::LookoutInteracted(int LookoutID)
{
	if (!AchievementData.FlagAllLookouts)
	{
		AchievementData.LookoutsInteracted[LookoutID] = true;
		for (bool HasInteracted : AchievementData.LookoutsInteracted)
		{
			if (!HasInteracted)
			{
				return;
			}
		}
		AchievementData.FlagAllLookouts = true;
		BP_WriteAchievementForSteam(ACH_ALL_LOOKOUTS);
	}
}

void UTT_GameInstance::WelcomeToGarcosa()
{
	BP_WriteAchievementForSteam(ACH_WELCOME_TO_GARCOSA);
}

void UTT_GameInstance::SpeedrunGhunter()
{
	//30 minutes = 1800 seconds
	if(!AchievementData.FlagSpeedrunCompleted && TimedPlayed < 1800)
	{
		AchievementData.FlagSpeedrunCompleted = true;
		BP_WriteAchievementForSteam(ACH_SPEEDRUN_GHUNTER);	
	}
}

void UTT_GameInstance::CheckCreature(FGameplayTag EnemyTag)
{
	if(EnemyTag.MatchesTag(FGameplayTags::Get().Enemy_Gamusino) && !AchievementData.FlagOneGamusino)
	{
		AchievementData.FlagOneGamusino = true;
		FirstGamusino();
	}
	else if(EnemyTag.MatchesTag(FGameplayTags::Get().Enemy_Cucafera) && !AchievementData.FlagOneCucafera)
	{
		AchievementData.FlagOneCucafera = true;
		FirstCucafera();
	}
	else if(EnemyTag.MatchesTag(FGameplayTags::Get().Enemy_Basilisco) && !AchievementData.FlagOneBasilisk)
	{
		AchievementData.FlagOneBasilisk = true;
		FirstBasilisk();
	}
}

void UTT_GameInstance::FirstGamusino()
{
	BP_WriteAchievementForSteam(ACH_FIRST_GAMUSINO);
}

void UTT_GameInstance::FirstCucafera()
{
	BP_WriteAchievementForSteam(ACH_FIRST_CUCAFERA);
}

void UTT_GameInstance::FirstBasilisk()
{
	BP_WriteAchievementForSteam(ACH_FIRST_BASILISK);
}

void UTT_GameInstance::TheTeam()
{
	if (!AchievementData.FlagTheTeam)
	{
		AchievementData.FlagTheTeam = true;
		BP_WriteAchievementForSteam(ACH_THE_TEAM);
	}
}

void UTT_GameInstance::ObtainAllNotes()
{
	if(!AchievementData.FlagAllNotesDiscovered)
	{
		for (int i = 0 ; i < GuncleNoteFound.Num(); i++)
		{
			if(!GuncleNoteFound[i])
			{
				return;
			}
		}
		AchievementData.FlagAllNotesDiscovered = true;
		BP_WriteAchievementForSteam(ACH_DISCOVER_ALL_NOTES);
	}
}

void UTT_GameInstance::FirstCuelebre()
{
	if(!AchievementData.FlagOneCuelebre)
	{
		AchievementData.FlagOneCuelebre = true;
		BP_WriteAchievementForSteam(ACH_FIRST_CUELEBRE);
	}
}

void UTT_GameInstance::UsedAHealingCircle()
{
	if(!AchievementData.FlagUseAHealingCircle)
	{
		AchievementData.FlagUseAHealingCircle = true;
		BP_WriteAchievementForSteam(ACH_USE_HEALING_CIRCLE);
	}
}

void UTT_GameInstance::OpenAllStatues()
{
	if(!AchievementData.FlagOpenedAllStatues)
	{
		++AchievementData.NumberOfStatuesOpened;
		if(AchievementData.NumberOfStatuesOpened == TOTAL_STATUES)
		{
			AchievementData.FlagOpenedAllStatues = true;
			BP_WriteAchievementForSteam(ACH_ALL_BACKDOORS);
		}
	}
}

void UTT_GameInstance::OpenAStatue()
{
	if(!AchievementData.FlagSolvedAStatue)
	{
		AchievementData.FlagSolvedAStatue = true;
		BP_WriteAchievementForSteam(ACH_OPEN_A_STATUE);
	}
}

void UTT_GameInstance::NoHit()
{
	if(!AchievementData.FlagPlayerDied)
	{
		BP_WriteAchievementForSteam(ACH_NO_HIT);
	}
}

void UTT_GameInstance::SpinThatWheel()
{
	if(!AchievementData.FlagSpinThatWheel)
	{
		AchievementData.FlagSpinThatWheel = true;
		BP_WriteAchievementForSteam(ACH_SPIN_THAT_WHEEL);
	}
}

void UTT_GameInstance::TotemUnlocked(FGameplayTag WeaponUnlocked)
{
	if(WeaponUnlocked == FGameplayTags::Get().Weapon_Range_Boil)
	{
		BP_WriteAchievementForSteam(ACH_BOIL_TOTEM);
	}
	else if(WeaponUnlocked == FGameplayTags::Get().Weapon_Range_Fry)
	{
		BP_WriteAchievementForSteam(ACH_FRY_TOTEM);
	}
}

void UTT_GameInstance::HandInASecondaryOrder()
{
	if(!AchievementData.FlagHandInSecondaryOrder)
	{
		AchievementData.FlagHandInSecondaryOrder = true;
		BP_WriteAchievementForSteam(ACH_SECONDARY_ORDER_COMPLETE);
	}
}

void UTT_GameInstance::JamApplied()
{
	if (AchievementData.EnemiesHitWithJam < 20)
	{
		AchievementData.EnemiesHitWithJam++;
		if (AchievementData.EnemiesHitWithJam >= 20)
		{
			BP_WriteAchievementForSteam(ACH_JAM_20_ENEMIES);
		}
	}
}

void UTT_GameInstance::WineApplied()
{
	if (AchievementData.EnemiesHitWithWine < 20)
	{
		AchievementData.EnemiesHitWithWine++;
		if (AchievementData.EnemiesHitWithWine >= 20)
		{
			BP_WriteAchievementForSteam(ACH_WINE_20_ENEMIES);
		}
	}
}

void UTT_GameInstance::MeleeLevelUp(FWeaponsLevel MeleeWeaponLevel)
{
	if(MeleeWeaponLevel == LVL2)
	{
		BP_WriteAchievementForSteam(ACH_GATTLEAXE);
	}
	else if(MeleeWeaponLevel == LVL3)
	{
		BP_WriteAchievementForSteam(ACH_SKEWER);
	}
}
