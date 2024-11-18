// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "BTT_Flee.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_Flee : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	UBTT_Flee(FObjectInitializer const& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	//Distance the player can be detected to know wheter its following this Gamusino
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	float PlayerDetectionRange;

	//Distance that the gamusino will go to before finish flee and entering chase
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	float MaxDistanceToPlayer;
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	UEnvQuery* EQSQuery;

	//Tag to enter the next behaviour
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	FGameplayTag NextStateTag;
	
	//Tag to enter if player follow the enemy 
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	FGameplayTag TimerFleeTag;

	//Time that the enemy can run before getting tired and stop 
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	float StopTimerLimit;

	//Min Time that the enemy is not moving because its tired 
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	float RunTimerMin;

	//Max Time that the enemy is not moving because its tired 
	UPROPERTY(EditAnywhere, Category = "TT| Flee Info")
	float RunTimerMax;
	
	float StopTimer = 0.0f;
	float RunTimer = 0.0f;
	float RunTimerLimit = 0.0f;
	FEnvQueryRequest QueryRequest;
	float QueryTimer = 0.0f;
	bool stopMoving = false;
	TObjectPtr<AEnemyBase> EnemyBase;
	FVector FinalFleeLocation;
	TObjectPtr<APlayerCharacter> Player;
	
	//Function accesed when calling EQS
	void EQSSearchFinished(TSharedPtr<FEnvQueryResult> Result);
	void EndTask(const FAbilityEndedData& Data);
};
