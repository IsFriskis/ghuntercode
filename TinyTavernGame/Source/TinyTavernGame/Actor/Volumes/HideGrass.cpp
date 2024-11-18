// TinyTavern


#include "HideGrass.h"

#include "Components/BrushComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


// Sets default values
AHideGrass::AHideGrass()
{
	// // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this,&AHideGrass::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this,&AHideGrass::OnOverlapEnd);
	if(GetBrushComponent())
	{
		GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	}

}

// Called when the game starts or when spawned
void AHideGrass::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHideGrass::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnActorBeginOverlap.RemoveDynamic(this,&AHideGrass::OnOverlapBegin);
	OnActorEndOverlap.RemoveDynamic(this,&AHideGrass::OnOverlapEnd);
}

void AHideGrass::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
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

void AHideGrass::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
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
// void AHideGrass::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

