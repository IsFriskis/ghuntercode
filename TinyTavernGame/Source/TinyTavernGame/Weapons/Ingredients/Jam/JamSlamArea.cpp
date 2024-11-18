// TinyTavern


#include "JamSlamArea.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

// Sets default values
AJamSlamArea::AJamSlamArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JamAreaSphere = CreateDefaultSubobject<USphereComponent>("Jam Sphere");
	RootComponent = JamAreaSphere.Get();

	JamParticles = CreateDefaultSubobject<UNiagaraComponent>("Jam Particles");
	JamParticles.Get()->SetupAttachment(JamAreaSphere);

	JamMeteorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Jam Meteor Mesh");
	JamMeteorMesh.Get()->SetupAttachment(JamAreaSphere);
	
	HitboxPreview = CreateDefaultSubobject<UStaticMeshComponent>("Hitbox Preview");
	HitboxPreview.Get()->SetupAttachment(JamAreaSphere);
	
}

void AJamSlamArea::SetPetrificationAreaRadius(float _PetrificationAreaRadius)
{
	PetrificationAreaRadius = _PetrificationAreaRadius;
	JamAreaSphere.Get()->SetSphereRadius(PetrificationAreaRadius);
}

void AJamSlamArea::IncreasePetrificationDuration()
{
	PetrificationDuration = PetrificationDurationAfterWine;
	GetWorld()->SpawnActor<ANiagaraActor>(WineParticlesInJam, GetActorTransform());
}

// Called when the game starts or when spawned
void AJamSlamArea::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), JamSlamFallSound.Get(), GetActorLocation());
	HitboxPreview.Get()->SetWorldLocation(FinalDestination + FVector(0,0,50)); 
	JamMeteorMesh.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	JamAreaSphere.Get()->OnComponentBeginOverlap.AddDynamic(this, &AJamSlamArea::OnOverlapBegin);
	//Line Trace from air position to floor
	//Collision Configuration
	TObjectPtr<AEnemyBase> enemyInTheWay;
	FHitResult hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() - FVector(0,0,5000),FColor::Green, false, 10.0f);
#endif
	bool isHit = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() - FVector(0,0,5000),ECC_Visibility, QueryParams);
	if(isHit)
	{
		if(enemyInTheWay = Cast<AEnemyBase>(hit.GetActor()))
		{
			FCollisionQueryParams QueryParams2;
			TArray<TObjectPtr<AActor>> actorsToIgnore;
			actorsToIgnore.Add(this);
			actorsToIgnore.Add(enemyInTheWay);
			QueryParams2.AddIgnoredActors(actorsToIgnore);
			isHit = GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() - FVector(0,0,5000),ECC_Visibility, QueryParams2);
		}
		FinalDestination = hit.Location;
	}
	else
	{
		//If can not find a collision object, make it fall a lot of distance
		FinalDestination = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 5000);
	}
}

// Called every frame
void AJamSlamArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO Check with Design
	if(ShouldKeepFalling)
	{
		FVector newLocation = GetActorLocation() + FVector(0.0f,0.0f,-1.0f) * JamSlamMeteorSpeed * DeltaTime;
		SetActorLocation(newLocation);
		if(GetActorLocation().Z <= FinalDestination.Z)
		{
			SetActorLocation(FinalDestination);
			ShouldKeepFalling = false;
			CreateSweep();

			SetLifeSpan(1.5f);
			//Destroy();
			// //Set timer, when finish, area dissapears
			// GetWorld()->GetTimerManager().SetTimer(
			// 	TimerHandle, // handle to cancel timer at a later time
			// 	this, // the owning object
			// 	&AJamSlamArea::EndJamSlamArea, // function to call on elapsed
			// 	PetrificationAreaTime, // float delay until elapsed
			// false);
		}
		HitboxPreview.Get()->SetWorldLocation(FinalDestination+ FVector(0,0,10)); 
	}
}

void AJamSlamArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (CanOverlap && Cast<AEnemyBase>(OtherActor))
	// {
	// 	TObjectPtr<UAbilitySystemComponent> targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	// 	ApplyPetrifiedEffect(targetASC);
	// 	targetASC.Get()->AddLooseGameplayTag(FGameplayTags::Get().States_Ingredient_Jam);
	// }
}

void AJamSlamArea::ApplyPetrifiedEffect(TObjectPtr<UAbilitySystemComponent> TargetASC)
{
	FGameplayEventData Payload;
	Payload.EventMagnitude = PetrificationDuration;
	TargetASC.Get()->HandleGameplayEvent(FGameplayTags::Get().Events_Passive_PetrificationPassive, &Payload);
}

void AJamSlamArea::CreateSweep()
{
	//Change visibility and spawn
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), JamSlamExplosionSound.Get(), GetActorLocation());
	JamMeteorMesh.Get()->SetVisibility(false);
	//JamAreaSplatterMesh.Get()->SetVisibility(true);
	JamParticles.Get()->Activate();
	//Collision Configuration
	TArray<FHitResult> hits;
	FCollisionShape collisionShape;
	collisionShape.SetSphere(PetrificationAreaRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), GetActorLocation(), PetrificationAreaRadius, 8, FColor::Cyan, false, 5.0f);
#endif
	//Check all colliding entities in the weapons shape range indicator
	bool isHit = GetWorld()->SweepMultiByChannel(hits, GetActorLocation(), GetActorLocation()+FVector(0.001f,0.001f,0.0f), GetActorQuat(), ECC_GameTraceChannel1, collisionShape, QueryParams);
	if (isHit)
	{
		for (int i = 0; i < hits.Num(); i++)
		{
			//If collided with enemy, apply drunk and vulnerable
			if (TObjectPtr<AEnemyBase> enemyBase = Cast<AEnemyBase>(hits[i].GetActor()))
			{
				TObjectPtr<UAbilitySystemComponent> targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(hits[i].GetActor());
				ApplyPetrifiedEffect(targetASC);
				//Jam Achievement
				TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
				if (ttGameInstance)
				{
					ttGameInstance.Get()->JamApplied();
				}
			}
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionNiagara, GetActorLocation(), FRotator::ZeroRotator, this->GetActorScale3D());

}

void AJamSlamArea::ActivateOverlaps()
{
	//CanOverlap = true;
}

void AJamSlamArea::EndJamSlamArea()
{
	Destroy();
}


