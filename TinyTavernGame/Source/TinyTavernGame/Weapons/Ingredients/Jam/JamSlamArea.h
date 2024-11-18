// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "JamSlamArea.generated.h"

class ANiagaraActor;
class UNiagaraComponent;
struct FActiveGameplayEffect;
class UAbilitySystemComponent;
class USphereComponent;
class UGameplayEffect;

UCLASS()
class TINYTAVERNGAME_API AJamSlamArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJamSlamArea();

	void SetPetrificationDuration(float _PetrificationDuration){PetrificationDuration = _PetrificationDuration;}
	void SetPetrificationDurationAfterWine(float _PetrificationDurationAfterWine){PetrificationDurationAfterWine = _PetrificationDurationAfterWine;}
	void SetPetrificationAreaRadius(float _PetrificationAreaRadius);
	void SetPetrificationAreaTime(float _PetrificationAreaTime){PetrificationAreaTime = _PetrificationAreaTime;}
	void SetTimeBeforePetrification(float _TimeBeforePetrification){TimeBeforePetrification = _TimeBeforePetrification;}
	void SetJamSlamMeteorSpeed(float _JamSlamMeteorSpeed){JamSlamMeteorSpeed = _JamSlamMeteorSpeed;}
	void SetPetrificationEffect(const TSubclassOf<UGameplayEffect>& _PetrificationEffect){PetrificationEffect = _PetrificationEffect;}
	void SetInProcessOfPetrificationEffect(const TSubclassOf<UGameplayEffect>& _InProcessOfPetrificationEffect){InProcessOfPetrificationEffect = _InProcessOfPetrificationEffect;}
	void SetExplosionVFX(UNiagaraSystem* _ExplosionVFX){ExplosionNiagara = _ExplosionVFX;}
	void IncreasePetrificationDuration();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> JamMeteorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> JamParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> JamAreaSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ANiagaraActor> WineParticlesInJam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> HitboxPreview;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USoundBase> JamSlamFallSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USoundBase> JamSlamExplosionSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem> ExplosionNiagara;
	// //Mesh used to represent the cheese splatter with a Physics Material
	// //for sound
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TObjectPtr<UStaticMeshComponent> JamAreaSplatterMesh;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	float PetrificationDuration;
	float PetrificationDurationAfterWine;
	float PetrificationAreaRadius;
	float PetrificationAreaTime;
	float TimeBeforePetrification;
	float JamSlamMeteorSpeed;
	TSubclassOf<UGameplayEffect> InProcessOfPetrificationEffect;
	TSubclassOf<UGameplayEffect> PetrificationEffect;
	
	//Save all petrification effects applied so that they can be removed on End Overlap
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveWaitPetrificationEffectHandles;
	//Final destination distance of jam slam meteor
	FVector FinalDestination;
	bool ShouldKeepFalling = true;
	//When area dissapears, it turns invisible and no enemies can enter, after 3 seconds, it is destroyed
	//This is done so that the JamSlamArea can execute the gameplay effect after some time
	bool EnemiesCanNoLongerEnter = false;
	//After sometime, activate overlaps
	FTimerHandle TimerHandle;
	FTimerHandle TimerActiveteOverlapsHandle;
	bool CanOverlap = true;
	
	void ApplyPetrifiedEffect(TObjectPtr<UAbilitySystemComponent> TargetASC);
	void CreateSweep();
	
	UFUNCTION()
	void ActivateOverlaps();
	UFUNCTION()
	void EndJamSlamArea();
	
};
