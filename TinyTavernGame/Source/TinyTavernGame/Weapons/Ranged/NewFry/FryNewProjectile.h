// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "FryNewProjectile.generated.h"

class UGameplayEffect;

UCLASS()
class TINYTAVERNGAME_API AFryNewProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFryNewProjectile();
	
	void SetTimer(float _Timer) { this->Timer = _Timer; }
	float GetTimer() const { return Timer; }

	void InitValues(float _Scale, float _TimeAlive, float _ExplosionRadius, float _Damage, float _Speed, FVector _Direction, TSubclassOf<
	                UGameplayEffect> _GameplayEffect, float _WineDamageMultiplier, TObjectPtr<UNiagaraSystem> _ExplosionEffect, USoundWave*
	                _ExplosionSound);
	void UpdateValues(float _Scale, float _TimeAlive, float _ExplosionRadius, float _Damage, float _Speed,
	                  FVector _Direction, TSubclassOf<UGameplayEffect> _GameplayEffect, float _WineDamageMultiplier, TObjectPtr<
	                  UNiagaraSystem> _ExplosionEffect, USoundWave* _ExplosionSound);
	void UpdateScale(float _Scale);
	void StartMoving();
	void PerformExplosionSweep(AEnemyBase* EnemyBase);
	void ApplyEffect(TSubclassOf<UGameplayEffect> Effect, float Magnitude, AActor* Actor,
	                 UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle EffectContextHandle);
	void SetNiagaraRotationSpeed(float RotationSpeed);
	void SetDistorsionBallLevel(float DistorsionBallLevel);

	bool Moving = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	void OnTimerEnd();
	
	UFUNCTION()
	void HandleParticlesGone(UNiagaraComponent* PSystem);

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
	                             bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Particles")
	float NewParticles = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Particles")
	float AuraSizeT1 = 0.7f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Particles")
	float AuraSizeT2 = 0.90f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TT| Particles")
	float AuraSizeT3 = 1.1f;
protected:
	// Called every frame
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> BoxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> NiagaraSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , Category = "TT|Params")
	TObjectPtr<class UNiagaraSystem> ExplosionEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , Category = "TT|Params")
	TObjectPtr<USoundWave> ExplosionSound;



	float WineDamageMultiplier = 1.0f;
	float ProjectileSpeed = 0.0f;
	float ExplosionRadius = 0.0f;
	float Damage = 0.0f;
	FVector Direction = FVector::Zero();

	bool hasExploded = false;
private:

	UPROPERTY()
	TObjectPtr<UObject> HitComp;

	float Timer = 0.0f;
};
