// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"
#include "BasiliscoIAController.generated.h"

UCLASS()
class TINYTAVERNGAME_API ABasiliscoIAController : public AEnemyAAIController
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ABasiliscoIAController(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
