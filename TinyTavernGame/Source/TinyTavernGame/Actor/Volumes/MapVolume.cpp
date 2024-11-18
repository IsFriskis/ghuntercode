// TinyTavern


#include "MapVolume.h"

#include "Components/BrushComponent.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


// Sets default values
AMapVolume::AMapVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if(GetBrushComponent())
	{
		GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	}
}

// Called when the game starts or when spawned
void AMapVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this,&AMapVolume::OnOverlapBegin);
	
}

// Called every frame
void AMapVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMapVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(OtherActor)
		Player = Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		UTT_GameInstance* GI = Cast<UTT_GameInstance> (GetGameInstance());
		GI->GhunterMapPosition = ID;
		Player->MapAreaID = MapAreaID;
	}
}

