// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "HideGrass.generated.h"

class UAISenseConfig;

UCLASS()
class TINYTAVERNGAME_API AHideGrass : public ATriggerVolume
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHideGrass();

	// Called every frame
	// virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	
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
