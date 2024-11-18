// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "WeaponDataTable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum FWeaponsLevel
{
	TierNull = 10,
	LVL1 = 0,
	LVL2 = 1,
	LVL3 = 2

	
};

USTRUCT(BlueprintType)
struct FWeaponDatableRowBase : public FTableRowBase
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Weapon"))
	FGameplayTag WeaponTag;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Events.Weapon"))
	FGameplayTag AbilityTag;

	/* ----------------------Till needs use----------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AimTag;
	*/
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<FWeaponsLevel> WeaponLevel = FWeaponsLevel::LVL2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMesh*> WeaponStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush WeaponSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isUnlocked = false;

	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};

UCLASS()
class TINYTAVERNGAME_API UWeaponDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
