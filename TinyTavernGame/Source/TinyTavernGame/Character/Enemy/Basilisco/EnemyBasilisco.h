// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "EnemyBasilisco.generated.h"

UCLASS()
class TINYTAVERNGAME_API AEnemyBasilisco : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBasilisco();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
