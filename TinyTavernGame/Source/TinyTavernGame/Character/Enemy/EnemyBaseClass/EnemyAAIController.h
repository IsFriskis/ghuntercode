#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBase.h"
#include "Perception/AISense_Sight.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyAAIController.generated.h"

class UCrowdFollowingComponent;
class AEnemyBase;
class UIA_EnemyManager;
class UBehaviorTreeComponent;

UCLASS(Abstract)
class  TINYTAVERNGAME_API AEnemyAAIController : public AAIController
{
	GENERATED_BODY()
public:
	
	AEnemyAAIController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	//Notifiers for the behavior tree (Example, one enemy is attacked inside the group)
	//TODO: Add notifiers

	UPROPERTY()
	TObjectPtr<AEnemyBase> EnemyOwner = nullptr;
	
	UPROPERTY()
	TObjectPtr<UCrowdFollowingComponent> CrowdFollowingComponent;

	float BaseSightRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SightParameters")
	float CurrentAlertDetectionTime;

	float MaxAlertDetectionTime;

	bool IsDetectingPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SightParameters")
	float CurrentRemoveInvestigationStateTimer;

	float MaxRemoveInvestigationStateTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
protected:


	UPROPERTY(EditAnywhere, Category = "Perception")
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;
	UPROPERTY(EditAnywhere, Category = "Perception")
	TObjectPtr<UAISenseConfig_Sight> EnemySightConfiguration;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);


};
