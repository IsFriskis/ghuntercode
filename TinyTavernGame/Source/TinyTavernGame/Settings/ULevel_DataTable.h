// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ArticyBaseTypes.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "CommonButtonBase.h"
#include "TinyTavernGame/Weapons/Data/WeaponDataTable.h"
#include "ULevel_DataTable.generated.h"

//---Data Tables
USTRUCT()
struct FLevelRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Level"))
	int Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "NPC."))
	TArray<FGameplayTag> NPCS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Number of Commands"))
	int NumberOfSecondaryCommands = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Number of Commands"))
	int NumberOfPlayerAvailableCommands = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Weapon."))
	TMap<FGameplayTag,TEnumAsByte<FWeaponsLevel>> WeaponsUnlockedInLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Gooker Dialog"))
	FArticyId GookerDialogForMainOrder;
	
};
//-------------------------
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UULevel_DataTable : public UDataTable
{
	GENERATED_BODY()
};
