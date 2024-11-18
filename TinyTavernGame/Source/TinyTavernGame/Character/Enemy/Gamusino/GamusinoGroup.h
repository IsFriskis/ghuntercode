// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyGroup.h"
#include "GamusinoGroup.generated.h"

UCLASS()
class TINYTAVERNGAME_API AGamusinoGroup : public AEnemyGroup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGamusinoGroup();

	void NotifyIsAttacked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Params", meta = (AllowPrivateAccess = "true"))
	UMaterial* AggroMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
