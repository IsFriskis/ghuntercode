// TinyTavern


#include "Chest.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/Chest/ChestUI.h"


// Sets default values
AChest::AChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChestTrigger = CreateDefaultSubobject<UBoxComponent>("Chest Trigger");
	RootComponent = ChestTrigger.Get();

	ChestAudio = CreateDefaultSubobject<UAudioComponent>("Chest Audio Sound");
	ChestAudio.Get()->SetupAttachment(ChestTrigger.Get());

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("Chest Mesh");
	ChestMesh.Get()->SetupAttachment(ChestTrigger.Get());
	
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>("Chest Interact Widget");
	InteractWidget.Get()->SetupAttachment(ChestTrigger.Get());
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	InteractWidget.Get()->SetVisibility(false);
	if(CheckIfShouldBeEnabled())
	{
		ChestTrigger.Get()->OnComponentBeginOverlap.AddDynamic(this, &AChest::OnOverlapBegin);
		ChestTrigger.Get()->OnComponentEndOverlap.AddDynamic(this, &AChest::OnEndOverlap);
	}
}

void AChest::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ChestTrigger.Get()->OnComponentBeginOverlap.RemoveAll(this);
	ChestTrigger.Get()->OnComponentEndOverlap.RemoveAll(this);
	if(ChestUI)
	{
		ChestUI.Get()->RemoveFromParent();
	}
	if(PlayerController)
	{
		PlayerController.Get()->OnInteract.RemoveAll(this);
	}
	if(PlayerHasInteracted)
	{
		Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Player_Tutorial);
		Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	}
}

void AChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		InteractWidget.Get()->SetVisibility(true);
		PlayerController = Cast<ATTPlayerController>(Player.Get()->GetController());
		PlayerController.Get()->OnInteract.RemoveAll(this);
		PlayerController.Get()->OnInteract.AddDynamic(this, &AChest::PlayerInteracted);
	}
}

void AChest::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		InteractWidget.Get()->SetVisibility(false);
		if(PlayerController)
		{
			if(ChestUI)
			{
				ChestUI.Get()->RemoveFromParent();
			}
			PlayerController.Get()->OnInteract.RemoveAll(this);
		}
	}
}

void AChest::ShowChestUI()
{
	if(Player)
	{
		Player.Get()->GetCharacterMovement()->DisableMovement();
	}
	ChestUI = CreateWidget<UChestUI>(GetWorld(), ChestUIWidget);
	ChestUI.Get()->AddToViewport(-1);
	ChestUI.Get()->ActivateWidget();
	ChestUI.Get()->SetChestRelic(this, RelicObtained);
	if(!Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(true);
	}
}

void AChest::PlayerInteracted()
{
	if(Player && !Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_WeaponsBlocked))
	{
		PlayerController.Get()->OnInteract.RemoveAll(this);
		InteractWidget.Get()->SetVisibility(false);
		PlayerHasInteracted = true;
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		BP_ChestFinished();
	}
}

bool AChest::CheckIfShouldBeEnabled()
{
	//Checks if player already opened this chest
	if(TTGameInstance.Get()->ChestOpened[ChestOpenedNPCValue() + (RelicObtained.NumberOfRelic-1)])
	{
		InteractWidget.Get()->SetVisibility(false);
		ChestTrigger.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ChestMesh.Get()->SetStaticMesh(ChestOpenMesh);
		BP_ChestAlreadyOpened();
		return false;
	}
	return true;
}

void AChest::ChestFinished()
{
	ChestTrigger.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChestUI.Get()->RemoveFromParent();
	ChestUI = nullptr;
	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	TTGameInstance.Get()->ChestOpened[ChestOpenedNPCValue() + (RelicObtained.NumberOfRelic-1)] = true;
	TTGameInstance.Get()->RelicsInventory[RelicObtained.RelicType] = TTGameInstance.Get()->RelicsInventory[RelicObtained.RelicType] + 1;
	Player.Get()->StopAnimMontage(ChestMontage);
	Player.Get()->GetWeaponComponent()->ChangeWeaponToRange();
	if(Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(false);
	}
}

int AChest::ChestOpenedNPCValue()
{
	switch(RelicObtained.RelicType)
	{
	case SACREDSPORES:
		return 0;
	case FUNGAlSILK:
		return 3;
	case GMITHRELIC:
		return 7;
	default: ;
	}
	return 0;
}

void AChest::BP_ShowRelic()
{
	switch(RelicObtained.RelicType)
	{
	case SACREDSPORES:
		Player.Get()->WeaponStaticMesh->SetStaticMesh(PouchofTheDamselsSporesMesh);
		break;
	case FUNGAlSILK:
		Player.Get()->WeaponStaticMesh->SetStaticMesh(ZerroleanSilkMesh);
		break;
	case GMITHRELIC:
		Player.Get()->WeaponStaticMesh->SetStaticMesh(GungstenBarMesh);
		break;
	default: ;
	}
	Player.Get()->WeaponStaticMesh->SetHiddenInGame(false);
	Player->WeaponStaticMesh.Get()->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"Chest_Socket");
}
