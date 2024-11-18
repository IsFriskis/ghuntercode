// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBurrow.h"
#include "GamusinoBurrow.generated.h"

UCLASS()
class TINYTAVERNGAME_API AGamusinoBurrow : public AEnemyBurrow
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGamusinoBurrow();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
