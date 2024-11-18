#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "EnemyGamusino.generated.h"

UCLASS()
class TINYTAVERNGAME_API AEnemyGamusino : public AEnemyBase
{
public:

private:
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Behaviour", meta = (AllowPrivateAccess = "true"))
	bool DoesDamage;

protected:
	
private:

	
};
