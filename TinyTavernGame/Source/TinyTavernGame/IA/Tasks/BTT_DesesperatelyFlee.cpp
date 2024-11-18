#include "BTT_DesesperatelyFlee.h"
#include "AIController.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Interfaces/IHttpResponse.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyAAIController.h"


UBTT_DesesperatelyFlee::UBTT_DesesperatelyFlee(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "DesperatelyFlee";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_DesesperatelyFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyBase = OwnerComp.GetAIOwner()->GetPawn<AEnemyBase>();
	QueryRequest = FEnvQueryRequest(EQSQuery, EnemyBase.Get());
	QueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &UBTT_DesesperatelyFlee::EQSSearchFinished);
	
	if (DesesperatlyFleeMontage && EnemyBase->GetMesh())
	{
		// Obtén la instancia de animación asociada al SkeletalMesh del personaje
		UAnimInstance* AnimInstance = EnemyBase->GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			// Reproduce el AnimMontage
			AnimInstance->Montage_Play(DesesperatlyFleeMontage);
		}
	}
	return  EBTNodeResult::InProgress;
}

void UBTT_DesesperatelyFlee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FVector Origin;
	FVector Extent;
	EnemyBase->GetActorBounds(false,Origin,Extent);
	if (FVector::Dist(BurrowLocation, EnemyBase->GetActorLocation()) < DistanceFromBurrowToChanneling + Extent.Length())
	{
		
		EnemyBase->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_Channeling);
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	}
	
	Timer += DeltaSeconds;
	if(Timer >= ErraticDodgeTimer)
	{
		float Result =  FMath::RandRange(0,100);
		if(Result <= ErraticDodgeProbability)
		{
			QueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &UBTT_DesesperatelyFlee::EQSSearchFinished);
			EnemyBase->GetAbilitySystemComponent()->AddLooseGameplayTag(ErraticDodgeTag);
		
			//EnemyBase->GetAbilitySystemComponent()->HandleGameplayEvent(ErraticDodgeTag,&Payload);
			FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		}
		Timer = 0.0f;
	}
	EnemyBase.Get()->AIController->MoveToLocation(BurrowLocation, -1.0,false,true,true, true, 0, true);
}

void UBTT_DesesperatelyFlee::EQSSearchFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if(Result->IsFinished())
	{
		FVector EQSResult = Result.Get()->GetItemAsLocation(0);
		if(BurrowLocation != EQSResult)
		{
			BurrowLocation = EQSResult;
		}
	}
}