// TinyTavern


#include "TallGrass.h"

#include "Components/BoxComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TinyTavernGame/Character/Enemy/Basilisco/EnemyBasilisco.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


// Sets default values
ATallGrass::ATallGrass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = TriggerBox;
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATallGrass::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATallGrass::OnOverlapEnd);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TallGrassMesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATallGrass::BeginPlay()
{
	Super::BeginPlay();
}

void ATallGrass::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	TriggerBox->OnComponentBeginOverlap.RemoveAll(this);
	TriggerBox->OnComponentEndOverlap.RemoveAll(this);
}

void ATallGrass::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<APlayerCharacter>(OtherActor);
	enemy = Cast<AEnemyBase>(OtherActor);

	OverlappingActors.Add(OtherActor);
	
	if(OverlappingActors.Contains(player) && OverlappingActors.Contains(enemy))
	{
		player->StimulusSource->bAutoRegister=true;
		SightSense = enemy->AIController->GetPerceptionComponent()->GetDominantSenseID();
		SenseConfig = enemy->AIController->GetPerceptionComponent()->GetSenseConfig(SightSense);
		SightConfig = Cast<UAISenseConfig_Sight>(SenseConfig);
		SightConfig->SightRadius = enemy->AIController->BaseSightRadius;
	}
	else
	{
		if(player)
		{
			UAIPerceptionSystem::GetCurrent(GetWorld())->UnregisterSource(*player, UAISense_Sight::StaticClass());
		}
		else if (enemy && enemy->AIController->GetPerceptionComponent() != nullptr)
		{
			SightSense = enemy->AIController->GetPerceptionComponent()->GetDominantSenseID();
			SenseConfig = enemy->AIController->GetPerceptionComponent()->GetSenseConfig(SightSense);
			SightConfig = Cast<UAISenseConfig_Sight>(SenseConfig);
			SightConfig->SightRadius = LoseSightRadius;
		}
	}
}

void ATallGrass::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	player = Cast<APlayerCharacter>(OtherActor);
	enemy = Cast<AEnemyBase>(OtherActor);
	OverlappingActors.Remove(OtherActor);
	if(player)
	{
		UAIPerceptionSystem::GetCurrent(GetWorld())->RegisterSource(*player);
	}
	else if (enemy && enemy->AIController->GetPerceptionComponent() != nullptr)
	{
		SightSense = enemy->AIController->GetPerceptionComponent()->GetDominantSenseID();
		SenseConfig = enemy->AIController->GetPerceptionComponent()->GetSenseConfig(SightSense);
		SightConfig = Cast<UAISenseConfig_Sight>(SenseConfig);
		SightConfig->SightRadius = enemy->AIController->BaseSightRadius;
	}
}

// Called every frame
void ATallGrass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

