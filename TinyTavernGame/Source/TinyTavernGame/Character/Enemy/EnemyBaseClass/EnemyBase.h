// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TinyTavernGame/Character/BasicCharacter.h"
#include "TinyTavernGame/UI/HUD/OverlayWidgetController.h"
#include "EnemyCorpse.h"
#include "EnemyGroup.h"
#include "Navigation/PathFollowingComponent.h"
#include "MotionWarpingComponent.h"
#include "EntitySystem/MovieSceneOverlappingEntityTracker.h"
#include "EnemyBase.generated.h"

class UNiagaraSystem;
class AEnemyGroup;
class AEnemyAAIController;

UENUM()
enum class EEnemyType : uint8
{
	None UMETA(DisplayName = "None"),
	Gamusino UMETA(DisplayName = "Gamusino"),
	Cucafera UMETA(DisplayName = "Cucafera"),
	Cuelebre UMETA(DisplayName = "Cuelebre"),
	Basilisco UMETA(DisplayName = "Basilisco")
};

/**
 * 
 */
UCLASS(Abstract)
class TINYTAVERNGAME_API AEnemyBase : public ABasicCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyBase();
	~AEnemyBase();
	
	virtual void BeginDestroy() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//IA SETUP
	void SetupIAController(TObjectPtr<AEnemyAAIController> _AIController) { AIController = _AIController; }
	void SetGroup(AEnemyGroup* _group) { group = _group; };
	TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return BehaviorTree; }

	UFUNCTION(BlueprintCallable)
	AEnemyGroup* GetGroup() const { return group; }
	TObjectPtr<class AEnemyAAIController> AIController = nullptr;
	
	//Delegates to pass the health and max health of the enemy to the CookBar UI
	UPROPERTY(BlueprintAssignable)
	FOnCookbarChangeSignature OnCookbarChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxCookbarChangeSignature OnMaxCookbarChanged;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetEnemyTypeExact(){return EnemyType_Tag;}

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetEnemyType() { return EnemyType_Tag.RequestDirectParent(); }

	UFUNCTION(BlueprintCallable)
		void CancelAllEnemyAbilitiesExcept(FGameplayTag AbilityTag);

	//Parameter to change the outline material for the method HighlightEnemy();
	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> HighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
	bool IsPremium = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|QTE")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMotionWarpingComponent* EnemyMotionWarpingComponent;
	//Dive Params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<UAnimMontage> DiveAnim;
	UPROPERTY()
	bool Dive;
	//Parameters to determine which state the enemy is currently
	UPROPERTY(EditAnywhere, Category = "TT|CookbarStatesPercentages", meta=(UIMin=0, UIMax=1))
	float DesesperatelyPercentaje;
	UPROPERTY(EditAnywhere, Category = "TT|CookbarStatesPercentages", meta=(UIMin=0, UIMax=1))
	float FalseFleePercentaje;
	float EnemyDesesperatelyState;
	float EnemyFalseFleeState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|CookbarStatesPercentages")
	float RawPoint;
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = "TT|CookbarStatesPercentages")
	float CookedPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|IA")
	FVector SpawnPosition;
	
	UPROPERTY(EditAnywhere, Category = "TT|CookbarStatesPercentages")
	TSubclassOf<UGameplayEffect> RawGE;
	UPROPERTY(EditAnywhere, Category = "TT|CookbarStatesPercentages")
	TSubclassOf<UGameplayEffect> CookedGE;
	UPROPERTY(EditAnywhere, Category = "TT|CookbarStatesPercentages")
	TSubclassOf<UGameplayEffect> BurntGE;
	//Enemy corpse
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category ="TT|Combat")
	TSubclassOf<AActor> DeathBody;
	//Params for burn Effect
	UPROPERTY(EditAnywhere, Category = "TT|Burn")
	TObjectPtr<UMaterialInterface> BurnMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = "TT|Burn")
	TObjectPtr<UNiagaraSystem> ExplosionEffect = nullptr;
	UPROPERTY(EditAnywhere, Category = "TT|Burn")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;
protected:
	virtual void InitAbilityActorInfo() override;
	//This method is called when the health of the enemy is equal to his max health and can be killed
	void HighlightEnemy();
	//This method is called when the enemy has the TAG(States.Damaged) to show the CookBar
	void HideShowUI();
	
	void CheckCookbar();
	//Initialize a GE that regenerate the enemy life
	// void InitializeCookbarRegeneration();
	//Destroy this enemy
	void EliminateEnemy();
	//Spawn a Corpse
	void SpawnCorpse();

	UFUNCTION()    
	void OnEnemyHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit );
	
	UPROPERTY(EditAnywhere, Category = "TT")
	FGameplayTag EnemyType_Tag;
	
	UPROPERTY(EditAnywhere, Category = "TT|IA")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	//Percentage needed to enter desperately flee (100 = 100%, 1,0 = 1%)
	UPROPERTY(EditAnywhere, Category = "TT|IA")
	float PercentageToDFlee;
private:
	UPROPERTY()
	TObjectPtr<AEnemyGroup> group = nullptr;
	// UPROPERTY(EditAnywhere, Category = "TT|Effects")
	// TSubclassOf<UGameplayEffect> RegenerationGE;
	UPROPERTY(EditAnywhere, Category = "TT|Corpse")
	TSubclassOf<AEnemyCorpse> EnemyCorpse;

	UPROPERTY()
	TObjectPtr<UTT_GameInstance> GI;
	bool UIIsInitialized = false;
};


