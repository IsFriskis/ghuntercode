// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Actor/Hazards/GuardianWall.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyGroup.h"
#include "CucaferaGroup.generated.h"

UCLASS()
class TINYTAVERNGAME_API ACucaferaGroup : public AEnemyGroup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACucaferaGroup();

	UPROPERTY(EditAnywhere,Category = "TT|CucaferaGroupParameters")
	bool HasGuardian = false;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT|CucaferaGroupParameters",meta=(EditCondition="IsGuardian=true"))
	TArray<AGuardianWall*> GuardianWallsArray;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
