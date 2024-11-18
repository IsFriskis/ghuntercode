// TinyTavern
#include "BasiliscoIAController.h"



// Sets default values
ABasiliscoIAController::ABasiliscoIAController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasiliscoIAController::BeginPlay()
{
	Super::BeginPlay();
}


