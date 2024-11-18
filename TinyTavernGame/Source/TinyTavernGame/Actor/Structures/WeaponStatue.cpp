// TinyTavern


#include "WeaponStatue.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/Statue/StatueUI.h"
#include "TinyTavernGame/Weapons/Data/WeaponDataTable.h"


// Sets default values
AWeaponStatue::AWeaponStatue()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponStatue::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponStatue::PlayerInteracted()
{
	PlayerController.Get()->OnInteract.RemoveAll(this);
	InteractWidget.Get()->SetVisibility(false);
	PlayerHasInteracted = true;
	StatueUI = CreateWidget<UStatueUI>(GetWorld(), StatueUIWidget);
	StatueUI.Get()->AddToViewport(-1);
	//Player has the right melee or not. If so, show nice text, else don't show text
	if((WeaponUnlocked == FGameplayTags::Get().Weapon_Range_Boil &&
		TTGameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_CanObtainBoil])
		||
		(WeaponUnlocked == FGameplayTags::Get().Weapon_Range_Fry &&
		TTGameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_CanObtainFry]))
	{
		StatueUI.Get()->SetStatue(0,this, WeaponCanBeUnlockedText, false, WeaponUnlocked);
	}
	else
	{
		StatueUI.Get()->SetStatue(0,this, StatueRiddle, false, WeaponUnlocked);
	}
	
	StatueUI.Get()->ActivateWidget();
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	Player.Get()->GetCharacterMovement()->DisableMovement();
}

