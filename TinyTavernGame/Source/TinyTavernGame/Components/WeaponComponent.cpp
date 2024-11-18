// TinyTavern


#include "WeaponComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Weapons/Data/WeaponDataTable.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Weapons/Ingredients/Jam/JamSlamAbility.h"
#include "TinyTavernGame/Weapons/Ingredients/Mine/CheeseMineAbility.h"
#include "TinyTavernGame/Weapons/Ingredients/Wine/WineAbility.h"
#include "TinyTavernGame/Weapons/MeleeDash/MeleeDashAbility.h"
#include "TinyTavernGame/Weapons/QTE/FinisherAbility.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	FirstExecution = true;
	
	PlayerRef = Cast<APlayerCharacter>(GetOwner());
	TTAttributeSet = Cast<UTT_AttributeSet>(PlayerRef.Get()->GetAttributeSet());
	TTGameInstance = Cast<UTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	InitWeaponManager();

	//Init guns in selected slots
	SearchAndSetFirstMelee();
	SearchAndSetFirstRange();
	SearchAndSetFirstIngredient();


	// BIND ACTIONS TO FUNCTIONS
	TObjectPtr<ATTPlayerController> ttPlayerController = Cast<ATTPlayerController>(PlayerRef->GetController());
	ttPlayerController->OnHoldMelee.AddDynamic(this, &UWeaponComponent::ShootMelee);
	ttPlayerController->OnRange.AddDynamic(this, &UWeaponComponent::ShootRange);
	ttPlayerController->OnHoldIngredient.AddDynamic(this, &UWeaponComponent::ShootIngredient);

	ttPlayerController->OnRangeWeaponChange.AddDynamic(this, &UWeaponComponent::ChangeRangeWeapon);
	ttPlayerController->OnIngredientWeaponChange.AddDynamic(this, &UWeaponComponent::ChangeIngredientWeapon);

	PlayerRef.Get()->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		TTAttributeSet->GetGrilledAmmoAttribute()).AddUObject(this, &UWeaponComponent::GrillAmmoChanged);
	PlayerRef.Get()->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		TTAttributeSet->GetBoiledAmmoAttribute()).AddUObject(this, &UWeaponComponent::BoilAmmoChanged);
	PlayerRef.Get()->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		TTAttributeSet->GetFriedAmmoAttribute()).AddUObject(this, &UWeaponComponent::FryAmmoChanged);
	
	
	ObtainCauldronVFX();
	ChangeWeaponToRange();
	FirstExecution = false;
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	PlayerRef.Get()->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		TTAttributeSet->GetGrilledAmmoAttribute()).RemoveAll(this);
	PlayerRef.Get()->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		TTAttributeSet->GetBoiledAmmoAttribute()).RemoveAll(this);
	PlayerRef.Get()->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		TTAttributeSet->GetFriedAmmoAttribute()).RemoveAll(this);
}

void UWeaponComponent::InitWeaponManager()
{
	WeaponsData.Empty();
	
	if(TTGameInstance)
	{
		for(TTuple<FGameplayTag, bool> Tuple : TTGameInstance->UnlockedWeapons)
		{
			FName RowName = FName(*Tuple.Key.ToString());
			FWeaponDatableRowBase* WeaponData = WeaponDataTable->FindRow<FWeaponDatableRowBase>(RowName, TEXT("InitWeaponManager"));
			if(WeaponData)
			{
				WeaponData->isUnlocked = Tuple.Value;
			}
		}
		for(TTuple<FGameplayTag, TEnumAsByte<FWeaponsLevel>> Tuple : TTGameInstance->WeaponsLevel)
		{
			FName RowName = FName(*Tuple.Key.ToString());
			FWeaponDatableRowBase* WeaponData = WeaponDataTable->FindRow<FWeaponDatableRowBase>(RowName, TEXT("InitWeaponManager"));
			if(WeaponData)		
			{
				WeaponData->WeaponLevel = Tuple.Value;
			}
		}
	}
	OnWeaponUnlocked.Broadcast();
	//For having easier access to the DataTable
	ExtractDataTableInfo();
}

void UWeaponComponent::ExtractDataTableInfo()
{
	TArray<FName> RowNames = WeaponDataTable->GetRowNames();

	for(const FName& RowName : RowNames)
	{
		FWeaponDatableRowBase* DataRow = WeaponDataTable->FindRow<FWeaponDatableRowBase>(RowName, "");
		WeaponsData.Add(*DataRow);
	}
}

void UWeaponComponent::SearchAndSetFirstIngredient()
{
	unsigned int index = 0;

	//TODO: Should be easier way to check if tag is Weapon_Ingredient
	
	for(FWeaponDatableRowBase Weapon : WeaponsData)
	{
		if((Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Ingredient_Jam.GetSingleTagContainer()) ||
			Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Ingredient_Cheese.GetSingleTagContainer()) ||
				Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Ingredient_Wine.GetSingleTagContainer())) && Weapon.isUnlocked && Weapon.WeaponTag != WeaponsData[CurrentRangeWeapon].WeaponTag)
		{
			CurrentIngredientWeapon = index;
			OnWeaponChange.Broadcast(Weapon.WeaponTag);
			if(!FirstExecution)
			{
				TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
				FGameplayEffectContextHandle Taq = TargetAsc.Get()->MakeEffectContext();
				TargetAsc.Get()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Ingredient_Change, Taq);
			}
			break;
		}
		else
		{
			index++;
		}
	}
}

void UWeaponComponent::SearchAndSetFirstRange()
{
	unsigned int index = 0;


	//TODO: Should be easier way to check if tag is Weapon_Range
	for(FWeaponDatableRowBase Weapon : WeaponsData)
	{ 
		if(
				(
					Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Range_Boil.GetSingleTagContainer()) ||
					Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Range_Fry.GetSingleTagContainer()) ||
					Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Range_Grill.GetSingleTagContainer())
				)
				&& Weapon.isUnlocked
				&& Weapon.WeaponTag != WeaponsData[CurrentRangeWeapon].WeaponTag
			)
		{
			
			CurrentRangeWeapon = index;
			OnWeaponChange.Broadcast(Weapon.WeaponTag);
			if(!FirstExecution)
			{
				TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
				FGameplayEffectContextHandle Taq = TargetAsc.Get()->MakeEffectContext();
				TargetAsc.Get()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Range_Change, Taq);
			}
			break;
		}
		else
		{
			index++;
		}
	}
}

void UWeaponComponent::SearchAndSetFirstMelee()
{
	unsigned int index = 0;
	
	//TODO: Should be easier way to check if tag is Weapon_Range
	for(FWeaponDatableRowBase Weapon : WeaponsData)
	{
		if(Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Melee.GetSingleTagContainer()) && Weapon.isUnlocked)
		{
			
			MeleeWeapon = Weapon.WeaponTag;
			break;
		}
		else
		{
			index++;
		}
	}
}

void UWeaponComponent::ObtainCauldronVFX()
{
	GrillCauldronVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(GrillNiagaraSystem.Get(), PlayerRef.Get()->WeaponSkeletalMesh.Get(), "CauldronSocket",
				FVector::Zero(),
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset, false);
	GrillCauldronVFX.Get()->Deactivate();
	BoilCauldronVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(BoilNiagaraSystem.Get(), PlayerRef.Get()->WeaponSkeletalMesh.Get(), "CauldronSocket",
				FVector::Zero(),
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset, false);
	BoilCauldronVFX.Get()->Deactivate();
	FryCauldronVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(FryNiagaraSystem.Get(), PlayerRef.Get()->WeaponSkeletalMesh.Get(), "CauldronSocket",
				FVector::Zero(),
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset, false);
	FryCauldronVFX.Get()->Deactivate();
}

void UWeaponComponent::UpdateCauldronVFX()
{
	//VFX of Cauldron if ammunition
	if(GetWeaponDataByTag(GetCurrentRangeWeaponTag()).WeaponTag == FGameplayTags::Get().Weapon_Range_Grill)
	{
		CurrentCauldronVFXNumber = 0;
		BoilCauldronVFX.Get()->Deactivate();
		FryCauldronVFX.Get()->Deactivate();
		if(TTAttributeSet.Get()->GetGrilledAmmo() > 0)
		{
			GrillCauldronVFX.Get()->Activate();
		}
		else
		{
			GrillCauldronVFX.Get()->Deactivate();
		}
	}
	else if(GetWeaponDataByTag(GetCurrentRangeWeaponTag()).WeaponTag == FGameplayTags::Get().Weapon_Range_Boil)
	{
		CurrentCauldronVFXNumber = 1;
		GrillCauldronVFX.Get()->Deactivate();
		FryCauldronVFX.Get()->Deactivate();
		if(TTAttributeSet.Get()->GetBoiledAmmo() > 0)
		{
			BoilCauldronVFX.Get()->Activate();
		}
		else
		{
			BoilCauldronVFX.Get()->Deactivate();
		}
	}
	else if(GetWeaponDataByTag(GetCurrentRangeWeaponTag()).WeaponTag == FGameplayTags::Get().Weapon_Range_Fry)
	{
		CurrentCauldronVFXNumber = 2;
		GrillCauldronVFX.Get()->Deactivate();
		BoilCauldronVFX.Get()->Deactivate();
		if(TTAttributeSet.Get()->GetFriedAmmo() > 0)
		{
			FryCauldronVFX.Get()->Activate();
		}
		else
		{
			FryCauldronVFX.Get()->Deactivate();
		}
	}
}

void UWeaponComponent::HideCauldronVFX()
{
	if(GrillCauldronVFX)
	{
		GrillCauldronVFX.Get()->Deactivate();
	}
	if(BoilCauldronVFX)
	{
		BoilCauldronVFX.Get()->Deactivate();
	}
	if(FryCauldronVFX)
	{
		FryCauldronVFX.Get()->Deactivate();
	}
}

void UWeaponComponent::GrillAmmoChanged(const FOnAttributeChangeData& Data) const
{
	if(CurrentCauldronVFXNumber == 0)
	{
		if(Data.NewValue <= 0)
		{
			GrillCauldronVFX.Get()->Deactivate();
		}
		else if(!GrillCauldronVFX.Get()->IsActive())
		{
			GrillCauldronVFX.Get()->Activate();
		}
	}
}

void UWeaponComponent::BoilAmmoChanged(const FOnAttributeChangeData& Data) const
{
	if(CurrentCauldronVFXNumber == 1)
	{
		if(Data.NewValue <= 0)
		{
			BoilCauldronVFX.Get()->Deactivate();
		}
		else if(!BoilCauldronVFX.Get()->IsActive())
		{
			BoilCauldronVFX.Get()->Activate();
		}
	}
}

void UWeaponComponent::FryAmmoChanged(const FOnAttributeChangeData& Data) const
{
	if(CurrentCauldronVFXNumber == 2)
	{
		if(Data.NewValue <= 0)
		{
			FryCauldronVFX.Get()->Deactivate();
		}
		else if(!FryCauldronVFX.Get()->IsActive())
		{
			FryCauldronVFX.Get()->Activate();
		}
	}
}

FGameplayTag UWeaponComponent::GetCurrentRangeWeaponTag()
{
	return WeaponsData[CurrentRangeWeapon].WeaponTag;
}

FGameplayTag UWeaponComponent::GetCurrentIngredientWeaponTag()
{
	return WeaponsData[CurrentIngredientWeapon].WeaponTag;
}

void UWeaponComponent::ChangeRangeWeapon()
{
	unsigned int index = 0;

	if (PlayerRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_WeaponsBlocked))
	{
		return;
	}


	bool hasChanged = false;

	//Search in all array of WeaponsData for the next Range Weapon
	for(FWeaponDatableRowBase Weapon : WeaponsData)
	{
		//TODO: Change for weapon_matches fucntion
		//If is weapon type
		if((Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Range_Boil.GetSingleTagContainer()) ||
			Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Range_Fry.GetSingleTagContainer()) ||
				Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Range_Grill.GetSingleTagContainer())) && Weapon.isUnlocked)
		{
			//And is greater than the current one
			if(index > CurrentRangeWeapon)
			{
				//Update Current Weapon
				OnWeaponChange.Broadcast(Weapon.WeaponTag);
				CurrentRangeWeapon = index;
				hasChanged = true;
				if (!FirstExecution)
				{
					TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
					FGameplayEffectContextHandle Taq = TargetAsc.Get()->MakeEffectContext();
					TargetAsc.Get()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Range_Change, Taq);
				}
				break;
			}
			else
			{
				index++;
			}
		}
		else
		{
			index++;
		}
	}
	//If no weapon was found, search for the first one.
	//It only happens when the last weapon is the selected one
	if(!hasChanged)
	{
		SearchAndSetFirstRange();
	}
	UpdateCauldronVFX();
}

void UWeaponComponent::ChangeIngredientWeapon()
{
	unsigned int index = 0;
	
	bool hasChanged = false;
	TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
	//Search in all array of WeaponsData for the next Ingredient Weapon

	if(TargetAsc->HasMatchingGameplayTag(FGameplayTags::Get().States_WeaponsBlocked))
	{
		return;
	}
	for(FWeaponDatableRowBase Weapon : WeaponsData)
	{
		//If is weapon type
		if((Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Ingredient_Jam.GetSingleTagContainer()) ||
			(Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Ingredient_Cheese.GetSingleTagContainer()) ||
				(Weapon.WeaponTag.MatchesAny(FGameplayTags::Get().Weapon_Ingredient_Wine.GetSingleTagContainer())))) && Weapon.isUnlocked)
		{
			//And is greater than the current one
			if(index > CurrentIngredientWeapon)
			{
				//Update Current Weapon
				OnWeaponChange.Broadcast(Weapon.WeaponTag);
				CurrentIngredientWeapon = index;
				hasChanged = true;
				FGameplayEffectContextHandle Taq = TargetAsc.Get()->MakeEffectContext();
				TargetAsc.Get()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Ingredient_Change, Taq);
				break;
			}
			else
			{
				index++;
			}
		}
		else
		{
			//If is not the weapon type, increment index
			index++;
		}
	}
	//If no weapon was found, search for the first one.
	//It only happens when the last weapon is the selected one
	if(!hasChanged)
	{
		SearchAndSetFirstIngredient();
	}
}

void UWeaponComponent::ShootMelee()
{
	FGameplayEventData Payload;
	TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
	if(!TargetAsc->HasMatchingGameplayTag(FGameplayTags::Get().States_HitReact) && !TargetAsc->HasMatchingGameplayTag(FGameplayTags::Get().States_Dead))
	{
		for (FWeaponDatableRowBase Weapon : WeaponsData)
		{
			if(Weapon.WeaponTag.MatchesTag(MeleeWeapon) && Weapon.isUnlocked)
			{
				//ChangeWeaponForDash(); is done in the ability, because it first cancels
				//other weapon ability(which change to WeaponRange), and then it changes to
				//the melee weapon
				Payload.EventMagnitude = static_cast<float>(Weapon.WeaponLevel.GetValue());
				TargetAsc.Get()->HandleGameplayEvent(Weapon.AbilityTag, &Payload);
				TargetAsc.Get()->OnAbilityEnded.AddUObject(this, &UWeaponComponent::OnAbilityEnded);
				break;
			}
		}
	}
}

void UWeaponComponent::ShootRange()
{
	if(bIsIngredientActive)
	{
		Cast<APlayerCharacter>(GetOwner())->GetAbilitySystemComponent()->AbilityLocalInputReleased(1);
	} else
	{
		FGameplayEventData Payload;
		TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
		if(!TargetAsc->HasMatchingGameplayTag(FGameplayTags::Get().States_WeaponsBlocked))
		{
			ChangeToRangeWeaponMesh();
			Payload.EventMagnitude = WeaponsData[CurrentRangeWeapon].WeaponLevel;
			TargetAsc.Get()->HandleGameplayEvent(WeaponsData[CurrentRangeWeapon].AbilityTag, &Payload);
			TargetAsc.Get()->OnAbilityEnded.AddUObject(this, &UWeaponComponent::OnAbilityEnded);
		}
	}
}

void UWeaponComponent::ShootIngredient()
{
	FGameplayEventData Payload;
    TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
	if(!TargetAsc->HasMatchingGameplayTag(FGameplayTags::Get().States_WeaponsBlocked) && CurrentIngredientWeapon != 0)
	{
		bIsIngredientActive = true;
		ChangeToIngredientWeaponMesh();
		Payload.EventMagnitude = WeaponsData[CurrentIngredientWeapon].WeaponLevel;
		TargetAsc.Get()->HandleGameplayEvent(WeaponsData[CurrentIngredientWeapon].AbilityTag, &Payload);
		TargetAsc.Get()->OnAbilityEnded.AddUObject(this, &UWeaponComponent::OnAbilityEnded);
	}
}

//Access DataTable to obtain Weapon's data with Tag
FWeaponDatableRowBase UWeaponComponent::GetWeaponDataByTag(const FGameplayTag WeaponTag)
{
	TArray<FName> RowNames = WeaponDataTable->GetRowNames();

	for(const FName& RowName : RowNames)
	{
		FWeaponDatableRowBase* WeaponData = WeaponDataTable->FindRow<FWeaponDatableRowBase>(RowName, "");
		if(WeaponData && WeaponData->WeaponTag == WeaponTag)
		{
			return *WeaponData;
		}
	}
	return FWeaponDatableRowBase();
}


void UWeaponComponent::SetWeapon(FGameplayTag WeaponTag)
{
	for(FWeaponDatableRowBase Weapon : WeaponsData)
	{
		if(Weapon.WeaponTag.MatchesTag(WeaponTag))
		{

			OnWeaponChange.Broadcast(Weapon.WeaponTag);

			//Attach Weapon to Player.
			//Player->WeaponActor->SetChildActorClass(WeaponData.WeaponActor);
			//Player->WeaponActor->SetChildActorClass(Weapon.WeaponActor);
			PlayerRef->WeaponSkeletalMesh->SetSkeletalMesh(Weapon.WeaponMesh);
			PlayerRef->WeaponSkeletalMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerRef->WeaponSkeletalMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Weapon-Socket");
			//Audio for change Weapon
			PlayerRef->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Weapon_Change);
		}
	}
}
void UWeaponComponent::ChangeWeaponForDash()
{
	ClearWeapons();
	for (FWeaponDatableRowBase Weapon : WeaponsData)
	{
		if(Weapon.WeaponTag.MatchesTag(MeleeWeapon))
		{
			PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(true);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(false);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetStaticMesh(Weapon.WeaponStaticMesh[Weapon.WeaponLevel]);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerRef.Get()->WeaponStaticMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Dash_Socket");
			HideCauldronVFX();
			break;
		}
	}
}

void UWeaponComponent::ChangeWeaponToQTE()
{
	ClearWeapons();
	for (FWeaponDatableRowBase Weapon : WeaponsData)
	{
		if(Weapon.WeaponTag.MatchesTag(MeleeWeapon))
		{
			PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(true);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(false);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetStaticMesh(Weapon.WeaponStaticMesh[Weapon.WeaponLevel]);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerRef.Get()->WeaponStaticMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Dash_Socket");
			HideCauldronVFX();
			break;
		}
	}
	
}

void UWeaponComponent::ChangeWeaponToRange()
{
	ClearWeapons();
	PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(true);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(false);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetSkeletalMesh(GetWeaponDataByTag(GetCurrentRangeWeaponTag()).WeaponMesh);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Weapon-Socket");
	UpdateCauldronVFX();
}

void UWeaponComponent::ChangeToMeleeWeaponMesh()
{
	for (FWeaponDatableRowBase Weapon : WeaponsData)
	{
		if(Weapon.WeaponTag.MatchesTag(MeleeWeapon))
		{
			PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(true);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(false);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetStaticMesh(Weapon.WeaponStaticMesh[Weapon.WeaponLevel]);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerRef.Get()->WeaponStaticMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Weapon-Socket");
			break;
		}
	}
}

void UWeaponComponent::ClearWeapons()
{
	PlayerRef.Get()->WeaponSkeletalMesh->SetSkeletalMesh(nullptr);
	PlayerRef.Get()->WeaponStaticMesh->SetStaticMesh(nullptr);
	HideCauldronVFX();
}

void UWeaponComponent::ChangeToRangeWeaponMesh()
{
	PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(true);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(false);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetSkeletalMesh(GetWeaponDataByTag(GetCurrentRangeWeaponTag()).WeaponMesh);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerRef.Get()->WeaponSkeletalMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Weapon-Socket");
	UpdateCauldronVFX();
}


void UWeaponComponent::ChangeToIngredientWeaponMesh()
{
	if(GetCurrentIngredientWeaponTag() == FGameplayTags::Get().Weapon_Ingredient_Jam)
	{
		if(Cast<UTT_AttributeSet>(PlayerRef->GetAttributeSet())->GetJamAmmo() == 0)
		{
			
		}
		else
		{
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(false);
			PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(true);
			//PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetSkeletalMesh(GetWeaponDataByTag(GetCurrentIngredientWeaponTag()).WeaponMesh);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetStaticMesh(GetWeaponDataByTag(GetCurrentIngredientWeaponTag()).WeaponStaticMesh[0]);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerRef.Get()->WeaponStaticMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Melee-Socket");
			HideCauldronVFX();
		}
	} else if (GetCurrentIngredientWeaponTag() == FGameplayTags::Get().Weapon_Ingredient_Cheese)
	{
		if(Cast<UTT_AttributeSet>(PlayerRef->GetAttributeSet())->GetCheeseAmmo() == 0)
		{
			
		}
		else
		{
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(false);
			PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(true);
			//PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetSkeletalMesh(GetWeaponDataByTag(GetCurrentIngredientWeaponTag()).WeaponMesh);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetStaticMesh(GetWeaponDataByTag(GetCurrentIngredientWeaponTag()).WeaponStaticMesh[0]);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerRef.Get()->WeaponStaticMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Melee-Socket");
			HideCauldronVFX();
		}
	} else if (GetCurrentIngredientWeaponTag() == FGameplayTags::Get().Weapon_Ingredient_Wine)
	{
		if(Cast<UTT_AttributeSet>(PlayerRef->GetAttributeSet())->GetWineAmmo() == 0)
		{
			
		}
		else
		{
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetHiddenInGame(false);
			PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetHiddenInGame(true);
			//PlayerRef.Get()->WeaponSkeletalMesh.Get()->SetSkeletalMesh(GetWeaponDataByTag(GetCurrentIngredientWeaponTag()).WeaponMesh);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetStaticMesh(GetWeaponDataByTag(GetCurrentIngredientWeaponTag()).WeaponStaticMesh[0]);
			PlayerRef.Get()->WeaponStaticMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlayerRef.Get()->WeaponStaticMesh.Get()->AttachToComponent(PlayerRef->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Melee-Socket");
			HideCauldronVFX();
		}
	}
	
}

void UWeaponComponent::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if(AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UMeleeDashAbility::StaticClass()) ||
		AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UCheeseMineAbility::StaticClass()) ||
		AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UJamSlamAbility::StaticClass()) ||
		AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UWineAbility::StaticClass()) ||
		AbilityEndedData.AbilityThatEnded.GetClass()->IsChildOf(UFinisherAbility::StaticClass()))
	{
		bIsIngredientActive = false;
		TObjectPtr<UAbilitySystemComponent> TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerRef);
		TargetAsc.Get()->OnAbilityEnded.RemoveAll(this);
		ChangeWeaponToRange();
	}
}
