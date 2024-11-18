// TinyTavern


#include "Statue.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/Statue/StatueUI.h"
#include "TinyTavernGame/UI/Menus/Buttons/InventorySlot.h"


// Sets default values
AStatue::AStatue()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatueTrigger = CreateDefaultSubobject<UBoxComponent>("Statue Trigger");
	RootComponent = StatueTrigger.Get();

	StatueAudio = CreateDefaultSubobject<UAudioComponent>("Statue Audio Sound");
	StatueAudio.Get()->SetupAttachment(StatueTrigger.Get());

	StatueMesh = CreateDefaultSubobject<UStaticMeshComponent>("Statue Mesh");
	StatueMesh.Get()->SetupAttachment(StatueTrigger.Get());

	StatueDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Statue Door Mesh");
	StatueDoorMesh.Get()->SetupAttachment(StatueTrigger.Get());

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>("Interact Widget");
	InteractWidget.Get()->SetupAttachment(StatueTrigger.Get());
}

// Called when the game starts or when spawned
void AStatue::BeginPlay()
{
	Super::BeginPlay();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	StatueBlend.SetBlendTime(AnimationDoorTime);
	InteractWidget.Get()->SetVisibility(false);
	if(CheckIfShouldBeEnabled())
	{
		StatueTrigger.Get()->OnComponentBeginOverlap.AddDynamic(this, &AStatue::OnOverlapBegin);
		StatueTrigger.Get()->OnComponentEndOverlap.AddDynamic(this, &AStatue::OnEndOverlap);
	}
	StatueDoorInitialPosition = StatueDoorMesh.Get()->GetRelativeLocation();
}

void AStatue::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	StatueTrigger.Get()->OnComponentBeginOverlap.RemoveAll(this);
	StatueTrigger.Get()->OnComponentEndOverlap.RemoveAll(this);
	if(StatueUI)
	{
		StatueUI.Get()->RemoveFromParent();
	}
	if(PlayerController)
	{
		PlayerController.Get()->OnInteract.RemoveAll(this);
	}
	if(PlayerHasInteracted)
	{
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
		Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	}
}

void AStatue::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		InteractWidget.Get()->SetVisibility(true);
		PlayerController = Cast<ATTPlayerController>(Player.Get()->GetController());
		PlayerController.Get()->OnInteract.RemoveAll(this);
		PlayerController.Get()->OnInteract.AddDynamic(this, &AStatue::PlayerInteracted);
	}
}

void AStatue::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		InteractWidget.Get()->SetVisibility(false);
		if(PlayerController)
		{
			if(StatueUI)
			{
				StatueUI.Get()->RemoveFromParent();
			}
			PlayerController.Get()->OnInteract.RemoveAll(this);
		}
	}
}

// Called every frame
void AStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(StatueHasBeenCompleted)
	{
		StatueBlend.Update(DeltaTime);
		float newLocationZ = (StatueDoorInitialPosition.Z + StatueDoorInitialPosition.Z * StatueBlend.GetBlendedValue()) * 0.95f;
		StatueDoorMesh.Get()->SetRelativeLocation(FVector(StatueDoorMesh.Get()->GetRelativeLocation().X,StatueDoorMesh.Get()->GetRelativeLocation().Y, newLocationZ));
		if(StatueBlend.IsComplete())
		{
			StatueAudio.Get()->Stop();
			StatueHasBeenCompleted = false;
			StatueDoorMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AStatue::PlayerInteracted()
{
	PlayerController.Get()->OnInteract.RemoveAll(this);
	InteractWidget.Get()->SetVisibility(false);
	PlayerHasInteracted = true;
	StatueUI = CreateWidget<UStatueUI>(PlayerController, StatueUIWidget);
	StatueUI.Get()->ActivateWidget();
	StatueUI.Get()->AddToViewport(-1);
	StatueUI.Get()->SetStatue(StatueID,this, StatueRiddle);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	
	Player.Get()->GetCharacterMovement()->DisableMovement();
	if(!Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(true);
	}
}

bool AStatue::CheckIfShouldBeEnabled()
{
	//Checks if player already discovered this statue
	if(TTGameInstance.Get()->StatuesOpened[StatueID])
	{
		StatueBlend.SetBlendTime(0.1f);
		StatueBlend.Reset();
		StatueHasBeenCompleted = true;
		InteractWidget.Get()->SetVisibility(false);
		StatueTrigger.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BP_StatueAlreadyOpened();
		return false;
	}
	return true;
}

void AStatue::StatueCompleted()
{
	StatueTrigger.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StatueUI.Get()->RemoveFromParent();
	StatueUI = nullptr;
	StatueHasBeenCompleted = true;
	StatueAudio.Get()->SetSound(StatueOpeningSound);
	StatueAudio.Get()->Play();
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	TTGameInstance.Get()->StatuesOpened[StatueID] = true;
	//Achievement of solving your first Statue
	TTGameInstance.Get()->OpenAStatue();
	BP_StatueCompleted();
	Player.Get()->GetWeaponComponent()->InitWeaponManager();
	if(Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(false);
	}
}

void AStatue::StatueExit()
{
	InteractWidget.Get()->SetVisibility(true);
	StatueUI.Get()->RemoveFromParent();
	StatueUI = nullptr;
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	PlayerController.Get()->OnInteract.AddDynamic(this, &AStatue::PlayerInteracted);
	if(Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(false);
	}
}
