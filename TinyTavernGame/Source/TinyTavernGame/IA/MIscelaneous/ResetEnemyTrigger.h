// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyGroup.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "ResetEnemyTrigger.generated.h"

class UBoxComponent;
UCLASS()
class TINYTAVERNGAME_API AResetEnemyTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResetEnemyTrigger();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="TT|ResetEnemyTriggerParameters")
	TArray<TObjectPtr<AEnemyGroup>>EnemyGroupsArray;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="TT|ResetEnemyTriggerParameters")
	// TArray<TObjectPtr<UBoxComponent>> ActivationBoxes;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	TObjectPtr<APlayerCharacter>PlayerCharacter;
};
