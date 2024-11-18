// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_GameplayTaskBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "BTT_FalseFlee.generated.h"


/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_FalseFlee : public UBTTask_GameplayTaskBase
{
	GENERATED_BODY()
public:
	UBTT_FalseFlee(FObjectInitializer const& ObjectInitializer);
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	//In basilisk case it should be the distance to the player to start the tail sweep
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	float PlayerDetectionRange;

	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	float AcceptableRadiusReachDestination;

	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	UEnvQuery* EQSQuery;

	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	FGameplayTag BlockFalseFleeTag;

	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	FGameplayTag BlockTailSweepFalseFleeTag;

	TObjectPtr<AEnemyBase> EnemyBase;
	
	FVector FinalFleeLocation;
	
	TObjectPtr<APlayerCharacter> Player;
	
	FEnvQueryRequest QueryRequest;
	
	void EQSSearchFinished(TSharedPtr<FEnvQueryResult> Result);

	bool stopMoving = false;
};
