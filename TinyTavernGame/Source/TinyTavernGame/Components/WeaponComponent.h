// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Weapons/Data/WeaponDataTable.h"
#include "WeaponComponent.generated.h"

class UTT_GameInstance;
class UNiagaraComponent;
class UNiagaraSystem;
class UTT_AttributeSet;
class UImage;
class UGameplayEffect;
class APlayerCharacter;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangeToWidgetControllerSignature, FGameplayTag, newWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnlockedSignature);

UENUM(BlueprintType)
enum class FWeaponsType: uint8
{
	Melee,
	Range,
	Ingredient
};

UCLASS()
class UPayloadReleaseIngredientSignature : public UObject
{
	
	GENERATED_BODY()
	
public:
	UPayloadReleaseIngredientSignature(){};
	
	UPROPERTY(BlueprintAssignable)
	FReleaseIngredientSignature ReleaseInput;
	
};
USTRUCT()
struct FPayloadReleaseMeleeSignature : public FGameplayEventData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FReleaseMeleeSignature ReleaseInput;
	
};


USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UImage> WeaponSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Ammo_Cost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> AddTagToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isUnlocked = false;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TINYTAVERNGAME_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();
	
	
	UPROPERTY()
	FGameplayTag MeleeWeapon;
	
	UPROPERTY()
	unsigned int CurrentRangeWeapon;
	
	UPROPERTY()
	unsigned int CurrentIngredientWeapon;
	
	//--------------DELEGATES-------------//
	
	UPROPERTY(BlueprintAssignable, Category = "TT| Weapons")
	FOnWeaponChangeToWidgetControllerSignature OnWeaponChange;
	UPROPERTY(BlueprintAssignable, Category = "TT| Weapons")
	FOnWeaponUnlockedSignature OnWeaponUnlocked;
	
	//-----------FUNCTIONS----------

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	FGameplayTag GetCurrentRangeWeaponTag();

	UFUNCTION()
	FGameplayTag GetCurrentIngredientWeaponTag();

	UFUNCTION()
	void ChangeRangeWeapon();
	
	UFUNCTION()
	void ChangeIngredientWeapon();
	
	UFUNCTION()
	void ShootMelee();
	
	UFUNCTION()
	void ShootRange();
	
	UFUNCTION()
	void ShootIngredient();
	
	UFUNCTION(BlueprintCallable)
	FWeaponDatableRowBase GetWeaponDataByTag(const FGameplayTag WeaponTag);
	
	UFUNCTION()
	void SetWeapon(FGameplayTag WeaponTag);
	UFUNCTION(BlueprintCallable)
	void ChangeWeaponForDash();
	UFUNCTION(BlueprintCallable)
	void ChangeWeaponToQTE();
	UFUNCTION(BlueprintCallable)
	void ChangeWeaponToRange();


	TObjectPtr<UDataTable> GetWeaponDataTable() const{return WeaponDataTable;}
	void SetWeaponDataTable(TObjectPtr<UDataTable> _WeaponDataTable){ this->WeaponDataTable = _WeaponDataTable;}

	UFUNCTION()
	void ChangeToMeleeWeaponMesh();
	UFUNCTION(BlueprintCallable)
	void ClearWeapons();

	UFUNCTION()
	void ChangeToRangeWeaponMesh();
	void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData);

	UFUNCTION()
	void ChangeToIngredientWeaponMesh();
	void InitWeaponManager();
	
	bool bIsIngredientActive = false;
protected:
	//WeaponManager Variables
	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> WeaponsContainer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> GrillNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> BoilNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> FryNiagaraSystem;

private:

	
	TArray<FWeaponDatableRowBase> WeaponsData; 
	
	TObjectPtr<APlayerCharacter> PlayerRef;
	TObjectPtr<UTT_AttributeSet> TTAttributeSet;
	TObjectPtr<UNiagaraComponent> GrillCauldronVFX;
	TObjectPtr<UNiagaraComponent> BoilCauldronVFX;
	TObjectPtr<UNiagaraComponent> FryCauldronVFX;
	int CurrentCauldronVFXNumber = -1;
	TObjectPtr<UTT_GameInstance> TTGameInstance;
	//-----------FUNCTIONS----------
	void ExtractDataTableInfo();
	
	void SearchAndSetFirstIngredient();
	void SearchAndSetFirstRange();
	void SearchAndSetFirstMelee();
	void ObtainCauldronVFX();
	void UpdateCauldronVFX();
	void HideCauldronVFX();
	void GrillAmmoChanged(const FOnAttributeChangeData& Data) const;
	void BoilAmmoChanged(const FOnAttributeChangeData& Data) const;
	void FryAmmoChanged(const FOnAttributeChangeData& Data) const;

	bool FirstExecution = true;
};


	

