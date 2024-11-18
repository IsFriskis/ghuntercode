// TinyTavern


#include "LookOut.h"

#include "AbilitySystemComponent.h"
#include "RootMotionModifier.h"
#include "MotionWarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/UI/HUD/LookOutUI.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"


// Sets default values
ALookOut::ALookOut()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent")));
	// Create and attach the StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Create and attach the BoxComponent
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	// Create and attach the CameraComponent
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	// Create and attach the WidgetComponent
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	MotionWarpingLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MotionWarpingLocation"));
	MotionWarpingLocation->SetupAttachment(RootComponent);
	
	PlayerRef = nullptr;
}


// Called when the game starts or when spawned
void ALookOut::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALookOut::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALookOut::OnOverlapEnd);

	if(WidgetComponent->GetWidget())
	{
		WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

// Called every frame
void ALookOut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// bIsInside ? ShowOrHideCanInteract(bIsInside) : ShowOrHideCanInteract(bIsInside);
}

void ALookOut::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		bIsInside = true;
		PlayerController = Cast<ATTPlayerController>(Player->GetController());
		if(PlayerController)
		{
			PlayerController.Get()->OnInteract.AddDynamic(this, &ALookOut::Interact);
		}

		ShowOrHideCanInteract(true);
		PlayerRef = Player;
		AlreadyUsed = false;
	}
}

void ALookOut::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		bIsInside = false;
		PlayerController = Cast<ATTPlayerController>(Player->GetController());
		if(PlayerController)
		{
			PlayerController.Get()->OnInteract.RemoveAll(this);
			PlayerController.Get()->OnBackAction.RemoveAll(this);
		}

		ShowOrHideCanInteract(false);
		
		Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
		Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
	}
}

void ALookOut::Interact()
{
	PlayerController->OnInteract.RemoveAll(this);
	PlayerController.Get()->OnBackAction.AddDynamic(this, &ALookOut::ExitLogic);
	if (PlayerRef && !PlayerRef->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_OnDashAttack))
	{
		if(!bCanExit && !AlreadyUsed)
		{
			ShowOrHideCanInteract(false);
			AlreadyUsed = true;
			if (PlayerController)
			{
				PlayerController->SetViewTargetWithBlend(this, 2.0f);
			}
			
			if(InSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, InSound, GetActorLocation());
			}
			
			if(ApproachToLookOutMontage)
			{
				FVector WarpingLocation = MotionWarpingLocation->GetComponentLocation();
				FRotator WarpingRotator = MotionWarpingLocation->GetComponentRotation();
				UMotionWarpingComponent* WarpingComponent = PlayerRef->GetWarpComponent();
				FMotionWarpingTarget WarpingTarget;
				WarpingTarget.Name = "LookOut";
				WarpingTarget.Location = WarpingLocation;
				WarpingTarget.Rotation = WarpingRotator;
				WarpingComponent->AddOrUpdateWarpTarget(WarpingTarget);
				PlayerRef->PlayAnimMontage(ApproachToLookOutMontage);
				
				PlayerRef->GetWeaponComponent()->ClearWeapons();
				PlayerRef->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
				PlayerRef->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_AimBlocked);
				PlayerRef->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection);
				PlayerRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
				if(!PlayerRef->GetHUDIsHidden())
				{
					PlayerRef->SetHUDVisibility(true);
				}
				bool FirstExec = true;
				for(FAnimNotifyEvent NotifyEvent : ApproachToLookOutMontage->Notifies)
				{
					
					UTTAnimNotify* Notify = Cast<UTTAnimNotify>(NotifyEvent.Notify);
					if(Notify && !Notify->OnNotified.IsBound())
					{
						if(FirstExec)
						{
							NotifyToSetExit = Notify;
							Notify->OnNotified.AddDynamic(this, &ALookOut::SetCanExit);
							FirstExec = false;
						}
						else
						{
							NotifyToBlockMovement = Notify;
							Notify->OnNotified.AddDynamic(this, &ALookOut::BlockMovement);
							break;
						}
					}
				}
				//Show LookOut Widget
				LookOutWidget = Cast<ULookOutUI>(CreateWidget(GetWorld(), LookOutWidgetClass));
				LookOutWidget.Get()->AddToViewport(-1);

				//Interacted with all of the LookOuts
				TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
				if (ttGameInstance)
				{
					ttGameInstance.Get()->LookoutInteracted(LookoutID);
				}
			}
			else
			{
				print("No ApproachToLookOutMontage set on LookOut");
			}
		}
		else
		{
			ExitLogic();
		}
	}
}
void ALookOut::ExitLogic()
{
	if (PlayerController && AlreadyUsed && bCanExit)
	{
		if(PlayerRef->GetHUDIsHidden())
		{
			PlayerRef->SetHUDVisibility(false);
		}
		AlreadyUsed = false;
		PlayerController.Get()->OnBackAction.RemoveAll(this);
		if(WidgetComponent)
		{
			WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		}
		if(BackSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BackSound, GetActorLocation());
		}
		PlayerRef->StopAnimMontage();
		PlayerRef->GetCharacterMovement()->SetDefaultMovementMode();
		PlayerController->SetViewTargetWithBlend(PlayerRef, 2.0f);
		bCanExit = false;// Adjust blend time as needed
		PlayerRef->GetWeaponComponent()->ChangeWeaponToRange();
		PlayerRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked, 100);
		PlayerRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_AimBlocked, 100);
		PlayerRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerRecolection, 100);
		if(LookOutWidget)
		{
			LookOutWidget.Get()->RemoveFromParent();
		}
		PlayerController.Get()->OnInteract.AddDynamic(this, &ALookOut::Interact);
		ShowOrHideCanInteract(true);
	}
}

void ALookOut::SetCanExit()
{
	if(NotifyToSetExit)
	{
		NotifyToSetExit->OnNotified.RemoveDynamic(this, &ALookOut::SetCanExit);
	}
	bCanExit = true;
	
}void ALookOut::BlockMovement()
{
	if(NotifyToBlockMovement)
	{
		NotifyToBlockMovement->OnNotified.RemoveDynamic(this, &ALookOut::BlockMovement);
	}
	if(PlayerRef)
	{
		PlayerRef->GetCharacterMovement()->DisableMovement();
	}
}

void ALookOut::ShowOrHideCanInteract(bool bCanInteract)
{
	if(!bCanInteract )
	{
		if(WidgetComponent)
		{
			WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		}
		if(StaticMeshComponent)
		{
			StaticMeshComponent->SetOverlayMaterial(nullptr);
		}
	}
	else if (bCanInteract)
	{
		if(WidgetComponent)
		{
			WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
		}
		if(OverlayMaterial)
		{
			StaticMeshComponent->SetOverlayMaterial(OverlayMaterial);
		}
	}
}