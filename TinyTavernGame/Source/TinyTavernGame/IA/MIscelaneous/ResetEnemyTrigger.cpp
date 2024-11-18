// TinyTavern


#include "ResetEnemyTrigger.h"

#include "Components/BoxComponent.h"


// Sets default values
AResetEnemyTrigger::AResetEnemyTrigger()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBoxResetEnemy");
	RootComponent = TriggerBox;
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AResetEnemyTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AResetEnemyTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AResetEnemyTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	TriggerBox->OnComponentBeginOverlap.RemoveAll(this);
}

void AResetEnemyTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if(PlayerCharacter)
	{
		for (TObjectPtr<AEnemyGroup> EnemyGroup : EnemyGroupsArray)
		{
			EnemyGroup->ResetEnemies();
		}
	}
}



