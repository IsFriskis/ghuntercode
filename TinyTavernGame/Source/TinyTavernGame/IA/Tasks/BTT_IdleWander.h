// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTT_IdleWander.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBTT_IdleWander : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTT_IdleWander(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void OnFinishedMove(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Idle Wander Info", meta = (AllowPrivateAccess = "true"))
	float WanderRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Idle Wander Info", meta = (AllowPrivateAccess = "true"))
	float WaitDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Idle Wander Info", meta = (AllowPrivateAccess = "true"))
	bool AlertBreak = true;
	
	TObjectPtr<AEnemyBase> Enemy;
	bool IsWaiting = true;
	float TimeInWait = 0;
	FNavLocation TargetLocation;
	//Generates a New Location inside de Nav Mesh
	void GenerateNewTargetLocation();
	
};
