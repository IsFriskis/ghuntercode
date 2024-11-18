// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoisonSpitProjectile.generated.h"

class UGameplayEffect;
class UNiagaraComponent;

UCLASS()
class TINYTAVERNGAME_API APoisonSpitProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APoisonSpitProjectile();
	
	float GetMaxDistance() const{return MaxDistance;}
	void SetMaxDistance(float _MaxDistance){MaxDistance = _MaxDistance;}
	void SetProjectileSpeed(float _ProjectileSpeed){ProjectileSpeed = _ProjectileSpeed;}
	void SetProjectileDirection(FVector _ProjectileDirection){ProjectileDirection = _ProjectileDirection;}
	void SetProjectileRadius(float _ProjectileRadius){ProjectileRadius = _ProjectileRadius;}
	void SetProjectileExplosionRadius(float _ProjectileExplosionRadius){ProjectileExplosionRadius = _ProjectileExplosionRadius;}
	void SetDamageEffect(const TSubclassOf<UGameplayEffect>& _DamageEffect){DamageEffect = _DamageEffect;}
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> PoisonSpitMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> PoisonSpitParticles;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float MaxDistance = 800.0f;
	float ProjectileSpeed = 200.0f;
	float ProjectileRadius = 50.0f;
	float ProjectileExplosionRadius = 100.0f;
	FVector ProjectileDirection;
	FVector InitialPosition;
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	void CollisionCheck();
	void Explode();
};
