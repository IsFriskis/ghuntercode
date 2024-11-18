// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyTavernGame/IA/MIscelaneous/Waypoint.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "EnemyGroup.generated.h"

class UTT_GameInstance;
class AEnemyBurrow;
class AEnemyAAIController;
class UBehaviorTreeComponent;
class AEnemyBase;
class AController;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyRespawn,bool)

UCLASS(Abstract)
class TINYTAVERNGAME_API AEnemyGroup : public AActor
{
	GENERATED_BODY()

public:
	AEnemyGroup();
	virtual void Tick(float DeltaTime) override;

	FOnEnemyRespawn OnEnemyRespawn;
	
	UFUNCTION(BlueprintCallable)
	TArray<AEnemyBase*> GetEnemiesInGroup() const { return enemiesInGroup; }
	UFUNCTION(BlueprintCallable)
	void AddEnemyInGroup(AEnemyBase* enemyToAdd) {enemiesInGroup.Push(enemyToAdd);}
	UFUNCTION(BlueprintCallable)
	void DeleteEnemyInGroup(AEnemyBase* enemyToDelete) {enemiesInGroup.Remove(enemyToDelete);}

	TArray<AEnemyBurrow*> GetEnemiesBurrow() const { return enemiesBurrow; }
	void AddEnemyBurrow(AEnemyBurrow* enemyBurrowToAdd) {enemiesBurrow.Push(enemyBurrowToAdd);}
	void DeleteEnemyBurrow(AEnemyBurrow* enemyBurrowToDelete) {enemiesBurrow.Remove(enemyBurrowToDelete);}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true"))
	float spawnRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true"))
	int32 enemiesInGroupCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true", ToolTip = "The distance between each enemy when spawned"))
	float spawnDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true", ToolTip = "Check if you want the enemy group to respawn when it drops below a certain number"))
	bool doesRespawn = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true", ToolTip = "Number of enemies to spawn", EditCondition="DoesRespawn==true"))
	int numToSpawn;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true", ToolTip = "Set in the blackboard a value to wander from the group"))
	bool wanderFromGroup = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true", ToolTip = "Set in the blackboard a value to patrol"))
	bool doesPatrol = false;
	
	void SetWaypointInMap();

	UFUNCTION()
	void ResetEnemies();

	UFUNCTION()
	void RespawnEnemies();

	/* Not working, need to fix it setting the correct values in the editor
	 *
	 * 
	 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params|AI", 
		meta = (AllowPrivateAccess = "true", ToolTip =
			"The behavior tree to use for the enemies (Not choosing the correct one might lead to some crashes or directly not working)"
		));
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params|AI",
		meta = (AllowPrivateAccess = "true", ToolTip =
			"The AI controller to use for the enemies (Not choosing the correct one might lead to some crashes or directly not working)"
		))
	TSubclassOf<AController> EnemyAaiControllerThisOne;
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DebugMode", meta = (AllowPrivateAccess = "true", ToolTip = "Turns on debug spheres and all lines for the group of enemies"))
	bool debugMode;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true", ToolTip = "The type of enemy to spawn"))
	TSubclassOf<AEnemyBase> NormalEnemyType;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn Params", meta = (AllowPrivateAccess = "true", ToolTip = "The type of enemy to spawn"))
	TSubclassOf<AEnemyBase> PremiumEnemyType;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "BurrowsToManage", meta = (AllowPrivateAccess = "true"))
	TMap<AWaypoint*,AEnemyBase*>PatrolWaypointsMap;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "BurrowsToManage", meta = (AllowPrivateAccess = "true"))
	TArray<AWaypoint*>WaypointsArray;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<AEnemyBase>> enemiesInGroup;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	// bool PlayerInZone;
protected:
	
	virtual void BeginPlay() override;
private:

	UPROPERTY(EditAnywhere)
	bool AllowedPremium;
	
	UPROPERTY(EditAnywhere)
	float MaxPercentage;

	UPROPERTY(EditAnywhere)
	float MinPercentage;

	int NormalEnemies;
	int PremiumEnemies;
	
	void SpawnEnemies(int enemies,TSubclassOf<AEnemyBase> EnemyType);
	
	TObjectPtr<UTT_GameInstance> GI;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BurrowsToManage", meta = (AllowPrivateAccess = "true", ToolTip = "Burrows for the group of enemies"))
	TArray<AEnemyBurrow*> enemiesBurrow;
	
	
};
