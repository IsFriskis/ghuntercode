// TinyTavern


#include "Lever.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"


// Sets default values
ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeverTrigger = CreateDefaultSubobject<UBoxComponent>("Lever Trigger");
	SetRootComponent(LeverTrigger.Get());
	LeverMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Lever Mesh");
	LeverMesh.Get()->SetupAttachment(LeverTrigger.Get());
	UsedLeverMesh = CreateDefaultSubobject<UStaticMeshComponent>("UsedLever Mesh");
	UsedLeverMesh.Get()->SetupAttachment(LeverTrigger.Get());
	UsedLeverMesh->SetVisibility(false);
	LeverAudio = CreateDefaultSubobject<UAudioComponent>("Lever Audio Sound");
	LeverAudio.Get()->SetupAttachment(LeverTrigger.Get());
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>("Interact Widget");
	InteractWidget.Get()->SetupAttachment(LeverTrigger.Get());

	MotionWarpingLocation = CreateDefaultSubobject<USceneComponent>("MotionWarping Location");
	MotionWarpingLocation.Get()->SetupAttachment(LeverTrigger.Get());

	Sound = false;
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	InteractWidget.Get()->SetVisibility(false);
	LeverTrigger.Get()->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnOverlapBegin);
	LeverTrigger.Get()->OnComponentEndOverlap.AddDynamic(this, &ALever::OnEndOverlap);
	for (int Id : TTGameInstance.Get()->UsedLevers)
	{
		if (Id == LeverID)
		{
			LeverMesh->SetOverlayMaterial(nullptr);
			LeverMesh->SetVisibility(false, false);
			UsedLeverMesh->SetVisibility(true);
			LeverTrigger.Get()->OnComponentBeginOverlap.RemoveAll(this);
			LeverTrigger.Get()->OnComponentEndOverlap.RemoveAll(this);
		}
	}
}

void ALever::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		if (!LeverUsed)
		{
			LeverMesh->SetOverlayMaterial(OverlayMaterial.Get());
		}
		InteractWidget.Get()->SetVisibility(true);
		PlayerController = Cast<ATTPlayerController>(Player.Get()->GetController());
		PlayerController.Get()->OnInteract.AddDynamic(this, &ALever::PlayerInteracted);
	}
}

void ALever::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		LeverMesh->SetOverlayMaterial(nullptr);
		InteractWidget.Get()->SetVisibility(false);
		PlayerController.Get()->OnInteract.RemoveAll(this);
	}
}

void ALever::PlayerInteracted()
{
	if (!Player.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_WeaponsBlocked))
	{
		PlayerController.Get()->OnInteract.RemoveAll(this);
		Player.Get()->GetWeaponComponent()->ClearWeapons();
		InteractWidget.Get()->SetVisibility(false);
		LeverMesh->SetOverlayMaterial(nullptr);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		if(!Sound)
		{
			LeverAudio.Get()->Activate();
			Sound = true;
		}


		FVector WarpingLocation = MotionWarpingLocation.Get()->GetComponentLocation();
		FRotator WarpingRotator = MotionWarpingLocation.Get()->GetComponentRotation();
		UMotionWarpingComponent* WarpingComponent = Player.Get()->GetWarpComponent();
		FMotionWarpingTarget WarpingTarget;
		WarpingTarget.Name = WarpTargerName;
		WarpingTarget.Location = WarpingLocation;
		WarpingTarget.Rotation = WarpingRotator;
		WarpingComponent->AddOrUpdateWarpTarget(WarpingTarget);
		Player.Get()->PlayAnimMontage(ActorInteractionAnimation);
		LeverMesh.Get()->PlayAnimation(LeverAnimation, false);

		
		// for (int i = 0; i < ActorInteractionAnimation->Notifies.Num(); i++)
		// {
		// 	// Notify = Cast<UTTAnimNotify>(ActorInteractionAnimation->Notifies[i].Notify.Get());
		// 	// if(Notify)
		// 	// {
		// 		if (ActorInteractionAnimation->Notifies[i].Notify.Get()->GetName() == "TTAnimNotify_0")
				// {
					Notify = Cast<UTTAnimNotify>(ActorInteractionAnimation->Notifies[1].Notify.Get());
					Notify->OnNotified.AddDynamic(this, &ALever::EndAnimation);
				// }
				// if (ActorInteractionAnimation->Notifies[i].Notify.Get()->GetName() == "TTAnimNotify_1")
				// {
					Notify2 = Cast<UTTAnimNotify>(ActorInteractionAnimation->Notifies[2].Notify.Get());
					Notify2->OnNotified.AddDynamic(this, &ALever::ShowWeapon);
				// }
			// }
		// }
	}
}

void ALever::EndAnimation()
{
	Notify->OnNotified.RemoveAll(this);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(
		FGameplayTags::Get().States_PlayerRecolection);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(
		FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(
		FGameplayTags::Get().States_AimBlocked);
	LeverUsedDelegate.Broadcast(LeverID);
	LeverTrigger.Get()->OnComponentBeginOverlap.RemoveAll(this);
	LeverTrigger.Get()->OnComponentEndOverlap.RemoveAll(this);
	LeverMesh->SetVisibility(false, false);
	UsedLeverMesh->SetVisibility(true);
	TTGameInstance.Get()->UsedLevers.Add(LeverID);
	LeverAudio.Get()->Stop();
}

void ALever::ShowWeapon()
{
	Player.Get()->GetWeaponComponent()->ChangeWeaponToRange();
	Notify2->OnNotified.RemoveAll(this);
}