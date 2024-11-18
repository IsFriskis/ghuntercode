// TinyTavern


#include "GamusinoIAController.h"

#define print(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT(x));}

// Sets default values
AGamusinoIAController::AGamusinoIAController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGamusinoIAController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGamusinoIAController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

