// TinyTavern


#include "GetPoisonForKing.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/PoisonKingUI.h"
#include "TinyTavernGame/UI/Menus/TT_CommonActivatableWidget.h"


// Sets default values
AGetPoisonForKing::AGetPoisonForKing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CircleCollision = CreateDefaultSubobject<USphereComponent>("Poison Collision Sphere");
	RootComponent = CircleCollision.Get();

	PoisonKingPopUpWidget = CreateDefaultSubobject<UWidgetComponent>("Narrative Widget");
	PoisonKingPopUpWidget.Get()->SetupAttachment(CircleCollision.Get());
}

// Called when the game starts or when spawned
void AGetPoisonForKing::BeginPlay()
{
	Super::BeginPlay();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	if(TTGameInstance.Get()->PoisonRoyalPlate)
	{
		Destroy();
	}
	else
	{
		CircleCollision.Get()->OnComponentBeginOverlap.AddDynamic(this, &AGetPoisonForKing::OnOverlapBegin);
		CircleCollision.Get()->OnComponentEndOverlap.AddDynamic(this, &AGetPoisonForKing::OnEndOverlap);
	}
}

void AGetPoisonForKing::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	CircleCollision.Get()->OnComponentBeginOverlap.RemoveAll(this);
	CircleCollision.Get()->OnComponentEndOverlap.RemoveAll(this);
	if(PoisonKingUI)
	{
		PoisonKingUI.Get()->RemoveFromParent();
	}
	if(PlayerController)
	{
		PlayerController.Get()->OnInteract.RemoveAll(this);
	}
	if(PlayerHasInteracted)
	{
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Player)->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Player_Tutorial);
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Player)->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	}
}

void AGetPoisonForKing::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		PoisonKingPopUpWidget.Get()->SetVisibility(true);
		PlayerInsideTrigger = true;
		PlayerController = Cast<ATTPlayerController>(Player.Get()->GetController());
		if(PlayerController)
		{
			PlayerController.Get()->OnInteract.RemoveAll(this);
			PlayerController.Get()->OnInteract.AddDynamic(this, &AGetPoisonForKing::PlayerInteracted);
		}
	}
}

void AGetPoisonForKing::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		PlayerInsideTrigger = false;
		if(PlayerController)
		{
			PoisonKingPopUpWidget.Get()->SetVisibility(false);
			if(PoisonKingUI)
			{
				PoisonKingUI.Get()->RemoveFromParent();
			}
			PlayerController.Get()->OnInteract.RemoveAll(this);
		}
	}
}

void AGetPoisonForKing::PlayerInteracted()
{
	PlayerController.Get()->OnInteract.RemoveAll(this);
	PlayerController.Get()->Pause();
	PoisonKingPopUpWidget.Get()->SetVisibility(false);
	PlayerHasInteracted = true;
	PoisonKingUI = CreateWidget<UPoisonKingUI>(GetWorld(), PoisonKingUIClass);
	PoisonKingUI.Get()->ActivateWidget();
	PoisonKingUI.Get()->AddToViewport(-1);
	PoisonKingUI.Get()->SetPoisonForKingSphere(this);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().Events_Player_Tutorial);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	Player.Get()->GetCharacterMovement()->DisableMovement();
	if(!Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(true);
	}
}

void AGetPoisonForKing::PoisonObtained()
{
	PlayerController.Get()->Pause();
	TTGameInstance.Get()->PoisonRoyalPlate = true; //TODO Remove
	TTGameInstance.Get()->PlayerDecidedToPoisonTheKing = true;
	PoisonKingUI.Get()->RemoveFromParent();
	PoisonKingUI = nullptr;
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Player_Tutorial);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	if(Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(false);
	}
}

void AGetPoisonForKing::PoisonDenied()
{
	PlayerController.Get()->Pause();
	PoisonKingUI.Get()->RemoveFromParent();
	PoisonKingUI = nullptr;
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Player_Tutorial);
    Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
    Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
    Player.Get()->GetCharacterMovement()->SetDefaultMovementMode();
	PoisonKingPopUpWidget.Get()->SetVisibility(true);
	PlayerController.Get()->OnInteract.AddDynamic(this, &AGetPoisonForKing::PlayerInteracted);
	if(Player.Get()->GetHUDIsHidden())
	{
		Player.Get()->SetHUDVisibility(false);
	}
}
