#include "EnemyAAIController.h"

#include "EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "Navigation/CrowdFollowingComponent.h"

AEnemyAAIController::AEnemyAAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	PrimaryActorTick.bCanEverTick = true;
	
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
	//Perception System
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	EnemySightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfiguration");

	IsDetectingPlayer = false;
	
	EnemySightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	EnemySightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;
	EnemySightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	
	EnemyPerceptionComponent->ConfigureSense(*EnemySightConfiguration);
	EnemyPerceptionComponent->SetDominantSense(EnemySightConfiguration->GetSenseImplementation());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAAIController::OnTargetPerceptionUpdated);
}

void AEnemyAAIController::BeginPlay()
{
	Super::BeginPlay();
	MaxAlertDetectionTime = CurrentAlertDetectionTime;
	MaxRemoveInvestigationStateTimer = CurrentRemoveInvestigationStateTimer;
	BaseSightRadius = EnemySightConfiguration->SightRadius;
}

void AEnemyAAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//Start the behavior tree of the controller
	
	EnemyOwner = Cast<AEnemyBase>(InPawn);
	
	if(EnemyOwner)
	{
		if(UBehaviorTree* const tree = EnemyOwner->GetBehaviorTree())
		{
			UBlackboardComponent* bb;
			UseBlackboard(tree->BlackboardAsset, bb);
			Blackboard = bb;
			RunBehaviorTree(tree);
		}
	}
}

void AEnemyAAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!EnemyOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Alert) && !EnemyOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_ResetEnemy))
	{
		if(IsDetectingPlayer && CurrentAlertDetectionTime > 0.0f)
		{
			CurrentAlertDetectionTime -= DeltaTime;
		}
		else if(!IsDetectingPlayer && CurrentAlertDetectionTime > 0.0 && CurrentAlertDetectionTime <= MaxAlertDetectionTime)
		{
			CurrentAlertDetectionTime += DeltaTime;
		}
		else if(CurrentAlertDetectionTime <= 0.0f)
		{
			EnemyOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_Alert);
			EnemyOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_IA_Investigating);
		}
		if(EnemyOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_IA_Investigating) && !IsDetectingPlayer)
		{
			CurrentRemoveInvestigationStateTimer -= DeltaTime;
			if(CurrentRemoveInvestigationStateTimer <= 0.0f)
			{
				EnemyOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_IA_Investigating);
			}
		}
		else if(EnemyOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_IA_Investigating) && IsDetectingPlayer)
		{
			CurrentRemoveInvestigationStateTimer = MaxRemoveInvestigationStateTimer;
		}
	}
}

void AEnemyAAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	Actor = CastChecked<APlayerCharacter>(Actor);
	if(Actor && Stimulus.WasSuccessfullySensed() && !EnemyOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_Alert))
	{
		IsDetectingPlayer = true;
		if(!EnemyOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().States_IA_Investigating))
		{
			EnemyOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_IA_Investigating);
		}
	}
	else
	{
		IsDetectingPlayer = false;
	}
}


