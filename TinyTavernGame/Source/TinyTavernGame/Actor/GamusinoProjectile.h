// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GamusinoProjectile.generated.h"

UCLASS()
class TINYTAVERNGAME_API AGamusinoProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGamusinoProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UBoxComponent> BoxComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
	TSubclassOf<UGameplayEffect> DamageEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
	float ProjectileSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
	TObjectPtr<UNiagaraSystem> NiagaraSystemOnExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
	TObjectPtr<USoundBase> SoundOnExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
	float LifeTime = 0.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector PlayerDir = FVector::ZeroVector;
public:
	
	// Function to handle the hit event
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void BeforeDestroy();

private:
	float Timer;
	UPROPERTY()
	TObjectPtr<UObject> HitComp;
};
