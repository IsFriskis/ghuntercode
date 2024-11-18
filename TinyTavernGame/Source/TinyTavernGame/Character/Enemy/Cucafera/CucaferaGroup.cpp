// TinyTavern


#include "CucaferaGroup.h"

// Sets default values
ACucaferaGroup::ACucaferaGroup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACucaferaGroup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACucaferaGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(HasGuardian && enemiesInGroup.Num() == 0)
	{
		for (AGuardianWall* wall : GuardianWallsArray)
		{
			wall->HideWallsAgain(DeltaTime);
		}
	}
}

