// TinyTavern


#include "Destroyable.h"


// Sets default values
ADestroyable::ADestroyable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>("ResourceMesh");
	RootComponent = ResourceMesh;
	ResourceMeshCollision = CreateDefaultSubobject<UBoxComponent>("ResourceMeshCollision");
	ResourceMesh->SetCanEverAffectNavigation(false);
	ResourceMeshCollision->SetCanEverAffectNavigation(false);
	ResourceMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	FCollisionResponseContainer CollisionResponse;
	
	CollisionResponse.SetAllChannels(ECR_Ignore);
	CollisionResponse.SetResponse(ECollisionChannel::ECC_GameTraceChannel2, ECR_Block	);
	CollisionResponse.SetResponse(ECollisionChannel::ECC_EngineTraceChannel2, ECR_Block);
	ResourceMeshCollision->SetCollisionResponseToChannels(CollisionResponse);
	ResourceMeshCollision->SetCollisionResponseToChannels(CollisionResponse);
	ResourceMeshCollision->SetupAttachment(ResourceMesh);
}

// Called when the game starts or when spawned
void ADestroyable::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ADestroyable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestroyable::BeginDestroy()
{
	Super::BeginDestroy();

	if (DestructionVFX)
	{
		// Spawn the VFX actor at the location and rotation of this actor
		GetWorld()->SpawnActor<AActor>(DestructionVFX, GetActorLocation(), GetActorRotation());
	}
}

