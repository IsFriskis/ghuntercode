// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "EnemyCucafera.generated.h"
//DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGhostDeath, float, DamageAmount, FGameplayTag, Cooktype);
UCLASS()
class TINYTAVERNGAME_API AEnemyCucafera : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCucafera();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT|CucaferaParameters", meta=(EditCondition="IsPremium==true"))
	TArray<AActor*> GhostAlliesArray;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "TT|CucaferaParameters", meta=(EditCondition="IsPremium==true"))
	float TotalGhostsDamage;
};
