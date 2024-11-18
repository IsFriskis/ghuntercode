// TinyTavern


#include "TitleScreenLevel.h"


// Sets default values
ATitleScreenLevel::ATitleScreenLevel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATitleScreenLevel::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ATitleScreenLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

