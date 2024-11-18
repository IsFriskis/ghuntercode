// TinyTavern


#include "FryNewProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FryWeaponAbility.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"


// Sets default values
AFryNewProjectile::AFryNewProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxCollider.Get();

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System"));
	NiagaraSystem.Get()->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFryNewProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(NiagaraSystem)
	{
		NiagaraSystem.Get()->SetFloatParameter("droplets", 0);
	}
}
// Called every frame
void AFryNewProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Moving)
	{
		Timer -= DeltaTime;
		
		if(Timer <= 0)
		{
			OnTimerEnd();
		}
		
		FVector currentLocation = GetActorLocation();
		FVector newLocation = currentLocation + Direction * ProjectileSpeed * DeltaTime;
		SetActorLocation(newLocation, true);
	}
}

void AFryNewProjectile::OnTimerEnd()
{
	//BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//BoxCollider->SetCollisionObjectType(ECC_WorldDynamic);
	PerformExplosionSweep(nullptr);
	
	if (NiagaraSystem)
	{
		//NiagaraSystem->OnSystemFinished.AddDynamic(this, &AFryNewProjectile::HandleParticlesGone);
	}
	else
	{
		print("Niagara System is null");
	}
	
}

void AFryNewProjectile::HandleParticlesGone(UNiagaraComponent* PSystem)
{
	NiagaraSystem->OnSystemFinished.RemoveAll(this);
	
}

void AFryNewProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(HitComp != OtherActor)
	{
		HitComp = OtherActor;
		AEnemyBase* EnemyBase = Cast<AEnemyBase>(OtherActor);
		print("He chocado con algo");
		if(EnemyBase)
		{
			FGameplayEffectContextHandle effectContextHandle = EnemyBase->GetAbilitySystemComponent()->MakeEffectContext();
			effectContextHandle.AddSourceObject(this);

			if(EnemyBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Drunk))
			{
				ApplyEffect(GameplayEffect, Damage * WineDamageMultiplier, EnemyBase, EnemyBase->GetAbilitySystemComponent(), effectContextHandle);
			}
			else
			{
				ApplyEffect(GameplayEffect, Damage, EnemyBase, EnemyBase->GetAbilitySystemComponent(), effectContextHandle);
				
			}
		}
		PerformExplosionSweep(EnemyBase);
		Timer = 0;
	}
}

void AFryNewProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OtherActor);
	print("He chocado con algo");
	if(EnemyBase)
	{
		FGameplayEffectContextHandle effectContextHandle = EnemyBase->GetAbilitySystemComponent()->MakeEffectContext();
		effectContextHandle.AddSourceObject(this);

		if(EnemyBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Drunk))
		{
			ApplyEffect(GameplayEffect, Damage * WineDamageMultiplier, EnemyBase, EnemyBase->GetAbilitySystemComponent(), effectContextHandle);
		}
		else
		{
			ApplyEffect(GameplayEffect, Damage, EnemyBase, EnemyBase->GetAbilitySystemComponent(), effectContextHandle);
			
		}
	}
	PerformExplosionSweep(EnemyBase);
	
}




void AFryNewProjectile::InitValues(float _Scale, float _TimeAlive, float _ExplosionRadius, float _Damage, float _Speed,FVector _Direction, TSubclassOf<UGameplayEffect> _GameplayEffect, float _WineDamageMultiplier, TObjectPtr<UNiagaraSystem> _ExplosionEffect, USoundWave* _ExplosionSound)
{
	BoxCollider.Get()->SetWorldScale3D(FVector(_Scale));
	SetTimer(_TimeAlive);
	ExplosionRadius = _ExplosionRadius;
	ProjectileSpeed = _Speed;
	Damage = _Damage;
	Direction = _Direction;
	GameplayEffect = _GameplayEffect;
	WineDamageMultiplier = _WineDamageMultiplier;
	ExplosionEffect = _ExplosionEffect;
	ExplosionSound = _ExplosionSound;

	NiagaraSystem.Get()->SetFloatParameter("ballSpeed", ProjectileSpeed);
	NiagaraSystem.Get()->SetFloatParameter("new particles", NewParticles);
	NiagaraSystem.Get()->SetFloatParameter("aura", AuraSizeT1);
}

void AFryNewProjectile::UpdateValues(float _Scale, float _TimeAlive, float _ExplosionRadius, float _Damage, float _Speed,FVector _Direction, TSubclassOf<UGameplayEffect> _GameplayEffect, float _WineDamageMultiplier, TObjectPtr<UNiagaraSystem> _ExplosionEffect, USoundWave* _ExplosionSound)
{
	BoxCollider.Get()->SetWorldScale3D(FVector(_Scale));
	SetTimer(_TimeAlive);
	ExplosionRadius = _ExplosionRadius;
	ProjectileSpeed = _Speed;
	Damage = _Damage;
	Direction = _Direction;
	GameplayEffect = _GameplayEffect;
	WineDamageMultiplier = _WineDamageMultiplier;
	ExplosionEffect = _ExplosionEffect;
	ExplosionSound = _ExplosionSound;
	
	NiagaraSystem.Get()->SetFloatParameter("ballSpeed", ProjectileSpeed);
	
	
}

void AFryNewProjectile::UpdateScale(float _Scale)
{
	BoxCollider.Get()->SetWorldScale3D(FVector(_Scale));
}


void AFryNewProjectile::StartMoving()
{
	Moving = true;
	NiagaraSystem.Get()->SetFloatParameter("droplets", 20);
	NiagaraSystem.Get()->SetFloatParameter("new particles", 0);
	NiagaraSystem.Get()->SetFloatParameter("aura size", 0);
	BoxCollider->OnComponentHit.AddDynamic(this, &AFryNewProjectile::OnComponentHit);
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

void AFryNewProjectile::PerformExplosionSweep(AEnemyBase* EnemyBase)
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 1000); // Raycast downwards 1000 units

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore self

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);
	// Define the parameters for the sweep
	FCollisionQueryParams SweepParams;
	SweepParams.AddIgnoredActor(this); // Ignore self; // Ignore the player (assuming the player is the owner)
	if(EnemyBase)
	{
		SweepParams.AddIgnoredActor(EnemyBase); // Ignore the EnemyBase
	}

	// Define the start and end points for the sweep (same point for a circular sweep)
	FVector SweepStart = HitResult.ImpactPoint;
	FVector SweepEnd = SweepStart;

	// Perform the sweep
	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByChannel(
		HitResults,
		SweepStart,
		SweepEnd,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(ExplosionRadius),
		SweepParams
	);

#if  WITH_EDITOR
	// Draw debug sphere
	DrawDebugSphere(
	  GetWorld(),
	  SweepEnd,
	  ExplosionRadius, // Radius of the debug sphere
		  12, // Number of segments
	  FColor::Green,
	  false, // Persistent lines
	  5.0f // Duration
  );
#endif

	TArray<AEnemyBase*> HitEnemies;
	// Process the hit results
	for (const FHitResult& Hit : HitResults)
	{
		// Handle each hit result as needed
		AActor* HitActor = Hit.GetActor();
		AEnemyBase* HitEnemy = Cast<AEnemyBase>(HitActor);
		if (HitActor && HitActor != this && HitActor != GetOwner() && HitEnemy != EnemyBase && HitEnemy && !HitEnemies.Contains(HitEnemy))
		{
			HitEnemies.Add(HitEnemy);
			if(HitEnemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Drunk))
			{
				ApplyEffect(GameplayEffect, Damage * WineDamageMultiplier, HitEnemy, HitEnemy->GetAbilitySystemComponent(), HitEnemy->GetAbilitySystemComponent()->MakeEffectContext());
			}
			else
			{
				ApplyEffect(GameplayEffect, Damage, HitEnemy, HitEnemy->GetAbilitySystemComponent(), HitEnemy->GetAbilitySystemComponent()->MakeEffectContext());
			}
		}
	}
	
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator, this->GetActorScale3D());
	}
	if(ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
	hasExploded = true;
	NiagaraSystem.Get()->Deactivate();
	
	Destroy();
}

void AFryNewProjectile::ApplyEffect(TSubclassOf<UGameplayEffect> Effect, float Magnitude, AActor* Actor, UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle EffectContextHandle)
{
	FGameplayEffectSpecHandle effectSpecHandle = TargetASC->MakeOutgoingSpec(Effect, 1, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(effectSpecHandle, FGameplayTags::Get().States_Cook_Fried, Magnitude);
	TargetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());
}

void AFryNewProjectile::SetNiagaraRotationSpeed(float RotationSpeed)
{
	NiagaraSystem.Get()->SetVariableVec3("rotation", FVector(0.0f,RotationSpeed,0.0f));
	NiagaraSystem.Get()->SetVectorParameter("rotation", FVector(0.0f,RotationSpeed,0.0f));
}
void AFryNewProjectile::SetDistorsionBallLevel(float DistorsionBallLevel)
{
	NiagaraSystem.Get()->SetFloatParameter("meshDistorsion", DistorsionBallLevel);
}
