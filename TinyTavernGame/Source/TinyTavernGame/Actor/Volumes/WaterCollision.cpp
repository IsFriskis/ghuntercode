// TinyTavern


#include "WaterCollision.h"

#include "Components/BrushComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


// Sets default values
AWaterCollision::AWaterCollision()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	if(GetBrushComponent())
	{
		GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
	}
}

// Called when the game starts or when spawned
void AWaterCollision::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this,&AWaterCollision::OnOverlapBegin);
	
}

// Called every frame
void AWaterCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaterCollision::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(OtherActor)
		Enemy = Cast<AEnemyBase>(OtherActor);
		if(Enemy)
		{
			if(!Enemy->Dive)
			{
				Enemy.Get()->CancelAllEnemyAbilitiesExcept(FGameplayTags::Get().States_Invulnerable);
				UAnimInstance* animinstance = Enemy.Get()->GetMesh()->GetAnimInstance();
				animinstance->Montage_Play(Enemy.Get()->DiveAnim);
				animinstance->OnPlayMontageNotifyBegin.AddDynamic(this,&AWaterCollision::DiveAnimEnd);
				Enemy.Get()->Dive = true;
			}
		}
}

void AWaterCollision::DiveAnimEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	UKismetSystemLibrary::Delay(this, 0.2f,LatentActionInfo);
	this->OnActorBeginOverlap.RemoveAll(this);
	if(Enemy)
	{
		Enemy->Destroy();
	}
}



