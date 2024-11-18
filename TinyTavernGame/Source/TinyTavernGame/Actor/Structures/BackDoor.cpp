// TinyTavern


#include "BackDoor.h"

#include "Lever.h"
#include "Statue.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"


ABackDoor::ABackDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BackDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Door Mesh");
	SetRootComponent(BackDoorMesh);
	BackDoorAudio = CreateDefaultSubobject<UAudioComponent>("DoorAudio");
	BackDoorAudio->SetupAttachment(BackDoorMesh);
	BackDoorBlend.SetBlendTime(AnimationDoorTime);
	BackDoorBlend.SetBlendOption(BlendType);
	Sound = false;
	Ended = false;
}


void ABackDoor::OpenDoor(int ID)
{
	Open = true;
}

void ABackDoor::BeginPlay()
{
	Super::BeginPlay();

	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	BackDoorInitialPosition = GetActorLocation();

	for(int Id : TTGameInstance.Get()->OpenBackDoors)
	{
		if(Id == DoorID )
		{
			SetActorLocation(FVector(BackDoorInitialPosition.X,BackDoorInitialPosition.Y,BackDoorInitialPosition.Z - BackDoorHeight));
		}
	}
	
	TargetPosition = BackDoorInitialPosition - FVector(0,0,BackDoorHeight);
	Lever.Get()->LeverUsedDelegate.AddDynamic(this,&ABackDoor::OpenDoor);
}

void ABackDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABackDoor::BackDoorCompleted()
{
	
	TTGameInstance.Get()->OpenBackDoors.Add(DoorID);
	Lever.Get()->LeverUsedDelegate.RemoveAll(this);
	Ended = true;
	BP_DoorEnd();
}

void ABackDoor::BackDoorExit()
{
}




void ABackDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Ended)
	{
		if(Open)
		{
			BP_DoorActivate();
			BackDoorBlend.Update(DeltaTime);
			FVector NewPosition = FMath::Lerp(BackDoorInitialPosition,TargetPosition,BackDoorBlend.GetAlpha());
			SetActorLocation(NewPosition);
			
			if(BackDoorBlend.IsComplete())
			{
				BackDoorCompleted();
			}
		}	
	}
}

/*void ABackDoor::PlayerInteracted()
{
}

bool ABackDoor::CheckIfShouldBeEnabled()
{
	return true;
}*/

