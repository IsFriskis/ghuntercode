// TinyTavern


#include "UIPlayer.h"

#include "ArticyFlowPlayer.h"
#include "CommonActivatableWidget.h"
#include "TinyTavernGame/UI/Menus/Tavern/Tavern.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TinyTavernGame/UI/Menus/Tavern/TavernMenu.h"

// Sets default values
AUIPlayer::AUIPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerArticyFlowPlayer = CreateDefaultSubobject<UArticyFlowPlayer>(TEXT("Player Articy Flow Player"));
}

// Called when the game starts or when spawned
void AUIPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(Subsystem)
		{
			Subsystem->AddMappingContext(TTInputMappingContext,0);
		}
	}
}

void AUIPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AUIPlayer::AddTavern(TObjectPtr<UTavernMenu> _TavernMenu, TObjectPtr<UTavern> _Tavern)
{
	Tavern = _Tavern;
	TavernMenu = _TavernMenu;
}

// Called every frame
void AUIPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUIPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(OpenSettingsAction, ETriggerEvent::Started,this, &AUIPlayer::OpenSettingsMenu);
	}
}

void AUIPlayer::OpenSettingsMenu()
{
	if(Tavern && TavernMenu.Get()->IsVisible())
	{
		if(!SettingsMenuOpen && CanInteractWithRecipeBook)
		{
			CanInteractWithRecipeBook = false;
			RecipeBook = Tavern.Get()->AddWidgetToTavernStack(FTavernWidgets::RecipeBook);
			SettingsMenuOpen = true;
			GetWorld()->GetTimerManager().SetTimer(
			TimerHandle, // handle to cancel timer at a later time
			this, // the owning object
			&AUIPlayer::PlayerIsAbleToCloseRecipeBook, // function to call on elapsed
			TimeOfRecipeBookAnimation, // float delay until elapsed
			false);
		}
		else if(CanInteractWithRecipeBook)
		{
			CanInteractWithRecipeBook = false;
			Tavern.Get()->CloseRecipeBook();
			RecipeBook.Get()->DeactivateWidget();
			SettingsMenuOpen = false;
			GetWorld()->GetTimerManager().SetTimer(
			TimerHandle, // handle to cancel timer at a later time
			this, // the owning object
			&AUIPlayer::PlayerIsAbleToCloseRecipeBook, // function to call on elapsed
			TimeOfRecipeBookAnimation, // float delay until elapsed
			false);
		}	
	}
}

void AUIPlayer::PlayerIsAbleToCloseRecipeBook()
{
	CanInteractWithRecipeBook = true;
}

