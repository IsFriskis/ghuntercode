// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "TallGrass.generated.h"

class UAISenseConfig;
class AEnemyBase;
class APlayerCharacter;
class UBoxComponent;

UCLASS()
class TINYTAVERNGAME_API ATallGrass : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATallGrass();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//***************** VARIABLES TRIGGERBOX*****************//
	TArray<AActor*> OverlappingActors;

	TObjectPtr<AEnemyBase> enemy;
	
	TObjectPtr<APlayerCharacter> player;
	
	UPROPERTY(EditAnywhere, Category="EnemySightParameters")
	float LoseSightRadius;
	
	FAISenseID SightSense;

	UAISenseConfig* SenseConfig;

	UAISenseConfig_Sight* SightConfig;
};
