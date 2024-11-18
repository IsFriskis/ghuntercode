#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "TinyTavernGame/Character/Enemy/Gamusino/EnemyGamusino.h"
#include "BTT_DesesperatelyFlee.generated.h"

class AEnemyBurrow;
class AGamusinoBurrow;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_DesesperatelyFlee : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_DesesperatelyFlee(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//Animation for desesperatly flee
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Animation Info")
	TObjectPtr<UAnimMontage> DesesperatlyFleeMontage;
	//Probability of doing erratic dodge(100 = 100%, 0,1 = 0,1%)
	UPROPERTY(EditAnywhere, Category = "TT|Flee Info")
	float ErraticDodgeProbability;
	//How much time can do erratic dodge
	UPROPERTY(EditAnywhere, Category = "TT|Flee Info")
	float ErraticDodgeTimer;
	//Tag that activates the ability
	UPROPERTY(EditAnywhere, Category = "TT|Flee Info")
	FGameplayTag ErraticDodgeTag;
private:
	UPROPERTY(EditAnywhere, Category = "TT|Flee Info")
	UEnvQuery* EQSQuery;

	UPROPERTY()
	TObjectPtr<AEnemyBase> EnemyBase;
	
	FEnvQueryRequest QueryRequest;
	
	UPROPERTY(EditAnywhere, Category = "TT|Flee Info")
	float DistanceFromBurrowToChanneling;
	
	FVector BurrowLocation;
	float Timer = 0.0f;

	void EQSSearchFinished(TSharedPtr<FEnvQueryResult> Result);
};