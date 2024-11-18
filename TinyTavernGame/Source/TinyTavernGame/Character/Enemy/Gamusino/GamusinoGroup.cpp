// TinyTavern


#include "GamusinoGroup.h"

#include "EnemyGamusino.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"


// Sets default values
AGamusinoGroup::AGamusinoGroup()
	{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGamusinoGroup::NotifyIsAttacked()
{
	//TODO: Implement behaviour when the group is attacked
	for (AEnemyBase* Enemy : GetEnemiesInGroup())
	{
		if(Enemy)
		{
			Enemy->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_Alert);
		}
	}
}

// Called when the game starts or when spawned
void AGamusinoGroup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGamusinoGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

