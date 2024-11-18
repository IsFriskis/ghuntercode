// TinyTavern


#include "TPWithChoices.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/RecipeBook/Map/BookMap.h"
#include "TinyTavernGame/TinyTavernUtils.h"


// Sets default values
ATPWithChoices::ATPWithChoices()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
	
	ColliderForEntering = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderForEntering"));
	ColliderForEntering.Get()->SetupAttachment(RootComponent);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent.Get()->SetupAttachment(RootComponent);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractZone"));
	InteractSphere.Get()->SetupAttachment(RootComponent);

	TPWarpingLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MotionWarpingLocation"));
	TPWarpingLocation.Get()->SetupAttachment(RootComponent);

	TPActivationWarpingLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TPActivationWarpingLocation"));
	TPActivationWarpingLocation.Get()->SetupAttachment(RootComponent);

	ColliderForUnlocking = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderForUnlocking"));
	ColliderForUnlocking.Get()->SetupAttachment(RootComponent);
	
	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX.Get()->SetupAttachment(RootComponent);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio.Get()->SetupAttachment(RootComponent);
}

void ATPWithChoices::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GameInstance.Get()->DeleteTPFromArray(ID);
}

void ATPWithChoices::UnlockedTP()
{
	if(!VFX.Get()->IsActive())
	{
		VFX.Get()->Activate();
	}
	Audio.Get()->Activate(true);
	Audio.Get()->Play(0);
	Unlocked = true;
	GameInstance.Get()->SetTPUnlocked(ID);
	if(OnActivationAnim)
	{
		Player.Get()->GetWeaponComponent()->ChangeWeaponToRange();
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
		OnActivationAnim = false;
	}
}

FVector ATPWithChoices::GetTPLocation()
{
	return GetActorTransform().TransformPosition(TpLocation.GetLocation());
}

FRotator ATPWithChoices::GetTPRotator()
{
	return TpLocation.Rotator();
}


// Called when the game starts or when spawned
void ATPWithChoices::BeginPlay()
{
	Super::BeginPlay();
	
	GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	//Check if TP is already unlocked in GameInstance
	Unlocked = GameInstance.Get()->IsTpUnlocked(ID);
	//Add TP to GameInstance Array of TPs
	GameInstance.Get()->TPsInWorld.Add(this);
	
	if(Unlocked)
	{
		UnlockedTP();
	}
	else //Make Sure VFX is not active if tp is not unlocked
	{
		VFX.Get()->Deactivate();
	}
	
	InteractWidget = WidgetComponent.Get()->GetUserWidgetObject();
	InteractWidget->SetVisibility(ESlateVisibility::Hidden);
	
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ATPWithChoices::OnOverlapBegin);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ATPWithChoices::OnOverlapEnd);

	ColliderForUnlocking->OnComponentBeginOverlap.AddDynamic(this, &ATPWithChoices::OnOverlapUnlockBegin);
}


void ATPWithChoices::PlayTPMontage()
{
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	FVector WarpingLocation = TPWarpingLocation.Get()->GetComponentLocation();
	FRotator WarpingRotator = TPWarpingLocation.Get()->GetComponentRotation();
	UMotionWarpingComponent* WarpingComponent = Player.Get()->GetWarpComponent();
	FMotionWarpingTarget WarpingTarget;
	WarpingTarget.Name = "TP";
	WarpingTarget.Location = WarpingLocation;
	WarpingTarget.Rotation = WarpingRotator;
	WarpingComponent->AddOrUpdateWarpTarget(WarpingTarget);
	Player.Get()->CanFall = true;
	Player.Get()->PlayAnimMontage(TPAnimMontage);
	
	ColliderForEntering->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	if(ShouldTPToTavern)
	{
		UUserWidget* Widget = CreateWidget(GetWorld(),FadeOutWidget);
		Widget->AddToViewport();
		for(FAnimNotifyEvent NotifyEvent : TPAnimMontage->Notifies)
		{
			if(UTTAnimNotify* Notify = Cast<UTTAnimNotify>(NotifyEvent.Notify.Get()))
			{
				Notify->OnNotified.AddDynamic(this, &ATPWithChoices::TPToTavern);
			}
		}
	}
}

void ATPWithChoices::TPToTavern()
{
	ColliderForEntering->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	UGameplayStatics::OpenLevel(GetWorld(),"TavernGym");
}


void ATPWithChoices::Interact()
{
	if(!Unlocked && !OnActivationAnim)
	{
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		//Hide Weapon
		Player.Get()->GetWeaponComponent()->ClearWeapons();
		FVector WarpingLocation = TPActivationWarpingLocation.Get()->GetComponentLocation();
		FRotator WarpingRotator = TPActivationWarpingLocation.Get()->GetComponentRotation();
		UMotionWarpingComponent* WarpingComponent = Player.Get()->GetWarpComponent();
		FMotionWarpingTarget WarpingTarget;
		WarpingTarget.Name = "TPActivation";
		WarpingTarget.Location = WarpingLocation;
		WarpingTarget.Rotation = WarpingRotator;
		WarpingComponent->AddOrUpdateWarpTarget(WarpingTarget);
		Player.Get()->PlayAnimMontage(TPActivationAnimMontage);
		OnActivationAnim = true;
		for(FAnimNotifyEvent NotifyEvent : TPActivationAnimMontage->Notifies)
		{
			if(UTTAnimNotify* Notify = Cast<UTTAnimNotify>(NotifyEvent.Notify.Get()))
			{
				Notify->OnNotified.RemoveAll(this);
				Notify->OnNotified.AddDynamic(this, &ATPWithChoices::UnlockedTP);
			}
		}
	}
	else if(!ShouldTPToTavern && !OnActivationAnim)
	{
		TObjectPtr<UBookMap> MapWidget = Cast<UBookMap>(CreateWidget(GetWorld(),MapWidgetClass));
		MapWidget.Get()->SetShouldManageClose();
		if(ATTPlayerController* PlayerController = Cast<ATTPlayerController>(Player->GetController()))
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
		}
		MapWidget->OnButtonClicked.AddDynamic(this, &ATPWithChoices::PlayTPMontage);
		MapWidget->SetFocus();
		MapWidget->AddToViewport();
		MapWidget->ShowButtons(ID);
		MapWidget->ActivateWidget();
	}
	else if(Unlocked)
	{
		
		PlayTPMontage();
	}
}

void ATPWithChoices::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayerCharacter>(OtherActor);
	InteractWidget->SetVisibility(ESlateVisibility::Visible);
	
	if(Player)
	{
		//HideUI();
		Cast<ATTPlayerController>(Player->GetController())->OnInteract.AddDynamic(this, &ATPWithChoices::Interact);
		Player->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
	}
}

void ATPWithChoices::OnOverlapUnlockBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!Unlocked)
	{
		UnlockedTP();
	}
}

void ATPWithChoices::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Player)
	{
		if(ATTPlayerController* PlayerController = Cast<ATTPlayerController>(Player->GetController()))
		{
			PlayerController->OnInteract.RemoveAll(this);
			Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
		}
		Player.Get()->CanFall = false;
		InteractWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}



