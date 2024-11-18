// TinyTavern


#include "CamStreamingVolumes.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"


// Sets default values
ACamStreamingVolumes::ACamStreamingVolumes()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
}

// Called when the game starts or when spawned
void ACamStreamingVolumes::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACamStreamingVolumes::OnOverlapBegin);
	
}

void ACamStreamingVolumes::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	BoxComponent->OnComponentBeginOverlap.RemoveAll(this);
}

void ACamStreamingVolumes::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		bToCombatCam ? Player->SetCombatCam(true) : Player->SetCombatCam(false);
	}
}

// Called every frame
void ACamStreamingVolumes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

