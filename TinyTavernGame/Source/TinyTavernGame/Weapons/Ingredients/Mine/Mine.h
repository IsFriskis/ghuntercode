// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "TinyTavernGame/Actor/EffectActor.h"
#include "Mine.generated.h"

class USphereComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API AMine : public AEffectActor
{
	GENERATED_BODY()
	
	//A Cheese Mine is made out of two Ingredients, and thus two mines
	//One represents the mine inner radius with the stun effect
	//And the other one, the outer radius with the slow down effect
public:
	AMine();

	bool IsMineTrigger(){return  MineTrigger;}
	void ChangeWasAffectedByWine();
	void ModifySlowDownEffect(bool ShouldChangeSpeed);
	void SetIndicatorMesh(TObjectPtr<UStaticMesh> IndicatorMesh);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MineMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> MineSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> WineVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> MineExtraVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> MineAreaVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> CheeseIndicatorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> WineParticlesInMine;
	//Mesh used to represent the cheese splatter with a Physics Material
	//for sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> CheeseSplatterMesh;
	
	//The Mine counter part to activate it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Mine Inner Rig")
	TSubclassOf<AMine> OuterMineClass;
	//If it is the Trigger Mine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Mine Inner Rig")
	bool MineTrigger = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Mine Inner Rig")
	float LifeSpanOfMine = 10;
	UPROPERTY(EditAnywhere, meta=(ClampMin=0), meta=(ClampMax=1), meta=(UIMin=0), meta=(UIMax=1), Category = "TT| Mine Outer Rig")
	float NormalSlowDownPercentage;
	UPROPERTY(EditAnywhere, meta=(ClampMin=0), meta=(ClampMax=1), meta=(UIMin=0), meta=(UIMax=1), Category = "TT| Mine Outer Rig")
	float AfterWineSlowDownPercentage;
	UPROPERTY(EditAnywhere, Category = "TT| Mine Outer Rig")
	TSubclassOf<UGameplayEffect> DurationSlowDownEffect;
	UPROPERTY(EditAnywhere, meta=(ClampMin=0), meta=(ClampMax=1), meta=(UIMin=0), meta=(UIMax=1), Category = "TT| Mine Outer Rig")
	TSubclassOf<UGameplayEffect> InfiniteSlowDownEffect;


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Tick(float DeltaSeconds) override;
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BLUEPRINTCALLABLE)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	//There is a bug where activating outermine, in enters begin and end overlap instantaneous
	//this variable is to control the overlaps
	int NumberOfOverlaps = 0;
	bool AlreadyTriggerBeginOverlap = false;
	//Outer Mine Rig that spawns
	TObjectPtr<AMine> OuterMine;
	//Slow Down Percentage used in mine outer rig
	float SlowDownPercentage;
	//If Wine collided with mine
	bool WasAffectedByWine = false;
	//Save all infinite slow down effects applied so that they can be removed on End Overlap
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveInfiniteSlowDownEffectHandles;
	
	void ApplySlowDownEffectInfinite(TObjectPtr<UAbilitySystemComponent> TargetASC);
	void ApplySlowDownEffectDuration(TObjectPtr<UAbilitySystemComponent> TargetASC);
	void RemoveInfiniteGameplayEffect(TObjectPtr<UAbilitySystemComponent> TargetASC);
	
};

