// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "GamusinoIAController.generated.h"

UCLASS()
class TINYTAVERNGAME_API AGamusinoIAController : public AEnemyAAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGamusinoIAController(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
