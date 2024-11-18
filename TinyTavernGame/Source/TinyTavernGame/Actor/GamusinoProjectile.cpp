// TinyTavern


#include "GamusinoProjectile.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


// Sets default values
AGamusinoProjectile::AGamusinoProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(BoxComponent);
}

// Called when the game starts or when spawned
void AGamusinoProjectile::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentHit.AddDynamic(this, &AGamusinoProjectile::OnComponentHit);

	FVector PlayerPos = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetActorLocation();
	FVector Direction = PlayerPos - GetActorLocation();
	Direction.Normalize();
	SetActorRotation(Direction.Rotation());
	PlayerDir = Direction;
	Timer = LifeTime;
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

void AGamusinoProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if(HitComp != OtherActor && OtherActor != this)
	{
		HitComp = OtherActor;
		if(Player)
		{
			Player->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(DamageEffect.GetDefaultObject(), 1.0f, FGameplayEffectContextHandle());
		}
		BeforeDestroy();
	}
}

// Called every frame
void AGamusinoProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timer -= DeltaTime;

	if(Timer <= 0)
	{
		BeforeDestroy();
	}

	FVector NewLocation = GetActorLocation() + PlayerDir * ProjectileSpeed * DeltaTime;
	SetActorLocation(NewLocation, true);
}

void AGamusinoProjectile::BeforeDestroy()
{
	if(NiagaraSystemOnExplosion)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystemOnExplosion, GetActorLocation(), FRotator::ZeroRotator, this->GetActorScale3D());
	}
	if(SoundOnExplosion)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundOnExplosion, GetActorLocation());
	}
	Destroy();
}