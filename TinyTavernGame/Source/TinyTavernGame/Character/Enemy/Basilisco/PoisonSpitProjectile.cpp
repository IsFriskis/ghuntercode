// TinyTavern


#include "PoisonSpitProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/EnemyGamusino.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


// Sets default values
APoisonSpitProjectile::APoisonSpitProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PoisonSpitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Poison Spit Mesh"));
	RootComponent = PoisonSpitMesh.Get();
	
	PoisonSpitParticles = CreateDefaultSubobject<UNiagaraComponent>("Poison Spit VFX");
	PoisonSpitParticles.Get()->SetupAttachment(PoisonSpitMesh);
}

// Called when the game starts or when spawned
void APoisonSpitProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	InitialPosition = GetActorLocation();
}


// Called every frame
void APoisonSpitProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If projectile reached max distance, it explodes
	if(FVector::Dist(InitialPosition, GetActorLocation()) >= MaxDistance)
	{
		Destroy();
	}
	
	FVector newLocation = GetActorLocation() + ProjectileDirection * (ProjectileSpeed * DeltaTime);
	SetActorLocation(newLocation);

	//TODO Collision Check
	CollisionCheck();
}	

void APoisonSpitProjectile::CollisionCheck()
{
	//Collision Configuration
	FHitResult hit;
	FCollisionShape collisionShape;
	collisionShape.SetSphere(ProjectileRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetParentActor());

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileRadius, 8, FColor::Green, false, -1.0f);
#endif
	//Check all colliding entities in the weapons shape range indicator
	bool isHit = GetWorld()->SweepSingleByChannel(hit, GetActorLocation(), GetActorLocation() + FVector(0.001f,0.001f,0), GetActorQuat(), ECC_Visibility, collisionShape, QueryParams);
	if (isHit)
	{
		Explode();
	}
}

void APoisonSpitProjectile::Explode()
{
	//--Explode--
#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileExplosionRadius, 8, FColor::Emerald, false, 5.0f);
#endif
	//Collision Configuration
	TArray<FHitResult> hits;
	FCollisionShape collisionShape;
	collisionShape.SetSphere(ProjectileExplosionRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetParentActor());
	
	bool isHit = GetWorld()->SweepMultiByChannel(hits, GetActorLocation(), GetActorLocation() + FVector(0.001f,0.001f,0), GetActorQuat(), ECC_GameTraceChannel1, collisionShape, QueryParams);
	if (isHit)
	{
		for (int i = 0; i < hits.Num(); i++)
		{
			//If collided with gamusino, explode Gamusino
			if (TObjectPtr<AEnemyGamusino> EnemyGamusino = Cast<AEnemyGamusino>(hits[i].GetActor()))
			{
				//TODO CHANGE
				EnemyGamusino.Get()->Destroy();
			}
			//If collided with player, damage it
			else if (TObjectPtr<APlayerCharacter> EnemyBase = Cast<APlayerCharacter>(hits[i].GetActor()))
			{
				//Apply damage effect
				TObjectPtr<UAbilitySystemComponent> PlayerAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(hits[i].GetActor());
				//Grab the cooldown effect passed by reference and apply it setting the duration
				FGameplayEffectContextHandle EffectContextHandle = PlayerAsc->MakeEffectContext();
				EffectContextHandle.AddSourceObject(this);
				FGameplayEffectSpecHandle DamageSpecHandle = PlayerAsc.Get()->MakeOutgoingSpec(DamageEffect, 1, EffectContextHandle);

				//The duration is equal to the recovery time based on the charge amount of the weapon
				PlayerAsc.Get()->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
			}
		}
	}
	Destroy();
}
